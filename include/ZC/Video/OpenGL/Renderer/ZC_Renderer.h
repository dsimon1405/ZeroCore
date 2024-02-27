#pragma once

#include "ZC_RendererSet.h"

#include <map>
#include <forward_list>

using namespace std;

struct ZC_Renderer
{
    typedef typename ZC_RendererSet::Level RSLevel;

    ZC_Renderer() = delete;
    static void DrawAll();
    static void Add(RSLevel lvl, ZC_RendererSet* pRS);
    static void Erase(RSLevel lvl, ZC_RendererSet* pRS);
    static void EnablePointSize();
    static void SetStencilShP(ZC_ShProg* _shPStencil);

private:
    static inline ZC_ShProg* pShPStencil;
    static inline map<RSLevel, map<ZC_ShProg*, forward_list<ZC_RendererSet*>>> rendSets;

    static void Draw(map<ZC_ShProg*, forward_list<ZC_RendererSet*>>& fl);
    static void DrawStencil(map<ZC_ShProg*, forward_list<ZC_RendererSet*>>& fl);
    static void DrawImGui(map<ZC_ShProg*, forward_list<ZC_RendererSet*>>& fl);
};