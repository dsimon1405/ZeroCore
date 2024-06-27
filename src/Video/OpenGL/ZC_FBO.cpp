#include <ZC/Video/OpenGL/ZC_FBO.h>

#include <ZC/Events/ZC_Events.h>

#include <cassert>

ZC_FBO::ZC_FBO(const ZC_Viewport& _viewport, GLsizei _samples, ColorInternalFormat _cif, DepthStencilInternalFormat _dsif, GLbitfield _resultTexMask, bool useWindowSize)
    : viewport(_viewport),
    samples(_samples),
    cif(_cif),
    dsif(_dsif),
    buffersController(CreateBufferController()),
    resultTexMask(_resultTexMask),
    rendBuffColor(CreateColorRenderbuffer()),
    rendBuffDepthStencil(CreateDepthStencilRenderbuffer()),
    texColor(CreateColorTexture()),
    texDepthStencil(CreateDepthStencilTexture()),
    framebuffer(CreateFrameBuffer()),
    framebufferForBlit(CreateFrameBufferForBlit()),
    ecWindowResize(useWindowSize ? ZC_Events::ConnectWindowResize({ &ZC_FBO::WindowResize, this }) : ZC_EC())
{
    assert((resultTexMask & (~(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT))) == 0);
}

ZC_FBO::ZC_FBO(ZC_FBO&& fbo)
    : viewport(fbo.viewport),
    samples(fbo.samples),
    cif(fbo.cif),
    dsif(fbo.dsif),
    buffersController(fbo.buffersController),
    resultTexMask(fbo.resultTexMask),
    rendBuffColor(std::move(fbo.rendBuffColor)),
    rendBuffDepthStencil(std::move(fbo.rendBuffDepthStencil)),
    texColor(std::move(fbo.texColor)),
    texDepthStencil(std::move(fbo.texDepthStencil)),
    framebuffer(std::move(fbo.framebuffer)),
    framebufferForBlit(std::move(fbo.framebufferForBlit)),
    ecWindowResize(fbo.ecWindowResize.IsConnected() ? ZC_Events::ConnectWindowResize({ &ZC_FBO::WindowResize, this }) : ZC_EC())    //  if connected need reconnect, in signal pointer to moved object
{}

ZC_FBO::~ZC_FBO()
{
    ecWindowResize.Disconnect();
}

void ZC_FBO::Activate()
{
    viewport.Use();
    framebuffer.Bind();
    buffersController.MakeActive();
    buffersController.GlClear();
}

void ZC_FBO::Deactivate()
{
    if (framebufferForBlit.GetId() != 0)    //  the framebuffer for the blit is configured, main framebuffer is multisample
        framebuffer.GLBlitNamedFramebuffer(framebufferForBlit, 0, 0, viewport.GetWidth(), viewport.GetHeight(), 0, 0, viewport.GetWidth(), viewport.GetHeight(), resultTexMask);
    //  don't calls framebuffer.Unbind(), next call from Activate() wiil rebind them
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
    ecWindowResize(ZC_Events::ConnectWindowResize({ &ZC_FBO::WindowResize, this }))
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
    case DSIF_GL_STENCIL_INDEX8: stencil = true; break;
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
    {   //  for multisampling allways creates
        if (samples > 0) return ZC_Renderbuffer::GLNamedRenderbufferStorageMultisample(samples, cif, viewport.GetWidth(), viewport.GetHeight());
        //  without multisampling, creates only, if don't need resulting texture with color
        else if ((resultTexMask & GL_COLOR_BUFFER_BIT) == 0) return ZC_Renderbuffer::GLNamedRenderbufferStorage(cif, viewport.GetWidth(), viewport.GetHeight());       
    }
    return {};
}

ZC_Renderbuffer ZC_FBO::CreateDepthStencilRenderbuffer()
{
    //  needs depth-stencil attachment and don't uses depth-stencil attachment of the default renderbuffer
    if (dsif != DSIF_None && dsif != DSIF_Default)
    {   //  for multisampling allways creates
        if (samples > 0) return ZC_Renderbuffer::GLNamedRenderbufferStorageMultisample(samples, dsif, viewport.GetWidth(), viewport.GetHeight());
        //  without multisampling, create if don't need result texture with depth or stencil
        else if ((resultTexMask & (GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT)) == 0)
            return ZC_Renderbuffer::GLNamedRenderbufferStorage(dsif, viewport.GetWidth(), viewport.GetHeight());
    }
    return {};
}

ZC_Texture ZC_FBO::CreateColorTexture()
{
    return cif == CIF_None || cif == CIF_Default ? ZC_Texture()
        : ZC_Texture::TextureStorage2D(cif, 0, viewport.GetWidth(), viewport.GetHeight(), false, GL_CLAMP_TO_EDGE, GL_CLAMP_TO_EDGE, GL_LINEAR, GL_LINEAR);
}

