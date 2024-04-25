#pragma once

#include <ZC/Tools/Function/ZC_Function.h>
#include "ZC_RSController.h"

struct ZC_RenderLevelDrawer
{
    virtual ~ZC_RenderLevelDrawer() = default;

    static ZC_uptr<ZC_RenderLevelDrawer> GetRendererLevelDrawer(ZC_DrawLevel lvl);
    static void SetUsersFunction_GetRendererLevelDrawer(ZC_Function<ZC_uptr<ZC_RenderLevelDrawer>(ZC_DrawLevel)>&& func);

    virtual void VAdd(ZC_RSController* pRSController) = 0;
    //  return true if empty, otherwise false
    virtual bool VErase(ZC_RSController* pRSController) = 0;
    virtual void VDraw() = 0;

private:
    static inline ZC_Function<ZC_uptr<ZC_RenderLevelDrawer>(ZC_DrawLevel)> funcUsers_GetRendererLevelDrawer;
};