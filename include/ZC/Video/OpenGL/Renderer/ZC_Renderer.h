#pragma once

#include "ZC_RSController.h"
#include <ZC/Tools/Function/ZC_Function.h>
#include "ZC_RBufferCleaner.h"
#include "ZC_RendererLevelDrawer.h"

#include <map>

class ZC_Renderer
{
public:
    ZC_Renderer(ZC_Function<void()>&& _funcSwapBuffer);
    ~ZC_Renderer();

    void Configure(bool useDepthTest);
    static void Add(ZC_RSController* pRSController);
    static void Erase(ZC_RSController* pRSController);

    void Draw();

private:
    static inline ZC_Renderer* pCurrentRenderer;

    std::map<ZC_RendererLevel, ZC_uptr<ZC_RendererLevelDrawer>> rendererLevelDrawers;
    ZC_RBufferCleaner bufferCleaner;
    ZC_Function<void()> funcSwapBuffer;
};