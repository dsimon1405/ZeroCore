#include <ZC/Video/OpenGL/Renderer/ZC_Render.h>

#include <ZC/Video/OpenGL/ZC_OpenGL.h>
#include <ZC/Video/OpenGL/Renderer/ZC_Renderer.h>
#include <ZC/Tools/Container/ZC_ContFunc.h>
#include <ZC/ErrorLogger/ZC_ErrorLogger.h>

#include <cassert>

ZC_Render::ZC_Render(ZC_Render&& r)
    : frameBuffer(r.frameBuffer),
    drawState(r.drawState),
    fbo(std::move(r.fbo)),
    renderLevelDrawers(std::move(r.renderLevelDrawers))
{}

ZC_Render::~ZC_Render()
{
    if (drawState != DS_None) ZC_Renderer::Erase(this);
}

bool ZC_Render::operator < (ZC_FrameBuffer rl) const noexcept
{
    return frameBuffer < rl;
}

bool ZC_Render::operator == (ZC_FrameBuffer rl) const noexcept
{
    return frameBuffer == rl;
}

void ZC_Render::Add(ZC_RSController* pRSController, ZC_DrawLevel drawLevel)
{
    auto rendLvlDrawersIter = renderLevelDrawers.find(drawLevel);
    if (rendLvlDrawersIter == renderLevelDrawers.end()) //  add new level in renderLevelDrawers and save iterator on it
        rendLvlDrawersIter = renderLevelDrawers.emplace(drawLevel, ZC_RenderLevelDrawer::GetRendererLevelDrawer(drawLevel)).first;

    rendLvlDrawersIter->second->VAdd(pRSController);
}
    
void ZC_Render::Erase(ZC_RSController* pRSController, ZC_DrawLevel drawLevel)
{
    auto rendLvlDrawersIter = renderLevelDrawers.find(drawLevel);
    assert(rendLvlDrawersIter != renderLevelDrawers.end());   //  can't find Lvl
    //  erase from renderer level, if returns true, renderer level empty and must be erased from renderLevelDrawers.
    if (rendLvlDrawersIter->second->VErase(pRSController)) renderLevelDrawers.erase(rendLvlDrawersIter);
}

void ZC_Render::SetDrawState(DrawState _drawState)
{
    if (drawState == _drawState) return;
    if (_drawState == DS_None) ZC_Renderer::Erase(this);
    else if (drawState == DS_None) ZC_Renderer::Add(this);
    drawState = _drawState;
}

bool ZC_Render::Draw()
{
    if (!renderLevelDrawers.empty())
    {
        this->UpdateUBO();
        fbo.Use();
        for (auto& upRendererLevelDrawerPair : renderLevelDrawers) upRendererLevelDrawerPair.second->VDraw(fbo.GetBuffersController());
        fbo.Finish();
    }
    
    if (drawState == DS_OneFrame) drawState = DS_None;
    return drawState == DS_Loop;
}

const ZC_Texture* ZC_Render::GetColorTexture() const noexcept
{
    return fbo.GetColorTexture();
}

const ZC_Texture* ZC_Render::GetDepthTexture() const noexcept
{
    return fbo.GetDepthTexture();
}

ZC_Render::ZC_Render(ZC_FrameBuffer _frameBuffer, ZC_FBO&& _fbo)
    : frameBuffer(_frameBuffer),
    fbo(std::move(_fbo))
{}

void ZC_Render::SetClearColor(float red, float green, float blue, float alpha)
{
    fbo.SetClearColor(red, green, blue, alpha);
}



// //     int SCR_WIDTH = 1850, SCR_HEIGHT = 1011;
//     int width = 800, height = 600;

//     int samples = 4;

//     unsigned int framebufferMultisampled;
//     unsigned int rboMultisampledColor;
//     unsigned int rboMultisampledDepth;
//     unsigned int rboMultisampledStencil;
    
//     uint tempFrameBuffer;
//     unsigned int texture;

//     uint vboCube;

// void ZC_Renderer::Draw()
// {
//     static bool inited = false;
//     if (!inited)
//     {
//         GLenum result;
//         glGenFramebuffers(1, &framebufferMultisampled);
//         glBindFramebuffer(GL_FRAMEBUFFER, framebufferMultisampled);

