NAME = triangles
TEST_NAME = test
OBJS = cli.o image.o triangle.o artist.o location.o
CC = clang++
CCFLAGS = -Wall -std=c++11 -Wc++11-extensions -c -O3
IMAGE_MAGICK = `Magick++-config --cxxflags --cppflags --ldflags --libs`

triangles: main.o $(OBJS)
	$(CC) main.o $(OBJS) $(IMAGE_MAGICK) -O3 -o $(NAME)

main.o: main.h main.cpp cli.h image.h artist.h triangle.h location.h
	$(CC) $(CCFLAGS) $(IMAGE_MAGICK) main.cpp

test: test.o $(OBJS)
	$(CC) test.o $(OBJS) $(IMAGE_MAGICK) -g -o $(TEST_NAME)

test.o: test.h test.cpp
	$(CC) $(CCFLAGS) $(IMAGE_MAGICK) test.cpp

cli.o: cli.h cli.cpp
	$(CC) $(CCFLAGS) cli.cpp

image.o: image.h image.cpp
	$(CC) $(CCFLAGS) $(IMAGE_MAGICK) image.cpp

triangle.o: triangle.h triangle.cpp
	$(CC) $(CCFLAGS) triangle.cpp

artist.o: artist.h artist.cpp cli.h image.h triangle.h
	$(CC) $(CCFLAGS) $(IMAGE_MAGICK) artist.cpp

location.o: location.h location.cpp cli.h
	$(CC) $(CCFLAGS) $(IMAGE_MAGICK) location.cpp

# Utility
clean: 
	rm -f *.o $(NAME) $(TEST_NAME) && clear 2> /dev/null
