#include "main.h"

int main(int argc, char ** argv)
{	
	/* Turns command line arguments into hyper parameters */
	parseArgs(argc, argv);
	/* Allows use of ImageMagick */
	Magick::InitializeMagick(*argv);

	/* Open the source Image */
	Magick::Image source = openImage(IMAGE_PATH);

	/* Initialize Artist settings with runtime parameters. */
	Artist::initializeGenomeLength(GENOME_LENGTH);
	Artist::initializeCrossoverChance(XOVER_CHANCE);
	Artist::initializeRandomByteGenerator(RANDOM_SEED);

	/* Generate a list of Artists */
	std::vector<Artist> artists;
	artists.reserve(POPULATION_SIZE); /* Save us reallocating a few times */
	
	for(size_t i = 0; i < POPULATION_SIZE; i++)
	{
		artists.emplace_back();
	}

	/* Main loops - runs for # of GENERATIONS */
	size_t number_of_generations_run = 0;  /* Keep track of which generation we're on */
	bool run_forever = (GENERATIONS == 0); /* If # of generations is 0 -> run forever */
	for(;number_of_generations_run < GENERATIONS || run_forever; number_of_generations_run++)
	{
		/* Run through the list of artists, perform crossover, mutate them and
		   and score them. [ thread this later ]
		 */
		for(auto a = artists.begin(); a != artists.end(); ++a)
		{
			(*a);
		}

		/* Crossover the genes */
		/* Mutate */
		/* Score */
		/* Mate */
	}


}