//         // // create a multisampled color attachment texture
//         // glGenTextures(1, &textureColorBufferMultiSampled);
//         // glBindTexture(GL_TEXTURE_2D_MULTISAMPLE, textureColorBufferMultiSampled);
//         // glTexImage2DMultisample(GL_TEXTURE_2D_MULTISAMPLE, samples, GL_RGBA, SCR_WIDTH, SCR_HEIGHT, GL_TRUE);
//         // glBindTexture(GL_TEXTURE_2D_MULTISAMPLE, 0);
//         // glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D_MULTISAMPLE, textureColorBufferMultiSampled, 0);
        


//         // uint texColor;
//         // glGenTextures(1, &texColor);
//         // glBindTexture(GL_TEXTURE_2D, texColor);
//         // glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, nullptr);
//         // glBindTexture(GL_TEXTURE_2D, texColor);
//         // glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, texColor, 0);
        
//         // uint texDepth;
//         // glGenTextures(1, &texDepth);
//         // glBindTexture(GL_TEXTURE_2D, texDepth);
//         // glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT, width, height, 0, GL_DEPTH_COMPONENT, GL_FLOAT, nullptr);
//         // glBindTexture(GL_TEXTURE_2D, texDepth);
//         // glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, texDepth, 0);

//         // uint rboStencil;
//         // glGenRenderbuffers(1, &rboStencil);
//         // glBindRenderbuffer(GL_RENDERBUFFER, rboStencil);
//         // glRenderbufferStorage(GL_RENDERBUFFER, GL_STENCIL_INDEX8, width, height);
//         // glFramebufferRenderbuffer(GL_FRAMEBUFFER,  GL_STENCIL_ATTACHMENT, GL_RENDERBUFFER, rboStencil);

//         // auto result = glCheckFramebufferStatus(GL_FRAMEBUFFER);
//         // if (result != GL_FRAMEBUFFER_COMPLETE)
//         // {
//         //     int a = 3;
//         // }


//         // create a (multisampled) renderbuffer object for depth and stencil attachments
//         glGenRenderbuffers(1, &rboMultisampledColor);
//         glBindRenderbuffer(GL_RENDERBUFFER, rboMultisampledColor);
//         glRenderbufferStorageMultisample(GL_RENDERBUFFER, samples, GL_RGBA8, width, height);
//         glBindRenderbuffer(GL_RENDERBUFFER, 0);
//         glFramebufferRenderbuffer(GL_FRAMEBUFFER,  GL_COLOR_ATTACHMENT0, GL_RENDERBUFFER, rboMultisampledColor);

//         // create a (multisampled) renderbuffer object for depth and stencil attachments
//         glGenRenderbuffers(1, &rboMultisampledDepth);
//         glBindRenderbuffer(GL_RENDERBUFFER, rboMultisampledDepth);
//         glRenderbufferStorageMultisample(GL_RENDERBUFFER, samples, GL_DEPTH24_STENCIL8, width, height);
//         glBindRenderbuffer(GL_RENDERBUFFER, 0);
//         glFramebufferRenderbuffer(GL_FRAMEBUFFER,  GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, rboMultisampledDepth);

//         // // create a (multisampled) renderbuffer object for depth and stencil attachments
//         // glGenRenderbuffers(1, &rboMultisampledStencil);
//         // glBindRenderbuffer(GL_RENDERBUFFER, rboMultisampledStencil);
//         // glRenderbufferStorageMultisample(GL_RENDERBUFFER, samples, GL_STENCIL_INDEX8, SCR_WIDTH, SCR_HEIGHT);
//         // glBindRenderbuffer(GL_RENDERBUFFER, 0);
//         // glFramebufferRenderbuffer(GL_FRAMEBUFFER,  GL_STENCIL_ATTACHMENT, GL_RENDERBUFFER, rboMultisampledStencil);
        
//         // glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, 0); //  use default rbo

//         result = glCheckFramebufferStatus(GL_FRAMEBUFFER);
//         if (result != GL_FRAMEBUFFER_COMPLETE)
//         {
//             int a = 3;
//         }

//         uint fbo,
//             rboC,
//             rboDS;
//         glGenFramebuffers(1, &fbo);
//         glBindFramebuffer(GL_FRAMEBUFFER, fbo);

