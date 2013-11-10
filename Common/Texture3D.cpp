#include <cassert>
#include <iostream>
#include "Texture3D.h"


/*
 * Construct Texture3D from file names
 */
Texture3D::Texture3D(
    const char* const * filenames,
    int    depth,
    GLenum minFilter,
    GLenum magFilter,
    GLenum wrapS,
    GLenum wrapT,
    GLenum wrapR,
    float  aniso)
    : Texture(GL_TEXTURE_3D),
    depth(depth),
    minFilter(minFilter),
    magFilter(magFilter),
    wrapS(wrapS),
    wrapT(wrapT),
    wrapR(wrapR),
    aniso(aniso),
    dataType(GL_UNSIGNED_BYTE)
{
    // Debug assertions
    assert(filenames != NULL);
    assert(depth > 0);
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
    assert(wrapR == GL_CLAMP_TO_EDGE ||
           wrapR == GL_REPEAT        ||
           wrapR == GL_MIRRORED_REPEAT);
    assert(aniso >= 1.0f);

    // Create array of image data pointers
    GLubyte** data = new GLubyte*[depth];

    // For each layer
    for (int i = 0; i < depth; i++)
    {
        // Load the image from the file
        assert(filenames[i]);
        data[i] = Texture::LoadFile(filenames[i], &width, &height, &imageFormat);
        internalFormat = imageFormat;
    }

    // Initialize the texture with the image data
    InitTextureObject((const GLvoid* const *)data);

    // Free image data
    for (int i = 0; i < depth; i++)
    {
        free(data[i]);
    }

    // Free image data pointer array
    delete[] data;
}

/*
 * Construct Texture3D from non contiguous raw bytes
 */
Texture3D::Texture3D(
    const GLubyte* const * data,
    GLenum format, 
    int width, 
    int height,
    int depth,
    GLenum minFilter,
    GLenum magFilter,
    GLenum wrapS,
    GLenum wrapT,
    GLenum wrapR,
    float  aniso)
    : Texture(GL_TEXTURE_3D),
    imageFormat(format),
    width(width),
    height(height),
    depth(depth),
    minFilter(minFilter),
    magFilter(magFilter),
    wrapS(wrapS),
    wrapT(wrapT),
    wrapR(wrapR),
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
    assert(width > 0 && height > 0 && depth > 0);
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
    assert(wrapR == GL_CLAMP_TO_EDGE ||
           wrapR == GL_REPEAT        ||
           wrapR == GL_MIRRORED_REPEAT);
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

    // Load the image data
    InitTextureObject((const GLvoid* const *)data);
}

/*
 * Construct Texture3D from non contiguous raw floats
 */
Texture3D::Texture3D(
    const GLfloat* const * data,
    GLenum format, 
    int width, 
    int height,
    int depth,
    GLenum minFilter,
    GLenum magFilter,
    GLenum wrapS,
    GLenum wrapT,
    GLenum wrapR,
    float  aniso)
    : Texture(GL_TEXTURE_3D),
    imageFormat(format),
    width(width),
    height(height),
    depth(depth),
    minFilter(minFilter),
    magFilter(magFilter),
    wrapS(wrapS),
    wrapT(wrapT),
    wrapR(wrapR),
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
    assert(width > 0 && height > 0 && depth > 0);
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
    assert(wrapR == GL_CLAMP_TO_EDGE ||
           wrapR == GL_REPEAT        ||
           wrapR == GL_MIRRORED_REPEAT);
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

    // Load the image data
    InitTextureObject((const GLvoid* const *)data);
}

/*
 * Construct Texture3D from contiguous raw bytes
 */
Texture3D::Texture3D(
    const GLubyte* data,
    GLenum format, 
    int width, 
    int height,
    int depth,
    GLenum minFilter,
    GLenum magFilter,
    GLenum wrapS,
    GLenum wrapT,
    GLenum wrapR,
    float  aniso)
    : Texture(GL_TEXTURE_3D),
    imageFormat(format),
    width(width),
    height(height),
    depth(depth),
    minFilter(minFilter),
    magFilter(magFilter),
    wrapS(wrapS),
    wrapT(wrapT),
    wrapR(wrapR),
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
    assert(width > 0 && height > 0 && depth > 0);
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
    assert(wrapR == GL_CLAMP_TO_EDGE ||
           wrapR == GL_REPEAT        ||
           wrapR == GL_MIRRORED_REPEAT);
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

    // Load the image data
    InitTextureObject(data);
}

/*
 * Construct Texture3D from contiguous raw floats
 */
