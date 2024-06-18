#include <ZC/GUI/ZC_GUI.h>

bool ZC_GUI::Init()
{
    if (!drawManager.Init()) return false;
    eventManager.ChangeActivity(true);
    pGUI = this;
    return true;
}

void ZC_GUI::Configure()
{
    drawManager.Configure();
}

void ZC_GUI::Draw()
{
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
    pGUI->eventManager.UpdateWindowDrawState(pWindow);
}

void ZC_GUI::MakeWindowFocused(ZC_GUI_Window* pWindow)
{
    if (pGUI) pGUI->eventManager.UpdateWindowDrawState(pWindow);
}