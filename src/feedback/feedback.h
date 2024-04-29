#pragma once

#include "output.h"

#include <constants.h>

/**
 * A struct containing the output devices
 */
struct Feedback {
    /** The LED lit when the device is unlocked */
    Output unlockLed = Output(Constants::pinLedUnlock, Constants::invertLedUnlock);
    /** The LED lit when the device is locked */
    Output lockLed = Output(Constants::pinLedLock, Constants::invertLedLock);
    /** A buzzer */
    Output buzzer = Output(Constants::pinBuzzer, Constants::invertBuzzer);

    /**
     * The loop driving output animations
     */
    void loop();
};
