#include <ZC/Video/OpenGL/Renderer/ZC_RSController.h>

#include <ZC/Video/OpenGL/Renderer/ZC_Renderer.h>

#include <cassert>

ZC_RSController::ZC_RSController(ZC_ShProg* _pShProg, ZC_GLDraw* _pGLDraw, ZC_VAO* _pVAO, ZC_Texture* _pTexture,
        size_t _texturesCount, std::forward_list<ZC_uptr<ZC_RSPersonalData>>&& _personalData)
    : pShProg(_pShProg),
    pGLDraw(_pGLDraw),
    pVAO(_pVAO),
    pTexture(_pTexture),
    texturesCount(_texturesCount),
    personalData(std::move(_personalData))
{}

ZC_RSController::~ZC_RSController()
{
    if (rendererLevel != ZC_RL_None) ZC_Renderer::Erase(this);
}

void ZC_RSController::SwitchToLvl(ZC_RendererLevel lvl)
{
    if (rendererLevel == lvl) return;
    if (rendererLevel != ZC_RL_None) ZC_Renderer::Erase(this);
    rendererLevel = lvl;
    if (rendererLevel == ZC_RL_None) return;
    ZC_Renderer::Add(this);
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

void ZC_RSController::SetUniformsData(ZC_UniformName unNmae, void* pData)
{
    for (auto& upPersData : personalData)
    {
        if (upPersData->category == ZC_RSPDC_uniforms)
        {
            ZC_RSPDUniformData unData(unNmae, pData);
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

const void* ZC_RSController::GetDataFromUniforms(ZC_UniformName name) const
{
    for (auto& upPersData : personalData)
        if (upPersData->category == ZC_RSPDC_uniforms)
            return static_cast<const ZC_Uniforms*>(upPersData->GetPointerOnData())->Get(name);
    assert(false);  //  can't find uniforms!
    return nullptr;
}

ZC_RSController ZC_RSController::MakeCopy() const
{
    std::forward_list<ZC_uptr<ZC_RSPersonalData>> copyPersonalData;
    for (auto& upPersData : personalData) copyPersonalData.emplace_front(upPersData->MakeCopy());
    return { pShProg, pGLDraw, pVAO, pTexture, texturesCount, std::move(copyPersonalData) };
}
