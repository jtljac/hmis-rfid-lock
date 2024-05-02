#include <Arduino.h>

#include "constants.h"
#include "feedback/feedback.h"
#include "internet/internet.h"
#include "lock/lock.h"
#include "rfid/rfid.h"

Feedback feedback;
Internet internet;
Lock lock(feedback);
Rfid rfid(feedback);

/**
 * When either of the Wiegand pins change, update their state in the Rfid instance
 */
void wiegandPinStateChanged() {
    rfid.updatePinState();
}

/**
 * When the lock button is pressed, update it's state in the Lock instance
 */
void lockButtonPinPressed() {
    lock.lockButtonPressed();
}

void setup() {
    Serial.begin(115200);

    internet.setup();
    lock.setup();
    attachInterrupt(digitalPinToInterrupt(Constants::pinButtonLock), lockButtonPinPressed, FALLING);

    rfid.setup();
    attachInterrupt(digitalPinToInterrupt(Constants::pinD0), wiegandPinStateChanged, CHANGE);
    attachInterrupt(digitalPinToInterrupt(Constants::pinD1), wiegandPinStateChanged, CHANGE);
}

void loop() {
    internet.loop();
    rfid.loop();
    lock.loop();
    feedback.loop();

    // Check for unlock
    uint32_t nextKey;
    bool hasKey = rfid.getNextKey(&nextKey);
#if ENABLE_KEYPAD_PASSCODE
    uint32_t nextKeycode;
    bool hasPin = rfid.getNextKeyCode(&nextKeycode);
#endif

    // Only handle key if currently locked, otherwise discard
    if (hasKey && lock.isLocked()) {
        if (internet.isAuthorised(nextKey)) {
            Serial.print(F("Read Authorised key: "));
            Serial.println(nextKey, 16);
            lock.unlock();
        } else {
            Serial.print(F("Read Unauthorised key: "));
            Serial.println(nextKey, 16);
            feedback.unlockLed.blink(5, 100, false);
#if UNLOCK_BUZZ
            feedback.buzzer.blink(5, 100);
#endif
        }
    }
#if ENABLE_KEYPAD_PASSCODE
    if (hasPin && lock.isLocked()) {
        if (nextKeycode == Constants::keycode) {
            Serial.println(F("Keycode Unlock"));
            lock.unlock();
        } else {
            Serial.print(F("Unauthorised keycode: "));
            Serial.println(nextKeycode, 16);
            feedback.unlockLed.blink(5, 100, false);
#if UNLOCK_BUZZ
            feedback.buzzer.blink(5, 100);
#endif
        }
    }
#endif
}