#pragma once

class Input {
    /** The pin the input is on */
    int pin;

    /**
     * The state of the arduino pin that is "on"
     * <p>
     * Must be HIGH or LOW
     */
    int activeState;

    /**
     * The most upto date state of the pin, used for debouncing.
     * <p>
     * When <code>true</code>, the input is "on".
     */
    bool tempState;

    /**
     * The current state of the pin.
     * <p>
     * When <code>true</code>, the input is "on".
     */
    bool state;

    /**
     * The timestamp in millis of the last change
     */
    unsigned long lastChangeMillis = 0UL;

    /**
     * The timestamp in millis of the read, used for debouncing
     */
    unsigned long debounceMillis = 0UL;


public:
    Input(int pin, int activeState);

    /**
     * Read the current state of the input, asynchronously debounces by not updating the state unless the read value has
     * been the same for more than 50ms
     *
     * @return True when the input is toggled on
     */
    bool readState();

    /**
     * Get the amount of time in milliseconds the input has held the current state
     *
     * @return How long the input has been in the current state
     */
    unsigned long getStateDuration() const;
};
