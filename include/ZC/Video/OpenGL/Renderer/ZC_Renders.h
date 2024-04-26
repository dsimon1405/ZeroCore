#pragma once

#include "ZC_Render.h"

class ZC_Renders
{
public:
    static ZC_Render* CreateRender(ZC_RenderLevel renderLevel, typename ZC_Render::DrawState drawState, ZC_FBO&& fbo);
    static void EraseRender(ZC_RenderLevel renderLevel);
    static ZC_Render* GetRender(ZC_RenderLevel renderLevel);

private:
    static inline std::forward_list<ZC_Render> renders;
};