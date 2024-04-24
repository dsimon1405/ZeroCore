#pragma once

#include <ZC/Collision/ZC_MouseCollisionWindow.h>
#include <ZC/Events/ZC_SingleClickButton.h>

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

    ZC_SConnection sconMove;
    ZC_SingleClickButton scbButtonLeft,
        scbButtonRight;
    std::forward_list<Pair> moveCollisions;
    std::forward_list<Pair> leftButtonCollisions;
    std::forward_list<Pair> rightButtonCollisions;
    ZC_MouseCollisionWindow* pMCWLastActive;
    
    // ZC_MouseCollisionWindow* lastMmcs = nullptr;    //  last actual mouse move collision
    // /*
    // In ZC_MouseMoveCollision coulde be object wich also in ZC_MousePressCollision. In that case high probability that object from press event already
    // in ZC_MouseMoveCollision if it called faster, for that object don't need to make collision again. Can't be shoore wich event happens first ZC_MouseMoveCollision
    // or ZC_MousePressCollision, so if ZC_MouseMoveCollision called faster pointer on collisioned object will be set in lastMmcs. That object will be actual while poll events
    // don't start again and about it's actuality says isLastMmcsActual.
    // */
    // bool isLastMmcsActual = false;

    void MakeCollision(std::forward_list<Pair>& rPairs, MCWEvent event, float cursorPosX, float cursorPosY, float time);
    void MoveCollision(float cursorPosX, float cursorPosY, float cursorRelX, float cursorRelY, float time);
    void ButtonDownLeft(float time);
    void ButtonDownRight(float time);
};