#include <ZC/GUI/ZC_GUI_EventManager.h>

#include <ZC/Video/ZC_SWindow.h>

#include <cassert>

void ZC_GUI_EventManager::Configure()
{
    for (auto pOpenableWin : openableWins)  //  on start all drawn windows sets as focused, make them unfocused
        if (pOpenableWin != openableWins.front()) pOpenableWin->MakeUnfocused();    //  first window in the list stands focused (if it's drawing...)
}

void ZC_GUI_EventManager::ChangeActivity(bool active)
{
    isActive = active;
}

void ZC_GUI_EventManager::AddWindow(ZC_GUI_Window* pWindow)
{   //  choose stacionar window or openable, and if window drawing, add in front of list, otherwise in back
    if (pWindow->VIsStacionar_Obj()) pWindow->VIsDrawing_Obj() ? stacionarWins.emplace_front(pWindow) : stacionarWins.emplace_back(pWindow);
    else pWindow->VIsDrawing_Obj() ? openableWins.emplace_front(pWindow) : openableWins.emplace_back(pWindow);
}

void ZC_GUI_EventManager::EraseWindow(ZC_GUI_Window* pWindow)
{
    std::erase(pWindow->VIsStacionar_Obj() ? stacionarWins : openableWins, pWindow);
}

void ZC_GUI_EventManager::UpdateWindowState(ZC_GUI_Window* pWindow)
{       //    return true, if was changed list
    auto lambResetWindow = [pWindow](std::list<ZC_GUI_Window*>& rWins, bool empl_front)
    {
        if (empl_front ? rWins.front() == pWindow : rWins.back() == pWindow) return false;
        std::erase(rWins, pWindow);
        if (empl_front)
        {
            rWins.front()->MakeUnfocused();
            rWins.emplace_front(pWindow);
        }
        else rWins.emplace_back(pWindow);
        return true;
    };
    
    if (!pWindow->VIsDrawing_Obj())    //  window is non-drawing, set it to the end of list
    {
        DisableClosedWindowObj(pWindow, pObj_cursorMove);    //  must be called before UpdateCursorCollision()
        DisableClosedWindowObj(pWindow, mbLeft.pPressed);
        DisableClosedWindowObj(pWindow, mbRight.pPressed);
        if (lambResetWindow(pWindow->VIsStacionar_Obj() ? stacionarWins : openableWins, false)) UpdateCursorCollision();    //  set window to the back of list
    }
    else    //  window is drawing, set it to the begin of list
    {
        if (lambResetWindow(pWindow->VIsStacionar_Obj() ? stacionarWins : openableWins, true)) UpdateCursorCollision();     //  set window to the front of list
        pWindow->SetFocuseDepthAndColor();
    }
}

void ZC_GUI_EventManager::DisableClosedWindowObj(ZC_GUI_Window* pClosedWindow, ZC_GUI_Obj*& rpObj)
{
    if (!rpObj || pClosedWindow != rpObj->GetWindow()) return;  //  object does not exist (nullptr) or does not belong to the window being closed
    rpObj->VWindowStopDrawing_Obj();
    rpObj = nullptr;
}

void ZC_GUI_EventManager::UpdateCursorCollision()
{
    float x = 0.f, y = 0.f;
    ZC_SWindow::GetCursorPosition(x, y);
    CursorMove(x, y, 0.f, 0.f, 1.f);
}

bool ZC_GUI_EventManager::CursorMove(float x, float y, float rel_x, float rel_y, float time)
{
    if (!isActive) return true;
    if (pObj_cursorMove) return false;  //  have movable object, will be called in CursorMoveOnceInFrame()
    pObj_scroll = nullptr;   //  unset scroll object

        //  returns false, if under the cursor
    auto lambCollisionWindow = [this, x, y, time](std::list<ZC_GUI_Window*>& windows)
    {
        ZC_GUI_Obj* pWin_temp = nullptr;
        ZC_GUI_Obj* pObj_temp = nullptr;
        for (ZC_GUI_Window* pWindow : windows)  //  find new collision window and object
        {   //  collision only with drawing windows, all drawing window in front of both lists (openableWins, stacionarWins)
            if (!pWindow->VIsDrawing_Obj()) break;  //  reach not drawing windows
            if (pWindow->VMakeCursorCollision_Obj(x, y, pWin_temp, pObj_temp, pObj_scroll))
            {
                if (pObj_temp != pObj_underCursor)
                {
                    if (pObj_underCursor) pObj_underCursor->VCursorCollisionEnd_Obj(time);
                    if (pObj_temp) pObj_temp->VCursorCollisionStart_Obj(time);
                    pObj_underCursor = pObj_temp;
                }
                if (pWin_temp != pWin_underCursor)
                {
                    if (pWin_underCursor) pWin_underCursor->VCursorCollisionEnd_Obj(time);
                    if (pWin_temp) pWin_temp->VCursorCollisionStart_Obj(time);
                    pWin_underCursor = dynamic_cast<ZC_GUI_Window*>(pWin_temp);
                }
                return false;
            }
        }   //  not found window and object, make current window and object nullptr
        if (pObj_underCursor)
        {
            pObj_underCursor->VCursorCollisionEnd_Obj(time);
            pObj_underCursor = nullptr;
        }
        if (pWin_underCursor)
        {
            pWin_underCursor->VCursorCollisionEnd_Obj(time);
            pWin_underCursor = nullptr;
        }
        return true;
    };
        //  check window one by one, starting from openable (if one of labmdas retuns false, then window had cursor collision, so return false from method)
    return lambCollisionWindow(openableWins) && lambCollisionWindow(stacionarWins);
}

