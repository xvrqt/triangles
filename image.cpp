#include "image.h"

/* Take the name of a PNG and returns an array of floats in RGB order */
Image openPNG(const char * filename)
{
  /* Image dimensions */
  unsigned int width;
  unsigned int height;
  
  /* Store the image in the respecitve formats */
  Image image;
  std::vector<unsigned char> lode_png_image;

  /* Decode the image */
  unsigned error = lodepng::decode(lode_png_image, width, height, filename);

  /* Error and Dimension checking */
  if(error)
  { 
    fprintf(stderr, "Decoding Error %u: %s\n", error, lodepng_error_text(error));
    exit(1);
  }
  else if(width > MAX_PNG_WIDTH || height > MAX_PNG_HEIGHT)
  {
    fprintf(stderr, "Image dimensions must be between %ix%i\n %s: %ux%u", MAX_PNG_WIDTH, MAX_PNG_HEIGHT, filename, width, height);
    exit(1);
  }
  else
  {
    image.width = width;
    image.height = height;
    image.data = (RGB *)malloc(sizeof(RGB) * width * height);
    
    auto it = lode_png_image.begin();
    for(size_t i = 0; i < width * height; i++)
    {
      image.data[i].r = (*it++);
      image.data[i].g = (*it++);
      image.data[i].b = (*it++);
      (*it++); /* Skip the opacity layer */
    }
  }

  return image;
}

/* Given an Image struct, saves it as a PNG file */
void savePNG(const char * filename, Image image)
{
  size_t width = image.width;
  size_t height = image.height;
  std::vector<unsigned char> lode_png_image;

  /* Cycle through the array of RGB structs */
  for(size_t i = 0; i < width * height; i++)
  {
    lode_png_image.push_back(image.data[i].r);
    lode_png_image.push_back(image.data[i].g);
    lode_png_image.push_back(image.data[i].b);
    lode_png_image.push_back(255); /* Opacity */
  }
  unsigned error = lodepng::encode(filename, lode_png_image, width, height);
  if(error) printf("Encoding Error %u: %s\n", error, lodepng_error_text(error));
}