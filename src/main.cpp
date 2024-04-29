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

// When any of the pins have changed, update the state of the wiegand library
void wiegandPinStateChanged() {
    rfid.updatePinState();
}

void lockButtonPinPressed() {
    lock.lockButtonPressed();
}

// Initialize Wiegand reader
void setup() {
    Serial.begin(115200);

    internet.setup();
    lock.setup();
    attachInterrupt(digitalPinToInterrupt(Constants::PIN_BUT_LOCK), lockButtonPinPressed, FALLING);

    rfid.setup();
    attachInterrupt(digitalPinToInterrupt(Constants::PIN_D0), wiegandPinStateChanged, CHANGE);
    attachInterrupt(digitalPinToInterrupt(Constants::PIN_D1), wiegandPinStateChanged, CHANGE);
}

// Every few milliseconds, check for pending messages on the wiegand reader
// This executes with interruptions disabled, since the Wiegand library is not thread-safe
void loop() {
    internet.loop();
    rfid.loop();
    lock.loop();
    feedback.loop();

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