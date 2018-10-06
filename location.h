#include "cli.h"

#include <cmath>
#include <vector>
#include <iostream>

#include "artist.h"

/* Creates a table where the index of the first vecotor indicates the position
   on the "map." The second vector contains all places adjacent to that point 
   on the "map."
*/
std::vector<std::vector<size_t>> getAdjacencyMatrix(size_t POPULATION_SIZE, uint8_t SIMULATE_LOCATION);

/* Takes a vector of artists, an adjacency matrix and returns a vector of 
   artists pointers such that the artist at location N is at index N.
 */
void computeArtistLocation(	std::vector<Artist *> & artists, 
							std::vector<std::vector<size_t>> const & adjacency_matrix,
							std::vector<Artist *> & artist_locations);