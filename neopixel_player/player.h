#define __STDC_FORMAT_MACROS
#include <inttypes.h>

#define constrain(amt,low,high) ((amt)<(low)?(low):((amt)>(high)?(high):(amt)))

#define R 0
#define G 1
#define B 2
#define T 3

typedef int32_t FIXED;

#define FIXED_BITS        32
#define FIXED_WBITS       24
#define FIXED_FBITS       8
#define FIXED_TO_INT(a)   ((a) >> FIXED_FBITS)
#define FIXED_FROM_INT(a) (((FIXED) (a)) << FIXED_FBITS)
#define FIXED_MAKE(a)     (FIXED_FROM_INT(a))
#define FIXED_MUL(a,b)    (((a) * (b)) >> FIXED_FBITS)
#define FIXED_DIV(a,b)    (((a) << FIXED_FBITS) / (b))

typedef struct {
  uint16_t pixel;
  uint8_t (*data)[4];

  FIXED r;
  FIXED g;
  FIXED b;

  FIXED r0;
  FIXED g0;
  FIXED b0;

  FIXED r1;
  FIXED g1;
  FIXED b1;

  int index;
  FIXED t;
  FIXED elapsedMsec;
} PlayerData;

void playerInitData(PlayerData* pData, uint16_t pixel, uint8_t (*data)[4]);

uint8_t playerGetRed(PlayerData* pData);
uint8_t playerGetGreen(PlayerData* pData);
uint8_t playerGetBlue(PlayerData* pData);

void playerUpdate(PlayerData* pData, unsigned long dt);
void playerResetIfDone(PlayerData* pData);
