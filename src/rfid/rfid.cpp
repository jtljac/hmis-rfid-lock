//
// Created by jacob on 28/04/24.
//

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
    Serial.println(plugged ? "CONNECTED" : "DISCONNECTED");
}

void Rfid::receivedData(const uint8_t* data, uint8_t bits) {
    if (bits == 32) {
        hasKey = true;
        nextKey = packRfidBytes(data);
    }
}

void Rfid::receivedError(Wiegand::DataError error, const uint8_t* data, uint8_t bits) {
    Serial.print(Wiegand::DataErrorStr(error));
    Serial.print(" - Raw data: ");
    Serial.print(bits);
    Serial.print("bits / ");

    //Print value in HEX
    uint8_t bytes = (bits+7)/8;
    for (int i=0; i<bytes; i++) {
        Serial.print(data[i] >> 4, 16);
        Serial.print(data[i] & 0xF, 16);
    }
    Serial.println();
}

void Rfid::setup() {
    //Install listeners and initialize Wiegand reader
    wiegand.onStateChange(stateChangedStatic, this);
    wiegand.onReceive(receivedDataStatic, this);
    wiegand.onReceiveError(receivedDataErrorStatic, this);
    wiegand.begin(Wiegand::LENGTH_ANY, true);

    //initialize pins as INPUT and attaches interruptions
    pinMode(Constants::PIN_D0, INPUT);
    pinMode(Constants::PIN_D1, INPUT);

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

uint32_t Rfid::packRfidBytes(const uint8_t* bytes) {
    uint32_t key = 0;
    for (int i = 0; i < 4; ++i) {
        key |= bytes[i] << (i * 8);
    }
    return key;
}

void Rfid::updatePinState() {
    wiegand.setPin0State(digitalRead(Constants::PIN_D0));
    wiegand.setPin1State(digitalRead(Constants::PIN_D1));
}
