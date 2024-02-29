#include "ZC_Renderer.h"

#include <ZC/Video/OpenGL/Shader/ZC_ShProgs.h>
#include <Video/OpenGL/ZC_OpenGL.h>
#include <ZC/Video/OpenGL/Buffer/ZC_UBOs.h>
#include <ZC/Tools/Container/ZC_ContFunc.h>
#include <ZC/ErrorLogger/ZC_ErrorLogger.h>
#ifdef ZC_IMGUI
#include <Video/imgui/ZC_ImGui.h>
#endif

void ZC_Renderer::Init(bool enableDepthTest)
{
    auto pShProg = ZC_ShProgs::Get(ZC_ShProgs::Name::ZCR_Stencil);
    if (pShProg) pShPStencil = &(pShProg->shProg);  //  sets stencil shader if it exists
    if (enableDepthTest) bufferCleaner.GlEnable(GL_DEPTH_TEST); //  sets depth test
}

void ZC_Renderer::DrawAll()
{
    bufferCleaner.GlClear();
    ZC_UBOs::Update();
    for (auto& rendSetsP : rendererSets)
    {
        switch (rendSetsP.first)
        {
        case RSLevel::Drawing: DrawDrawing(rendSetsP.second); break;
        case RSLevel::Stencil: DrawStencil(rendSetsP.second); break;
#ifdef ZC_IMGUI
        case RSLevel::ImGui: DrawImGui(rendSetsP.second); break;
#endif
        case RSLevel::None: break;
        }
    }
    glBindVertexArray(0);
}

void ZC_Renderer::Add(RSLevel lvl, ZC_RendererSet* pRS)
{
    auto rendSetsIter = rendererSets.find(lvl);
    if (rendSetsIter == rendererSets.end()) rendererSets.emplace(lvl, std::map<ZC_ShProg*, std::forward_list<ZC_RendererSet*>> { { pRS->GetShProg(), { pRS } } });
    else
    {
        auto rendSetsShPIter = rendSetsIter->second.find(pRS->GetShProg());
        if (rendSetsShPIter == rendSetsIter->second.end()) rendSetsIter->second.emplace(pRS->GetShProg(), std::forward_list<ZC_RendererSet*>{ pRS });
        else rendSetsShPIter->second.emplace_front(pRS);
    }
}
    
void ZC_Renderer::Erase(RSLevel lvl, ZC_RendererSet* pRS)
{
    auto rendSetsIter = rendererSets.find(lvl);
    if (rendSetsIter != rendererSets.end())
    {
        auto rendSetsShPIter = rendSetsIter->second.find(pRS->GetShProg());
        if (rendSetsShPIter != rendSetsIter->second.end())
        {
            if (ZC_ForwardListErase(rendSetsShPIter->second, pRS))
            {
                if (rendSetsShPIter->second.empty())
                {
                    rendSetsIter->second.erase(rendSetsShPIter);
                    if (rendSetsIter->second.empty()) rendererSets.erase(rendSetsIter);
                }
            }
            else ZC_ErrorLogger::Err("Can't find ZC_RendererSet* in forward list of rendSets for erasing!", __FILE__, __LINE__);
        }
        else ZC_ErrorLogger::Err("Can't find map of ZC_ShProg* in rendSets for erasing!", __FILE__, __LINE__);
    }
    else ZC_ErrorLogger::Err("For erasing can't find in rendSets ZC_RendererSet::Level: " + std::to_string(lvl), __FILE__, __LINE__);
}

void ZC_Renderer::DrawDrawing(std::map<ZC_ShProg*, std::forward_list<ZC_RendererSet*>>& mapShPRS)
{
    for (auto pairShPRS : mapShPRS)
    {
        if(pairShPRS.first) pairShPRS.first->UseProgram();
        for (auto pRendSet : pairShPRS.second) 
            pRendSet->Draw(RSLevel::Drawing);
    }
}

void ZC_Renderer::DrawStencil(std::map<ZC_ShProg*, std::forward_list<ZC_RendererSet*>>& mapShPRS)
{
    bufferCleaner.GlEnable(GL_STENCIL_TEST);
    glStencilFunc(GL_ALWAYS, 1, 255);
    glStencilOp(GL_REPLACE, GL_REPLACE, GL_REPLACE);

    bufferCleaner.GlDisable(GL_DEPTH_TEST);
    pShPStencil->UseProgram();
    for (auto pairShPRS : mapShPRS)
        for (auto pRendSet : pairShPRS.second)
            pRendSet->Draw(RSLevel::Stencil);
    bufferCleaner.GlEnable(GL_DEPTH_TEST);

    // glStencilFunc(GL_ALWAYS, 1, 255);   //  ??
    glStencilOp(GL_KEEP, GL_KEEP, GL_REPLACE);
    for (auto pairShPRS : mapShPRS)
    {
        pairShPRS.first->UseProgram();
        for (auto pRendSet : pairShPRS.second) pRendSet->Draw(RSLevel::Stencil);
    }
    
    bufferCleaner.GlDisable(GL_STENCIL_TEST);
}

void ZC_Renderer::DrawImGui(std::map<ZC_ShProg*, std::forward_list<ZC_RendererSet*>>& mapShPRS)
{
    ZC_ImGui::FrameStart();
    for (auto pairShPRS : mapShPRS)
    {
        if(pairShPRS.first) pairShPRS.first->UseProgram();
        for (auto pRendSet : pairShPRS.second) pRendSet->Draw(RSLevel::Drawing);
    }
    ZC_ImGui::FrameEnd();
}