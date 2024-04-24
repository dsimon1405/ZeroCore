#include <ZC/Video/OpenGL/Renderer/ZC_RenderSet.h>

#include <ZC/Tools/Container/ZC_ContFunc.h>

#include <cassert>

ZC_RenderSet::ZC_RenderSet(ZC_ShP_ShPInitSet* pShPInitSet, ZC_VAO&& _vao, ZC_uptr<ZC_GLDraw>&& _upGLDraw,
        std::forward_list<ZC_Buffer>&& _buffers, std::forward_list<ZC_TexturesSet>&& _texSets, std::forward_list<ZC_RSC_RenderSet> _renderSets)
    : pShP(&(pShPInitSet->shProg)),
    pBaseUniforms(pShPInitSet->uniforms.uniforms.empty() ? nullptr : &(pShPInitSet->uniforms)),     //  if shader program have uniforms add pointer to pBaseUniforms
    vao(std::move(_vao)),
    upGLDraw(std::move(_upGLDraw)),
    buffers(std::move(_buffers)),
    texSets(std::move(_texSets)),
    renderSets(std::move(_renderSets))
{}

ZC_uptr<ZC_RenderSet> ZC_RenderSet::CreateUptr(ZC_ShP_ShPInitSet* pShPInitSet, ZC_VAO&& _vao, ZC_uptr<ZC_GLDraw>&& _upGLDraw,
    std::forward_list<ZC_Buffer>&& _buffers, std::forward_list<ZC_TexturesSet>&& _texSets, std::forward_list<ZC_RSC_RenderSet> _renderSets)
{
    return { new ZC_RenderSet(pShPInitSet, std::move(_vao), std::move(_upGLDraw), std::move(_buffers), std::move(_texSets), std::move(_renderSets)) };
}

ZC_sptr<ZC_RenderSet> ZC_RenderSet::CreateShptr(ZC_ShP_ShPInitSet* pShPInitSet, ZC_VAO&& _vao, ZC_uptr<ZC_GLDraw>&& _upGLDraw,
    std::forward_list<ZC_Buffer>&& _buffers, std::forward_list<ZC_TexturesSet>&& _texSets, std::forward_list<ZC_RSC_RenderSet> _renderSets)
{
    return { new ZC_RenderSet(pShPInitSet, std::move(_vao), std::move(_upGLDraw), std::move(_buffers), std::move(_texSets), std::move(_renderSets)) };
}

ZC_RSController ZC_RenderSet::MakeZC_RSController(int texSetId, std::forward_list<ZC_uptr<ZC_RSPersonalData>> personalData)
{
    //  if pBaseUniforms was added make copy
    if (pBaseUniforms) personalData.emplace_front(ZC_RSPDUniforms::Make(pBaseUniforms->GetCopy()));
    //  if no textures sets create without pointer on textures
    if (texSetId == -1) return ZC_RSController(this->pShP, upGLDraw.Get(), &vao, nullptr, 0, std::move(personalData), renderSets);

    auto pTexSet = ZC_Find(texSets, texSetId);
    assert(pTexSet);  //  can't find texture set
    
    return ZC_RSController(this->pShP, upGLDraw.Get(), &vao, &*(pTexSet->textures.begin()),
        static_cast<ulong>(pTexSet->textures.size()), std::move(personalData), renderSets);
    
    
    // ZC_Texture* pTexture = nullptr;     //  pointer on begin of ZC_TexturesSet::textures
    // ulong texturesCount = 0;

    // for (auto& texSet : texSets)
    // {
    //     if (texSet.id == rscConfig.texSetId)
    //     {
    //         pTexture = &*(texSet.textures.begin());
    //         texturesCount = static_cast<ulong>(texSet.textures.size());
    //     }
    // }
    // assert(pTexture);  //  can't find texture set
    
    // return ZC_RSController(this->pShP, upGLDraw.Get(), &vao, pTexture, texturesCount, std::move(rscConfig.personalData), renderSets);
}

void ZC_RenderSet::AddRenderLevel(ZC_FrameBuffer renderLevel)
{
    assert(!ZC_Find(renderSets, renderLevel));    //  allready added that ZC_RenderLevel
    renderSets.emplace_front(ZC_RSC_RenderSet(renderLevel));
}


//  ZC_TexturesSet

bool ZC_TexturesSet::operator == (int _id) const noexcept
{
    return id == _id;
}