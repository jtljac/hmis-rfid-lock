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

    if (!lock.isLocked()) return;
    
    uint32_t nextKey;
    if (rfid.getNextKey(&nextKey)) {
        // TODO: Validate key, unlock if necessary
    }
}