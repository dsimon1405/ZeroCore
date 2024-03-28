#pragma once

#include "ZC_RLDForwardLists.h"

struct ZC_RLDDrawing : public ZC_RLDForwardLists<const ZC_ShProg*, const ZC_VAO*, ZC_TexturesHolder, ZC_RLDData_Uniforms_GLDraw>
{
    void Add(ZC_RSController* pRSController) override;
    bool Erase(ZC_RSController* pRSController) override;
    void Draw(ZC_RBufferCleaner& rBufferCleaner) override;
};