#pragma once

#include "ZC_RenderLevelDrawer.h"
#include "ZC_RLDForwardList.h"

struct ZC_RLDDrawing : public ZC_RenderLevelDrawer
{
    ZC_RLDForwardList<const ZC_ShProg*, const ZC_VAO*, ZC_TexturesHolder, ZC_RLDData_Uniforms_GLDraw> fl;

    void VAdd(ZC_RSController* pRSController) override;
    bool VErase(ZC_RSController* pRSController) override;
    void VDraw() override;
};