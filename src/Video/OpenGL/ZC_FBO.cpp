#include <ZC/Video/OpenGL/ZC_FBO.h>

#include <ZC/Events/ZC_Events.h>
#include <ZC/Video/OpenGL/ZC_OpenGL.h>

#include <cassert>

ZC_FBO::ZC_FBO(const ZC_Viewport& _viewport, int _samples, ColorInternalFormat _cif, DepthStencilInternalFormat _dsif, ResultTexture resultTexture, bool useWindowSize)
    : viewport(_viewport),
    samples(_samples),
    cif(_cif),
    dsif(_dsif),
    buffersController(CreateBufferController()),
    blitColor(resultTexture == RT_Color || resultTexture == RT_Color_Depth || resultTexture ==  RT_Color_Depth_Stencil),
    blitDepth(resultTexture == RT_Depth || resultTexture == RT_Depth_Stencil || resultTexture == RT_Color_Depth_Stencil),
    blitStencil(resultTexture == RT_Depth_Stencil || resultTexture == RT_Color_Depth_Stencil),
    rendBuffColor(CreateColorRenderbuffer()),
    rendBuffDepthStencil(CreateDepthStencilRenderbuffer()),
    texColor(CreateColorTexture()),
    texDepthStencil(CreateDepthStencilTexture()),
    framebuffer(CreateFrameBuffer()),
    framebufferForBlit(CreateFrameBufferForBlit()),
    sconWindowResize(useWindowSize ? ZC_Events::ConnectWindowResize({ &ZC_FBO::WindowResize, this }) : ZC_SConnection())
{}

ZC_FBO::ZC_FBO(ZC_FBO&& fbo)
    : viewport(fbo.viewport),
    samples(fbo.samples),
    cif(fbo.cif),
    dsif(fbo.dsif),
    buffersController(fbo.buffersController),
    blitColor(fbo.blitColor),
    blitDepth(fbo.blitDepth),
    blitStencil(fbo.blitStencil),
    rendBuffColor(std::move(fbo.rendBuffColor)),
    rendBuffDepthStencil(std::move(fbo.rendBuffDepthStencil)),
    texColor(std::move(fbo.texColor)),
    texDepthStencil(std::move(fbo.texDepthStencil)),
    framebuffer(std::move(fbo.framebuffer)),
    framebufferForBlit(std::move(fbo.framebufferForBlit)),
    sconWindowResize(fbo.sconWindowResize.IsConnected() ? ZC_Events::ConnectWindowResize({ &ZC_FBO::WindowResize, this }) : ZC_SConnection())    //  if connected need reconnect, in signal pointer to moved object
{}

ZC_FBO::~ZC_FBO()
{
    sconWindowResize.Disconnect();
}

void ZC_FBO::Use()
{
    viewport.Use();
    framebuffer.Bind();
    buffersController.MakeActive();
    buffersController.GlClear();
}

void ZC_FBO::Finish()
{
    //  the framebuffer for the blit is configured, main framebuffer is multisample
    if (framebufferForBlit.GetId() != 0)
    {
        if (blitColor)
            framebuffer.BlitTo(framebufferForBlit, 0, 0, viewport.GetWidth(), viewport.GetHeight(), 0, 0, viewport.GetWidth(), viewport.GetHeight(), GL_COLOR_BUFFER_BIT);
        if (blitDepth)
            framebuffer.BlitTo(framebufferForBlit, 0, 0, viewport.GetWidth(), viewport.GetHeight(), 0, 0, viewport.GetWidth(), viewport.GetHeight(), GL_DEPTH_BUFFER_BIT);
        if (blitStencil)
            framebuffer.BlitTo(framebufferForBlit, 0, 0, viewport.GetWidth(), viewport.GetHeight(), 0, 0, viewport.GetWidth(), viewport.GetHeight(), GL_STENCIL_BUFFER_BIT);
    }

    framebuffer.Unbind();
}

