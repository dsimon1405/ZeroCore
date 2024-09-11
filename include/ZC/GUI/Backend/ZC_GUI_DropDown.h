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
    const bool isUnderCursorFlag;

    ZC_GUI_DropDown(const std::wstring& name, const std::vector<std::wstring>& variants, float width, float height, ZC_GUI_DropDownFlags dropDownFlags, const ZC_GUI_ColorsDropDown& colorsDropDown = {});

    ZC_GUI_DropDown(ZC_GUI_DropDown&& dd);

    virtual void VVariantChoosed(uint variant_index) {}
        //  for private call from ZC_GUI_DDVariant::VLeftButtonUp_BM()
    void VariantChoosed(ZC_GUI_DDVariant<ZC_GUI_DropDown>* pDDVariant_choosed);

private:
    ZC_uptr<ZC_GUI_DropDownIcon> upDropDownIcon;
    std::vector<ZC_GUI_DDVariant<ZC_GUI_DropDown>> ddVariants;
    ZC_GUI_WinImmutable ddWindow;

    std::vector<ZC_GUI_DDVariant<ZC_GUI_DropDown>> FillVariants(const std::wstring& name, const std::vector<std::wstring>& variants, float width, float height);

    void VSet_pBL_Obj(const ZC_Vec2<float>& _bl) override;
    void VMoveBL_Obj(float rel_x, float rel_y, int& update_borders) override;
    void VCursorCollisionStart_Obj(float time) override;
    void VCursorCollisionEnd_Obj(float time) override;
    bool VMouseButtonLeftDown_Obj(float time) override;
    void VMouseButtonLeftUp_Obj(float time) override;

    void UpdatePos_ddWindow();
};