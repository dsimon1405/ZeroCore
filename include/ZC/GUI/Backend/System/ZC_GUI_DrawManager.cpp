#include "ZC_GUI_DrawManager.h"

// #include <ZC/Video/OpenGL/Shader/ZC_ShaderManager.h>
// #include <ZC/Video/OpenGL/Shader/ZC_ShaderPath.h>
#include <ZC/GUI/Backend/Config/ZC_GUI_Bindings.h>
#include <ZC/File/ZC_File.h>
#include <ZC/Video/OpenGL/Shader/ZC_Shader.h>

#include <cassert>

ZC_GUI_DrawManager::ZC_GUI_DrawManager()
    : shProg(CreateShProg())
{
    texture = path_to_custom_icon_texture.empty() ?
        ZC_Texture::LoadTexture2D(ZC_FSPath(ZC_ZCDirPath).append("textures/gui/icons.png").string().c_str(), 0)
        : ZC_Texture::LoadTexture2D(path_to_custom_icon_texture.c_str(), ZC_GUI_Bindings::location_tex_Icons);

        //  bind texture locations
    shProg.ActivateOpenGL();
    glUniform1i(ZC_GUI_Bindings::location_tex_Icons, 0);    //  0 bind to GL_TEXTURE0, texture in current class
    glUniform1i(ZC_GUI_Bindings::location_tex_Text, 1);     //  1 bind to GL_TEXTURE1, texture in the ZC_GUI_TextManager


    // assert(ZC_ShaderPrograms::CompilationTest(ZC_ShaderPath::GetPath_Vertex(ZC_SV_gui).c_str(), nullptr, nullptr, ZC_ShaderPath::GetPath_Geometry(ZC_SG_gui).c_str(),
    //     ZC_ShaderPath::GetPath_Fragment(ZC_SF_gui).c_str()));
    // if (!(ZC_ShaderManager::LoadShaderProgram({ .path = ZC_ShaderPath::GetPath_Vertex(ZC_SV_gui), .customID = ZC_SV_gui }, GL_VERTEX_SHADER)
    //     && ZC_ShaderManager::LoadShaderProgram({ .path = ZC_ShaderPath::GetPath_Geometry(ZC_SG_gui), .customID = ZC_SG_gui }, GL_GEOMETRY_SHADER)
    //     && ZC_ShaderManager::LoadShaderProgram({ .path = ZC_ShaderPath::GetPath_Fragment(ZC_SF_gui), .customID = ZC_SF_gui }, GL_FRAGMENT_SHADER)))
    // {
    //     assert(false);
    //     return;
    // }

    // pPipeline = ZC_ShaderManager::GetPipeLine(ZC_SV_gui, ZC_Shader_None, ZC_Shader_None, ZC_SG_gui, ZC_SF_gui);
    // if (pPipeline == nullptr)
    // {
    //     assert(false);   //  create pipeline fail
    //     return;
    // }

    glCreateVertexArrays(1, &vao);
}

void ZC_GUI_DrawManager::AddWindow(ZC_GUI_Window* pWindow)
{
    if (pWindow->VIsMutableWin_Obj()) pWindow->VIsDrawing_Obj() ? mutableWins.emplace_front(pWindow) : mutableWins.emplace_back(pWindow);
    else pImmutableWin = pWindow;
}

void ZC_GUI_DrawManager::EraseWindow(ZC_GUI_Window* pWindow)
{
    if (pWindow->VIsMutableWin_Obj()) std::erase(mutableWins, pWindow);
}

void ZC_GUI_DrawManager::Configure()
{
    for (auto pWinObj : mutableWins) pWinObj->VConfigure_Obj();   //  in ZC_GUI_WinMutable create window, in ZC_GUI_WinImmutable only will total buffer
    if (pImmutableWin) pImmutableWin->VConfigure_Obj();
}

void ZC_GUI_DrawManager::Reconf_UpdateTextUV()
{
    for (auto pWinObj : mutableWins) pWinObj->VReconf_UpdateTextUV_W();
    if (pImmutableWin) pImmutableWin->VReconf_UpdateTextUV_W();
}

void ZC_GUI_DrawManager::Draw(const ZC_Texture& text_texture)
{
    if (!isActive) return;
    
    // texture.GLBindTextureUnit();
    texture.GLActivateAndBind(GL_TEXTURE0);
    text_texture.GLActivateAndBind(GL_TEXTURE1);
    
    glBindVertexArray(vao);
    // pPipeline->GLBindProgramPipeline();
    shProg.ActivateOpenGL();
    for (auto pWinObj : mutableWins)
    {
        if (!pWinObj->VIsDrawing_Obj()) break; //  first come the drawable windows, if the non-drawable ones are reached, out
        pWinObj->VDraw_W();
    }
    if (pImmutableWin) pImmutableWin->VDraw_W();
}

void ZC_GUI_DrawManager::UpdateWindowDrawState(ZC_GUI_Window* pWindow)
{
    if (!(pWindow->VIsMutableWin_Obj())) return;   //  change only mutable list
    if (pWindow->VIsDrawing_Obj() ? mutableWins.front() == pWindow : mutableWins.back() == pWindow) return; //  pWindow allready on that position
    std::erase(mutableWins, pWindow);
    pWindow->VIsDrawing_Obj() ? mutableWins.emplace_front(pWindow) : mutableWins.emplace_back(pWindow);
}

void ZC_GUI_DrawManager::SetPathToCustomIconTexture(std::string&& path)
{
    path_to_custom_icon_texture = std::move(path);
}

ZC_ShProg ZC_GUI_DrawManager::CreateShProg()
{
    ZC_Shader vs(ZC_Shader::ReadShaderFile(ZC_FSPath(ZC_ZCDirPath).append("shaders/GUI/gui.vs").string().c_str(), 0).pHead, GL_VERTEX_SHADER);
    ZC_Shader gs(ZC_Shader::ReadShaderFile(ZC_FSPath(ZC_ZCDirPath).append("shaders/GUI/gui.gs").string().c_str(), 0).pHead, GL_GEOMETRY_SHADER);
    ZC_Shader fs(ZC_Shader::ReadShaderFile(ZC_FSPath(ZC_ZCDirPath).append("shaders/GUI/gui.fs").string().c_str(), 0).pHead, GL_FRAGMENT_SHADER);
    return  ZC_ShProg(vs.id, fs.id, gs.id);
}