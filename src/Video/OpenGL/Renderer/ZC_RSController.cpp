#include <ZC/Video/OpenGL/Renderer/ZC_RSController.h>

#include <ZC/Video/OpenGL/Renderer/ZC_Renders.h>
#include <ZC/Tools/Container/ZC_ContFunc.h>

#include <cassert>

ZC_RSController::ZC_RSController(const ZC_ShProg* _pShProg, const ZC_GLDraw* _pGLDraw, const ZC_VAO* _pVAO, const ZC_Texture* _pTexture,
        ulong _texturesCount, std::forward_list<ZC_uptr<ZC_RSPersonalData>>&& _personalData, std::forward_list<RenderSet> _renderSets)
    : pShProg(_pShProg),
    pGLDraw(_pGLDraw),
    pVAO(_pVAO),
    pTexture(_pTexture),
    texturesCount(_texturesCount),
    personalData(std::move(_personalData)),
    renderSets(std::move(_renderSets))
{}

ZC_RSController::~ZC_RSController()
{
    for (auto& renderSet : renderSets) renderSet.SwitchToDrawLevel(ZC_DL_None, this);
}

void ZC_RSController::SwitchToDrawLvl(ZC_FrameBuffer frameBuffer, ZC_DrawLevel drawLevel)
{
    auto pRenderSet = ZC_Find(renderSets, frameBuffer);
    assert(pRenderSet);     //  tries to add on render's level, wich was not add while ZC_RSController created.
    pRenderSet->SwitchToDrawLevel(drawLevel, this);
}

void ZC_RSController::SetData(ZC_RSPDCategory category, ZC_RSPDStoredData* pData)
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

void ZC_RSController::SetUniformsData(ZC_UniformName unName, void* pData)
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

const void* ZC_RSController::GetPersonalData(ZC_RSPDCategory category) const
{
    for (auto& upPersData : personalData)
        if (upPersData->category == category) return upPersData->GetPointerOnData();
    return nullptr;
}

const void* ZC_RSController::GetDataFromUniforms(ZC_UniformName unName) const
{
    for (auto& upPersData : personalData)
        if (upPersData->category == ZC_RSPDC_uniforms)
            return static_cast<const ZC_Uniforms*>(upPersData->GetPointerOnData())->Get(unName);
    assert(false);  //  can't find uniforms!
    return nullptr;
}

ZC_RSController ZC_RSController::MakeCopy() const
{
    std::forward_list<ZC_uptr<ZC_RSPersonalData>> copyPersonalData;
    for (auto& upPersData : personalData) copyPersonalData.emplace_front(upPersData->MakeCopy());
    
    auto copyRenderSets = renderSets;
    for (auto& renderSet : copyRenderSets) renderSet.drawLevel = ZC_DL_None;

    return { pShProg, pGLDraw, pVAO, pTexture, texturesCount, std::move(copyPersonalData), std::move(copyRenderSets) };
}

bool ZC_RSController::IsDrawing(ZC_FrameBuffer renderLevel)
{
    auto pRenderSet = ZC_Find(renderSets, renderLevel);
    assert(pRenderSet);     //  tries to add on render's level, wich was not add while ZC_RSController created.
    return pRenderSet->drawLevel != ZC_DL_None;
}

void ZC_RSController::AddRenderLevel(ZC_FrameBuffer renderLevel)
{
    assert(!ZC_Find(renderSets, renderLevel));    //  allready added that ZC_RenderLevel
    renderSets.emplace_front(RenderSet(renderLevel));
}


//  RenderSet

ZC_RSController::RenderSet::RenderSet(ZC_FrameBuffer renderLevel)
    : pRender(ZC_Renders::GetRender(renderLevel)),
    drawLevel(ZC_DL_None)
{}

bool ZC_RSController::RenderSet::operator == (ZC_FrameBuffer frameBuffer)
{
    return pRender->frameBuffer == frameBuffer;
}

void ZC_RSController::RenderSet::SwitchToDrawLevel(ZC_DrawLevel _drawLevel, ZC_RSController* pRSController)
{
    if (drawLevel == _drawLevel) return;
    if (drawLevel != ZC_DL_None) pRender->Erase(pRSController, drawLevel);
    drawLevel = _drawLevel;
    if (drawLevel == ZC_DL_None) return;
    pRender->Add(pRSController, drawLevel);
}