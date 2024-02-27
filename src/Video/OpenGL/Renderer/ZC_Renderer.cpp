#include <ZC/Video/OpenGL/Renderer/ZC_Renderer.h>

#include <Video/OpenGL/ZC_OpenGL.h>
#include <ZC/Video/OpenGL/Buffer/ZC_UBOs.h>
#include <ZC/Tools/Container/ZC_ContFunc.h>
#include <ZC/ErrorLogger/ZC_ErrorLogger.h>
#ifdef ZC_IMGUI
#include <Video/imgui/ZC_ImGui.h>
#endif

void ZC_Renderer::DrawAll()
{
    ZC_UBOs::Update();
    for (auto& rendSetsP : rendSets)
    {
        switch (rendSetsP.first)
        {
        case RSLevel::Drawing: Draw(rendSetsP.second); break;
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
    auto rendSetsIter = rendSets.find(lvl);
    if (rendSetsIter == rendSets.end())
    {
        rendSets.emplace(lvl, std::map<ZC_ShProg*, std::forward_list<ZC_RendererSet*>> { { pRS->GetShProg(), { pRS } } });
    }
    else
    {
        auto rendSetsShPIter = rendSetsIter->second.find(pRS->GetShProg());
        if (rendSetsShPIter == rendSetsIter->second.end())
        {
            rendSetsIter->second.emplace(pRS->GetShProg(), std::forward_list<ZC_RendererSet*>{ pRS });
        }
        else
        {
            rendSetsShPIter->second.emplace_front(pRS);
        }
    }
}
    
void ZC_Renderer::Erase(RSLevel lvl, ZC_RendererSet* pRS)
{
    auto rendSetsIter = rendSets.find(lvl);
    if (rendSetsIter != rendSets.end())
    {
        auto rendSetsShPIter = rendSetsIter->second.find(pRS->GetShProg());
        if (rendSetsShPIter != rendSetsIter->second.end())
        {
            if (ZC_ForwardListErase(rendSetsShPIter->second, pRS))
            {
                if (rendSetsShPIter->second.empty())
                {
                    rendSetsIter->second.erase(rendSetsShPIter);
                    if (rendSetsIter->second.empty())
                        rendSets.erase(rendSetsIter);
                }
            }
            else ZC_ErrorLogger::Err("Can't find ZC_RendererSet* in forward list of rendSets for erasing!", __FILE__, __LINE__);
        }
        else ZC_ErrorLogger::Err("Can't find map of ZC_ShProg* in rendSets for erasing!", __FILE__, __LINE__);
    }
    else ZC_ErrorLogger::Err("For erasing can't find in rendSets ZC_RendererSet::Level: " + std::to_string(lvl), __FILE__, __LINE__);
}

void ZC_Renderer::EnablePointSize()
{
    glEnable(GL_PROGRAM_POINT_SIZE);
}

void ZC_Renderer::SetStencilShP(ZC_ShProg* _shPStencil)
{
    pShPStencil = _shPStencil;
}

void ZC_Renderer::Draw(map<ZC_ShProg*, forward_list<ZC_RendererSet*>>& mapShPRS)
{
    for (auto pairShPRS : mapShPRS)
    {
        if(pairShPRS.first) pairShPRS.first->UseProgram();
        for (auto pRendSet : pairShPRS.second) pRendSet->Draw(RSLevel::Drawing);
    }
}

void ZC_Renderer::DrawStencil(map<ZC_ShProg*, forward_list<ZC_RendererSet*>>& mapShPRS)
{
    glEnable(GL_STENCIL_TEST);
    glStencilFunc(GL_ALWAYS, 1, 255);
    glStencilOp(GL_REPLACE, GL_REPLACE, GL_REPLACE);

    glDisable(GL_DEPTH_TEST);
    pShPStencil->UseProgram();
    for (auto pairShPRS : mapShPRS)
        for (auto pRendSet : pairShPRS.second) pRendSet->Draw(RSLevel::Stencil);
    glEnable(GL_DEPTH_TEST);

    // glStencilFunc(GL_ALWAYS, 1, 255);   //  ??
    glStencilOp(GL_KEEP, GL_KEEP, GL_REPLACE);
    for (auto pairShPRS : mapShPRS)
    {
        pairShPRS.first->UseProgram();
        for (auto pRendSet : pairShPRS.second) pRendSet->Draw(RSLevel::Stencil);
    }
    
    glDisable(GL_STENCIL_TEST);
}

void ZC_Renderer::DrawImGui(map<ZC_ShProg*, forward_list<ZC_RendererSet*>>& mapShPRS)
{
    ZC_ImGui::FrameStart();
    for (auto pairShPRS : mapShPRS)
    {
        if(pairShPRS.first) pairShPRS.first->UseProgram();
        for (auto pRendSet : pairShPRS.second) pRendSet->Draw(RSLevel::Drawing);
    }
    ZC_ImGui::FrameEnd();
}