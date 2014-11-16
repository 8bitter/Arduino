#ifdef ARDUINO
  #define dprintf
#else
  #include <stdio.h>
  #define dprintf printf
#endif

#include "player.h"

void playerInitData(PlayerData* pData, uint16_t pixel, Note* pNote) {
  Note note;
  
  pData->pixel = pixel;
  pData->pNotes = pData->pNote = pNote;

  memcpy_P(&note, pData->pNote, sizeof(note));

  pData->r = pData->r0 = FIXED_MAKE(note.r);
  pData->g = pData->g0 = FIXED_MAKE(note.g);
  pData->b = pData->b0 = FIXED_MAKE(note.b);

  ++pData->pNote;
  memcpy_P(&note, pData->pNote, sizeof(note));

  pData->r1 = FIXED_MAKE(note.r);
  pData->g1 = FIXED_MAKE(note.g);
  pData->b1 = FIXED_MAKE(note.b);
  pData->t = FIXED_MAKE(note.msec);

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
  Note note;
  
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
      memcpy_P(&note, pData->pNote, sizeof(note));

      if (note.msec < 0) {
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

      pData->r1 = FIXED_MAKE(note.r);
      pData->g1 = FIXED_MAKE(note.g);
      pData->b1 = FIXED_MAKE(note.b);

      pData->t = FIXED_MAKE(note.msec);
      pData->elapsedMsec -= pData->t;
    }
  }
}

void playerResetIfDone(PlayerData* pData) {
  if (pData->pNote == 0) {
    pData->pNote = pData->pNotes;
  }
}


#if 0
#include <stdio.h>

int main() {
  Note notes[] = {
    { 255, 255,   0,   0,   0 },
    { 255,   0, 255, 128,  15 },
    { 255, 255,   0,   0,  15 },
    { 255,   0, 255, 128,  15 },
    { 255, 255,   0,   0,  -1 },
  };

  PlayerData player;
  playerInitData(&player, 0, notes);

  int idx;
  for (idx = 0; idx < 30; ++idx) {
    playerUpdate(&player, 5);
    printf(
        "%3d: pixel=%d rgb=(%3d,%3d,%3d) pNote=%ld\n",
        idx,
        player.pixel,
        playerGetRed(&player),
        playerGetGreen(&player),
        playerGetBlue(&player),
        player.pNote == 0 ? -1 : player.pNote - player.pNotes);
    playerResetIfDone(&player);
  }
  return 0;
}
#endif
