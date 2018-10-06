#include "location.h"

/* Creates a table where the index of the first vecotor indicates the position
   on the "map." The second vector contains all places adjacent to that point 
   on the "map."
*/
std::vector<std::vector<size_t>> getAdjacencyMatrix(size_t POPULATION_SIZE, uint8_t SIMULATE_LOCATION)
{
  /* What we'll be returning */
  std::vector<std::vector<size_t>> location_map;
  location_map.resize(POPULATION_SIZE);

  unsigned int half = SIMULATE_LOCATION / 2;
  unsigned int second_half;

  if(SIMULATE_LOCATION % 2) { second_half = (half + 1); }
  else { second_half = half; }

  for(size_t i = 0; i != POPULATION_SIZE; i++)
  {
    for(int j = 0; j < half; j++)
    {
      int idx = (j + i - half);
      if(idx < 0) { idx += POPULATION_SIZE; }

      location_map[i].push_back(idx);
    }

    for(int j = 0; j < second_half; j++)
    {
      int idx = (j + i + 1) % POPULATION_SIZE;

      location_map[i].push_back(idx);
    }
  }
    
  return location_map;
}

/* Takes a vector of artists, an adjacency matrix and modifies a vector of 
   artists pointers such that the artist at location N is at index N.
 */
void computeArtistLocation( std::vector<Artist *> & artists, 
                            std::vector<std::vector<size_t>> const & adjacency_matrix,
                            std::vector<Artist *> & artist_locations)
{
  /* What we'll be returning. Fill with void pointers. */
  artist_locations.resize(artists.size());
  std::fill(artist_locations.begin(), artist_locations.end(), (Artist *)NULL);

  /* Find each artist a location on the map */
  for(auto a = artists.begin(); a != artists.end(); ++a)
  {
    /* The artist's last location (and current preference) */
    size_t location_index = (*a)->getLocationIndex();
        
    /* If the location is available - add the Artist there */
    if(artist_locations[location_index] == NULL)
    {
      artist_locations[location_index] = (*a);
    }
    else /* Find a new location based on the precomputed map. */
    {
      bool location_found = false;
      /* Try locations immediately adjacent */
      for(int i = 0; i < POPULATION_SIZE; i++)
      {
        if(location_found) { break; }

        /* Start at the artist's current location, if this fails, try locations
           +/- 1, +/- 2, etc... as these are the closest to this location.
         */
        int idx = (i % 2) ? (i * -1) : i;
        idx = idx - (idx / 2);

        idx = (location_index + idx) % POPULATION_SIZE;
        if(idx < 0) { idx = POPULATION_SIZE + idx; }

        /* Get a list of locations adjacent to this location */
        auto v = adjacency_matrix[idx];
        
        /* See if any adjacent locations are open */
        for(auto loc = v.begin(); loc != v.end(); ++loc)
        {
          size_t new_location_index = (*loc);
          if(artist_locations[new_location_index] == NULL)
          {
            artist_locations[new_location_index] = (*a);
            (*a)->setLocationIndex(new_location_index);
            location_found = true;
            break;
          }
        }
      }
    }
  }
}