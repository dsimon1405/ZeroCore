#include <ZC/Video/OpenGL/Renderer/ZC_RLDStencilBorder.h>

#include <ZC/Video/OpenGL/Shader/ZC_ShProgs.h>
#include <Video/OpenGL/ZC_OpenGL.h>

#include <cassert>

//  ZC_Uniforms_GLDraw_StencilBorder

bool ZC_RLDData_Uniforms_GLDraw_StencilBorder::operator == (const ZC_RLDData_Uniforms_GLDraw_StencilBorder& unifAndGLDraw) const noexcept
{
    return pUniforms == unifAndGLDraw.pUniforms && pGLDraw == unifAndGLDraw.pGLDraw;
}
void ZC_RLDData_Uniforms_GLDraw_StencilBorder::Draw() const
{
    if (pUniforms) pUniforms->Activate();
    pGLDraw->Draw();
}


//  ZC_RLDStencilBorder

ZC_RLDStencilBorder::ZC_RLDStencilBorder()
{
    if (!pShPStencilBorder)
    {
        auto pShProg = ZC_ShProgs::Get(ZC_ShProgs::Name::ZCR_Stencil);
        assert (pShProg);      //  stencil shader data wasn't loaded
        pShPStencilBorder = &(pShProg->shProg);
        pUniformsStencilBorder = &(pShProg->uniforms);
    }
}

void ZC_RLDStencilBorder::Add(ZC_RSController* pRSController)
{
    ZC_RLDData_Uniforms_GLDraw_StencilBorder uniforms_glDraw_stencilBorder{ static_cast<const ZC_Uniforms*>(pRSController->GetPersonalData(ZC_RSPDC_uniforms)),
        pRSController->pGLDraw, static_cast<const ZC_RSPDStencilBorderData*>(pRSController->GetPersonalData(ZC_RSPDC_stencilBorder)) };
    this->AddInMap(pRSController->pShProg, pRSController->pVAO, ZC_TexturesHolder{ pRSController->pTexture, pRSController->texturesCount }, uniforms_glDraw_stencilBorder);
}

bool ZC_RLDStencilBorder::Erase(ZC_RSController* pRSController)
{
    ZC_RLDData_Uniforms_GLDraw_StencilBorder uniforms_glDraw_stencilBorder{ static_cast<const ZC_Uniforms*>(pRSController->GetPersonalData(ZC_RSPDC_uniforms)),
        pRSController->pGLDraw, static_cast<const ZC_RSPDStencilBorderData*>(pRSController->GetPersonalData(ZC_RSPDC_stencilBorder)) };
    return this->EraseFromMap(pRSController->pShProg, pRSController->pVAO, ZC_TexturesHolder{ pRSController->pTexture, pRSController->texturesCount }, uniforms_glDraw_stencilBorder);
}

void ZC_RLDStencilBorder::Draw(ZC_RBufferCleaner& rBufferCleaner)
{
    rBufferCleaner.GlEnable(GL_STENCIL_TEST);

    glStencilFunc(GL_ALWAYS, 1, 255);
    glStencilOp(GL_REPLACE, GL_REPLACE, GL_REPLACE);
    glStencilMask(255);
    //  draw as usual
    this->DrawRendererSets();   //  ZC_RLDContainerMap::DrawRendererSets();

    glStencilFunc(GL_NOTEQUAL, 1, 255);
    rBufferCleaner.GlDisable(GL_DEPTH_TEST);
    pShPStencilBorder->ActivateOpenGL();    //  use stencil shader program
    for (auto& shProgAndVAOsPair : this->rendererSets)
    {
        for (auto& vaoAndTexHoldersPair : shProgAndVAOsPair.second)
        {
            vaoAndTexHoldersPair.first->ActivateOpenGL();
            for (auto& texHolderAndUniforms_GLDraw_StencilBorderPair : vaoAndTexHoldersPair.second)
            {
                for (auto& uniforms_glDraw_stencilBorder : texHolderAndUniforms_GLDraw_StencilBorderPair.second)
                {
                    //  set stencil uniforms data
                    auto model = *static_cast<const ZC_Mat4<float>*>(uniforms_glDraw_stencilBorder.pUniforms->Get(ZC_UN_unModel));
                    float scaleValue = uniforms_glDraw_stencilBorder.pStencilBorderData->scale;
                    model.Scale(scaleValue, scaleValue, scaleValue);
                    pUniformsStencilBorder->Set(ZC_UN_unModel, &model);
                    pUniformsStencilBorder->Set(ZC_UN_unColor, &(uniforms_glDraw_stencilBorder.pStencilBorderData->color));
                    pUniformsStencilBorder->Activate();     //  activate stencil uniforms and draw
                    uniforms_glDraw_stencilBorder.pGLDraw->Draw();
                }
            }
        }
    }
    rBufferCleaner.GlEnable(GL_DEPTH_TEST);
    
    rBufferCleaner.GlDisable(GL_STENCIL_TEST);
}