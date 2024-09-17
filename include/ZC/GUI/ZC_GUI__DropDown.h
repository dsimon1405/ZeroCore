#pragma once

#include "ZC_GUI__Obj.h"
#include <ZC/GUI/Backend/ZC_GUI_DropDown.h>

struct ZC_GUI__DropDown : public ZC_GUI__Obj<ZC_GUI_DropDown>
{
    /*
    Params:
    - name - buttons name.
    - variants - variant vector. Indexes from that vector uses as parameter in VVariantChoosed(variant_index) method.
    - width - pizel width of name button and variants. Ignorce for name button if less then name pixel width (see ZC_GUI_DropDownFlag::ZC_GUI_DDF__DropIcon), for variants if less then longest variant pixel length.
    - dropDownFlags - see ZC_GUI_DropDownFlag for info.
    - _callback - function calls on choosing some variant. Parameter - uint, will contain index (from variants vector) of choosed variant.
    - colorsDropDown - drop down collors (may stay default).
    */
    ZC_GUI__DropDown(const std::wstring& name, const std::vector<std::wstring>& variants, float width, float height, ZC_GUI_DropDownFlags dropDownFlags, ZC_Function<void(uint)>&& _callback,
        const ZC_GUI_ColorsDropDown& colorsDropDown = {});
};