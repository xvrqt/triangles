#ifndef TRIANGLE_H
#define TRIANGLE_H

#include <cstdlib>

/*
[xxxxxxxxxx][yyyyyyyyyy][xxxxxxxxxx][yyyyyyyyyy][xxxxxxxxxx][yyyyyyyyyy]|[vvvv]|[rrrrrrrr][gggggggg][bbbbbbbb]|[aaaaaaaa]
[                                points                                ]|[show]|[           colors           ]|[opacity ]

Total size: 12 bytes (3 bits unsued at the end)
Packed to preserve alignment.
*/

struct __attribute__((__packed__)) Triangle {
  /* First Point */
  uint8_t x1 : 8;
  uint8_t y1 : 8;
  
  /* Second Point */
  uint8_t x2 : 8;
  uint8_t y2 : 8;

  /* Third Point */
  uint8_t x3 : 8;
  uint8_t y3 : 8;

  /* Color */
  uint8_t r : 8;
  uint8_t g : 8;
  uint8_t b : 8;
  uint8_t a : 8;

  /* If visible */
  uint8_t visible : 8;
};

/* Mark important offsets */
extern size_t BG_COLOR_OFFSET;
extern size_t BG_COLOR_SIZE;
extern size_t TRIANGLE_LIST_BEGIN;

/* Compare two triangles */
bool operator==(const Triangle& lhs, const Triangle& rhs);


#endif
