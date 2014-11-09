#include <Adafruit_NeoPixel.h>

extern "C" {
  #include "player.h"
}

#define PIN 1

#define NEOPIXEL_COUNT 4


// Parameter 1 = number of pixels in strip
// Parameter 2 = pin number (most are valid)
// Parameter 3 = pixel type flags, add together as needed:
//   NEO_KHZ800  800 KHz bitstream (most NeoPixel products w/WS2812 LEDs)
//   NEO_KHZ400  400 KHz (classic 'v1' (not v2) FLORA pixels, WS2811 drivers)
//   NEO_GRB     Pixels are wired for GRB bitstream (most NeoPixel products)
//   NEO_RGB     Pixels are wired for RGB bitstream (v1 FLORA pixels, not v2)
Adafruit_NeoPixel strip = Adafruit_NeoPixel(NEOPIXEL_COUNT, PIN, NEO_GRB + NEO_KHZ800);

uint8_t NEO_0[][4] = {
  { (uint8_t) 255, (uint8_t)   0, (uint8_t)   0, (uint8_t)   0 },
  { (uint8_t) 255, (uint8_t)   0, (uint8_t)   0, (uint8_t) 250 },
  { (uint8_t)   0, (uint8_t) 255, (uint8_t)   0, (uint8_t) 250 },
  { (uint8_t)   0, (uint8_t)   0, (uint8_t) 255, (uint8_t) 250 },
  { (uint8_t)   0, (uint8_t)   0, (uint8_t)   0, (uint8_t)   0 },
  { (uint8_t)   0, (uint8_t)   0, (uint8_t)   0, (uint8_t) 255 },
};

uint8_t NEO_1[][4] = {
  { (uint8_t) 128, (uint8_t) 128, (uint8_t)   0, (uint8_t)   0 },
  { (uint8_t)   0, (uint8_t) 128, (uint8_t) 128, (uint8_t) 250 },
  { (uint8_t) 128, (uint8_t) 128, (uint8_t)   0, (uint8_t) 250 },
  { (uint8_t)   0, (uint8_t) 128, (uint8_t) 128, (uint8_t) 250 },
  { (uint8_t) 128, (uint8_t) 128, (uint8_t)   0, (uint8_t) 250 },
  { (uint8_t)   0, (uint8_t) 128, (uint8_t) 128, (uint8_t) 255 },
};

uint8_t NEO_2[][4] = {
  { (uint8_t)   0, (uint8_t) 255, (uint8_t)   0, (uint8_t)   0 },
  { (uint8_t)   0, (uint8_t)   0, (uint8_t) 255, (uint8_t) 250 },
  { (uint8_t)   0, (uint8_t) 255, (uint8_t)   0, (uint8_t) 250 },
  { (uint8_t)   0, (uint8_t)   0, (uint8_t) 255, (uint8_t) 250 },
  { (uint8_t)   0, (uint8_t) 255, (uint8_t)   0, (uint8_t) 250 },
  { (uint8_t)   0, (uint8_t)   0, (uint8_t)   0, (uint8_t) 255 },
};

uint8_t NEO_3[][4] = {
  { (uint8_t) 255, (uint8_t) 255, (uint8_t)   0, (uint8_t)   0 },
  { (uint8_t) 255, (uint8_t)   0, (uint8_t) 255, (uint8_t) 250 },
  { (uint8_t) 255, (uint8_t) 255, (uint8_t)   0, (uint8_t) 250 },
  { (uint8_t) 255, (uint8_t)   0, (uint8_t) 255, (uint8_t) 250 },
  { (uint8_t) 255, (uint8_t) 255, (uint8_t)   0, (uint8_t) 250 },
  { (uint8_t) 255, (uint8_t)   0, (uint8_t)   0, (uint8_t) 255 },
};

PlayerData players[4];
unsigned long timestamp;
    
void setup() {
  strip.begin();
  strip.show(); // Initialize all pixels to 'off'
  
  playerInitData(&players[0], 0, NEO_0);
  playerInitData(&players[1], 0, NEO_1);
  playerInitData(&players[2], 0, NEO_2);
  playerInitData(&players[3], 0, NEO_3);
  timestamp = millis();
}

void loop() {
  unsigned long now = millis();
  unsigned long dt = now - timestamp;
  timestamp = now;

  PlayerData* p = players;
  for (uint16_t pixel = 0; pixel < strip.numPixels(); ++pixel, ++p) {
    playerUpdate(p, dt);
    strip.setPixelColor(
        pixel,
        playerGetRed(p),
        playerGetGreen(p),
        playerGetBlue(p));

    playerResetIfDone(p);
  }
  strip.show();
}
