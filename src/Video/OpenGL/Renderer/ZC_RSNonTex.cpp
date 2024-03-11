#include <ZC/Video/OpenGL/Renderer/ZC_RSNonTex.h>

#include <ZC/Video/OpenGL/Renderer/ZC_RendererSetAndDrawingSet.h>

ZC_RSNonTex::ZC_RSNonTex(typename ZC_ShProgs::ShPInitSet* pShPInitSet, ZC_VAO&& _vao, ZC_uptr<ZC_GLDraw>&& _upDraw,
        std::forward_list<ZC_Buffer>&& _buffers)
    : ZC_RS(pShPInitSet, std::move(_vao), std::move(_upDraw), std::move(_buffers))
{}

ZC_RSNonTex::ZC_RSNonTex(ZC_RSNonTex&& rs)
    : ZC_RS(dynamic_cast<ZC_RS&&>(rs))
{}

void ZC_RSNonTex::Draw(Level lvl)
{
    vao.BindVertexArray();
    levelController.Draw(lvl, upGLDraw, nullptr, 0);
}

ZC_uptr<ZC_RendererSetAndDrawingSet> ZC_RSNonTex::Make_uptrRendererSetDrawingSet(const char* texSetName, float stencilScale, unsigned int stencilColor)
{
    return { new ZC_RendererSetAndDrawingSet(this, { pBaseUniforms->GetCopy(), Level::None, nullptr, stencilScale, stencilColor }) };
}

ZC_sptr<ZC_RendererSetAndDrawingSet> ZC_RSNonTex::Make_sptrRendererSetDrawingSet(const char* texSetName, float stencilScale, unsigned int stencilColor)
{
    return { new ZC_RendererSetAndDrawingSet(this, { pBaseUniforms->GetCopy(), Level::None, nullptr, stencilScale, stencilColor }) };
}

void ZC_RSNonTex::Add(DrawingSet* pDS)
{
    if (levelController.Add(pDS)) AddToRenderer(pDS->lvl);
}

void ZC_RSNonTex::Erase(DrawingSet* pDS)
{
    if (levelController.Erase(pDS)) EraseFromRenderer(pDS->lvl);
}