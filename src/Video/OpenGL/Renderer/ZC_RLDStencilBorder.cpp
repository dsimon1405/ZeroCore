#include <ZC/Video/OpenGL/Renderer/ZC_RLDStencilBorder.h>

#include <ZC/Video/OpenGL/Shader/ZC_ShProgs.h>
#include <ZC/Video/OpenGL/ZC_OpenGL.h>
#include <ZC/Tools/Math/ZC_Mat4.h>

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
        auto pShProg = ZC_ShProgs::Get(ShPN_ZCR_StencilBorder);
        assert (pShProg);      //  stencil shader data wasn't loaded
        pShPStencilBorder = &(pShProg->shProg);
        pUniformsStencilBorder = &(pShProg->uniforms);
    }
}

void ZC_RLDStencilBorder::VAdd(ZC_RSController* pRSController)
{
    fl.Add(pRSController->pShProg, pRSController->pVAO, ZC_TexturesHolder{ pRSController->pTexture, pRSController->texturesCount },
        { static_cast<const ZC_Uniforms*>(pRSController->GetPersonalData(ZC_RSPDC_uniforms)),
            pRSController->pGLDraw, static_cast<const ZC_RSPDStencilBorderData*>(pRSController->GetPersonalData(ZC_RSPDC_stencilBorder)) });
}

bool ZC_RLDStencilBorder::VErase(ZC_RSController* pRSController)
{
    return fl.Erase(pRSController->pShProg, pRSController->pVAO, ZC_TexturesHolder{ pRSController->pTexture, pRSController->texturesCount },
        { static_cast<const ZC_Uniforms*>(pRSController->GetPersonalData(ZC_RSPDC_uniforms)), pRSController->pGLDraw, nullptr });   //  ZC_RLDData_Uniforms_GLDraw_StencilBorder last parameter don't need for (==) compaer in forward_list
}

void ZC_RLDStencilBorder::VDraw(ZC_FBOBuffersController& rBuffersController)
{
    rBuffersController.GlClear(GL_STENCIL_BUFFER_BIT);
    rBuffersController.GlEnable(GL_DEPTH_TEST);
    rBuffersController.GlEnable(GL_STENCIL_TEST);
 
    glStencilFunc(GL_ALWAYS, 1, 255);
    glStencilOp(GL_REPLACE, GL_REPLACE, GL_REPLACE);
    glStencilMask(255);
    //  draw as usual
    fl.Draw();

    // rBuffersController.GlDisable(GL_DEPTH_TEST);

    glStencilFunc(GL_NOTEQUAL, 1, 255);
    pShPStencilBorder->ActivateOpenGL();    //  use stencil shader program
    for (auto& shProgAndVAOsPair : fl.pairs)
    {
        for (auto& vaoAndTexHoldersPair : shProgAndVAOsPair.second.pairs)
        {
            vaoAndTexHoldersPair.first->ActivateOpenGL();
            for (auto& texHolderAndUniforms_GLDraw_StencilBorderPair : vaoAndTexHoldersPair.second.pairs)
            {
                for (auto& uniforms_glDraw_stencilBorder : texHolderAndUniforms_GLDraw_StencilBorderPair.second.datas)
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

    rBuffersController.GlDisable(GL_DEPTH_TEST);    // !!!
    
    rBuffersController.GlDisable(GL_STENCIL_TEST);
}