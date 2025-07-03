/*
  This sketch shows how to work with the ESP32-S3 Extension Development Board.
  It comes with a socket for an ESP32-S3 Development Board, connectors for an OLED and
  TFT Display with optional touch surface.
  Additionally a DHT11 environment sensor and a 10 KOhm Trimpot Trimmer Potentiometer are
  on the PCB.

  This is an example for an 0.96 inch OLED SSD1306 display showing the color of the ESP32-S3
  board's RGB LED.

  Purpose of the sketch:
  - light the RGB LED in Red, Green, Blue, White, Rainbow mode and off
  - display the color on an 0.96 inch OLED SSD1306 display

  This is tested with ESP32 Board version 3.2.0 with SDK 5.4.1 on Arduino IDE 2.3.6
  Adafruit_SSD1306 version:      2.5.13
  Adafruit-GFX-Library version:  1.12.1
  FastLED version:               3.9.15

*/

/*
Version Management
03.07.2025 V02 Tutorial version
23.06.2025 V01 Initial programming
*/

#include "Oled_Ssd1306.h"
#include "Leds.h"

long rgbLedDurationMillis = 5000;  // 5 seconds
long rgbLedMillis = 0;
uint8_t rgbLedColorIndex = 0;
bool showRainbow = false;

void setup() {
  // Initialize Serial Monitor
  Serial.begin(115200);
  delay(500);
  Serial.println(F("ESP32 S3 Development Board with SSD1306 OLED and LEDs"));

  initDisplay();

  display1 = "ESP32-S3 Dev Board";
  display2 = "SSD1306 OLED";
  display3 = "LEDs";
  displayValues6Rows();

  initRgbLed();

  display4 = "     LED color:";
  rgbLedMillis = millis();
}

void loop() {

  if (showRainbow) {
    showRgbRainbow();
  }

  if (millis() - rgbLedMillis > rgbLedDurationMillis) {
    showRainbow = false;
    if (rgbLedColorIndex == 0) {
      display5 = "     -= RED =-";
      displayValues6Rows();
      showRgbLedRed();
      rgbLedColorIndex++;
    } else if (rgbLedColorIndex == 1) {
      display5 = "     -= GREEN =-";      
      displayValues6Rows();
      showRgbLedGreen();
      rgbLedColorIndex++;
    } else if (rgbLedColorIndex == 2) {
      display5 = "     -= BLUE =-";
      displayValues6Rows();
      showRgbLedBlue();
      rgbLedColorIndex++;
    } else if (rgbLedColorIndex == 3) {
      display5 = "     -= WHITE =-";
      displayValues6Rows();
      showRgbLedWhite();
      rgbLedColorIndex++;
    } else if (rgbLedColorIndex == 4) {
      display5 = "   -= RAINBOW =-";
      displayValues6Rows();
      showRainbow = true;
      rgbLedColorIndex++;
    } else if (rgbLedColorIndex == 5) {
      display5 = "     -= OFF =-";
      displayValues6Rows();
      showRgbLedBlack();
      rgbLedColorIndex = 0;
    }
    rgbLedMillis = millis();
  }
}
