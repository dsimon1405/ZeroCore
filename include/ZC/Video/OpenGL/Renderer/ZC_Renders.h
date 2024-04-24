#pragma once

#include "ZC_Render.h"

class ZC_Renders
{
public:
    static ZC_Render* CreateRender(ZC_FrameBuffer frameBuffer, typename ZC_Render::DrawState drawState, ZC_FBO&& fbo);
    static void EraseRender(ZC_FrameBuffer frameBuffer);
    static ZC_Render* GetRender(ZC_FrameBuffer frameBuffer);

private:
    static inline std::forward_list<ZC_Render> renders;
};