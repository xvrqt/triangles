#ifndef TRIANGLE_H
#define TRIANGLE_H

#include <cstdlib>

/*
     8         8         8          8          10          10          10          10          10          10
[rrrrrrrr][gggggggg][bbbbbbbb]|[aaaaaaaa]|[xxxxxxxxxx][yyyyyyyyyy][xxxxxxxxxx][yyyyyyyyyy][xxxxxxxxxx][yyyyyyyyyy][v][øøø]
[           colors           ]|[opacity ]|[                                points                                ][show][ø]

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

  /* If the triangle is visible or not */
  bool visible : 1;

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

/* Generates an array of random bits the same size as N triangles. Sets the
   visible bit to 0 except for the first triangle.
 */
Triangle * generate_random_triangle_array(size_t n); /* [ delete me ] */


#endif
