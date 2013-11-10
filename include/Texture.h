#ifndef TEXTURE_H
#define TEXTURE_H

#include <GL/glew.h>

/**
 * \brief Abstract base class for texture types
 *
 * \author Jonathan Henze
 */
class Texture
{
public:

    /**
     * \brief Texture destructor
     */
    virtual ~Texture();

    /**
     * \brief Gets whether the texture is currently bound to a texture unit
     *
     * \return Whether the texture is currently bound to a texture unit
     */
    bool IsBound() const;

    /**
     * \brief Binds the texture to a texture unit so it can be used when drawing
     *
     * Binds the texture to a specified texture unit so it may be used when
     * rendering.  Only one texture may be bound to a given texture unit at a
     * time.  Choosing a texture unit that is already in use by another texture
     * causes that texture to be unbound from the texture unit, allowing this
     * texture to bind to the unit.  The exact number of texture units in the
     * system may vary, but it is guaranteed to be at least 16.
     *
     * \param[in] textureUnit - Texture unit number to bind to.  Values range
     *                          from 0 (inclusive) to max_texture_units (exclusive),
     *                          where the maximum is system dependant, but is
     *                          guaranteed to be at least 16
     */
    void Bind(int textureUnit);

    /**
     * \brief Gets the ID of the texture
     *
     * \return The ID of the texture
     */
    inline GLuint GetID() const
    {
        return textureId;
    }

    /**
     * \brief Gets the texture unit the texture is currently bound to, if any
     *
     * \return The texture unit the texture is bound to, or -1 if it is not bound
     */
    inline int GetTextureUnit() const
    {
        return IsBound() ? textureUnit : -1;
    }

protected:

    /**
     * \brief Creates the base texture object for a derived class
     *
     * Creates the texture object and image buffer for the texture.
     * The derived class must fill the image buffer with texture data,
     * but should not touch the texture object yet (binding it will modify
     * which texture is bound to the current texture unit, causing textures
     * to become mysteriously unbound).  The image buffer will be actively
     * bound to GL_PIXEL_UNPACK_BUFFER when the base constructor returns and
     * the derived constructor is run.  The derived constructor should unbind
     * the image buffer when it finishes.
     *
     * \param[in] target - Target to be used with glBindTexture commands
     */
    Texture(GLenum target);

    /**
     * \brief Initializes the texture with the appropriate glTexImage command
     *
     * Initializes the currently bound texture object using the appropriate
     * glTexImage command.  The image buffer will be actively bound to the
     * GL_PIXEL_UNPACK_BUFFER, meaning that the data pointer in the glTexImage
     * command is an offset into the pixel buffer
     */
    //virtual void InitTextureObject()=0;

    /**
     * \brief Sets the texture parameters such as min/mag filter and wrap modes
     *
     * Sets the texture parameters
     */
    virtual void SetTextureParams()=0;

    /**
     * \brief Wrapper to handle loading images with the stb_image library
     *
     * \param[in]  filename    - Name and path of the image file to load
     * \param[out] nWidth      - Returns the width of the loaded image
     * \param[out] nHeight     - Returns the height of the loaded image
     * \param[out] eFormat     - Returns the format of the loaded image
     *
     * \return Array of color bytes in the image allocated with malloc,
     *         must be free'd by the caller
     *
     * \author Steve Kautz
     */
    static GLubyte* LoadFile(
        const char* filename, 
        int* nWidth, 
        int* nHeight,
        GLenum* eFormat);

    /**
     * \brief Generates a texture to be used when one cannot be loaded
     *
     * \param[out] width      - Returns the width of the texture
     * \param[out] height     - Returns the height of the texture
     * \param[out] format     - Returns the format of the texture
     *
     * \return The malloc'd bytes for the texture
     */
    static GLubyte* ErrorTexture(int* width, int* height, GLenum* format);

    /**
     * \brief Gets the number of bytes used per pixel in a given format
     *
     * \param[in] format - Format to check
     *
     * \return The number of bytes in a pixel using the given format
     */
    static GLsizei BytesPerPixel(GLenum format);

private:

    GLenum target;        //!< The target of glBindTexture commands
    GLuint textureId;     //!< The ID of the texture
    //GLuint imageBufferId; //!< ID of the buffer used to hold the image data
    int    textureUnit;   //!< The texture unit the texture was last bound to
    //bool   imageStale;    //!< Flag whether the texture image data needs updating

    static GLuint activeTextures[]; //!< The ID of the texture bound to each unit

    static const GLenum textureUnits[]; //!< Lookup table of texture enums

    Texture(const Texture&);            //!< No copy constructor
    Texture& operator=(const Texture&); //!< No assignment operator
};

#endif
