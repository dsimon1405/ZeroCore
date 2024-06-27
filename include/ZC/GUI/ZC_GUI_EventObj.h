#pragma once

struct ZC_GUI_Window;

struct ZC_GUI_EventObj
{
    virtual ~ZC_GUI_EventObj() = default;
    //  return true on collision
    virtual bool VMakeCursorCollision_EO(float x, float y, ZC_GUI_EventObj*& rpWindow, ZC_GUI_EventObj*& rpObj, ZC_GUI_EventObj*& rpScroll)  { return false; }
    //  return true if had collision (if window, collision checks only if background drawing) 
    virtual bool VCheckCursorCollision_EO(float x, float y) { return false; }

    virtual void VCursorCollisionStart_EO(float time) {}
    virtual void VCursorCollisionEnd_EO(float time) {}
        //  Called after calls VLeftButtonDown_EO(...) or VRightButtonDown_EO(...) with cursorMoveWhilePressed = true;
        //  Called once per frame (parrams: x, y - have last position; rel_x, rel_y - have sum of all changes in frame).
    virtual void VCursoreMove_EO(float rel_x, float rel_y) {}
        //  Must return true, if override. If last param true current object wil be called on cursor move events, while button is pressed.
        //  Only one object can be movable at a time.
    virtual bool VLeftButtonDown_EO(float time, bool& cursorMoveWhilePressed) { return false; }
    virtual void VLeftButtonUp_EO(float time) {}
        //  Must return true, if override. If last param true current object wil be called on cursor move eventsб while button is pressed.
        //  Only one object can be movable at a time.
    virtual bool VRightButtonDown_EO(float time, bool& cursorMoveWhilePressed) { return false; }
    virtual void VRightButtonUp_EO(float time) {}
    virtual void VScroll_EO(float vertical, float time) {}

    bool Collision(float x, float y, float bl_x, float bl_y, float tr_x, float tr_y)
    {
        return bl_x <= x && bl_y <= y && x <= tr_x && y <= tr_y;
    }
};