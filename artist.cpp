#include "artist.h"

/* Static member initialization */
std::default_random_engine Artist::rand_engine;
std::independent_bits_engine<std::default_random_engine, 8, unsigned char> Artist::rand_byte_generator;

std::vector<std::vector<std::pair<double, size_t>>> Artist::location_liklihood_map; 

double Artist::crossover_chance = 0.7;
double Artist::mutation_rate = 0.005;
size_t Artist::number_of_triangles = 10;
size_t Artist::genome_length = (Artist::number_of_triangles * sizeof(Triangle) + 4 * sizeof(uint8_t));
size_t Artist::count = 0;

/* Set up the static random byte generator */
void Artist::initializeRandomByteGenerator(size_t RANDOM_SEED)
{
  Artist::rand_engine.seed(RANDOM_SEED);
  std::independent_bits_engine<std::default_random_engine, 8, unsigned char> dummy(Artist::rand_engine);
  Artist::rand_byte_generator = dummy;
}

/* Set the crossover chance */
void Artist::initializeCrossoverChance(double XOVER_CHANCE)
{
  if(XOVER_CHANCE >= 0 && XOVER_CHANCE <= 1) { Artist::crossover_chance = XOVER_CHANCE; }
  else 
  { 
    std::cerr << "Crossover chance must be between 0.0 and 1.0.\nChance \
                  provided: " << XOVER_CHANCE << std::endl;
    exit(1);
  }
}

/* Set the mutation rate */
void Artist::initializeMutationRate(double MUTATION_RATE)
{
  if(MUTATION_RATE >= 0 && MUTATION_RATE <= 1) { Artist::mutation_rate = MUTATION_RATE; }
  else 
  { 
    std::cerr << "Mutation rate must be between 0.0 and 1.0.\nRate \
                  provided: " << MUTATION_RATE << std::endl;
    exit(1);
  }
}

/* Set the max number of triangles, and genome byte length */
void Artist::initializeGenomeLength(size_t GENOME_LENGTH)
{
  /* The CLI parameter "GENOME_LENGTH" actually specifys the number of 
     triangles. This is because the user won't know how many bytes to specify.
   */
  Artist::number_of_triangles = GENOME_LENGTH;

  /* Calculate the byte length */
  size_t size_of_bg_rbg = (4 * sizeof(uint8_t));
  size_t size_of_triangles = (Artist::number_of_triangles * sizeof(Triangle));
  Artist::genome_length = size_of_bg_rbg + size_of_triangles;
}

/* Generates an artist with a random genotype, with only the first
   triangle set as visible.
 */
Artist::Artist()
{
  chromosome.dominant = (uint8_t *) malloc(Artist::genome_length);
  chromosome.recessive = (uint8_t *) malloc(Artist::genome_length);

  /* Randomize the bits in the byte array */
  for(size_t i = 0; i < (Artist::genome_length) ; i++)
  {
    chromosome.dominant[i] = rand_byte_generator(); 
    chromosome.recessive[i] = rand_byte_generator(); 
  }

  /* Go through and ensure only the first triangle is visible */
  Triangle * dominant = (Triangle *)(chromosome.dominant + TRIANGLE_LIST_BEGIN);
  Triangle * recessive = (Triangle *)(chromosome.recessive + TRIANGLE_LIST_BEGIN);
  for(size_t i = 0; i < Artist::number_of_triangles; i++)
  {
    /* 0111 - more mutation resiliant */
    dominant[i].visible = (i == 0) ? 7 : 0;
    recessive[i].visible = 0;
  }

  /* Set the fitness as high as possible to ensure it's replaced. */
  fitness = std::numeric_limits<double>::max();
  expected_reproduction = 1.0; /* This will be overwritten later */

  /* Sets the location index */
  location_index = Artist::count++;
}

/* Copy constuctor */
Artist::Artist(Artist const &that)
{
  fitness = that.fitness;
  location_index = that.location_index;
  expected_reproduction = that.expected_reproduction;

  /* Allocate new memory for the new chromosome */
  chromosome.dominant = (uint8_t *) malloc(Artist::genome_length);
  chromosome.recessive = (uint8_t *) malloc(Artist::genome_length);

  /* Copy the bits into the new chromosome */
  memcpy(chromosome.dominant, that.chromosome.dominant, Artist::genome_length);
  memcpy(chromosome.recessive, that.chromosome.recessive, Artist::genome_length);
}

/* Assignment constructor */
void Artist::operator=(Artist const &that)
{
  fitness = that.fitness;
  location_index = that.location_index;
  expected_reproduction = that.expected_reproduction;

  /* Allocate new memory for the new chromosome */
  chromosome.dominant = (uint8_t *) malloc(Artist::genome_length);
  chromosome.recessive = (uint8_t *) malloc(Artist::genome_length);

  /* Copy the bits into the new chromosome */
  memcpy(chromosome.dominant, that.chromosome.dominant, Artist::genome_length);
  memcpy(chromosome.recessive, that.chromosome.recessive, Artist::genome_length);
}

