#include <ZC/GUI/ZC_GUI_ButtonMouse.h>

#include "ZC_GUI_IconUV.h"

ZC_GUI_ButtonMouse::ZC_GUI_ButtonMouse(float width, float height, ZC_GUI_BM__Flags _bm_flags)
    : ZC_GUI_ButtonMouse(width, height, _bm_flags, ZC_GUI_IconUV::button)
{}

ZC_GUI_ButtonMouse::ZC_GUI_ButtonMouse(float width, float height, ZC_GUI_BM__Flags _bm_flags, const ZC_GUI_UV& uv)
    : ZC_GUI_ButtonState(ZC_GUI_ObjData{ .width = width, .height = height, .uv = uv }),
    bm_flags(_bm_flags)
{}

bool ZC_GUI_ButtonMouse::VIsDrawing_Obj() const noexcept
{
    return true;
}

bool ZC_GUI_ButtonMouse::VMakeCursorCollision_Obj(float x, float y, ZC_GUI_Obj*& rpWindow, ZC_GUI_Obj*& rpObj, ZC_GUI_Obj*& rpScroll)
{
    return MakeCursorCollision_Obj(x, y, rpWindow, rpObj, rpScroll);
}

// bool ZC_GUI_ButtonMouse::VCheckCursorCollision_Obj(float x, float y)
// { 
//     return Collision(x, y, (*this->pBL)[0], (*this->pBL)[1], (*this->pBL)[0] + this->pObjData->width, (*this->pBL)[1] + this->pObjData->height);
// }