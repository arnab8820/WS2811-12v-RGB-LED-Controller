#include <Adafruit_NeoPixel.h>
#include "shared_vars.h"
#include "../fileSystem/fsHandler.h"

//led props
#define PIN 2   // input pin Neopixel is attached to
#define NUMPIXELS      1 // number of neopixels in strip

Adafruit_NeoPixel pixels = Adafruit_NeoPixel(NUMPIXELS, PIN, NEO_RGB + NEO_KHZ800);

void initLed(){
  pixels.begin();
}

void setLedColor(){
  int redMapped = map(red, 0, 255, 0, ceil(brightness*2.55));
  int blueMapped = map(blue, 0, 255, 0, ceil(brightness*2.55));
  int greenMapped = map(green, 0, 255, 0, ceil(brightness*2.55));
  Serial.printf("\nRed: %d, Green: %d, Blue: %d \n", redMapped, greenMapped, blueMapped);
  pixels.setPixelColor(0, pixels.Color(255 - redMapped, 255 - greenMapped, 255 - blueMapped));
  pixels.show();
}
