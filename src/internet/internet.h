#pragma once

#include <cstdint>

#include "constants.h"

/**
 * A class wrapping the internet based functionality of this embedded program
 * <p>
 * Much functionality of this class was borrowed from <a href="https://randomnerdtutorials.com/esp32-http-get-post-arduino/#http-get-1">here</a>
 */
class Internet {
    int rfidCount = 0;
    uint32_t authCache[Constants::maxCachedRfids]{};

    unsigned long lastCacheUpdateMillis = 0UL;

public:

    /**
     * Setup for the Internet based functionality:
     * <ul>
     *     <li>Setup and wait for wifi connection</li>
     *     <li>Fetch initial copy of authorised users</li>
     * </ul>
     */
    void setup();

    /**
     * Wifi loop method:
     * <ul>
     *     <li>Every 20 minutes, update cache</li>
     * </ul>
     */
    void loop();

private:

    /**
     * Update the cache of authorised users for this lock
     */
    void updateAuthCache();

    /**
     * Empty the auth cache, zeroing all the currently stored values
     */
    void clearAuthCache();

    /**
     * Convert an 8 character RFID string to a 4 byte integer
     * @param rfidString A pointer to the 8 character RFID string
     * @return An integer packing the RFID hex
     */
    static uint32_t readRfidString(char* rfidString);
};