const ZC_Texture* ZC_FBO::GetColorTexture() const noexcept
{
    return texColor.GetId() != 0 ? &texColor : nullptr;
}

const ZC_Texture* ZC_FBO::GetDepthTexture() const noexcept
{
    return texDepthStencil.GetId() != 0 ? &texDepthStencil : nullptr;
}

ZC_FBO::ZC_FBO()
    : viewport(ZC_Viewport::CreateStandardWindowViewport()),
    buffersController{ true, true, true },
    sconWindowResize(ZC_Events::ConnectWindowResize({ &ZC_FBO::WindowResize, this }))
{}

ZC_FBOBuffersController ZC_FBO::CreateBufferController()
{
    bool color = false,
        depth = false,
        stencil = false;

    if (cif != CIF_None) color = true;

    switch (dsif)
    {
    case DSIF_None: break;
    case DSIF_GL_DEPTH_COMPONENT16: depth = true; break;
    case DSIF_GL_DEPTH_COMPONENT24: depth = true; break;
    case DSIF_GL_DEPTH_COMPONENT32F: depth = true; break;
    default:    //  other cases are depth and stencil supported
    {
        depth = true;
        stencil = true;
    } break;
    }

    return { color, depth, stencil };
}

ZC_Renderbuffer ZC_FBO::CreateColorRenderbuffer()
{
    //  needs color attachment and don't uses color attachment of the default renderbuffer
    if (cif != CIF_None && cif != CIF_Default)
    {
        if (samples > 0) return ZC_Renderbuffer(samples, cif, viewport.GetWidth(), viewport.GetHeight());   //  for multisampling allways creates
        //  without multisampling creates only, if don't need resulting texture with color
        else if (!blitColor) return ZC_Renderbuffer(cif, viewport.GetWidth(), viewport.GetHeight());            
    }
    return {};
}

ZC_Renderbuffer ZC_FBO::CreateDepthStencilRenderbuffer()
{
    //  needs depth-stencil attachment and don't uses depth-stencil attachment of the default renderbuffer
    if (dsif != DSIF_None && dsif != DSIF_Default)
    {
        if (samples > 0) return ZC_Renderbuffer(samples, dsif, viewport.GetWidth(), viewport.GetHeight());   //  for multisampling allways creates
        //  without multisampling, create if don't need result texture with depth
        else if (!blitDepth) return ZC_Renderbuffer(dsif, viewport.GetWidth(), viewport.GetHeight());
    }
    return {};
}

