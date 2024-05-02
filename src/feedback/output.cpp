#include "output.h"

#include <Arduino.h>

void Output::clearAnimation() {
    animationRemaining = 0;
}

Output::Output(int pin, bool invert) : pin(pin), invert(invert) {
    pinMode(pin, OUTPUT);
    off();
}

void Output::loop() {
    if (animationRemaining != 0) {
        unsigned long currentMillis = millis();
        if (currentMillis - lastAnimMillis >= animationInterval) {
            lastAnimMillis = currentMillis;

            // We gotta do this weird pass around to avoid the toggle resetting the animation remaining
            int tempRemaining = animationRemaining;
            toggle();
            animationRemaining = tempRemaining;

            if (state == animEndState && animationRemaining > 0) {
                // Only decrement when we change to the end state, as so the last action of the animation is always to
                // switch to the end state.
                --animationRemaining;
            }
        }
    }
}

bool Output::isOn() const {
    return state;
}

bool Output::isOff() const {
    return !state;
}

bool Output::toggle() {
    if (state) off();
    else on();

    return state;
}

void Output::on() {
    clearAnimation();
    state = true;

    if (invert) digitalWrite(pin, LOW);
    else digitalWrite(pin, HIGH);
}

void Output::off() {
    clearAnimation();
    state = false;

    if (invert) digitalWrite(pin, HIGH);
    else digitalWrite(pin, LOW);
}

void Output::setState(bool newState) {
    if (newState) on();
    else off();
}

void Output::blink(int count, unsigned long interval, bool endState) {
    // Set first as the act of setting clears animationRemaining
    setState(!endState);

    animationInterval = interval;
    animationRemaining = count;
    lastAnimMillis = millis();
}
