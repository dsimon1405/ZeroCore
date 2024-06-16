#pragma once

#include <ZC/Video/OpenGL/Shader/ZC_Pipeline.h>
#include <ZC/Video/OpenGL/Texture/ZC_Texture.h>
#include <ZC/GUI/ZC_GUI_EventManager.h>
#include <ZC/GUI/ZC_GUI_Window.h>

#include <list>

struct ZC_GUI
{
        //  DELETE
    uint vao;

    static inline ZC_GUI* pGUI = nullptr;
    
    ZC_Pipeline* pPipeline = nullptr;
    ZC_Texture texture;
    std::list<ZC_GUI_Window*> windows;
    ZC_GUI_EventManager eventManager;

    bool Init();

    static void AddWindow(ZC_GUI_Window* pWindow);

    static void EraseWindow(ZC_GUI_Window* pWindow);

    void Configure();

    void Draw();
};
