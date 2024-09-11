#pragma once

#include <ZC/GUI/Backend/Button/ZC_GUI_ButtonMouseText.h>
#include "ZC_GUI_DDVariant.h"
#include <ZC/Tools/ZC_uptr.h>
#include <ZC/GUI/Backend/Window/ZC_GUI_WinImmutable.h>

#include <vector>

enum ZC_GUI_DropDownFlag
{
    ZC_GUI_DDF__None            = 0,
    ZC_GUI_DDF__DropIcon        = 1,        //  Button name will contain down arrow icon and have drop down window length (if it's longer then button).
    ZC_GUI_DDF__UnderCursor     = 1 << 1,   //  If button name under cursor open drop down window. The window is drawn while it is under the cursor.
};
typedef int ZC_GUI_DropDownFlags;

struct ZC_GUI_DropDown : public ZC_GUI_ButtonMouseText
{
    ZC_GUI_DropDown(const std::wstring& name, std::vector<std::wstring> variants, ZC_GUI_DropDownFlags dropDownFlags, float width, float height, const ZC_GUI_ColorsDropDown& colorsDropDown = {});

    virtual void VVariantChossed(uint variant_index) {}

private:
    struct DDWindow : public ZC_GUI_WinImmutable
    {
        ZC_GUI_DropDown* pHolder;

        DDWindow(ZC_GUI_DropDown* _pHolder, float width, float height);

        bool VMakeCursorCollision_Obj(float x, float y, ZC_GUI_Obj*& rpObj, ZC_GUI_Obj*& rpScroll) override;
    };

    const bool isUnderCursorFlag;
    ZC_uptr<ZC_GUI_DropDownIcon> upDropDownIcon;
    std::vector<ZC_GUI_DDVariant<ZC_GUI_DropDown>> ddVariants;
    DDWindow ddWindow;

    std::vector<ZC_GUI_DDVariant<ZC_GUI_DropDown>> FillVariants(const std::wstring& name, cosnt std::vector<std::wstring>& variants, float width, float height);

    void VSet_pBL_Obj(const ZC_Vec2<float>& _bl) override;
    void VMoveBL_Obj(float rel_x, float rel_y, int& update_borders) override;
    void VCursorCollisionStart_Obj(float time) override;
    void VCursorCollisionEnd_Obj(float time) override;
};

ZC_GUI_DropDown::ZC_GUI_DropDown(const std::wstring& name, std::vector<std::wstring> variants, ZC_GUI_DropDownFlags dropDownFlags, float width, float height, const ZC_GUI_ColorsDropDown& colorsDropDown = {})
    : ZC_GUI_ButtonMouseText(width, height, ZC_GUI_BF__None,ZC_GUI_TextForButton(ZC_GUI_TextForButton::Indent(ZC_GUI_DropDownIcon::GetTextIndentX(),
        ZC_GUI_TextForButton::Indent::Left), name, true, 0, ZC_GUI_TextAlignment::Left, colorsDropDown.color_text), colorsDropDown.colorsButton),
    isUnderCursorFlag(dropDownFlags & ZC_GUI_DDF__UnderCursor),
    upDropDownIcon(dropDownFlags & ZC_GUI_DDF__DropIcon ? new ZC_GUI_DropDownIcon(colorsDropDown.color_arrow) : nullptr),
    ddVariants(FillVariants(name, variants)),
    ddWindow(this, ddVariants.front().VGetWidth_Obj(), this->GetHeight())
{
    if (dropDownFlags & ZC_GUI_DDF__DropIcon) this->VSetWidth_Obj(ddWindow.VGetWidth_Obj());     //  need use drio down window width in button
    else
    {
        float text_indent_x = ZC_GUI_DropDownIcon::GetTextIndentX();
        float width = text_indent_x + this->textForButton.VGetWidth_Obj() + text_indent_x;
        if (this->VGetWidth_Obj() < width) this->VSetWidth_Obj(width);
    }
}

