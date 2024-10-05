#pragma once

#include <ZC/GUI/Backend/Button/ZC_GUI_ButtonMouseAndKeyboard.h>
#include <ZC/GUI/Backend/Button/ZC_GUI_ButtonMouseText.h>
#include <ZC/GUI/Backend/Config/ZC_GUI_Bindings.h>
#include <ZC/GUI/Backend/Config/ZC_GUI_IconUV.h>
#include <ZC/Tools/Function/ZC_Function.h>

#include <vector>

template <typename T>
concept ZC_GUI_cSwitch = std::same_as<T, ZC_GUI_ButtonMouse>
                            || std::same_as<T, ZC_GUI_ButtonMouseAndKeyboard>
                            || std::same_as<T, ZC_GUI_ButtonMouseText>;

struct ZC_GUI_KeyboardUV
{
    ZC_GUI_UV uv;
    ZC_ButtonID buttonId;
};

template <ZC_GUI_cSwitch TSwitch>    //  TSwitch is ZC_GUI_ButtonMouse or ZC_GUI_ButtonMouseAndKeyboard
struct ZC_GUI_Switch : public TSwitch
{
    struct Variant : public TSwitch
    {
            //  ctr with ZC_GUI_ButtonMouse specialization
        Variant(const ZC_GUI_UV& uv, float width, float height, const ZC_Vec2<float>& bl, const ZC_GUI_ColorsButton& _colorsButton);
            //  ctr with ZC_GUI_ButtonMouseAndKeyboard specialization
        Variant(const ZC_GUI_KeyboardUV& keyboardUV, float width, float height, const ZC_Vec2<float>& bl, const ZC_GUI_ColorsButton& _colorsButton);
            //  ctr with ZC_GUI_ButtonMouseText specialization
        Variant(const std::wstring& name, float width, float height, const ZC_Vec2<float>& bl, const ZC_GUI_ColorsButton& _colorsButton, uint _color_text);

        bool VMouseButtonLeftDown_Obj(float time) override;
        void VMouseButtonLeftUp_Obj(float time) override;
        bool VKeyboardButtonDown_Obj(float time) override;
        void VKeyboardButtonUp_Obj(float time) override;
    };

        //  ctr with ZC_GUI_ButtonMouse specialization
    ZC_GUI_Switch(const std::vector<ZC_GUI_UV>& uvs, float width, float height, bool orientation_horizontal, float distance, ZC_Function<void(uint)>&& _callback,
        uint active_variant = UINT_MAX, const ZC_GUI_ColorsButton& _colorsButton = {});
        //  ctr with ZC_GUI_ButtonMouseAndKeyboard specialization
    ZC_GUI_Switch(const std::vector<ZC_GUI_KeyboardUV>& keyboardUVs, float width, float height, bool orientation_horizontal, float distance,
        ZC_Function<void(uint)>&& _callback, uint active_variant = UINT_MAX, const ZC_GUI_ColorsButton& _colorsButton = {});
        //  ctr with ZC_GUI_ButtonMouseText specialization
    ZC_GUI_Switch(const std::vector<std::wstring>& names, float width, float height, bool orientation_horizontal, float distance, ZC_Function<void(uint)>&& _callback,
        uint active_variant = UINT_MAX, const ZC_GUI_ColorsButton& _colorsButton = {}, uint _color_text = ZC_GUI_Colors::dropDownSwitch_text);

    ZC_GUI_Switch(ZC_GUI_Switch&& sw);

    void MakeVariantActive(uint index, bool use_callback);

private:
    typedef typename ZC_GUI_ButtonBase::ButtonState ZC_GUI_BB_BS;

    std::vector<Variant> variants;
    TSwitch* pBM_active = nullptr;
    ZC_Function<void(uint)> callback;

    float VGetWidthComposite_Obj() override;
    float VGetHeightComposite_Obj() override;
    void VSet_pBL_Obj(const ZC_Vec2<float>& _bl) override;
    bool VMakeCursorCollision_Obj(float x, float y, ZC_GUI_Obj*& rpObj, ZC_GUI_Obj*& rpScroll) override;
    bool VMouseButtonLeftDown_Obj(float time) override;
    void VMouseButtonLeftUp_Obj(float time) override;
    bool VKeyboardButtonDown_Obj(float time) override;
    void VKeyboardButtonUp_Obj(float time) override;

