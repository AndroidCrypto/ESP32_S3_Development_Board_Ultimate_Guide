/*
Wiring OLED Display - ESP32
OLED - ESP32
GND  - GND
VCC  - 3.3V don't use 5V !
SDA  - Pin 42
SCL  - Pin 41
*/

// ----------------------------------------------------------------
// OLED SSD1306
#define OLED_I2C_SDA_PIN 42
#define OLED_I2C_SCL_PIN 41

#define OLED_RESET_PIN -1
#define OLED_I2C_ADDRESS 0x3C  // See datasheet for Address, could be 0x3D for 128x64 as well
#define OLED_SCREEN_WIDTH 128
#define OLED_SCREEN_HEIGHT 64

#include <Wire.h>
#include <Adafruit_GFX.h> // https://github.com/adafruit/Adafruit-GFX-Library
#include <Adafruit_SSD1306.h> // https://github.com/adafruit/Adafruit_SSD1306
#include "FONT_MONOSPACE_9.h"
// For a connection via I2C using the Arduino Wire include:
Adafruit_SSD1306 display(OLED_SCREEN_WIDTH, OLED_SCREEN_HEIGHT, &Wire, OLED_RESET_PIN);

// vars for displaying line 1 to 7
String display1 = "";
String display2 = "";
String display3 = "";
String display4 = "";
String display5 = "";
String display6 = "";
String display7 = "";

void initDisplay() {
  // ----------------------------------------------------------------
  // init the OLED display
  // Initialize the I2C communication
  Wire.begin(OLED_I2C_SDA_PIN, OLED_I2C_SCL_PIN);  // Use GPIO x for SDA and GPIO x for SCL
  Serial.println(F("Wire.begin is done"));
  display.begin(SSD1306_SWITCHCAPVCC, OLED_I2C_ADDRESS);  // Check your display's I2C address
}

void displayValues6Rows() {
  display.clearDisplay();
  display.setTextSize(1);
  display.setTextWrap(false);
  display.setTextColor(SSD1306_WHITE);
  display.setCursor(0, 0);
  display.print(display1);
  display.setCursor(0, 11);
  display.print(display2);
  display.setCursor(0, 22);
  display.print(display3);
  display.setCursor(0, 33);
  display.print(display4);
  display.setCursor(0, 44);
  display.print(display5);
  display.setCursor(0, 55);
  display.print(display6);
  display.display();
}

void displayValues5Rows() {
  display.clearDisplay();
  display.setTextSize(1);
  display.setTextWrap(false);
  display.setTextColor(SSD1306_WHITE);
  display.setCursor(0, 0);
  display.print(display1);
  display.setCursor(0, 12);
  display.print(display2);
  display.setCursor(0, 24);
  display.print(display3);
  display.setCursor(0, 36);
  display.print(display4);
  display.setCursor(0, 48);
  display.print(display5);
  display.display();
}

void displayValues3Rows() {
  display.clearDisplay();
  display.setTextWrap(false);
  display.setTextColor(SSD1306_WHITE);
  display.setTextSize(1);
  display.setCursor(0, 0);
  display.print(display1);
  display.setTextSize(2);
  display.setCursor(0, 20);
  display.print(display2);
  display.setCursor(0, 45);
  display.print(display3);
  display.display();
}

void clearDisplayValues() {
  display1 = "";
  display2 = "";
  display3 = "";
  display4 = "";
  display5 = "";
  display6 = "";
  display7 = "";
}
