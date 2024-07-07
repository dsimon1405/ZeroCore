#pragma once

#include <ZC/GUI/ZC_GUI_ButtonState.h>

enum ZC_GUI_MB__Flag
{
    ZC_GUI_MB__None                     = 0,
    ZC_GUI_MB__CursorMoveOnMBLPress     = 1,        //  if the left mouse button is pressed on the button, VCursorMove_Obj() will be called until the button is released
    ZC_GUI_MB__Scroll                   = 1 << 1,   //  if the mouse cursor under the button, VScroll_Obj() will be called until the cursore leave button's area
    ZC_GUI_MB__DoubleCLick              = 1 << 2,   //  mouse left button double click (fast) will call VLeftButtonDoubleClick() on the second click, not VLeftButtonDown_BM()
    ZC_GUI_MB__MBLPress                 = 1 << 3,   //  will call VLeftButtonPressed_BM() event, instead VLeftButtonDown_BM(), if mbl will pressed
};
typedef int ZC_GUI_MB__Flags;

#include <iostream>
struct ZC_GUI_ButtonMouse : public ZC_GUI_ButtonState
{
    const ZC_GUI_MB__Flags mb_flags;

    static inline const long nanosecondLimit = 300000000;   //  uses ass nanosec limit in lmb: ZC_GUI_MB__DoubleCLick, ZC_GUI_MB__MBLPress events evetns

        //  may be override
    void VScroll_Obj(float vertical, float time) override
    {
        std::cout<<"scroll "<<vertical<<std::endl;
    }
    void VCursorMove_Obj(float rel_x, float rel_y) override
    {
        std::cout<<"cursor move event: x = "<<rel_x<<"; y = "<<rel_y<<std::endl;
    }
    virtual void VLeftButtonDown_BM(float time) {
            std::cout<<"down"<<std::endl;}
    virtual void VLeftButtonDoubleClick_BM(float time) {
            std::cout<<"double click"<<std::endl;}
    virtual void VLeftButtonPressed_BM(float time) {
            std::cout<<"pressed"<<std::endl;}
    virtual void VLeftButtonUp_BM(float time) {
            std::cout<<"up"<<std::endl;}



    ZC_GUI_ButtonMouse(float width, float height, ZC_GUI_MB__Flags _mb_flags);
    ZC_GUI_ButtonMouse(float width, float height, ZC_GUI_MB__Flags _mb_flags, const ZC_GUI_UV& uv);

    bool VIsDrawing_Obj() const noexcept override
    {
        return true;
    }
    
    bool VIsUseCursorMoveEventOnMBLetfDown_Obj() const noexcept override
    {
        return this->mb_flags & ZC_GUI_MB__CursorMoveOnMBLPress;
    }

    bool VIsUseScrollEvent_Obj() const noexcept override
    {
        return this->mb_flags & ZC_GUI_MB__Scroll;
    }

    void VStopEventActivity_Obj() override
    {
        this->StopEventActivity_BS();   //  same in ZC_GUI_ButtonKeyboard
    }
    
    bool VMakeCursorCollision_Obj(float x, float y, ZC_GUI_Obj*& rpObj, ZC_GUI_Obj*& rpScroll) override;

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

    bool VMouseButtonLeftDown_Obj(float time) override
    {
        if (this->bs_keyboardButton == BS_Pressed) return false;  //  don't do anything while uses another button down event
        if (this->bs_mouseButton == BS_Released)
        {
            this->pObjData->color = color_pressed;
            VMapObjData_Obj(pObjData, offsetof(ZC_GUI_ObjData, color), sizeof(ZC_GUI_ObjData::color), &(this->pObjData->color));
            this->bs_mouseButton = BS_Pressed;
            
            if (this->mb_flags & ZC_GUI_MB__DoubleCLick)  //  call double click if in limit and restart double time in each case
            {
                this->clock.Time<ZC_Nanoseconds>() <= nanosecondLimit ? VLeftButtonDoubleClick_BM(time) : VLeftButtonDown_BM(time);
                this->clock.Start();
            }
            else
            {
                VLeftButtonDown_BM(time);   //  call event in each case on released button
                if (this->mb_flags & ZC_GUI_MB__MBLPress) this->clock.Start();    //  if uses bml press event start (restart) time
            }
        }
        else if (this->mb_flags & ZC_GUI_MB__MBLPress && this->clock.Time<ZC_Nanoseconds>() >= nanosecondLimit) VLeftButtonPressed_BM(time);  //  if uses mbl press event and it's time, call them
        return true;
    }

    void VMouseButtonLeftUp_Obj(float time) override
    {
        this->pObjData->color = color_default;
        VMapObjData_Obj(pObjData, offsetof(ZC_GUI_ObjData, color), sizeof(ZC_GUI_ObjData::color), &(this->pObjData->color));
        if (CheckCursorCollision_Obj()) VLeftButtonUp_BM(time);
        this->bs_mouseButton = BS_Released;
    }
};

// #include <iostream>
    // static inline int counter = 0;
    // int myID;
// std::cout<<"pressed but "<<myID<<std::endl;
// {myID = counter++;}