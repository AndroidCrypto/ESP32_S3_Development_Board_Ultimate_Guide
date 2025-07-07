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
  - get the raw coordinates from the top left and bottom right screen positions
  - map the data to screen coordinates
  - test the mapped data by drawing a small circle at the touch position
  - provide the calibration data for other sketches

  This is tested with ESP32 Board version 3.2.0 with SDK 5.4.1 on Arduino IDE 2.3.6
  TFT_eSPI by Bodmer, version: 2.5.43 (https://github.com/Bodmer/TFT_eSPI)
  TFT_eSPI modified: https://github.com/AndroidCrypto/TFT_eSPI
  XPT2046_Touchscreen library by Paul Stoffregen, version: 1.4 (https://github.com/PaulStoffregen/XPT2046_Touchscreen)

  When uploading use ESP32S3 Dev Module, don't forget to enable 'Tools-USB-CDC On Boot' for Serial Monitor outputs

*/

/*
Version Management
05.07.2025 V02 Tutorial version
05.07.2025 V01 Initial programming
*/

// -------------------------------------------------------------------------------
// Sketch and Board information
const char *PROGRAM_VERSION = "ESP32-S3 TFT ILI9341 Touch Calibration V02";

// -------------------------------------------------------------------------------
// Debug printing
bool DEBUG = false;
//bool DEBUG = true; // uncomment this line to see much more data 

// -------------------------------------------------------------------------------
// TFT Display

#include <TFT_eSPI.h>  // Hardware-specific library
#include <SPI.h>

TFT_eSPI tft = TFT_eSPI();  // Invoke custom library with default width and height

// -------------------------------------------------------------------------------
// Touchscreen

#include <XPT2046_Touchscreen.h>

// Touchscreen pins
//#define XPT2046_IRQ  -1 // T_IRQ not connected
#define XPT2046_MOSI 2   // T_DIN
#define XPT2046_MISO 41  // T_OUT
#define XPT2046_CLK 42   // T_CLK
#define XPT2046_CS 1     // T_CS

SPIClass touchscreenSPI = SPIClass(VSPI); // the ILI9488 requires the HSPI class, for ILI9341 use VSPI
XPT2046_Touchscreen touchscreen(XPT2046_CS);

// -------------------------------------------------------------------------------
// Vars and definitions

#define DISPLAY_PORTRAIT_MODE 2
#define SCREEN_WIDTH 240
#define SCREEN_HEIGHT 320

#define THRESHOLD_Z 500
#define THRESHOLD_Z_CALIBRATION 150

// Touchscreen coordinates: (x, y) and pressure (z)
int16_t x, y, z;

// average values for the two corners
int32_t av_X_TL = 0;
int32_t av_Y_TL = 0;
int32_t av_X_BR = 0;
int32_t av_Y_BR = 0;

void drawCalibrationCorners(uint8_t size) {
  tft.fillRect(0, 0, size + 1, size + 1, TFT_RED);                                               // top left corner
  tft.fillRect(SCREEN_WIDTH - size - 1, SCREEN_HEIGHT - size - 1, size + 1, size + 1, TFT_RED);  // bottom right corner
}

void drawCalibrationArrowTopLeft(uint8_t size) {
  tft.fillRect(0, 0, size + 1, size + 1, TFT_RED);
  tft.drawLine(0, 0, 0, size, TFT_WHITE);
  tft.drawLine(0, 0, size, 0, TFT_WHITE);
  tft.drawLine(0, 0, size, size, TFT_WHITE);
}

void drawCalibrationArrowBottomRight(uint8_t size) {
  tft.fillRect(SCREEN_WIDTH - size - 1, SCREEN_HEIGHT - size - 1, size + 1, size + 1, TFT_RED);  // bottom right corner
  tft.drawLine(SCREEN_WIDTH - size - 1, SCREEN_HEIGHT - size - 1, SCREEN_WIDTH - 1, SCREEN_HEIGHT - 1, TFT_WHITE);
  tft.drawLine(SCREEN_WIDTH - 1, SCREEN_HEIGHT - 1 - size, SCREEN_WIDTH - 1, SCREEN_HEIGHT - 1, TFT_WHITE);
  tft.drawLine(SCREEN_WIDTH - 1 - size, SCREEN_HEIGHT - 1, SCREEN_WIDTH - 1, SCREEN_HEIGHT - 1, TFT_WHITE);
}

// Print Touchscreen info about X, Y and Pressure (Z) on the Serial Monitor
void printTouchToSerial(uint8_t sample, int16_t touchX, int16_t touchY, int16_t touchZ) {
  Serial.printf("Nr sample: %2d | X = %4d | Y = %4d | Z = %4d\n", sample, touchX, touchY, touchZ);
}

void printCalibrationData(int32_t av_X_TL, int32_t av_Y_TL, int32_t av_X_BR, int32_t av_Y_BR) {
  Serial.println(F("--== Calibration Data ==--"));
  Serial.printf("x0 %4d x1 %4d y0 %4d y1 %4d\n", av_X_TL, av_X_BR, av_Y_TL, av_Y_BR);
  Serial.println(F("use this mapping:"));
  Serial.printf("x = map(p.x, %d, %d, 1, SCREEN_WIDTH);\n", av_X_TL, av_X_BR);
  Serial.printf("y = map(p.y, %d, %d, 1, SCREEN_HEIGHT);\n", av_Y_TL, av_Y_BR);
  Serial.println(F("--== Calibration Data End ==--"));
}

// this is throwing away the first point and building an average above the remaining points
void getTouchParameters(const uint8_t nrPoint, int16_t *touchX, int16_t *touchY, int16_t *touchZ) {
  int32_t tempX = 0;
  int32_t tempY = 0;
  int32_t tempZ = 0;
  int32_t avX[nrPoint], avY[nrPoint], avZ[nrPoint];
  TS_Point p;
  p = touchscreen.getPoint();  // throw away this point
  // collecting nrPoint data
  for (int i = 0; i < nrPoint; i++) {
    p = touchscreen.getPoint();
    avX[i] = p.x;
    avY[i] = p.y;
    //avZ[i] = p.z;
    if (i == 0) tempZ = p.z; // take the z value from the first touch only
    if (DEBUG) printTouchToSerial(200 + i, p.x, p.y, p.z);
    delay(10);
  }
  // get the sum
  for (int i = 0; i < nrPoint; i++) {
    tempX += avX[i];
    tempY += avY[i];
    //tempZ += avZ[i];
  }
  if (DEBUG) printTouchToSerial(201, tempX, tempY, tempZ);
  if (DEBUG) Serial.println(F("Averages"));
  if (DEBUG) printTouchToSerial(202, tempX / nrPoint, tempY / nrPoint, tempZ);
  // calculate the average
  *touchX = (int16_t) tempX / nrPoint;
  *touchY = (int16_t) tempY / nrPoint;
  *touchZ = (int16_t) tempZ;
}

void setup(void) {
  Serial.begin(115200);
  delay(1000);
  Serial.println(PROGRAM_VERSION);
  // Start the SPI for the touchscreen and init the touchscreen
  touchscreenSPI.begin(XPT2046_CLK, XPT2046_MISO, XPT2046_MOSI, XPT2046_CS);
  touchscreen.begin(touchscreenSPI);
  // Set the Touchscreen rotation in portrait mode
  touchscreen.setRotation(DISPLAY_PORTRAIT_MODE);

  tft.begin();
  tft.setRotation(DISPLAY_PORTRAIT_MODE);
  tft.fillScreen(TFT_BLACK);

  tft.setTextColor(TFT_BLACK, TFT_WHITE);
  tft.drawCentreString("Run Calibration", SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2 - 50, 4);
  //drawCalibrationCorners(15);

  // start with top left corner
  drawCalibrationArrowTopLeft(15);
  tft.drawCentreString("Touch in Top Left", SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2, 4);
  tft.drawCentreString("Corner and hold", SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2 + 50, 4);

  // wait to collect the samples for the top left corner
  bool isSamplesCollected = false;
  const uint8_t SAMPLES = 20;
  int valuesX[SAMPLES], valuesY[SAMPLES];  // collecting 20 samples
  uint8_t collectedSamples = 0;
  Serial.println(F("=== Collecting data for the top left corner ==="));
  while (!isSamplesCollected) {
    if (touchscreen.touched()) {
      // Get Touchscreen points
      getTouchParameters(4, &x, &y, &z);
      // collect data only if there is a real touch
      if (z > THRESHOLD_Z_CALIBRATION) {
        valuesX[collectedSamples] = x;
        valuesY[collectedSamples] = y;
        printTouchToSerial(collectedSamples, x, y, z);
        collectedSamples++;
        if (collectedSamples == SAMPLES) {
          isSamplesCollected = true;
        }
      }
    }
  }

  tft.fillScreen(TFT_BLACK);
  tft.setTextColor(TFT_BLACK, TFT_WHITE);
  tft.drawCentreString("Run Calibration", SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2 - 50, 4);
  //drawCalibrationCorners(15);
  tft.drawCentreString("STOP TOUCHING", SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2, 4);
  // calculate the averages
  for (int i = 0; i < SAMPLES; i++) {
    av_X_TL += valuesX[i];
    av_Y_TL += valuesY[i];
  }
  av_X_TL = av_X_TL / SAMPLES;
  av_Y_TL = av_Y_TL / SAMPLES;

  delay(2000);

  // now get the samples for the bottom right corner
  tft.fillScreen(TFT_BLACK);
  tft.setTextColor(TFT_BLACK, TFT_WHITE);
  tft.drawCentreString("Run Calibration", SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2 - 100, 4);
  // drawCalibrationCorners(15);
  drawCalibrationArrowBottomRight(15);
  tft.drawCentreString("Touch in", SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2 - 50, 4);
  tft.drawCentreString("Bottom Right", SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2, 4);
  tft.drawCentreString("Corner and hold", SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2 + 50, 4);

  // clean arrays
  for (int i = 0; i < SAMPLES; i++) {
    valuesX[i] = 0;
    valuesY[i] = 0;
  }
  collectedSamples = 0;
  isSamplesCollected = false;
  Serial.println(F("=== Collecting data for the bottom right corner ==="));
  while (!isSamplesCollected) {
    if (touchscreen.touched()) {
      // Get Touchscreen points
      getTouchParameters(4, &x, &y, &z);
      // collect data only if there is a real touch
      if (z > THRESHOLD_Z_CALIBRATION) {
        valuesX[collectedSamples] = x;
        valuesY[collectedSamples] = y;
        printTouchToSerial(collectedSamples, x, y, z);
        collectedSamples++;
        if (collectedSamples == SAMPLES) {
          isSamplesCollected = true;
        }
      }
    }
  }

  tft.fillScreen(TFT_BLACK);
  tft.setTextColor(TFT_BLACK, TFT_WHITE);
  tft.drawCentreString("Run Calibration", SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2 - 50, 4);
  //drawCalibrationCorners(15);
  tft.drawCentreString("STOP TOUCHING", SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2, 4);
  // calculate the averages
  for (int i = 0; i < SAMPLES; i++) {
    av_X_BR += valuesX[i];
    av_Y_BR += valuesY[i];
  }
  av_X_BR = av_X_BR / SAMPLES;
  av_Y_BR = av_Y_BR / SAMPLES;

  delay(2000);

  tft.fillScreen(TFT_BLACK);
  tft.setTextColor(TFT_BLACK, TFT_WHITE);
  tft.drawCentreString("Calibration done", SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2 - 50, 4);
  printCalibrationData(av_X_TL, av_Y_TL, av_X_BR, av_Y_BR);
  tft.drawCentreString("Test Touch", SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2, 4);
}

void loop() {

  if (touchscreen.touched()) {
    // Get Touchscreen points

    getTouchParameters(4, &x, &y, &z);
    if (z > THRESHOLD_Z) {
      if (DEBUG) Serial.print("Raw: ");
      if (DEBUG) printTouchToSerial(0, x, y, z);
      x = map(x, av_X_TL, av_X_BR, 1, SCREEN_WIDTH);
      y = map(y, av_Y_TL, av_Y_BR, 1, SCREEN_HEIGHT);

      if (DEBUG) Serial.print("Map: ");
      if (DEBUG) printTouchToSerial(0, x, y, z);
      // draw a small white circle at the position of the touch 
      tft.fillCircle(x, y, 2, TFT_RED);
      delay(20);
    }
  }
}
