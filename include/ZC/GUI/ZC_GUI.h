#pragma once

#include <ZC/GUI/ZC_GUI_DrawManager.h>
#include <ZC/GUI/ZC_GUI_EventManager.h>

struct ZC_GUI
{
    static inline ZC_GUI* pGUI = nullptr;

    ZC_GUI_DrawManager drawManager;
    ZC_GUI_EventManager eventManager;

    bool Init();
    void Configure();

    void Draw();

    static void AddWindow(ZC_GUI_Window* pWindow);
    static void EraseWindow(ZC_GUI_Window* pWindow);
    //  Updates draw state of the window in the gui draw and event systems. pWindow->VIsDraw() -> MUST return actual state! Also if necessary calls pWindow->SetFocuseDepth().
    static void UpdateWindowDrawState(ZC_GUI_Window* pWindow);
    //  Updates gui event system and call pWindow->SetFocuseDepth(). 
    static void MakeWindowFocused(ZC_GUI_Window* pWindow);
};
