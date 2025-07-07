/*
  This sketch shows how to work with the ESP32-S3 Expansion Development Board.
  It comes with a socket for an ESP32-S3 Development Board, connectors for an OLED and
  TFT Display with optional touch surface.
  Additionally a DHT11 environment sensor and a 10 KOhm Trimpot Trimmer Potentiometer are
  on the PCB.

  The sketch shows how to work with a TFT Display. It has a size of 240x320 pixels and is 
  driven by an ILI9341 chip. Additionally I'm using a display with a Resistive Touch surface.
  
  The library for the display is TFT_eSPI in a modified version, because the original
  version (2.5.4) can't run on ESP32 SDK 3.x so far.

  Purpose of the sketch:
  - get the temperature and humditiy from onboard DHT11 sensor *1)
  - display the temperature and humidity on analog meters
  - draw a 'virtual' button (a green rectangle) on the display and when touching
    this area the onboard RGB toggles on and off

  Note *1): you have to wire the DHT11 sensor to GPIO 36 and don't use a breadboard jumper, because
            this would brake the touch functionality.

  This is tested with ESP32 Board version 3.2.0 with SDK 5.4.1 on Arduino IDE 2.3.6
  TFT_eSPI by Bodmer, version: 2.5.43 (https://github.com/Bodmer/TFT_eSPI)
  TFT_eSPI modified: https://github.com/AndroidCrypto/TFT_eSPI
  TFT_eWidget by Bodmer, version 0.0.5 [Aug 13, 2022]  (https://github.com/Bodmer/TFT_eWidget)
  XPT2046_Touchscreen library by Paul Stoffregen, version: 1.4 (https://github.com/PaulStoffregen/XPT2046_Touchscreen)
  DHT Sensor Library by Adafruit, version 1.4.6 (https://github.com/adafruit/DHT-sensor-library)
  Adafruit Unified Sensor Lib by Adafruit, version 1.1.5 (https://github.com/adafruit/Adafruit_Sensor)

  When uploading use ESP32S3 Dev Module, don't forget to enable 'Tools-USB-CDC On Boot' for Serial Monitor outputs

*/

/*
Version Management
05.07.2025 V02 Tutorial version
01.07.2025 V01 Initial programming
*/

// -------------------------------------------------------------------------------
// Sketch and Board information
const char *PROGRAM_VERSION = "ESP32-S3 ILI9341 DHT11 V02";
const char *PROGRAM_VERSION_SHORT = "ESP32-S3 ILI9341 DHT11 V02";


// -------------------------------------------------------------------------------
// TFT Display

#include <TFT_eSPI.h>     // Hardware-specific library
#include <TFT_eWidget.h>  // Widget library https://github.com/Bodmer/TFT_eWidget/tree/main
#include <SPI.h>

TFT_eSPI tft = TFT_eSPI();  // Invoke custom library with default width and height


// -------------------------------------------------------------------------------
// XPT2046_Touchscreen library

#include <XPT2046_Touchscreen.h>

// Touchscreen pins
//#define XPT2046_IRQ  -1 // T_IRQ
#define XPT2046_MOSI 2   // T_DIN
#define XPT2046_MISO 41  // T_OUT
#define XPT2046_CLK 42   // T_CLK
#define XPT2046_CS 1     // T_CS

SPIClass touchscreenSPI = SPIClass(VSPI);
//SPIClass touchscreenSPI = SPIClass(HSPI);

XPT2046_Touchscreen touchscreen(XPT2046_CS);

#define DISPLAY_PORTRAIT_MODE 2
#define SCREEN_WIDTH 240
#define SCREEN_HEIGHT 320

// Touchscreen coordinates: (x, y) and pressure (z)
int x, y, z;

// -------------------------------------------------------------------------------
// On-/Off button

// Stores current button state
bool buttonState = false;

// Button position and size
#define FRAME_X 143
#define FRAME_Y 266
#define FRAME_W 100
#define FRAME_H 50

void drawButton() {
  tft.fillRect(FRAME_X, FRAME_Y, FRAME_W, FRAME_H, TFT_GREEN);
}

// Print Touchscreen info about X, Y and Pressure (Z) on the Serial Monitor
void printTouchToSerial(int touchX, int touchY, int touchZ) {
  Serial.print("X = ");
  Serial.print(touchX);
  Serial.print(" | Y = ");
  Serial.print(touchY);
  Serial.print(" | Pressure = ");
  Serial.print(touchZ);
  Serial.println();
}

