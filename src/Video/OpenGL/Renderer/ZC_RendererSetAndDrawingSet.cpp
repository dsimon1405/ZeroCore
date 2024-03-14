#include <ZC/Video/OpenGL/Renderer/ZC_RendererSetAndDrawingSet.h>

ZC_RendererSetAndDrawingSet::ZC_RendererSetAndDrawingSet(ZC_RendererSet* _pRendSet, RSDrawingSet&& _drawingSet)
    : pRendSet(_pRendSet),
    drawingSet(std::move(_drawingSet))
{}

ZC_uptr<ZC_RendererSetAndDrawingSet> ZC_RendererSetAndDrawingSet::Make_uptrCopy() const
{
    ZC_uptr<ZC_RendererSetAndDrawingSet> upRSADS(new ZC_RendererSetAndDrawingSet(pRendSet, drawingSet.GetCopy()));
    upRSADS->SwitchToLvl(drawingSet.lvl);
    return upRSADS;
}

ZC_sptr<ZC_RendererSetAndDrawingSet> ZC_RendererSetAndDrawingSet::Make_sptrCopy() const
{
    ZC_sptr<ZC_RendererSetAndDrawingSet> upRSADS(new ZC_RendererSetAndDrawingSet(pRendSet, drawingSet.GetCopy()));
    upRSADS->SwitchToLvl(drawingSet.lvl);
    return upRSADS;
}

void ZC_RendererSetAndDrawingSet::SwitchToLvl(RSLvl lvl)
{
    if (drawingSet.lvl == lvl) return;
    if (drawingSet.lvl != RSLvl::None) pRendSet->Erase(&drawingSet);
    drawingSet.lvl = lvl;
    if (lvl == RSLvl::None) return;
    pRendSet->Add(&drawingSet);
}

void ZC_RendererSetAndDrawingSet::SetUniformsData(typename ZC_Uniform::Name name, void* data)
{
    drawingSet.uniforms.Set(name, data);
}

void ZC_RendererSetAndDrawingSet::SetUniformsData(typename ZC_Uniform::Name name, const void* data)
{
    drawingSet.uniforms.Set(name, const_cast<void*>(data));
}

void ZC_RendererSetAndDrawingSet::SetDrawingSetData(float* stencilScale, unsigned int* stencilColor) noexcept
{
    if (stencilScale) drawingSet.stencilScale = *stencilScale;
    if (stencilColor) drawingSet.stencilColor = *stencilColor;
}

const void* ZC_RendererSetAndDrawingSet::GetUniformsData(typename ZC_Uniform::Name name) const
{
    return drawingSet.uniforms.Get(name);
}