ZC_Texture ZC_FBO::CreateColorTexture()
{
    switch (cif)
    {
    case CIF_GL_R8: return ZC_Texture(GL_TEXTURE_2D, cif, viewport.GetWidth(), viewport.GetHeight(), GL_RED, GL_UNSIGNED_BYTE, nullptr, GL_CLAMP_TO_EDGE, GL_CLAMP_TO_EDGE, GL_LINEAR, GL_LINEAR, false);
    case CIF_GL_R8UI: return ZC_Texture(GL_TEXTURE_2D, cif, viewport.GetWidth(), viewport.GetHeight(), GL_RED_INTEGER, GL_UNSIGNED_BYTE, nullptr, GL_CLAMP_TO_EDGE, GL_CLAMP_TO_EDGE, GL_LINEAR, GL_LINEAR, false);
    case CIF_GL_R8I: return ZC_Texture(GL_TEXTURE_2D, cif, viewport.GetWidth(), viewport.GetHeight(), GL_RED_INTEGER, GL_BYTE, nullptr, GL_CLAMP_TO_EDGE, GL_CLAMP_TO_EDGE, GL_LINEAR, GL_LINEAR, false);
    case CIF_GL_R16UI: return ZC_Texture(GL_TEXTURE_2D, cif, viewport.GetWidth(), viewport.GetHeight(), GL_RED_INTEGER, GL_UNSIGNED_SHORT, nullptr, GL_CLAMP_TO_EDGE, GL_CLAMP_TO_EDGE, GL_LINEAR, GL_LINEAR, false);
    case CIF_GL_R16I: return ZC_Texture(GL_TEXTURE_2D, cif, viewport.GetWidth(), viewport.GetHeight(), GL_RED_INTEGER, GL_SHORT, nullptr, GL_CLAMP_TO_EDGE, GL_CLAMP_TO_EDGE, GL_LINEAR, GL_LINEAR, false);
    case CIF_GL_R32UI: return ZC_Texture(GL_TEXTURE_2D, cif, viewport.GetWidth(), viewport.GetHeight(), GL_RED_INTEGER, GL_UNSIGNED_INT, nullptr, GL_CLAMP_TO_EDGE, GL_CLAMP_TO_EDGE, GL_LINEAR, GL_LINEAR, false);
    case CIF_GL_R32I: return ZC_Texture(GL_TEXTURE_2D, cif, viewport.GetWidth(), viewport.GetHeight(), GL_RED_INTEGER, GL_INT, nullptr, GL_CLAMP_TO_EDGE, GL_CLAMP_TO_EDGE, GL_LINEAR, GL_LINEAR, false);
    case CIF_GL_RG8: return ZC_Texture(GL_TEXTURE_2D, cif, viewport.GetWidth(), viewport.GetHeight(), GL_RG, GL_UNSIGNED_BYTE, nullptr, GL_CLAMP_TO_EDGE, GL_CLAMP_TO_EDGE, GL_LINEAR, GL_LINEAR, false);
    case CIF_GL_RG8UI: return ZC_Texture(GL_TEXTURE_2D, cif, viewport.GetWidth(), viewport.GetHeight(), GL_RG_INTEGER, GL_UNSIGNED_BYTE, nullptr, GL_CLAMP_TO_EDGE, GL_CLAMP_TO_EDGE, GL_LINEAR, GL_LINEAR, false);
    case CIF_GL_RG8I: return ZC_Texture(GL_TEXTURE_2D, cif, viewport.GetWidth(), viewport.GetHeight(), GL_RG_INTEGER, GL_BYTE, nullptr, GL_CLAMP_TO_EDGE, GL_CLAMP_TO_EDGE, GL_LINEAR, GL_LINEAR, false);
    case CIF_GL_RG16UI: return ZC_Texture(GL_TEXTURE_2D, cif, viewport.GetWidth(), viewport.GetHeight(), GL_RG_INTEGER, GL_UNSIGNED_SHORT, nullptr, GL_CLAMP_TO_EDGE, GL_CLAMP_TO_EDGE, GL_LINEAR, GL_LINEAR, false);
    case CIF_GL_RG16I: return ZC_Texture(GL_TEXTURE_2D, cif, viewport.GetWidth(), viewport.GetHeight(), GL_RG_INTEGER, GL_SHORT, nullptr, GL_CLAMP_TO_EDGE, GL_CLAMP_TO_EDGE, GL_LINEAR, GL_LINEAR, false);
    case CIF_GL_RG32UI: return ZC_Texture(GL_TEXTURE_2D, cif, viewport.GetWidth(), viewport.GetHeight(), GL_RG_INTEGER, GL_UNSIGNED_INT, nullptr, GL_CLAMP_TO_EDGE, GL_CLAMP_TO_EDGE, GL_LINEAR, GL_LINEAR, false);
    case CIF_GL_RG32I: return ZC_Texture(GL_TEXTURE_2D, cif, viewport.GetWidth(), viewport.GetHeight(), GL_RG_INTEGER, GL_INT, nullptr, GL_CLAMP_TO_EDGE, GL_CLAMP_TO_EDGE, GL_LINEAR, GL_LINEAR, false);
    case CIF_GL_RGB8: return ZC_Texture(GL_TEXTURE_2D, cif, viewport.GetWidth(), viewport.GetHeight(), GL_RGB, GL_UNSIGNED_BYTE, nullptr, GL_CLAMP_TO_EDGE, GL_CLAMP_TO_EDGE, GL_LINEAR, GL_LINEAR, false);
    case CIF_GL_RGB565: return ZC_Texture(GL_TEXTURE_2D, cif, viewport.GetWidth(), viewport.GetHeight(), GL_RGB, GL_UNSIGNED_SHORT_5_6_5, nullptr, GL_CLAMP_TO_EDGE, GL_CLAMP_TO_EDGE, GL_LINEAR, GL_LINEAR, false);
    case CIF_GL_RGBA8: return ZC_Texture(GL_TEXTURE_2D, cif, viewport.GetWidth(), viewport.GetHeight(), GL_RGBA, GL_UNSIGNED_BYTE, nullptr, GL_CLAMP_TO_EDGE, GL_CLAMP_TO_EDGE, GL_LINEAR, GL_LINEAR, false);
    case CIF_GL_SRGB8_ALPHA8: return ZC_Texture(GL_TEXTURE_2D, cif, viewport.GetWidth(), viewport.GetHeight(), GL_RGBA, GL_BYTE, nullptr, GL_CLAMP_TO_EDGE, GL_CLAMP_TO_EDGE, GL_LINEAR, GL_LINEAR, false);
    case CIF_GL_RGB5_A1: return ZC_Texture(GL_TEXTURE_2D, cif, viewport.GetWidth(), viewport.GetHeight(), GL_RGBA, GL_UNSIGNED_SHORT_5_5_5_1, nullptr, GL_CLAMP_TO_EDGE, GL_CLAMP_TO_EDGE, GL_LINEAR, GL_LINEAR, false);
    case CIF_GL_RGBA4: return ZC_Texture(GL_TEXTURE_2D, cif, viewport.GetWidth(), viewport.GetHeight(), GL_RGBA, GL_UNSIGNED_SHORT_4_4_4_4, nullptr, GL_CLAMP_TO_EDGE, GL_CLAMP_TO_EDGE, GL_LINEAR, GL_LINEAR, false);
    case CIF_GL_RGB10_A2: return ZC_Texture(GL_TEXTURE_2D, cif, viewport.GetWidth(), viewport.GetHeight(), GL_RGBA, GL_UNSIGNED_INT_10_10_10_2, nullptr, GL_CLAMP_TO_EDGE, GL_CLAMP_TO_EDGE, GL_LINEAR, GL_LINEAR, false);
    case CIF_GL_RGBA8UI: return ZC_Texture(GL_TEXTURE_2D, cif, viewport.GetWidth(), viewport.GetHeight(), GL_RGBA_INTEGER, GL_UNSIGNED_BYTE, nullptr, GL_CLAMP_TO_EDGE, GL_CLAMP_TO_EDGE, GL_LINEAR, GL_LINEAR, false);
    case CIF_GL_RGBA8I: return ZC_Texture(GL_TEXTURE_2D, cif, viewport.GetWidth(), viewport.GetHeight(), GL_RGBA_INTEGER, GL_BYTE, nullptr, GL_CLAMP_TO_EDGE, GL_CLAMP_TO_EDGE, GL_LINEAR, GL_LINEAR, false);
    case CIF_GL_RGB10_A2UI: return ZC_Texture(GL_TEXTURE_2D, cif, viewport.GetWidth(), viewport.GetHeight(), GL_RGBA_INTEGER, GL_UNSIGNED_INT_10_10_10_2, nullptr, GL_CLAMP_TO_EDGE, GL_CLAMP_TO_EDGE, GL_LINEAR, GL_LINEAR, false);
    case CIF_GL_RGBA16UI: return ZC_Texture(GL_TEXTURE_2D, cif, viewport.GetWidth(), viewport.GetHeight(), GL_RGBA_INTEGER, GL_UNSIGNED_SHORT, nullptr, GL_CLAMP_TO_EDGE, GL_CLAMP_TO_EDGE, GL_LINEAR, GL_LINEAR, false);
    case CIF_GL_RGBA16I: return ZC_Texture(GL_TEXTURE_2D, cif, viewport.GetWidth(), viewport.GetHeight(), GL_RGBA_INTEGER, GL_SHORT, nullptr, GL_CLAMP_TO_EDGE, GL_CLAMP_TO_EDGE, GL_LINEAR, GL_LINEAR, false);
    case CIF_GL_RGBA32I: return ZC_Texture(GL_TEXTURE_2D, cif, viewport.GetWidth(), viewport.GetHeight(), GL_RGBA_INTEGER, GL_INT, nullptr, GL_CLAMP_TO_EDGE, GL_CLAMP_TO_EDGE, GL_LINEAR, GL_LINEAR, false);
    case CIF_GL_RGBA32UI: return ZC_Texture(GL_TEXTURE_2D, cif, viewport.GetWidth(), viewport.GetHeight(), GL_RGBA_INTEGER, GL_UNSIGNED_INT, nullptr, GL_CLAMP_TO_EDGE, GL_CLAMP_TO_EDGE, GL_LINEAR, GL_LINEAR, false);
    default: return {};
    }
}

