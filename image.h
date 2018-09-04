#ifndef IMAGE_H 
#define IMAGE_H

#include "lodepng.h"

#include <iostream>
#include <float.h>
#include <stdio.h>
#include <stdlib.h>

#define MAX_PNG_HEIGHT 1024
#define MAX_PNG_WIDTH 1024

/* RGB, 256 bit color encoding per pixel */
struct RGB {
  unsigned char r = 0;
  unsigned char g = 0;
  unsigned char b = 0;
};

struct Image {
  size_t width = 0;
  size_t height = 0;
  RGB * data = 0;
};

/* Take the name of a PNG and returns an array of 8-bit pixels in RGB order */
Image openPNG(const char * filename);

/* Save an Image as a PNG */
void savePNG(const char * filename, Image image);

#endif
