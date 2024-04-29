#pragma once

#include "output.h"

#include <constants.h>

/**
 * A struct containing the output devices
 */
struct Feedback {
    /** The LED lit when the device is unlocked */
    Output unlockLed = Output(Constants::pinLedUnlock, true);
    /** The LED lit when the device is locked */
    Output lockLed = Output(Constants::pinLedLock, true);
    /** A buzzer */
    Output buzzer = Output(Constants::pinBuzzer, true);

    /**
     * The loop driving output animations
     */
    void loop();
};
