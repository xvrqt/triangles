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
  unsigned short x1 : 10;
  unsigned short y1 : 10;
  
  /* Second Point */
  unsigned short x2 : 10;
  unsigned short y2 : 10;

  /* Third Point */
  unsigned short x3 : 10;
  unsigned short y3 : 10;

  /* The triangle with the higher visible value is drawn. A value of 0 is not 
     drawn. Equal values are broken in a consistent manner.
   */
  unsigned short visible : 4;

  /* Align to the next byte, don't split so the chars are easier to read */
  unsigned : 0;

  /* Color */
  unsigned char r : 8;
  unsigned char g : 8;
  unsigned char b : 8;
  unsigned char a : 8;
};

/* Mark important offsets */
extern size_t BG_COLOR_OFFSET;
extern size_t BG_COLOR_SIZE;
extern size_t TRIANGLE_LIST_BEGIN;
extern size_t TRIANGLE_SIZE;

/* Compare two triangles */
bool operator==(const Triangle& lhs, const Triangle& rhs);


#endif