/* Destructor */
Artist::~Artist()
{
  free(chromosome.dominant);
  free(chromosome.recessive);
}

/* Expresses the genotype, compares it to the submitted image and scores
 * it based on similarity. Sets and returns the fitness.
 */
void Artist::score(const Magick::Image & source)
{
  /* Extract the source dimensions */
  std::string source_width(std::to_string(source.columns()));
  std::string source_height(std::to_string(source.rows()));
  std::string image_dimensions = source_width + "x" + source_height;

  /* Extract the RGBA background color from the dominant genome */
  uint8_t * rgba = chromosome.dominant + BG_COLOR_OFFSET;

  /* Create a new canvas, same size as the source with the encoded background 
     color.
   */
  Magick::Color bg_color(rgba[0], rgba[1], rgba[2], rgba[3]);
  Magick::Image canvas(image_dimensions, bg_color);

  /* Set some drawing defaults on the image */
  canvas.strokeWidth(0.0);
  canvas.strokeAntiAlias(true);

  /* Create a list of triangles to draw to the canvas */
  Triangle * dominant = (Triangle *)(chromosome.dominant + TRIANGLE_LIST_BEGIN);
  Triangle * recessive = (Triangle *)(chromosome.recessive + TRIANGLE_LIST_BEGIN);
  std::vector<Magick::Drawable> triangle_list;

  for(size_t i = 0; i < GENOME_LENGTH; i++)
  {
    Triangle tri_dom = dominant[i];
    Triangle tri_rec = recessive[i];
    
    /* Draw the triangle with the higher visible parameter */
    Triangle tri = (tri_dom.visible >= tri_rec.visible) ? tri_dom : tri_rec;
    
    /* If both triangles were visibilty 0; draw neither */
    if(tri.visible == 0) { continue; }
    
    /* Transform the triangle into a DrawablePolygons */      
    /* Translate the coordinates */
    Magick::CoordinateList coordinates;
    coordinates.push_back(Magick::Coordinate(tri.x1,tri.y1));
    coordinates.push_back(Magick::Coordinate(tri.x2,tri.y2));
    coordinates.push_back(Magick::Coordinate(tri.x3,tri.y3));
    Magick::DrawablePolyline drawable_triangle(coordinates);

    /* Set the fill/stroke color */
    Magick::Color color(tri.r, tri.g, tri.b, tri.a);
    canvas.strokeColor(color);
    canvas.fillColor(color);

    /* Push the colors and then the shape onto the drawable lsit */
    triangle_list.push_back(Magick::DrawableFillColor(color));
    triangle_list.push_back(Magick::DrawableStrokeColor(color));
    triangle_list.push_back(drawable_triangle);
  }

  /* Draw the triangles! */
  canvas.draw(triangle_list);
  // canvas.write("testing_resources/test_canvas.png");

  /* The current fitness function is only the RMSE between the source and the 
    newly drawn image.
   */
  fitness = canvas.compare(source, Magick::RootMeanSquaredErrorMetric);
}

/* Returns the fitness of the Artist #getters */
double Artist::getFitness() const
{
  return fitness;
}

/* Returns the expected_reproduction of the Artist. # getters */
double Artist::getExpectedReproduction() const
{
  return expected_reproduction;
}

/* Take a random double between [0,1] - if lower than or equal to 
  crossover_chance, swap part of the dominant and recessive genomes. The index
  to swap from is draw from an equal distribution from the 0th bit to the 
  last bit.
 */
void Artist::crossover()
{
  /* Only crossover $(crossover_chance)% of the time */
  double rand_zero_to_one = (double)(rand()/RAND_MAX);
  if(rand_zero_to_one < Artist::crossover_chance)
  {
      /* Pick a bit to crossover at */
      size_t genome_bit_length = Artist::genome_length * 8;
      double div = (RAND_MAX/(genome_bit_length - 1));
      size_t bit_index = (size_t)(std::floor(((double)rand()/div)));

      /* Determine the byte index and swap the bytes in the range: 
         [(byte_index + 1), genome_length].
       */
      size_t byte_index = bit_index / 8;
      /* There are no bytes to copy when crossover happens in the last byte */
      if(byte_index < (Artist::genome_length - 1))
      {
        /* Number of bytes to swap */
        size_t swap_size = Artist::genome_length - byte_index - 1;
        uint8_t * byte_swap_space = (uint8_t *)malloc(sizeof(uint8_t) * swap_size);
        memcpy(byte_swap_space, (chromosome.dominant + byte_index + 1), swap_size);
        memcpy((chromosome.dominant + byte_index + 1), (chromosome.recessive + byte_index + 1), swap_size);
        memcpy((chromosome.recessive + byte_index + 1), byte_swap_space, swap_size);

        /* Don't forget to clean up */
        free(byte_swap_space);
      }

      /* For the byte that is split, swap the bits */
      uint8_t dom_byte = chromosome.dominant[byte_index];
      uint8_t rec_byte = chromosome.recessive[byte_index];
      uint8_t intra_byte_index = bit_index % 8;
      uint8_t num_bits = 8 - intra_byte_index;

      /* Prepare the bytes for writing to */
      uint8_t mask1 = (uint8_t)GETMASK(intra_byte_index, num_bits);
      chromosome.dominant[byte_index] |= mask1;
      chromosome.recessive[byte_index] |= mask1;

      /* Prepare the bits to be written */
      uint8_t dom_bits = (dom_byte & mask1) | (~mask1);
      uint8_t rec_bits = (rec_byte & mask1) | (~mask1);

      /* Write the bits to the byte */
      chromosome.dominant[byte_index] &= rec_bits;
      chromosome.recessive[byte_index] &= dom_bits;
  }
}

