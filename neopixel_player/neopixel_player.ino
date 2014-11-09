#include <Adafruit_NeoPixel.h>

extern "C" {
  #include "player.h"
}

#define PIN 1

// Parameter 1 = number of pixels in strip
// Parameter 2 = pin number (most are valid)
// Parameter 3 = pixel type flags, add together as needed:
//   NEO_KHZ800  800 KHz bitstream (most NeoPixel products w/WS2812 LEDs)
//   NEO_KHZ400  400 KHz (classic 'v1' (not v2) FLORA pixels, WS2811 drivers)
//   NEO_GRB     Pixels are wired for GRB bitstream (most NeoPixel products)
//   NEO_RGB     Pixels are wired for RGB bitstream (v1 FLORA pixels, not v2)
Adafruit_NeoPixel strip = Adafruit_NeoPixel(2, PIN, NEO_GRB + NEO_KHZ800);

uint8_t data[][4] = {
  { (uint8_t) 255, (uint8_t)   0, (uint8_t)   0, (uint8_t)   0 },
  { (uint8_t) 255, (uint8_t)   0, (uint8_t)   0, (uint8_t) 250 },
  { (uint8_t)   0, (uint8_t) 255, (uint8_t)   0, (uint8_t) 250 },
  { (uint8_t)   0, (uint8_t)   0, (uint8_t) 255, (uint8_t) 250 },
  { (uint8_t)   0, (uint8_t)   0, (uint8_t)   0, (uint8_t)   0 },
  { (uint8_t)   0, (uint8_t)   0, (uint8_t)   0, (uint8_t) 255 },
};

PlayerData p;
unsigned long timestamp;
    
void setup() {
  strip.begin();
  strip.show(); // Initialize all pixels to 'off'
  
  playerInitData(&p, 0, data);
  timestamp = millis();
}

void loop() {
  unsigned long now = millis();
  unsigned long dt = now - timestamp;
  timestamp = now;
  
  playerUpdate(&p, dt);
  for (uint16_t pixel = 0; pixel < strip.numPixels(); ++pixel) {
    strip.setPixelColor(pixel, playerGetRed(&p), playerGetGreen(&p), playerGetBlue(&p));
  }
  strip.show();
}
