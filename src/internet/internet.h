#pragma once

#include <cstdint>

#include "constants.h"

/**
 * A class wrapping the internet based functionality of this embedded program
 */
class Internet {
    /** The number of RFIDs stored in the cache */
    int rfidCount = 0;
    /** A cache of RFIDs for looking up access rights */
    uint32_t authCache[Constants::maxCachedRfids]{};

    /** The last time a cache update occurred */
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
     * Loop for internet functionality:
     * <ul>
     *     <li>Every 20 minutes, update cache</li>
     * </ul>
     */
    void loop();

    /**
     * check if the given RFID is authorised for this Lock.
     * @param rfid The RFID to check
     * @return True if the given RFID is authorised for this Lock
     */
    bool isAuthorised(uint32_t rfid);

private:
    /**
     * Check the given keycode is authorised with HMIS.
     * @param keyCode The keycode to check.
     */
    bool checkAuthLive(uint32_t keyCode);
    /**
     * Check the given keycode is authorised with the cache (Internet::authCache).
     * @param keyCode The keycode to check.
     */
    bool checkAuthCache(uint32_t keyCode);

    /**
     * Update the cache of authorised users for this Lock.
     */
    void updateAuthCache();

    /**
     * Empty the auth cache, zeroing all the currently stored values.
     */
    void clearAuthCache();

    /**
     * Convert an 8 character RFID string to a 4 byte integer.
     * @param rfidString A pointer to the 8 character RFID string
     * @return An integer packing the RFID hex
     */
    static uint32_t readRfidString(char* rfidString);
};
