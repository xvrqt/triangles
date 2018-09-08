#ifndef CLI_H
#define CLI_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <getopt.h>

/* Path to the image that the artist is to draw */
extern char * IMAGE_PATH;

/* Minimum number of triangles to attempt to draw the image with. Since the 
   chromosomes are diploid, the maximum amount of triangles is double this.
 */
extern size_t GENOME_LENGTH;

/* Number of artists in each generation. Must be a square number. 
   Defaults to 25.
 */
extern size_t POPULATION_SIZE;

/* Number of generations to run. Defaults to 0 (infinite) */
extern size_t GENERATIONS;

/* srand() seed for repeatable testing. Defaults to 0. */
extern unsigned int RANDOM_SEED;

/* Puts all the arguments into the variables */
void parseArgs(int argc, char ** argv);

#endif
