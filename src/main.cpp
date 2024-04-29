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
    // Only handle key if currently locked, otherwise discard
    if (rfid.getNextKey(&nextKey) && lock.isLocked()) {
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
}