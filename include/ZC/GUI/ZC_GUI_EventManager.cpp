#include <ZC/GUI/ZC_GUI_EventManager.h>

#include <ZC/Video/ZC_SWindow.h>
#include <Events/Button/ZC_ButtonPressedDown.h>

#include <cassert>

void ZC_GUI_EventManager::Configure()
{
    for (auto pOpenableWin : openableWins)  //  on start all drawn windows sets as focused, make them unfocused
        if (pOpenableWin != openableWins.front()) pOpenableWin->MakeUnfocused();    //  first window in the list stands focused (if it's drawing...)
}

void ZC_GUI_EventManager::ChangeActivity(bool active)
{
    if (active == isActive) return;
    isActive = active;
    if (isActive) UpdateCursorCollision();  //  set up event manager state
    else
    {   //  disable active objects
        bool sameObjs = pObj_pressed == pObj_underCursor;
        if (pObj_pressed) StopObjEventActivity(pObj_pressed);
        pObj_cursorMove = nullptr;  //  same pointer as pObj_pressed
        if (sameObjs) pObj_underCursor = nullptr;   //  same pointer as pObj_pressed
        else if (pObj_underCursor) StopObjEventActivity(pObj_underCursor);
        pObj_scroll = nullptr;  //  same pointer as pObj_underCursor
    }
}

bool ZC_GUI_EventManager::IsActiveEventManager()
{
    return isActive;
}

void ZC_GUI_EventManager::SetPressedObj(ZC_GUI_Obj* _pObj_pressed)
{
    pObj_pressed = _pObj_pressed;
}

