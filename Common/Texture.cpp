#include <cassert>
#include <iostream>
#include "stb_image.h"
#include "Texture.h"

/*
 * Active texture ID for each unit
 */
GLuint Texture::activeTextures[32] =
{
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0
};

/*
 * Look-up table of texture unit enum values
 */
const GLenum Texture::textureUnits[32] =
{
    GL_TEXTURE0,
    GL_TEXTURE1,
    GL_TEXTURE2,
    GL_TEXTURE3,
    GL_TEXTURE4,
    GL_TEXTURE5,
    GL_TEXTURE6,
    GL_TEXTURE7,
    GL_TEXTURE8,
    GL_TEXTURE9,
    GL_TEXTURE10,
    GL_TEXTURE11,
    GL_TEXTURE12,
    GL_TEXTURE13,
    GL_TEXTURE14,
    GL_TEXTURE15,
    GL_TEXTURE16,
    GL_TEXTURE17,
    GL_TEXTURE18,
    GL_TEXTURE19,
    GL_TEXTURE20,
    GL_TEXTURE21,
    GL_TEXTURE22,
    GL_TEXTURE23,
    GL_TEXTURE24,
    GL_TEXTURE25,
    GL_TEXTURE26,
    GL_TEXTURE27,
    GL_TEXTURE28,
    GL_TEXTURE29,
    GL_TEXTURE30,
    GL_TEXTURE31
};

/*
 * Texture constructor
 */
Texture::Texture(GLenum target)
    : target(target),
    textureUnit(-1)
{
    // We need to use a texture unit in the process of initializing the texture.
    // If there was a texture already bound to 0, it will be unbound
    activeTextures[0] = 0;
    glActiveTexture(textureUnits[0]);

    // Create and bind the texture object
    glGenTextures(1, &textureId);
    glBindTexture(target, textureId);

    // It is up to the derived class to fill the texture with data now
}

/*
 * Destructor
 */
Texture::~Texture()
{
    // Delete the texture object
    glDeleteTextures(1, &textureId);
}

/*
 * Is bound
 */
bool Texture::IsBound() const
{
    return textureUnit != -1 &&
        activeTextures[textureUnit] == textureId;
}

/*
 * Bind
 */
void Texture::Bind(int textureUnit)
{
    // Set the texture unit active
    glActiveTexture(textureUnits[textureUnit]);

    // Attach our texture object
    glBindTexture(target, textureId);

    // Set the texture parameters on the current texture unit
    SetTextureParams();

    // Keep track of where we are bound to help with debugging
    this->textureUnit = textureUnit;
    Texture::activeTextures[textureUnit] = textureId;
}

GLsizei Texture::BytesPerPixel(GLenum format)
{
    switch (format)
    {
    case GL_RED:
        return 1;
    case GL_RG:
        return 2;
    case GL_RGB:
        return 3;
    case GL_BGR:
        return 3;
    case GL_RGBA:
        return 4;
    case GL_BGRA:
        return 4;

    default:
        // Bad or unsupported format
        assert(false);
        return 4;
    }
}

/*
 * Load image file
 */
GLubyte* Texture::LoadFile(
    const char* filename, 
    int* nWidth, 
    int* nHeight,
    GLenum* eFormat)
{
    int comp;
    GLubyte * temp = stbi_load(filename, nWidth, nHeight, &comp, 0);
    if (temp == NULL)
    {
        std::cerr << "Unable to load image file " << filename << std::endl;
        return ErrorTexture(nWidth, nHeight, eFormat);
    }
    if (comp == 3)
    {
        *eFormat = GL_RGB;
    }
    else if (comp == 4)
    {
        *eFormat = GL_RGBA;
    }
    else
    {
        std::cerr << "Image is not RGB or RGBA: " << filename << std::endl;
        return ErrorTexture(nWidth, nHeight, eFormat);
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

/*
 * Generate error fallback texture
 */
GLubyte* Texture::ErrorTexture(int* width, int* height, GLenum* format)
{
    // Make a 2x2 magenta texture, immediately recognizable as a bad texture
    *width       = 2;
    *height      = 2;
    *format      = GL_RGBA;
    GLubyte* tex = (GLubyte*)malloc(2 * 2 * 4 * sizeof(GLubyte));
    for (int i = 0; i < 4; i++)
    {
        // Magenta is (1, 0, 1, 1)
        tex[4*i + 0] = 255;
        tex[4*i + 1] = 0;
        tex[4*i + 2] = 255;
        tex[4*i + 3] = 255;
    }

    return tex;
}