bool ZC_GUI_EventManager::CursorMoveOnceInFrame(float x, float y, float rel_x, float rel_y, float time)
{
    if (!isActive || !pObj_cursorMove) return true;
    pObj_cursorMove->VCursorMove_Obj(rel_x, rel_y);
    return false;
}

bool ZC_GUI_EventManager::ScrollWheel()
{
    if (!isActive) return true;
    else if (pObj_underCursor) return false;    //  some object under cursor, so and window under cursor, hold scroll event (no metter will it be used or not)
    else if (pWin_underCursor) return !(pWin_underCursor->IsBackground());  //  if have background hold scroll event
    return true;
}

bool ZC_GUI_EventManager::ScrollWheelOnceInFrame(float verticalScroll, float time)
{
    if (!isActive || !pObj_scroll) return true;
    pObj_scroll->VScroll_Obj(verticalScroll, time);
    return false;
}

bool ZC_GUI_EventManager::ButtonDown(ZC_ButtonID buttonID, float time)
{
    if (!isActive) return true;
    return mbLeft.ButtonDown(buttonID, pWin_underCursor, pObj_underCursor, time, pObj_cursorMove)
        && mbRight.ButtonDown(buttonID, pWin_underCursor, pObj_underCursor, time, pObj_cursorMove);
}
 
bool ZC_GUI_EventManager::ButtonUp(ZC_ButtonID buttonID, float time)
{
    if (!isActive) return true;
    bool isOtherMayUseUpEvent = mbLeft.ButtonUp(buttonID, time, pObj_cursorMove) && mbRight.ButtonUp(buttonID, time, pObj_cursorMove);
    UpdateCursorCollision();
    return isOtherMayUseUpEvent;
}


//  ZC_GUI_EventManager::MouseButton

bool ZC_GUI_EventManager::MouseButton::ButtonDown(ZC_ButtonID _buttonID, ZC_GUI_Window* pWin, ZC_GUI_Obj* pObj, float time, ZC_GUI_Obj*& rpObj_cursorMove)
{
    if (buttonID != _buttonID) return true;     //  wrong button
    if (buttonPressed) return false;    //  button already pressed
    buttonPressed = true;
        //  returns false if object was not collisioned
    auto lambCallButtonDown = [this, time, &rpObj_cursorMove](ZC_GUI_Obj* pEventObj)
    {
        if (!pEventObj) return false; //  no object
        bool cursorMoveWhilePressed = false;    //  call button down
        if (buttonID == ZC_ButtonID::M_LEFT ? pEventObj->VLeftButtonDown_Obj(time, cursorMoveWhilePressed) : pEventObj->VRightButtonDown_Obj(time, cursorMoveWhilePressed))
        {
            pPressed = pEventObj;
            if (cursorMoveWhilePressed)
            {
                assert(!rpObj_cursorMove);   //  reset of active rpEO_cursorMove
                rpObj_cursorMove = pPressed;    //  set cursor move object
            }
            return true;
        }
        return false;
    };
    if (lambCallButtonDown(pObj)) pWin->MakeForcused();      //  call object's button down, if object use that button (object exists), and window may only be focused,
    else lambCallButtonDown(pWin);                          //  if object don't use button (or not exists at all), try window's button down
    return pPressed == nullptr;     //  one of objectes override button event, and may became pressed
}

bool ZC_GUI_EventManager::MouseButton::ButtonUp(ZC_ButtonID _buttonID, float time, ZC_GUI_Obj*& rpObj_cursorMove)
{
    if (buttonID != _buttonID) return true;    //  wrong button
    buttonPressed = false;
    if (!pPressed) return true;     //  haven't pressed object
    buttonID == ZC_ButtonID::M_LEFT ? pPressed->VLeftButtonUp_Obj(time) : pPressed->VRightButtonUp_Obj(time);
    if (rpObj_cursorMove == pPressed) rpObj_cursorMove = nullptr;     //  if move object is current button, unset cursor move object
    pPressed = nullptr;
    return false;
}