ZC_Texture ZC_FBO::CreateDepthStencilTexture()
{
    switch (dsif)
    {
    case DSIF_GL_DEPTH_COMPONENT16: return ZC_Texture(GL_TEXTURE_2D, dsif, viewport.GetWidth(), viewport.GetHeight(), GL_DEPTH_COMPONENT, GL_UNSIGNED_SHORT, nullptr, GL_CLAMP_TO_EDGE, GL_CLAMP_TO_EDGE, GL_NEAREST, GL_NEAREST, false);
    case DSIF_GL_DEPTH_COMPONENT24: return ZC_Texture(GL_TEXTURE_2D, dsif, viewport.GetWidth(), viewport.GetHeight(), GL_DEPTH_COMPONENT, GL_UNSIGNED_INT, nullptr, GL_CLAMP_TO_EDGE, GL_CLAMP_TO_EDGE, GL_NEAREST, GL_NEAREST, false);
    case DSIF_GL_DEPTH_COMPONENT32F: return ZC_Texture(GL_TEXTURE_2D, dsif, viewport.GetWidth(), viewport.GetHeight(), GL_DEPTH_COMPONENT, GL_FLOAT, nullptr, GL_CLAMP_TO_EDGE, GL_CLAMP_TO_EDGE, GL_NEAREST, GL_NEAREST, false);
    case DSIF_GL_DEPTH24_STENCIL8: return ZC_Texture(GL_TEXTURE_2D, dsif, viewport.GetWidth(), viewport.GetHeight(), GL_DEPTH_STENCIL, GL_UNSIGNED_INT_24_8, nullptr, GL_CLAMP_TO_EDGE, GL_CLAMP_TO_EDGE, GL_NEAREST, GL_NEAREST, false);
    case DSIF_GL_DEPTH32F_STENCIL8: return ZC_Texture(GL_TEXTURE_2D, dsif, viewport.GetWidth(), viewport.GetHeight(), GL_DEPTH_STENCIL, GL_FLOAT_32_UNSIGNED_INT_24_8_REV, nullptr, GL_CLAMP_TO_EDGE, GL_CLAMP_TO_EDGE, GL_NEAREST, GL_NEAREST, false);
    default: return {};
    }
}

