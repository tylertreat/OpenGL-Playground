#include <cassert>
#include <iostream>
#include "stb_image.h"
#include "Texture1D.h"

/*
 * Construct Texture1D from file name
 */
Texture1D::Texture1D(
    const char* filename,
    GLenum minFilter,
    GLenum magFilter,
    GLenum wrapS)
    : Texture(GL_TEXTURE_1D),
    minFilter(minFilter),
    magFilter(magFilter),
    wrapS(wrapS),
    dataType(GL_UNSIGNED_BYTE)
{
    // Debug assertions
    assert(filename);
    assert(minFilter == GL_NEAREST                ||
           minFilter == GL_LINEAR                 ||
           minFilter == GL_NEAREST_MIPMAP_NEAREST ||
           minFilter == GL_NEAREST_MIPMAP_LINEAR  ||
           minFilter == GL_LINEAR_MIPMAP_NEAREST  ||
           minFilter == GL_LINEAR_MIPMAP_LINEAR);
    assert(magFilter == GL_NEAREST ||
           magFilter == GL_LINEAR);
    assert(wrapS == GL_CLAMP_TO_EDGE ||
           wrapS == GL_REPEAT        ||
           wrapS == GL_MIRRORED_REPEAT);

    // Load the image from the file
    int height;
    GLubyte* data  = Texture::LoadFile(filename, &width, &height, &imageFormat);
    internalFormat = imageFormat;

    // Fill the texture with pixel data
    InitTextureObject(data);

    // Done with the image data
    free(data);
}

/*
 * Construct Texture1D from raw bytes
 */
Texture1D::Texture1D(
    const GLubyte* data, 
    GLenum format, 
    int width,
    GLenum minFilter,
    GLenum magFilter,
    GLenum wrapS)
    : Texture(GL_TEXTURE_1D),
    imageFormat(format),
    width(width),
    minFilter(minFilter),
    magFilter(magFilter),
    wrapS(wrapS),
    dataType(GL_UNSIGNED_BYTE)
{
    // Debug assertions
    assert(width > 0);
    assert(format == GL_RED  ||
           format == GL_RG   ||
           format == GL_RGB  ||
           format == GL_BGR  ||
           format == GL_RGBA ||
           format == GL_BGRA);
    assert(width > 0);
    assert(minFilter == GL_NEAREST                ||
           minFilter == GL_LINEAR                 ||
           minFilter == GL_NEAREST_MIPMAP_NEAREST ||
           minFilter == GL_NEAREST_MIPMAP_LINEAR  ||
           minFilter == GL_LINEAR_MIPMAP_NEAREST  ||
           minFilter == GL_LINEAR_MIPMAP_LINEAR);
    assert(magFilter == GL_NEAREST ||
           magFilter == GL_LINEAR);
    assert(wrapS == GL_CLAMP_TO_EDGE ||
           wrapS == GL_REPEAT        ||
           wrapS == GL_MIRRORED_REPEAT);

    // Figure out the internal format to use for the texture
    switch(format)
    {
    case GL_BGR:
        internalFormat = GL_RGB;
        break;
    case GL_BGRA:
        internalFormat = GL_RGBA;
        break;
    default:
        internalFormat = format;
    }

    // Fill the texture with pixel data
    InitTextureObject(data);
}

/*
 * Construct Texture1D from raw floats
 */
Texture1D::Texture1D(
    const GLfloat* data, 
    GLenum format, 
    int width,
    GLenum minFilter,
    GLenum magFilter,
    GLenum wrapS)
    : Texture(GL_TEXTURE_1D),
    imageFormat(format),
    width(width),
    minFilter(minFilter),
    magFilter(magFilter),
    wrapS(wrapS),
    dataType(GL_FLOAT)
{
    // Debug assertions
    assert(width > 0);
    assert(format == GL_RED  ||
           format == GL_RG   ||
           format == GL_RGB  ||
           format == GL_BGR  ||
           format == GL_RGBA ||
           format == GL_BGRA);
    assert(width > 0);
    assert(minFilter == GL_NEAREST                ||
           minFilter == GL_LINEAR                 ||
           minFilter == GL_NEAREST_MIPMAP_NEAREST ||
           minFilter == GL_NEAREST_MIPMAP_LINEAR  ||
           minFilter == GL_LINEAR_MIPMAP_NEAREST  ||
           minFilter == GL_LINEAR_MIPMAP_LINEAR);
    assert(magFilter == GL_NEAREST ||
           magFilter == GL_LINEAR);
    assert(wrapS == GL_CLAMP_TO_EDGE ||
           wrapS == GL_REPEAT        ||
           wrapS == GL_MIRRORED_REPEAT);

    // Figure out the internal format to use for the texture
    switch(format)
    {
    case GL_BGR:
        internalFormat = GL_RGB;
        break;
    case GL_BGRA:
        internalFormat = GL_RGBA;
        break;
    default:
        internalFormat = format;
    }

    // Fill the texture with pixel data
    InitTextureObject(data);
}

/*
 * Destructor
 */
Texture1D::~Texture1D()
{
    // Nothing to do here, it is all handled in the base destructor
}

/*
 * Init texture object
 */
void Texture1D::InitTextureObject(const GLvoid* pixels)
{
    // Set texture object info, reading into the image buffer
    glTexImage1D(
        GL_TEXTURE_1D, 
        0,
        internalFormat, 
        width,
        0, 
        imageFormat, 
        dataType, 
        pixels);

    // Generate mip maps if necessary
    if (pixels != NULL &&
        (minFilter == GL_NEAREST_MIPMAP_NEAREST ||
         minFilter == GL_NEAREST_MIPMAP_LINEAR  ||
         minFilter == GL_LINEAR_MIPMAP_NEAREST  ||
         minFilter == GL_LINEAR_MIPMAP_LINEAR))
    {
        glGenerateMipmap(GL_TEXTURE_1D);
    }
}

/*
 * Set texture parameters
 */
void Texture1D::SetTextureParams()
{
    glTexParameteri(GL_TEXTURE_1D, GL_TEXTURE_MIN_FILTER, minFilter);
    glTexParameteri(GL_TEXTURE_1D, GL_TEXTURE_MAG_FILTER, magFilter);
    glTexParameteri(GL_TEXTURE_1D, GL_TEXTURE_WRAP_S, wrapS);
}
