#ifndef FRAME_BUFFER_H
#define FRAME_BUFFER_H

#include <cassert>
#include <GL/glew.h>
#include "DepthTexture2D.h"
#include "Texture2D.h"

/**
 * \brief Class for managing an OpenGL frame buffer object (FBO)
 *
 * \author Jonathan Henze
 */
class FrameBuffer
{
public:

    /**
     * \brief Creates a new frame buffer object
     *
     * Creates a new frame buffer object.  Texture unit 0 will be used in the
     * initialization of the frame buffer object, so any texture currently
     * bound to unit 0 will need to be rebound.
     *
     * \param[in] createColorTexture - Whether a texture should be created to
     *                                 hold the image created when rendering to
     *                                 the FBO.  The color texture can then be
     *                                 used to render with, such as with post
     *                                 processing effects
     * \param[in] createDepthTexture - Whether a texture should be created to
     *                                 hold the depth information generated while
     *                                 rendering to the FBO.  The depth texture
     *                                 can then be used to render with, such as
     *                                 with shadow mapping
     * \param[in] width  - Width of the FBO textures in pixels
     * \param[in] height - Height of the FBO textures in pixels
     */
    FrameBuffer(
        bool createColorTexture,
        bool createDepthTexture,
        int  width,
        int  height);

    /**
     * \brief Creates a new frame buffer object
     *
     * Creates a new frame buffer object.  Texture unit 0 will be used in the
     * initialization of the frame buffer object, so any texture currently
     * bound to unit 0 will need to be rebound.
     *
     * \param[in] numColorAttachments - Number of color attachments to create
     *                                  for use with multiple render targets,
     *                                  such as when performing deferred rendering.
     *                                  A texture will be created for each color
     *                                  attachment
     * \param[in] createDepthTexture  - Whether a texture should be created to
     *                                  hold the depth information generated while
     *                                  rendering to the FBO.  The depth texture
     *                                  can then be used to render with, such as
     *                                  with shadow mapping
     * \param[in] width  - Width of the FBO textures in pixels
     * \param[in] height - Height of the FBO textures in pixels
     */
    FrameBuffer(
        int  numColorAttachments,
        bool createDepthTexture,
        int  width,
        int  height);

    /**
     * \brief FrameBuffer destructor
     */
    ~FrameBuffer();

    /**
     * \brief Binds the frame buffer, causing it to be the target for rendering
     */
    void Bind();

    /**
     * \brief Unbinds the frame buffer, setting the render target to the back buffer
     *
     * The viewport will be restored to what is was before the FBO was bound
     */
    static void Unbind();

    /**
     * \brief Checks whether the FBO is currently bound
     *
     * \return Whether the FBO is currently bound
     */
    inline bool IsBound() const
    {
        return activeFbo == fboId;
    }

    /**
     * \brief Gets the ID of the FBO
     *
     * \return The ID of the FBO
     */
    inline GLuint GetID() const
    {
        return fboId;
    }

    /**
     * \brief Gets a color texture from the FBO
     *
     * Gets a texture that had its color filled in by the image generated while
     * rendering to this FBO.  This can only be called if the FBO was created
     * with at least 1 color attachment.  Undefined behavior will result if
     * this texture is used to render while the FBO is active
     *
     * \param[in] attachment - Optionally, which color attachment to get the
     *                         color texture for when using multiple render
     *                         targets
     */
    inline Texture2D& GetColorTexture(int attachment = 0)
    {
        assert(attachment < numColorAttachments && attachment >= 0);
        assert(colorTextures);
        assert(colorTextures[attachment]);

        return *(colorTextures[attachment]);
    }

    /**
     * \brief Gets a color texture from the FBO
     *
     * Gets a texture that had its color filled in by the image generated while
     * rendering to this FBO.  This can only be called if the FBO was created
     * with at least 1 color attachment.  Undefined behavior will result if
     * this texture is used to render while the FBO is active
     *
     * \param[in] attachment - Optionally, which color attachment to get the
     *                         color texture for when using multiple render
     *                         targets
     */
    inline const Texture2D& GetColorTexture(int attachment = 0) const
    {
        assert(attachment < numColorAttachments && attachment >= 0);
        assert(colorTextures);
        assert(colorTextures[attachment]);

        return *(colorTextures[attachment]);
    }

    /**
     * \brief Gets the depth texture from the FBO
     *
     * Gets a texture containing depth values on the range [0,1] in the red
     * channel, generated while rendering to this FBO.  This can only be called
     * if the FBO was created with a depth texture.  Undefined behavior will
     * result if this texture is used to render while the FBO is active
     */
    inline Texture2D& GetDepthTexture()
    {
        assert(depthTexture);
        return *depthTexture;
    }

    /**
     * \brief Gets the depth texture from the FBO
     *
     * Gets a texture containing depth values on the range [0,1] in the red
     * channel, generated while rendering to this FBO.  This can only be called
     * if the FBO was created with a depth texture.  Undefined behavior will
     * result if this texture is used to render while the FBO is active
     */
    inline const Texture2D& GetDepthTexture() const
    {
        assert(depthTexture);
        return *depthTexture;
    }

private:
    GLuint fboId;                 //!< ID of the FBO
    int numColorAttachments;      //!< Number of color attachments used
    DepthTexture2D* depthTexture; //!< Depth texture or NULL
    Texture2D** colorTextures;    //!< Array of color textures or NULL

    int width;  //!< Width of FBO viewport and textures
    int height; //!< Height of FBO viewport and textures

    GLuint depthRenderBuffer; //!< Depth render buffer if there is not a depth texture

    static GLuint activeFbo; //!< ID of currently active FBO, 0 is the back buffer

    static const GLenum colorAttachmentEnums[]; //!< GL_COLOR_ATTACHMENT enums

    static GLint viewport[]; //!< Holds the viewport to be restored on unbind

    /**
     * \brief Helper to initialize the FBO
     */
    void InitFBO(bool createDepthTexture);

    FrameBuffer(const FrameBuffer&);            //!< No copy constructor
    FrameBuffer& operator=(const FrameBuffer&); //!< No assignment operator
};

#endif