/* Chance to flip some of the bits */
void Artist::mutate()
{
  /* Create a binomial distribution to detect the number of mutations */
  size_t num_bits = 8 * genome_length * 2; /* 2 because diploid chromosome */
  std::binomial_distribution<size_t> distribution(num_bits, MUTATION_RATE);
  size_t num_mutations = distribution(Artist::rand_engine);

  /* Selecte a random bit to flip per mutation. */
  std::uniform_int_distribution<size_t> bit_selector(0, (num_bits - 1));
  for(size_t i = 0; i < num_mutations; i++)
  {
    /* Indices into the bits & bytes */
    size_t bit_index = bit_selector(Artist::rand_engine);
    size_t byte_index = bit_index / 8;
    uint8_t intra_byte_index = bit_index % 8;

    /* Select which genome to operate on. Dominant|Recessive */
    uint8_t * genome = (byte_index >= genome_length) ? chromosome.dominant : chromosome.recessive;
    byte_index %= genome_length;

    /* Flip the bit */
    uint8_t mask = (uint8_t)GETMASK(intra_byte_index, 1);
    genome[byte_index] ^= mask;
  }
}

/* Sets the proportion the artists should reproduce */
void Artist::setReproductionProportion(double avg_fitness, double std_dev)
{
  if(std_dev == 0) { expected_reproduction = 1; }
  else 
  {
    expected_reproduction = 1 - ((fitness - avg_fitness) / (2 * std_dev));
    /* Everyone has a smöl chance to make it */
    if(expected_reproduction <= 0) { expected_reproduction = 0.1; }
  }
}

/* Allows us to sort without copying */
bool Artist::operator <(const Artist &a) const
{
  return fitness < a.fitness;
}

/* Generates N approximately equidistant points on sphere, and pre
   computes the distance between them, storing the results in a vector.
   Artists ar point P choose the Pth index in the vector, which is 
   another vector of pairs containing the precomputed distances between 
   every other point, and the index of the point.

   This must be done before constructing artists.
 */
void Artist::precomputeDistances(size_t POPULATION_SIZE)
{
  /* Generate POPULATION_SIZE points around a unit sphere */
  std::vector<Point> points;
  double offset = 2.0/(double)POPULATION_SIZE;
  double increment = M_PI * (3.0 - std::sqrt(5.0));

  for(size_t i = 0; i < POPULATION_SIZE; i++)
  {
    Point p;
    p.y = (((double)i * offset) -1.0) + (offset / 2.0);
    double r = std::sqrt(1.0 - std::pow(p.y, 2.0));
    double phi = ((i + 1) % POPULATION_SIZE) * increment;
    p.x = std::cos(phi) * r;
    p.z = std::sin(phi) * r;

    points.emplace_back(p);
  }

  location_liklihood_map.clear();
  location_liklihood_map.resize(POPULATION_SIZE);

  /* For each point, compute the distance between each other pair of points. 
     Sum the distances together, and compute the % of total distance. Sort the
     % of total distance, point pairs by highest percent to lowest (closer 
     points are ranked higher).
   */
  for(size_t i = 0; i < points.size(); i++)
  {
    /* Caculate the total distance between p and all other points */
    Point p = points[i];
    double distance_sum = 0.0;
    /* std::vector<std::pair<double, size_t>> */
    auto v = location_liklihood_map[i];
    for(size_t j = 0; j < points.size(); j++)
    {
      /* Don't include yourself */
      if(i == j) { continue; }

      /* Calculate distance */
      Point o = points[j];
      double x_diff = std::pow((p.x - o.x), 2);
      double y_diff = std::pow((p.y - o.y), 2);
      double z_diff = std::pow((p.z - o.z), 2);
      double distance = std::sqrt(x_diff + y_diff + z_diff);
      distance_sum += distance;

      /* Push point distance, point index pair into vector */
      v.push_back(std::pair<double, size_t>(distance, j));
    }

    /* Go back through v and change the distances into relative proportions */
    for(size_t j = 0; j < v.size(); j++)
    {
      v[j].first /= distance_sum;
    }

    /* Sort the proportions from largest to smallest */
    std::sort(v.begin(), v.end(), [](const std::pair<double, size_t> & a, const std::pair<double, size_t> & b) -> bool { 
        return a.first > b.first; 
    });
  }

}