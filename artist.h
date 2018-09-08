#ifndef ARTIST_H
#define ARTIST_H

#include <Magick++.h>

#include "cli.h"
#include "image.h"
#include "triangle.h"

#include <vector>
#include <random>
#include <string>
#include <climits>
#include <cstdlib>
#include <cstdint>
#include <cmath>
#include <limits>

/* Helpful for the bit twiddling */
#define GETMASK(index, size) (((1 << (size)) - 1) << (index))
#define READFROM(data, index, size) (((data) & GETMASK((index), (size))) >> (index))
#define WRITETO(data, index, size, value) ((data) = ((data) & (~GETMASK((index), (size)))) | ((value) << (index)))


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

  /* Max number of triangles in the chromosome */
  static size_t number_of_triangles;

  /* Length of the genome in bytes */
  static size_t genome_length;

  /* Static random engine */
  static std::independent_bits_engine<std::default_random_engine, 8, unsigned char> rand_byte_generator;

  /* Chance to cross over */
  static double crossover_chance;

    public:
      /* Generates an artist with a random genotype, with only the first
        triangle set as visible.
       */
      Artist();
      
      /* Clean up the very obvious sources of memory leaks (chromosome) */
      ~Artist();

      /* Expresses the genotype, compares it to the submitted image and scores
       * it based on similarity.
       */
      double score(const Magick::Image & source);

      /* Take a random double between [0,1] - if lower than or equal to 
        crossover_chance, swap part of the dominant and recessive genomes. The index
        to swap from is draw from an equal distribution from [0, (GENOME_LENGTH -1)].
       */
      void crossover();

      /* Seed the random byte generator */
      static void initializeRandomByteGenerator(size_t RANDOM_SEED);

      /* Set the crossover chance */
      static void initializeCrossoverChance(double XOVER_CHANCE);

      /* Set the max number of triangles, and genome byte length */
      static void initializeGenomeLength(size_t GENOME_LENGTH);

      /* Set the mutation chance */

};

#endif
