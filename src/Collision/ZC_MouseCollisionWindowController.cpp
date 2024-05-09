#include "ZC_MouseCollisionWindowController.h"

#include <ZC/Tools/Container/ZC_ContFunc.h>
#include <ZC/Events/ZC_Events.h>
#include <ZC_Config.h>
#ifdef ZC_IMGUI
#include <ZC_IGWindow.h>
#endif
#include <ZC/Events/ZC_ButtonID.h>
#include <ZC/Video/ZC_Window.h>

#include <cassert>

ZC_MouseCollisionWindowController::ZC_MouseCollisionWindowController()
{
    pMCWC = this;
}

ZC_MouseCollisionWindowController::~ZC_MouseCollisionWindowController()
{
    ecMove.Disconnect();
    pMCWC = nullptr;
}

void ZC_MouseCollisionWindowController::Add(ZC_MouseCollisionWindow* pMCW)
{
    if (!pMCWC) return;

    auto add = [](std::forward_list<Pair>& rPairs, ZC_MouseCollisionWindow* pMCW)
    {
        auto pPair = ZC_Find(rPairs, pMCW->border);
        if (pPair) pPair->mcws.emplace_front(pMCW);
        else rPairs.emplace_front(Pair{ pMCW->border, std::forward_list<ZC_MouseCollisionWindow*>{ pMCW } });
    };

    if (pMCW->eventMove)
    {
        add(pMCWC->moveCollisions, pMCW);
        if (!pMCWC->ecMove.IsConnected()) pMCWC->ecMove.NewConnection(ZC_Events::ConnectMouseMoveOnceInFrame({ &ZC_MouseCollisionWindowController::CrusorMove, pMCWC }));
    }
    if (pMCW->eventLeftButton)
    {
        add(pMCWC->leftButtonCollisions, pMCW);
        if (!pMCWC->ecButtonLeft.IsConnected())
            pMCWC->ecButtonLeft.NewConnection(ZC_Events::ConnectButtonClick(ZC_ButtonID::M_LEFT, { &ZC_MouseCollisionWindowController::ButtonDown, pMCWC },
                { &ZC_MouseCollisionWindowController::ButtonUp, pMCWC }));
    }
    if (pMCW->eventRightButtom)
    {
        add(pMCWC->rightButtonCollisions, pMCW);
        if (!pMCWC->ecButtonRight.IsConnected())
            pMCWC->ecButtonRight.NewConnection(ZC_Events::ConnectButtonClick(ZC_ButtonID::M_RIGHT, { &ZC_MouseCollisionWindowController::ButtonDown, pMCWC },
                { &ZC_MouseCollisionWindowController::ButtonUp, pMCWC }));
    }
}

void ZC_MouseCollisionWindowController::Erase(ZC_MouseCollisionWindow* pMCW)
{
    if (!pMCWC) return;

    auto erase = [](std::forward_list<Pair>& rPairs, ZC_MouseCollisionWindow* pMCW)
    {
        auto prevIter = rPairs.before_begin();
        for (auto iter = rPairs.begin(); iter != rPairs.end(); )
        {
            if (*iter == pMCW->border)
            {
                ZC_ForwardListErase(iter->mcws, pMCW);
                if (iter->mcws.empty()) rPairs.erase_after(prevIter);
                return;
            }
            prevIter = iter;
        }
        assert(false);  //  must not reach here
    };

    if (pMCW->eventMove)
    {
        erase(pMCWC->moveCollisions, pMCW);
        if (pMCWC->moveCollisions.empty()) pMCWC->ecMove.Disconnect();
    }
    if (pMCW->eventLeftButton)
    {
        erase(pMCWC->leftButtonCollisions, pMCW);
        if (pMCWC->leftButtonCollisions.empty()) pMCWC->ecButtonLeft.Disconnect();
    }
    if (pMCW->eventRightButtom)
    {
        erase(pMCWC->rightButtonCollisions, pMCW);
        if (pMCWC->rightButtonCollisions.empty()) pMCWC->ecButtonRight.Disconnect();
    }
}

bool ZC_MouseCollisionWindowController::IsCursorInArea()
{
    return pMCWC ? pMCWC->pMCWLastActive != nullptr : false;
}

void ZC_MouseCollisionWindowController::MakeCollision(std::forward_list<Pair>& rPairs, MCWEvent event, bool isButtonDown, float cursorPosX, float cursorPosY, float time)
{
#ifdef ZC_IMGUI
    if (ZC_IGWindow::IsCursorInOneOfWindows()) return;  //  cursor into one of ZC_IGWindow windows
#endif
    if (pMCWLastActive)
    {   //  if pMCWLastActive != nullptr, check is cursor still in it's area
        if (pMCWLastActive->MakeCollision(event, isButtonDown, cursorPosX, cursorPosY, time)) return;
        else    //  if no longe in area of pMCWLastActive, end collision for that object and set pMCWLastActive = nullptr
        {
            pMCWLastActive->CollisionEnd(time);
            pMCWLastActive = nullptr;
        }
    }
    for (auto& pair : rPairs)
    {
        pMCWLastActive = pair.MakeCollision(event, isButtonDown, cursorPosX, cursorPosY, time);
        if (pMCWLastActive) return;
    }
}

void ZC_MouseCollisionWindowController::CrusorMove(float cursorPosX, float cursorPosY, float cursorRelX, float cursorRelY, float time)
{
    MakeCollision(moveCollisions, MCWEvent::E_Move, false, cursorPosX, cursorPosY, time);
}

void ZC_MouseCollisionWindowController::ButtonDown(ZC_ButtonID buttonId, float time)
{
    float cursorPosX, cursorPosY;
    ZC_Window::GetCursorPosition(cursorPosX, cursorPosY);
    MakeCollision(leftButtonCollisions, buttonId == M_LEFT ? MCWEvent::E_Mouse_Button_Left : MCWEvent::E_Mouse_Button_Right, true, cursorPosX, cursorPosY, time);
}

void ZC_MouseCollisionWindowController::ButtonUp(ZC_ButtonID buttonId, float time)
{
    float cursorPosX, cursorPosY;
    ZC_Window::GetCursorPosition(cursorPosX, cursorPosY);
    MakeCollision(leftButtonCollisions, buttonId == M_LEFT ? MCWEvent::E_Mouse_Button_Left : MCWEvent::E_Mouse_Button_Right, false, cursorPosX, cursorPosY, time);
}


//  Pair

bool ZC_MouseCollisionWindowController::Pair::operator == (const MCWBorder& _border)
{
    return border == _border;
}

ZC_MouseCollisionWindow* ZC_MouseCollisionWindowController::Pair::MakeCollision(MCWEvent event, bool isButtonDown, float cursorX, float cursorY, float time)
{
    if (border.MakeCollision(cursorX, cursorY))     //  if cursor in border check all ZC_MouseCollisionWindow in that border
        for (auto pMCW : mcws) if (pMCW->MakeCollision(event, isButtonDown, cursorX, cursorY, time)) return pMCW;
    return nullptr;
}