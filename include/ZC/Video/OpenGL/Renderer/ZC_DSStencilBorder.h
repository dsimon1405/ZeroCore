#pragma once

#include "ZC_DrawerFL.h"

struct ZC_DSStencilBorder : public ZC_DrawerFL<const ZC_ShProg*, const ZC_VAO*, ZC_TexturesHolder, ZC_RLDData_Uniforms_GLDraw_StencilBorder>
{
    ZC_DSStencilBorder();

    void VCallDraw() override;
};