#include "ZC_GUI_BranchOpenable.h"

#include <ZC/GUI/Backend/Config/ZC_GUI_IconUV.h>
#include <ZC/GUI/Backend/Config/ZC_GUI_Bindings.h>
#include "ZC_GUI_Tree.h"

ZC_GUI_BranchOpenable::ZC_GUI_BranchOpenable(const std::wstring& name, bool isOpen, const ColorsBranchOpenable& branchOpenableColors)
    : ZC_GUI_ButtonBase(ZC_GUI_ObjData(0.f, 0.f, 0, ZC_GUI_IconUV::button, ZC_GUI_Bindings::bind_tex_Icons), ZC_GUI_BF_M__DoubleCLick, branchOpenableColors.colorsBranch.colorsBranchButton),
    ZC_GUI_BranchSimple(name, branchOpenableColors.colorsBranch),
    arrowButton(float(ZC_GUI_TextManager::GetFontHeight() / 1.5f), isOpen, branchOpenableColors.colorsArrowButton)
{
    this->VAddObj_Obj(&arrowButton, nullptr);
}

ZC_GUI_BranchOpenable::ZC_GUI_BranchOpenable(ZC_GUI_BranchOpenable&& bo)
    : ZC_GUI_ButtonBase(static_cast<ZC_GUI_ButtonBase&&>(bo)),
    ZC_GUI_BranchSimple(static_cast<ZC_GUI_BranchSimple&&>(bo)),
    arrowButton(std::move(bo.arrowButton))
{
    this->VAddObj_Obj(&arrowButton, nullptr);
}

void ZC_GUI_BranchOpenable::VSet_pBL_Obj(const ZC_Vec2<float>& _bl)
{
    arrowButton.VSet_pBL_Obj({ _bl[0], _bl[1] + (this->GetHeight() - arrowButton.GetHeight()) / 2.f });      //  at first open/close button
    *pBL = { _bl[0] + arrowButton.VGetWidth_Obj(), _bl[1] };    //  text button righter, on half font height
    this->objs.front()->VSet_pBL_Obj(*this->pBL);   //  set texts bl
}

void ZC_GUI_BranchOpenable::VLeftButtonDoubleClick_BM(float time)
{
    dynamic_cast<ZC_GUI_Tree*>(this->pObjHolder)->ChangeOpenableBranchState(this);
    arrowButton.ChangeArrow();
}

bool ZC_GUI_BranchOpenable::VIsOpenable_Br()
{
    return true;
}

bool ZC_GUI_BranchOpenable::VIsOpen_Br()
{
    return arrowButton.IsOpen();
}

bool ZC_GUI_BranchOpenable::VMakeCursorCollision_Obj(float x, float y, ZC_GUI_Obj*& rpObj, ZC_GUI_Obj*& rpScroll)
{
    return MakeCursorCollision_Obj(x, y, rpObj, rpScroll) || arrowButton.MakeCursorCollision_Obj(x, y, rpObj, rpScroll);
}


    //  ZC_GUI_BranchOpenable::ColorsBranchOpenable

ZC_GUI_BranchOpenable::ColorsBranchOpenable::ColorsBranchOpenable(const ZC_GUI_BranchSimple::ColorsBranch& _colorsBranch, const ZC_GUI_ButtonBase::ColorsButton& _colorsArrowButton)
    : colorsBranch(_colorsBranch),
    colorsArrowButton(_colorsArrowButton)
{}


    //  ZC_GUI_BranchOpenable::ArrowButton

ZC_GUI_BranchOpenable::ArrowButton::ArrowButton(float size, bool isOpen, const ZC_GUI_ButtonBase::ColorsButton& colorsArrowButton)
    : ZC_GUI_ButtonBase(ZC_GUI_ObjData(size, size, 0, isOpen ? ZC_GUI_IconUV::arrow_down : ZC_GUI_IconUV::arrow_right, ZC_GUI_Bindings::bind_tex_Icons), ZC_GUI_BF__None, colorsArrowButton),
    ZC_GUI_ButtonMouse(size, size, ZC_GUI_BF__None)
{}

bool ZC_GUI_BranchOpenable::ArrowButton::IsOpen()
{
    return this->pObjData->uv.bl[0] == ZC_GUI_IconUV::arrow_down.bl[0];
}

void ZC_GUI_BranchOpenable::ArrowButton::ChangeArrow()
{
    this->pObjData->uv = IsOpen() ? ZC_GUI_IconUV::arrow_right : ZC_GUI_IconUV::arrow_down;
    this->VMapObjData_Obj(this->pObjData, offsetof(ZC_GUI_ObjData, uv), sizeof(ZC_GUI_ObjData::uv), &(this->pObjData->uv));
}

void ZC_GUI_BranchOpenable::ArrowButton::VLeftButtonUp_BM(float time)
{
    dynamic_cast<ZC_GUI_Tree*>(this->pObjHolder->pObjHolder)->ChangeOpenableBranchState(dynamic_cast<ZC_GUI_BranchOpenable*>(this->pObjHolder));
    ChangeArrow();
}