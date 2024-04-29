#pragma once

#include "output.h"

#include <constants.h>

/**
 * A struct containing the output devices
 */
struct Feedback {
    /** The LED lit when the device is unlocked */
    Output unlockLed = Output(Constants::PIN_LED_UNLOCK, true);
    /** The LED lit when the device is locked */
    Output lockLed = Output(Constants::PIN_LED_LOCK, true);
    /** A buzzer */
    Output buzzer = Output(Constants::PIN_BUZ, true);

    /**
     * The loop driving output animations
     */
    void loop();
};
