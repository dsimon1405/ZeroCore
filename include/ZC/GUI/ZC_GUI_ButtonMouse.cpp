#include <ZC/GUI/ZC_GUI_ButtonMouse.h>

#include "ZC_GUI_IconUV.h"

ZC_GUI_ButtonMouse::ZC_GUI_ButtonMouse(float width, float height, ZC_GUI_MB__Flags _mb_flags)
    : ZC_GUI_ButtonMouse(width, height, _mb_flags, ZC_GUI_IconUV::button)
{}

ZC_GUI_ButtonMouse::ZC_GUI_ButtonMouse(float width, float height, ZC_GUI_MB__Flags _mb_flags, const ZC_GUI_UV& uv)
    : ZC_GUI_ButtonState(ZC_GUI_ObjData{ .width = width, .height = height, .uv = uv }),
    mb_flags(_mb_flags)
{}

bool ZC_GUI_ButtonMouse::VIsDrawing_Obj() const noexcept
{
    return true;
}

bool ZC_GUI_ButtonMouse::VIsUseCursorMoveEventOnMBLetfDown_Obj() const noexcept
{
    return this->mb_flags & ZC_GUI_MB__CursorMoveOnMBLPress;
}

bool ZC_GUI_ButtonMouse::VIsUseScrollEvent_Obj() const noexcept
{
    return this->mb_flags & ZC_GUI_MB__Scroll;
}

void ZC_GUI_ButtonMouse::VStopEventActivity_Obj()
{
    this->StopEventActivity_BS();   //  same in ZC_GUI_ButtonKeyboard
}

bool ZC_GUI_ButtonMouse::VMakeCursorCollision_Obj(float x, float y, ZC_GUI_Obj*& rpObj, ZC_GUI_Obj*& rpScroll)
{
    return MakeCursorCollision_Obj(x, y, rpObj, rpScroll);
}

void ZC_GUI_ButtonMouse::VCursorCollisionStart_Obj(float time)
{
    if (this->pObjData->color == color_pressed) return;     //  button pressed, wait while up
    this->pObjData->color = color_under_cursor;
    VMapObjData_Obj(pObjData, offsetof(ZC_GUI_ObjData, color), sizeof(ZC_GUI_ObjData::color), &(this->pObjData->color));
}

void ZC_GUI_ButtonMouse::VCursorCollisionEnd_Obj(float time)
{
    if (this->pObjData->color == color_pressed) return;     //  button pressed, wait while up
    this->pObjData->color = color_default;
    VMapObjData_Obj(pObjData, offsetof(ZC_GUI_ObjData, color), sizeof(ZC_GUI_ObjData::color), &(this->pObjData->color));
}

bool ZC_GUI_ButtonMouse::VMouseButtonLeftDown_Obj(float time)
{
    if (this->bs_keyboardButton != BS_Released || this->bs_mouseButton == BS_HoldUntilRelease) return false;  //  don't do anything while uses another button down event
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

void ZC_GUI_ButtonMouse::VMouseButtonLeftUp_Obj(float time)
{
    if (this->bs_mouseButton == BS_Pressed)
    {
        if (CheckCursorCollision_Obj())
        {
            this->pObjData->color = color_under_cursor;
            VLeftButtonUp_BM(time);
        }
        else this->pObjData->color = color_default;
        VMapObjData_Obj(pObjData, offsetof(ZC_GUI_ObjData, color), sizeof(ZC_GUI_ObjData::color), &(this->pObjData->color));
    }
    this->bs_mouseButton = BS_Released;
}