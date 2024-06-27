#pragma once

#include <ZC/Video/OpenGL/Shader/ZC_Pipeline.h>
#include <ZC/Video/OpenGL/Texture/ZC_Texture.h>
#include <ZC/GUI/ZC_GUI_Window.h>

#include <list>

struct ZC_GUI_DrawManager
{
    std::list<ZC_GUI_Window*> mutableWins;
    ZC_GUI_Window* pImmutableWin = nullptr;   //  no metter wich of immutable wins, uses only for call virtual methods that work with static buffers

        //  DELETE
    uint vao;

    ZC_Pipeline* pPipeline = nullptr;
    ZC_Texture texture;

    bool Init();
    void AddWindow(ZC_GUI_Window* pWindow);
    void EraseWindow(ZC_GUI_Window* pWindow);
    void Configure();

    void Draw();
    void UpdateWindowDrawState(ZC_GUI_Window* pWindow);
};