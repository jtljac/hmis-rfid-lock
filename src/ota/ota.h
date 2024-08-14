//
// Created by jacob on 14/08/24.
//

#pragma once

#include "../feedback/feedback.h"

class Ota {
    Feedback& feedback;
public:
    Ota(Feedback& feedback) : feedback(feedback) {}

    /**
    * Setup for Over The Air updates:
    * <ul>
    *     <li>Setup ArduinoOTA</li>
    * </ul>
    */
    void setup();

    /**
     * Loop for Over The Air updates:
     * <ul>
     *     <li>Poll for OTA</li>
     * </ul>
     */
    void loop();
};
