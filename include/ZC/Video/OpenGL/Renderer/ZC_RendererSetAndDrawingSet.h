#pragma once

#include "ZC_RendererSet.h"

struct ZC_RendererSetAndDrawingSet
{
    typedef typename ZC_RendererSet::DrawingSet RSDS;
    ZC_RendererSetAndDrawingSet(ZC_RendererSet* _pRendSet, RSDS&& _drawingSet);

    typedef typename ZC_RendererSet::Level RSLvl;
    
    //  lvl - where to switch in ZC_Renderer. If lvl - None, just remove from ZC_Renderer.
    void SwitchToLvl(RSLvl lvl);
    void SetUniformsData(typename ZC_Uniform::Name name, void* data);
    void SetDrawingSetData(float* stencilScale, unsigned int* stencilColor) noexcept;

private:
    ZC_RendererSet* pRendSet;
    RSDS drawingSet;
};