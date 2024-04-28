#pragma once

#include <Wiegand.h>

#include "feedback/feedback.h"

class Rfid {
    Feedback& feedback;
    Wiegand wiegand;

    uint32_t nextKey = 0;
    bool hasKey = false;

    /** The last time a wiegand flush occurred */
    unsigned long lastFlush = 0UL;
public:
    explicit Rfid(Feedback& feedback);

    void setup();
    void loop();

    bool getNextKey(uint32_t* key);

    void updatePinState();

    void stateChange(bool plugged);
    void receivedData(const uint8_t* data, uint8_t bits);
    void receivedError(Wiegand::DataError error, const uint8_t* data, uint8_t bits);
};
