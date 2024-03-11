#include <ZC/Video/OpenGL/Renderer/ZC_RendererSetAndDrawingSet.h>

ZC_RendererSetAndDrawingSet::ZC_RendererSetAndDrawingSet(ZC_RendererSet* _pRendSet, RSDrawingSet&& _drawingSet)
    : pRendSet(_pRendSet),
    drawingSet(std::move(_drawingSet))
{}

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

void ZC_RendererSetAndDrawingSet::SetDrawingSetData(float* stencilScale, unsigned int* stencilColor) noexcept
{
    if (stencilScale) drawingSet.stencilScale = *stencilScale;
    if (stencilColor) drawingSet.stencilColor = *stencilColor;
}