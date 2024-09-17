#include "ZC_GUI__DropDown.h"

ZC_GUI__DropDown::ZC_GUI__DropDown(const std::wstring& name, const std::vector<std::wstring>& variants, float width, float height, ZC_GUI_DropDownFlags dropDownFlags, ZC_Function<void(uint)>&& _callback,
        const ZC_GUI_ColorsDropDown& colorsDropDown)
    : ZC_GUI__Obj<ZC_GUI_DropDown>(ZC_GUI_DropDown(name, variants, width, height, dropDownFlags, std::move(_callback), colorsDropDown))
{}