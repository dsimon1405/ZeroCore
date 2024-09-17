#include "ZC_GUI_Switch.h"

    //  ZC_GUI_Switch::Variant    subclass comes first caure all specialization must be before creation of object in ZC_GUI_Switch ctr.

template <>     //  ZC_GUI_ButtonMouse specialization
ZC_GUI_Switch<ZC_GUI_ButtonMouse>::Variant::Variant(const ZC_GUI_UV& uv, float width, float height, const ZC_Vec2<float>& bl, const ZC_GUI_ColorsButton& _colorsButton)
    : ZC_GUI_ButtonBase(ZC_GUI_ObjData(width, height, 0, uv, ZC_GUI_Bindings::bind_tex_Icons), ZC_GUI_BF__None, _colorsButton),
    ZC_GUI_ButtonMouse(width, height, ZC_GUI_BF__None, uv, _colorsButton)
{
    this->VSet_pBL_Obj(bl);
}

template <>     //  ZC_GUI_ButtonMouseAndKeyboard specialization
ZC_GUI_Switch<ZC_GUI_ButtonMouseAndKeyboard>::Variant::Variant(const ZC_GUI_KeyboardUV& keyboardUV, float width, float height, const ZC_Vec2<float>& bl, const ZC_GUI_ColorsButton& _colorsButton)
    : ZC_GUI_ButtonBase(ZC_GUI_ObjData(width, height, 0, keyboardUV.uv, ZC_GUI_Bindings::bind_tex_Icons), ZC_GUI_BF__None, _colorsButton),
    ZC_GUI_ButtonMouseAndKeyboard(width, height, ZC_GUI_BF__None, keyboardUV.buttonId, keyboardUV.uv, _colorsButton)
{
    this->VSet_pBL_Obj(bl);
}

template <>     //  ZC_GUI_ButtonMouseText specialization
ZC_GUI_Switch<ZC_GUI_ButtonMouseText>::Variant::Variant(const std::wstring& name, float width, float height, const ZC_Vec2<float>& bl, const ZC_GUI_ColorsButton& _colorsButton, uint _color_text)
    : ZC_GUI_ButtonBase(ZC_GUI_ObjData(width, height, 0, ZC_GUI_IconUV::quad, ZC_GUI_Bindings::bind_tex_Icons), ZC_GUI_BF__None, _colorsButton),
    ZC_GUI_ButtonMouseText(width, height, ZC_GUI_BF__None, ZC_GUI_TextForButton(ZC_GUI_TFB_Indent(0, ZC_GUI_TFB_Indent::Center), name, true, 0, ZC_GUI_TextAlignment::Left, _color_text), _colorsButton)
{
    this->VSet_pBL_Obj(bl);
}

    //  ZC_GUI_Switch
    
template <>     //  ZC_GUI_ButtonMouse specialization
ZC_GUI_Switch<ZC_GUI_ButtonMouse>::ZC_GUI_Switch(const std::vector<ZC_GUI_UV>& uvs, float width, float height, bool orientation_horizontal, float distance,
        ZC_Function<void(uint)>&& _callback, uint active_variant, const ZC_GUI_ColorsButton& _colorsButton)
    : ZC_GUI_ButtonBase(ZC_GUI_ObjData(width, height, 0, uvs[0], ZC_GUI_Bindings::bind_tex_Icons), ZC_GUI_BF__None, _colorsButton),
    ZC_GUI_ButtonMouse(width, height, ZC_GUI_BF__None, uvs[0], _colorsButton),
    callback(std::move(_callback))
{
    float bl_x = 0.f;
    float bl_y = 0.f;
          //  miss bl for uv setted in ZC_GUI_Switch object
    if (orientation_horizontal) bl_x = width + distance;
    else
    {
        (*(this->pBL))[1] = (distance + height) * (uvs.size() - 1);
        bl_y = this->Get_bl_Obj()[1] - (distance + height);
    }

    variants.reserve(uvs.size() - 1);
    for (size_t i = 1; i < uvs.size(); ++i)
    {
        this->VAddObj_Obj(&(variants.emplace_back(Variant(uvs[i], width, height, { bl_x, bl_y }, _colorsButton))));
        orientation_horizontal ? bl_x += width + distance : bl_y -= (distance + height);
    }
    MakeVariantActive(active_variant, false);
}

