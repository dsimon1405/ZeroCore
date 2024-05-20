#pragma once

#include "ZC_Framebuffer.h"
#include "ZC_Viewport.h"
#include "ZC_Renderbuffer.h"
#include "Texture/ZC_Texture.h"
#include "ZC_FBOBuffersController.h"
#include <ZC/Events/ZC_EC.h>

class ZC_SWindowHolder;

struct ZC_FBO
{
    friend class ZC_SWindowHolder;
    //  specifies FBO's color format (or attachs renderbuffer of the main window to FBO)
    enum ColorInternalFormat
    {
        CIF_None,        //  fbo without color
        CIF_Default,     //  attachs color renderbuffer of the main window                                 is it supported???

        CIF_GL_R8 = GL_R8,
        CIF_GL_R8_SNORM = GL_R8_SNORM,
        CIF_GL_R16 = GL_R16,
        CIF_GL_R16_SNORM = GL_R16_SNORM,
        CIF_GL_RG8 = GL_RG8,
        CIF_GL_RG8_SNORM = GL_RG8_SNORM,
        CIF_GL_RG16 = GL_RG16,
        CIF_GL_RG16_SNORM = GL_RG16_SNORM,
        CIF_GL_R3_G3_B2 = GL_R3_G3_B2,
        CIF_GL_RGB4 = GL_RGB4,
        CIF_GL_RGB5 = GL_RGB5,
        CIF_GL_RGB8 = GL_RGB8,
        CIF_GL_RGB8_SNORM = GL_RGB8_SNORM,
        CIF_GL_RGB10 = GL_RGB10,
        CIF_GL_RGB12 = GL_RGB12,
        CIF_GL_RGB16_SNORM = GL_RGB16_SNORM,
        CIF_GL_RGBA2 = GL_RGBA2,
        CIF_GL_RGBA4 = GL_RGBA4,
        CIF_GL_RGB5_A1 = GL_RGB5_A1,
        CIF_GL_RGBA8 = GL_RGBA8,
        CIF_GL_RGBA8_SNORM = GL_RGBA8_SNORM,
        CIF_GL_RGB10_A2 = GL_RGB10_A2,
        CIF_GL_RGB10_A2UI = GL_RGB10_A2UI,
        CIF_GL_RGBA12 = GL_RGBA12,
        CIF_GL_RGBA16 = GL_RGBA16,
        CIF_GL_SRGB8 = GL_SRGB8,
        CIF_GL_SRGB8_ALPHA8 = GL_SRGB8_ALPHA8,
        CIF_GL_R16F = GL_R16F,
        CIF_GL_RG16F = GL_RG16F,
        CIF_GL_RGB16F = GL_RGB16F,
        CIF_GL_RGBA16F = GL_RGBA16F,
        CIF_GL_R32F = GL_R32F,
        CIF_GL_RG32F = GL_RG32F,
        CIF_GL_RGB32F = GL_RGB32F,
        CIF_GL_RGBA32F = GL_RGBA32F,
        CIF_GL_R11F_G11F_B10F = GL_R11F_G11F_B10F,
        CIF_GL_RGB9_E5 = GL_RGB9_E5,
        CIF_GL_R8I = GL_R8I,
        CIF_GL_R8UI = GL_R8UI,
        CIF_GL_R16I = GL_R16I,
        CIF_GL_R16UI = GL_R16UI,
        CIF_GL_R32I = GL_R32I,
        CIF_GL_R32UI = GL_R32UI,
        CIF_GL_RG8I = GL_RG8I,
        CIF_GL_RG8UI = GL_RG8UI,
        CIF_GL_RG16I = GL_RG16I,
        CIF_GL_RG16UI = GL_RG16UI,
        CIF_GL_RG32I = GL_RG32I,
        CIF_GL_RG32UI = GL_RG32UI,
        CIF_GL_RGB8I = GL_RGB8I,
        CIF_GL_RGB8UI = GL_RGB8UI,
        CIF_GL_RGB16I = GL_RGB16I,
        CIF_GL_RGB16UI = GL_RGB16UI,
        CIF_GL_RGB32I = GL_RGB32I,
        CIF_GL_RGB32UI = GL_RGB32UI,
        CIF_GL_RGBA8I = GL_RGBA8I,
        CIF_GL_RGBA8UI = GL_RGBA8UI,
        CIF_GL_RGBA16I = GL_RGBA16I,
        CIF_GL_RGBA16UI = GL_RGBA16UI,
        CIF_GL_RGBA32I = GL_RGBA32I,
        CIF_GL_RGBA32UI = GL_RGBA32UI,
    };

