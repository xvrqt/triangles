#include "artist.h"

/* Static member initialization */
std::independent_bits_engine<std::default_random_engine, 8, unsigned char> Artist::rand_byte_generator;
double Artist::crossover_chance = 0.7;

/* Set up the static random byte generator */
void Artist::initializeRandomByteGenerator(size_t RANDOM_SEED)
{
  std::default_random_engine rand_engine(RANDOM_SEED);
  std::independent_bits_engine<std::default_random_engine, 8, unsigned char> dummy(rand_engine);
  Artist::rand_byte_generator = dummy;
}

/* Set the crossover chance */
void Artist::initializeCrossoverChance(double chance)
{
  if(chance >= 0 && chance <= 1) { Artist::crossover_chance = chance; }
  else 
  { 
    std::cerr << "Crossover chance must be between 0.0 and 1.0.\nChance \
                  provided: " << chance << std::endl;
    exit(1);
  }
}

/* Generates an artist with a random genotype, with only the first
   triangle set as visible.
 */
Artist::Artist(size_t GENOME_LENGTH)
{
  /* Create a byte array to hold the RGB for the background color, and the
     coordinates/colors of all the triangles.
   */
  size_t size_of_bg_rbg = (4 * sizeof(uint8_t));
  size_t size_of_triangles = (GENOME_LENGTH * sizeof(Triangle));
  size_t size_of_genome = size_of_bg_rbg + size_of_triangles;

  chromosome.dominant = (uint8_t *) malloc(size_of_genome);
  chromosome.recessive = (uint8_t *) malloc(size_of_genome);

  /* Randomize the bits in the byte array */
  for(size_t i = 0; i < (size_of_genome) ; i++)
  {
    chromosome.dominant[i] = rand_byte_generator(); 
    chromosome.recessive[i] = rand_byte_generator(); 
  }

  /* Go through and ensure only the first triangle is visible */
  Triangle * dominant = (Triangle *)(chromosome.dominant + TRIANGLE_LIST_BEGIN);
  Triangle * recessive = (Triangle *)(chromosome.recessive + TRIANGLE_LIST_BEGIN);
  for(size_t i = 0; i < GENOME_LENGTH; i++)
  {
    /* 0111 - more mutation resiliant */
    dominant[i].visible = (i == 0) ? 7 : 0;
    recessive[i].visible = 0;
  }

  /* Set the fitness to 0 */
  fitness = 0;
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
double Artist::score(const Magick::Image & source)
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
  return fitness;
}