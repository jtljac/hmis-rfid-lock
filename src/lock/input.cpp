#include "input.h"

#include "HardwareSerial.h"

Input::Input(const int pin, const int activeState): pin(pin), activeState(activeState) {
    pinMode(pin, INPUT_PULLUP);
    state = digitalRead(pin) == activeState;
    tempState = state;
}

bool Input::readState() {
    const unsigned long currentMillis = millis();
    const bool newState = digitalRead(pin) == activeState;

    if (newState != tempState) {
        debounceMillis = currentMillis;
        tempState = newState;
    } else if (state != tempState && currentMillis - debounceMillis > 50) {
        state = newState;
        lastChangeMillis = currentMillis;
    }

    return state;
}

unsigned long Input::getStateDuration() const {
    return millis() - lastChangeMillis;
}