std::vector<ZC_GUI_DDVariant<ZC_GUI_DropDown>> ZC_GUI_DropDown::FillVariants(const std::wstring& name, cosnt std::vector<std::wstring>& variants, float width, float height)
{
    float text_indent_x = ZC_GUI_DropDownIcon::GetTextIndentX();
    float name_width = text_indent_x + ZC_GUI_TextManager::CalculateWstrWidth(name) + text_indent_x;      //  text_indent_x - distances from button border left and right to text
    if (upDropDownIcon) name_width += text_indent_x + ZC_GUI_DropDownIcon::width;  //  uses drop down icon
    if (width < name_width) width = name_width;

    for (const std::wstring& var : variants)    //  get longest of variants width
    {
        float var_width = text_indent_x + ZC_GUI_TextManager::CalculateWstrWidth(var) + text_indent_x;      //  text_indent_x - distances from button border left and right to text
        if (width < var_width) width = var_width;
    }

    std::vector<ZC_GUI_DDVariant<ZC_GUI_DropDown>> ddVars;
    ddVars.reserve(variants.size());
    for (const std::wstring& var : variants)    //  get longest of buttons width
        ddVars.emplace_back(this, width, height, var);
    
    return ddVars;
}

void ZC_GUI_DropDown::VSet_pBL_Obj(const ZC_Vec2<float>& _bl)
{       //  make it here to avoid that in move ctr
    if (upDropDownIcon) this->VAddObj_Obj(upDropDownIcon.Get(), nullptr);
    for (ZC_GUI_DDVariant<ZC_GUI_DropDown>& ddVar : ddVariants)
    {
        ddVar.pHolder = this;     //  that not pObjHolder!
        ddVariants.AddRow(ZC_GUI_ObjBorder::Row({}, { &ddVar }));
    }

    *(this->pBL) = _bl;
    this->objs.front()->VSet_pBL_Obj(_bl);  //  set bl for textButton
    if (upDropDownIcon) upDropDownIcon->VSet_pBL_Obj(ZC_Vec2<float>(_bl[0] + this->VGetWidth_Obj() - upDropDownIcon->VGetWidth_Obj(), _bl[1] + (this->GetHeight() - upDropDownIcon->GetHeight()) / 2.f));
}

void ZC_GUI_DropDown::VMoveBL_Obj(float rel_x, float rel_y, int& update_borders)
{
    MoveVec2(rel_x, rel_y, *(this->pBL));
    for (ZC_GUI_Obj* pObj : objs) pObj->VMoveBL_Obj(rel_x, rel_y, update_borders);
    ddWindow.VSetDrawState_W(false);
}

void ZC_GUI_DropDown::VCursorCollisionStart_Obj(float time)
{
    if (isUnderCursorFlag)
    {

    }
    else    //  default behaviour
    {
        if (this->pObjData->color == colorsButton.color_button_pressed) return;     //  button pressed, wait while up
        this->SetButtonColor_BS(this->colorsButton.color_button_under_cursor, true);
    }
}

void ZC_GUI_DropDown::VCursorCollisionEnd_Obj(float time)
{
    if (isUnderCursorFlag) return;
        //  default behaviour
    if (this->pObjData->color == colorsButton.color_button_pressed) return;     //  button pressed, wait while up
    this->SetButtonColor_BS(this->colorsButton.color_button, true);
}






ZC_GUI_DropDown::DDWindow::DDWindow(ZC_GUI_DropDown* _pHolder, float width, float height)
    : ZC_GUI_WinImmutable(ZC_WOIData(width, height, 0.f, 0.f, ZC_WOIF__X_Right_Pixel | ZC_WOIF__Y_Top_Pixel), _pHolder->isUnderCursorFlag ? ZC_GUI_WF__None : ZC_GUI_WF__OutAreaClickClose),
    pHolder(_pHolder)
{}

bool ZC_GUI_DropDown::DDWindow::VMakeCursorCollision_Obj(float x, float y, ZC_GUI_Obj*& rpObj, ZC_GUI_Obj*& rpScroll)
{
    if (!VCheckCursorCollision_Obj(x, y)) return false;
    
    if (!(this->pObjHolder)) rpObj = this;     //  top level, it's window
    if (VIsUseScrollEvent_Obj())
    {
        rpScroll = this;
        if (upScroll->VMakeCursorCollision_Obj(x, y, rpObj, rpScroll)) return true;   //  must be before all, at first make scroll buttons collision
    }
    
    for (Row& row : rows)    //  window collision
        for (ZC_GUI_Obj* pObj : row.objs)
            if (pObj->VMakeCursorCollision_Obj(x, y, rpObj, rpScroll)) return true;
    return true;
}