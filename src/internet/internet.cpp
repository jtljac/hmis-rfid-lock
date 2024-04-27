#include "internet.h"

#include <ArduinoJson.h>
#include <HTTPClient.h>
#include <StreamUtils.h>
#include <WiFi.h>

void Internet::setup() {
    WiFi.mode(WIFI_STA);
    WiFi.begin(Constants::wifiSsid, Constants::wifiPass);
    while(WiFi.status() != WL_CONNECTED) {
        delay(500);
        Serial.print(".");
    }

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

void Internet::updateAuthCache() {
    if (WiFi.status() == WL_CONNECTED) {
        Serial.println(F("No WiFi, skipping cache update"));
        return;
    }

    HTTPClient http;
    http.begin(Constants::hmisLockUrl);
    http.setAuthorizationType("Token");
    http.setAuthorization(Constants::hmisToken);

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
