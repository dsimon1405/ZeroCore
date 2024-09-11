#pragma once

#include "ZC_Render.h"
#include <ZC/GUI/Backend/ZC_GUI.h>

class ZC_Renderer : public ZC_UBOs
{
public:
    ZC_Renderer(ZC_Function<void()>&& _funcSwapBuffer);
    ~ZC_Renderer();

    static void AddUBOs(ZC_UBO* ubo, ZC_Function<void()>&& fUpdate);
    static void EraseUBOs(ZC_UBO* pUbo);
    static void Add(ZC_Render* pRender);
    static void Erase(ZC_Render* pRender);
    void Draw(ZC_GUI& gui);
    
private:
    static inline ZC_Renderer* pRenderer;

    ZC_Function<void()> funcSwapBuffer;
    std::forward_list<ZC_Render*> renders;
};