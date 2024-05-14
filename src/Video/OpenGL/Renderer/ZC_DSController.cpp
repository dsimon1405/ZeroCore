#include <ZC/Video/OpenGL/Renderer/ZC_DSController.h>

#include <ZC/Video/OpenGL/Renderer/ZC_Renders.h>
#include <ZC/Tools/Container/ZC_ContFunc.h>
#include <ZC/Tools/Math/ZC_Mat4.h>
#include <ZC/Video/OpenGL/Shader/ZC_ShProgs.h>

#include <cassert>

//  ZC_TexturesHolder

bool ZC_TexturesHolder::operator == (const ZC_TexturesHolder& th) const noexcept
{
    return pTexture == th.pTexture;
}

void ZC_TexturesHolder::ActivateOpenGL() const
{
    for (uint i = 0; i < texturesCount; i++) pTexture->ActiveTexture(i);
}


//  ZC_DSController

ZC_DSController::ZC_DSController(const ZC_ShProg* _pShProg, const ZC_GLDraw* _pGLDraw, const ZC_VAO* _pVAO, const ZC_TexturesHolder& _texturesHolder,
        std::forward_list<ZC_uptr<ZC_RSPersonalData>>&& _personalData, std::forward_list<RenderSet> _renderSets)
    : pShProg(_pShProg),
    pGLDraw(_pGLDraw),
    pVAO(_pVAO),
    texturesHolder(_texturesHolder),
    personalData(std::move(_personalData)),
    renderSets(std::move(_renderSets))
{}

ZC_DSController::~ZC_DSController()
{
    for (auto& rendSet : renderSets) rendSet.SwitchToDrawLevel(ZC_DL_None, this);
}

void ZC_DSController::SwitchToDrawLvl(ZC_RenderLevel renderLevel, ZC_DrawerLevel drawerLevel)
{
    auto pRenderSet = ZC_Find(renderSets, renderLevel);
    assert(pRenderSet);     //  tries to add on render's level, wich was not add while ZC_DSController created.
    pRenderSet->SwitchToDrawLevel(drawerLevel, this);
}

void ZC_DSController::SetData(ZC_RSPDCategory category, ZC_RSPDStoredData* pData)
{
    for (auto& upPersData : personalData)
    {
        if (upPersData->category == category)
        {
            upPersData->SetData(pData);
            return;
        }
    }
    assert(false);  //  can't find category!
}

void ZC_DSController::SetUniformsData(ZC_UniformName unName, void* pData)
{
    for (auto& upPersData : personalData)
    {
        if (upPersData->category == ZC_RSPDC_uniforms)
        {
            ZC_RSPDUniformData unData(unName, pData);
            upPersData->SetData(&unData);
            return;
        }
    }
    assert(false);  //  can't find category!
}

const void* ZC_DSController::GetPersonalData(ZC_RSPDCategory category) const
{
    for (auto& upPersData : personalData)
        if (upPersData->category == category) return upPersData->GetPointerOnData();
    return nullptr;
}

const void* ZC_DSController::GetDataFromUniforms(ZC_UniformName unName) const
{
    for (auto& upPersData : personalData)
        if (upPersData->category == ZC_RSPDC_uniforms)
            return static_cast<const ZC_Uniforms*>(upPersData->GetPointerOnData())->Get(unName);
    assert(false);  //  can't find uniforms!
    return nullptr;
}

ZC_DSController ZC_DSController::MakeCopy() const
{
    std::forward_list<ZC_uptr<ZC_RSPersonalData>> copyPersonalData;
    for (auto& upPersData : personalData) copyPersonalData.emplace_front(upPersData->MakeCopy());
    
    auto copyRenderSets = renderSets;
    for (auto& drawerSet : copyRenderSets) drawerSet.drawerLevel = ZC_DL_None;

    return { pShProg, pGLDraw, pVAO, texturesHolder, std::move(copyPersonalData), std::move(copyRenderSets) };
}

