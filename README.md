# hmis-locks
A [PlatformIO](https://platformio.org/) program for a electronic locks that are unlocked using a Wiegand based RFID 
scanner and authorises users using a HMIS endpoint.

# Keypad support
Wiegand readers with keypads are supported. You can configure the valid keycode by setting the `enable_keypad_passcode`
option to a numeric string up to 8 numbers long.

When using the keypad, the '*' (star) button will clear (provided it maps to 0x0A), and the '#' (Hashtag) button will
submit (provided it maps to 0x0B).

# Setup
## Platform IO
This application uses [PlatformIO](https://platformio.org/) for dependency management and installation. Installation
instructions for your platform can be found [here](https://platformio.org/install/cli).

## Config
Configuration options have been exposed in [platformio.ini](/platformio.ini) under the `[base_config]` section.

The `WiFi` and `HMIS` options must be configured for your network and HMIS server. 

The `Unlock` and `Auth` options can be configured to suit your needs. 

The `Pins` options should be configured for your ESP, preferably in a separate section that inherits the `[base_config]` 
section (see the `[config_dfrobot_firebeetle2_esp32e]` section for an example of this).

The `Pin Inversions` options should be configured to match your circuit.

A preset environment has been provided for the [FireBeetle 2 ESP32-E](https://wiki.dfrobot.com/FireBeetle_Board_ESP32_E_SKU_DFR0654#target_5)
(my chosen ESP32), you will need to provide your own environment with it's required pin setup if you have a different
ESP32 (we welcome PRs for alternative setups).

## Installation
Run the following to build and upload the project to your ESP32:
```shell
pio run -t upload -e [your-env]
```