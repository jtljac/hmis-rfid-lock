#pragma once

#include "../constants.h"
#include "output.h"


struct Feedback {
    Output unlockLed = Output(Constants::PIN_LED_UNLOCK, true);
    Output lockLed = Output(Constants::PIN_LED_UNLOCK, true);
    Output buzzer = Output(Constants::PIN_LED_UNLOCK, true);

    void loop();
};