    void MakeActive(TSwitch* _pBM_active, bool use_callback);
        //  find longest of names pixel width
    float CalculateNamesWidth(const std::vector<std::wstring>& names, float width);
};


    //  ZC_GUI_Switch

template <ZC_GUI_cSwitch TSwitch>
ZC_GUI_Switch<TSwitch>::ZC_GUI_Switch(ZC_GUI_Switch<TSwitch>&& sw)
    : ZC_GUI_ButtonBase(static_cast<ZC_GUI_ButtonBase&&>(sw)),
    TSwitch(static_cast<TSwitch&&>(sw)),
    variants(std::move(sw.variants)),
    callback(std::move(sw.callback))
{
    for (Variant& var : variants)
        this->VAddObj_Obj(&var);
}

template <ZC_GUI_cSwitch TSwitch>
void ZC_GUI_Switch<TSwitch>::MakeVariantActive(uint index, bool use_callback)
{
    if (index > variants.size()) return;    //  out of range
    MakeActive(index == 0 ? dynamic_cast<TSwitch*>(this) : dynamic_cast<TSwitch*>(&(variants[index - 1])), use_callback);    //  index 0 is is ZC_GUI_Switch, not into the vector
}

template <ZC_GUI_cSwitch TSwitch>
float ZC_GUI_Switch<TSwitch>::VGetWidthComposite_Obj()
{
    Variant& last = variants.back();
    return (last.Get_bl_Obj()[0] + last.VGetWidth_Obj()) - this->Get_bl_Obj()[0];   //  orientation no metter, last object will be righter or at hte same vertical positoin as and first
}

template <ZC_GUI_cSwitch TSwitch>
float ZC_GUI_Switch<TSwitch>::VGetHeightComposite_Obj()
{
    return (this->Get_bl_Obj()[1] + this->GetHeight()) - variants.back().Get_bl_Obj()[1];   //  orientation no metter, last object will be lower or at hte same horizontal positoin as and first
}

template <ZC_GUI_cSwitch TSwitch>
void ZC_GUI_Switch<TSwitch>::VSet_pBL_Obj(const ZC_Vec2<float>& _bl)
{       //  relative bl position seted in ctr, now only move all objects to concretete positoin 
    if constexpr (std::same_as<TSwitch, ZC_GUI_ButtonMouseText>)
    {
        this->Set_pBL_BMT(_bl);
    }
    else *(this->pBL) += _bl;
    for (Variant& var : variants) var.VSet_pBL_Obj(var.Get_bl_Obj() + _bl);
}

template <ZC_GUI_cSwitch TSwitch>
bool ZC_GUI_Switch<TSwitch>::VMakeCursorCollision_Obj(float x, float y, ZC_GUI_Obj*& rpObj, ZC_GUI_Obj*& rpScroll)
{
    if (this->MakeCursorCollision_Obj(x, y, rpObj, rpScroll)) return true;
    for (Variant& var : variants)
        if (var.VMakeCursorCollision_Obj(x, y, rpObj, rpScroll)) return true;
    return false;
}

template <ZC_GUI_cSwitch TSwitch>
bool ZC_GUI_Switch<TSwitch>::VMouseButtonLeftDown_Obj(float time)
{
    if (this->bs_mouseButton == ZC_GUI_BB_BS::BS_HoldUntilRelease) return false;  //  don't do anything while uses another button down event
    if (this->bs_keyboardButton != ZC_GUI_BB_BS::BS_Released)
    {
        this->bs_mouseButton = ZC_GUI_BB_BS::BS_HoldUntilRelease;
        return false;
    }
    this->bs_mouseButton = ZC_GUI_BB_BS::BS_Pressed;
    return true;
}

template <ZC_GUI_cSwitch TSwitch>
void ZC_GUI_Switch<TSwitch>::VMouseButtonLeftUp_Obj(float time)
{
    if (this->bs_mouseButton == ZC_GUI_BB_BS::BS_Pressed && this->CheckCursorCollision_Obj())
        MakeActive(dynamic_cast<TSwitch*>(this), true);
    this->bs_mouseButton = ZC_GUI_BB_BS::BS_Released;
}

