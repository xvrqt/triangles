#ifndef CLI_H
#define CLI_H

#include <time.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <getopt.h>

#include <string>

/* Path to the image that the artist is to draw */
extern char * IMAGE_PATH;

/* Maximum number of triangles to attempt to draw the image with. Defaults to 
   10 triangles.
 */
extern size_t GENOME_LENGTH;

/* Number of artists in each generation. Defaults to 25 */
extern size_t POPULATION_SIZE;

/* Number of generations to run. Defaults to 0 (infinite) */
extern size_t GENERATIONS;

/* The top N Artists from each population will rollover into the next 
   generation. Defaults to 5.
 */
extern size_t ELITISM;

/* srand() seed for repeatable testing. Defaults to 0. */
extern unsigned int RANDOM_SEED;

/* Chance for a diploid chromosome to crossover. Defaults to 0.7. */
extern double XOVER_CHANCE;

/* Sets how crossover proceeds.
   - "bit"      -> the genome will crossover at an individual bit boundary
   - "byte"     -> the genmoe will crossover will be byte aligned
   - "triangle" -> the genome will crossover at triangle boundaries
 */
enum class Xover_type {BIT, BYTE, TRIANGLE};
extern Xover_type XOVER_TYPE;

/* Chance, per bit, of being flipped each generation. Defaults to 0.005. */
extern double MUTATION_RATE;

/* Puts all the arguments into the variables */
void parseArgs(int argc, char ** argv);

#endif
