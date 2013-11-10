#if !defined(INCLUDE_IMAGES_H)
#define INCLUDE_IMAGES_H

#include <stb_image.h>
#include <Angel.h>

GLubyte * loadFile(char * filename, int * nWidth, int * nHeight, int * nComponents, GLenum * eFormat)
{
  int comp;
  GLubyte * temp = stbi_load(filename, nWidth, nHeight, &comp, 0);
  if (temp == NULL)
  {
    std::cout << "Unable to load image file " << filename << std::endl;
    exit(1);
  }
  if (comp == 3)
  {
    *nComponents = GL_RGB;
    *eFormat = GL_RGB;
  }
  else if (comp == 4)
  {
    *nComponents = GL_RGBA;
    *eFormat = GL_RGBA;
  }
  else
  {
    std::cout << "Image is not RGB or RGBA: " << filename << std::endl;
    exit(1);
  }

  // Images are upside down
  int width = *nWidth;
  int height = *nHeight;
  GLubyte * swap = (GLubyte *) malloc(width * comp);
  for (int i = 0, j = height - 1; i < height / 2; ++i, --j)
  {
    memcpy(swap, temp + i * width * comp, width * comp);
    memcpy(temp + i * width * comp, temp + j * width * comp, width * comp);
    memcpy(temp + j * width * comp, swap, width * comp);
  }
  free(swap);
  return temp;
}


GLubyte * checkerboard(int * nWidth, int * nHeight, int * nComponents, GLenum * eFormat)
{
  const int  size = 64;
  const int comp = 3;
  GLubyte * image = (GLubyte *) malloc(size * size * comp);
  for ( int i = 0; i < size; i++ ) {
    for ( int j = 0; j < size; j++ ) {
      GLubyte c = (((i & 0x8) == 0) ^ ((j & 0x8)  == 0)) * 255;
      image[i * size * comp + j * comp] = c;
      image[i * size * comp + j * comp + 1] = c;
      image[i * size * comp + j * comp + 2] = c;
    }
  }
  
  *nWidth = *nHeight = size;
  *nComponents = GL_RGB;
  *eFormat = GL_RGB;
  return image;
}

GLubyte * solid(vec4 color, int size, int * nWidth, int * nHeight, int * nComponents, GLenum * eFormat)
{
  //const int  size = 64;
  const int comp = 3;
  GLubyte * image = (GLubyte *) malloc(size * size * comp);
  for ( int i = 0; i < size; i++ ) {
    for ( int j = 0; j < size; j++ ) {
      GLubyte c = (((i & 0x8) == 0) ^ ((j & 0x8)  == 0)) * 255;
      image[i * size * comp + j * comp] = (GLubyte) color[0] * 255;
      image[i * size * comp + j * comp + 1] = (GLubyte) color[1] * 255;
      image[i * size * comp + j * comp + 2] = (GLubyte) color[2] * 255;
    }
  }
  
  *nWidth = *nHeight = size;
  *nComponents = GL_RGB;
  *eFormat = GL_RGB;
  return image;
}


GLubyte * checkerboard2(int * nWidth, int * nHeight, int * nComponents, GLenum * eFormat)
{
  const int  size = 64;
  const int border = 1;
  const int comp = 3;
  GLubyte * image = (GLubyte *) malloc(size * size * comp);
  for ( int i = 0; i < size; i++ ) {
    for ( int j = 0; j < size; j++ ) {
      GLubyte c = (((i & 0x8) == 0) ^ ((j & 0x8)  == 0)) * 255;
      GLubyte r, g, b;
      if (i < border || j < border || i >= size - border || j >= size - border)
      {
        r = 255;
        g = 0; 
        b = 0;
      }
      else
      {
        r = g = b = c;
      }

      image[i * size * comp + j * comp] = r;
      image[i * size * comp + j * comp + 1] = g;
      image[i * size * comp + j * comp + 2] = b;
    }
  }
  *nWidth = *nHeight = size;
  *nComponents = GL_RGB;
  *eFormat = GL_RGB;
  return image;
}

GLubyte * transparentCheckerboard(int * nWidth, int * nHeight, int * nComponents, GLenum * eFormat)
{
  const int  size = 64;
  const int comp = 4;
  GLubyte * image = (GLubyte *) malloc(size * size * comp);
  for ( int i = 0; i < size; i++ ) {
    for ( int j = 0; j < size; j++ ) {
      GLubyte c = (((i & 0x8) == 0) ^ ((j & 0x8)  == 0)) * 255;
      image[i * size * comp + j * comp] = c;
      image[i * size * comp + j * comp + 1] = 0;
      image[i * size * comp + j * comp + 2] = 0;
      image[i * size * comp + j * comp + 3] = c / 1.5;
    }
  }
  
  *nWidth = *nHeight = size;
  *nComponents = GL_RGBA;
  *eFormat = GL_RGBA;
  return image;
}


#endif // INCLUDE_IMAGES_H