    //  specifies FBO's depth-stencil format (or attachs renderbuffer of the main window to FBO)
    enum DepthStencilInternalFormat
    {
        DSIF_None,       //  fbo without depth or stencil
        DSIF_Default,    //  attachs depth-stencil renderbuffer of the main window                                 is it supported???

        DSIF_GL_DEPTH_COMPONENT16 = GL_DEPTH_COMPONENT16,
        DSIF_GL_DEPTH_COMPONENT24 = GL_DEPTH_COMPONENT24,
        DSIF_GL_DEPTH_COMPONENT32F = GL_DEPTH_COMPONENT32F,
        DSIF_GL_DEPTH24_STENCIL8 = GL_DEPTH24_STENCIL8,
        DSIF_GL_DEPTH32F_STENCIL8 = GL_DEPTH32F_STENCIL8,
        DSIF_GL_STENCIL_INDEX8 = GL_STENCIL_INDEX8
    };

    /*
    Create textures for color and stencil
    
    Params:
    - viewport - viewport for rendering.
    - _samples - count of samples on pixel.
    - _cif - internal format of color buffer (if nessesary).
    - _dsif - internal format of depth stencil buffer (if nessesary).
    - _resultTexMask - mask of bits for resulting texture. May include each of GL_COLOR_BUFFER_BIT, GL_DEPTH_BUFFER_BIT, GL_STENCIL_BUFFER_IT or 0.
    // - resultTexture - resulting textures (if nessesary) (posible textures: color, depth/stencil).
    - useWindowSize - use window resize event to resize viewport, framebuffer's buffers, resultin textures.
    */
    ZC_FBO(const ZC_Viewport& _viewport, GLsizei _samples, ColorInternalFormat _cif, DepthStencilInternalFormat _dsif, GLbitfield _resultTexMask, bool useWindowSize);

    ZC_FBO(ZC_FBO&& fbo);

    ~ZC_FBO();

    //  Activates framebuffer, viewport and clear supported buffers (color, depth, stencil).
    void Activate();
    // Called after rendering in the framebuffer is complete. If framebuffer multisample, updates the result textures using glBlit. And bind deafult frambuffer.
    void Deactivate();
    const ZC_Texture* GetColorTexture() const noexcept;
    const ZC_Texture* GetDepthTexture() const noexcept;
    void SetClearColor(GLfloat red, GLfloat green, GLfloat blue, GLfloat alpha);

private:
    ZC_Viewport viewport;
    GLsizei samples = 0;
    ColorInternalFormat cif;
    DepthStencilInternalFormat dsif;
    ZC_FBOBuffersController buffersController;
    GLbitfield resultTexMask;
    // bool blitColor = false,
    //     blitDepth = false,
    //     blitStencil = false;

    ZC_Renderbuffer rendBuffColor;      //  uses on multisampling or if don't need resulting color texture (texColor)
    ZC_Renderbuffer rendBuffDepthStencil;     //  uses on multisampling or if don't need resulting depth/stencil texture (texDepthStencil)

    //  resulting textures from resultTexture
    ZC_Texture texColor,
        texDepthStencil;

    ZC_Framebuffer framebuffer,
        framebufferForBlit;     //  uses for blit from framebuffer in multisample case

    ZC_EC ecWindowResize;

    //  only for ZC_SWindowHolder use.
    ZC_FBO();

    ZC_FBOBuffersController CreateBufferController();
    ZC_Renderbuffer CreateColorRenderbuffer();
    ZC_Renderbuffer CreateDepthStencilRenderbuffer();
    ZC_Texture CreateColorTexture();
    ZC_Texture CreateDepthStencilTexture();
    ZC_Framebuffer CreateFrameBuffer();
    ZC_Framebuffer CreateFrameBufferForBlit();
    void WindowResize(float width, float height);
};