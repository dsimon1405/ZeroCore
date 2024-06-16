#include <ZC/GUI/ZC_GUI.h>

#include <ZC/Video/OpenGL/Shader/ZC_ShaderManager.h>
#include <ZC/Video/OpenGL/Shader/ZC_ShaderPath.h>

#include <cassert>

bool ZC_GUI::Init()
{
    texture = ZC_Texture::LoadTexture2D("/home/dmitry/Загрузки/awesomeface.png");
    
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

    pGUI = this;

    glCreateVertexArrays(1, &vao);

    eventManager.ChangeActivity(true);
    return true;
}

void ZC_GUI::AddWindow(ZC_GUI_Window* pWindow)
{
    if (!pGUI) return;
    pWindow->VIsMutable_W() ? pGUI->windows.emplace_front(pWindow) : pGUI->windows.emplace_back(pWindow);
    pGUI->eventManager.AddWindow(pWindow->IsStacionar(), pWindow);
}

void ZC_GUI::EraseWindow(ZC_GUI_Window* pWindow)
{
    if (!pGUI) return;
    std::erase(pGUI->windows, pWindow);
    pGUI->eventManager.EraseWindow(pWindow->IsStacionar(), pWindow);
}

void ZC_GUI::Configure()
{
    for (auto pWinObj : windows) pWinObj->VConfigureWindow_W();   //  in ZC_GUI_WinMutable create window, in ZC_GUI_WinImmutable only will total buffer
    // ZC_GUI_Window::ConfigureImmutableWindow();  //  create windows in ZC_GUI_WinImmutable
}

void ZC_GUI::Draw()
{
    glUseProgram(0);
    glBindVertexArray(vao);
    pPipeline->GLBindProgramPipeline();
    texture.GLBindTextureUnit(0);
    for (auto pWinObj : windows)
    {
        if (!(pWinObj->VIsMutable_W())) break; //  first in list ZC_GUI_WinMutable, when all they drawn, exit the loop and call the static function to drawing ZC_GUI_WinImmutable
        pWinObj->VDrawMutable_W();
    }
    // ZC_GUI_Window::DrawImmutable();
}