#pragma once

#include "ZC_RendererSet.h"

struct ZC_RendererSetAndDrawingSet
{
    typedef typename ZC_RendererSet::DrawingSet RSDrawingSet;
    ZC_RendererSetAndDrawingSet(ZC_RendererSet* _pRendSet, RSDrawingSet&& _drawingSet);

    //  make copy and switch it to drawing level
    ZC_uptr<ZC_RendererSetAndDrawingSet> Make_uptrCopy() const;
    //  make copy and switch it to drawing level
    ZC_sptr<ZC_RendererSetAndDrawingSet> Make_sptrCopy() const;

    typedef typename ZC_RendererSet::Level RSLvl;
    //  lvl - where to switch in ZC_Renderer. If lvl - None, just remove from ZC_Renderer.
    void SwitchToLvl(RSLvl lvl);
    void SetUniformsData(typename ZC_Uniform::Name name, void* data);
    void SetUniformsData(typename ZC_Uniform::Name name, const void* data);
    void SetDrawingSetData(float* stencilScale, unsigned int* stencilColor) noexcept;
    const void* GetUniformsData(typename ZC_Uniform::Name name) const;

private:
    ZC_RendererSet* pRendSet;
    RSDrawingSet drawingSet;
};