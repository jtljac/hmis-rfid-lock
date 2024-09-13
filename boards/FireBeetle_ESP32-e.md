# Firebeetle ESP32-e

Product Page: https://www.dfrobot.com/product-2195.html
Wiki: https://wiki.dfrobot.com/FireBeetle_Board_ESP32_E_SKU_DFR0654

# Image
![Board screenshot](./FireBeetle_ESP32-e.png)

# Components
- 1x Firebeetle ESP32-e
- 1x [5v relay board](https://www.switchelectronics.co.uk/products/5v-1-channel-low-level-trigger-relay-module-with-optocoupler)
- 1x [12v to 5v buck converter](https://www.aideepen.com/products/dc-dc-usb-9v-12v-19v-to-5v-3a-step-down-buck-converter-power-supply-charger-module-voltage-regulator-for-vehicle-car-board)
- 1x [Logic level shifter](https://www.sparkfun.com/products/12009)
- 2x 5.1k resistor (Optional for relay)
- 2x 4x1 2.54mm header
- 1x 3x1 2.54mm header
- 6x 2x1 2.54mm header

# Notes
- The ESP Power header in the top left is a place holder for wires that connect from the two through holes to the FireBeetle's
  VIN and GND pads underneath its usb port.
- The ESP and relay are powered using a 12v to 5v buck converter daughter board
  - The board listed doesn't perfectly fit 2.54mm spacing
- The FireBeetle, logic level shifter, and buck converter are expected to be soldered directly to the board.
- The Relay has an optional resistor incase you need to lower the current draw for the relay, a 5.1k resistor is suggested for the linked relay.
- A ground pin is provided next to the feedback for when using them to power outputs. When all these pins are inverted
  this ground pin can be ignored.
- A Logic Level Shifter is used to step down the the Wiegand data lines from 5v to 3v3 to prevent damage to the esp
- The button is using an internal pull-up resistor