// -------------------------------------------------------------------------------
// RGB LED Pin
#define RGB_LED 97  // inversed logic: OFF when HIGH and ON when LOW

// -------------------------------------------------------------------------------
// Temeperature and Humidity Meter

MeterWidget temp = MeterWidget(&tft);
MeterWidget hum = MeterWidget(&tft);
float presMap;

#define LOOP_PERIOD 35  // Display updates every 35 ms

// -------------------------------------------------------------------------------
// DHT11 sensor

#include "Dht11_Sensor.h"

int interval = 2000;

void setup(void) {
  Serial.begin(115200);
  delay(1000);
  Serial.println(PROGRAM_VERSION);

  initDht11Sensor();
  printInternalDht11Data();

  // Start the SPI for the touchscreen and init the touchscreen
  touchscreenSPI.begin(XPT2046_CLK, XPT2046_MISO, XPT2046_MOSI, XPT2046_CS);
  touchscreen.begin(touchscreenSPI);
  // Set the Touchscreen rotation in portrait mode
  touchscreen.setRotation(DISPLAY_PORTRAIT_MODE);

  tft.begin();

  tft.setRotation(DISPLAY_PORTRAIT_MODE);
  tft.fillScreen(TFT_BLACK);
  tft.setTextColor(TFT_GREEN);
  tft.drawCentreString(PROGRAM_VERSION_SHORT, 120, 0, 2);

  temp.analogMeter(0, 14, 36.0, "Temp °C", "0", "9", "18", "27", "36");          // Draw analogue meter at 0, 0
  hum.analogMeter(0, 136, 100.0, "Hum %", "0", "25.0", "50.0", "75.0", "100.0");  // Draw analogue meter at 0, 0

  //displayPressureLinearMeter();
  tft.setTextSize(2);
  tft.setTextColor(TFT_YELLOW, TFT_BLACK);
  tft.drawString("LED on/off:", 5, 291);
  drawButton();
  tft.setTextSize(1);

  pinMode(RGB_LED, OUTPUT);
  digitalWrite(RGB_LED, LOW);
}

void loop() {

  // Checks if Touchscreen was touched, and prints X, Y and Pressure (Z) info on the TFT display and Serial Monitor
  if (touchscreen.touched()) {
    // Get Touchscreen points
    TS_Point p = touchscreen.getPoint();
    // Calibrate Touchscreen points with map function to the correct width and height
    // see my calibration sketch to get the data 'Esp32_CYD_Tft_Ili9341_TouchCalibration_v02'
    x = map(p.x, 460, 3650, 1, SCREEN_WIDTH);
    y = map(p.y, 380, 3818, 1, SCREEN_HEIGHT);
    z = p.z;

    Serial.print("Raw: ");
    printTouchToSerial(p.x, p.y, p.z);
    Serial.print("Map: ");
    printTouchToSerial(x, y, z);

    if ((x > FRAME_X) && (x < (FRAME_X + FRAME_W))) {
      if ((y > FRAME_Y) && (y < (FRAME_Y + FRAME_H))) {
        // toggle buttonState
        buttonState = !buttonState;
        if (buttonState) {
          digitalWrite(RGB_LED, HIGH);
        } else {
          digitalWrite(RGB_LED, LOW);
        }
        delay(500); // debounce 'button'
      }
    }
  }
  //delay(1000);
  if ((millis() - interval) > 1000) {

    // Get temperature event and print its value.
    sensors_event_t event;
    dht.temperature().getEvent(&event);
    if (isnan(event.temperature)) {
      Serial.println(F("Error reading temperature!"));
    } else {
      temperature = event.temperature;
      Serial.print(F("Temperature: "));
      Serial.print(event.temperature);
      Serial.println(F("°C"));
    }
    // Get humidity event and print its value.
    dht.humidity().getEvent(&event);
    if (isnan(event.relative_humidity)) {
      Serial.println(F("Error reading humidity!"));
    } else {
      humidity = event.relative_humidity;
      Serial.print(F("Humidity: "));
      Serial.print(event.relative_humidity);
      Serial.println(F("%"));
    }

    temp.updateNeedle(temperature, 0);
    hum.updateNeedle(humidity, 0);
    interval = millis();
  }
}

float mapValue(float ip, float ipmin, float ipmax, float tomin, float tomax) {
  return tomin + (((tomax - tomin) * (ip - ipmin)) / (ipmax - ipmin));
}
