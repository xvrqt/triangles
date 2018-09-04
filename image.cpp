#include "image.h"

/* Take the name of a PNG and returns an array of floats in RGB order */
Magick::Image openImage(const char * filename)
{
  /* Image dimensions */
  size_t width;
  size_t height;

  /* Store the image in the respecitve formats */
  Magick::Image image;
  try { 
    image.read(filename);
  } 
  catch(Magick::Exception &error) 
  { 
      std::cerr << "Caught exception: " << error.what() << std::endl; 
      exit(1); 
  } 

  width = image.columns();
  height = image.rows();
  if(width > MAX_PNG_WIDTH || height > MAX_PNG_HEIGHT)
  {
    std::cout << "Image dimensions must be between " << MAX_PNG_WIDTH << "x" << MAX_PNG_HEIGHT << "\n";
    std::cout << filename << ": " << width << "x" << height << std::endl;

    image.resize("1024>x1024>");

    width = image.columns();
    height = image.rows();

    std::cout << "Resizing image to: " << width << "x" << height << std::endl;
  }

  return image;
}