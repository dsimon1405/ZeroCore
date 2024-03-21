#pragma once

#include "ZC_RLDForwardLists.h"

struct ZC_RLDText : public ZC_RLDForwardLists<ZC_ShProg*, ZC_TexturesHolder, ZC_VAO*, ZC_RLDData_Uniforms_GLDraw>
{
    void Add(ZC_RSController* pRSController) override;
    bool Erase(ZC_RSController* pRSController) override;
    void Draw(ZC_RBufferCleaner& rBufferCleaner) override;
};