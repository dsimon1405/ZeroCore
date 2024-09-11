#include "ZC_GUI_ButtonMouse.h"

#include <ZC/GUI/Backend/ZC_GUI_IconUV.h>
#include <ZC/GUI/Backend/ZC_GUI_Bindings.h>

ZC_GUI_ButtonMouse::ZC_GUI_ButtonMouse(float width, float height, ZC_GUI_ButtonFlags _buttonFlags, const ColorsButton& _colorsButton)
    : ZC_GUI_ButtonMouse(width, height, _buttonFlags, ZC_GUI_IconUV::button, _colorsButton)
{}

ZC_GUI_ButtonMouse::ZC_GUI_ButtonMouse(float width, float height, ZC_GUI_ButtonFlags _buttonFlags, const ZC_GUI_UV& uv, const ColorsButton& _colorsButton)
    : ZC_GUI_ButtonBase(ZC_GUI_ObjData(width, height, 0, uv, ZC_GUI_Bindings::bind_tex_Icons), _buttonFlags, _colorsButton)
{}

void ZC_GUI_ButtonMouse::SetDoubleClickLimit_BM(long _doubleClickLimit_nanosec)
{
    if (_doubleClickLimit_nanosec > 0) doubleClickLimit_nanosec = _doubleClickLimit_nanosec;
}

bool ZC_GUI_ButtonMouse::VIsUseCursorMoveEventOnMBLetfDown_Obj() const noexcept
{
    return this->buttonFlags & ZC_GUI_BF_M__CursorMoveOnMBLPress;
}

bool ZC_GUI_ButtonMouse::VIsUseScrollEvent_Obj() const noexcept
{
    return this->buttonFlags & ZC_GUI_BF_M__Scroll;
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
    if (this->pObjData->color == colorsButton.color_button_pressed) return;     //  button pressed, wait while up
    this->SetButtonColor_BS(colorsButton.color_button_under_cursor, true);
}

void ZC_GUI_ButtonMouse::VCursorCollisionEnd_Obj(float time)
{
    if (this->pObjData->color == colorsButton.color_button_pressed) return;     //  button pressed, wait while up
    this->SetButtonColor_BS(colorsButton.color_button, true);
}

bool ZC_GUI_ButtonMouse::VMouseButtonLeftDown_Obj(float time)
{
    if (this->bs_mouseButton == BS_HoldUntilRelease) return false;  //  don't do anything while uses another button down event
    if (this->bs_keyboardButton != BS_Released)
    {
        this->bs_mouseButton = BS_HoldUntilRelease;
        return false;
    }
    if (this->bs_mouseButton == BS_Released)
    {
        this->SetButtonColor_BS(this->colorsButton.color_button_pressed, true);
        this->bs_mouseButton = BS_Pressed;
        
        if (this->buttonFlags & ZC_GUI_BF_M__DoubleCLick)  //  call double click if in limit and restart double time in each case
        {
            this->clock.Time<ZC_Nanoseconds>() <= doubleClickLimit_nanosec ? VLeftButtonDoubleClick_BM(time) : VLeftButtonDown_BM(time);
            this->clock.Start();
            if (this->buttonFlags & ZC_GUI_BF__MBLPress) pressed_time += time;
        }
        else
        {
            VLeftButtonDown_BM(time);   //  call event in each case on released button
            if (this->buttonFlags & ZC_GUI_BF__MBLPress)
            {
                pressed_time += time;
                this->clock.Start();    //  if uses bml press event start (restart) time
            }
        }
    }
    else if (this->buttonFlags & ZC_GUI_BF__MBLPress && this->clock.Time<ZC_Nanoseconds>() >= waitPressLimit_nanosec)
    {
        pressed_time += time;
        if (pressed_time >= pressedInterval_nanosec)
        {
            VLeftButtonPressed_BM(time);  //  if uses mbl press event and it's time, call them
            pressed_time -= pressedInterval_nanosec;
        }
    }
    return true;
}

void ZC_GUI_ButtonMouse::VMouseButtonLeftUp_Obj(float time)
{
    if (this->bs_mouseButton == BS_Pressed)
    {
        if (CheckCursorCollision_Obj())
        {
            this->pObjData->color = colorsButton.color_button_under_cursor;
            VLeftButtonUp_BM(time);
        }
        else this->pObjData->color = colorsButton.color_button;
        VMapObjData_Obj(pObjData, offsetof(ZC_GUI_ObjData, color), sizeof(ZC_GUI_ObjData::color), &(this->pObjData->color));
        
        pressed_time = 0;
    }
    this->bs_mouseButton = BS_Released;
}