#pragma once

class Output {
    int pin;
    bool invert;
    bool state = false;
public:
    explicit Output(int pin, bool invert = false);

    bool toggle();
    void on();
    void off();
    void setState(bool state);


};
