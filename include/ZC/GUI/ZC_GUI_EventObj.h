#pragma once

struct ZC_GUI_Window;

struct ZC_GUI_EventObj
{

    virtual ~ZC_GUI_EventObj() = default;
    //  return true on collision
    virtual bool VMakeCursorCollision_EO(float x, float y, ZC_GUI_Window*& rpWindow, ZC_GUI_EventObj*& rpObj)  { return false; }
    //  retrurn true if had collision (if window, collision checks only if background drawing) 
    virtual bool VCheckCursorCollision_EO(float x, float y) { return false; }

    virtual bool VIsMovable_EO() { return false; }

    virtual void VCursorCollisionStart_EO(float time) {}
    virtual void VCursorCollisionEnd_EO(float time) {}
    virtual void VCursoreMove_EO(float x, float y, float rel_x, float rel_y, float time) {}
    virtual void VLeftButtonDown_EO(float time) {}
    virtual void VLeftButtonUp_EO(float time) {}
    virtual void VRightButtonDown_EO(float time) {}
    virtual void VRightButtonUp_EO(float time) {}
    virtual void VScroll_EO(float vertical, float time) {}

    bool Collision(float x, float y, float bl_x, float bl_y, float tr_x, float tr_y)
    {
        return bl_x <= x && bl_y <= y && x <= tr_x && y <= tr_y;
    }
};