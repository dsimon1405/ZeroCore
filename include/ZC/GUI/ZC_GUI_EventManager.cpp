#include <ZC/GUI/ZC_GUI_EventManager.h>

#include <ZC/Video/ZC_SWindow.h>

#include <cassert>

void ZC_GUI_EventManager::ChangeActivity(bool active)
{
    isActive = active;
}

void ZC_GUI_EventManager::AddWindow(ZC_GUI_Window* pWindow)
{   //  choose stacionar window or openable, and if window drawing, add in front of list, otherwise in back
    if (pWindow->IsStacionar()) pWindow->VIsDrawing_W() ? stacionarWins.emplace_front(pWindow) : stacionarWins.emplace_back(pWindow);
    else pWindow->VIsDrawing_W() ? openableWins.emplace_front(pWindow) : openableWins.emplace_back(pWindow);
}

void ZC_GUI_EventManager::EraseWindow(ZC_GUI_Window* pWindow)
{
    std::erase(pWindow->IsStacionar() ? stacionarWins : openableWins, pWindow);
}

void ZC_GUI_EventManager::UpdateWindowState(ZC_GUI_Window* pWindow)
{       //    return true, if was changed list
    auto lambResetWindow = [pWindow](std::list<ZC_GUI_Window*>& rWins, bool empl_front)
    {
        if ((empl_front && rWins.front() == pWindow) || (!empl_front && rWins.back() == pWindow)) return false;
        std::erase(rWins, pWindow);
        empl_front ? rWins.emplace_front(pWindow) : rWins.emplace_back(pWindow);
        return true;
    };
        //  if window is non-drawing, set it to the end of list
    if (!pWindow->VIsDrawing_W())
    {
        if (lambResetWindow(pWindow->IsStacionar() ? stacionarWins : openableWins, false)) UpdateCursorCollision();
    }
    else
    {       //  set window to the front of list
        if (lambResetWindow(pWindow->IsStacionar() ? stacionarWins : openableWins, true)) UpdateCursorCollision();
        pWindow->SetFocuseDepth();
    }
}

void ZC_GUI_EventManager::UpdateCursorCollision()
{
    float x = 0.f, y = 0.f;
    ZC_SWindow::GetCursorPosition(x, y);
    CursoreMove(x, y, 0.f, 0.f, 1.f);
}

bool ZC_GUI_EventManager::CursoreMove(float x, float y, float rel_x, float rel_y, float time)
{
    if (!isActive) return true;
    if (pEO_cursorMove) return false;  //  have movable object, will be called in CursoreMoveOnceInFrame()

        //  returns false, if under the cursor
    auto lambCollisitonWindow = [this, x, y, time](std::list<ZC_GUI_Window*>& windows)
    {
        ZC_GUI_Window* pWin_temp = nullptr;
        ZC_GUI_EventObj* pEO_temp = nullptr;
        for (ZC_GUI_Window* pWindow : windows)
        {   //  collision only with drawing windows, all drawing window in front of both lists (openableWins, stacionarWins)
            if (!pWindow->VIsDrawing_W()) break;
            if (pWindow->VMakeCursorCollision_EO(x, y, pWin_temp, pEO_temp))
            {
                if (pEO_temp != pEO_underCursor)
                {
                    if (pEO_underCursor) pEO_underCursor->VCursorCollisionEnd_EO(time);
                    if (pEO_temp) pEO_temp->VCursorCollisionStart_EO(time);
                    pEO_underCursor = pEO_temp;
                }
                if (pWin_temp != pWin_underCursor)
                {
                    if (pWin_underCursor) pWin_underCursor->VCursorCollisionEnd_EO(time);
                    if (pWin_temp) pWin_temp->VCursorCollisionStart_EO(time);
                    pWin_underCursor = pWin_temp;
                }
                return false;
            }
        }
        if (pEO_underCursor)
        {
            pEO_underCursor->VCursorCollisionEnd_EO(time);
            pEO_underCursor = nullptr;
        }
        if (pWin_underCursor)
        {
            pWin_underCursor->VCursorCollisionEnd_EO(time);
            pWin_underCursor = nullptr;
        }
        return true;
    };
        //  check window one by one, starting from openable (if one of labmdas retuns false, then window had cursor collision, so return false from method)
    return lambCollisitonWindow(openableWins) && lambCollisitonWindow(stacionarWins);
}

bool ZC_GUI_EventManager::CursoreMoveOnceInFrame(float x, float y, float rel_x, float rel_y, float time)
{
    if (!pEO_cursorMove) return true;
    pEO_cursorMove->VCursoreMove_EO(x, y, rel_x, rel_y, time);
    return false;
}

bool ZC_GUI_EventManager::ButtonDown(ZC_ButtonID buttonID, float time)
{
    if (!isActive) return true;
    return mbLeft.ButtonDown(buttonID, pWin_underCursor, pEO_underCursor, time, pEO_cursorMove)
        && mbRight.ButtonDown(buttonID, pWin_underCursor, pEO_underCursor, time, pEO_cursorMove);
}
 
bool ZC_GUI_EventManager::ButtonUp(ZC_ButtonID buttonID, float time)
{
    if (!isActive) return true;
    return mbLeft.ButtonUp(buttonID, time, pEO_cursorMove) && mbRight.ButtonUp(buttonID, time, pEO_cursorMove);
}


//  MouseButton

bool ZC_GUI_EventManager::MouseButton::ButtonDown(ZC_ButtonID _buttonID, ZC_GUI_Window* pWin, ZC_GUI_EventObj* pEO, float time, ZC_GUI_EventObj*& rpEO_cursorMove)
{
    if (buttonID != _buttonID) return true;     //  wrong button
    if (buttonPressed) return false;    //  button already pressed
    buttonPressed = true;
        //  returns false if object was not collisioned
    auto lambCallButtonDown = [this, time, &rpEO_cursorMove](ZC_GUI_EventObj* pEventObj)
    {
        if (!pEventObj) return false; //  no object
        bool cursorMoveWhilePressed = false;    //  call button down
        if (buttonID == ZC_ButtonID::M_LEFT ? pEventObj->VLeftButtonDown_EO(time, cursorMoveWhilePressed) : pEventObj->VRightButtonDown_EO(time, cursorMoveWhilePressed))
        {
            pPressed = pEventObj;
            if (cursorMoveWhilePressed)
            {
                assert(!rpEO_cursorMove);   //  reset of active rpEO_cursorMove
                rpEO_cursorMove = pPressed;    //  set cursor move object
            }
            return true;
        }
        return false;
    };
    if (lambCallButtonDown(pEO)) pWin->MakeForcused();      //  call object's button down, if object use that button (object exists), and window may only be focused,
    else lambCallButtonDown(pWin);                          //  if object don't use button (or not exists at all), check try to window's button down
    return pPressed == nullptr;     //  one of objectes override button event, and may became pressed
}

bool ZC_GUI_EventManager::MouseButton::ButtonUp(ZC_ButtonID _buttonID, float time, ZC_GUI_EventObj*& rpEO_cursorMove)
{
    if (buttonID != _buttonID) return true;    //  wrong button
    buttonPressed = false;
    if (!pPressed) return true;     //  haven't pressed object
    buttonID == ZC_ButtonID::M_LEFT ? pPressed->VLeftButtonUp_EO(time) : pPressed->VRightButtonUp_EO(time);
    if (rpEO_cursorMove == pPressed) rpEO_cursorMove = nullptr;     //  if move object is current button, unset cursor move object
    pPressed = nullptr;
    return false;
}