#pragma once

#include "ZC_Drawer.h"
#include "ZC_DrawerForwardList.h"
#include <ZC/Video/OpenGL/ZC_FBOBuffersController.h>
#include <ZC/Video/OpenGL/ZC_GLBlend.h>

template<typename... T>
class ZC_DrawerFL : public ZC_Drawer
{
public:
    ZC_DrawerFL(uint clearMask, bool depthTest, bool stencilTest, ZC_GLBlend blend);

    void VAdd(ZC_DSController* pRSController) override;
    bool VErase(ZC_DSController* pRSController) override;
    void VDraw() override;

    virtual void VCallDraw();

protected:
    ZC_DrawerForwardList<T...> fl;

private:
    uint clearMask;
    bool depthTest,
        stencilTest;
    ZC_GLBlend blend;
};


template<typename... T>
ZC_DrawerFL<T...>::ZC_DrawerFL(uint _clearMask, bool _depthTest, bool _stencilTest, ZC_GLBlend _blend)
    : clearMask(_clearMask),
    depthTest(_depthTest),
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
    depthTest ? ZC_FBOBuffersController::GlEnable(GL_DEPTH_TEST) : ZC_FBOBuffersController::GlDisable(GL_DEPTH_TEST);
    stencilTest ? ZC_FBOBuffersController::GlEnable(GL_STENCIL_TEST) : ZC_FBOBuffersController::GlDisable(GL_STENCIL_TEST);
    blend.Use();
    VCallDraw();
}

template<typename... T>
void ZC_DrawerFL<T...>::VCallDraw()
{
    fl.Draw();
}