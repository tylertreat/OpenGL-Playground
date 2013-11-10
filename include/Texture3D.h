#ifndef TEXTURE_3D_H
#define TEXTURE_3D_H

#include "Texture.h"

class Texture3D : public Texture
{
public:

    /** 
     * \brief Creates a 3D texture by loading many images
     *
     *
     * \param[in] filenames - Array of filenames to load for each level
     * \param[in] depth     - Number of layers in the 3D texture and length of
     *                        the filenames parameter
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
     * \param[in] wrapR     - Wrap mode to use when accessing texture coordinates
     *                        with r values outside of [0,1].  Valid values are:
     *                        GL_CLAMP_TO_EDGE
     *                        GL_REPEAT (default)
     *                        GL_MIRRORED_REPEAT
     * \param[in] aniso     - Maximum number of samples used for anisotropic
     *                        filtering.  Set to a value greater than 1 while
     *                        using a filter mode involving mipmaps to enable
     *                        anisotropic filtering.  Valid values range from
     *                        1 to GL_MAX_TEXTURE_MAX_ANISOTROPY_EXT (usually 16)
     */
    Texture3D(
        const char* const * filenames,
        int    depth,
        GLenum minFilter = GL_LINEAR,
        GLenum magFilter = GL_LINEAR,
        GLenum wrapS     = GL_REPEAT,
        GLenum wrapT     = GL_REPEAT,
        GLenum wrapR     = GL_REPEAT,
        float  aniso     = 1.0f);

    /** 
     * \brief Creates a cube texture from several arrays of pixel data
     *
     * All of the images must have the same size and format.
     *
     * \param[in] data      - Array of raw color data.  The first element is
     *                        the lower left corner at depth 0, proceeding to
     *                        the right until the lower right corner at depth 0,
     *                        then continuing upward through the subsequent rows
     *                        until the top right corner at depth 0.  Then it
     *                        continues to the lower left corner at depth 1
     *                        and so on.
     * \param[in] format    - Format of the color data.  Valid values are:
     *                        GL_RED
     *                        GL_RG
     *                        GL_RGB
     *                        GL_BGR
     *                        GL_RGBA
     *                        GL_BGRA
     * \param[in] width     - Width of each image in pixels
     * \param[in] height    - Height of each image in pixels
     * \param[in] depth     - Number of layers in the 3D texture
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
     * \param[in] wrapR     - Wrap mode to use when accessing texture coordinates
     *                        with r values outside of [0,1].  Valid values are:
     *                        GL_CLAMP_TO_EDGE (default)
     *                        GL_REPEAT
     *                        GL_MIRRORED_REPEAT
     * \param[in] aniso     - Maximum number of samples used for anisotropic
     *                        filtering.  Set to a value greater than 1 while
     *                        using a filter mode involving mipmaps to enable
     *                        anisotropic filtering.  Valid values range from
     *                        1 to GL_MAX_TEXTURE_MAX_ANISOTROPY_EXT (usually 16)
     */
    Texture3D(
        const GLubyte* data,
        GLenum format, 
        int width, 
        int height,
        int depth,
        GLenum minFilter = GL_LINEAR,
        GLenum magFilter = GL_LINEAR,
        GLenum wrapS     = GL_REPEAT,
        GLenum wrapT     = GL_REPEAT,
        GLenum wrapR     = GL_REPEAT,
        float  aniso     = 1.0f);
    Texture3D(
        const GLfloat* data,
        GLenum format, 
        int width, 
        int height,
        int depth,
        GLenum minFilter = GL_LINEAR,
        GLenum magFilter = GL_LINEAR,
        GLenum wrapS     = GL_REPEAT,
        GLenum wrapT     = GL_REPEAT,
        GLenum wrapR     = GL_REPEAT,
        float  aniso     = 1.0f);

