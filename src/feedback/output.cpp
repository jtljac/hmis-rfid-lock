//
// Created by jacob on 28/04/24.
//

#include "output.h"

#include <Arduino.h>

Output::Output(int pin, bool invert) : pin(pin), invert(invert) {
    pinMode(pin, OUTPUT);
    off();
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
