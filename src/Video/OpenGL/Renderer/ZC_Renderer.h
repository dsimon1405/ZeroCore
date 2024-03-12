#pragma once

#include <ZC/Video/OpenGL/Renderer/ZC_RendererSet.h>
#include "ZC_RBufferCleaner.h"
#include <ZC/Tools/Function/ZC_Function.h>

#include <map>
#include <forward_list>

struct ZC_Renderer
{
    ZC_Renderer(ZC_Function<void()>&& _funcSwapBuffer);
    ~ZC_Renderer();

    typedef typename ZC_RendererSet::Level RSLevel;
    static void Add(RSLevel lvl, ZC_RendererSet* pRS);
    static void Erase(RSLevel lvl, ZC_RendererSet* pRS);

    void Configure(bool useDepthTest);
    void DrawAll();

private:
    static inline ZC_Renderer* pRenderer;

    std::map<RSLevel, std::map<ZC_ShProg*, std::forward_list<ZC_RendererSet*>>> rendererSets;
    ZC_ShProg* pShPStencil;
    ZC_RBufferCleaner bufferCleaner;
    ZC_Function<void()> funcSwapBuffer;

    void DrawDrawing(std::map<ZC_ShProg*, std::forward_list<ZC_RendererSet*>>& mapShPRS);
    void DrawStencil(std::map<ZC_ShProg*, std::forward_list<ZC_RendererSet*>>& mapShPRS);
    void DrawImGui(std::map<ZC_ShProg*, std::forward_list<ZC_RendererSet*>>& mapShPRS);
    void DrawTextWidndow(std::map<ZC_ShProg*, std::forward_list<ZC_RendererSet*>>& mapShPRS);
};