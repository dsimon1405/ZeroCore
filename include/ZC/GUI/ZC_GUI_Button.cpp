#include <ZC/GUI/ZC_GUI_Button.h>

#include <ZC/GUI/ZC_GUI_Depth.h>
#include "ZC_GUI_IconUV.h"

ZC_GUI_Button::ZC_GUI_Button(float width, float height)
    : ZC_GUI_Obj(ZC_GUI_ObjData{ .width = width, .height = height, .depth = ZC_GUI_Depth::button, .uv = ZC_GUI_IconUV::button })
{}

ZC_GUI_Button::ZC_GUI_Button(float width, float height, const ZC_GUI_UV& uv)
    : ZC_GUI_Obj(ZC_GUI_ObjData{ .width = width, .height = height, .depth = ZC_GUI_Depth::button, .uv = uv })
{}

bool ZC_GUI_Button::VIsDrawing_Obj() const noexcept
{
    return true;
}

bool ZC_GUI_Button::VMakeCursorCollision_EO(float x, float y, ZC_GUI_EventObj*& rpWindow, ZC_GUI_EventObj*& rpObj, ZC_GUI_EventObj*& rpScroll)
{
    return MakeCursorCollision_Obj(x, y, rpWindow, rpObj, rpScroll);
}

bool ZC_GUI_Button::VCheckCursorCollision_EO(float x, float y)
{ 
    return Collision(x, y, (*this->pBL)[0], (*this->pBL)[1], (*this->pBL)[0] + this->pObjData->width, (*this->pBL)[1] + this->pObjData->height);
}