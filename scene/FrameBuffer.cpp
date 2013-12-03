#include <cassert>
#include <cstring>
#include <iostream>
#include "FrameBuffer.h"

/*
 * Active FBO ID
 */
GLuint FrameBuffer::activeFbo = 0;

/*
 * Color attachment enum values
 */
const GLenum FrameBuffer::colorAttachmentEnums[16] =
{
    GL_COLOR_ATTACHMENT0,
    GL_COLOR_ATTACHMENT1,
    GL_COLOR_ATTACHMENT2,
    GL_COLOR_ATTACHMENT3,
    GL_COLOR_ATTACHMENT4,
    GL_COLOR_ATTACHMENT5,
    GL_COLOR_ATTACHMENT6,
    GL_COLOR_ATTACHMENT7,
    GL_COLOR_ATTACHMENT8,
    GL_COLOR_ATTACHMENT9,
    GL_COLOR_ATTACHMENT10,
    GL_COLOR_ATTACHMENT11,
    GL_COLOR_ATTACHMENT12,
    GL_COLOR_ATTACHMENT13,
    GL_COLOR_ATTACHMENT14,
    GL_COLOR_ATTACHMENT15
};

/*
 * Viewport storage
 */
GLint FrameBuffer::viewport[4] =
{
    0, // x
    0, // y
    0, // width
    0  // height
};

/*
 * Frame buffer constructor
 */
FrameBuffer::FrameBuffer(
    bool createColorTexture,
    bool createDepthTexture,
    int  width,
    int  height)
    : fboId(0),
    numColorAttachments(createColorTexture ? 1 : 0),
    width(width),
    height(height)
{
    // Debug assertions
    assert(width > 0);
    assert(height > 0);

    InitFBO(createDepthTexture);
}

/*
 * Frame buffer MRT constructor
 */
FrameBuffer::FrameBuffer(
    int  numColorAttachments,
    bool createDepthTexture,
    int  width,
    int  height)
    : fboId(0),
    numColorAttachments(numColorAttachments),
    width(width),
    height(height)
{
    // Debug assertions
    assert(width > 0);
    assert(height > 0);
    assert(numColorAttachments >= 0 &&
           numColorAttachments <= 16);

    InitFBO(createDepthTexture);
}

/*
 * Framebuffer destructor
 */
FrameBuffer::~FrameBuffer()
{
    // Make sure we are unbound
    if (this->IsBound())
    {
        this->Unbind();
    }

    // Delete the depth texture or render buffer
    if (depthTexture != NULL)
    {
        delete depthTexture;
        depthTexture = NULL;
    }
    else
    {
        glDeleteRenderbuffers(1, &depthRenderBuffer);
    }

    // Delete any color textures we have
    if (colorTextures != NULL)
    {
        for (int i = 0; i < numColorAttachments; i++)
        {
            delete colorTextures[i];
            colorTextures[i] = NULL;
        }
        delete[] colorTextures;
        colorTextures = NULL;
    }

    // Delete the FBO
    glDeleteFramebuffers(1, &fboId);
}

/*
 * Bind
 */
void FrameBuffer::Bind()
{
    assert(!this->IsBound());

    // Check whether there is another FBO currently bound
    if (activeFbo == 0)
    {
        // No other FBO bound right now.  Save the viewport
        glGetIntegerv(GL_VIEWPORT, FrameBuffer::viewport);
    }

    // Bind the frame buffer
    glBindFramebuffer(GL_DRAW_FRAMEBUFFER, fboId);

    // If we aren't rendering to a color texture, we need to disable
    // the draw and read buffers
    if (numColorAttachments == 0)
    {
        glDrawBuffer(GL_NONE);
        //glReadBuffer(GL_NONE);
    }
    else
    {
        // Make sure the draw and read buffers are set to the back buffer
        // in case we had previously bound an FBO that did not have a color
        // texture, and had the buffers disabled
        if (numColorAttachments == 1)
        {
            glDrawBuffer(GL_BACK);
        }
        else
        {
            // Set multiple draw buffers, assuming the first n outputs
            glDrawBuffers(numColorAttachments, FrameBuffer::colorAttachmentEnums);
        }
        //glReadBuffer(GL_BACK);
    }

    // Set the viewport to match the size of the FBO
    glViewport(0, 0, width, height);

    // Keep track of the active FBO
    FrameBuffer::activeFbo = fboId;
}

/*
 * Unbind
 */
void FrameBuffer::Unbind()
{
    assert(FrameBuffer::activeFbo != 0);

    // Set the back buffer as active by using 0 for the FBO target
    glBindFramebuffer(GL_DRAW_FRAMEBUFFER, 0);

    // Make sure the draw and read buffers are set to their normal values
    glDrawBuffer(GL_BACK);
    glReadBuffer(GL_BACK);

    // Restore the viewport to the saved values
    glViewport(FrameBuffer::viewport[0], 
               FrameBuffer::viewport[1], 
               FrameBuffer::viewport[2], 
               FrameBuffer::viewport[3]);

    FrameBuffer::activeFbo = 0;
}

