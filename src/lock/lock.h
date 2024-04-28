#pragma once

#include "feedback/feedback.h"

/**
 * A class wrapping the functionality of the actual Lock relay and it's indicators
 */
class Lock {
    Feedback& feedback;

    /** The current state of the Lock, <code>true</code> represents the Lock being open */
    bool state = false;

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
     * This is used for deboucing the Lock button interrupt
     */
    volatile unsigned long lastLockButtonMillis = 0UL;
public:
    explicit Lock(Feedback& feedback) : feedback(feedback) {}

    /**
     * Setup the Lock functionality:
     * <ul>
     *     <li>Setup input & Output pins</li>
     *     <li>Default state to locked</li>
     * </ul>
     */
    void setup();

    /**
     * Loop for Lock functionality:
     * <ul>
     *     <li>Check if an interrupt has requested the state be inverted</li>
     *     <li>Check if the Lock timeout has exceeded</li>
     * </ul>
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
     * Handler for interrupt when the Lock button has been pressed
     */
    void lockButtonPressed();
};
