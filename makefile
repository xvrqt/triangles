NAME = triangles
TEST_NAME = test
OBJS = cli.o image.o lodepng.o
CC = clang++
CCFLAGS = -Wall -std=c++11 -Wc++11-extensions -c
IMAGE_MAGICK = `Magick++-config --cxxflags --cppflags --ldflags --libs`

triangles: main.o $(OBJS)
	$(CC) main.o $(OBJS) $(LFLAGS) -o $(NAME)

test: test.o $(OBJS)
	$(CC) test.o $(OBJS) $(LFLAGS) -g -o $(TEST_NAME)

test.o: test.h test.cpp
	$(CC) $(CCFLAGS) $(IMAGE_MAGICK) test.cpp

main.o: main.h main.cpp
	$(CC) $(CCFLAGS) main.cpp

cli.o: cli.h cli.cpp
	$(CC) $(CCFLAGS) cli.cpp

image.o: image.h image.cpp
	$(CC) $(CCFLAGS) image.cpp

lodepng.o: lodepng.h lodepng.cpp
	$(CC) $(CCFLAGS) lodepng.cpp

# Utility
clean: 
	rm -f *.o $(NAME) $(TEST_NAME) && clear 2> /dev/null
