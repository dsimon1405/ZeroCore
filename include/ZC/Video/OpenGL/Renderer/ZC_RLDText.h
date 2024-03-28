#pragma once

#include "ZC_RLDForwardLists.h"

struct ZC_RLDText : public ZC_RLDForwardLists<const ZC_ShProg*, ZC_TexturesHolder, const ZC_VAO*, ZC_RLDData_Uniforms_GLDraw>
{
    void Add(ZC_RSController* pRSController) override;
    bool Erase(ZC_RSController* pRSController) override;
    void Draw(ZC_RBufferCleaner& rBufferCleaner) override;
};

struct ZC_RLDTextWindowIntoScene : public ZC_RLDText
{
    void Draw(ZC_RBufferCleaner& rBufferCleaner) override;
};