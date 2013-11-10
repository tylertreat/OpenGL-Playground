#include <cassert>
#include <iostream>
#include "stb_image.h"
#include "TextureCube.h"

/*
 * Face enum values, for convenience
 */
const GLenum TextureCube::faceEnums[] =
{
    GL_TEXTURE_CUBE_MAP_POSITIVE_X,
    GL_TEXTURE_CUBE_MAP_NEGATIVE_X,
    GL_TEXTURE_CUBE_MAP_POSITIVE_Y,
    GL_TEXTURE_CUBE_MAP_NEGATIVE_Y,
    GL_TEXTURE_CUBE_MAP_POSITIVE_Z,
    GL_TEXTURE_CUBE_MAP_NEGATIVE_Z,
};

/*
 * Construct TextureCube from file names
 */
TextureCube::TextureCube(
    const char* posXFilename,
    const char* negXFilename,
    const char* posYFilename,
    const char* negYFilename,
    const char* posZFilename,
    const char* negZFilename,
    GLenum minFilter,
    GLenum magFilter,
    GLenum wrapS,
    GLenum wrapT,
    GLenum wrapR,
    float  aniso)
    : Texture(GL_TEXTURE_CUBE_MAP),
    minFilter(minFilter),
    magFilter(magFilter),
    wrapS(wrapS),
    wrapT(wrapT),
    wrapR(wrapR),
    aniso(aniso),
    dataType(GL_UNSIGNED_BYTE)
{
    // Debug assertions
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

    // For ease of use, put the filenames in a single array so we can loop
    // over them
    const char* filenames[] =
    {
        posXFilename,
        negXFilename,
        posYFilename,
        negYFilename,
        posZFilename,
        negZFilename
    };

    // For each cube face
    for (int i = 0; i < 6; i++)
    {
        assert(filenames[i]);

        // Load the image from the file
        GLubyte* data  = Texture::LoadFile(filenames[i], &width, &height, &imageFormat);
        internalFormat = imageFormat;

        // Fill the face texture with pixel data
        InitTextureObject(data, faceEnums[i]);

        // Done with the image data
        free(data);
    }
}

/*
 * Construct TextureCube from raw bytes
 */
TextureCube::TextureCube(
    const GLubyte* posXData, 
    const GLubyte* negXData,
    const GLubyte* posYData,
    const GLubyte* negYData,
    const GLubyte* posZData,
    const GLubyte* negZData,
    GLenum format, 
    int width, 
    int height,
    GLenum minFilter,
    GLenum magFilter,
    GLenum wrapS,
    GLenum wrapT,
    GLenum wrapR,
    float  aniso)
    : Texture(GL_TEXTURE_CUBE_MAP),
    imageFormat(format),
    width(width),
    height(height),
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

    // For ease of use, make an array of pixel data pointers
    // so we can loop over them
    const GLubyte* data[] =
    {
        posXData,
        negXData,
        posYData,
        negYData,
        posZData,
        negZData
    };

    // For each face
    for (int i = 0; i < 6; i++)
    {
        // Fill the face texture with pixel data
        InitTextureObject(data[i], faceEnums[i]);
    }
}

/*
 * Construct TextureCube from raw floats
 */
TextureCube::TextureCube(
    const GLfloat* posXData,
    const GLfloat* negXData,
    const GLfloat* posYData,
    const GLfloat* negYData,
    const GLfloat* posZData,
    const GLfloat* negZData,
    GLenum format, 
    int width, 
    int height,
    GLenum minFilter,
    GLenum magFilter,
    GLenum wrapS,
    GLenum wrapT,
    GLenum wrapR,
    float  aniso)
    : Texture(GL_TEXTURE_CUBE_MAP),
    imageFormat(format),
    width(width),
    height(height),
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

    // For ease of use, make an array of pixel data pointers
    // so we can loop over them
    const GLfloat* data[] =
    {
        posXData,
        negXData,
        posYData,
        negYData,
        posZData,
        negZData
    };

    // For each face
    for (int i = 0; i < 6; i++)
    {
        // Fill the face texture with pixel data
        InitTextureObject(data[i], faceEnums[i]);
    }
}

/*
 * Empty TextureCube constructor
 */
TextureCube::TextureCube(
    GLenum internalFormat,
    GLenum sourceFormat,
    GLenum dataType,
    int width, 
    int height,
    GLenum minFilter,
    GLenum magFilter,
    GLenum wrapS,
    GLenum wrapT,
    GLenum wrapR,
    float  aniso)
    : Texture(GL_TEXTURE_CUBE_MAP),
    internalFormat(internalFormat),
    imageFormat(sourceFormat),
    dataType(dataType),
    width(width),
    height(height),
    minFilter(minFilter),
    magFilter(magFilter),
    wrapS(wrapS),
    wrapT(wrapT),
    wrapR(wrapR),
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
    assert(wrapR == GL_CLAMP_TO_EDGE ||
           wrapR == GL_REPEAT        ||
           wrapR == GL_MIRRORED_REPEAT);
    assert(aniso >= 1.0f);

    // For each face
    for (int i = 0; i < 6; i++)
    {
        // Create empty face texture
        InitTextureObject(NULL, faceEnums[i]);
    }
}

/*
 * Destructor
 */
TextureCube::~TextureCube()
{
    // Nothing to do here, it is all handled in the base destructor
}

/*
 * Init texture object
 */
void TextureCube::InitTextureObject(const GLvoid* pixels, GLenum face)
{
    // Set texture object info, reading into the image buffer
    glTexImage2D(
        face, 
        0,
        internalFormat, 
        width, 
        height, 
        0, 
        imageFormat, 
        dataType, 
        pixels);

    // Generate mip maps after the last face if necessary.
    // This assumes that the negative Z face is done last
    if (face == GL_TEXTURE_CUBE_MAP_NEGATIVE_Z &&
        pixels != NULL &&
        (minFilter == GL_NEAREST_MIPMAP_NEAREST ||
         minFilter == GL_NEAREST_MIPMAP_LINEAR  ||
         minFilter == GL_LINEAR_MIPMAP_NEAREST  ||
         minFilter == GL_LINEAR_MIPMAP_LINEAR))
    {
        glGenerateMipmap(GL_TEXTURE_CUBE_MAP);
    }
}

/*
 * Set texture parameters
 */
void TextureCube::SetTextureParams()
{
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, minFilter);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, magFilter);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, wrapS);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, wrapT);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, wrapR);

    // Set aniso level if enabled
    if (aniso > 1.0f)
    {
        glTexParameterf(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAX_ANISOTROPY_EXT, aniso);
    }
}
