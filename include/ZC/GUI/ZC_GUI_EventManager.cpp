#include <ZC/GUI/ZC_GUI_EventManager.h>

void ZC_GUI_EventManager::AddWindow(bool isStacionar, ZC_GUI_EventObj* pWindow)
{
    isStacionar ? stacionarWins.emplace_back(pWindow) : openableWins.emplace_back(pWindow);
}

void ZC_GUI_EventManager::EraseWindow(bool isStacionar, ZC_GUI_EventObj* pWindow)
{
    isStacionar ? std::erase(stacionarWins, pWindow) : std::erase(openableWins, pWindow);
}

bool ZC_GUI_EventManager::CursoreMove(float x, float y, float rel_x, float rel_y, float time)
{
    if (!isActive) return true;

    if (pEO_pressedLBM && pEO_pressedLBM->VIsMovable_EO())  //  move pressed object, or window
    {
        pEO_pressedLBM->VCursoreMove_EO(x, y, rel_x, rel_y, time);
        return false;
    }

    //     //  if the window is first in openable, then the window is on top, can check is cursor still in it's object
    // if (pWin_underCursor && !pWin_underCursor->IsStacionar() && pWin_underCursor == openableWins.front())
    // {
    //     if (pEO_underCursor)
    //     {
    //         if (pEO_underCursor->VCheckCursorCollision_EO(x, y)) return false;     //  cursor still in the object
    //         //  cursor not in the object any more
    //         pEO_underCursor->VCursorCollisionEnd_EO(time);
    //     }
    //     pEO_underCursor = nullptr;
    //     ZC_GUI_Window* pWin_temp = nullptr;
    //     if (pWin_underCursor->VMakeCursorCollision_EO(x, y, pWin_temp, pEO_underCursor))     //  try collision for window
    //     {
    //         if (pEO_underCursor)
    //         {
    //             pEO_underCursor->VCursorCollisionStart_EO(time);    //  new object of the window under cursor
    //             return false;
    //         }
    //     }
    //     else    //  window don't have collision with cursor
    //     {
    //         pWin_underCursor->VCursorCollisionEnd_EO(time);
    //         pWin_underCursor = nullptr;
    //     }
    // }
        //  returns false, if under the cursor
    auto lambCollisitonWindow = [this, x, y, time](std::list<ZC_GUI_EventObj*>& windows)
    {
        ZC_GUI_Window* pWin_temp = nullptr;
        ZC_GUI_EventObj* pEO_temp = nullptr;
        for (ZC_GUI_EventObj* pWindow : windows)
        {
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
        return true;
    };
        //  check window one by one, starting from openable (if one of labmas retuns false, then window had cursor collision, so return false from method)
    return lambCollisitonWindow(openableWins) && lambCollisitonWindow(stacionarWins);
}

void ZC_GUI_EventManager::ChangeActivity(bool active)
{
    isActive = active;
}