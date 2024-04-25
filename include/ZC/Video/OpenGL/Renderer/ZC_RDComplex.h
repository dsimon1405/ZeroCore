#pragma once

#include "ZC_RenderLevelDrawer.h"
#include "ZC_RLDForwardList.h"
#include <ZC/Video/OpenGL/ZC_FBOBuffersController.h>

typedef int ZC_OpenGLParams;
enum ZC_OpenGLParam
{
    ZC_OGLP_None                                            = 0,        //  nothing to do
    ZC_OGLP_GL_COLOR_BUFFER_BIT                             = 1,        //  clear color buffer
    ZC_OGLP_GL_DEPTH_BUFFER_BIT                             = 1 << 1,   //  clear depth buffer
    ZC_OGLP_GL_STENCIL_BUFFER_BIT                           = 1 << 2,   //  clear stencil buffer
    ZC_OGLP_GL_DEPTH_TEST                                   = 1 << 3,   //  enable depth test
    ZC_OGLP_GL_STENCIL_TEST                                 = 1 << 4,   //  enable stencil test
    ZC_OGLP_GL_BLEND__SRC_ALPHA__ONE_MINUS_SRC_SLPHA        = 1 << 5,
};

template<typename... T>
struct ZC_RDComplex : public ZC_RenderLevelDrawer
{
    ZC_RDComplex(ZC_OpenGLParams _openGLParams);

    void VAdd(ZC_RSController* pRSController) override;
    bool VErase(ZC_RSController* pRSController) override;
    void VDraw() override;

    virtual void VCallDraw();

    void ActivateOpenGL();
    void DeactivateOpenGL();

    ZC_RLDForwardList<T...> fl;
    ZC_OpenGLParams openGLParams;
};


template<typename... T>
ZC_RDComplex<T...>::ZC_RDComplex(ZC_OpenGLParams _openGLParams)
    : openGLParams(_openGLParams)
{}

template<typename... T>
void ZC_RDComplex<T...>::VAdd(ZC_RSController* pRSController)
{
    fl.Add(pRSController->GetByType<T>()...);
}

template<typename... T>
bool ZC_RDComplex<T...>::VErase(ZC_RSController* pRSController)
{
    return fl.Erase(pRSController->GetByType<T>()...);
}

template<typename... T>
void ZC_RDComplex<T...>::VDraw()
{
    ActivateOpenGL();
    VCallDraw();
    DeactivateOpenGL();
}

template<typename... T>
void ZC_RDComplex<T...>::VCallDraw()
{
    fl.Draw();
}

template<typename... T>
void ZC_RDComplex<T...>::ActivateOpenGL()
{
    if (openGLParams == ZC_OGLP_None) return;
    uint clearMask = 0;
    if (openGLParams & ZC_OGLP_GL_COLOR_BUFFER_BIT) clearMask |= GL_COLOR_BUFFER_BIT;
    if (openGLParams & ZC_OGLP_GL_DEPTH_BUFFER_BIT) clearMask |= GL_DEPTH_BUFFER_BIT;
    if (openGLParams & ZC_OGLP_GL_STENCIL_BUFFER_BIT) clearMask |= GL_STENCIL_BUFFER_BIT;
    if (clearMask != 0) ZC_FBOBuffersController::GlClear(clearMask);
    //  depth testing must enable or disable on start of each ZC_RenderLevelDrawer::VDraw throught ZC_FBOBuffersController thats minimizing calls of glEnable/glDisable
    openGLParams & ZC_OGLP_GL_DEPTH_TEST ? ZC_FBOBuffersController::GlEnable(GL_DEPTH_TEST) : ZC_FBOBuffersController::GlDisable(GL_DEPTH_TEST);
    //  if uses, heir must override VCallDraw() and configure stencil testing in that method. In each case stencil testing must be disable in the end of VDraw()
    //  to avoid troubles in next object's ZC_RenderLevelDrawer() calls (usual stencil disables in DeactivateOpenGL())
    if (openGLParams & ZC_OGLP_GL_STENCIL_TEST) ZC_FBOBuffersController::GlEnable(GL_STENCIL_TEST);
    if (openGLParams & ZC_OGLP_GL_BLEND__SRC_ALPHA__ONE_MINUS_SRC_SLPHA)
    {
        glEnable
    }
}

template<typename... T>
void ZC_RDComplex<T...>::DeactivateOpenGL()
{
    if (openGLParams == ZC_OGLP_None) return;
    if (openGLParams & ZC_OGLP_GL_STENCIL_TEST) ZC_FBOBuffersController::GlDisable(GL_STENCIL_TEST);
}
