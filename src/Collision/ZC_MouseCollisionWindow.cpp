#include <ZC/Collision/ZC_MouseCollisionWindow.h>

#include "ZC_MouseCollisionWindowController.h"

ZC_MouseCollisionWindow::~ZC_MouseCollisionWindow()
{
    if (connected) ZC_MouseCollisionWindowController::Erase(this);
}

void ZC_MouseCollisionWindow::ConnectMouseCollision()
{
    if (connected) return;
    ZC_MouseCollisionWindowController::Add(this);
    connected = true;
}

void ZC_MouseCollisionWindow::DisconnectMouseConllision()
{
    if (!connected) return;
    ZC_MouseCollisionWindowController::Erase(this);
    connected = false;
}

ZC_MouseCollisionWindow::ZC_MouseCollisionWindow(Events events, const float* _pBlX, const float* _pBlY, const float* _pWidth, const float* _pHeight,
        bool needConnect, const Border& _border)
    : eventMove(events & E_Move),
    eventLeftButton(events & E_Down_Mouse_Left),
    eventRightButtom(events & E_Down_Mouse_Right),
    pBlX(_pBlX),
    pBlY(_pBlY),
    pWidth(_pWidth),
    pHeight(_pHeight),
    border(_border)
{
    if (needConnect) ConnectMouseCollision();
}

void ZC_MouseCollisionWindow::ReinstallCoords(const float* _pBlX, const float* _pBlY, const float* _pWidth, const float* _pHeight)
{
    pBlX = _pBlX;
    pBlY = _pBlY;
    pWidth = _pWidth;
    pHeight = _pHeight;
}

void ZC_MouseCollisionWindow::UseManualMove(bool _manualMove)
{
    manualMove = _manualMove;
}

bool ZC_MouseCollisionWindow::MakeCollision(Event event, float cursorX, float cursorY, float time)
{
    if (!(cursorX > *pBlX && cursorY > *pBlY && cursorX < *pBlX + *pWidth && cursorY < *pBlY + *pHeight)) return false;
    switch (event)
    {
    case E_Move:
    {
        if (!manualMove && moveCalled) return true;
        moveCalled = true;
        VMouseMoveCollision(time);
    } break;
    case E_Down_Mouse_Left: VMouseLeftDownCollision(time); break;
    case E_Down_Mouse_Right: VMouseRightDownCollision(time); break;
    }
    return true;
}

void ZC_MouseCollisionWindow::CollisionEnd(float time)
{
    if (eventMove) VMouseMoveCollisionEnd(time);
    moveCalled = false;
}


//  Border

ZC_MouseCollisionWindow::Border::Border(float _pBlX, float _pBlY, float width, float height)
    : blX(_pBlX),
    blY(_pBlY),
    trX(blX + width),
    trY(blY + height)
{}

bool ZC_MouseCollisionWindow::Border::operator == (const Border& frame) const noexcept
{
    return blX == frame.blX && blY == frame.blY && trX == frame.trX && trY == frame.trY;
}

bool ZC_MouseCollisionWindow::Border::MakeCollision(float cursorX, float cursorY)
{
    return blX < cursorX && blY < cursorY && trX > cursorX && trY > cursorY;
}