/*
 * Init FBO
 */
void FrameBuffer::InitFBO(bool createDepthTexture)
{
    // Generate and bind the frame buffer
    glGenFramebuffers(1, &fboId);
    glBindFramebuffer(GL_DRAW_FRAMEBUFFER, fboId);
    assert(!glGetError());
    // Either set up a depth render buffer or depth texture
    if (createDepthTexture)
    {
        // Not using a render buffer for depth
        depthRenderBuffer = 0;

        // Create the texture
        depthTexture = new DepthTexture2D(width, height);

        // Attach the depth texture to the active FBO
        glFramebufferTexture2D(
            GL_DRAW_FRAMEBUFFER, 
            GL_DEPTH_ATTACHMENT, 
            GL_TEXTURE_2D,
            depthTexture->GetID(),
            0);
    }
    else
    {
        // Not using a depth texture
        depthTexture = NULL;

        // Generate and bind a depth render buffer
        glGenRenderbuffers(1, &depthRenderBuffer);
        glBindRenderbuffer(GL_RENDERBUFFER, depthRenderBuffer);

        // Create storage for the render buffer
        glRenderbufferStorage(
            GL_RENDERBUFFER, 
            GL_DEPTH_COMPONENT24,
            width, 
            height);

        // Attach the render buffer
        glFramebufferRenderbuffer(
            GL_DRAW_FRAMEBUFFER,
            GL_DEPTH_ATTACHMENT,
            GL_RENDERBUFFER,
            depthRenderBuffer);
    }
    assert(!glGetError());
    // Create any color attachments we might have
    if (numColorAttachments > 0)
    {
        if (activeFbo != 0)
        {
            // Make sure the read and draw buffers are set
            glDrawBuffer(GL_BACK);
            glReadBuffer(GL_BACK);
        }

        colorTextures = new Texture2D*[numColorAttachments];

        for (int i = 0; i < numColorAttachments; i++)
        {
            // Create an empty color texture
            colorTextures[i] = new Texture2D(
                GL_RGBA, GL_RGBA, GL_FLOAT, width, height);

            // Attach the texture to the color attachment point
            glFramebufferTexture2D(
                GL_DRAW_FRAMEBUFFER,
                colorAttachmentEnums[i],
                GL_TEXTURE_2D,
                colorTextures[i]->GetID(),
                0);
        }
    }
    else
    {
        // No color attachments
        colorTextures = NULL;
        glDrawBuffer(GL_NONE);
        //glReadBuffer(GL_NONE);
    }
    assert(!glGetError());
    // Check that the FBO is complete
    GLenum status = glCheckFramebufferStatus(GL_DRAW_FRAMEBUFFER);
    if (status != GL_FRAMEBUFFER_COMPLETE)
    {
        std::cerr << "Frame buffer creation failed with ";

        switch (status)
        {
        case GL_FRAMEBUFFER_UNDEFINED:
            std::cerr << "GL_FRAMEBUFFER_UNDEFINED";
            break;

        case GL_FRAMEBUFFER_INCOMPLETE_ATTACHMENT:
            std::cerr << "GL_FRAMEBUFFER_INCOMPLETE_ATTACHMENT";
            break;

        case GL_FRAMEBUFFER_INCOMPLETE_MISSING_ATTACHMENT:
            std::cerr << "GL_FRAMEBUFFER_INCOMPLETE_MISSING_ATTACHMENT";
            break;

        case GL_FRAMEBUFFER_INCOMPLETE_DRAW_BUFFER:
            std::cerr << "GL_FRAMEBUFFER_INCOMPLETE_DRAW_BUFFER";
            break;

        case GL_FRAMEBUFFER_INCOMPLETE_READ_BUFFER:
            std::cerr << "GL_FRAMEBUFFER_INCOMPLETE_READ_BUFFER";
            break;

        case GL_FRAMEBUFFER_UNSUPPORTED:
            std::cerr << "GL_FRAMEBUFFER_UNSUPPORTED";
            break;

        case GL_FRAMEBUFFER_INCOMPLETE_MULTISAMPLE:
            std::cerr << "GL_FRAMEBUFFER_INCOMPLETE_MULTISAMPLE";
            break;

        case GL_FRAMEBUFFER_INCOMPLETE_LAYER_TARGETS:
            std::cerr << "GL_FRAMEBUFFER_INCOMPLETE_LAYER_TARGETS";
            break;

        default:
            std::cerr << "an unknown error: " << status;
            break;
        }

        std::cerr << std::endl;

        assert(false);
    }
    assert(!glGetError());
    // Unbind the FBO
    glBindFramebuffer(GL_DRAW_FRAMEBUFFER, 0);

    // Restore the read and draw buffers
    if (numColorAttachments == 0)
    {
        glDrawBuffer(GL_BACK);
        glReadBuffer(GL_BACK);
    }
    assert(!glGetError());
    // There is no active FBO at the moment.
    // If one was bound before this framebuffer was created,
    // it is now unbound
    FrameBuffer::activeFbo = 0;
}
