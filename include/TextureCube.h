#ifndef TEXTURE_CUBE
#define TEXTURE_CUBE

#include "Texture.h"

/**
 * \brief Class to manage a cube texture with 6 faces
 *
 * \author Jonathan Henze
 */
class TextureCube : public Texture
{
public:

    /** 
     * \brief Creates a cube texture by loading 6 images
     *
     *
     * \param[in] posXFilename - Name and path of the file to load for the +X face
     * \param[in] negXFilename - Name and path of the file to load for the -X face
     * \param[in] posYFilename - Name and path of the file to load for the +Y face
     * \param[in] negXFilename - Name and path of the file to load for the -Y face
     * \param[in] posZFilename - Name and path of the file to load for the +Z face
     * \param[in] negZFilename - Name and path of the file to load for the -Z face
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
    TextureCube(
        const char* posXFilename,
        const char* negXFilename,
        const char* posYFilename,
        const char* negYFilename,
        const char* posZFilename,
        const char* negZFilename,
        GLenum minFilter = GL_LINEAR,
        GLenum magFilter = GL_LINEAR,
        GLenum wrapS     = GL_CLAMP_TO_EDGE,
        GLenum wrapT     = GL_CLAMP_TO_EDGE,
        GLenum wrapR     = GL_CLAMP_TO_EDGE,
        float  aniso     = 1.0f);

    /** 
     * \brief Creates a cube texture from 6 arrays of pixel data
     *
     * All of the images must have the same size and format.
     *
     * \param[in] data      - Raw color data.  The first element
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
     * \param[in] width     - Width of each image in pixels
     * \param[in] height    - Height of each image in pixels
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
    TextureCube(
        const GLubyte* posXData, 
        const GLubyte* negXData,
        const GLubyte* posYData,
        const GLubyte* negYData,
        const GLubyte* posZData,
        const GLubyte* negZData,
        GLenum format, 
        int width, 
        int height,
        GLenum minFilter = GL_LINEAR,
        GLenum magFilter = GL_LINEAR,
        GLenum wrapS     = GL_CLAMP_TO_EDGE,
        GLenum wrapT     = GL_CLAMP_TO_EDGE,
        GLenum wrapR     = GL_CLAMP_TO_EDGE,
        float  aniso     = 1.0f);
    TextureCube(
        const GLfloat* posXData,
        const GLfloat* negXData,
        const GLfloat* posYData,
        const GLfloat* negYData,
        const GLfloat* posZData,
        const GLfloat* negZData,
        GLenum format, 
        int width, 
        int height,
        GLenum minFilter = GL_LINEAR,
        GLenum magFilter = GL_LINEAR,
        GLenum wrapS     = GL_CLAMP_TO_EDGE,
        GLenum wrapT     = GL_CLAMP_TO_EDGE,
        GLenum wrapR     = GL_CLAMP_TO_EDGE,
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
    TextureCube(
        GLenum internalFormat,
        GLenum sourceFormat,
        GLenum dataType,
        int width, 
        int height,
        GLenum minFilter = GL_LINEAR,
        GLenum magFilter = GL_LINEAR,
        GLenum wrapS     = GL_CLAMP_TO_EDGE,
        GLenum wrapT     = GL_CLAMP_TO_EDGE,
        GLenum wrapR     = GL_CLAMP_TO_EDGE,
        float  aniso     = 1.0f);

    /**
     * \brief TextureCube destructor
     */
    ~TextureCube();

protected:

    /**
     * \brief Initializes a face texture with the appropriate glTexImage command
     *
     * Initializes the currently bound texture object using the appropriate
     * glTexImage command.
     *
     * \param[in] pixels - Pixel data to initialize the face texture with
     * \param[in] face   - Which face to initialize
     */
    void InitTextureObject(const GLvoid* pixels, GLenum face);

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

    int    width;          //!< Width each face texture in pixels
    int    height;         //!< Height each face texture in pixels
    GLint  internalFormat; //!< Image format used when rendering
    GLenum imageFormat;    //!< Source image format
    GLenum dataType;       //!< Type of data in the image buffer

    static const GLenum faceEnums[]; //!< Array of cube face enums

    TextureCube(const TextureCube&);            //!< No copy constructor
    TextureCube& operator=(const TextureCube&); //!< No assignment operator
};

#endif
