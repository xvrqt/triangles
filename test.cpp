#include "test.h"

int main(int argc, char ** argv)
{
	/* Not testing cli.h since it has its own tests and it feels more redundant
	   than productive. I'm sure this comment won't come back to haunt me. */
    parseArgs(argc, argv);

    /* Test that we can load/save images, and that there is no distorition */
    Image test_image = openPNG("test_suite/load.png");
    savePNG("test_suite/save.png", test_image);
    /* Test that there is no difference between save.png and load.png */
}
