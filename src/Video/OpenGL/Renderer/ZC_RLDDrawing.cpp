#include <ZC/Video/OpenGL/Renderer/ZC_RLDDrawing.h>

#include <ZC/Video/OpenGL/ZC_FBOBuffersController.h>

void ZC_RLDDrawing::VAdd(ZC_RSController* pRSController)
{
    fl.Add(pRSController->pShProg, pRSController->pVAO, pRSController->texturesHolder,
        { static_cast<const ZC_Uniforms*>(pRSController->GetPersonalData(ZC_RSPDC_uniforms)), pRSController->pGLDraw });
}

bool ZC_RLDDrawing::VErase(ZC_RSController* pRSController)
{
    return fl.Erase(pRSController->pShProg, pRSController->pVAO, pRSController->texturesHolder,
        { static_cast<const ZC_Uniforms*>(pRSController->GetPersonalData(ZC_RSPDC_uniforms)), pRSController->pGLDraw });
}

void ZC_RLDDrawing::VDraw()
{
    ZC_FBOBuffersController::GlEnable(GL_DEPTH_TEST);
    fl.Draw();
}










// void ZC_RLDDrawing::VAdd(ZC_RSController* pRSController)
// {
//     fl.Add(pRSController->pShProg, pRSController->pVAO, pRSController->texturesHolder,
//         { static_cast<const ZC_Uniforms*>(pRSController->GetPersonalData(ZC_RSPDC_uniforms)), pRSController->pGLDraw });
// }

// bool ZC_RLDDrawing::VErase(ZC_RSController* pRSController)
// {
//     return fl.Erase(pRSController->pShProg, pRSController->pVAO, pRSController->texturesHolder,
//         { static_cast<const ZC_Uniforms*>(pRSController->GetPersonalData(ZC_RSPDC_uniforms)), pRSController->pGLDraw });
// }

// void ZC_RLDDrawing::VDraw()
// {
//     ZC_FBOBuffersController::GlEnable(GL_DEPTH_TEST);
//     fl.Draw();
// }