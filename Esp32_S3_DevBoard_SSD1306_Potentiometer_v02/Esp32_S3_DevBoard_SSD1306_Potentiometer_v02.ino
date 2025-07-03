/*
  This sketch shows how to work with the ESP32-S3 Extension Development Board.
  It comes with a socket for an ESP32-S3 Development Board, connectors for an OLED and
  TFT Display with optional touch surface.
  Additionally a DHT11 environment sensor and a 10 KOhm Trimpot Trimmer Potentiometer are
  on the PCB.

  This is an example for an 0.96 inch OLED SSD1306 display showing the raw and voltage
  data read out from the Analog Digital Converter (ADC) pin that is wired with the Potentiometer.
  Turning the knob on the potentiometer clockwise increases the values. As this is a 
  Trimming Potentiometer the values change very slowly.

  Purpose of the sketch:
  - read out the raw data from the selected Analog Digital Converter (ADC) pin
  - calculate the voltage from the raw data
  - display the raw and voltage data on an 0.96 inch OLED SSD1306 display

  Note: The sketch is working when you use a breadboard pin jumper on the right side of
        the potentiometer, as this is connected to GPIO pin 1. Don't use this jumper when
        using a TFT display with Touch surface. For details see 'Potentiometer.h'.

  This is tested with ESP32 Board version 3.2.0 with SDK 5.4.1 on Arduino IDE 2.3.6
  Adafruit_SSD1306 version:      2.5.13
  Adafruit-GFX-Library version:  1.12.1
*/

/*
Version Management
03.07.2025 V02 Tutorial version
23.06.2025 V01 Initial programming
*/

// 


#include "Oled_Ssd1306.h"
#include "Potentiometer.h";

void setup() {
  // Initialize Serial Monitor
  Serial.begin(115200);
  delay(500);
  Serial.println(F("ESP32 S3 Development Board with SSD1306 OLED and Potentiometer"));

  initDisplay();
  Serial.printf("The Potentiometer is read on GPIO %d\n", POTENTIOMETER_PIN);

  display1 = "ESP32-S3 Dev Board";
  display2 = "SSD1306 OLED";
  display3 = "Potentiometer";
  displayValues6Rows();

  delay(2000);
  display1 = "ESP32-S3 Potentiomet.";
}

void loop() {

  display2 = "Raw:  " + String(getPotentiometerValue());
  display3 = "Volt :" + String(getPotentiometerVoltsValue());
  displayValues3Rows();
  delay(1000);
}
