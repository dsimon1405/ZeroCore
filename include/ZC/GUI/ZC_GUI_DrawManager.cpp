#include <ZC/GUI/ZC_GUI_DrawManager.h>

#include <ZC/Video/OpenGL/Shader/ZC_ShaderManager.h>
#include <ZC/Video/OpenGL/Shader/ZC_ShaderPath.h>
#include <ZC/GUI/ZC_GUI_Bindings.h>

#include <cassert>

bool ZC_GUI_DrawManager::Init()
{
    texture = ZC_Texture::LoadTexture2D("/home/dmitry/Загрузки/icons/icons.png", ZC_GUI_Bindings::bind_Texture);
    
    assert(ZC_ShaderPrograms::CompilationTest(ZC_ShaderPath::GetPath_Vertex(ZC_SV_gui).c_str(), nullptr, nullptr, ZC_ShaderPath::GetPath_Geometry(ZC_SG_gui).c_str(),
        ZC_ShaderPath::GetPath_Fragment(ZC_SF_gui).c_str()));
    if (!(ZC_ShaderManager::LoadShaderProgram({ .path = ZC_ShaderPath::GetPath_Vertex(ZC_SV_gui), .customID = ZC_SV_gui }, GL_VERTEX_SHADER)
        && ZC_ShaderManager::LoadShaderProgram({ .path = ZC_ShaderPath::GetPath_Geometry(ZC_SG_gui), .customID = ZC_SG_gui }, GL_GEOMETRY_SHADER)
        && ZC_ShaderManager::LoadShaderProgram({ .path = ZC_ShaderPath::GetPath_Fragment(ZC_SF_gui), .customID = ZC_SF_gui }, GL_FRAGMENT_SHADER)))
    {
        assert(false);
        return false;
    }

    pPipeline = ZC_ShaderManager::GetPipeLine(ZC_SV_gui, ZC_Shader_None, ZC_Shader_None, ZC_SG_gui, ZC_SF_gui);
    if (pPipeline == nullptr)
    {
        assert(false);   //  create pipeline fail
        return false;
    }

    glCreateVertexArrays(1, &vao);

    return true;
}

void ZC_GUI_DrawManager::AddWindow(ZC_GUI_Window* pWindow)
{
    if (pWindow->VIsMutable_W()) pWindow->VIsDrawing_Obj() ? mutableWins.emplace_front(pWindow) : mutableWins.emplace_back(pWindow);
    else pImmutableWin = pWindow;
}

void ZC_GUI_DrawManager::EraseWindow(ZC_GUI_Window* pWindow)
{
    if (pWindow->VIsMutable_W()) std::erase(mutableWins, pWindow);
}

void ZC_GUI_DrawManager::Configure()
{
    for (auto pWinObj : mutableWins) pWinObj->VConfigure_Obj();   //  in ZC_GUI_WinMutable create window, in ZC_GUI_WinImmutable only will total buffer
    if (pImmutableWin) pImmutableWin->VConfigure_Obj();
}

void ZC_GUI_DrawManager::Draw()
{
    glUseProgram(0);
    glBindVertexArray(vao);
    pPipeline->GLBindProgramPipeline();
    texture.GLBindTextureUnit();
    for (auto pWinObj : mutableWins)
    {
        if (!pWinObj->VIsDrawing_Obj()) break; //  first come the drawable windows, if the non-drawable ones are reached, out
        pWinObj->VDraw_W();
    }
    if (pImmutableWin) pImmutableWin->VDraw_W();
}

void ZC_GUI_DrawManager::UpdateWindowDrawState(ZC_GUI_Window* pWindow)
{
    if (!pWindow->VIsMutable_W()) return;   //  change only mutable list
    if (pWindow->VIsDrawing_Obj() ? mutableWins.front() == pWindow : mutableWins.back() == pWindow) return; //  pWindow allready on that position
    std::erase(mutableWins, pWindow);
    pWindow->VIsDrawing_Obj() ? mutableWins.emplace_front(pWindow) : mutableWins.emplace_back(pWindow);
}