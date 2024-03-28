#include <ZC/Video/OpenGL/Renderer/ZC_RLDText.h>

#include <ZC/Video/OpenGL/ZC_OpenGL.h>

void ZC_RLDText::Add(ZC_RSController* pRSController)
{
    ZC_RLDData_Uniforms_GLDraw uniforms_glDraw{ static_cast<const ZC_Uniforms*>(pRSController->GetPersonalData(ZC_RSPDC_uniforms)), pRSController->pGLDraw };
    this->AddInMap(pRSController->pShProg, ZC_TexturesHolder{ pRSController->pTexture, pRSController->texturesCount }, pRSController->pVAO, uniforms_glDraw);
}

bool ZC_RLDText::Erase(ZC_RSController* pRSController)
{
    ZC_RLDData_Uniforms_GLDraw uniforms_glDraw{ static_cast<const ZC_Uniforms*>(pRSController->GetPersonalData(ZC_RSPDC_uniforms)), pRSController->pGLDraw };
    return this->EraseFromForwardList(pRSController->pShProg, ZC_TexturesHolder{ pRSController->pTexture, pRSController->texturesCount }, pRSController->pVAO, uniforms_glDraw);
}

void ZC_RLDText::Draw(ZC_RBufferCleaner& rBufferCleaner)
{
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    this->DrawRendererSets();   //  ZC_RLDContainerMap::DrawRendererSets();
    glDisable(GL_BLEND);
}


//  ZC_RLDTextWindowIntoScene

void ZC_RLDTextWindowIntoScene::Draw(ZC_RBufferCleaner& rBufferCleaner)
{
    rBufferCleaner.GlClear(GL_DEPTH_BUFFER_BIT);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    this->DrawRendererSets();   //  ZC_RLDContainerMap::DrawRendererSets();
    glDisable(GL_BLEND);
}