#include "ZC_GUI_DrawManager.h"

#include <ZC/GUI/Backend/Config/ZC_GUI_Bindings.h>
#include <ZC/File/ZC_File.h>
#include <ZC/Video/OpenGL/Shader/ZC_Shader.h>

#include <cassert>

ZC_GUI_DrawManager::ZC_GUI_DrawManager()
{
    textures.reserve(2ull);
    textures.emplace_back(path_to_custom_icon_texture.empty() ?
        ZC_Texture::LoadTexture2D(ZC_FSPath(ZC_ZCDirPath).append("textures/gui/icons.png").string().c_str(), 0, false)
        : ZC_Texture::LoadTexture2D(path_to_custom_icon_texture.c_str(), ZC_GUI_Bindings::location_tex_Icons, false));
    
    textures.emplace_back(ZC_Texture());    //  text texture default
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

ZC_Texture* ZC_GUI_DrawManager::GetTextTexture()
{
    return &(textures[1]);
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