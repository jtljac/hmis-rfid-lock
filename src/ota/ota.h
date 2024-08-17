//
// Created by jacob on 14/08/24.
//

#pragma once

#include "../feedback/feedback.h"

/**
 * A class for handling Over the Air updates
 *
 * This is its own class, despite most of its functionality being static, mainly for the purpose of encapsulation
 */
class Ota {
    /** A reference to the program's feedback class */
    Feedback& feedback;
public:
    Ota(Feedback& feedback) : feedback(feedback) {}

    /**
    * Setup for Over The Air updates:
    * <ul>
    * - Setup ArduinoOTA
    * </ul>
    */
    void setup();

    /**
     * Loop for Over The Air updates:
     * - Poll for OTA
     */
    void loop();
};
