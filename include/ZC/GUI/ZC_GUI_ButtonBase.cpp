#include <ZC/GUI/ZC_GUI_ButtonBase.h>

ZC_GUI_ButtonBase::ZC_GUI_ButtonBase(const ZC_GUI_ObjData& od, ZC_GUI_ButtonFlags _buttonFlags)
    : ZC_GUI_ObjComposite(od),
    buttonFlags(_buttonFlags)
{}

void ZC_GUI_ButtonBase::SetWaitPressLimit_BS(long _waitPressLimit_nanosec)
{
    if (_waitPressLimit_nanosec > 0) waitPressLimit_nanosec = _waitPressLimit_nanosec;
}

void ZC_GUI_ButtonBase::SetPressedInterval_BS(long _pressedInterval_nanosec)
{
    if (_pressedInterval_nanosec > 0) pressedInterval_nanosec = _pressedInterval_nanosec;
}

void ZC_GUI_ButtonBase::StopEventActivity_BS()
{
    if (bs_mouseButton == BS_Pressed) bs_mouseButton = BS_HoldUntilRelease;
    if (bs_keyboardButton == BS_Pressed) bs_keyboardButton = BS_HoldUntilRelease;
    if (this->pObjData->color == color_default) return;     //  nothing to change
    this->pObjData->color = color_default;
    VMapObjData_Obj(pObjData, offsetof(ZC_GUI_ObjData, color), sizeof(ZC_GUI_ObjData::color), &(this->pObjData->color));
}

void ZC_GUI_ButtonBase::SetButtonColor_BS(uint color)
{
    if (this->pObjData->color == color) return;
    this->pObjData->color = color;
    VMapObjData_Obj(this->pObjData, offsetof(ZC_GUI_ObjData, color), sizeof(ZC_GUI_ObjData::color), &(this->pObjData->color));
}