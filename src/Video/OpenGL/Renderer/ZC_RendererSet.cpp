#include <ZC/Video/OpenGL/Renderer/ZC_Renderer.h>

#include <ZC/Video/OpenGL/Renderer/ZC_RendererSetAndDrawingSet.h>

bool ZC_RendererSet::operator == (ZC_RendererSet* pRS) const noexcept
{
    return this == pRS;
}

ZC_uptr<ZC_RendererSetAndDrawingSet> ZC_RendererSet::Make_uptrRendererSetDrawingSet(const char* texSetName, float stencilScale, unsigned int stencilColor)
{
    return nullptr;
}

ZC_sptr<ZC_RendererSetAndDrawingSet> ZC_RendererSet::Make_sptrRendererSetDrawingSet(const char* texSetName, float stencilScale, unsigned int stencilColor)
{
    return nullptr;
}

void ZC_RendererSet::AddToRenderer(Level lvl)
{
    ZC_Renderer::Add(lvl, this);
}

void ZC_RendererSet::EraseFromRenderer(Level lvl)
{
    ZC_Renderer::Erase(lvl, this);
}