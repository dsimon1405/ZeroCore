#include <ZC/Video/OpenGL/Renderer/ZC_RLDDrawing.h>

void ZC_RLDDrawing::Add(ZC_RSController* pRSController)
{
    ZC_RLDData_Uniforms_GLDraw uniforms_glDraw{ static_cast<const ZC_Uniforms*>(pRSController->GetPersonalData(ZC_RSPDC_uniforms)), pRSController->pGLDraw };
    this->AddInMap(pRSController->pShProg, pRSController->pVAO, ZC_TexturesHolder{ pRSController->pTexture, pRSController->texturesCount }, uniforms_glDraw);
}

bool ZC_RLDDrawing::Erase(ZC_RSController* pRSController)
{
    ZC_RLDData_Uniforms_GLDraw uniforms_glDraw{ static_cast<const ZC_Uniforms*>(pRSController->GetPersonalData(ZC_RSPDC_uniforms)), pRSController->pGLDraw };
    return this->EraseFromForwardList(pRSController->pShProg, pRSController->pVAO, ZC_TexturesHolder{ pRSController->pTexture, pRSController->texturesCount }, uniforms_glDraw);
}

void ZC_RLDDrawing::Draw(ZC_RBufferCleaner& rBufferCleaner)
{
    this->DrawRendererSets();   //  ZC_RLDContainerMap::DrawRendererSets();
}