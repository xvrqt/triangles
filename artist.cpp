#include "artist.h"

/* Generates an artist with a random genotype, with only the first
   triangle set as visible.
 */
Artist::Artist(size_t GENOME_LENGTH, std::independent_bits_engine<std::default_random_engine, 8, unsigned char> rand_byte_generator)
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
    dominant[i].visible = (i == 0) ? true : false;
    recessive[i].visible = false;
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
    
    /* Add to an array that we can loop through and break early if the
      Triangles on the dom/rec genes are equal.
     */
    Triangle triangles[2];
    triangles[0] = tri_dom;
    triangles[1] = tri_rec;

    /* Transform the triangles into DrawablePolygons */
    for(size_t j = 0; j < 2; j++)
    {
      Triangle tri = triangles[j];

      /* Don't draw triangles that are not visible */
      if(tri.visible)
      {
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

      /* If the dominant/recessive triangles are the same, don't draw the
        recessive triangle as it is redundant.
       */
      if(tri_dom == tri_rec) { break; }
    }
  }

  /* Draw the triangles! */
  canvas.draw(triangle_list);

  /* Compare to the original */
  canvas.verbose(true);

  /* The current fitness function is only the PPME */
  return canvas.compare(source, Magick::RootMeanSquaredErrorMetric);
}