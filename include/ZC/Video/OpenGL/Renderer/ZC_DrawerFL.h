#pragma once

#include "ZC_Drawer.h"
#include "ZC_DrawerForwardList.h"
#include <ZC/Video/OpenGL/ZC_FBOBuffersController.h>
#include <ZC/Video/OpenGL/ZC_GLBlend.h>
#include <ZC/Video/OpenGL/ZC_GLCullFace.h>
#include <ZC/Video/OpenGL/ZC_GLDepth.h>
#include <ZC/Video/OpenGL/ZC_GLStencil.h>

template<typename... T>
class ZC_DrawerFL : public ZC_Drawer
{
public:
    ZC_DrawerFL(uint clearMask, ZC_GLDepth _depth, ZC_GLStencil _stencil, ZC_GLBlend _blend, ZC_GLCullFace _cull_face);

    void VAdd(ZC_DSController* pRSController) override;
    bool VErase(ZC_DSController* pRSController) override;
    void VDraw() override;

    virtual void VCallDraw();

protected:
    ZC_DrawerForwardList<T...> fl;

private:
    GLbitfield clearMask;
    ZC_GLDepth depth;
    ZC_GLStencil stencil;
    ZC_GLBlend blend;
    ZC_GLCullFace cull_face;
};


template<typename... T>
ZC_DrawerFL<T...>::ZC_DrawerFL(GLbitfield _clearMask, ZC_GLDepth _depth, ZC_GLStencil _stencil, ZC_GLBlend _blend, ZC_GLCullFace _cull_face)
    : clearMask(_clearMask),
    depth(_depth),
    stencil(_stencil),
    blend(_blend),
    cull_face(_cull_face)
{
    assert((clearMask & (~(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT))) == 0u);
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
    depth.Use();
    stencil.Use();
    blend.Use();
    cull_face.Use();
    ZC_FBOBuffersController::GlClear(clearMask);    //  make clear after changing buffer's state, remember GUI glDepthMask(GL_FALSE);
    VCallDraw();
}

template<typename... T>
void ZC_DrawerFL<T...>::VCallDraw()
{
    fl.Draw();
}