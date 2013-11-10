#ifndef TEXTURE_2D_H
#define TEXTURE_2D_H

#include "Texture.h"

/**
 * \brief Class to manage a 2-dimensional texture
 *
 * \author Jonathan Henze
 */
class Texture2D : public Texture
{
public:

    /** 
     * \brief Creates a texture by loading an image
     *
     * \param[in] filename - Name and path of the file to load
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
     * \param[in] wrapT     - Wrap mode to use when accessing texture coordinates
     *                        with t values outside of [0,1].  Valid values are:
     *                        GL_CLAMP_TO_EDGE
     *                        GL_REPEAT (default)
     *                        GL_MIRRORED_REPEAT
     * \param[in] aniso     - Maximum number of samples used for anisotropic
     *                        filtering.  Set to a value greater than 1 while
     *                        using a filter mode involving mipmaps to enable
     *                        anisotropic filtering.  Valid values range from
     *                        1 to GL_MAX_TEXTURE_MAX_ANISOTROPY_EXT (usually 16)
     */
    Texture2D(
        const char* filename,
        GLenum minFilter = GL_LINEAR,
        GLenum magFilter = GL_LINEAR,
        GLenum wrapS     = GL_REPEAT,
        GLenum wrapT     = GL_REPEAT,
        float  aniso     = 1.0f);

    /** 
     * \brief Creates a texture from an array of pixel data
     *
     * \param[in] data      - Raw unsigned bytes of color data.  The first element
     *                        is the lower left corner of the image, with the
     *                        elements continuing left to right of the bottom row,
     *                        before moving to the upper rows
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
     *                        GL_CLAMP_TO_EDGE
     *                        GL_REPEAT (default)
     *                        GL_MIRRORED_REPEAT
     * \param[in] wrapT     - Wrap mode to use when accessing texture coordinates
     *                        with t values outside of [0,1].  Valid values are:
     *                        GL_CLAMP_TO_EDGE
     *                        GL_REPEAT (default)
     *                        GL_MIRRORED_REPEAT
     * \param[in] aniso     - Maximum number of samples used for anisotropic
     *                        filtering.  Set to a value greater than 1 while
     *                        using a filter mode involving mipmaps to enable
     *                        anisotropic filtering.  Valid values range from
     *                        1 to GL_MAX_TEXTURE_MAX_ANISOTROPY_EXT (usually 16)
     */
    Texture2D(
        const GLubyte* data, 
        GLenum format, 
        int width, 
        int height,
        GLenum minFilter = GL_LINEAR,
        GLenum magFilter = GL_LINEAR,
        GLenum wrapS     = GL_REPEAT,
        GLenum wrapT     = GL_REPEAT,
        float  aniso     = 1.0f);
    Texture2D(
        const GLfloat* data,
        GLenum format, 
        int width, 
        int height,
        GLenum minFilter = GL_LINEAR,
        GLenum magFilter = GL_LINEAR,
        GLenum wrapS     = GL_REPEAT,
        GLenum wrapT     = GL_REPEAT,
        float  aniso     = 1.0f);

    /**
     * \brief Creates an empty texture object with storage allocated
     *
     * \param[in] internalFormat - Format used when rendering
     * \param[in] sourceFormat   - Format of the data the will be placed into
     *                             into the texture from outside means (such
     *                             as from a FBO)
     * \param[in] dataType       - Integral data type of the incoming data such
     *                             as GL_FLOAT or GL_UNSIGNED_BYTE
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
     *                        GL_CLAMP_TO_EDGE
     *                        GL_REPEAT (default)
     *                        GL_MIRRORED_REPEAT
     * \param[in] wrapT     - Wrap mode to use when accessing texture coordinates
     *                        with t values outside of [0,1].  Valid values are:
     *                        GL_CLAMP_TO_EDGE
     *                        GL_REPEAT (default)
     *                        GL_MIRRORED_REPEAT
     * \param[in] aniso     - Maximum number of samples used for anisotropic
     *                        filtering.  Set to a value greater than 1 while
     *                        using a filter mode involving mipmaps to enable
     *                        anisotropic filtering.  Valid values range from
     *                        1 to GL_MAX_TEXTURE_MAX_ANISOTROPY_EXT (usually 16)
     */
    Texture2D(
        GLenum internalFormat,
        GLenum sourceFormat,
        GLenum dataType,
        int width, 
        int height,
        GLenum minFilter = GL_LINEAR,
        GLenum magFilter = GL_LINEAR,
        GLenum wrapS     = GL_REPEAT,
        GLenum wrapT     = GL_REPEAT,
        float  aniso     = 1.0f);

    /**
     * \brief Texture2D destructor
     */
    ~Texture2D();

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
    GLenum wrapT;     //!< Wrap mode for T coordinates
    float  aniso;     //!< Maximum samples for anisotropic filtering

    int    width;          //!< Width of the texture in pixels
    int    height;         //!< Height of the texture in pixels
    GLint  internalFormat; //!< Image format used when rendering
    GLenum imageFormat;    //!< Source image format
    GLenum dataType;       //!< Type of data in the image buffer

    Texture2D(const Texture2D&);            //!< No copy constructor
    Texture2D& operator=(const Texture2D&); //!< No assignment operator
};

#endif
