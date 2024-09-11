#pragma once

#include <ZC/GUI/Backend/Button/ZC_GUI_ButtonMouseText.h>
#include <ZC/GUI/Backend/Window/ZC_GUI_WinImmutable.h>
#include "ZC_GUI_DDVariant.h"

/*
Drop down class with switch system. One of variants allways choosen and shown in button for drop down open. Takes all variants in vector of std::wstring. If variant changes calls VVariantChoosed(int new_variant) with
index from vector of the selected variant in new_variant.
*/
struct ZC_GUI_SwitchDropDown : public ZC_GUI_ButtonMouseText
{
    ZC_GUI_SwitchDropDown(const std::vector<std::wstring>& variants, uint active_variant, float width, float height, const ZC_GUI_ColorsDropDown& colorsDropDownSwitch = {});

    ZC_GUI_SwitchDropDown(ZC_GUI_SwitchDropDown&& dds);
    
    virtual void VVariantChoosed(uint variant_index) {}

    void MakeVariantActive(uint index);
        //  for private call from ZC_GUI_DDVariant::VLeftButtonUp_BM()
    void VariantChoosed(ZC_GUI_DDVariant<ZC_GUI_SwitchDropDown>* pDDVariant_choosed);

private:
    ZC_GUI_DropDownIcon obj_dd_icon;
    std::vector<ZC_GUI_DDVariant<ZC_GUI_SwitchDropDown>> ddVariants;
    ZC_GUI_WinImmutable ddWindow;
    ZC_GUI_DDVariant<ZC_GUI_SwitchDropDown>* pDDVariant_active = nullptr;

    void VSet_pBL_Obj(const ZC_Vec2<float>& _bl) override;
    void VLeftButtonUp_BM(float time) override;
    void VMoveBL_Obj(float rel_x, float rel_y, int& update_borders) override;

    float CalculateWidth(const std::vector<std::wstring>& variants, float width);
    std::vector<ZC_GUI_DDVariant<ZC_GUI_SwitchDropDown>> Fill_variants(const std::vector<std::wstring> variants);
    void SetActiveBMTDrawState(bool needDraw);
    void UpdatePos_ddWindow();
};