    /** 
     * \brief Creates a cube texture from several arrays of pixel data
     *
     * All of the images must have the same size and format.
     *
     * \param[in] data      - Array of raw color data pointers.  Must have the
     *                        same length as the "depth" parameter.  The first
     *                        color data pointer points to the pixel data for
     *                        depth 0, with each subsequent color data pointer
     *                        being used for larger depths.  In each color data
     *                        array, the first element is the lower left corner
     *                        of the image, with the elements continuing left
     *                        to right of the bottom row, before moving to the
     *                        upper rows
     * \param[in] format    - Format of the color data.  Valid values are:
     *                        GL_RED
     *                        GL_RG
     *                        GL_RGB
     *                        GL_BGR
     *                        GL_RGBA
     *                        GL_BGRA
     * \param[in] width     - Width of each image in pixels
     * \param[in] height    - Height of each image in pixels
     * \param[in] depth     - Number of layers in the 3D texture
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
     * \param[in] wrapR     - Wrap mode to use when accessing texture coordinates
     *                        with r values outside of [0,1].  Valid values are:
     *                        GL_CLAMP_TO_EDGE (default)
     *                        GL_REPEAT
     *                        GL_MIRRORED_REPEAT
     * \param[in] aniso     - Maximum number of samples used for anisotropic
     *                        filtering.  Set to a value greater than 1 while
     *                        using a filter mode involving mipmaps to enable
     *                        anisotropic filtering.  Valid values range from
     *                        1 to GL_MAX_TEXTURE_MAX_ANISOTROPY_EXT (usually 16)
     */
    Texture3D(
        const GLubyte* const * data,
        GLenum format, 
        int width, 
        int height,
        int depth,
        GLenum minFilter = GL_LINEAR,
        GLenum magFilter = GL_LINEAR,
        GLenum wrapS     = GL_REPEAT,
        GLenum wrapT     = GL_REPEAT,
        GLenum wrapR     = GL_REPEAT,
        float  aniso     = 1.0f);
    Texture3D(
        const GLfloat* const * data,
        GLenum format, 
        int width, 
        int height,
        int depth,
        GLenum minFilter = GL_LINEAR,
        GLenum magFilter = GL_LINEAR,
        GLenum wrapS     = GL_REPEAT,
        GLenum wrapT     = GL_REPEAT,
        GLenum wrapR     = GL_REPEAT,
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
     * \param[in] depth     - Number of layers in the 3D texture
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
     * \param[in] wrapR     - Wrap mode to use when accessing texture coordinates
     *                        with r values outside of [0,1].  Valid values are:
     *                        GL_CLAMP_TO_EDGE
     *                        GL_REPEAT (default)
     *                        GL_MIRRORED_REPEAT
     * \param[in] aniso     - Maximum number of samples used for anisotropic
     *                        filtering.  Set to a value greater than 1 while
     *                        using a filter mode involving mipmaps to enable
     *                        anisotropic filtering.  Valid values range from
     *                        1 to GL_MAX_TEXTURE_MAX_ANISOTROPY_EXT (usually 16)
     */
    Texture3D(
        GLenum internalFormat,
        GLenum sourceFormat,
        GLenum dataType,
        int width, 
        int height,
        int depth,
        GLenum minFilter = GL_LINEAR,
        GLenum magFilter = GL_LINEAR,
        GLenum wrapS     = GL_REPEAT,
        GLenum wrapT     = GL_REPEAT,
        GLenum wrapR     = GL_REPEAT,
        float  aniso     = 1.0f);

    /**
     * \brief Texture3D destructor
     */
    ~Texture3D();

protected:

    /**
     * \brief Initializes the texture with the appropriate glTexImage command
     *
     * Initializes the currently bound texture object using the appropriate
     * glTexImage command.
     *
     * \param[in] pixels - Array of pixel pointers, must have the same length as
     *                     the member variable "depth", or can be NULL
     */
    void InitTextureObject(const GLvoid* const * pixels);

    /**
     * \brief Initializes the texture with the appropriate glTexImage command
     *
     * Initializes the currently bound texture object using the appropriate
     * glTexImage command.
     *
     * \param[in] pixels - Array of pixel data or NULL
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
    GLenum wrapR;     //!< Wrap mode for R coordinates
    float  aniso;     //!< Maximum samples for anisotropic filtering

    int    width;          //!< Width each texture in pixels
    int    height;         //!< Height each texture in pixels
    int    depth;          //!< How many texture layers deep the image is
    GLint  internalFormat; //!< Image format used when rendering
    GLenum imageFormat;    //!< Source image format
    GLenum dataType;       //!< Type of data in the image buffer

    Texture3D(const Texture3D&);            //!< No copy constructor
    Texture3D& operator=(const Texture3D&); //!< No assignment operator
};

#endif
