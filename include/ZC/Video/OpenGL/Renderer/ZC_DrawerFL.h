#pragma once

#include "ZC_Drawer.h"
#include "ZC_DrawerForwardList.h"
#include <ZC/Video/OpenGL/ZC_FBOBuffersController.h>
#include <ZC/Video/OpenGL/ZC_GLBlend.h>

template<typename... T>
class ZC_DrawerFL : public ZC_Drawer
{
public:
    //  - _depthMask - depth test do, but don't rewrite results.
    ZC_DrawerFL(uint clearMask, bool depthTest, bool _depthMask, bool stencilTest, ZC_GLBlend blend);

    void VAdd(ZC_DSController* pRSController) override;
    bool VErase(ZC_DSController* pRSController) override;
    void VDraw() override;

    virtual void VCallDraw();

protected:
    ZC_DrawerForwardList<T...> fl;

private:
    GLbitfield clearMask;
    bool depthTest;
    bool depthMask;
    bool stencilTest;
    ZC_GLBlend blend;
};


template<typename... T>
ZC_DrawerFL<T...>::ZC_DrawerFL(GLbitfield _clearMask, bool _depthTest, bool _depthMask, bool _stencilTest, ZC_GLBlend _blend)
    : clearMask(_clearMask),
    depthTest(_depthTest),
    depthMask(_depthMask),
    stencilTest(_stencilTest),
    blend(_blend)
{
    assert((clearMask & (~(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT))) == 0);
}

template<typename... T>
void ZC_DrawerFL<T...>::VAdd(ZC_DSController* pRSController)
{
    fl.Add(pRSController->GetByType<T>()...);
}

template<typename... T>
bool ZC_DrawerFL<T...>::VErase(ZC_DSController* pRSController)
{
    return fl.Erase(pRSController->GetByType<T>()...);
}

template<typename... T>
void ZC_DrawerFL<T...>::VDraw()
{
    ZC_FBOBuffersController::GlClear(clearMask);
    if (depthTest)
    {
        ZC_FBOBuffersController::GlEnable(GL_DEPTH_TEST);
        // ZC_FBOBuffersController::GLDepthMask(depthMask);
    }
    else ZC_FBOBuffersController::GlDisable(GL_DEPTH_TEST);
    ZC_FBOBuffersController::GLDepthMask(depthMask);

    stencilTest ? ZC_FBOBuffersController::GlEnable(GL_STENCIL_TEST) : ZC_FBOBuffersController::GlDisable(GL_STENCIL_TEST);
    blend.Use();
    VCallDraw();
}

template<typename... T>
void ZC_DrawerFL<T...>::VCallDraw()
{
    fl.Draw();
}