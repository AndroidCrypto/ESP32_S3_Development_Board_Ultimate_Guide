# ESP32-S3 Development Board Ultimate Guide
The repository contains informations and sketches that work with a Development Board that comes with pre-wired TFT Display and OLED Display connectors, a DHT11 temperature and humidity sensor and a 10K Ohm potentiometer

This is the accompanying repository for my article "The Ultimate Guide to the ESP32-S3 Development Board Kit with 2.8 Inch Display" available here: https://medium.com/@androidcrypto/the-ultimate-guide-to-the-esp32-s3-expansion-board-kit-with-2-8-inch-touch-display-8fbb68760a1c

I'm providing these sketches for testing the board and components:

a) **Esp32_S3_DevBoard_SSD1306_Dht11_v02**: The sketch is working with the onboard DHT11 environment sensor and displays the current temperature and humidity on the OLED SSD1306 display. [Link to short tutorial](https://medium.com/@androidcrypto/esp32-s3-expansion-board-kit-with-oled-ssd1306-display-and-onboard-dht11-environment-sensor-4e267486d73b)

b) **Esp32_S3_DevBoard_SSD1306_Potentiometer_v02**: The sketch is working with the onboard Trimpot Trimmer Potentiometer and displays the measured value from the "Analog Digital Converter ("ADC") that results on rotating the knob of the Potentiometer on the OLED SSD1306 display. [Link to short tutorial](https://medium.com/@androidcrypto/esp32-s3-expansion-board-kit-with-oled-ssd1306-display-and-onboard-10-kilo-ohm-trimpot-trimmer-3021703de8f6)

c) **Esp32_S3_DevBoard_SSD1306_Leds_v2**: light the ESP32-S3 Development Board onboard RGB LED in Red, Green, Blue, White, Rainbow mode and off. Displays the color on an 0.96 inch OLED SSD1306 display. [Link to short tutorial](https://medium.com/@androidcrypto/esp32-s3-expansion-board-kit-with-oled-ssd1306-display-and-onboard-rgb-led-6eab85a926bb)

d) **Esp32_S3_DevBoard_Tft_Ili9341_TouchCalibration_v02**: calibrate the TFT display, driven by an ILI9341 display with touch surface. [Link to short tutorial](https://medium.com/@androidcrypto/esp32-s3-expansion-board-with-2-8-inch-ili9341-tft-display-with-touch-surface-calibration-9baa7df554ac)

e) **ESP32-S3 Expansion Board with 2.8 inch ILI9341 TFT Display with Touch Surface, DHT11 sensor and RGB on-/off toggle**: measures the temperature and humidity with onboard DHT11, display the data on Analog Meters and draw a 'virtual' (greem) rectangle on the TFT display. Touch to this area toggles the onboard RGB LED on and off.  [Link to short tutorial](https://medium.com/@androidcrypto/esp32-s3-expansion-board-with-2-8-37af6b08ca46)

## Overview of internal pin settings 

````
Wiring of the TFT Display connector from left to right to the ESP32-S3:
-- Display --
VCC   3.3 Volts
GND   GND
CS    14
RESET 21
DC    47
MOSI  45 ('SDI')
SCK    3 ('SCLK')
LED   3.3 Volts
MISO  46 ('SDO')
-- Touch --
T_CLK 42
T_CS   1
T_DIN  2 ('MOSI')
T_DO  41 ('MISO')
T_IRQ not connected

Wiring of the OLED Display connector from left to right to the ESP32-S3:
-- Display --
GND   GND
VCC   3.3 Volts
SCL   41
SDA   42

The Data pin of the DHT11 is wired to the upper breadboard pin, the lower breadboard pin 
is wired to the GPIO 2.

But beware: GPIO 2 is the T_DIN pin of the Touch display as well, and when using this setup
your Touch part will not work any longer ! My solution: use a jumper wire and connect the
upper pin (here on the right side) to any GPIO pin available, e.g. GPIO 36.

The wiper is connected to the upper breadboard pin (see arrow), the lower breadboard pin
is wired to the GPIO 1.

But beware: GPIO 1 is the T_CS pin of the Touch display as well, and when using this setup
your Touch part will not work any longer ! My solution: use a jumper wire and connect the
upper pin to any GPIO pin with Analog-Digital Converter ("ADC") available, e.g. GPIO 4.
````

