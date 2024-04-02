#include <ZC/Video/OpenGL/Renderer/ZC_RLDDrawing.h>

void ZC_RLDDrawing::Add(ZC_RSController* pRSController)
{
    fl.Add(pRSController->pShProg, pRSController->pVAO, ZC_TexturesHolder{ pRSController->pTexture, pRSController->texturesCount },
        { static_cast<const ZC_Uniforms*>(pRSController->GetPersonalData(ZC_RSPDC_uniforms)), pRSController->pGLDraw });
}

bool ZC_RLDDrawing::Erase(ZC_RSController* pRSController)
{
    return fl.Erase(pRSController->pShProg, pRSController->pVAO, ZC_TexturesHolder{ pRSController->pTexture, pRSController->texturesCount },
        { static_cast<const ZC_Uniforms*>(pRSController->GetPersonalData(ZC_RSPDC_uniforms)), pRSController->pGLDraw });
}

void ZC_RLDDrawing::Draw(ZC_RBufferCleaner& rBufferCleaner)
{
    fl.Draw();
}