ZC_Framebuffer ZC_FBO::CreateFrameBuffer()
{
    ZC_Framebuffer _framebuffer(true);
    _framebuffer.Bind();

    //  color
    if (rendBuffColor.GetId() != 0) glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_RENDERBUFFER, rendBuffColor.GetId());
    else if (texColor.GetId() != 0) glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, texColor.GetId(), 0);
    else if (cif == CIF_Default) glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_RENDERBUFFER, 0);
    else
    {   //  color buffer don't uses, disable drawing and reading for them!
        glDrawBuffer(GL_NONE);
        glReadBuffer(GL_NONE);
    }
    //  depth stencil
    GLenum attechment = buffersController.haveDepthBuffer && buffersController.haveStencilBuffer ? GL_DEPTH_STENCIL_ATTACHMENT :
        buffersController.haveDepthBuffer ? GL_DEPTH_ATTACHMENT : GL_STENCIL_ATTACHMENT;
    if (rendBuffDepthStencil.GetId() != 0) glFramebufferRenderbuffer(GL_FRAMEBUFFER, attechment, GL_RENDERBUFFER, rendBuffDepthStencil.GetId());
    else if (texDepthStencil.GetId() != 0) glFramebufferTexture2D(GL_FRAMEBUFFER, attechment, GL_TEXTURE_2D, texDepthStencil.GetId(), 0);
    else if (dsif == DSIF_Default)  glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, 0);

    ErrorCheck();
    return _framebuffer;
}

