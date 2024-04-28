#include "internet.h"

#include <ArduinoJson.h>
#include <HTTPClient.h>
#include <StreamUtils.h>
#include <WiFi.h>

void Internet::setup() {
    WiFi.mode(WIFI_STA);
    WiFi.begin(Constants::wifiSsid, Constants::wifiPass);
    Serial.print(F("Connecting to WiFi Network:"));
    Serial.println(Constants::wifiSsid);

    while(WiFi.status() != WL_CONNECTED) {
        delay(500);
        Serial.print(".");
    }

    Serial.print(F("Successfully connected, Local IP: "));
    Serial.println(WiFi.localIP());

    // Enable auto-reconnecting on WiFi loss
    WiFi.setAutoReconnect(true);
    WiFi.persistent(true);

    updateAuthCache();
}

void Internet::loop() {
    if (millis() - lastCacheUpdateMillis >= Constants::cacheUpdateInterval) {
        lastCacheUpdateMillis += Constants::cacheUpdateInterval;
        updateAuthCache();
    }
}

bool Internet::isAuthorised(uint32_t rfid) {
    uint32_t* currentRfid = authCache;
    for (int i = 0; i < rfidCount; ++i, ++currentRfid) {
        if (rfid == *currentRfid) return true;
    }
    return false;
}

void Internet::updateAuthCache() {
    // Much functionality of this was borrowed from https://randomnerdtutorials.com/esp32-http-get-post-arduino/#http-get-1
    if (WiFi.status() != WL_CONNECTED) {
        Serial.println(F("No WiFi, skipping cache update"));
        return;
    }

    HTTPClient http;
    http.begin(Constants::hmisLockUrl);
    http.setAuthorizationType("Token");
    http.setAuthorization(Constants::hmisToken);

    Serial.print("Getting from ");
    Serial.println(Constants::hmisLockUrl);
    int response = http.GET();

    if (response != 200) {
        Serial.print(F("Invalid HTTP response code: "));
        Serial.println(response);
        return;
    }

    Stream& payload = http.getStream();
    char buffer[8];
    payload.readBytes(buffer, 1);
    if (buffer[0] != '[') {
        Serial.println(F("Unexpected response from cache list update"));
        return;
    }

    clearAuthCache();

    // This is going to take a bunch of memory, we're gonna try to process the stream in as small steps as possible
    do {
        payload.find("{\"rfid\":\"");
        payload.readBytes(buffer, 8);
        authCache[rfidCount++] = readRfidString(buffer);
        
        Serial.print("RFID Read: ");
        Serial.write(buffer, 8);
        Serial.println();

        // If we fill up the cache then we need to back out
        if (rfidCount == Constants::maxCachedRfids) {
            Serial.println(F("Cache is full"));
            break;
        }
    } while (payload.findUntil(",","]"));

}

void Internet::clearAuthCache() {
    // We only need to clear up as many as we have stored
    for (int i = 0; i < rfidCount; i++) authCache[i] = 0;

    rfidCount = 0;
}

// Implementation adapted from https://stackoverflow.com/a/75882454
uint32_t Internet::readRfidString(char* rfidString) {
    uint32_t rfidInt = 0;

    for (int i = 0; i < 8; ++i) {
        char hexChar = *rfidString++;
        rfidInt = (rfidInt << 4) | (hexChar % 16 + 9 * (rfidInt >> 6));
    }
    return rfidInt;
}
