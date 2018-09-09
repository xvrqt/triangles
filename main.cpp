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
	Artist::initializeMutationRate(MUTATION_RATE);
	Artist::initializeCrossoverChance(XOVER_CHANCE);
	Artist::initializeRandomByteGenerator(RANDOM_SEED);

	/* Initialize the srand for crossover/mutation decisions */
	srand(RANDOM_SEED);

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
			(*a).crossover();
			(*a).mutate();
			(*a).score(source);
		}

		for(auto a = artists.begin(); a != artists.end(); ++a)
		{
			std::cout << (*a).getFitness() << std::endl;
		}


		/* Sort the artists from best to worst */
		std::sort(artists.begin(), artists.end());
		std::cout << "---" << std::endl;
		std::cout << (*artists.begin()).getFitness() << std::endl;
		std::cout << (*(artists.end() -1)).getFitness() << std::endl;
		std::cout << "---" << std::endl;

		if(number_of_generations_run > 2) { exit(0); }


		// /* Calculate the average fitness & std dev of the artists */
		// double std_dev = 0.0;
		// double avg_fitness = 0.0;
		// for(auto a = artists.begin(); a != artists.end(); ++a)
		// {
		// 	avg_fitness += (*a).getFitness();
		// }
		// avg_fitness /= artists.size();

		// /* Calculate the std dev of the artists */
		// for(auto a = artists.begin(); a != artists.end(); ++a)
		// {
		// 	std_dev += pow((*a).getFitness() - avg_fitness, 2);
		// 	std_dev = sqrt(std_dev/POPULATION_SIZE);
		// }

		// std::cout << "std dev: " << std_dev << std::endl;

		// /* Calculate the number of times an artist should reproduce */
		// for(auto a = artists.begin(); a != artists.end(); ++a)
		// {
		// 	// (*a).setReproductionProportion(avg_fitness);
		// }

		/* Mate the artists to produce the next generation in proportion to
		   their fitness.
		 */



	}


}
