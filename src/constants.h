#pragma once

#include <WString.h>
#include <pins_arduino.h>

#define TOSTRINGIMPL(x) #x
#define TOSTRING(x) TOSTRINGIMPL(x)

namespace Constants {
    // Wifi
    /** The SSID of the wifi network to use */
    static const String wifiSsid = F(TOSTRING(WIFI_SSID));
    /** The password of the wifi network to use */
    static const String wifiPass = F(TOSTRING(WIFI_PASS));

    // HMIS
    /** The HMIS URL for fetching a list of authorised RFIDs */
    static const String hmisLockUrl = F(TOSTRING(HMIS_URL/rfid/lock/LOCK_ID));
    /** The HMIS URL for checking if an RFID is authorised */
    static const String hmisCheckUrl = F(TOSTRING(HMIS_URL/rfid/check/LOCK_ID/));
    /** The token to authenticate with the HMIS server */
    static constexpr char hmisToken[] = TOSTRING(HMIS_TOKEN\0);
    /** The Id that represents this lock in HMIS */
    static const String lockId = F(TOSTRING(LOCK_ID));

    /** The maximum amount of RFIDs to cache (Memory usage is this * 4 bytes) */
    static constexpr int maxCachedRfids = 400; // 1.6KiB
    /** The interval in milliseconds between cache updates */
    static constexpr unsigned long cacheUpdateInterval = 1800UL * 1000UL; // 30 minutes

    // Lock
    /** The length of time in seconds to hold the lock open for */
    static const int unlockDelay = UNLOCK_DELAY;

    // Pins
    /** D0 for Wiegand data */
    static constexpr int PIN_D0  = 12;
    /** D1 for Wiegand Data */
    static constexpr int PIN_D1  = 4;
    /** Green LED enable (Low to turn on) */
    static constexpr int PIN_LED = 4;
    /** Button observer (High for pressed) */
    static constexpr int PIN_BUT = 5;
    /** Relay enable (Low to close relay) */
    static constexpr int PIN_REL = 6;
}