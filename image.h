#ifndef IMAGE_H 
#define IMAGE_H

#include <Magick++.h> 

#include <iostream>
#include <string>

#include <stdio.h>
#include <stdlib.h>

#define MAX_PNG_HEIGHT 1024
#define MAX_PNG_WIDTH 1024

/* Open an image given a file path. */
Magick::Image openImage(const char * filename);

#endif
