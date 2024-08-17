#include <Arduino.h>
#include <WiFi.h>

#include "constants.h"
#include "feedback/feedback.h"
#include "authentication/authentication.h"
#include "lock/lock.h"
#include "ota/ota.h"
#include "rfid/rfid.h"

Feedback feedback;
Authentication auth;
Lock lock(feedback);
Ota ota(feedback);
Rfid rfid(feedback);

unsigned long lastBadAuth = 0UL;

/**
 * When either of the Wiegand pins change, update their state in the Rfid instance
 */
void wiegandPinStateChanged() {
    rfid.updatePinState();
}

/**
 * When the lock button is pressed, update it's state in the Lock instance
 */
void lockButtonPinPressed() {
    lock.lockButtonPressed();
}

void setupWifi() {
    WiFi.mode(WIFI_STA);
    WiFi.begin(Constants::wifiSsid, Constants::wifiPass);
    Serial.print(F("Connecting to WiFi Network:"));
    Serial.println(Constants::wifiSsid);

    while(WiFi.status() != WL_CONNECTED) {
        delay(500);
        Serial.print(".");
    }
    Serial.println();
    Serial.print(F("Successfully connected, Local IP: "));
    Serial.println(WiFi.localIP());

    // Enable auto-reconnecting on WiFi loss
    WiFi.setAutoReconnect(true);
    WiFi.persistent(true);
}

void setup() {
    Serial.begin(115200);

    setupWifi();
    auth.setup();
    lock.setup();
    attachInterrupt(digitalPinToInterrupt(Constants::pinButtonLock), lockButtonPinPressed, FALLING);

    rfid.setup();
    attachInterrupt(digitalPinToInterrupt(Constants::pinD0), wiegandPinStateChanged, CHANGE);
    attachInterrupt(digitalPinToInterrupt(Constants::pinD1), wiegandPinStateChanged, CHANGE);

    ota.setup();
}

void loop() {
    auth.loop();
    rfid.loop();
    lock.loop();
    ota.loop();
    feedback.loop();

    // Check for unlock
    uint32_t nextKey;
    bool hasKey = rfid.getNextKey(&nextKey);
#if ENABLE_KEYPAD_PASSCODE
    uint32_t nextKeycode;
    bool hasPin = rfid.getNextKeyCode(&nextKeycode);
#endif

    unsigned long currentMillis = millis();

    if (currentMillis - lastBadAuth < Constants::badAuthDelay) {
        return;
    }

    // Only handle key if currently locked, otherwise discard
    if (hasKey && lock.isLocked()) {
        if (auth.isAuthorised(nextKey)) {
            Serial.print(F("Read Authorised key: "));
            Serial.println(nextKey, 16);
            lock.unlock();
        } else {
            lastBadAuth = currentMillis;

            Serial.print(F("Read Unauthorised key: "));
            Serial.println(nextKey, 16);

            feedback.unlockLed.blink(Constants::badAuthFlashCount, 100, false);
#if UNLOCK_BUZZ
            feedback.buzzer.blink(5, 100);
#endif
        }
    }
#if ENABLE_KEYPAD_PASSCODE
    if (hasPin && lock.isLocked()) {
        if (nextKeycode == Constants::keycode) {
            Serial.println(F("Keycode Unlock"));
            lock.unlock();
        } else {
            lastBadAuth = currentMillis;

            Serial.print(F("Unauthorised keycode: "));
            Serial.println(nextKeycode, 16);

            feedback.unlockLed.blink(Constants::badAuthFlashCount, 100, false);
#if UNLOCK_BUZZ
            feedback.buzzer.blink(5, 100);
#endif
        }
    }
#endif
}