//         // // create a multisampled color attachment texture
//         // glGenTextures(1, &textureColorBufferMultiSampled);
//         // glBindTexture(GL_TEXTURE_2D_MULTISAMPLE, textureColorBufferMultiSampled);
//         // glTexImage2DMultisample(GL_TEXTURE_2D_MULTISAMPLE, samples, GL_RGBA, SCR_WIDTH, SCR_HEIGHT, GL_TRUE);
//         // glBindTexture(GL_TEXTURE_2D_MULTISAMPLE, 0);
//         // glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D_MULTISAMPLE, textureColorBufferMultiSampled, 0);
        
//         // create a (multisampled) renderbuffer object for depth and stencil attachments
//         glGenRenderbuffers(1, &rboC);
//         glBindRenderbuffer(GL_RENDERBUFFER, rboC);
//         glRenderbufferStorageMultisample(GL_RENDERBUFFER, 2, GL_RGBA8, width, height);
//         glBindRenderbuffer(GL_RENDERBUFFER, 0);
//         glFramebufferRenderbuffer(GL_FRAMEBUFFER,  GL_COLOR_ATTACHMENT0, GL_RENDERBUFFER, rboC);

//         // create a (multisampled) renderbuffer object for depth and stencil attachments
//         glGenRenderbuffers(1, &rboDS);
//         glBindRenderbuffer(GL_RENDERBUFFER, rboDS);
//         glRenderbufferStorageMultisample(GL_RENDERBUFFER, 2, GL_DEPTH24_STENCIL8, width, height);
//         glBindRenderbuffer(GL_RENDERBUFFER, 0);
//         glFramebufferRenderbuffer(GL_FRAMEBUFFER,  GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, rboDS);

//         result = glCheckFramebufferStatus(GL_FRAMEBUFFER);
//         if (result != GL_FRAMEBUFFER_COMPLETE)
//         {
//             int a = 3;
//         }
//             glBindFramebuffer(GL_READ_FRAMEBUFFER, framebufferMultisampled);
//             glBindFramebuffer(GL_DRAW_FRAMEBUFFER, fbo);
//             glBlitFramebuffer(0, 0, width, height, 0, 0, width, height, GL_COLOR_BUFFER_BIT, GL_NEAREST);

//         // //  temp buffer(not multisampled)
//         // glGenFramebuffers(1, &tempFrameBuffer);
//         // glBindFramebuffer(GL_FRAMEBUFFER, tempFrameBuffer);

//         // // create color attachment texture
//         // glGenTextures(1, &texture);
//         // glBindTexture(GL_TEXTURE_2D, texture);
//         // glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, SCR_WIDTH, SCR_HEIGHT, 0, GL_RGBA, GL_UNSIGNED_BYTE, NULL);
//         // glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
//         // glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
//         // glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, texture, 0);	// we only need a color buffer

//         // result = glCheckFramebufferStatus(GL_FRAMEBUFFER);
//         // if (result != GL_FRAMEBUFFER_COMPLETE)
//         // {
//         //     int a = 3;
//         // }

//         inited = true;
//     }


//         glBindFramebuffer(GL_FRAMEBUFFER, framebufferMultisampled);

//     bufferCleaner.GlClear();
//     ZC_UBOs::Update();
//     for (auto& upRendererLevelDrawerPair : renderLevelDrawers) upRendererLevelDrawerPair.second->VDraw(bufferCleaner);
//     glBindVertexArray(0);
// #ifdef ZC_IMGUI
//     ZC_IGWindow::Draw();
// #endif
    
//     //  polnaya huynia )
//             // glBindFramebuffer(GL_READ_FRAMEBUFFER, 0);
//             // glBindFramebuffer(GL_DRAW_FRAMEBUFFER, framebufferMultisampled);
//             // glBlitFramebuffer(0, 0, SCR_WIDTH, SCR_HEIGHT, 0, 0, SCR_WIDTH, SCR_HEIGHT, GL_COLOR_BUFFER_BIT, GL_NEAREST);

//             glBindFramebuffer(GL_READ_FRAMEBUFFER, framebufferMultisampled);
//             glBindFramebuffer(GL_DRAW_FRAMEBUFFER, 0);
//             glBlitFramebuffer(0, 0, width, height, 0, 0, width, height, GL_COLOR_BUFFER_BIT, GL_NEAREST);

//     funcSwapBuffer();
// }