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
	Magick::Color red(127,0,0,127);
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
	canvas.strokeColor("black");
	canvas.strokeAntiAlias(true);
	canvas.strokeWidth(5);
	canvas.fillColor(Magick::Color(tri.r, tri.g, tri.b, tri.a));

	/* Draw the shape */
	Magick::CoordinateList polyline_coordinate_list;
	polyline_coordinate_list.push_back(Magick::Coordinate(0, 0));
	polyline_coordinate_list.push_back(Magick::Coordinate(tri.x2, tri.y2));
	polyline_coordinate_list.push_back(Magick::Coordinate(tri.x3, 0));
	canvas.draw(Magick::DrawablePolygon(polyline_coordinate_list));

	canvas.write("testing_resources/triangle.png");

	/* Test generating random bits and drawing them */
	
}