void ZC_GUI_EventManager::SetCursorMoveObj(ZC_GUI_Obj* _pObj_cursorMove)
{
    pObj_cursorMove = _pObj_cursorMove;
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

bool ZC_GUI_EventManager::IsWindowFocused(ZC_GUI_Obj* pWindow)
{
    return !(pWindow->VIsStacionar_Obj()) && pWindow->VIsDrawing_Obj() ? openableWins.front() == pWindow : false;
}

void ZC_GUI_EventManager::UpdateWindowState(ZC_GUI_Window* pWindow)
{       //    return true, if was changed list
    auto lambResetWindow = [pWindow](std::list<ZC_GUI_Window*>& rWins, bool empl_front)
    {
        if (empl_front ? rWins.front() == pWindow : rWins.back() == pWindow) return false;
        std::erase(rWins, pWindow);
        if (empl_front)
        {
            rWins.front()->MakeUnfocused(); //  make previous window unfocused
            rWins.emplace_front(pWindow);   //  set new window in list's front
        }
        else rWins.emplace_back(pWindow);
        return true;
    };
    
    if (pWindow->VIsDrawing_Obj())    //  window is drawing, set it to the begin of list
    {
        if (!(pWindow->VIsStacionar_Obj()) && pObj_pressed)   //  opens openable window, stop all activity of previous window
        {
            ZC_ButtonPressedDown::DisablePressedButton(pObj_pressed);
            StopObjEventActivity(pObj_pressed);
            pObj_cursorMove = nullptr;    //  must be before UpdateCursorCollision()
        }
        if (lambResetWindow(pWindow->VIsStacionar_Obj() ? stacionarWins : openableWins, true)) UpdateCursorCollision();     //  set window to the front of list, and update cursor position
        pWindow->SetFocuseDepthAndColor();
    }
    else    //  window is non-drawing, set it to the end of list
    {
        if (pObj_pressed)   //  if event belong to current window disable pointer to them
        {
            ZC_GUI_Obj* pFocusedWindow = pObj_pressed->GetWindow();
            if (pWindow == pFocusedWindow)  //  objects belong to the closing window
            {   //  pObj_pressed and pObj_cursorMove if not null then same pointers
                ZC_ButtonPressedDown::DisablePressedButton(pObj_pressed);
                StopObjEventActivity(pObj_pressed);
                pObj_cursorMove = nullptr;    //  must be before UpdateCursorCollision()
            }
        }
        if (lambResetWindow(pWindow->VIsStacionar_Obj() ? stacionarWins : openableWins, false)) UpdateCursorCollision();    //  set window to the back of list, and update cursor position
        pWindow->SetFocuseDepthAndColor();
    }
}

void ZC_GUI_EventManager::StopObjEventActivity(ZC_GUI_Obj*& rpObj)
{
    rpObj->VStopEventActivity_Obj();
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
        ZC_GUI_Obj* pObj_temp = nullptr;
        for (ZC_GUI_Window* pWindow : windows)  //  find new collision window and object
        {   //  collision only with drawing windows, all drawing window in front of both lists (openableWins, stacionarWins)
            if (!pWindow->VIsDrawing_Obj()) break;  //  reach not drawing windows
            if (pWindow->VMakeCursorCollision_Obj(x, y, pObj_temp, pObj_scroll))
            {
                if (pObj_temp != pObj_underCursor)
                {
                    if (pObj_underCursor) pObj_underCursor->VCursorCollisionEnd_Obj(time);
                    if (pObj_temp) pObj_temp->VCursorCollisionStart_Obj(time);
                    pObj_underCursor = pObj_temp;
                }
                return false;
            }
        }   //  not found window and object, make current window and object nullptr
        if (pObj_underCursor)
        {
            pObj_underCursor->VCursorCollisionEnd_Obj(time);
            pObj_underCursor = nullptr;
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
    // else if (pWin_underCursor) return !(pWin_underCursor->IsBackground());  //  if have background hold scroll event
    return true;
}

bool ZC_GUI_EventManager::ScrollWheelOnceInFrame(float verticalScroll, float time)
{
    if (!isActive || !pObj_scroll) return true;
    pObj_scroll->VScroll_Obj(verticalScroll, time);
    return false;
}

ZC_GUI_Obj* ZC_GUI_EventManager::GetButtonDownObject(ZC_ButtonID buttonID)
{
    if (!isActive) return nullptr;
    if (buttonID == ZC_ButtonID::M_LEFT || buttonID == ZC_ButtonID::M_RIGHT) return pObj_underCursor;
                                                //  create list for all object with keyboard buttom event, and find in it
    return nullptr;
}
 
void ZC_GUI_EventManager::MouseButtonUp()
{
    pObj_pressed = nullptr;     //  free pointer
    if (!pObj_cursorMove) return;
    pObj_cursorMove = nullptr;  //  free pointer, must be before UpdateCursorCollision()
    UpdateCursorCollision();    //  update cursor collision after left mouse button release
}


// //  ZC_GUI_EventManager::MouseButton

// bool ZC_GUI_EventManager::MouseButton::ButtonDown(ZC_ButtonID _buttonID, ZC_GUI_Window* pWin, ZC_GUI_Obj* pObj, float time, ZC_GUI_Obj*& rpObj_cursorMove)
// {
//     if (buttonID != _buttonID) return true;     //  wrong button
//     if (buttonPressed) return false;    //  button already pressed
//     buttonPressed = true;
//         //  returns false if object was not collisioned
//     auto lambCallButtonDown = [this, time, &rpObj_cursorMove](ZC_GUI_Obj* pEventObj)
//     {
//         if (!pEventObj) return false; //  no object
//         bool cursorMoveWhilePressed = false;    //  call button down
//         if (buttonID == ZC_ButtonID::M_LEFT ? pEventObj->VMouseButtonLeftDown_Obj(time, cursorMoveWhilePressed) : pEventObj->VRightButtonDown_Obj(time, cursorMoveWhilePressed))
//         {
//             pPressed = pEventObj;
//             if (cursorMoveWhilePressed)
//             {
//                 assert(!rpObj_cursorMove);   //  reset of active rpEO_cursorMove
//                 rpObj_cursorMove = pPressed;    //  set cursor move object
//             }
//             return true;
//         }
//         return false;
//     };
//     if (lambCallButtonDown(pObj)) pWin->MakeWindowFocused();      //  call object's button down, if object use that button (object exists), and window may only be focused,
//     else lambCallButtonDown(pWin);                          //  if object don't use button (or not exists at all), try window's button down
//     return pPressed == nullptr;
// }

// bool ZC_GUI_EventManager::MouseButton::ButtonUp(ZC_ButtonID _buttonID, float time, ZC_GUI_Obj*& rpObj_cursorMove)
// {
//     if (buttonID != _buttonID) return true;    //  wrong button
//     buttonPressed = false;
//     if (!pPressed) return true;     //  haven't pressed object
//     buttonID == ZC_ButtonID::M_LEFT ? pPressed->VMouseButtonLeftUp_Obj(time) : pPressed->VRightButtonUp_Obj(time);
//     if (rpObj_cursorMove == pPressed) rpObj_cursorMove = nullptr;     //  if move object is current button, unset cursor move object
//     pPressed = nullptr;
//     return false;
// }