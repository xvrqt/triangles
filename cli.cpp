#include "cli.h"

/* The image we're operating on */
char * IMAGE_PATH = 0;

/* Maximum number of triangles to attempt to draw the image with. */
size_t GENOME_LENGTH = 10;

/* "One At A Time Mode" - Number of generations before another triangle is
   allowed to be expressed.
 */
size_t OAAT_MODE = 0;

/* Number of artists in each generation */
size_t POPULATION_SIZE = 25;

/* Number of generations to run */
size_t GENERATIONS = 0;

/* The top N Artists from each population will rollover into the next 
   generation.
 */
size_t ELITISM = POPULATION_SIZE / 10;

/* srand() seed for repeatable testing */
unsigned int RANDOM_SEED = time(NULL);

/* Chance for a diploid chromosome to crossover. */
double XOVER_CHANCE = 0.7;

/* Where the genome is allowed to crossover */
Xover_type XOVER_TYPE = Xover_type::BIT;

/* Chance, per bit, of being flipped each generation. Defaults to 0.005. */
double MUTATION_RATE = 0.005;

/* Enables/Disables Artist location. If set to 0 Artists will mate based on 
   reproductive proportion only, and not take locality into account. If set
   it will act as though artists are adjacent to N number of other artists 
   and restrict mating choices to those artists.
   Defaults to 4.
 */
uint8_t SIMULATE_LOCATION = 4;

void parseArgs(int argc, char ** argv)
{
    int c;
    while(true)
    {
        static struct option long_options[] =
        {
            {"image",               required_argument, 0, 'i'},
            {"genome-length",       required_argument, 0, 'g'},
            {"oaat-mode",           required_argument, 0, 'o'},
            {"population-size",     required_argument, 0, 'p'},
            {"generations",         required_argument, 0, 'n'},
            {"elitism",             required_argument, 0, 'e'},
            {"random-seed",         required_argument, 0, 'r'},
            {"crossover-chance",    required_argument, 0, 'x'},
            {"crossover-type",      required_argument, 0, 't'},
            {"mutation-rate",       required_argument, 0, 'm'},
            {"simulate-location",   required_argument, 0, 'l'},
            {0, 0, 0, 0}
        };

        /* getopt_long stores the option index here. */
        int option_index = 0;

        /* Short codes for characters */
        const char * short_options = "i:g:o:e:p:n:r:x:t:m:l:";

        c = getopt_long(argc, argv, short_options, long_options, &option_index);

        /* End of CLI arguments */
        if(c == -1) { break; }

        /* Almighty switch statement */
        switch(c)
        {
            case 0:
                if(long_options[option_index].flag != 0)
                    break;
                printf("option %s", long_options[option_index].name);
                if(optarg) { printf(" with arg %s", optarg); }
                printf("\n");
                break;

            case 'i': {
                IMAGE_PATH = strdup(optarg);
                break;
            }
            case 'g': {
                int g = atoi(optarg);
                if(g < 0)
                {
                    printf("The genome length must be greater than 0.\n");
                    exit(1);
                }
                else
                {
                    GENOME_LENGTH = g;
                }                
                break;
            }
            case 'o': {
                int o = atoi(optarg);
                if(o < 0)
                {
                    printf("Number of generations without improvement should be greater than 0.\n");
                    exit(1);
                }
                else
                {
                    OAAT_MODE = (size_t)o;
                }                
                break;
            }
            case 'e': {
                int e = atoi(optarg);
                if(e < 0)
                {
                    printf("The number of elites must be greater than or equal to 0.\n");
                    exit(1);
                }
                else
                {
                    ELITISM = (size_t)e;
                }                
                break;
            }
            case 'p': {
                int p = atoi(optarg);
                if(p < 0)
                {
                    printf("The population size must be greater than 0.\n");
                    exit(1);
                }
                else
                {
                    POPULATION_SIZE = p;
                }                
                break;
            }
            case 'n': {
                int n = atoi(optarg);
                if(n < 0)
                {
                    printf("The number of generations must be greater than 0.\n");
                    exit(1);
                }
                else
                {
                    GENERATIONS = n;
                }
                break;
            }
            case 'r': {
                RANDOM_SEED = atoi(optarg); 
                break;
            }
            case 'x': {
                XOVER_CHANCE = std::stod(optarg);
                if(XOVER_CHANCE < 0 || XOVER_CHANCE > 1)
                { 
                    printf("Crossover chance must be between 0.0 and 1.0.\nChance provided:%f", XOVER_CHANCE);
                    exit(1);
                }
                break;
            }
            case 't': {
                std::string type(optarg);
                std::transform(type.begin(), type.end(), type.begin(), ::tolower);

                if(type == "bit") { XOVER_TYPE = Xover_type::BIT; }
                else if(type == "byte") { XOVER_TYPE = Xover_type::BYTE; }
                else if(type == "triangle") { XOVER_TYPE = Xover_type::TRIANGLE; }
                else
                { 
                    printf("Crossover type must be one of bit, byte, triangle.\nType provided: %s\n", optarg);
                    exit(1);
                }
                break;
            }
            case 'm': {
                MUTATION_RATE = std::stod(optarg);
                if(MUTATION_RATE < 0 || MUTATION_RATE > 1)
                { 
                    printf("Mutation rate must be between 0.0 and 1.0.\nRate provided:%f", MUTATION_RATE);
                    exit(1);
                }
                break;
            }
            case 'l': {
                int l = atoi(optarg);
                if(l <= 1 && l != 0)
                { 
                    printf("Simulate-location must be either 0 or >1 in value.\nYou provided: %s\n", optarg);
                    exit(1);
                }
                else
                {
                    SIMULATE_LOCATION = l;
                }
                break;
            }
            case '?': {
                break;
            }
            default: {
                abort();
            }
        }
    }
  
    /* Remaining non-option arguments */

    /* Image path is the default argument */
    if(optind < argc)
    {
        IMAGE_PATH = argv[optind];
    }

    /* Image path is the only required argument */
    if(!IMAGE_PATH)
    {
        printf("You must supply a path to an image using "
               "triangles -i \"path/to/image\"\n");
        exit(1);
    }
}
