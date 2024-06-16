#pragma once

#include <ZC/GUI/ZC_GUI_Window.h>

#include <list>

struct ZC_GUI_EventManager
{
    ZC_GUI_Window* pWin_underCursor = nullptr;
    ZC_GUI_EventObj* pEO_underCursor = nullptr;
    ZC_GUI_EventObj* pEO_pressedLBM = nullptr;  //  object pressed by left button mouse (uses for objects that wish to save pressed state until unpress in same area, and if unpress in othe area then discard event)

    std::list<ZC_GUI_EventObj*> stacionarWins;
    std::list<ZC_GUI_EventObj*> openableWins;

    bool isActive = false;  //  active mouse event manager or not

    void AddWindow(bool isStacionar, ZC_GUI_EventObj* pWindow);

    void EraseWindow(bool isStacionar, ZC_GUI_EventObj* pWindow);

    //  returns true if other events may use cursore move event
    bool CursoreMove(float x, float y, float rel_x, float rel_y, float time);

    void ChangeActivity(bool active);
};
