#pragma once

#include "ZC_GUI__VariantChoosed.h"
#include "ZC_GUI__Obj.h"
#include <ZC/GUI/Backend/ZC_GUI_DropDown.h>

/*
Drop down struct.
Heir must override method: void VVariantChoosed(uint variant_index). Parameter (variant_index) contain choosed variant index (from variants vector).
*/
struct ZC_GUI__DropDown : public ZC_GUI__Obj<ZC_GUI__VariantChoosed<ZC_GUI__DropDown, ZC_GUI_DropDown>>
{
    /*
    Params:
    - name - buttons name.
    - variants - variant vector. Indexes from that vector uses as parameter in VVariantChoosed(variant_index) method.
    - width - pizel width of name button and variants. Ignorce for name button if less then name pixel width (see ZC_GUI_DropDownFlag::ZC_GUI_DDF__DropIcon), for variants if less then longest variant pixel length.
    - dropDownFlags - see ZC_GUI_DropDownFlag for info.
    - colorsDropDown - drop down collors (may be default).
    */
    ZC_GUI__DropDown(const std::wstring& name, const std::vector<std::wstring>& variants, float width, float height, ZC_GUI_DropDownFlags dropDownFlags, const ZC_GUI_ColorsDropDown& colorsDropDown = {});
    
    /*
    Method must be averride. Calls on choosing variant.
    
    Params:
    - variant_index - choosed variant index (from variants vector).
    */
    virtual void VVariantChoosed(uint variant_index) {}
};

ZC_GUI__DropDown::ZC_GUI__DropDown(const std::wstring& name, const std::vector<std::wstring>& variants, float width, float height, ZC_GUI_DropDownFlags dropDownFlags, const ZC_GUI_ColorsDropDown& colorsDropDown)
    : ZC_GUI__Obj<ZC_GUI__VariantChoosed<ZC_GUI__DropDown, ZC_GUI_DropDown>>(ZC_GUI__VariantChoosed<ZC_GUI__DropDown, ZC_GUI_DropDown>(this, ZC_GUI_DropDown(name, variants, width, height, dropDownFlags, colorsDropDown)))
{}