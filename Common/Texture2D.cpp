#include <cassert>
#include <iostream>
#include "stb_image.h"
#include "Texture2D.h"

/*
 * Construct Texture2D from file name
 */
Texture2D::Texture2D(
    const char* filename,
    GLenum minFilter,
    GLenum magFilter,
    GLenum wrapS,
    GLenum wrapT,
    float  aniso)
    : Texture(GL_TEXTURE_2D),
    minFilter(minFilter),
    magFilter(magFilter),
    wrapS(wrapS),
    wrapT(wrapT),
    aniso(aniso),
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
    assert(wrapT == GL_CLAMP_TO_EDGE ||
           wrapT == GL_REPEAT        ||
           wrapT == GL_MIRRORED_REPEAT);
    assert(aniso >= 1.0f);

    // Load the image from the file
    GLubyte* data  = Texture::LoadFile(filename, &width, &height, &imageFormat);
    internalFormat = imageFormat;

    // Fill the texture with pixel data
    InitTextureObject(data);

    // Done with the image data
    free(data);
}

/*
 * Construct Texture2D from raw bytes
 */
Texture2D::Texture2D(
    const GLubyte* data, 
    GLenum format, 
    int width, 
    int height,
    GLenum minFilter,
    GLenum magFilter,
    GLenum wrapS,
    GLenum wrapT,
    float  aniso)
    : Texture(GL_TEXTURE_2D),
    imageFormat(format),
    width(width),
    height(height),
    minFilter(minFilter),
    magFilter(magFilter),
    wrapS(wrapS),
    wrapT(wrapT),
    aniso(aniso),
    dataType(GL_UNSIGNED_BYTE)
{
    // Debug assertions
    assert(format == GL_RED  ||
           format == GL_RG   ||
           format == GL_RGB  ||
           format == GL_BGR  ||
           format == GL_RGBA ||
           format == GL_BGRA);
    assert(width > 0 && height > 0);
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
    assert(wrapT == GL_CLAMP_TO_EDGE ||
           wrapT == GL_REPEAT        ||
           wrapT == GL_MIRRORED_REPEAT);
    assert(aniso >= 1.0f);

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
 * Construct Texture2D from raw floats
 */
Texture2D::Texture2D(
    const GLfloat* data, 
    GLenum format, 
    int width, 
    int height,
    GLenum minFilter,
    GLenum magFilter,
    GLenum wrapS,
    GLenum wrapT,
    float  aniso)
    : Texture(GL_TEXTURE_2D),
    imageFormat(format),
    width(width),
    height(height),
    minFilter(minFilter),
    magFilter(magFilter),
    wrapS(wrapS),
    wrapT(wrapT),
    aniso(aniso),
    dataType(GL_FLOAT)
{
    // Debug assertions
    assert(format == GL_RED  ||
           format == GL_RG   ||
           format == GL_RGB  ||
           format == GL_BGR  ||
           format == GL_RGBA ||
           format == GL_BGRA);
    assert(width > 0 && height > 0);
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
    assert(wrapT == GL_CLAMP_TO_EDGE ||
           wrapT == GL_REPEAT        ||
           wrapT == GL_MIRRORED_REPEAT);
    assert(aniso >= 1.0f);

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
 * Empty Texture2D constructor
 */
Texture2D::Texture2D(
    GLenum internalFormat,
    GLenum sourceFormat,
    GLenum dataType,
    int width, 
    int height,
    GLenum minFilter,
    GLenum magFilter,
    GLenum wrapS,
    GLenum wrapT,
    float  aniso)
    : Texture(GL_TEXTURE_2D),
    internalFormat(internalFormat),
    imageFormat(sourceFormat),
    dataType(dataType),
    width(width),
    height(height),
    minFilter(minFilter),
    magFilter(magFilter),
    wrapS(wrapS),
    wrapT(wrapT),
    aniso(aniso)
{
    // Debug assertions
    assert(width > 0 && height > 0);
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
    assert(wrapT == GL_CLAMP_TO_EDGE ||
           wrapT == GL_REPEAT        ||
           wrapT == GL_MIRRORED_REPEAT);
    assert(aniso >= 1.0f);

    // Create empty texture object
    InitTextureObject(NULL);
}

/*
 * Destructor
 */
Texture2D::~Texture2D()
{
    // Nothing to do here, it is all handled in the base destructor
}

/*
 * Init texture object
 */
void Texture2D::InitTextureObject(const GLvoid* pixels)
{
    // Set texture object info, reading into the image buffer
    glTexImage2D(
        GL_TEXTURE_2D, 
        0,
        internalFormat, 
        width, 
        height, 
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
        glGenerateMipmap(GL_TEXTURE_2D);
    }
}

/*
 * Set texture parameters
 */
void Texture2D::SetTextureParams()
{
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, minFilter);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, magFilter);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, wrapS);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, wrapT);

    // Set aniso level if enabled
    if (aniso > 1.0f)
    {
        glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAX_ANISOTROPY_EXT, aniso);
    }
}
