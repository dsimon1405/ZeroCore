#include <ZC/Video/OpenGL/Renderer/ZC_DrawerSet.h>

#include <ZC/Tools/Container/ZC_ContFunc.h>

#include <cassert>

ZC_DrawerSet::ZC_DrawerSet(ZC_ShP_ShPInitSet* pShPInitSet, ZC_VAO&& _vao, ZC_uptr<ZC_GLDraw>&& _upGLDraw,
        std::forward_list<ZC_Buffer>&& _buffers, std::forward_list<ZC_TexturesSet>&& _texSets, std::forward_list<ZC_RSC_RenderSet> _renderSets, const ZC_ShPCompute& _shPCompute)
    : pShP(&(pShPInitSet->shProg)),
    pBaseUniforms(pShPInitSet->uniforms.uniforms.empty() ? nullptr : &(pShPInitSet->uniforms)),     //  if shader program have uniforms add pointer to pBaseUniforms
    vao(std::move(_vao)),
    upGLDraw(std::move(_upGLDraw)),
    buffers(std::move(_buffers)),
    texSets(std::move(_texSets)),
    renderSets(std::move(_renderSets)),
    shPCompute(_shPCompute)
{}

ZC_uptr<ZC_DrawerSet> ZC_DrawerSet::CreateUptr(ZC_ShP_ShPInitSet* pShPInitSet, ZC_VAO&& _vao, ZC_uptr<ZC_GLDraw>&& _upGLDraw,
    std::forward_list<ZC_Buffer>&& _buffers, std::forward_list<ZC_TexturesSet>&& _texSets, std::forward_list<ZC_RSC_RenderSet> _renderSets, const ZC_ShPCompute& _shPCompute)
{
    return { new ZC_DrawerSet(pShPInitSet, std::move(_vao), std::move(_upGLDraw), std::move(_buffers), std::move(_texSets), std::move(_renderSets), _shPCompute) };
}

ZC_sptr<ZC_DrawerSet> ZC_DrawerSet::CreateShptr(ZC_ShP_ShPInitSet* pShPInitSet, ZC_VAO&& _vao, ZC_uptr<ZC_GLDraw>&& _upGLDraw,
    std::forward_list<ZC_Buffer>&& _buffers, std::forward_list<ZC_TexturesSet>&& _texSets, std::forward_list<ZC_RSC_RenderSet> _renderSets, const ZC_ShPCompute& _shPCompute)
{
    return { new ZC_DrawerSet(pShPInitSet, std::move(_vao), std::move(_upGLDraw), std::move(_buffers), std::move(_texSets), std::move(_renderSets), _shPCompute) };
}

ZC_DSController ZC_DrawerSet::MakeZC_DSController(int texSetId, std::forward_list<ZC_uptr<ZC_RSPersonalData>> personalData)
{
        //  if pBaseUniforms was added make copy
    if (pBaseUniforms) personalData.emplace_front(ZC_RSPDUniforms::Make(pBaseUniforms->GetCopy()));

    std::forward_list<ZC_Buffer*> ssbo_buffers;
    for (ZC_Buffer& buf : buffers)
        if (buf.GetType() == GL_SHADER_STORAGE_BUFFER) ssbo_buffers.emplace_front(&buf);

        //  if no textures sets create without pointer on textures
    if (texSetId == -1) return ZC_DSController(pShP, upGLDraw.Get(), &vao, ZC_TexturesHolder{ nullptr, 0 }, std::move(personalData), renderSets,
        std::move(ssbo_buffers), shPCompute.pShP ? &shPCompute : nullptr);

    auto pTexSet = ZC_Find(texSets, texSetId);
    assert(pTexSet);  //  can't find texture set
    
    return ZC_DSController(pShP, upGLDraw.Get(), &vao, ZC_TexturesHolder{ pTexSet->textures.data(), static_cast<uint>(pTexSet->textures.size()) },
        std::move(personalData), renderSets, std::move(ssbo_buffers), shPCompute.pShP ? &shPCompute : nullptr);
}

void ZC_DrawerSet::AddRenderLevel(ZC_RenderLevel renderLevel)
{
    assert(!ZC_Find(renderSets, renderLevel));    //  allready added that ZC_RenderLevel
    renderSets.emplace_front(ZC_RSC_RenderSet(renderLevel));
}


//  ZC_TexturesSet

bool ZC_TexturesSet::operator == (int _id) const noexcept
{
    return id == _id;
}