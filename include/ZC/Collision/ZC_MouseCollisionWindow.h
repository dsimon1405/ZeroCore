#pragma once

#include <ZC/Tools/Math/ZC_Math.h>

class ZC_MouseCollisionWindowController;

/*
Class for trekking actions(move, click left, right buttons) of the mouse cursor for heir's quad area (for example textured area like button).
Class uses const pointers on quad area(sets in constructor) to avoid manual reinstall, but also have function ReinstallCoords(...) for it's reinstall.
*/
class ZC_MouseCollisionWindow
{
    friend class ZC_MouseCollisionWindowController;
public:
    struct Border
    {
        float blX,    //  bottom left corner
            blY,    //  bottom left corner
            trX,    //  top right corner
            trY;    //  top right corner

        //  by default set the width and height to be too high to fit the window exactly
        Border(float _blX = 0, float _blY = 0, float width = ZC_INT_MAX / 2, float height = ZC_INT_MAX / 2);
        bool operator == (const Border& frame) const noexcept;
        bool MakeCollision(float cursorX, float cursorY);
    };

    virtual ~ZC_MouseCollisionWindow();

    void ConnectMouseCollision();
    void DisconnectMouseConllision();

protected:
    typedef int Events;
    enum Event
    {
        E_Move                 = 1,
        E_Down_Mouse_Left      = 1 << 1,
        E_Down_Mouse_Right     = 1 << 2
    };

    /*
    Params:
    - events - trekking events. coude be several at a time (E_Move | E_Down_Mouse_Right). E_Move - overriden VMouseMoveCollision() calls on start (look UseManualMove()) move cursor collision,
        overriden VMouseMoveCollisionEnd() calls on end move cursor collision. E_Down_Mouse_Left - overriden VMouseLeftDownCollision() calls on click left mouse button.
        E_Down_Mouse_Right - overriden VMouseRightDownCollision() calls on click right mouse button.
    - _blX - bottom left X coord of quad (window's pixel coords).
    - _blY - bottom left Y coord of quad (window's pixel coords).
    - _width - quad's width (pixels).
    - _height - quad's height (pixels).
    - needConnect - connects to events after cereaetion, calls ConnectMouseCollision().
    - _border - border it's a quad in wich must be you quad. It's uses for grouping several area's in one zone and minimize count of checking areas if them in other border.
        By default taking too large(more then window dimension, to avoid trecking it's size chenging) border quad same for all.
        Example, you have 10(10 heirs) buttons in 2 different quad areas (5 buttons in each area), to avoid check all 10 buttons on each event, set border,
        wich on event start check cursor coords in border or not, and if not, don't check buttons in that border.
    */
    ZC_MouseCollisionWindow(Events events, const float* _blX, const float* _blY, const float* _width, const float* _height, bool needConnect, const Border& _border = {});
    //  Reinstall references of tracking frame.
    void ReinstallCoords(const float* _blX, const float* _blY, const float* _width, const float* _height);
    /*
    By default VMouseMoveCollision() calls only ance on start of collision and don't call till call VMouseMoveCollisionEnd(). If set manual move true,
    VMouseMoveCollision() continue calls on each move, into tracking area.
    */
    void UseManualMove(bool _manualMove);

private:
    const bool eventMove,
        eventLeftButton,
        eventRightButtom;
    const float* pBlX,
        * pBlY,
        * pWidth,
        * pHeight;
    const Border border;
    bool moveCalled = false,
        manualMove = false,
        connected = false;  //  connection to ZC_MouseCollisionWindowController

    //  Calls if move collision heppens. Must be implimented if uses mouse move collision.
    virtual void VMouseMoveCollision(float time) {}
    //  Сalls when the cursor leaves the bounds. Must be implimented if uses mouse move collision.
    virtual void VMouseMoveCollisionEnd(float time) {}
    //  must be implimented if uses mouse left button collision.
    virtual void VMouseLeftDownCollision(float time) {}
    //  must be implimented if uses mouse right button collision.
    virtual void VMouseRightDownCollision(float time) {}

    //  returns true if collision happened.
    bool MakeCollision(Event event, float cursorX, float cursorY, float time);
    void CollisionEnd(float time);
};