ZC_Texture ZC_FBO::CreateDepthStencilTexture()
{
    return dsif == DSIF_None || dsif == DSIF_Default ? ZC_Texture()
        : ZC_Texture::TextureStorage2D(dsif, 0, viewport.GetWidth(), viewport.GetHeight(), false, GL_CLAMP_TO_EDGE, GL_CLAMP_TO_EDGE, GL_NEAREST, GL_NEAREST);
}

ZC_Framebuffer ZC_FBO::CreateFrameBuffer()
{
    ZC_Framebuffer _framebuffer = ZC_Framebuffer::CreateFramebuffer();

    //  color
    if (rendBuffColor.GetId() != 0) _framebuffer.GLNamedFramebufferRenderbuffer(GL_COLOR_ATTACHMENT0, rendBuffColor.GetId());
    else if (texColor.GetId() != 0) _framebuffer.GLNamedFramebufferTexture(GL_COLOR_ATTACHMENT0, texColor.GetId());
    else if (cif == CIF_Default) _framebuffer.GLNamedFramebufferRenderbuffer(GL_COLOR_ATTACHMENT0, 0);
    else
    {   //  color buffer don't uses, disable drawing and reading for them!
        glDrawBuffer(GL_NONE);
        glReadBuffer(GL_NONE);
    }
    //  depth stencil
    GLenum attechment = buffersController.haveDepthBuffer && buffersController.haveStencilBuffer ? GL_DEPTH_STENCIL_ATTACHMENT :
        buffersController.haveDepthBuffer ? GL_DEPTH_ATTACHMENT : GL_STENCIL_ATTACHMENT;
    if (rendBuffDepthStencil.GetId() != 0) _framebuffer.GLNamedFramebufferRenderbuffer(attechment, rendBuffDepthStencil.GetId());
    else if (texDepthStencil.GetId() != 0) _framebuffer.GLNamedFramebufferTexture(attechment, texDepthStencil.GetId());
    else if (dsif == DSIF_Default) _framebuffer.GLNamedFramebufferRenderbuffer(GL_DEPTH_STENCIL_ATTACHMENT, 0);

    _framebuffer.CheckCreation();
    return _framebuffer;
}

ZC_Framebuffer ZC_FBO::CreateFrameBufferForBlit()
{
    //  framebuffer for blit creates only if main framebuffer multisample, and if needs result texture
    if (samples == 0 || resultTexMask == 0) return {};

    ZC_Framebuffer _framebufferForBlit = ZC_Framebuffer::CreateFramebuffer();

    //  color
    if (resultTexMask & GL_COLOR_BUFFER_BIT) _framebufferForBlit.GLNamedFramebufferTexture(GL_COLOR_ATTACHMENT0, texColor.GetId());
    //  depth stencil
    if ((resultTexMask & GL_DEPTH_BUFFER_BIT) && (resultTexMask & GL_STENCIL_BUFFER_BIT))
        _framebufferForBlit.GLNamedFramebufferTexture(GL_DEPTH_STENCIL_ATTACHMENT, texDepthStencil.GetId());
    else if (resultTexMask & GL_DEPTH_BUFFER_BIT) _framebufferForBlit.GLNamedFramebufferTexture(GL_DEPTH_ATTACHMENT, texDepthStencil.GetId());
    else if (resultTexMask & GL_STENCIL_BUFFER_BIT) _framebufferForBlit.GLNamedFramebufferTexture(GL_STENCIL_ATTACHMENT, texDepthStencil.GetId());

    _framebufferForBlit.CheckCreation();
    return _framebufferForBlit;
}

void ZC_FBO::WindowResize(float width, float height)
{
    viewport.SetSize(static_cast<GLsizei>(width), static_cast<GLsizei>(height));
    if (rendBuffColor.GetId() != 0) rendBuffColor = std::move(CreateColorRenderbuffer());
    if (rendBuffDepthStencil.GetId() != 0) rendBuffDepthStencil = std::move(CreateDepthStencilRenderbuffer());
    if (texColor.GetId() != 0) texColor = std::move(CreateColorTexture());
    if (texDepthStencil.GetId() != 0) texDepthStencil = std::move(CreateDepthStencilTexture());
    if (framebuffer.GetId() != 0) framebuffer = std::move(CreateFrameBuffer());
    if (framebufferForBlit.GetId() != 0) framebufferForBlit = std::move(CreateFrameBufferForBlit());
}

void ZC_FBO::SetClearColor(GLfloat red, GLfloat green, GLfloat blue, GLfloat alpha)
{
    buffersController.SetClearColor({ red, green, blue, alpha });
}