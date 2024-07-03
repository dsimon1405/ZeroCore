#include <ZC/GUI/ZC_GUI.h>

bool ZC_GUI::Init()
{
    if (!textManager.Init()) return false;
    if (!drawManager.Init()) return false;
    eventManager.ChangeActivity(true);
    pGUI = this;
    return true;
}

void ZC_GUI::Configure()
{
    textManager.Configure();    //  must be before drawManager.Configure(), must calculate uv for ZC_GUI_Text objects
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

void ZC_GUI::UpdateWindowDrawState(ZC_GUI_Window* pWindow)
{
    if (!pGUI) return;
    pGUI->drawManager.UpdateWindowDrawState(pWindow);
    pGUI->eventManager.UpdateWindowState(pWindow);
}