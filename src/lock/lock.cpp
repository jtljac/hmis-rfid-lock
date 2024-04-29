#include <Arduino.h>

#include "lock.h"
#include "constants.h"

void Lock::setup() {
    pinMode(Constants::PIN_BUT_LOCK, INPUT);
    pinMode(Constants::PIN_REL, OUTPUT);

    lock();
}

void Lock::loop() {
    if (shouldSwitch) {
        shouldSwitch = false;
        if (state) unlock();
        else lock();
    }
#if UNLOCK_DELAY != 0
    if (!state && millis() - lastUnlockMillis >= Constants::unlockDelay) {
        lock();
    }
#endif
}

void Lock::lock() {
    state = true;

    digitalWrite(Constants::PIN_REL, HIGH);
    feedback.unlockLed.off();
    feedback.lockLed.on();
    feedback.buzzer.off();
}

void Lock::unlock() {
    state = false;
    lastUnlockMillis = millis();

    digitalWrite(Constants::PIN_REL, LOW);
    feedback.unlockLed.on();
    feedback.lockLed.off();

    // Only buzzer if enabled
#if UNLOCK_BUZZ
    feedback.buzzer.on();
#endif
}

bool Lock::isLocked() const {
    return state;
}

void Lock::lockButtonPressed() {
    if (state) return;

    unsigned long currentMillis = millis();
    if (currentMillis - lastLockButtonMillis > 200) {
        shouldSwitch = true;
    }
    lastLockButtonMillis = currentMillis;
}
