#include <ZC/GUI/ZC_GUI.h>

ZC_GUI::ZC_GUI()
{
    pGUI = this;
    eventManager.AddWindow(dynamic_cast<ZC_GUI_Window*>(&textInputWindow));     //  here adds only ZC_GUI_TextInputWinodw caurse pGUI unseted while it trys to add from ZC_GUI_WinImmutable ctr
}

void ZC_GUI::Configure()
{
    textManager.Configure(false);    //  must be before drawManager.Configure(), must calculate uv for ZC_GUI_Text objects
    eventManager.Configure();   //  must be before drawManager to unset color_focused on unfocused windows
    drawManager.Configure();
}

void ZC_GUI::Draw()
{
    textManager.BindTextureUnit();
    drawManager.Draw();
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