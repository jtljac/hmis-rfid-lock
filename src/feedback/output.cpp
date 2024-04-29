//
// Created by jacob on 28/04/24.
//

#include "output.h"

#include <Arduino.h>

Output::Output(int pin, bool invert) : pin(pin), invert(invert) {
    pinMode(pin, OUTPUT);
    off();
}

void Output::loop() {
    if (animationRemaining != 0) {
        unsigned long currentMillis = millis();
        if (currentMillis - lastAnimMillis >= animationInterval) {
            lastAnimMillis = currentMillis;

            // Only decrement when we change to the end state, as so the last action of the animation is always to
            // switch to the end state.
            if (toggle() == animEndState && animationRemaining > 0) --animationRemaining;
        }
    }
}

bool Output::toggle() {
    if (state) off();
    else on();

    return state;
}

void Output::on() {
    state = true;

    if (invert) digitalWrite(pin, LOW);
    else digitalWrite(pin, HIGH);
}

void Output::off() {
    state = false;

    if (invert) digitalWrite(pin, HIGH);
    else digitalWrite(pin, LOW);
}

void Output::setState(bool newState) {
    if (newState) on();
    else off();
}

void Output::blink(int count, unsigned long interval, bool endState) {
    animationInterval = interval;
    animationRemaining = count;
    lastAnimMillis = millis();
    setState(!endState);
}
