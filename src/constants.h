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
    /** The Id that represents this Lock in HMIS */
    static const String lockId = F(LOCK_ID);
    /** The HMIS URL for fetching a list of authorised RFIDs */
    static const String hmisLockUrl = "https://your.hmis.rfid/rfid/lock/" + lockId  + "/";
    /** The token to authenticate with the HMIS server */
    static constexpr char hmisToken[] = TOSTRING(HMIS_TOKEN\0);

    /** The maximum amount of RFIDs to cache (Memory usage is this * 4 bytes) */
    static constexpr int maxCachedRfids = 400; // 1.6KiB
    /** The interval in milliseconds between cache updates */
    static constexpr unsigned long cacheUpdateInterval = 1800UL * 1000UL; // 30 minutes

    // Lock
    /** The length of time in seconds to hold the Lock open for (0 to disable) */
    static const unsigned long unlockDelay = UNLOCK_DELAY * 1000;
    /** If true, buzz whilst the door is unlocked */
    static const bool unlockBuzz = UNLOCK_BUZZ;

    // Pins
    /** D0 for Wiegand data */
    static constexpr int PIN_D0  = 22;
    /** D1 for Wiegand Data */
    static constexpr int PIN_D1  = 21;
    /** Unlocked LED enable (Low to turn on) */
    static constexpr int PIN_LED_UNLOCK = 15;
    /** Locked LED enable (Low to turn on) */
    static constexpr int PIN_LED_LOCK = 4;
    /** Buzzer enable (Low to turn on) */
    static constexpr int PIN_BUZ = 16;
    /** Lock Button observer (Low for pressed) */
    static constexpr int PIN_BUT_LOCK = 35;
    /** Relay enable (Low to close relay) */
    static constexpr int PIN_REL = 17;
}