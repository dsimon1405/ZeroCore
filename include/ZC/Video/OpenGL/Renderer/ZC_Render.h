#pragma once

#include "ZC_Drawer.h"
#include <ZC/Video/OpenGL/ZC_FBO.h>
#include <ZC/Video/OpenGL/Buffer/ZC_UBOs.h>

#include <map>

class ZC_Renders;

class ZC_Render : public ZC_UBOs
{
    friend class ZC_Renders;
public:
    enum DrawState
    {
        DS_None,
        DS_Loop,
        DS_OneFrame     //  draw in fbo only in next frame (if don't need to update fbo's buffer every frame)
    };

    const ZC_RenderLevel renderLevel;

    ZC_Render(ZC_Render&& r);

    ~ZC_Render();

    bool operator < (ZC_RenderLevel rl) const noexcept;
    bool operator == (ZC_RenderLevel rl) const noexcept;

    void Add(ZC_DSController* pRSController, ZC_DrawerLevel drawerLevel);
    void Erase(ZC_DSController* pRSController, ZC_DrawerLevel drawerLevel);
    void SetDrawState(DrawState _drawState);
    //  Returns false if don't need render in next frame
    bool Draw();
    //  Returns pointer on color texture if ZC_Render.fbo multisample, with ZC_FBO::ResultTexture one of variants with Color, otherwise nullptr.
    const ZC_Texture* GetColorTexture() const noexcept;
    //  Returns pointer on depthStencil texture if ZC_Render.fbo multisample, with ZC_FBO::ResultTexture one of variants with Depth (Depth/Stencil), otherwise nullptr.
    const ZC_Texture* GetDepthTexture() const noexcept;
    void SetClearColor(float red, float green, float blue, float alpha);

private:
    DrawState drawState = DS_None;
    ZC_FBO fbo;
    std::map<ZC_DrawerLevel, ZC_uptr<ZC_Drawer>> renderLevelDrawers;

    ZC_Render(ZC_RenderLevel _renderLevel, ZC_FBO&& _fbo);
};