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

    public:
      /* Generates an artist with a random genotype, with only the first
        triangle set as visible.
       */
      Artist(size_t GENOME_LENGTH, std::independent_bits_engine<std::default_random_engine, 8, uint8_t> rand_byte_generator);
      
      /* Clean up the very obvious sources of memory leaks (chromosome) */
      ~Artist();

      /* Expresses the genotype, compares it to the submitted image and scores
       * it based on similarity.
       */
      double score(Magick::Image source);
};

#endif