ZC_Framebuffer ZC_FBO::CreateFrameBufferForBlit()
{
    //  framebuffer for blit creates only if main framebuffer multisample, and if needs result texture
    if (samples == 0 || (!blitColor && !blitDepth && !blitStencil)) return { false };

    ZC_Framebuffer _framebufferForBlit(true);
    _framebufferForBlit.Bind();

    if (blitColor) glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, texColor.GetId(), 0);
    if (blitDepth) glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, texDepthStencil.GetId(), 0);
    if (blitStencil) glFramebufferTexture2D(GL_FRAMEBUFFER, GL_STENCIL_ATTACHMENT, GL_TEXTURE_2D, texDepthStencil.GetId(), 0);

    ErrorCheck();
    return _framebufferForBlit;
}

void ZC_FBO::ErrorCheck()
{
    switch (glCheckFramebufferStatus(GL_FRAMEBUFFER))
    {
    case GL_FRAMEBUFFER_COMPLETE: break;
    case GL_FRAMEBUFFER_UNDEFINED: assert(false); break;
    case GL_FRAMEBUFFER_INCOMPLETE_ATTACHMENT: assert(false); break;
    case GL_FRAMEBUFFER_INCOMPLETE_MISSING_ATTACHMENT: assert(false); break;
    case GL_FRAMEBUFFER_INCOMPLETE_DRAW_BUFFER: assert(false); break;
    case GL_FRAMEBUFFER_INCOMPLETE_READ_BUFFER: assert(false); break;
    case GL_FRAMEBUFFER_UNSUPPORTED: assert(false); break;
    case GL_FRAMEBUFFER_INCOMPLETE_MULTISAMPLE: assert(false); break;
    case GL_FRAMEBUFFER_INCOMPLETE_LAYER_TARGETS: assert(false); break;
    case 0: assert(false); break;  //  errors: GL_INVALID_ENUM or GL_INVALID_OPERATION
    default: assert(false); //  undefined behaviour
    }
}

void ZC_FBO::WindowResize(float width, float height)
{
    // if (framebuffer.GetId() == 0) viewport.SetSize(width, height);
    // else viewport.SetSize(200, 200);

    viewport.SetSize(width, height);
    if (rendBuffColor.GetId() != 0) rendBuffColor = std::move(CreateColorRenderbuffer());
    if (rendBuffDepthStencil.GetId() != 0) rendBuffDepthStencil = std::move(CreateDepthStencilRenderbuffer());
    if (texColor.GetId() != 0) texColor = std::move(CreateColorTexture());
    if (texDepthStencil.GetId() != 0) texDepthStencil = std::move(CreateDepthStencilTexture());
    if (framebuffer.GetId() != 0) framebuffer = std::move(CreateFrameBuffer());
    if (framebufferForBlit.GetId() != 0) framebufferForBlit = std::move(CreateFrameBufferForBlit());
}

void ZC_FBO::SetClearColor(float red, float green, float blue, float alpha)
{
    buffersController.SetClearColor({ red, green, blue, alpha });
}