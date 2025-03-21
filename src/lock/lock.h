#pragma once

#include "feedback/feedback.h"

/**
 * A class wrapping the functionality of the actual Lock relay and it's indicators
 */
class Lock {
    /** A reference to the program's feedback class */
    Feedback& feedback;

    /**
     * An output representing the relay that is the locking mechanism
     */
    Output relay = Output(Constants::pinRelay, Constants::invertRelay);

    /**
     * Value to signify that the Lock should change.
     * <p>
     * This is used as so the Lock can be changed via an interrupt without needing to do too much work in said interrupt
     */
    volatile bool shouldSwitch = false;

    /**
     * The time in millis that the Lock was unlocked
     * <p>
     * This is used to track when the Lock was unlocked for automatic re-locking
     */
    unsigned long lastUnlockMillis = 0UL;

    /**
     * The time in millis that the Lock button was last pressed
     * <p>
     * This is used for debouncing the Lock button interrupt
     */
    volatile unsigned long lastLockButtonMillis = 0UL;
public:
    explicit Lock(Feedback& feedback) : feedback(feedback) {}

    /**
     * Set up the Lock functionality:
     * - Setup input & Output pins
     * - Default state to locked
     */
    void setup();

    /**
     * Loop for Lock functionality:
     * - Check if an interrupt has requested the state be inverted
     * - Check if the Lock timeout has exceeded
     */
    void loop();

    /**
     * Lock the device
     */
    void lock();

    /**
     * Unlock the device
     */
    void unlock();

    /**
     * Get if the lock is currently locked
     * @return True if locked
     */
    bool isLocked() const;

    /**
     * Get if the lock is currently unlocked
     * @return True if unlocked
     */
    bool isUnlocked() const;

    /**
     * Handler for interrupt when the Lock button has been pressed
     */
    void buttonPressed();
};
