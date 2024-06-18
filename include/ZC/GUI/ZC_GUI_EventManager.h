#pragma once

#include <ZC/GUI/ZC_GUI_Window.h>
#include <ZC/Events/ZC_ButtonID.h>

#include <list>

struct ZC_GUI_EventManager
{
    bool isActive = false;  //  active mouse event manager or not

    ZC_GUI_Window* pWin_underCursor = nullptr;  //  under cursor now
    ZC_GUI_EventObj* pEO_underCursor = nullptr; //  under cursor now

    ZC_GUI_EventObj* pEO_cursorMove = nullptr;  //  object pressed by left button mouse (uses for objects that wish to save pressed state until unpress in same area, and if unpress in othe area then discard event)

    std::list<ZC_GUI_Window*> stacionarWins;
    std::list<ZC_GUI_Window*> openableWins;

    struct MouseButton
    {
        ZC_ButtonID buttonID;
        bool buttonPressed = false;
        ZC_GUI_EventObj* pPressed = nullptr;
            //  return true if button may be used in other events
        bool ButtonDown(ZC_ButtonID _buttonID, ZC_GUI_EventObj* pWin, ZC_GUI_EventObj* pEO, float time);
            //  return true if button may be used in other events
        bool ButtonUp(ZC_ButtonID _buttonID, float time);
    };
    MouseButton mbLeft { .buttonID = ZC_ButtonID::M_LEFT };
    MouseButton mbRight { .buttonID = ZC_ButtonID::M_RIGHT };


    void ChangeActivity(bool active);
    void AddWindow(ZC_GUI_Window* pWindow);
    void EraseWindow(ZC_GUI_Window* pWindow);

    void UpdateWindowDrawState(ZC_GUI_Window* pWindow);
    void UpdateCursorCollision();

        //  returns true if other events may use cursore move event
    bool CursoreMove(float x, float y, float rel_x, float rel_y, float time);
        //  returns true if other events may use button down event
    bool ButtonDown(ZC_ButtonID buttonID, float time);
        //  returns true if other events may use button down event
    bool ButtonUp(ZC_ButtonID buttonID, float time);

    // void SetCursorMoveObject()
};
