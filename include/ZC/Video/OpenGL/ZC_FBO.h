#pragma once

#include "ZC_Framebuffer.h"
#include "ZC_Viewport.h"
#include "ZC_Renderbuffer.h"
#include "Texture/ZC_Texture.h"
#include "ZC_FBOBuffersController.h"
#include <ZC/Tools/Signal/ZC_SConnection.h>

class ZC_WindowHolder;

struct ZC_FBO
{
    friend class ZC_WindowHolder;
    //  specifies FBO's color format (or attachs renderbuffer of the main window to FBO)
    enum ColorInternalFormat
    {
        CIF_None,        //  fbo without color
        CIF_Default,     //  attachs color renderbuffer of the main window                                 is it supported???
        CIF_GL_R8 = GL_R8,
        CIF_GL_R8UI = GL_R8UI,
        CIF_GL_R8I = GL_R8I,
        CIF_GL_R16UI = GL_R16UI,
        CIF_GL_R16I = GL_R16I,
        CIF_GL_R32UI = GL_R32UI,
        CIF_GL_R32I = GL_R32I,
        CIF_GL_RG8 = GL_RG8,
        CIF_GL_RG8UI = GL_RG8UI,
        CIF_GL_RG8I = GL_RG8I,
        CIF_GL_RG16UI = GL_RG16UI,
        CIF_GL_RG16I = GL_RG16I,
        CIF_GL_RG32UI = GL_RG32UI,
        CIF_GL_RG32I = GL_RG32I,
        CIF_GL_RGB8 = GL_RGB8,
        CIF_GL_RGB565 = GL_RGB565,
        CIF_GL_RGBA8 = GL_RGBA8,
        CIF_GL_SRGB8_ALPHA8 = GL_SRGB8_ALPHA8,
        CIF_GL_RGB5_A1 = GL_RGB5_A1,
        CIF_GL_RGBA4 = GL_RGBA4,
        CIF_GL_RGB10_A2 = GL_RGB10_A2,
        CIF_GL_RGBA8UI = GL_RGBA8UI,
        CIF_GL_RGBA8I = GL_RGBA8I,
        CIF_GL_RGB10_A2UI = GL_RGB10_A2UI,
        CIF_GL_RGBA16UI = GL_RGBA16UI,
        CIF_GL_RGBA16I = GL_RGBA16I,
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
    };
    
    enum ResultTexture
    {
        RT_None,
        RT_Color,
        RT_Depth,
        RT_Color_Depth,
        RT_Depth_Stencil,
        RT_Color_Depth_Stencil
    };

    /*
    Create textures for color and stencil
    
    Params:
    - viewport - viewport for rendering.
    - _samples - count of samples on pixel.
    - _cif - internal format of color buffer (if nessesary).
    - _dsif - internal format of depth stencil buffer (if nessesary).
    - resultTexture - resulting textures (if nessesary) (posible textures: color, depth/stencil).
    - useWindowSize - use window resize event to resize viewport, framebuffer's buffers, resultin textures.
    */
    ZC_FBO(const ZC_Viewport& _viewport, int _samples, ColorInternalFormat _cif, DepthStencilInternalFormat _dsif, ResultTexture resultTexture, bool useWindowSize);

    ZC_FBO(ZC_FBO&& fbo);

    ~ZC_FBO();

    ZC_FBOBuffersController& GetBuffersController();
    //  Activates framebuffer, viewport and clear supported buffers (color, depth, stencil).
    void Use();
    // Called after rendering in the framebuffer is complete. If framebuffer multisample, updates the result textures using glBlit. And bind deafult frambuffer.
    void Finish();
    const ZC_Texture* GetColorTexture() const noexcept;
    const ZC_Texture* GetDepthTexture() const noexcept;
    void SetClearColor(float red, float green, float blue, float alpha);

private:
    ZC_Viewport viewport;
    int samples = 0;
    ColorInternalFormat cif;
    DepthStencilInternalFormat dsif;
    ZC_FBOBuffersController buffersController;
    bool blitColor = false,
        blitDepth = false,
        blitStencil = false;

    ZC_Renderbuffer rendBuffColor;      //  uses on multisampling or if don't need resulting color texture
    ZC_Renderbuffer rendBuffDepthStencil;     //  uses on multisampling or if don't need resulting color texture

    //  resulting textures from resultTexture
    ZC_Texture texColor,
        texDepthStencil;

    ZC_Framebuffer framebuffer { false },
        framebufferForBlit { false };     //  use for blit from framebuffer in multisample case

    ZC_SConnection sconWindowResize;

    //  only for ZC_WindowHolder use.
    ZC_FBO();

    ZC_FBOBuffersController CreateBufferController();
    ZC_Renderbuffer CreateColorRenderbuffer();
    ZC_Renderbuffer CreateDepthStencilRenderbuffer();
    ZC_Texture CreateColorTexture();
    ZC_Texture CreateDepthStencilTexture();
    ZC_Framebuffer CreateFrameBuffer();
    ZC_Framebuffer CreateFrameBufferForBlit();
    void ErrorCheck();
    void WindowResize(float width, float height);
};