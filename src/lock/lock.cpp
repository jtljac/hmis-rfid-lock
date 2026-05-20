#include <Arduino.h>

#include "lock.h"
#include "constants.h"

void Lock::setup() {
    lock();
}

void Lock::loop() {
    // Handle
    if (isLocked() ? Constants::buttonUnlock : Constants::buttonLock) {
        shouldSwitch = button.readState() && button.getStateDuration() > Constants::buttonHoldTime;
    } else {
        Serial.println("No button pressed");
    }

    if (shouldSwitch) {
        shouldSwitch = false;
        if (isLocked()) unlock();
        else lock();
        // Wait for button release
        while (button.readState()) {
            delay(100);
        }
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

#if !UNLOCK_BUZZ
    feedback.buzzer.on();
    delay(Constants::lockBeepTime);
#endif
    // Turn off whether unlock buzz is true of false
    feedback.buzzer.off();
    Serial.println("Lock locked");
}

void Lock::unlock() {
    lastUnlockMillis = millis();

    relay.off();
    feedback.unlockLed.on();
    feedback.lockLed.off();

    Serial.println("Lock unlocked");

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
