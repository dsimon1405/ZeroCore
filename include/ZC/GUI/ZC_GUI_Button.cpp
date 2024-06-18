#include <ZC/GUI/ZC_GUI_Button.h>

#include <ZC/GUI/ZC_GUI_Depth.h>

ZC_GUI_Button::ZC_GUI_Button(float width, float height)
    : ZC_GUI_Obj(ZC_GUI_ObjData{.width = width, .height = height, .depth = ZC_GUI_Depth::button, .uv = {.bl{0.f, 0.f}, .tr{1.f,1.f}}})
{}

bool ZC_GUI_Button::VMakeCursorCollision_EO(float x, float y, ZC_GUI_Window*& rpWindow, ZC_GUI_EventObj*& rpObj)
{
    if (Collision(x, y, (*this->pBL)[0], (*this->pBL)[1], (*this->pBL)[0] + this->pObjData->width, (*this->pBL)[1] + this->pObjData->height))
    {
        rpObj = this;
        return true;
    }
    return false;
}

bool ZC_GUI_Button::VCheckCursorCollision_EO(float x, float y)
{ 
    return Collision(x, y, (*this->pBL)[0], (*this->pBL)[1], (*this->pBL)[0] + this->pObjData->width, (*this->pBL)[1] + this->pObjData->height);
}