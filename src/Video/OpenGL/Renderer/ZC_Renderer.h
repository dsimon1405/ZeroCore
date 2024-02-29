#pragma once

#include <ZC/Video/OpenGL/Renderer/ZC_RendererSet.h>
#include "ZC_RBufferCleaner.h"

#include <map>
#include <forward_list>

struct ZC_Renderer
{
    ZC_Renderer() = delete;
    static void Init(bool enableDepthTest);
    static void DrawAll();
    typedef typename ZC_RendererSet::Level RSLevel;
    static void Add(RSLevel lvl, ZC_RendererSet* pRS);
    static void Erase(RSLevel lvl, ZC_RendererSet* pRS);

private:
    static inline std::map<RSLevel, std::map<ZC_ShProg*, std::forward_list<ZC_RendererSet*>>> rendererSets;
    static inline ZC_ShProg* pShPStencil;
    static inline ZC_RBufferCleaner bufferCleaner;

    static void DrawDrawing(std::map<ZC_ShProg*, std::forward_list<ZC_RendererSet*>>& fl);
    static void DrawStencil(std::map<ZC_ShProg*, std::forward_list<ZC_RendererSet*>>& fl);
    static void DrawImGui(std::map<ZC_ShProg*, std::forward_list<ZC_RendererSet*>>& fl);
};