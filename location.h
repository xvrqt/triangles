#include "cli.h"

#include <vector>
#include <cmath>
#include <iostream>

/* Precompute the liklihood of mating based on location and store it in this
   table. The ith index of the first vector returns a vector of pairs. These 
   are sorted so closest points have lower indices. Randomly select a number
   between [0,1) and sum the liklihoods until sum > rand. The second part of 
   the pair is the location index of Artist to mate with.
*/
std::vector<std::vector<size_t>> getLocationLikelihoodMap(size_t POPULATION_SIZE);
