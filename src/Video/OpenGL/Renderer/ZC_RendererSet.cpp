#include <ZC/Video/OpenGL/Renderer/ZC_RendererSet.h>

#include <cassert>

ZC_uptr<ZC_RendererSet> ZC_RendererSet::CreateUptr(typename ZC_ShProgs::ShPInitSet* pShPInitSet, ZC_VAO&& _vao, ZC_uptr<ZC_GLDraw>&& _upGLDraw,
    std::forward_list<ZC_Buffer>&& _buffers, std::forward_list<ZC_TexturesSet>&& _texSets)
{
    return { new ZC_RendererSet(pShPInitSet, std::move(_vao), std::move(_upGLDraw), std::move(_buffers), std::move(_texSets)) };
}

ZC_sptr<ZC_RendererSet> ZC_RendererSet::CreateShptr(typename ZC_ShProgs::ShPInitSet* pShPInitSet, ZC_VAO&& _vao, ZC_uptr<ZC_GLDraw>&& _upGLDraw,
    std::forward_list<ZC_Buffer>&& _buffers, std::forward_list<ZC_TexturesSet>&& _texSets)
{
    return { new ZC_RendererSet(pShPInitSet, std::move(_vao), std::move(_upGLDraw), std::move(_buffers), std::move(_texSets)) };
}

ZC_RSController ZC_RendererSet::MakeZC_RSController(int texSetId, std::forward_list<ZC_uptr<ZC_RSPersonalData>>&& personalData)
{
    //  if pBaseUniforms was added make copy
    if (pBaseUniforms) personalData.emplace_front(ZC_RSPDUniforms::Make(pBaseUniforms->GetCopy()));
    //  if no textures sets create without pointer on textures
    if (texSetId == -1) return ZC_RSController(this->pShP, upGLDraw.Get(), &vao, nullptr, 0, std::move(personalData));
    
    ZC_Texture* pTexture = nullptr;     //  pointer on begin of ZC_TexturesSet::textures
    size_t texturesCount = 0;
    for (auto& texSet : texSets)
    {
        if (texSet.id == texSetId)
        {
            pTexture = &*(texSet.textures.begin());
            texturesCount = texSet.textures.size();
        }
    }
    assert(pTexture);  //  can't find texture set
    return ZC_RSController(this->pShP, upGLDraw.Get(), &vao, pTexture, texturesCount, std::move(personalData));
}
    
ZC_RendererSet::ZC_RendererSet(typename ZC_ShProgs::ShPInitSet* pShPInitSet, ZC_VAO&& _vao, ZC_uptr<ZC_GLDraw>&& _upGLDraw,
        std::forward_list<ZC_Buffer>&& _buffers, std::forward_list<ZC_TexturesSet>&& _texSets)
    : pShP(&(pShPInitSet->shProg)),
    pBaseUniforms(pShPInitSet->uniforms.uniforms.empty() ? nullptr : &(pShPInitSet->uniforms)),     //  if shader program have uniforms add pointer to pBaseUniforms
    vao(std::move(_vao)),
    upGLDraw(std::move(_upGLDraw)),
    buffers(std::move(_buffers)),
    texSets(std::move(_texSets))
{}