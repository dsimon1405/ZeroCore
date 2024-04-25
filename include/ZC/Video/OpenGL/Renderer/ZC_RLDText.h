#pragma once

#include "ZC_RenderLevelDrawer.h"
#include "ZC_RLDForwardList.h"

struct ZC_RLDOrthoBlend : public ZC_RenderLevelDrawer
{
    ZC_RLDForwardList<const ZC_ShProg*, ZC_TexturesHolder, const ZC_VAO*, ZC_RLDData_Uniforms_GLDraw> fl;

    void VAdd(ZC_RSController* pRSController) override;
    bool VErase(ZC_RSController* pRSController) override;
    void VDraw() override;

protected:
    void Draw();
};

struct ZC_RLDTextScene : public ZC_RLDOrthoBlend
{
    void VDraw() override;
};

struct ZC_RLDTextWindowIntoScene : public ZC_RLDOrthoBlend
{
    void VDraw() override;
};