Texture3D::Texture3D(
    const GLfloat* data,
    GLenum format, 
    int width, 
    int height,
    int depth,
    GLenum minFilter,
    GLenum magFilter,
    GLenum wrapS,
    GLenum wrapT,
    GLenum wrapR,
    float  aniso)
    : Texture(GL_TEXTURE_3D),
    imageFormat(format),
    width(width),
    height(height),
    depth(depth),
    minFilter(minFilter),
    magFilter(magFilter),
    wrapS(wrapS),
    wrapT(wrapT),
    wrapR(wrapR),
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
    assert(width > 0 && height > 0 && depth > 0);
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
    assert(wrapR == GL_CLAMP_TO_EDGE ||
           wrapR == GL_REPEAT        ||
           wrapR == GL_MIRRORED_REPEAT);
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

    // Load the image data
    InitTextureObject(data);
}

/*
 * Empty Texture3D constructor
 */
Texture3D::Texture3D(
    GLenum internalFormat,
    GLenum sourceFormat,
    GLenum dataType,
    int width, 
    int height,
    int depth,
    GLenum minFilter,
    GLenum magFilter,
    GLenum wrapS,
    GLenum wrapT,
    GLenum wrapR,
    float  aniso)
    : Texture(GL_TEXTURE_3D),
    internalFormat(internalFormat),
    imageFormat(sourceFormat),
    dataType(dataType),
    width(width),
    height(height),
    depth(depth),
    minFilter(minFilter),
    magFilter(magFilter),
    wrapS(wrapS),
    wrapT(wrapT),
    wrapR(wrapR),
    aniso(aniso)
{
    // Debug assertions
    assert(width > 0 && height > 0 && depth > 0);
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
    assert(wrapR == GL_CLAMP_TO_EDGE ||
           wrapR == GL_REPEAT        ||
           wrapR == GL_MIRRORED_REPEAT);
    assert(aniso >= 1.0f);

    // Initialize the texture object with no data
    InitTextureObject((const GLvoid*)NULL);
}

/*
 * Destructor
 */
Texture3D::~Texture3D()
{
    // Nothing to do here, it is all handled in the base destructor
}

/*
 * Init texture object with non contiguous data
 */
void Texture3D::InitTextureObject(const GLvoid* const * pixels)
{
    // Create empty 3D texture
    glTexImage3D(
        GL_TEXTURE_3D,
        0,
        internalFormat, 
        width, 
        height,
        depth,
        0, 
        imageFormat, 
        dataType, 
        NULL);

    if (pixels != NULL)
    {
        // Load each layer individually, allows layers to not be contiguous
        for (int i = 0; i < depth; i++)
        {
            assert(pixels[i] != NULL);

            glTexSubImage3D(
                GL_TEXTURE_3D,
                0,
                0,
                0,
                i,
                width,
                height,
                1,
                imageFormat,
                dataType,
                pixels[i]);
        }

        // Generate mip maps after all layers loaded, if necessary
        if (minFilter == GL_NEAREST_MIPMAP_NEAREST ||
             minFilter == GL_NEAREST_MIPMAP_LINEAR  ||
             minFilter == GL_LINEAR_MIPMAP_NEAREST  ||
             minFilter == GL_LINEAR_MIPMAP_LINEAR)
        {
            glGenerateMipmap(GL_TEXTURE_3D);
        }
    }
}

/*
 * Init texture object
 */
void Texture3D::InitTextureObject(const GLvoid* pixels)
{
    // Initialize 3D texture
    glTexImage3D(
        GL_TEXTURE_3D,
        0,
        internalFormat, 
        width, 
        height,
        depth,
        0, 
        imageFormat, 
        dataType, 
        pixels);

    // Generate mip maps after all layers loaded, if necessary
    if (pixels != NULL &&
        (minFilter == GL_NEAREST_MIPMAP_NEAREST ||
        minFilter == GL_NEAREST_MIPMAP_LINEAR  ||
        minFilter == GL_LINEAR_MIPMAP_NEAREST  ||
        minFilter == GL_LINEAR_MIPMAP_LINEAR))
    {
        glGenerateMipmap(GL_TEXTURE_3D);
    }
}

/*
 * Set texture parameters
 */
void Texture3D::SetTextureParams()
{
    glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_MIN_FILTER, minFilter);
    glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_MAG_FILTER, magFilter);
    glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_WRAP_S, wrapS);
    glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_WRAP_T, wrapT);
    glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_WRAP_R, wrapR);

    // Set aniso level if enabled
    if (aniso > 1.0f)
    {
        glTexParameterf(GL_TEXTURE_3D, GL_TEXTURE_MAX_ANISOTROPY_EXT, aniso);
    }
}
