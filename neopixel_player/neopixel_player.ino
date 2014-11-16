#include <Adafruit_NeoPixel.h>

extern "C" {
  #include "player.h"
}

#define PIN 1

#define NEOPIXEL_COUNT 4

#if 0
#include <stdarg.h>

void dprintf(char *fmt, ... ) {
  char buf[128]; // resulting string limited to 128 chars
  va_list args;
  va_start(args, fmt );
  vsnprintf(buf, 128, fmt, args);
  va_end(args);
  Serial.print(buf);
}
#endif

// Parameter 1 = number of pixels in strip
// Parameter 2 = pin number (most are valid)
// Parameter 3 = pixel type flags, add together as needed:
//   NEO_KHZ800  800 KHz bitstream (most NeoPixel products w/WS2812 LEDs)
//   NEO_KHZ400  400 KHz (classic 'v1' (not v2) FLORA pixels, WS2811 drivers)
//   NEO_GRB     Pixels are wired for GRB bitstream (most NeoPixel products)
//   NEO_RGB     Pixels are wired for RGB bitstream (v1 FLORA pixels, not v2)
Adafruit_NeoPixel strip = Adafruit_NeoPixel(NEOPIXEL_COUNT, PIN, NEO_GRB + NEO_KHZ800);

PROGMEM Note NEO_0[] = {
  { 255, 255,   0,   0,   0 },
  { 255, 255,   0,   0, 250 },
  { 255,   0, 255,   0, 250 },
  { 255,   0,   0, 255, 250 },
  { 255,   0,   0,   0,   0 },
  { 255,   0,   0,   0,  -1 },
};

PROGMEM Note NEO_1[] = {
  { 255, 128, 128,   0,   0 },
  { 255,   0, 128, 128, 250 },
  { 255, 128, 128,   0, 250 },
  { 255,   0, 128, 128, 250 },
  { 255, 128, 128,   0, 250 },
  { 255,   0, 128, 128,  -1 },
};

PROGMEM Note NEO_2[] = {
  { 255,   0, 255,   0,   0 },
  { 255,   0,   0, 255, 250 },
  { 255,   0, 255,   0, 250 },
  { 255,   0,   0, 255, 250 },
  { 255,   0, 255,   0, 250 },
  { 255,   0,   0,   0,  -1 },
};

PROGMEM Note NEO_3[] = {
  { 255, 255, 255,   0,   0 },
  { 255, 255,   0, 255, 250 },
  { 255, 255, 255,   0, 250 },
  { 255, 255,   0, 255, 250 },
  { 255, 255, 255,   0, 250 },
  { 255, 255,   0,   0,  -1 },
};

PlayerData players[4];
#define PLAYERDATA_END (players + sizeof(players) / sizeof(*players))

unsigned long timestamp;

void setup() {
  timestamp = millis();
  playerInitData(&players[0], 0, NEO_0);
  playerInitData(&players[1], 1, NEO_1);
  playerInitData(&players[2], 2, NEO_2);
  playerInitData(&players[3], 3, NEO_3);

  strip.begin();
  strip.clear();
  strip.show(); // Initialize all pixels to 'off'  
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
