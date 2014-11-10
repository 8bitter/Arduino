#ifdef ARDUINO
  #define dprintf
#else
  #include <stdio.h>
  #define dprintf printf
#endif

#include "player.h"

void playerInitData(PlayerData* pData, uint16_t pixel, Note* pNote) {
  pData->pixel = pixel;
  pData->pOrigNote = pNote;
  pData->pNote = pNote;

  pData->r = pData->r0 = FIXED_MAKE(pData->pNote->r);
  pData->g = pData->g0 = FIXED_MAKE(pData->pNote->g);
  pData->b = pData->b0 = FIXED_MAKE(pData->pNote->b);

  ++pData->pNote;
  pData->r1 = FIXED_MAKE(pData->pNote->r);
  pData->g1 = FIXED_MAKE(pData->pNote->g);
  pData->b1 = FIXED_MAKE(pData->pNote->b);

  pData->t = FIXED_MAKE(pData->pNote->msec);
  pData->elapsedMsec = 0;
}

uint8_t playerGetRed(PlayerData* pData) {
  return (uint8_t) constrain(FIXED_TO_INT(pData->r), 0, 255);
}

uint8_t playerGetGreen(PlayerData* pData) {
  return (uint8_t) constrain(FIXED_TO_INT(pData->g), 0, 255);
}

uint8_t playerGetBlue(PlayerData* pData) {
  return (uint8_t) constrain(FIXED_TO_INT(pData->b), 0, 255);
}

FIXED lerp(FIXED v0, FIXED v1, FIXED t) {
  return FIXED_MUL((FIXED_MAKE(1) - t), v0) + FIXED_MUL(t, v1);
}

void playerUpdate(PlayerData* pData, unsigned long dt) {
  if (pData->pNote == 0) {
    // end of data reached
    return;
  }
  
  pData->elapsedMsec += FIXED_MAKE(dt);

  if (pData->elapsedMsec < pData->t) {
    dprintf(
        "interpolate: elapsedMsec=%d t=%d\n",
        FIXED_TO_INT(pData->elapsedMsec), FIXED_TO_INT(pData->t));
    // interpolate
    FIXED tt = (pData->t == 0) ? FIXED_MAKE(1) : FIXED_DIV(pData->elapsedMsec, pData->t);
    pData->r = lerp(pData->r0, pData->r1, tt);
    pData->g = lerp(pData->g0, pData->g1, tt);
    pData->b = lerp(pData->b0, pData->b1, tt);
    
  } else {
    while (pData->elapsedMsec >= pData->t) {
      // advance index and see if end of data reached
      ++pData->pNote;

      dprintf(
          "----------------: elapsedMsec=%d t=%d msec=%d\n",
          FIXED_TO_INT(pData->elapsedMsec),
          FIXED_TO_INT(pData->t),
          pData->pNote->msec);

      if (pData->pNote->msec < 0) {
        // set the final color
        pData->r = pData->r1;
        pData->g = pData->g1;
        pData->b = pData->b1;
        pData->pNote = 0;
        return;
      }

      pData->r = pData->r0 = pData->r1;
      pData->g = pData->g0 = pData->g1;
      pData->b = pData->b0 = pData->b1;

      pData->r1 = FIXED_MAKE(pData->pNote->r);
      pData->g1 = FIXED_MAKE(pData->pNote->g);
      pData->b1 = FIXED_MAKE(pData->pNote->b);

      pData->t = FIXED_MAKE(pData->pNote->msec);
      pData->elapsedMsec -= pData->t;
    }
  }
}

void playerResetIfDone(PlayerData* pData) {
  if (pData->pNote == 0) {
    pData->pNote = pData->pOrigNote;
  }
}

#ifndef ARDUINO
int main() {
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

  playerInitData(players + 0, 0, NEO_0);
  playerInitData(players + 1, 1, NEO_1);
  playerInitData(players + 2, 2, NEO_2);
  playerInitData(players + 3, 3, NEO_3);

  timestamp = 0;

  int loop;
  for (loop = 0; loop < 200; ++loop) {
    unsigned long now = loop * 100;
    unsigned long dt = now - timestamp;
    timestamp = now;

    PlayerData* p;
    for (p = players; p < PLAYERDATA_END; ++p) {
      playerUpdate(p, dt);
      dprintf("%d: rgb=(%d, %d, %d) pNote=%08lx pOrigNote=%08lx\n",
              p->pixel, playerGetRed(p), playerGetGreen(p), playerGetBlue(p), p->pNote, p->pOrigNote);
      playerResetIfDone(p);
    }
  }

  return 0;
}
#endif
