#pragma once

#include "ZC_RenderLevelDrawer.h"
#include "ZC_RLDForwardList.h"

struct ZC_RLDData_Uniforms_GLDraw_StencilBorder
{
    const ZC_Uniforms* pUniforms;
    const ZC_GLDraw* pGLDraw;
    const ZC_RSPDStencilBorderData* pStencilBorderData;
    
    bool operator == (const ZC_RLDData_Uniforms_GLDraw_StencilBorder& unifAndGLDraw) const noexcept;
    void Draw() const;
};

struct ZC_RLDStencilBorder : public ZC_RenderLevelDrawer
{
    ZC_RLDStencilBorder();

    void VAdd(ZC_RSController* pRSController) override;
    bool VErase(ZC_RSController* pRSController) override;
    void VDraw(ZC_FBOBuffersController& rBuffersController) override;

private:
    static inline ZC_ShProg* pShPStencilBorder;
    static inline ZC_Uniforms* pUniformsStencilBorder;

    ZC_RLDForwardList<const ZC_ShProg*, const ZC_VAO*, ZC_TexturesHolder, ZC_RLDData_Uniforms_GLDraw_StencilBorder> fl;
};