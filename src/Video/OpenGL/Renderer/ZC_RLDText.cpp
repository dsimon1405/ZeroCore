#include <ZC/Video/OpenGL/Renderer/ZC_RLDText.h>

#include <ZC/Video/OpenGL/ZC_OpenGL.h>

void ZC_RLDText::Add(ZC_RSController* pRSController)
{
    fl.Add(pRSController->pShProg, { pRSController->pTexture, pRSController->texturesCount }, pRSController->pVAO,
        { static_cast<const ZC_Uniforms*>(pRSController->GetPersonalData(ZC_RSPDC_uniforms)), pRSController->pGLDraw });
}

bool ZC_RLDText::Erase(ZC_RSController* pRSController)
{
    return fl.Erase(pRSController->pShProg, { pRSController->pTexture, pRSController->texturesCount }, pRSController->pVAO,
        { static_cast<const ZC_Uniforms*>(pRSController->GetPersonalData(ZC_RSPDC_uniforms)), pRSController->pGLDraw });
}

void ZC_RLDText::Draw(ZC_RBufferCleaner& rBufferCleaner)
{
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    fl.Draw();
    glDisable(GL_BLEND);
}


//  ZC_RLDTextWindowIntoScene

void ZC_RLDTextWindowIntoScene::Draw(ZC_RBufferCleaner& rBufferCleaner)
{
    rBufferCleaner.GlClear(GL_DEPTH_BUFFER_BIT);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    fl.Draw();
    glDisable(GL_BLEND);
}