template <>     //  ZC_GUI_ButtonMouseAndKeyboard specialization
ZC_GUI_Switch<ZC_GUI_ButtonMouseAndKeyboard>::ZC_GUI_Switch(const std::vector<ZC_GUI_KeyboardUV>& keyboardUVs, float width, float height, bool orientation_horizontal, float distance,
        ZC_Function<void(uint)>&& _callback, uint active_variant, const ZC_GUI_ColorsButton& _colorsButton)
    : ZC_GUI_ButtonBase(ZC_GUI_ObjData(width, height, 0, keyboardUVs[0].uv, ZC_GUI_Bindings::bind_tex_Icons), ZC_GUI_BF__None, _colorsButton),
    ZC_GUI_ButtonMouseAndKeyboard(width, height, ZC_GUI_BF__None, keyboardUVs[0].buttonId, keyboardUVs[0].uv, _colorsButton),
    callback(std::move(_callback))
{
    float bl_x = 0.f;
    float bl_y = 0.f;
          //  miss bl for uv setted in ZC_GUI_Switch object
    if (orientation_horizontal) bl_x = width + distance;
    else
    {
        (*(this->pBL))[1] = (distance + height) * (keyboardUVs.size() - 1);
        bl_y = this->Get_bl_Obj()[1] - (distance + height);
    }

    variants.reserve(keyboardUVs.size() - 1);
    for (size_t i = 1; i < keyboardUVs.size(); ++i)
    {
        this->VAddObj_Obj(&(variants.emplace_back(Variant(keyboardUVs[i], width, height, { bl_x, bl_y }, _colorsButton))));
        orientation_horizontal ? bl_x += width + distance : bl_y -= (distance + height);
    }
    MakeVariantActive(active_variant, false);
}

template <>     //  ZC_GUI_ButtonMouseText specialization
ZC_GUI_Switch<ZC_GUI_ButtonMouseText>::ZC_GUI_Switch(const std::vector<std::wstring>& names, float width, float height, bool orientation_horizontal, float distance,
        ZC_Function<void(uint)>&& _callback, uint active_variant, const ZC_GUI_ColorsButton& _colorsButton, uint _color_text)
    : ZC_GUI_ButtonBase(ZC_GUI_ObjData(CalculateNamesWidth(names, width), height, 0, ZC_GUI_IconUV::quad, ZC_GUI_Bindings::bind_tex_Icons), ZC_GUI_BF__None, _colorsButton),
    ZC_GUI_ButtonMouseText(this->VGetWidth_Obj(), height, ZC_GUI_BF__None,
        ZC_GUI_TextForButton(ZC_GUI_TFB_Indent(0, ZC_GUI_TFB_Indent::Center), names[0], true, 0, ZC_GUI_TextAlignment::Left, _color_text), _colorsButton),
    callback(std::move(_callback))
{
    width = this->VGetWidth_Obj();
    height = this->GetHeight();
    float bl_x = 0.f;
    float bl_y = 0.f;
          //  miss bl for uv setted in ZC_GUI_Switch object
    if (orientation_horizontal) bl_x = width + distance;
    else
    {
        (*(this->pBL))[1] = (distance + height) * (names.size() - 1);
        bl_y = this->Get_bl_Obj()[1] - (distance + height);
    }

    variants.reserve(names.size() - 1);
    for (size_t i = 1; i < names.size(); ++i)
    {
        this->VAddObj_Obj(&(variants.emplace_back(Variant(names[i], width, height, { bl_x, bl_y }, _colorsButton, _color_text))));
        orientation_horizontal ? bl_x += width + distance : bl_y -= (distance + height);
    }
    MakeVariantActive(active_variant, false);
}