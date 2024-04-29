#pragma once

/**
 * A class representing an output pin.
 */
class Output {
    /** The pin the output is on */
    int pin;
    /**
     * Invert the pin so "on" is represented by low.
     * <p>
     * When <code>true</code>, "on" sets the pin to LOW,
     * <br>
     * When <code>false</code>, "on" sets the pin to HIGH.
     */
    bool invert;
    /**
     * The current state of the pin.
     * <p>
     * When <code>true</code>, the pin is "on".
     */
    bool state = false;

    // Animation
    /** The interval between animation events */
    unsigned long animationInterval = 100L;
    /** The last millis an animation event occurred */
    unsigned long lastAnimMillis = 0L;
    /** The number of animation events remaining */
    int animationRemaining = 0;
    /** The state the animation should finish on */
    bool animEndState = false;

    /**
     * Clear the currently running animation.
     */
    void clearAnimation();
public:
    /**
     * @param pin The pin this output is on
     * @param invert If <code>true</code>, when the pin is "on", it will output LOW, otherwise "on" will output HIGH
     */
    explicit Output(int pin, bool invert = false);

    /**
     * Animation handling loop
     */
    void loop();

    /**
     * Get whether this output is "on"
     * @return <code>true</code> if "on"
     */
    bool isOn() const;

    /**
     * Get whether this output is "off"
     * @return <code>true</code> if "off"
     */
    bool isOff() const;

    /**
     * Flip the state of the pin.
     * @return The new state of the pin
     */
    bool toggle();

    /**
     * Set the pin to "on".
     */
    void on();

    /**
     * Set the pin to "off".
     */
    void off();

    /**
     * Set the pin to a specific state.
     * @param newState The new state of the pin (<code>true</code> for "on")
     */
    void setState(bool newState);

    /**
     * Blink the output
     * @param count The number of times the output should blink
     * @param interval The interval between blinks
     * @param endState The state the blink should end on (If <code>true</code>, then it will end with the output set to
     *                 "on"
     */
    void blink(int count = 1, unsigned long interval = 200, bool endState = false);
};
