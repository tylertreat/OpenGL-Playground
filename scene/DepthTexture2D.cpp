#include <cassert>
#include <cstring>
#include "DepthTexture2D.h"

/*
 * Depth Texture 2D constructor
 */
DepthTexture2D::DepthTexture2D(
    int width,
    int height,
    GLenum minFilter,
    GLenum magFilter,
    GLenum wrapS,
    GLenum wrapT,
    float  aniso,
    GLenum compareFunc,
    GLenum format)
    : Texture2D(
    format,
    GL_DEPTH_COMPONENT,
    GL_FLOAT,
    width,
    height,
    minFilter,
    magFilter,
    wrapS,
    wrapT,
    aniso),
    compareFunc(compareFunc)
{
    // Debug assertions
    assert(format == GL_DEPTH_COMPONENT   ||
           format == GL_DEPTH_COMPONENT16 ||
           format == GL_DEPTH_COMPONENT24 ||
           format == GL_DEPTH_COMPONENT32);
    assert(compareFunc == GL_LEQUAL   ||
           compareFunc == GL_GEQUAL   ||
           compareFunc == GL_LESS     ||
           compareFunc == GL_GREATER  ||
           compareFunc == GL_EQUAL    ||
           compareFunc == GL_NOTEQUAL ||
           compareFunc == GL_ALWAYS   ||
           compareFunc == GL_NEVER);

    // Texture2D constructor handles everything else
}

/*
 * Destructor
 */
DepthTexture2D::~DepthTexture2D()
{
    // Everything is handled by the Texture destructor
}

/*
 * Set texture parameters
 */
void DepthTexture2D::SetTextureParams()
{
    // Set depth parameters
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_COMPARE_MODE, GL_COMPARE_REF_TO_TEXTURE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_COMPARE_FUNC, compareFunc);

    // Call base method to set the rest of the parameters
    Texture2D::SetTextureParams();
}
