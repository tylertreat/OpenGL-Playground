#ifndef DEPTH_TEXTURE_2D_H
#define DEPTH_TEXTURE_2D_H

#include "Texture2D.h"

/**
 * \brief Class representing a 2-dimensional depth texture
 *
 * \author Jonathan Henze
 */
class DepthTexture2D : public Texture2D
{
public:

    /** 
     * \brief Creates a texture from an array of pixel data
     *
     * \param[in] data      - Raw unsigned bytes of color data.  The first element
     *                        is the lower left corner of the image, with the
     *                        elements continuing left to right of the bottom row,
     *                        before moving to the upper rows.  Can be null, in which
     *                        case the texture will have storage allocated but not
     *                        initialized
     * \param[in] format    - Format of the color data.  Valid values are:
     *                        GL_RED
     *                        GL_RG
     *                        GL_RGB
     *                        GL_BGR
     *                        GL_RGBA
     *                        GL_BGRA
     * \param[in] width     - Width of the image in pixels
     * \param[in] height    - Height of the image in pixels
     * \param[in] minFilter - Minification filter to use when the texture
     *                        is drawn on small surfaces.  Valid values are:
     *                        GL_NEAREST
     *                        GL_LINEAR (default, aka bilinear filtering)
     *                        GL_NEAREST_MIPMAP_NEAREST
     *                        GL_LINEAR_MIPMAP_NEAREST
     *                        GL_NEAREST_MIPMAP_LINEAR
     *                        GL_LINEAR_MIPMAP_LINEAR (aka trilinear filtering)
     * \param[in] magFilter - Magnification filter to use when the texture
     *                        is drawn on large surfaces.  Valid values are:
     *                        GL_NEAREST
     *                        GL_LINEAR (default)
     * \param[in] wrapS     - Wrap mode to use when accessing texture coordinates
     *                        with s values outside of [0,1].  Valid values are:
     *                        GL_CLAMP_TO_EDGE (default)
     *                        GL_REPEAT
     *                        GL_MIRRORED_REPEAT
     * \param[in] wrapT     - Wrap mode to use when accessing texture coordinates
     *                        with t values outside of [0,1].  Valid values are:
     *                        GL_CLAMP_TO_EDGE (default)
     *                        GL_REPEAT
     *                        GL_MIRRORED_REPEAT
     * \param[in] aniso     - Maximum number of samples used for anisotropic
     *                        filtering.  Set to a value greater than 1 while
     *                        using a filter mode involving mipmaps to enable
     *                        anisotropic filtering.  Valid values range from
     *                        1 to GL_MAX_TEXTURE_MAX_ANISOTROPY_EXT (usually 16)
     * \param[in] compareFunc - Comparison function to use when comparing sampled
     *                          depth values against the fragment's current depth.
     *                          Valid values are:
     *                          GL_LEQUAL (default)
     *                          GL_GEQUAL
     *                          GL_LESS
     *                          GL_GREATER
     *                          GL_EQUAL
     *                          GL_NOTEQUAL
     *                          GL_ALWAYS
     *                          GL_NEVER
     * \param[in] format      - Format to use internally.  Valid values are:
     *                          GL_DEPTH_COMPONENT
     *                          GL_DEPTH_COMPONENT16
     *                          GL_DEPTH_COMPONENT24
     *                          GL_DEPTH_COMPONENT32
     */
    DepthTexture2D(
        int width,
        int height,
        GLenum minFilter   = GL_LINEAR,
        GLenum magFilter   = GL_LINEAR,
        GLenum wrapS       = GL_CLAMP_TO_EDGE,
        GLenum wrapT       = GL_CLAMP_TO_EDGE,
        float  aniso       = 1.0f,
        GLenum compareFunc = GL_LEQUAL,
        GLenum format      = GL_DEPTH_COMPONENT24);

    /**
     * \brief DepthTexture2D destructor
     */
    virtual ~DepthTexture2D();

protected:

    /**
     * \brief Sets the texture parameters such as min/mag filter and wrap modes
     *
     * Sets the texture parameters
     */
    virtual void SetTextureParams();

private:
    GLenum compareFunc; //!< Comparison function used

    DepthTexture2D(const DepthTexture2D&);            //!< No copy constructor
    DepthTexture2D& operator=(const DepthTexture2D&); //!< No assignment operator
};

#endif
