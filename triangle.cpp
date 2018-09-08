#include "triangle.h"

/* Mark important _byte_ offsets */
size_t BG_COLOR_OFFSET = 0;
size_t BG_COLOR_SIZE = 4;
size_t TRIANGLE_LIST_BEGIN = BG_COLOR_SIZE;
size_t TRIANGLE_SIZE = sizeof(Triangle);
