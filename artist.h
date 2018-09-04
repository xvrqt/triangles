#ifndef ARTIST_H
#define ARTIST_H

#include "image.h"
#include "triangle.h"

#include <vector>
#include <climits>
#include <cstdlib>

#define NUM_TRIANGLES 100
#define NUM_ATTEMPTS 10
#define BACKGROUND_COLOR 255

class Artist
{
  static Image original;
  static size_t num_shapes;

  /* This artist's closeness to the original. Lower is better. */
  unsigned long distance = ULONG_MAX;

  /* This is where this artist will draw to */
  std::vector<RGB> canvas;

  /* This is a list of triangles that make up the artist's painting */
  std::vector<Triangle> triangles;

  /* This is the index of the triangle to mutate */
  size_t triangle_index = 0;

    public:
    
      static void setNumberOfShapes(size_t num);
      static void setOriginalImage(Image original);
      /* Combines the genetic material of two artists */
      static void crossover(Artist &a, Artist &b);

      Artist();
      Artist(std::vector<Triangle> &protoype);
      Artist(const Artist &);
      ~Artist();

      /* Returns the artist's distance */
      unsigned long getDistance() const;

      /* Attempts to improve the score through random mutation */
      void mutate();

      /* Calculates the image's distance */
      void calcDistance();

      /* Returns a copy Image of the artist's best rendition */
      Image getImageCopy();

      /* Returns a copy of a Triangle at the specified index */
      Triangle getTriangle(size_t index);
      /* Sets a triangle at a specific index */
      void setTriangle(size_t index, Triangle t);

      /* Allows us to sort without copying */
      bool operator <(const Artist &a) const;
};

#endif