template <ZC_GUI_cSwitch TSwitch>
bool ZC_GUI_Switch<TSwitch>::VKeyboardButtonDown_Obj(float time)
{
    if (this->bs_keyboardButton == ZC_GUI_BB_BS::BS_HoldUntilRelease) return false;  //  don't do anything while uses another button down event
    if (this->bs_mouseButton != ZC_GUI_BB_BS::BS_Released)
    {
        this->bs_keyboardButton = ZC_GUI_BB_BS::BS_HoldUntilRelease;
        return false;
    }
    this->bs_keyboardButton = ZC_GUI_BB_BS::BS_Pressed;
    return true;
}

template <ZC_GUI_cSwitch TSwitch>
void ZC_GUI_Switch<TSwitch>::VKeyboardButtonUp_Obj(float time)
{
    if (this->bs_keyboardButton == ZC_GUI_BB_BS::BS_Pressed)
        MakeActive(dynamic_cast<TSwitch*>(this), true);
    this->bs_keyboardButton = ZC_GUI_BB_BS::BS_Released;
}

template <ZC_GUI_cSwitch TSwitch>
void ZC_GUI_Switch<TSwitch>::MakeActive(TSwitch* _pBM_active, bool use_callback)
{
    if (pBM_active && pBM_active == _pBM_active) return;
    if (pBM_active) pBM_active->SetColor_Obj(pBM_active->colorsButton.color_button, true);
    pBM_active = _pBM_active;
    pBM_active->SetColor_Obj(pBM_active->colorsButton.color_button_pressed, true);
    if (use_callback) callback(pBM_active == this ? 0 : pBM_active - dynamic_cast<TSwitch*>(&(variants.front())) + 1);  //  first (0) variant is ZC_GUI_Switch, not into the vector
}

template <ZC_GUI_cSwitch TSwitch>
float ZC_GUI_Switch<TSwitch>::CalculateNamesWidth(const std::vector<std::wstring>& names, float width)
{
    for (const std::wstring& name : names)
    {
        float temp_width = ZC_GUI_TextManager::CalculateWstrWidth(name);
        if (temp_width > width) width = temp_width;
    }
    return width;
}


    //  ZC_GUI_Switch::Variant

template <ZC_GUI_cSwitch TSwitch>
bool ZC_GUI_Switch<TSwitch>::Variant::VMouseButtonLeftDown_Obj(float time)
{
    if (this->bs_mouseButton == ZC_GUI_BB_BS::BS_HoldUntilRelease) return false;  //  don't do anything while uses another button down event
    if (this->bs_keyboardButton != ZC_GUI_BB_BS::BS_Released)
    {
        this->bs_mouseButton = ZC_GUI_BB_BS::BS_HoldUntilRelease;
        return false;
    }
    this->bs_mouseButton = ZC_GUI_BB_BS::BS_Pressed;
    return true;
}

template <ZC_GUI_cSwitch TSwitch>
void ZC_GUI_Switch<TSwitch>::Variant::VMouseButtonLeftUp_Obj(float time)
{
    if (this->bs_mouseButton == ZC_GUI_BB_BS::BS_Pressed && this->CheckCursorCollision_Obj())
        dynamic_cast<ZC_GUI_Switch<TSwitch>*>(this->pObjHolder)->MakeActive(dynamic_cast<TSwitch*>(this), true);
    this->bs_mouseButton = ZC_GUI_BB_BS::BS_Released;
}

template <ZC_GUI_cSwitch TSwitch>
bool ZC_GUI_Switch<TSwitch>::Variant::VKeyboardButtonDown_Obj(float time)
{
    if (this->bs_keyboardButton == ZC_GUI_BB_BS::BS_HoldUntilRelease) return false;  //  don't do anything while uses another button down event
    if (this->bs_mouseButton != ZC_GUI_BB_BS::BS_Released)
    {
        this->bs_keyboardButton = ZC_GUI_BB_BS::BS_HoldUntilRelease;
        return false;
    }
    this->bs_keyboardButton = ZC_GUI_BB_BS::BS_Pressed;
    return true;
}

template <ZC_GUI_cSwitch TSwitch>
void ZC_GUI_Switch<TSwitch>::Variant::VKeyboardButtonUp_Obj(float time)
{
    if (this->bs_keyboardButton == ZC_GUI_BB_BS::BS_Pressed)
        dynamic_cast<ZC_GUI_Switch<TSwitch>*>(this->pObjHolder)->MakeActive(dynamic_cast<TSwitch*>(this), true);
    this->bs_keyboardButton = ZC_GUI_BB_BS::BS_Released;
}