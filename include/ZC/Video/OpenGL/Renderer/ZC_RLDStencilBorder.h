#pragma once

#include "ZC_RLDForwardLists.h"

struct ZC_RLDData_Uniforms_GLDraw_StencilBorder
{
    const ZC_Uniforms* pUniforms;
    const ZC_GLDraw* pGLDraw;
    const ZC_RSPDStencilBorderData* pStencilBorderData;
    
    bool operator == (const ZC_RLDData_Uniforms_GLDraw_StencilBorder& unifAndGLDraw) const noexcept;
    void Draw() const;
};

struct ZC_RLDStencilBorder : public ZC_RLDForwardLists<const ZC_ShProg*, const ZC_VAO*, ZC_TexturesHolder, ZC_RLDData_Uniforms_GLDraw_StencilBorder>
{
    ZC_RLDStencilBorder();

    void Add(ZC_RSController* pRSController) override;
    bool Erase(ZC_RSController* pRSController) override;
    void Draw(ZC_RBufferCleaner& rBufferCleaner) override;

private:
    static inline ZC_ShProg* pShPStencilBorder;
    static inline ZC_Uniforms* pUniformsStencilBorder;
};