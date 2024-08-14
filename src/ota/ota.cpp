//
// Created by jacob on 14/08/24.
//

#include "ota.h"

#include <ArduinoOTA.h>
#include <constants.h>

void Ota::setup() {
    if (Constants::otaHostname.length() > 0) {
      ArduinoOTA.setHostname(Constants::otaHostname.c_str());
    }
    if (Constants::otaPassword.length() > 0) {
      ArduinoOTA.setPassword(Constants::otaPassword.c_str());
    }

    ArduinoOTA
        .onStart([this] {
            String type;
            if (ArduinoOTA.getCommand() == U_FLASH)
              type = "sketch";
            else // U_SPIFFS
              type = "filesystem";

            // NOTE: if updating SPIFFS this would be the place to unmount SPIFFS using SPIFFS.end()
            Serial.println("Start updating " + type);

            // Indicate we're uploading, as it's unresponsive otherwise, switch off buzzer just in case
            feedback.unlockLed.on();
            feedback.lockLed.on();
            feedback.buzzer.off();
        })
        .onEnd([] {
            Serial.println("\nEnd");
        })
        .onProgress([](const unsigned int progress, const unsigned int total) {
            Serial.printf("Progress: %u%%\r", (progress / (total / 100)));
        })
        .onError([](const ota_error_t error) {
            Serial.printf("Error[%u]: ", error);
            if (error == OTA_AUTH_ERROR) Serial.println("Auth Failed");
            else if (error == OTA_BEGIN_ERROR) Serial.println("Begin Failed");
            else if (error == OTA_CONNECT_ERROR) Serial.println("Connect Failed");
            else if (error == OTA_RECEIVE_ERROR) Serial.println("Receive Failed");
            else if (error == OTA_END_ERROR) Serial.println("End Failed");
        });

    ArduinoOTA.begin();
}

void Ota::loop() {
    ArduinoOTA.handle();
}