#pragma once

#include <ZC/GUI/ZC_GUI_ButtonState.h>
#include <ZC/Tools/Time/ZC_Clock.h>

enum ZC_GUI_BM__Flag
{
    ZC_GUI_BM__None                          = 0,
    ZC_GUI_BM__CursorMoveOnButtonLeftPress   = 1,        //  if the left mouse button is pressed on the button, VCursorMove_Obj() will be called until the button is released
    ZC_GUI_BM__Scroll                        = 1 << 1,   //  if the mouse cursor under the button, VScroll_Obj() will be called until the cursore leave button's area
    ZC_GUI_BM__DoubleCLick                   = 1 << 2    //  mouse left button double click (fast) will call VLeftButtonDoubleClick() on the second click, not VLeftButtonDown_BM()
};
typedef int ZC_GUI_BM__Flags;

#include <iostream>
struct ZC_GUI_ButtonMouse : public ZC_GUI_ButtonState
{
    const ZC_GUI_BM__Flags bm_flags;

    static inline const long doubleClickNanosecondsLimit = 300000000;
    ZC_Clock clock_doubleClick;

        //  may be override
    void VScroll_Obj(float vertical, float time) override
    {
        std::cout<<"scroll "<<vertical<<std::endl;
    }
    virtual void VLeftButtonDown_BM(float time) {
            std::cout<<"down"<<std::endl;}
    virtual void VLeftButtonDoubleClick_BM(float time) {
            std::cout<<"double click"<<std::endl;}
    virtual void VLeftButtonUp_BM(float time) {
            std::cout<<"up"<<std::endl;}

    void VCursorMove_Obj(float rel_x, float rel_y) override
    {
        std::cout<<"cursor move event: x = "<<rel_x<<"; y = "<<rel_y<<std::endl;
    }


    ZC_GUI_ButtonMouse(float width, float height, ZC_GUI_BM__Flags _bm_flags);
    ZC_GUI_ButtonMouse(float width, float height, ZC_GUI_BM__Flags _bm_flags, const ZC_GUI_UV& uv);

    bool VIsDrawing_Obj() const noexcept override;
    

    bool VIsUseScrollEvent_Obj() const noexcept override
    {
        return bm_flags & ZC_GUI_BM__Scroll;
    }

    void VWindowStopDrawing_Obj() override
    {
        if (this->pObjData->color == color_default) return;     //  nothing to change
        this->pObjData->color = color_default;
        VMapObjData_Obj(pObjData, offsetof(ZC_GUI_ObjData, color), sizeof(ZC_GUI_ObjData::color), &(this->pObjData->color));
    }
    
    bool VMakeCursorCollision_Obj(float x, float y, ZC_GUI_Obj*& rpWindow, ZC_GUI_Obj*& rpObj, ZC_GUI_Obj*& rpScroll) override;
    // bool VCheckCursorCollision_Obj(float x, float y) override;

    // bool VIsMovable_EO() override { return false; }
    void VCursorCollisionStart_Obj(float time) override
    {
        if (this->pObjData->color == color_pressed) return;     //  button pressed, wait while up
        this->pObjData->color = color_under_cursor;
        VMapObjData_Obj(pObjData, offsetof(ZC_GUI_ObjData, color), sizeof(ZC_GUI_ObjData::color), &(this->pObjData->color));
    }

    void VCursorCollisionEnd_Obj(float time) override
    {
        if (this->pObjData->color == color_pressed) return;     //  button pressed, wait while up
        this->pObjData->color = color_default;
        VMapObjData_Obj(pObjData, offsetof(ZC_GUI_ObjData, color), sizeof(ZC_GUI_ObjData::color), &(this->pObjData->color));
    }

    bool VLeftButtonDown_Obj(float time, bool& cursorMoveWhilePressed) override
    {
        this->pObjData->color = color_pressed;
        VMapObjData_Obj(pObjData, offsetof(ZC_GUI_ObjData, color), sizeof(ZC_GUI_ObjData::color), &(this->pObjData->color));
        cursorMoveWhilePressed = bm_flags & ZC_GUI_BM__CursorMoveOnButtonLeftPress;  //  will be used cusor move
        if (bm_flags == ZC_GUI_BM__DoubleCLick)  //  call double click if in limit and restart double time in each case
        {
            clock_doubleClick.Time<ZC_Nanoseconds>() <= doubleClickNanosecondsLimit ? VLeftButtonDoubleClick_BM(time) : VLeftButtonDown_BM(time);
            clock_doubleClick.Restart<ZC_Nanoseconds>();
        }
        else VLeftButtonDown_BM(time);
        return true;
    }

    void VLeftButtonUp_Obj(float time) override
    {
        this->pObjData->color = color_default;
        VMapObjData_Obj(pObjData, offsetof(ZC_GUI_ObjData, color), sizeof(ZC_GUI_ObjData::color), &(this->pObjData->color));
        if (CheckCursorCollision_Obj()) VLeftButtonUp_BM(time);
    }
};

// #include <iostream>
    // static inline int counter = 0;
    // int myID;
// std::cout<<"pressed but "<<myID<<std::endl;
// {myID = counter++;}