#include "artist.h"

Image Artist::original;
size_t Artist::num_shapes;

/* Create an artist with a random list of triangles */
Artist::Artist()
{
  std::cout << "%" << std::endl;
  for(size_t i = 0; i < Artist::num_shapes; i++)
  {
    size_t max_width = Artist::original.width;
    size_t max_height = Artist::original.height;
    triangles.push_back(Triangle(max_width, max_height));
  }

  /* Allocate a canvas to draw to */
  size_t size = Artist::original.width * Artist::original.height;
  canvas.resize(size);
}

/* Create an artist from an existing list of triangles */
Artist::Artist(std::vector<Triangle> &prototype)
{
  triangles.assign(prototype.begin(), prototype.end());

  /* Randomize starting triangle */
  triangle_index = rand() % Artist::num_shapes;

  /* Allocate a canvas to draw to */
  size_t size = Artist::original.width * Artist::original.height;
  canvas.resize(size);
}

Artist::Artist(const Artist &a)
{
  /* Copy the triangles */
  triangles.assign(a.triangles.begin(), a.triangles.end());
  distance = a.getDistance();

  /* Create a new canvas on copy */
  canvas.assign(a.canvas.begin(), a.canvas.end());
}

Artist::~Artist()
{
}

/* Returns and Image of the artist's best rendition so far. */
Image Artist::getImageCopy()
{
  Image img;
  img.width = Artist::original.width;
  img.height = Artist::original.height;

  /* Allocate new array for the Image copy */
  size_t size = Artist::original.width * Artist::original.height * sizeof(RGB);
  img.data = (RGB *)malloc(size);

  /* Copy from the vector */
  size_t data_index = 0;
  for(auto it = canvas.begin(); it != canvas.end(); ++it)
  {
    img.data[data_index++] = (*it);
  }
  
  return img;
}

/* Allows us to sort without copying */
bool Artist::operator <(const Artist &a) const
{
  return distance < a.getDistance();
}

/* Takes two Artist's and cross co indexed triangles */
void Artist::crossover(Artist &a, Artist &mate)
{
  // unsigned long a_dist = a.getDistance();
  // unsigned long mate_dist = mate.getDistance();
  // double ratio = (double)a_dist / (a_dist + mate_dist);

  size_t i = rand() % num_shapes;

  // double aormate = (double)rand() / RAND_MAX; 
  // if(aormate < ratio) /* A gives its triangle to Mate */
  // {
    mate.setTriangle(i, a.getTriangle(i));
    mate.calcDistance();
  // }
  // else /* Mate gives its triangle to A */
  // {
  //   a.setTriangle(i, mate.getTriangle(i));
  //   a.calcDistance();
  // }
}

/* Returns a copy of a Triangle at a specified index */
Triangle Artist::getTriangle(size_t index)
{
  if(index >= Artist::num_shapes)
  {
    std::cerr << "Tried to access out of index triangle." << std::endl;
    return Triangle(1,1);
  }
  return triangles[index];
}

/* Sets a triangle at a specific index */
void Artist::setTriangle(size_t index, Triangle t)
{
  if(index >= Artist::num_shapes)
  {
    std::cerr << "Tried to access out of index triangle." << std::endl;
  }
  triangles[index] = t;
}

/* Set the number of shapes */
void Artist::setNumberOfShapes(size_t num)
{
  Artist::num_shapes = num;
}

/* Set the static original image data used to compare drawings to */
void Artist::setOriginalImage(Image image)
{
  original = image;
}

/* Returns an artist's fitness */
unsigned long Artist::getDistance() const
{
  return distance;
}

void Artist::mutate()
{
    /* Mutate a triangle in the list */
    triangles[triangle_index].mutate();

    triangle_index += (rand() % 10);
    triangle_index %= Artist::num_shapes;

    std::sort(triangles.begin(), triangles.end());
}

/* Calculates the image's distance */
void Artist::calcDistance()
{
  std::sort(triangles.begin(), triangles.end());

  size_t width = Artist::original.width;
  size_t height = Artist::original.height;
  size_t size = width * height;

  /* Clear the canvas */
  for(auto it = canvas.begin(); it != canvas.end(); ++it)
  {
    (*it).r = (unsigned char)BACKGROUND_COLOR;
    (*it).g = (unsigned char)BACKGROUND_COLOR;
    (*it).b = (unsigned char)BACKGROUND_COLOR;
  }

  /* Draw the triangles to the canvas */
  for(auto it = triangles.begin(); it != triangles.end(); ++it)
  {
    (*it).draw(canvas);
  }

  /* Calculate the distance between this array and the original */
  unsigned long new_dist = 0;
  for(size_t i = 0; i < size; i++)
  {
    RGB c = canvas[i];
    RGB o = original.data[i];

    size_t r_diff = (c.r > o.r) ? (c.r - o.r) : (o.r - c.r); 
    size_t g_diff = (c.g > o.g) ? (c.g - o.g) : (o.g - c.g); 
    size_t b_diff = (c.b > o.b) ? (c.b - o.b) : (o.b - c.b); 

    new_dist += (unsigned long)(r_diff * r_diff);
    new_dist += (unsigned long)(g_diff * g_diff);
    new_dist += (unsigned long)(b_diff * b_diff);
  }
  distance = new_dist;
}
