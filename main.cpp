#include "main.h"

int main(int argc, char ** argv)
{	
	/* Turns command line arguments into hyper parameters */
	parseArgs(argc, argv);
	/* Allows use of ImageMagick */
	Magick::InitializeMagick(*argv);

	/* Open the Image */
	Magick::Image source = openImage(IMAGE_PATH);

	/* Generate a list of Artists */
	


}
