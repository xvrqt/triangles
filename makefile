NAME = triangles
TEST_NAME = test
OBJS = cli.o image.o triangle.o artist.o location.o

# Determine which OS we're on to determine the compiler
OS := $(shell uname -s)
ifeq ($(OS), Darwin)
        CC = clang++
endif
ifeq ($(OS), Linux)
        CC = g++
endif

CCFLAGS = -Wall -std=c++11 -c -O3
IMAGE_MAGICK := $(shell Magick++-config --cxxflags --cppflags --ldflags --libs)
OPEN_CV := $(pkg-config --cflags --libs opencv)

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
