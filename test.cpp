#include "test.h"

int main(int argc, char ** argv)
{
	/* Not testing cli.h since it has its own tests and it feels more redundant
	   than productive. I'm sure this comment won't come back to haunt me. */
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

	/* Create a new canvas, same size as the source. */
	Magick::Image canvas(image_dimensions, "white");
	canvas.write("testing_resources/blank_canvas.png");

	/* Test drawing several triangles */
}
