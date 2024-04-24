#include <ZC/Video/OpenGL/Renderer/ZC_RLDText.h>

#include <ZC/Video/OpenGL/ZC_OpenGL.h>

void ZC_RLDOrthoBlend::VAdd(ZC_RSController* pRSController)
{
    fl.Add(pRSController->pShProg, { pRSController->pTexture, pRSController->texturesCount }, pRSController->pVAO,
        { static_cast<const ZC_Uniforms*>(pRSController->GetPersonalData(ZC_RSPDC_uniforms)), pRSController->pGLDraw });
}

bool ZC_RLDOrthoBlend::VErase(ZC_RSController* pRSController)
{
    return fl.Erase(pRSController->pShProg, { pRSController->pTexture, pRSController->texturesCount }, pRSController->pVAO,
        { static_cast<const ZC_Uniforms*>(pRSController->GetPersonalData(ZC_RSPDC_uniforms)), pRSController->pGLDraw });
}

void ZC_RLDOrthoBlend::VDraw(ZC_FBOBuffersController& rBuffersController)
{
    Draw();
}

void ZC_RLDOrthoBlend::Draw()
{
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    fl.Draw();
    glDisable(GL_BLEND);
}


//  ZC_RLDTextScene

void ZC_RLDTextScene::VDraw(ZC_FBOBuffersController& rBuffersController)
{
    rBuffersController.GlEnable(GL_DEPTH_TEST);
    this->Draw();   //  ZC_RLDOrthoBlend::Draw()
    rBuffersController.GlDisable(GL_DEPTH_TEST);
}


//  ZC_RLDTextWindowIntoScene

void ZC_RLDTextWindowIntoScene::VDraw(ZC_FBOBuffersController& rBuffersController)
{
    rBuffersController.GlClear(GL_DEPTH_BUFFER_BIT);
    rBuffersController.GlEnable(GL_DEPTH_TEST);
    this->Draw();   //  ZC_RLDOrthoBlend::Draw()
    rBuffersController.GlDisable(GL_DEPTH_TEST);
}