bool ZC_DSController::IsDrawing(ZC_RenderLevel renderLevel)
{
    auto pRenderSet = ZC_Find(renderSets, renderLevel);
    assert(pRenderSet);     //  tries to add on render's level, wich was not add while ZC_DSController created.
    return pRenderSet->drawerLevel != ZC_DL_None;
}

ZC_RenderLevel ZC_DSController::GetDrawingLevel(ZC_RenderLevel renderLevel)
{
    auto pRenderSet = ZC_Find(renderSets, renderLevel);
    assert(pRenderSet);     //  tries to add on render's level, wich was not add while ZC_DSController created.
    return pRenderSet->drawerLevel;
}

void ZC_DSController::AddRender(ZC_RenderLevel renderLevel)
{
    assert(!ZC_Find(renderSets, renderLevel));    //  allready added that ZC_RenderLevel
    renderSets.emplace_front(RenderSet(renderLevel));
}

void ZC_DSController::SetTexturesHolder(const ZC_TexturesHolder& _texturesHolder)
{
    texturesHolder = _texturesHolder;
}


//  ZC_DSController::RenderSet

ZC_DSController::RenderSet::RenderSet(ZC_RenderLevel renderLevel)
    : pRender(ZC_Renders::GetRender(renderLevel)),
    drawerLevel(ZC_DL_None)
{}

bool ZC_DSController::RenderSet::operator == (ZC_RenderLevel renderLevel)
{
    return pRender->renderLevel == renderLevel;
}

void ZC_DSController::RenderSet::SwitchToDrawLevel(ZC_DrawerLevel _drawLevel, ZC_DSController* pRSController)
{
    if (drawerLevel == _drawLevel) return;
    if (drawerLevel != ZC_DL_None) pRender->Erase(pRSController, drawerLevel);
    drawerLevel = _drawLevel;
    if (drawerLevel == ZC_DL_None) return;
    pRender->Add(pRSController, drawerLevel);
}


//  ZC_Uniforms_GLDraw

bool ZC_RLDData_Uniforms_GLDraw::operator == (const ZC_RLDData_Uniforms_GLDraw& unifAndGLDraw) const noexcept
{
    return pUniforms == unifAndGLDraw.pUniforms && pGLDraw == unifAndGLDraw.pGLDraw;
}

void ZC_RLDData_Uniforms_GLDraw::Draw()
{
    if (pUniforms) pUniforms->Activate();
    pGLDraw->Draw();
}

//  ZC_Uniforms_GLDraw_StencilBorder

bool ZC_RLDData_Uniforms_GLDraw_StencilBorder::operator == (const ZC_RLDData_Uniforms_GLDraw_StencilBorder& unifAndGLDraw) const noexcept
{
    return pUniforms == unifAndGLDraw.pUniforms && pGLDraw == unifAndGLDraw.pGLDraw;
}

void ZC_RLDData_Uniforms_GLDraw_StencilBorder::Draw()
{
    if (isFirstDraw)    //  first draw makes as a usual
    {
        isFirstDraw = false;

        if (pUniforms) pUniforms->Activate();
        pGLDraw->Draw();
    }
    else    //  second draw is drawing border, need reset stencil border's shader uniforms (scale object's model matrix) and draw
    {
        isFirstDraw = true;

        static ZC_Uniforms* pUniformsStencilBorder = &(ZC_ShProgs::Get(ShPN_ZCR_StencilBorder)->uniforms);

        auto model = *static_cast<const ZC_Mat4<float>*>(pUniforms->Get(ZC_UN_unModel));
        model.Scale(pStencilBorderData->scaleX, pStencilBorderData->scaleY, pStencilBorderData->scaleZ);
        pUniformsStencilBorder->Set(ZC_UN_unModel, &model);
        pUniformsStencilBorder->Set(ZC_UN_unColor, &(pStencilBorderData->color));
        pUniformsStencilBorder->Activate();     //  activate stencil uniforms and draw
        pGLDraw->Draw();
    }
}