#ifndef ARTIST_H
#define ARTIST_H

#include <Magick++.h>

#include "cli.h"
#include "image.h"
#include "triangle.h"

#include <vector>
#include <random>
#include <string>
#include <cmath>
#include <utility>
#include <climits>
#include <cstdlib>
#include <cstdint>
#include <limits>

/* Helpful for the bit twiddling */
#define GETMASK(index, size) (((1 << (size)) - 1) << (index))

/* Semantic struct to represent the diploid nature of the genes */
struct Chromosome {
  uint8_t * dominant;
  uint8_t * recessive;

  /* Length in bytes */
  size_t length;

  /* Crossover point */
  size_t crossover_point = 0;
};

/* An artist represents a genome, with some additional state information and
   functionality. 

   It houses two genomes to make up a diploid chromosome, its
   current fitness for easy comparisons and an Image to draw upon so we can
   thread this program.

   For functionality, it can decipher and draw its phenotype to an Image, it
   can perform crossover on itself and it can mutate itself.
 */
class Artist
{
  /* The genetic information */
  Chromosome chromosome;

  /* Fitnesss of the Artist */
  double fitness;

  /* The expected amount of times an Artist will reproduce */
  double expected_reproduction;

  /* Index into where the artist lives in the Artist Location Map */
  size_t location_index;

  /* Max number of triangles in the chromosome */
  static size_t number_of_triangles;

  /* Length of the genome in bytes */
  static size_t genome_length;

  /* Static random engine */
  static std::default_random_engine rand_engine;
  static std::independent_bits_engine<std::default_random_engine, 8, unsigned char> rand_byte_generator;

  /* Chance to cross over */
  static double crossover_chance;

  /* Chance, per bit, to be flipped per generation */
  static double mutation_rate;

  /* Keep count of the number of Artists. Used to set the location index. */
  static size_t count;

  /* Precompute the liklihood of mating based on location and store it in this
     table. The ith index of the first vector returns a vector of pairs. These 
     are sorted so closest points have lower indices. Randomly select a number
     between [0,1) and sum the liklihoods until sum > rand. The second part of 
     the pair is the location index of Artist to mate with.
   */
  static std::vector<std::vector<std::pair<double, size_t>>> location_liklihood_map; 

  /* Generates the artist's location */

    public:
      /* Generates an artist with a random genotype, with only the first
        triangle set as visible.
       */
      Artist();

      /* Generates a new Artist from two parent artists. Caller is responsible
         for freeing the returned baby.
       */
      Artist(const Artist &a, const Artist &b);
      
      /* Clean up the very obvious sources of memory leaks (chromosome) */
      ~Artist();

      /* Allows us to sort without copying. Sorts via fitness. */
      bool operator <(const Artist &a) const;
      Artist(Artist const &that);
      void operator=(Artist const &that);

      /* Expresses the genotype, compares it to the submitted image and scores
       * it based on similarity.
       */
      void score(const Magick::Image & source);

      /* Returns the fitness of the Artist #getters */
      double getFitness() const;

      /* Returns the expected_reproduction of the Artist. # getters */
      double getExpectedReproduction() const;

      /* Returns the location index of the Artist */
      size_t getLocationIndex() const;

      /* Set the location index - in case it already exists */
      void setLocationIndex(size_t index);

      /* Take a random double between [0,1] - if lower than or equal to 
        crossover_chance, swap part of the dominant and recessive genomes. The index
        to swap from is draw from an equal distribution from [0, (GENOME_LENGTH -1)].
       */
      void crossover();

      /* Chance to flip some of the bits */
      void mutate();

      /* Sets the proportion the artists should reproduce */
      void setReproductionProportion(double avg_fitness, double std_dev); 

      /* Convenience funtion that calls the other initialization functions and ensures
         they are called in the correct order.
       */
      static void initialization(size_t GENOME_LENGTH, double MUTATION_RATE, double XOVER_CHANCE, size_t RANDOM_SEED);

      /* Seed the random byte generator */
      static void initializeRandomByteGenerator(size_t RANDOM_SEED);

      /* Set the crossover chance */
      static void initializeCrossoverChance(double XOVER_CHANCE);

      /* Set the mutation rate */
      static void initializeMutationRate(double MUTATION_RATE);

      /* Set the max number of triangles, and genome byte length */
      static void initializeGenomeLength(size_t GENOME_LENGTH);
};

#endif
