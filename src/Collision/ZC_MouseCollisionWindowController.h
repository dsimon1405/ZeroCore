#pragma once

#include <ZC/Collision/ZC_MouseCollisionWindow.h>
#include <ZC/Events/ZC_EC.h>
#include <ZC/Events/ZC_ButtonID.h>

#include <forward_list>

//  mouse move collision controller
class ZC_MouseCollisionWindowController
{
    typedef typename ZC_MouseCollisionWindow::Border MCWBorder;
    typedef typename ZC_MouseCollisionWindow::Event MCWEvent;

    struct Pair
    {
        MCWBorder border;
        std::forward_list<ZC_MouseCollisionWindow*> mcws;

        bool operator == (const MCWBorder& _border);

        ZC_MouseCollisionWindow* MakeCollision(MCWEvent event, float cursorX, float cursorY, float time);
    };

public:
    ZC_MouseCollisionWindowController();

    ~ZC_MouseCollisionWindowController();

    static void Add(ZC_MouseCollisionWindow* pMCW);
    static void Erase(ZC_MouseCollisionWindow* pMCW);
    //  returns true, if mouse cursor in one of ZC_MouseCollisionWindow areas.
    static bool IsCursorInArea();

private:
    static inline ZC_MouseCollisionWindowController* pMCWC;

    ZC_EC ecMove,
        ecButtonLeft,
        ecButtonRight;
    std::forward_list<Pair> moveCollisions;
    std::forward_list<Pair> leftButtonCollisions;
    std::forward_list<Pair> rightButtonCollisions;
    ZC_MouseCollisionWindow* pMCWLastActive;

    void MakeCollision(std::forward_list<Pair>& rPairs, MCWEvent event, float cursorPosX, float cursorPosY, float time);
    void MoveCollision(float cursorPosX, float cursorPosY, float cursorRelX, float cursorRelY, float time);
    void ButtonDownLeft(ZC_ButtonID buttonId, float time);
    void ButtonDownRight(ZC_ButtonID buttonId, float time);
};