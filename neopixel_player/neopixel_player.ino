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

Note NEO_0[] = {
  { 0, 255,   0,   0,    0 },
  { 0, 255,   0,   0, 2500 },
  { 0,   0, 255,   0, 2500 },
  { 0,   0,   0, 255, 2500 },
  { 0,   0,   0,   0, 2500 },
  { 0,   0,   0,   0,   -1 },
};

Note NEO_1[] = {
  { 0, 128, 128,   0,    0 },
  { 0,   0, 128, 128, 2500 },
  { 0, 128, 128,   0, 2500 },
  { 0,   0, 128, 128, 2500 },
  { 0, 128, 128,   0, 2500 },
  { 0,   0, 128, 128,   -1 },
};

Note NEO_2[] = {
  { 0,   0, 255,   0,    0 },
  { 0,   0,   0, 255, 2500 },
  { 0,   0, 255,   0, 2500 },
  { 0,   0,   0, 255, 2500 },
  { 0,   0, 255,   0, 2500 },
  { 0,   0,   0,   0,   -1 },
};

Note NEO_3[] = {
  { 0, 255, 255,   0,    0 },
  { 0, 255,   0, 255, 2500 },
  { 0, 255, 255,   0, 2500 },
  { 0, 255,   0, 255, 2500 },
  { 0, 255, 255,   0, 2500 },
  { 0, 255,   0,   0,   -1 },
};

PlayerData players[4];
unsigned long timestamp;
    
#define PLAYERDATA_END (players + sizeof(players) / sizeof(*players))

void setup() {
  // Initialize all pixels to 'off'
  strip.begin();
  strip.clear();
  strip.show();
  
  playerInitData(&players[0], 0, NEO_0);
  playerInitData(&players[1], 1, NEO_1);
  playerInitData(&players[2], 2, NEO_2);
  playerInitData(&players[3], 3, NEO_3);
  timestamp = millis();
}

void loop() {
  unsigned long now = millis();
  unsigned long dt = now - timestamp;
  timestamp = now;

  for (PlayerData* p = players; p < PLAYERDATA_END; ++p) {
    playerUpdate(p, dt);
    strip.setPixelColor(
        p->pixel,
        playerGetRed(p),
        playerGetGreen(p),
        playerGetBlue(p));

    playerResetIfDone(p);
  }
  strip.show();
}
