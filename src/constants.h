#pragma once

#include <WString.h>
#include <Arduino.h>

#define TOSTRINGIMPL(x) #x
#define TOSTRING(x) TOSTRINGIMPL(x)

namespace Constants {
    // Wifi
    /** The SSID of the Wi-Fi network to use */
    const String wifiSsid = F(TOSTRING(WIFI_SSID));
    /** The password of the Wi-Fi network to use */
    const String wifiPass = F(TOSTRING(WIFI_PASS));

    // OTA
    /** The hostname of the ESP for OTA updates */
    const String otaHostname = F(TOSTRING(OTA_HOST));
    /** The password for authenticating OTA updates */
    const String otaPassword = F(TOSTRING(OTA_PASS));

    // HMIS
    /** The ID that represents this Lock in HMIS */
    const String lockId = F(LOCK_ID);
    /** The HMIS URL for fetching a list of authorised RFIDs */
    const String hmisLockUrl = String(HMIS_URL) + "/rfid/lock/" + lockId  + "/";
    /** The HMIS URL for checking if an RFID is authorised */
    const String hmisCheckUrl = String(HMIS_URL) + "/rfid/check/" + lockId  + "/";
    /** The token to authenticate with the HMIS server */
    constexpr char hmisToken[] = TOSTRING(HMIS_TOKEN);
    /** If true, query HMIS directly if a user is authorised, otherwise always check the cache */
    constexpr bool liveCheck = LIVE_CHECK;

    /** The maximum amount of RFIDs to cache (Memory usage is this * 4 bytes) */
    constexpr int maxCachedRfids = 400; // 1.6KiB
    /** The interval in milliseconds between rfid cache updates */
    constexpr unsigned long cacheUpdateInterval = CACHE_UPDATE_INTERVAL * 1000UL; // 30 minutes

    // Lock
    /** The length of time in milliseconds to hold the Lock open for (0 to disable) */
    constexpr unsigned long unlockDelay = UNLOCK_DELAY * 1000;
    /** If true, buzz whilst the door is unlocked */
    constexpr bool unlockBuzz = UNLOCK_BUZZ;
    /**
     * The length of time in milliseconds to block any access attempt (RFID or Keypad) after a failed auth (Bad keycard
     * or Passcode)
     */
    constexpr unsigned long badAuthDelay = BAD_AUTH_DELAY * 1000;
    /**
     * The number of times to flash the LED when an unauthorised input RFID card is scanned, or a bad passcode is
     * entered
     * <p>
     * Each blink is 0.1 seconds, each blink has 2 parts, on then off, therefore to find the number of blinks to cover
     * the full Constants::badAuthDelay, we need to divide the number of seconds to blink for by the length of each
     * blink (0.1 * 2). Constants::badAuthDelay can be 0, so we want to blink atleast 5 times
     */
    const int badAuthFlashCount = max<int>(BAD_AUTH_DELAY / 0.2, 5);
    /** If true, enable entering a keycode using the keypad */
    constexpr bool enableKeycode = ENABLE_KEYPAD_PASSCODE;
    /** The length of the keycode */
    constexpr int keycodeLength = sizeof(KEYPAD_PASSCODE) - 1;
    /** A mask used to clear the unused bits of the keycode */
    constexpr uint32_t keycodeClearMask = 0xFFFFFFFF << keycodeLength * 4;
    /** The keycode packed into a uint32 and correctly masked */
    const uint32_t keycode = keycodeClearMask | strtol(KEYPAD_PASSCODE, nullptr, 16);
    /** If true, the button will unlock the Lock when locked */
    constexpr bool buttonUnlock = BUTTON_UNLOCK;
    /** If true, the button can will lock the Lock when unlocked */
    constexpr bool buttonLock = BUTTON_LOCK;

    // Pins
    /** D0 for Wiegand data */
    constexpr int pinD0 = PIN_D0;
    /** D1 for Wiegand Data */
    constexpr int pinD1 = PIN_D1;
    /** Unlocked LED enable (Low to turn on) */
    constexpr int pinLedUnlock = PIN_LED_UNLOCK;
    /** Locked LED enable (Low to turn on) */
    constexpr int pinLedLock = PIN_LED_LOCK;
    /** Buzzer enable (Low to turn on) */
    constexpr int pinBuzzer = PIN_BUZ;
    /** Lock Button observer (Low for pressed) */
    constexpr int pinButtonLock = PIN_BUT_LOCK;
    /** Relay enable (Low to close relay) */
    constexpr int pinRelay = PIN_REL;

    // Pin Inversions
    /** Invert the LED Unlock pin's output so "on" is LOW */
    constexpr bool invertLedUnlock = INVERT_LED_UNLOCK;
    /** Invert the LED Lock pin's output so "on" is LOW */
    constexpr bool invertLedLock = INVERT_LED_LOCK;
    /** Invert the Buzzer pin's output so "on" is LOW */
    constexpr bool invertBuzzer = INVERT_BUZZER;
    /** Invert the Relay pin's output so "on" is LOW */
    constexpr bool invertRelay = INVERT_RELAY;
}