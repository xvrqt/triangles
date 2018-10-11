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

  int half = SIMULATE_LOCATION / 2;
  int second_half;

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
      /* Keep track of where we've already tried */
      std::vector<bool> tried_locations(POPULATION_SIZE, false);
      tried_locations[location_index] = true;

      /* Add adjacent locations to the queue of locations to try. */
      std::queue<size_t> untried_locations;
      auto ami = adjacency_matrix[location_index];
      for(auto & location : ami) { untried_locations.push(location); }

      /* While we still have locations to try... */
      while(!(untried_locations.empty()))
      {
        size_t location = untried_locations.front();
        if(artist_locations[location] == NULL)
        {
          artist_locations[location] = (*a);
          (*a)->setLocationIndex(location);
          break;
        }
        else /* Add items adjacent to this location to the queue. */
        {
          auto ami = adjacency_matrix[location];
          for(auto & loc : ami) 
          { 
            /* Only add a location if it's not already in the queue */
            if(!(tried_locations[loc]))
            {
              untried_locations.push(loc);
              tried_locations[loc] = true;
            }
          }
          untried_locations.pop();
        }
      }
    }
  }
}
