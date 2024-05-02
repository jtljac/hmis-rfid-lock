#pragma once

#include <Wiegand.h>

#include "feedback/feedback.h"

/**
 * A class wrapping the RFID functionality.
 */
class Rfid {
    /** A reference to the program's feedback class */
    Feedback& feedback;

    /** An instance of the Wiegand library */
    Wiegand wiegand;

    /** Whether Rfid::nextKey currently has a valid key stored */
    bool hasKey = false;
    /** The next key for the main loop to process */
    uint32_t nextKey = 0;

#if ENABLE_KEYPAD_PASSCODE
    /** Whether Rfid::nextKeycode currently has a valid pin stored */
    bool hasKeycode = false;
    /**
     * The next pin for the main loop to process
     * <p>
     * Initialise to all Fs as the keypad cannot provide a value higher than 9, making it impossible to accidentally get
     * the passcode without typing it fully
     */
    uint32_t nextKeycode = 0xFFFFFFFF;
#endif

    /** The last time a wiegand flush occurred */
    unsigned long lastFlush = 0UL;
public:
    /**
     * @param feedback The programs instance of Feedback
     */
    explicit Rfid(Feedback& feedback);

    /**
     * Setup for RFID based functionality:
     * <ul>
     *     <li>Setup the Wiegand library</li>
     *     <li>Setup the data pins for wiegand communication</li>
     * </ul>
     */
    void setup();

    /**
     * Loop for RFID based functionality:
     * <ul>
     *     <li>Flush data from Wiegand pins every 100ms</li>
     * </ul>
     */
    void loop();

    /**
     * Get a new key read by the Wiegand reader.
     * @param key The memory location to store the key in
     * @return <code>true</code> if there is a key available
     */
    bool getNextKey(uint32_t* key);

#if ENABLE_KEYPAD_PASSCODE
    /**
     * Get a new pin entered in the Wiegand reader.
     * @param pin The memory location to store the pin in
     * @return <code>true</code> if there is a pin available
     */
    bool getNextKeyCode(uint32_t* key);
#endif

    /**
     * Callback to update the state of the pins.
     * <p>
     * This should be called by an interrupt that will need setting up manually.
     */
    void updatePinState();

    /**
     * Callback for the Wiegand device being unplugged/plugged in.
     * @param plugged The new plugged in state of the Wiegand device
     */
    void stateChange(bool plugged);

    /**
     * Callback for when data has been received from the Wiegand device.
     * @param data The data received
     * @param bits The number of bits received
     */
    void receivedData(const uint8_t* data, uint8_t bits);

    /**
     * Callback for when an error occurs receiving data from the Wiegand device.
     * @param error The error
     * @param data The data received
     * @param bits The number of bits received
     */
    void receivedError(Wiegand::DataError error, const uint8_t* data, uint8_t bits);

private:
    /**
     * Pack 4 bytes into a single uint32_t
     * <p>
     * This also reverses the order of the bytes, as they are received in big-endian but are needed in little-endian.
     * @param bytes A uint8_t array of length 4 containing the bytes to pack
     * @return The packed bytes
     */
    static uint32_t packRfidBytes(const uint8_t* bytes);
};
