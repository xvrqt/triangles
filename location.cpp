#include "location.h"

/* Generates N approximately equidistant points on sphere, and pre
   computes the distance between them, storing the results in a vector.
   Artists ar point P choose the Pth index in the vector, which is 
   another vector of pairs containing the precomputed distances between 
   every other point, and the index of the point.

   This must be done before constructing artists.
 */
std::vector<std::vector<std::pair<double, size_t>>> getLocationLikelihoodMap(size_t POPULATION_SIZE)
{
  /* What we'll be returning */
  std::vector<std::vector<std::pair<double, size_t>>> location_liklihood_map;
  location_liklihood_map.resize(POPULATION_SIZE);

  /* Generate POPULATION_SIZE points around a unit sphere */
  std::vector<Point> points;
  double offset = 2.0/(double)POPULATION_SIZE;
  double increment = M_PI * (3.0 - std::sqrt(5.0));

  for(size_t i = 0; i < POPULATION_SIZE; i++)
  {
    Point p;
    p.y = (((double)i * offset) -1.0) + (offset / 2.0);
    double r = std::sqrt(1.0 - std::pow(p.y, 2.0));
    double phi = ((i + 1) % POPULATION_SIZE) * increment;
    p.x = std::cos(phi) * r;
    p.z = std::sin(phi) * r;

    points.emplace_back(p);
  }

  /* For each point, compute the distance between each other pair of points. 
     Sum the distances together, and compute the % of total distance. Sort the
     % of total distance, point pairs by highest percent to lowest (closer 
     points are ranked higher).
   */
  for(size_t i = 0; i < points.size(); i++)
  {
    /* Calculate the total distance between p and all other points */
    Point p = points[i];
    double distance_sum = 0.0;
    double max_distance = std::sqrt(12); /* [(1,1,1), (-1,-1,-1)] */
    /* std::vector<std::pair<double, size_t>> - convenience variable */
    std::vector<std::pair<double, size_t>> & v = location_liklihood_map[i];
    for(size_t j = 0; j < points.size(); j++)
    {
      /* Don't include yourself */
      if(i == j) { continue; }

      /* Calculate distance */
      Point o = points[j];
      double x_diff = std::pow((p.x - o.x), 2);
      double y_diff = std::pow((p.y - o.y), 2);
      double z_diff = std::pow((p.z - o.z), 2);
      double distance = std::sqrt(x_diff + y_diff + z_diff);

      /* In this way, the smallest difference will make up the largest 
      	 proportion, saving us a few loops.
       */
      double distance_diff = std::pow(max_distance - distance, std::sqrt(POPULATION_SIZE));
      distance_sum += distance_diff;

      /* Push point distance difference, point index pair into vector. */
      v.emplace_back(distance_diff, j);
    }

    /* Go back through v and change the distances into inverse relative 
       proportions (so that closest ones have the largest proportions).
     */
    for(size_t j = 0; j < v.size(); j++)
    {
      v[j].first /= distance_sum;
    }

    /* Sort the proportions from largest to smallest */
    std::sort(v.begin(), v.end(), [](const std::pair<double, size_t> & a, const std::pair<double, size_t> & b) -> bool { 
        return a.first > b.first; 
    });
  }
  return location_liklihood_map;
}