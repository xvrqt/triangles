#include "location.h"

/* Generates N approximately equidistant points on sphere, and pre
   computes the distance between them, storing the results in a vector.
   Artists ar point P choose the Pth index in the vector, which is 
   another vector of pairs containing the precomputed distances between 
   every other point, and the index of the point.

   This must be done before constructing artists.
 */
std::vector<std::vector<size_t>> getLocationLikelihoodMap(size_t POPULATION_SIZE)
{
  /* What we'll be returning */
  std::vector<std::vector<size_t>> location_map;
  location_map.resize(POPULATION_SIZE);

  int sq_root = std::floor(std::sqrt(POPULATION_SIZE));
  int half = sq_root / 2;

  for(size_t i = 0; i != POPULATION_SIZE; i++)
  {
    for(int j = 0; j < half; j++)
    {
      int idx = (j + i - half);
      if(idx < 0) { idx += POPULATION_SIZE; }

      location_map[i].push_back(idx);
    }

    for(int j = 0; j < half; j++)
    {
      int idx = (j + i + 1) % POPULATION_SIZE;

      location_map[i].push_back(idx);
    }
  }
    
  return location_map;
}
