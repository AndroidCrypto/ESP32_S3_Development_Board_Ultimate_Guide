// -------------------------------------------------------------------------------
// Potentiometer
// Note: this does not require any additional libraries

// Note: don't use pin 1 when having a TFT display with touch surface attached as
// the touch driver uses pin 1 as well, use e.g. pin 4 instead by wiring a jumper cable
#define POTENTIOMETER_PIN 1 // Digital pin connected to the Potentiometer
// GPIO 1 = ADC 1 Channel 0
//#define POTENTIOMETER_PIN 4 // Digital pin connected to the Potentiometer
// GPIO 4 = ADC 1 Channel 3

// general note on using ADC channels: don't use the ADC 2 when Wi-Fi is included in
// your sketch as this would break your measurement.

int potValue = 0;
float potMilliVolts = 0.0;

int getPotentiometerValue() {
  potValue = analogRead(POTENTIOMETER_PIN);
  return potValue;
}

float getPotentiometerVoltsValue() {
  potMilliVolts = map(potValue, 0, 4095, 0, 3300);
  return potMilliVolts / 1000.0;
}