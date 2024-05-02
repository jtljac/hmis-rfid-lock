#include "rfid.h"

#include <Arduino.h>

Rfid::Rfid(Feedback& feedback) : feedback(feedback) {}

// Notifies when a reader has been connected or disconnected.
// Instead of a message, the seconds parameter can be anything you want -- Whatever you specify on `wiegand.onStateChange()`
void stateChangedStatic(bool plugged, Rfid* rfid) {
    rfid->stateChange(plugged);
}

// Notifies when a card was read.
// Instead of a message, the seconds parameter can be anything you want -- Whatever you specify on `wiegand.onReceive()`
void receivedDataStatic(uint8_t* data, uint8_t bits, Rfid* rfid) {
    rfid->receivedData(data, bits);
}

// Notifies when an invalid transmission is detected
void receivedDataErrorStatic(Wiegand::DataError error, uint8_t* rawData, uint8_t rawBits, Rfid* rfid) {
    rfid->receivedError(error, rawData, rawBits);
}

void Rfid::stateChange(bool plugged) {
    // Do nothing for now
}

void Rfid::receivedData(const uint8_t* data, uint8_t bits) {
#if ENABLE_KEYPAD_PASSCODE
    if (bits == 4) {
        if (*data == 0x0A) {
            // This is '*', clear
            hasKeycode = false;
            nextKeycode = 0xFFFFFFFF;
        } else if (*data == 0x0B) {
            // This is '#', send it through
            hasKeycode = true;
        } else {
            nextKeycode = Constants::keycodeClearMask | nextKeycode << 4 | *data;
        }

        return;
    }
#endif
    if (bits == 32) {
        hasKey = true;
        nextKey = packRfidBytes(data);
    }
}

void Rfid::receivedError(Wiegand::DataError error, const uint8_t* data, uint8_t bits) {
}

void Rfid::setup() {
    //Install listeners and initialize Wiegand reader
    wiegand.onStateChange(stateChangedStatic, this);
    wiegand.onReceive(receivedDataStatic, this);
    wiegand.onReceiveError(receivedDataErrorStatic, this);
    wiegand.begin(Wiegand::LENGTH_ANY, true);

    //initialize pins as INPUT and attaches interruptions
    pinMode(Constants::pinD0, INPUT);
    pinMode(Constants::pinD1, INPUT);

    //Sends the initial pin state to the Wiegand library
    updatePinState();
}

void Rfid::loop() {
    unsigned long currentMillis = millis();
    if (currentMillis - lastFlush >= 100) {
        lastFlush = millis();

        noInterrupts();
        wiegand.flush();
        interrupts();
    }
}

bool Rfid::getNextKey(uint32_t* key) {
    if (hasKey) {
        *key = nextKey;
        nextKey = 0;
        hasKey = false;
        return true;
    }
    return false;
}
#if ENABLE_KEYPAD_PASSCODE
bool Rfid::getNextKeyCode(uint32_t* key) {
    if (hasKeycode) {
        *key = nextKeycode;
        nextKeycode = 0xFFFFFFFF;
        hasKeycode = false;
        return true;
    }
    return false;
}
#endif

uint32_t Rfid::packRfidBytes(const uint8_t* bytes) {
    uint32_t key = 0;
    for (int i = 0; i < 4; ++i) {
        key |= bytes[i] << (i * 8);
    }
    return key;
}

void Rfid::updatePinState() {
    wiegand.setPin0State(digitalRead(Constants::pinD0));
    wiegand.setPin1State(digitalRead(Constants::pinD1));
}
