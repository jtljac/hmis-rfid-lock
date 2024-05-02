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
    static const String hmisLockUrl = String(HMIS_URL) + "/rfid/lock/" + lockId  + "/";
    /** The HMIS URL for checking if an RFID is authorised */
    static const String hmisCheckUrl = String(HMIS_URL) + "/rfid/check/" + lockId  + "/";
    /** The token to authenticate with the HMIS server */
    static constexpr char hmisToken[] = TOSTRING(HMIS_TOKEN);
    /** If true, query HMIS directly if a user is authorised, otherwise always check the cache */
    static constexpr bool liveCheck = LIVE_CHECK;

    /** The maximum amount of RFIDs to cache (Memory usage is this * 4 bytes) */
    static constexpr int maxCachedRfids = 400; // 1.6KiB
    /** The interval in milliseconds between rfid cache updates */
    static constexpr unsigned long cacheUpdateInterval = CACHE_UPDATE_INTERVAL * 1000UL; // 30 minutes

    // Lock
    /** The length of time in seconds to hold the Lock open for (0 to disable) */
    static const unsigned long unlockDelay = UNLOCK_DELAY * 1000;
    /** If true, buzz whilst the door is unlocked */
    static const bool unlockBuzz = UNLOCK_BUZZ;
    /** If true, enable entering a keycode using the keypad */
    static const bool enableKeycode = ENABLE_KEYPAD_PASSCODE;
    /** The length of the keycode */
    static const int keycodeLength = sizeof(KEYPAD_PASSCODE) - 1;
    /** A mask used to clear the unused bits of the keycode */
    static const uint32_t keycodeClearMask = 0xFFFFFFFF << keycodeLength * 4;
    /** The keycode packed into a uint32 and correctly masked */
    static const uint32_t keycode = keycodeClearMask | strtol(KEYPAD_PASSCODE, nullptr, 16);

    // Pins
    /** D0 for Wiegand data */
    static constexpr int pinD0 = PIN_D0;
    /** D1 for Wiegand Data */
    static constexpr int pinD1 = PIN_D1;
    /** Unlocked LED enable (Low to turn on) */
    static constexpr int pinLedUnlock = PIN_LED_UNLOCK;
    /** Locked LED enable (Low to turn on) */
    static constexpr int pinLedLock = PIN_LED_LOCK;
    /** Buzzer enable (Low to turn on) */
    static constexpr int pinBuzzer = PIN_BUZ;
    /** Lock Button observer (Low for pressed) */
    static constexpr int pinButtonLock = PIN_BUT_LOCK;
    /** Relay enable (Low to close relay) */
    static constexpr int pinRelay = PIN_REL;

    // Pin Inversions
    /** Invert the LED Unlock pin's output so "on" is LOW */
    static constexpr bool invertLedUnlock = INVERT_LED_UNLOCK;
    /** Invert the LED Lock pin's output so "on" is LOW */
    static constexpr bool invertLedLock = INVERT_LED_LOCK;
    /** Invert the Buzzer pin's output so "on" is LOW */
    static constexpr bool invertBuzzer = INVERT_BUZZER;
    /** Invert the Relay pin's output so "on" is LOW */
    static constexpr bool invertRelay = INVERT_RELAY;
}