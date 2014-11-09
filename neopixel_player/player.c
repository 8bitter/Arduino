#include "player.h"

void playerInitData(PlayerData* pData, uint16_t pixel, uint8_t (*data)[4]) {
  pData->pixel = pixel;
  pData->data = data;

  pData->r = pData->r0 = FIXED_MAKE(data[0][R]);
  pData->g = pData->g0 = FIXED_MAKE(data[0][G]);
  pData->b = pData->b0 = FIXED_MAKE(data[0][B]);

  pData->r1 = FIXED_MAKE(data[1][R]);
  pData->g1 = FIXED_MAKE(data[1][G]);
  pData->b1 = FIXED_MAKE(data[1][B]);

  pData->t = FIXED_MAKE(data[1][T] * 10);

  pData->index = 1;
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
  if (pData->index == -1) {
    // end of data reached
    return;
  }
  
  pData->elapsedMsec += FIXED_MAKE(dt);

  if (pData->elapsedMsec < pData->t) {
    // interpolate
    FIXED tt = (pData->t == 0) ? FIXED_MAKE(1) : FIXED_DIV(pData->elapsedMsec, pData->t);
    pData->r = lerp(pData->r0, pData->r1, tt);
    pData->g = lerp(pData->g0, pData->g1, tt);
    pData->b = lerp(pData->b0, pData->b1, tt);
    
  } else {
    while (pData->elapsedMsec >= pData->t) {
      // advance index and see if end of data reached
      pData->index++;
      if (pData->data[pData->index][T] == (uint8_t) 255) {
        // set the final color
        pData->r = pData->r1;
        pData->g = pData->g1;
        pData->b = pData->b1;
	pData->index = -1;
	return;
      }

      pData->r = pData->r0 = pData->r1;
      pData->g = pData->g0 = pData->g1;
      pData->b = pData->b0 = pData->b1;

      pData->r1 = FIXED_MAKE(pData->data[pData->index][R]);
      pData->g1 = FIXED_MAKE(pData->data[pData->index][G]);
      pData->b1 = FIXED_MAKE(pData->data[pData->index][B]);

      pData->t = FIXED_MAKE(pData->data[pData->index][T] * 10);
      pData->elapsedMsec -= pData->t;
    }
  }
}

#if 0
int main() {
    uint8_t data[][4] = {
      { 255,   0,   0,   0 },
      {   0, 255, 128,  10 },
      { 255,   0,   0,  10 },
      {   0,   0,   0, 255 },
    };

    PlayerData player;
    playerInitData(&player, 0, data);

    int idx;
    for (idx = 0; idx < 100; ++idx) {
      playerUpdate(&player, 5);
      printf("idx=%d rgb=(%d, %d, %d)\n",
	     idx, playerGetRed(&player), playerGetGreen(&player), playerGetBlue(&player));
    }
    return 0;
}
#endif
