#ifndef TRIANGLE_H
#define TRIANGLE_H

struct Point {
  unsigned short x = 0;
  unsigned short y = 0;
};

struct Edge {
  int A = 0;
  int B = 0;
  int C = 0;
};

/*
     8         8         8          8          10          10          10          10          10          10
[rrrrrrrr][gggggggg][bbbbbbbb]|[aaaaaaaa]|[xxxxxxxxxx][yyyyyyyyyy][xxxxxxxxxx][yyyyyyyyyy][xxxxxxxxxx][yyyyyyyyyy][v][øøø]
[           colors           ]|[opacity ]|[                                points                                ][show][ø]

Total size: 12 bytes (3 bits unsued at the end)
Packed to preserve alignment.
*/

struct __attribute__((__packed__)) Triangle {
  /* Color */
  unsigned char r : 8;
  unsigned char g : 8;
  unsigned char b : 8;
  unsigned char a : 8;

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
};


#endif
