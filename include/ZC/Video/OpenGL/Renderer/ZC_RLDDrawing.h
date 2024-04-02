#pragma once

#include "ZC_RendererLevelDrawer.h"
#include "ZC_RLDForwardList.h"

struct ZC_RLDDrawing : public ZC_RendererLevelDrawer
{
    ZC_RLDForwardList<const ZC_ShProg*, const ZC_VAO*, ZC_TexturesHolder, ZC_RLDData_Uniforms_GLDraw> fl;

    void Add(ZC_RSController* pRSController) override;
    bool Erase(ZC_RSController* pRSController) override;
    void Draw(ZC_RBufferCleaner& rBufferCleaner) override;
};