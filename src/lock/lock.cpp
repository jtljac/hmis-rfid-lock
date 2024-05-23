#include <Arduino.h>

#include "lock.h"
#include "constants.h"

void Lock::setup() {
    pinMode(Constants::pinButtonLock, INPUT_PULLUP);

    lock();
}

void Lock::loop() {
    if (shouldSwitch) {
        shouldSwitch = false;
        if (isLocked()) unlock();
        else lock();
    }
#if UNLOCK_DELAY != 0
    if (isUnlocked() && millis() - lastUnlockMillis >= Constants::unlockDelay) {
        lock();
    }
#endif
}

void Lock::lock() {
    relay.on();
    feedback.unlockLed.off();
    feedback.lockLed.on();
    feedback.buzzer.off();
}

void Lock::unlock() {
    lastUnlockMillis = millis();

    relay.off();
    feedback.unlockLed.on();
    feedback.lockLed.off();

    // Only buzzer if enabled
#if UNLOCK_BUZZ
    feedback.buzzer.on();
#endif
}

bool Lock::isLocked() const {
    return relay.isOn();
}

bool Lock::isUnlocked() const {
    return relay.isOff();
}

void Lock::lockButtonPressed() {
    if (isLocked()) return;

    unsigned long currentMillis = millis();
    if (currentMillis - lastLockButtonMillis > 200) {
        shouldSwitch = true;
    }
    lastLockButtonMillis = currentMillis;
}
