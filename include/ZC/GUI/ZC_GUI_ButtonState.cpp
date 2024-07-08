#include <ZC/GUI/ZC_GUI_ButtonState.h>

ZC_GUI_ButtonState::ZC_GUI_ButtonState(const ZC_GUI_ObjData& od)
    : ZC_GUI_Obj(od)
{}

void ZC_GUI_ButtonState::StopEventActivity_BS()
{
    if (bs_mouseButton == BS_Pressed) bs_mouseButton = BS_HoldUntilRelease;
    if (bs_keyboardButton == BS_Pressed) bs_keyboardButton = BS_HoldUntilRelease;
    if (this->pObjData->color == color_default) return;     //  nothing to change
    this->pObjData->color = color_default;
    VMapObjData_Obj(pObjData, offsetof(ZC_GUI_ObjData, color), sizeof(ZC_GUI_ObjData::color), &(this->pObjData->color));
}