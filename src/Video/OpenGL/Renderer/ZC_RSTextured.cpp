#include <ZC/Video/OpenGL/Renderer/ZC_RSTextured.h>

#include <ZC/Video/OpenGL/Renderer/ZC_RendererSetAndDrawingSet.h>
#include <ZC/ErrorLogger/ZC_ErrorLogger.h>

#include <algorithm>

ZC_RSTextured::ZC_RSTextured(typename ZC_ShProgs::ShPInitSet* pShPInitSet, ZC_VAO&& _vao, ZC_uptr<ZC_GLDraw>&& _upDraw,
        std::forward_list<ZC_Buffer>&& _buffers, std::vector<TexSet>&& _texSets)
    : ZC_RS(pShPInitSet, std::move(_vao), std::move(_upDraw), std::move(_buffers)),
    texSets(std::move(_texSets))
{
    std::fill(lvlCounter, lvlCounter + Level::None, 0);
}

ZC_RSTextured::ZC_RSTextured(ZC_RSTextured&& rs)
    : ZC_RS(dynamic_cast<ZC_RS&&>(rs)),
    texSets(std::move(rs.texSets))
{}

void ZC_RSTextured::Draw(Level lvl)
{
    vao.BindVertexArray();
    for (auto& texSet : texSets) texSet.levelController.Draw(lvl, upGLDraw, &(texSet.textures[0]), texSet.textures.size());
}

ZC_uptr<ZC_RendererSetAndDrawingSet> ZC_RSTextured::Make_uptrRendererSetDrawingSet(const char* texSetName, float stencilScale, unsigned int stencilColor)
{
    auto texSetsIter = std::find(texSets.begin(), texSets.end(), texSetName);
    if (texSetsIter != texSets.end())
        return { new ZC_RendererSetAndDrawingSet(this, { pBaseUniforms->GetCopy(), Level::None, &(*texSetsIter), stencilScale, stencilColor }) };
    return nullptr;
}

ZC_sptr<ZC_RendererSetAndDrawingSet> ZC_RSTextured::Make_sptrRendererSetDrawingSet(const char* texSetName, float stencilScale, unsigned int stencilColor)
{
    auto texSetsIter = std::find(texSets.begin(), texSets.end(), texSetName);
    if (texSetsIter != texSets.end())
        return { new ZC_RendererSetAndDrawingSet(this, { pBaseUniforms->GetCopy(), Level::None, &(*texSetsIter), stencilScale, stencilColor }) };
    return nullptr;
}

void ZC_RSTextured::Add(DrawingSet* pDS)
{
    auto texSetsIter = std::find(texSets.begin(), texSets.end(), pDS->pTexSet);
    if (texSetsIter == texSets.end()) ZC_ErrorLogger::Err("Can't find ZC_RSTexs::TexSet!", __FILE__,__LINE__);
    texSetsIter->levelController.Add(pDS);
    if ((lvlCounter[pDS->lvl])++ == 0) AddToRenderer(pDS->lvl);
}

void ZC_RSTextured::Erase(DrawingSet* pDS)
{
    auto texSetsIter = std::find(texSets.begin(), texSets.end(), pDS->pTexSet);
    if (texSetsIter == texSets.end()) ZC_ErrorLogger::Err("Can't find ZC_RSTexs::TexSet!", __FILE__,__LINE__);
    texSetsIter->levelController.Erase(pDS);
    if (--(lvlCounter[pDS->lvl]) == 0) EraseFromRenderer(pDS->lvl);
}


//  TexSet start

ZC_RSTextured::TexSet::TexSet(std::string&& _setName, std::vector<ZC_Texture>&& _textures)
    : setName(std::move(_setName)),
    textures(std::move(_textures))
{}

ZC_RSTextured::TexSet::TexSet(TexSet&& ts)
    : setName(std::move(ts.setName)),
    textures(std::move(ts.textures)),
    levelController(std::move(ts.levelController))
{}

bool ZC_RSTextured::TexSet::operator == (const char* _name) const
{
    return setName == _name;
}

bool ZC_RSTextured::TexSet::operator == (void* pTexSet) const
{
    return this == pTexSet;
}