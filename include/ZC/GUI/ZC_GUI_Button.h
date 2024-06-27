#pragma once

#include <ZC/GUI/ZC_GUI_Window.h>

struct ZC_GUI_Button : public ZC_GUI_Obj
{
    ZC_GUI_Button(float width, float height);
    ZC_GUI_Button(float width, float height, const ZC_GUI_UV& uv);

    bool VIsDrawing_Obj() const noexcept override;
    
    bool VMakeCursorCollision_EO(float x, float y, ZC_GUI_EventObj*& rpWindow, ZC_GUI_EventObj*& rpObj, ZC_GUI_EventObj*& rpScroll) override;
    bool VCheckCursorCollision_EO(float x, float y) override;

    // bool VIsMovable_EO() override { return false; }
    void VCursorCollisionStart_EO(float time) override {}
    void VCursorCollisionEnd_EO(float time) override {}
    void VCursoreMove_EO(float rel_x, float rel_y) override {}
    bool VLeftButtonDown_EO(float time, bool& cursorMoveWhilePressed) override { return true; }
    void VLeftButtonUp_EO(float time) override {}
    bool VRightButtonDown_EO(float time, bool& cursorMoveWhilePressedorMove) override { return true; }
    void VRightButtonUp_EO(float time) override {}
    void VScroll_EO(float vertical, float time) override {}
};

// #include <iostream>
    // static inline int counter = 0;
    // int myID;
// std::cout<<"pressed but "<<myID<<std::endl;
// {myID = counter++;}