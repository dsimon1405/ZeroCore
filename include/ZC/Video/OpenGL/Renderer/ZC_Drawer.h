#pragma once

#include <ZC/Tools/Function/ZC_Function.h>
#include "ZC_DSController.h"

struct ZC_Drawer
{
    virtual ~ZC_Drawer() = default;

    static ZC_uptr<ZC_Drawer> GetRendererLevelDrawer(ZC_DrawerLevel lvl);
    static void SetUsersFunction_GetRendererLevelDrawer(ZC_Function<ZC_uptr<ZC_Drawer>(ZC_DrawerLevel)>&& func);

    virtual void VAdd(ZC_DSController* pRSController) = 0;
    //  return true if empty, otherwise false
    virtual bool VErase(ZC_DSController* pRSController) = 0;
    virtual void VDraw() = 0;

private:
    static inline ZC_Function<ZC_uptr<ZC_Drawer>(ZC_DrawerLevel)> funcUsers_GetDrawer;
};