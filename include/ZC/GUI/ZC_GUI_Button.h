#pragma once

#include <ZC/GUI/ZC_GUI_Window.h>

struct ZC_GUI_Button : public ZC_GUI_Obj
{
    ZC_GUI_Button(float width, float height);
    
    bool VMakeCursorCollision_EO(float x, float y, ZC_GUI_Window*& rpWindow, ZC_GUI_EventObj*& rpObj) override;
    bool VCheckCursorCollision_EO(float x, float y) override;

    bool VIsMovable_EO() override { return false; }
    void VCursorCollisionStart_EO(float time) override {}
    void VCursorCollisionEnd_EO(float time) override {}
    void VCursoreMove_EO(float x, float y, float rel_x, float rel_y, float time) override {}
    void VLeftButtonDown_EO(float time) override {}
    void VLeftButtonUp_EO(float time) override {}
    void VRightButtonDown_EO(float time) override {}
    void VRightButtonUp_EO(float time) override {}
    void VScroll_EO(float vertical, float time) override {}
};
