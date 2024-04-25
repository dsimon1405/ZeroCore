#pragma once

#include "ZC_RenderLevelDrawer.h"
#include "ZC_RLDForwardList.h"

struct ZC_RLDStencilBorder : public ZC_RenderLevelDrawer
{
    ZC_RLDStencilBorder();

    void VAdd(ZC_RSController* pRSController) override;
    bool VErase(ZC_RSController* pRSController) override;
    void VDraw() override;

private:
    static inline ZC_ShProg* pShPStencilBorder;
    static inline ZC_Uniforms* pUniformsStencilBorder;

    ZC_RLDForwardList<const ZC_ShProg*, const ZC_VAO*, ZC_TexturesHolder, ZC_RLDData_Uniforms_GLDraw_StencilBorder> fl;
};