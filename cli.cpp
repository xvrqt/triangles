#include "cli.h"

/* The image we're operating on */
char * IMAGE_PATH = 0;

/* Number of triangles to use to attempt to draw the image Because the
   Chromosomes are diploid, this number must be even.
 */
size_t GENOME_LENGTH = 20;

/* Number of artists in each generation */
size_t POPULATION_SIZE = 25;

/* Number of generations to run */
size_t GENERATIONS = 0;

/* srand() seed for repeatable testing */
unsigned int RANDOM_SEED = 0;

void parseArgs(int argc, char ** argv)
{
    int c;
    while(true)
    {
        static struct option long_options[] =
        {
            {"image",               required_argument, 0, 'i'},
            {"genome-length",       required_argument, 0, 'g'},
            {"population-size",     required_argument, 0, 'p'},
            {"generations",         required_argument, 0, 'n'},
            {"random-seed",         required_argument, 0, 'r'},
            {0, 0, 0, 0}
        };

        /* getopt_long stores the option index here. */
        int option_index = 0;

        /* Short codes for characters */
        const char * short_options = "i:g:p:n:r:";

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
                    printf("The genome length  must be greater than 0.\n");
                    exit(1);
                }
                else if(GENOME_LENGTH % 2 != 0)
                {
                    printf("The length of the genome must be an even number.");
                    exit(1);
                }
                else
                {
                    GENOME_LENGTH = g;
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
                printf("%s\n", optarg);
                RANDOM_SEED = atoi(optarg); 
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

    /* Remainning non-option arguments */

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
