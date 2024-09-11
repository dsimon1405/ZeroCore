#pragma once

#include <ZC/GUI/Backend/ZC_GUI_DrawManager.h>
#include <ZC/GUI/Backend/ZC_GUI_EventManager.h>
#include <ZC/GUI/Backend/Text/ZC_GUI_TextManager.h>
#include <ZC/GUI/Backend/Window/ZC_GUI_TextInputWindow.h>

struct ZC_GUI
{
    static inline ZC_GUI* pGUI = nullptr;

    ZC_GUI_TextManager textManager;
    ZC_GUI_DrawManager drawManager;
    ZC_GUI_EventManager eventManager;

    ZC_GUI_TextInputWindow textInputWindow{ float(textManager.font.GetHeight()) };

    ZC_GUI();

    void Configure();

    void Draw();

    static void AddWindow(ZC_GUI_Window* pWindow);
    static void EraseWindow(ZC_GUI_Window* pWindow);
        //  Updates draw state of the window in the gui draw and event systems. pWindow->VIsDraw() -> MUST return actual state! Also if necessary calls pWindow->SetFocuseDepth().
    static void UpdateWindowDrawState(ZC_GUI_Obj* pWindow);
};
