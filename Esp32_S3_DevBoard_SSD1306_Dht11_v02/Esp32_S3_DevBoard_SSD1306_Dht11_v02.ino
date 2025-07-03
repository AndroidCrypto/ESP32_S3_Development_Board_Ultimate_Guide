/*
  This sketch shows how to work with the ESP32-S3 Extension Development Board.
  It comes with a socket for an ESP32-S3 Development Board, connectors for an OLED and
  TFT Display with optional touch surface.
  Additionally a DHT11 environment sensor and a 10 KOhm Trimpot Trimmer Potentiometer are
  on the PCB.

  This is an example for an 0.96 inch OLED SSD1306 display showing the temperature and
  humidity read out from the DHT11 sensor.

  Purpose of the sketch:
  - read out the temperature and from the DHT11 sensor
  - display the data on an 0.96 inch OLED SSD1306 display

  Note: The sketch is working when you use a breadboard pin jumper on the left side of
        the DHT11 sensor, as this is connected to GPIO pin 2. Don't use this jumper when
        using a TFT display with Touch surface. For details see 'Dht11_Sensor.h'.

  This is tested with ESP32 Board version 3.2.0 with SDK 5.4.1 on Arduino IDE 2.3.6
  Adafruit_SSD1306 version:      2.5.13
  Adafruit-GFX-Library version:  1.12.1

*/

/*
Version Management
03.07.2025 V02 Tutorial version
23.06.2025 V01 Initial programming
*/

#include "Oled_Ssd1306.h"
#include "Dht11_Sensor.h"

void setup() {
  // Initialize Serial Monitor
  Serial.begin(115200);
  delay(1000);
  Serial.println(F("ESP32 S3 Development Board with SSD1306 OLED and DHT11 Sensor"));

  initDisplay();

  display1 = "ESP32-S3 Dev Board";
  display2 = "SSD1306 OLED";
  display3 = "DHT11 Env. Sensor";
  displayValues6Rows();

  initDht11Sensor();
  display4 = "Init DHT11 sensor OK";
  displayValues6Rows();
  printInternalDht11Data();
  delay(2000);
  display1 = " ESP32-S3 DHT11 OLED";
 
}

void loop() {
  bool isValidSensorData = getDht11Data();
  if (isValidSensorData) {
    display2 = "T: " + String(temperature,1) + " Cels.";
    display3 = "H: " + String(humidity, 0) + " % rH";
    displayValues3Rows();
  } else {
    display2 = "T: ERROR";
    display3 = "H: ERROR";
    displayValues3Rows();
  }

  delay(1000);
}
