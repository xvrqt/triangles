#include "test.h"

int main(int argc, char ** argv)
{
	/* Not testing cli.h since it has its own tests and it feels more redundant
	   than productive. I'm sure this comment won't come back to haunt me. 
	 */
    parseArgs(argc, argv);

    /* Allows use of ImageMagick */
	Magick::InitializeMagick(*argv);

	/* Test Image Library */
	/* Image too big -> should save as a smaller size and warn user */
	Magick::Image source = openImage("testing_resources/too_big.png");
	/* Save an image */
	source.write("testing_resources/copy_of_too_big.png");
	/* Copy the dimensions */
	std::string source_width(std::to_string(source.columns()));
	std::string source_height(std::to_string(source.rows()));
	std::string image_dimensions = source_width + "x" + source_height;

	/* Create a new canvas, same size as the source, but bright red */
	Magick::Color red(255,0,0,255);
	Magick::Image canvas(image_dimensions, red);
	canvas.write("testing_resources/blank_canvas.png");

	/* Test drawing a triangle. The 128 should over flow so this should be blue
	   in color, half transparent, centered triangle, like an envelope.
	 */
	Triangle tri;
	tri.r = 128;
	tri.g = 128;
	tri.b = 127;
	tri.a = 100;

	tri.x1 = 0;
	tri.y1 = 0;

	tri.x2 = canvas.columns() / 2;
	tri.y2 = canvas.rows() /2;

	tri.x3 = canvas.columns() - 1; /* Over flow at 1024 */
	tri.y3 = 0;

	/* Drawing settings */
	Magick::Color tri_color(tri.r, tri.g, tri.b, tri.a);
	
	canvas.fillColor(tri_color);
	canvas.strokeWidth(0);
	canvas.strokeColor(tri_color);
	canvas.strokeAntiAlias(true);

	/* Draw the shape */
	Magick::CoordinateList polyline_coordinate_list;
	polyline_coordinate_list.push_back(Magick::Coordinate(0, 0));
	polyline_coordinate_list.push_back(Magick::Coordinate(tri.x2, tri.y2));
	polyline_coordinate_list.push_back(Magick::Coordinate(tri.x3, 0));
	canvas.draw(Magick::DrawablePolygon(polyline_coordinate_list));

	canvas.write("testing_resources/triangle.png");

	/* Test that triangle equality is working */
	Triangle t = {0,0,0,0,0,0,0,0,0,0};
	Triangle t_clone = {0,0,0,0,0,0,0,0,0,0};
	Triangle t_diff = {0,0,0,0,0,1,0,0,0,0}; /* Has y3 = 1 instead of 0 */

	assert(t == t_clone);
	assert(!(t == t_diff));

	/* Test generating random bits and drawing them */
	/* Check packing of triangle struct */
	if(sizeof(Triangle) != 12) { std::cout << "Triangle is not packed correctly; is " << sizeof(Triangle) << " bytes, should be 12."; }

	/* Setup the random bit engines */
    std::default_random_engine rand_engine(RANDOM_SEED);
	std::independent_bits_engine<std::default_random_engine, 8, unsigned char> rand_byte_generator(rand_engine);
	
	/* Generate a random artist, calculate the fitness, and then delete it */
	{
		Artist a(GENOME_LENGTH, rand_byte_generator);
	}

	/* Generate a random artist and then draw it */
	Artist a(GENOME_LENGTH, rand_byte_generator);
	a.score(source);

}
