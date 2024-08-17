#include "authentication.h"

#include <HTTPClient.h>

void Authentication::setup() {
    updateAuthCache();

    for (uint32_t auth : authCache) {
        Serial.println(auth, 16);
    }
}

void Authentication::loop() {
    if (millis() - lastCacheUpdateMillis >= Constants::cacheUpdateInterval) {
        lastCacheUpdateMillis += Constants::cacheUpdateInterval;
        updateAuthCache();
    }
}

bool Authentication::isAuthorised(uint32_t rfid) {
#if LIVE_CHECK
    return checkAuthLive(rfid);
#else
    return checkAuthCache(rfid);
#endif
}

bool Authentication::checkAuthLive(uint32_t keyCode) {
    if (WiFi.status() != WL_CONNECTED) {
        Serial.println(F("No WiFi, falling back to cache"));
        return checkAuthCache(keyCode);
    }

    HTTPClient http;
    char stringKeycode[9];
    sprintf(stringKeycode, "%08x", keyCode);
    const String url = Constants::hmisCheckUrl + stringKeycode + "/";
    http.begin(url);
    http.setAuthorizationType("Token");
    http.setAuthorization(Constants::hmisToken);

    Serial.print("Getting from ");
    Serial.println(url);
    const int response = http.GET();

    if (response != HTTP_CODE_OK) {
        Serial.print(F("Invalid response from HMIS: "));
        Serial.println(response);
        Serial.println(F("Falling back to cache"));
        http.end();
        return checkAuthCache(keyCode);
    }

    // Only check first 4 characters to limit memory consumed if something goes wrong
    Stream& payload = http.getStream();
    char buffer[4];
    payload.readBytes(buffer, 4);

    http.end();
    return strncmp(buffer, "true", 4) == 0;
}

bool Authentication::checkAuthCache(uint32_t keyCode) {
    for (int i = 0; i < rfidCount; ++i) {
        if (keyCode == authCache[i]) return true;
    }

    return false;
}

void Authentication::updateAuthCache() {
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

    if (response != HTTP_CODE_OK) {
        Serial.print(F("Invalid HTTP response code: "));
        Serial.println(response);
        http.end();
        return;
    }

    Stream& payload = http.getStream();
    char buffer[8];
    payload.readBytes(buffer, 1);
    if (buffer[0] != '[') {
        Serial.println(F("Unexpected response from cache list update"));
        http.end();
        return;
    }

    clearAuthCache();

    // As the json array of the keys are well-defined, we're gonna skip json processing all together, and pull the
    // values out directly.
    // This is going to take a bunch of memory, so we're gonna try to process the stream in as small steps as possible.
    do {
        payload.find(R"({"rfid":")");
        payload.readBytes(buffer, 8);
        authCache[rfidCount++] = readRfidString(buffer);

        // If we fill up the cache then we need to back out
        if (rfidCount == Constants::maxCachedRfids) {
            Serial.println(F("Cache is full"));
            break;
        }
    } while (payload.findUntil(",","]"));
    http.end();
}

void Authentication::clearAuthCache() {
    // We only need to clear up as many as we have stored
    for (int i = 0; i < rfidCount; i++) authCache[i] = 0;

    rfidCount = 0;
}

uint32_t Authentication::readRfidString(char* rfidString) {
    uint32_t key = strtoul(rfidString, nullptr, 16);
    return key;
}
