#pragma once

#include <ZC/GUI/ZC_GUI_Window.h>
#include <ZC/Events/ZC_ButtonID.h>

#include <list>

struct ZC_GUI_EventManager
{
    bool isActive = true;  //  active mouse event manager or not

    ZC_GUI_Obj* pObj_underCursor = nullptr; //  object under cursor now
    ZC_GUI_Obj* pObj_pressed = nullptr;     //  object pressed now (uses to end object's events, if window close)  
    ZC_GUI_Obj* pObj_scroll = nullptr;      //  object used on scroll
    ZC_GUI_Obj* pObj_cursorMove = nullptr;  //  object pressed by left button mouse (uses for objects that wish to save pressed state until unpress in same area, and if unpress in othe area then discard event)
    ZC_GUI_Obj* pObj_mouseButtonDown_watcher = nullptr;     //  object wich need to know about down left/right mouse buttons

    std::list<ZC_GUI_Window*> stacionarWins;
    std::list<ZC_GUI_Window*> openableWins;

    ZC_GUI_Window* pTextInputWindow = nullptr;

    void Configure();
    void ChangeActivity(bool active);
    bool IsActiveEventManager();
    void SetPressedObj(ZC_GUI_Obj* _pObj_pressed);
    void SetCursorMoveObj(ZC_GUI_Obj* _pObj_cursorMove);
    void SetMouseButtonDownWatcherObj(ZC_GUI_Obj* _pObj_mouseButtonDown_watcher);
    void AddWindow(ZC_GUI_Window* pWindow);
    void EraseWindow(ZC_GUI_Window* pWindow);
    bool IsWindowFocused(ZC_GUI_Obj* pWindow);
        //  update openable windows depthes
    void UpdateDepths();

    void UpdateWindowState(ZC_GUI_Window* pWindow);
        //  if pObj belongs to the pClosedWindow, call object's close window event
    void StopObjEventActivity(ZC_GUI_Obj*& rpObj);
    void UpdateCursorCollision();

        //  returns true if other events may use cursore move event
    bool CursorMove(float x, float y, float rel_x, float rel_y, float time);
        //  returns true if other events may use cursore move event
    bool CursorMoveOnceInFrame(float x, float y, float rel_x, float rel_y, float time);
        //  returns true if other events may use scroll wheel event
    bool ScrollWheel();
        //  returns true if other events may use scroll wheel event
    bool ScrollWheelOnceInFrame(float verticalScroll, float time);
        //  returns a pointer to the GUI object that this event belongs to, or nullptr
    ZC_GUI_Obj* GetButtonDownObject(ZC_ButtonID buttonID);
        //  free event pointers (calls from ZC_Obj::ButtonUp())
    void MouseButtonUp();
};