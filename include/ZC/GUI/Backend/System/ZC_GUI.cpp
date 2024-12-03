#include "ZC_GUI.h"

ZC_GUI::ZC_GUI()
    : drawManager(),
    textManager(drawManager.GetTextTexture())
{
    pGUI = this;
    drawManager.AddWindow(dynamic_cast<ZC_GUI_Window*>(&textInputWindow));     //  here adds only ZC_GUI_TextInputWinodw caurse pGUI unseted while it trys to add from ZC_GUI_WinImmutable ctr
    eventManager.AddWindow(dynamic_cast<ZC_GUI_Window*>(&textInputWindow));     //  here adds only ZC_GUI_TextInputWinodw caurse pGUI unseted while it trys to add from ZC_GUI_WinImmutable ctr
}

ZC_GUI::~ZC_GUI()
{
    pGUI = nullptr;
}

void ZC_GUI::Configure()
{
    textManager.Configure();    //  must be before drawManager.Configure(), must calculate uv for ZC_GUI_Text objects
    eventManager.Configure();   //  must be before drawManager to unset color_focused on unfocused windows
    drawManager.Configure();
}

void ZC_GUI::SetState(bool active)
{
    if (!pGUI || pGUI->drawManager.isActive == active) return;
    pGUI->drawManager.isActive = active;
    pGUI->eventManager.isActive = active;
}

bool ZC_GUI::GetState()
{
    return pGUI->drawManager.isActive;
}

void ZC_GUI::AddWindow(ZC_GUI_Window* pWindow)
{
    if (!pGUI) return;
    pGUI->drawManager.AddWindow(pWindow);
    pGUI->eventManager.AddWindow(pWindow);
}

void ZC_GUI::EraseWindow(ZC_GUI_Window* pWindow)
{
    if (!pGUI) return;
    pGUI->drawManager.EraseWindow(pWindow);
    pGUI->eventManager.EraseWindow(pWindow);
}

void ZC_GUI::UpdateWindowDrawState(ZC_GUI_Obj* pWindow)
{
    if (!pGUI) return;
    pGUI->drawManager.UpdateWindowDrawState(dynamic_cast<ZC_GUI_Window*>(pWindow));
    pGUI->eventManager.UpdateWindowState(dynamic_cast<ZC_GUI_Window*>(pWindow));
}