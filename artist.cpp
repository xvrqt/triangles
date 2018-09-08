#include "artist.h"

/* Generates an artist with a random genotype, with only the first
   triangle set as visible.
 */
Artist::Artist(size_t GENOME_LENGTH, std::independent_bits_engine<std::default_random_engine, 8, unsigned char> rand_byte_generator)
{
  /* Diploid chromosome, each strand takes half the triangles */
  size_t number_of_triangles = GENOME_LENGTH / 2;

  /* Create a byte array to hold the RGB for the background color, and the
     coordinates/colors of all the triangles.
   */
  size_t size_of_bg_rbg = (4 * sizeof(uint8_t));
  size_t size_of_triangles = (number_of_triangles * sizeof(Triangle));
  size_t size_of_genome = size_of_bg_rbg + size_of_triangles;

  chromosome.dominant = (uint8_t *) malloc(size_of_genome);
  chromosome.recessive = (uint8_t *) malloc(size_of_genome);

  /* Randomize the bits in the byte array */
  for(size_t i = 0; i < (size_of_genome) ; i++)
  {
    chromosome.dominant[i] = rand_byte_generator(); 
    chromosome.recessive[i] = rand_byte_generator(); 
  }
}

/* Destructor */
Artist::~Artist()
{
  free(chromosome.dominant);
  free(chromosome.recessive);
}

/* Expresses the genotype, compares it to the submitted image and scores
 * it based on similarity.
 */
double Artist::score(Magick::Image source)
{
  /* Extract the source dimensions */
  std::string source_width(std::to_string(source.columns()));
  std::string source_height(std::to_string(source.rows()));
  std::string image_dimensions = source_width + "x" + source_height;

  /* Extract the RGBA background color from the dominant genome */
  unsigned char * rgba = chromosome.dominant + BG_COLOR_OFFSET;

  /* Create a new canvas, same size as the source with the encoded background 
     color.
   */
  Magick::Color bg_color(rgba[0], rgba[1], rgba[2], rgba[3]);
  Magick::Image canvas(image_dimensions, bg_color);

  canvas.write("testing_resources/test_canvas.png");

  return 0.0;
}