#ifndef TEXTURE_1D_H
#define TEXTURE_1D_H

#include "Texture.h"

class Texture1D : public Texture
{
public:
    /** 
     * \brief Creates a 1D texture by loading an image
     *
     * 1D textures should be images that have a height of 1 pixel.  If the image
     * is taller, the bottom row of the image is used to create the 1D texture.
     *
     * \param[in] filename  - Name and path of the image file to load
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
     *                        GL_CLAMP_TO_EDGE
     *                        GL_REPEAT (default)
     *                        GL_MIRRORED_REPEAT
     */
    Texture1D(
        const char* filename,
        GLenum minFilter = GL_LINEAR,
        GLenum magFilter = GL_LINEAR,
        GLenum wrapS     = GL_REPEAT);

    /** 
     * \brief Creates a 1D texture from an array of pixel data
     *
     * \param[in] data      - Raw unsigned bytes of color data.  The first
     *                        element is the leftmost pixel, and continues
     *                        rightwards from there
     * \param[in] format    - Format of the color data.  Valid values are:
     *                        GL_RED
     *                        GL_RG
     *                        GL_RGB
     *                        GL_BGR
     *                        GL_RGBA
     *                        GL_BGRA
     * \param[in] width     - Width of the image in pixels
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
     *                        GL_CLAMP_TO_EDGE
     *                        GL_REPEAT (default)
     *                        GL_MIRRORED_REPEAT
     */
    Texture1D(
        const GLubyte* data, 
        GLenum format, 
        int width,
        GLenum minFilter = GL_LINEAR,
        GLenum magFilter = GL_LINEAR,
        GLenum wrapS     = GL_REPEAT);
    Texture1D(
        const GLfloat* data,
        GLenum format, 
        int width,
        GLenum minFilter = GL_LINEAR,
        GLenum magFilter = GL_LINEAR,
        GLenum wrapS     = GL_REPEAT);

    /**
     * \brief Texture1D destructor
     */
    virtual ~Texture1D();

protected:

    /**
     * \brief Initializes the texture with the appropriate glTexImage command
     *
     * Initializes the currently bound texture object using the appropriate
     * glTexImage command.
     *
     * \param[in] pixels - Pixel data to initialize the texture with
     */
    void InitTextureObject(const GLvoid* pixels);

    /**
     * \brief Sets the texture parameters such as min/mag filter and wrap modes
     *
     * Sets the texture parameters
     */
    virtual void SetTextureParams();

private:
    GLenum minFilter; //!< Filter to use when shrinking the texture
    GLenum magFilter; //!< Filter to use when expanding the texture
    GLenum wrapS;     //!< Wrap mode for S coordinates

    int    width;          //!< Width of the texture in pixels
    GLint  internalFormat; //!< Image format used when rendering
    GLenum imageFormat;    //!< Source image format
    GLenum dataType;       //!< Type of data in the image buffer

    Texture1D(const Texture1D&);            //!< No copy constructor
    Texture1D& operator=(const Texture1D&); //!< No assignment operator
};

#endif
