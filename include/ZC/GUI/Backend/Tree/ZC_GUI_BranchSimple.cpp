#include "ZC_GUI_BranchSimple.h"

#include <ZC/GUI/Backend/ZC_GUI_IconUV.h>
#include <ZC/GUI/Backend/ZC_GUI_Bindings.h>
#include "ZC_GUI_Tree.h"

ZC_GUI_BranchSimple::ZC_GUI_BranchSimple(const std::wstring& name, const ColorsBranch& colorsBranch)
    : ZC_GUI_ButtonBase(ZC_GUI_ObjData(0.f, 0.f, 0, ZC_GUI_IconUV::button, ZC_GUI_Bindings::bind_tex_Icons), ZC_GUI_BF__None, colorsBranch.colorsBranchButton),
    ZC_GUI_ButtonMouseText(0.f, 0.f, ZC_GUI_BF__None, ZC_GUI_TextForButton(ZC_GUI_TextForButton::Indent(0.f, ZC_GUI_TextForButton::Indent::Left), name, false, 0, ZC_GUI_TextAlignment::Left, colorsBranch.color_text))
{}

ZC_GUI_BranchSimple::ZC_GUI_BranchSimple(ZC_GUI_BranchSimple&& bs)
    : ZC_GUI_ButtonBase(static_cast<ZC_GUI_ButtonBase&&>(bs)),
    ZC_GUI_ButtonMouseText(static_cast<ZC_GUI_ButtonMouseText&&>(bs))
{}

void ZC_GUI_BranchSimple::VLeftButtonUp_BM(float time)
{
    if (dynamic_cast<ZC_GUI_Tree*>(this->pObjHolder)->MakeActive(this)) VBecameActive_Br();
}

bool ZC_GUI_BranchSimple::VIsOpenable_Br()
{
    return false;
}

bool ZC_GUI_BranchSimple::VIsOpen_Br()
{
    return false;
}

void ZC_GUI_BranchSimple::UpdateName(const std::wstring& name)
{
    ZC_GUI_TextManager::Text* pText = ZC_GUI_TextManager::GetText(name, false, 0, ZC_GUI_TextAlignment::Left);
    this->UpdateText_BMT(pText);

    this->pObjData->width = pText->width;
    this->VMapObjData_Obj(this->pObjData, offsetof(ZC_GUI_ObjData, width), sizeof(ZC_GUI_ObjData::width), &(this->pObjData->width));
}

void ZC_GUI_BranchSimple::SetMaxSymbolsCount(int count)
{
    if (count > max_symbols) max_symbols = count;
}


    //  ZC_GUI_BranchSimple::ColorsBranch

ZC_GUI_BranchSimple::ColorsBranch::ColorsBranch(uint _color_text, const ZC_GUI_ButtonBase::ColorsButton& _colorsBranchButton)
    : color_text(_color_text),
    colorsBranchButton(_colorsBranchButton)
{}