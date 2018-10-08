#include "main.h"

int main(int argc, char ** argv)
{	
	/* Turns command line arguments into global parameters. See cli.h for 
	   details.
	 */
	parseArgs(argc, argv);
	
	/* Allows use of ImageMagick */
	Magick::InitializeMagick(*argv);

	/* Initialize Artist settings with runtime parameters. This sets static
	   member variables so we can use the default constructor for Artists.
	 */
	Artist::initialization(RANDOM_SEED, GENOME_LENGTH, MUTATION_RATE, XOVER_CHANCE, XOVER_TYPE);

	/* If One At A Time Mode is set Artists begin only expressing one 
	   triangle. 
	 */
	if(OAAT_MODE) { Artist::setExpressionLimit(1); }

	/* Create a matrix of adjacent locations. */
	std::vector<std::vector<size_t>> adjacency_matrix;
	if(SIMULATE_LOCATION) 
	{ 
		adjacency_matrix = getAdjacencyMatrix(POPULATION_SIZE, SIMULATE_LOCATION);
	}

	/* Open the source Image */
	Magick::Image source = openImage(IMAGE_PATH);

	/* Copy the image into the Artist class so it can be used as a reference 
	   for scoring the artists later.
	 */
	Artist::initializeSourceImage(source);

	/* Initialize the srand for crossover/mutation decisions. Initialize a 
	   a random engine so we can */
	srand(RANDOM_SEED);
    std::mt19937 rand_engine(RANDOM_SEED);
    std::uniform_int_distribution<int> rand_artist_index(0,POPULATION_SIZE - 1);

	/* Generate a list of Artists */
	std::vector<Artist *> artists;
	artists.reserve(POPULATION_SIZE);
	for(size_t i = 0; i < POPULATION_SIZE; i++)
	{
		artists.push_back(new Artist());
	}

    /* If SIMULATE_LOCATION is set - this is used to store artist locations on
       the "map."
     */
    std::vector<Artist *> artists_locations(POPULATION_SIZE);

	/* If # of generations is 0 -> run forever */
	bool run_forever = (GENERATIONS == 0); 

    /* Keep track of which generation we're on */
	size_t number_of_generations_run = 0;  
	
	/* One at time mode - artists render 1 triangle until they make an 
	   improvement. Once an improvement has been made it begins counting 
	   generations hence; reseting when fitness improves. Once the number of
	   generations without improvement > OAAT_MODE it increases the number of 
	   triangles permitted to be expressed.
	 */
	bool has_made_improvement = false;     /* Don't add a triangle until it helps */
	size_t num_gens_no_improvement = 0;    /* Keep track for OAAT Mode */

	/* Forces every artist (even clones) to be rescored */
	bool force_rescore = true;
	
	/* The best fitness we've seen thus far */
	double best_fitness = std::numeric_limits<double>::max();

	/* The number of processors available to us will determine how many threads
	   we will want to create.
	 */
	unsigned int num_threads = std::thread::hardware_concurrency();
	if(num_threads == 0) { num_threads = 1; }

	/* Used to allocate a portion of the population to each thread */
	size_t num_artists_per_thread = (POPULATION_SIZE / num_threads);
	size_t additional_artist_index = (POPULATION_SIZE % num_threads);

	/* Main loops - runs for # of GENERATIONS */
	for(;number_of_generations_run < GENERATIONS || run_forever; number_of_generations_run++)
	{
		/* Add the artists to a location indexed vector so that it's easy to 
		   look them up later for mating. If a space is already occupied, it
		   will search for the nearest empty location to fill.
		*/
		if(SIMULATE_LOCATION)
		{
			computeArtistLocation(artists, adjacency_matrix, artists_locations);
		}

		/* Run through the list of artists, perform crossover, mutate them and
		   and score them. Split the list to take advantage of the number of 
		   processors on the machine.
		 */
		std::vector<std::thread> threads;
		size_t prev_index = 0;
		for(size_t i = 0; i < num_threads; i++)
		{
			/* Determine the number of artists this thread will act on */
			size_t num_artists = num_artists_per_thread;
			if(i < additional_artist_index) { num_artists++; }
			
			/* Copy the portion of the population into the thread */
			std::vector<Artist *> a_t(num_artists);
			auto start_index = artists.begin() + prev_index;
			auto end_index = start_index + num_artists;
			std::copy(start_index, end_index, a_t.begin());
			
			/* Create the thread */
			threads.push_back(std::thread([](std::vector<Artist *> const & v) {
				for(auto & a : v)
				{
					a->crossover();
					a->mutate();
					a->score();
				}
			}, a_t));

			/* Update the beginning of the range */
			prev_index += num_artists;
		}

		/* Wait for all threads to complete */
		for (auto & t : threads) { t.join(); }
		force_rescore = false;

		/* Sort the artists from best to worst */
		std::sort(artists.begin(), artists.end(), [](const Artist * a, const Artist * b) -> bool { 
        	return (a->getFitness() < b->getFitness()); 
    	});

		/* Calculate the average fitness & std dev of the artists */
		double std_dev = 0.0;
		double avg_fitness = 0.0;
		for(auto a = artists.begin(); a != artists.end(); ++a)
		{
			avg_fitness += (*a)->getFitness();
		}
		avg_fitness /= artists.size();

		/* Calculate the std dev of the artists */
		for(auto a = artists.begin(); a != artists.end(); ++a)
		{
			std_dev += pow((*a)->getFitness() - avg_fitness, 2);
			std_dev = sqrt(std_dev/POPULATION_SIZE);
		}

		/* Calculate the number of times an artist should reproduce */
		for(auto a = artists.begin(); a != artists.end(); ++a)
		{
			(*a)->setReproductionProportion(avg_fitness, std_dev);
		}

		/* Add artists to a new vector, in proportion to their fitness, with
		   a small amount of randomness for good measure.
		 */
		std::vector<Artist *> artists_proportional;
		artists_proportional.reserve(POPULATION_SIZE);

		double roulette = (double) ((double)rand()/(double)RAND_MAX);
		double sum = 0.0;
		size_t index = 0;
		for(;artists_proportional.size() < artists.size();)
		{
			double expected_reproduction = artists[index]->getExpectedReproduction();
			for(sum += expected_reproduction; sum > roulette; roulette++)
			{
				artists_proportional.push_back(artists[index]);
			}
			index++;
		}

		/* Mate the artists to produce the next generation in proportion to
		   their fitness.
		 */
		std::vector<Artist *> next_generation;
		next_generation.reserve(POPULATION_SIZE);
		size_t num_of_elite = 0;
		for(auto a = artists_proportional.begin(); a != artists_proportional.end(); ++a)
		{
			if(next_generation.size() >= POPULATION_SIZE) { break; }
			if(num_of_elite++ < ELITISM)
			{
				next_generation.push_back(new Artist(**a));
			}

            size_t index = rand_artist_index(rand_engine);

			Artist * mate = NULL;
            if(SIMULATE_LOCATION) /* Mate only with adjacent dealios */
            {
			  auto am = adjacency_matrix[(*a)->getLocationIndex()];
              mate = artists_locations[am[index % am.size()]];
            }
            else /* Mate randomly (but in proportion) */
            {
               mate = artists_proportional[index];
            }

			/* Push the baby of the artists into the next generation. */
			next_generation.push_back(new Artist(**a, *mate));
		}

		/* Check if this artist has the best fitness seen so far */
		double this_gen_best_fitness = artists[0]->getFitness();
		if(this_gen_best_fitness < best_fitness)
		{
			best_fitness = this_gen_best_fitness;
			num_gens_no_improvement = 0;
			has_made_improvement = true;
		}
		if(has_made_improvement) { num_gens_no_improvement++; }

		/* Print out the best fitness each round */
		std::cout << this_gen_best_fitness << std::endl;

		/* Check if we need to increase the number of triangles allowed to be expressed */
		size_t num_triangles = (Artist::getExpressionLimit() > GENOME_LENGTH) ? GENOME_LENGTH : Artist::getExpressionLimit();
		if(number_of_generations_run % 50 == 0)
		{
			/* Print out the best image from that number of triangles */
			Magick::Image * best_image = artists[0]->draw();
			std::string num_gens = std::to_string(number_of_generations_run);
			std::string num_tri = std::to_string(num_triangles);
			std::string best_fit = std::to_string(best_fitness);
			best_image->write("output/" + num_gens + "_" + num_tri + "_" + best_fit + ".png");
			delete best_image;
		}

		/* Reset the count, allow one more triangle */
		if(OAAT_MODE && num_gens_no_improvement >= OAAT_MODE)
		{
			has_made_improvement = false;
			num_gens_no_improvement = 0;
			force_rescore = true;
			Artist::incrementExpressionLimit();
			std::cout << "Adding Trianle - " << num_triangles << " triangles in total\n";
		}

		/* Delete old artists and copy the next_generation into the artists vector. */
		for(auto a : artists)
		{
			delete a;
		}
		artists.clear();
		artists = next_generation;
	}
}
