

// -------------------------------------------------------------------------------
// RGB LED
// This is the RGB LED on the ESP32-S3 Development Board
#include <FastLED.h>  // https://github.com/FastLED/FastLED FastLED version: 3.9.15
#define RGB_LED_PIN 48
#define RGB_NUM_LEDS 1
#define LED_TYPE WS2811
#define COLOR_ORDER GRB
#define RGB_LED_BRIGHTNESS_PERCENT 30
CRGB leds[RGB_NUM_LEDS];
#define FRAMES_PER_SECOND 120  // rainbow effect
uint8_t gHue = 0;              // rotating "base color" used by many of the patterns

void initRgbLed() {
  // setup the RGB LED
  CFastLED::addLeds<LED_TYPE, RGB_LED_PIN, COLOR_ORDER>(leds, RGB_NUM_LEDS);
  FastLED.setBrightness(RGB_LED_BRIGHTNESS_PERCENT);
  Serial.println(F("initRgbLed is done"));
}

void showRgbRainbow() {
  // Rainbow effect
  // send the 'leds' array out to the actual LED strip
  // call in loop() without delays
  FastLED.show();
  // insert a delay to keep the framerate modest
  FastLED.delay(1000 / FRAMES_PER_SECOND);

  // do some periodic updates
  EVERY_N_MILLISECONDS(20) {
    gHue++;
  }  // slowly cycle the "base color" through the rainbow
  fill_rainbow(&(leds[0]), RGB_NUM_LEDS, gHue);
}

void showRgbLedBlack() {
  leds[0] = CRGB::Black;
  FastLED.delay(500);
}

void showRgbLedRed() {
  leds[0] = CRGB::Red;
  FastLED.delay(500);
}

void showRgbLedGreen() {
  leds[0] = CRGB::Green;
  FastLED.delay(500);
}

void showRgbLedBlue() {
  leds[0] = CRGB::Blue;
  FastLED.delay(500);
}

void showRgbLedWhite() {
  leds[0] = CRGB::White;
  FastLED.delay(500);
}
