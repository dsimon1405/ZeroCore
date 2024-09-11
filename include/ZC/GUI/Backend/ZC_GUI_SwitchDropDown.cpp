#include "ZC_GUI_SwitchDropDown.h"

#include "ZC_GUI_IconUV.h"
#include "ZC_GUI_Bindings.h"
#include <ZC/GUI/Backend/Text/ZC_GUI_TextManager.h>
#include <ZC/GUI/Backend/ZC_GUI.h>

ZC_GUI_SwitchDropDown::ZC_GUI_SwitchDropDown(const std::vector<std::wstring>& variants, uint active_variant, float width, float height, const ZC_GUI_ColorsDropDown& colorsDropDownSwitch)
    : ZC_GUI_ButtonBase(ZC_GUI_ObjData(CalculateWidth(variants, width), height, 0, ZC_GUI_IconUV::quad, ZC_GUI_Bindings::bind_tex_Icons), ZC_GUI_BF__None, colorsDropDownSwitch.colorsButton),
    ZC_GUI_ButtonMouseText(this->VGetWidth_Obj(), this->GetHeight(), ZC_GUI_BF__None,
        ZC_GUI_TextForButton(ZC_GUI_TextForButton::Indent(ZC_GUI_DropDownIcon::GetTextIndentX(), ZC_GUI_TextForButton::Indent::Left), variants[active_variant], false,
            this->VGetWidth_Obj() - ZC_GUI_DropDownIcon::GetTextIndentX() - ZC_GUI_DropDownIcon::width, ZC_GUI_TextAlignment::Left, colorsDropDownSwitch.color_text)),
    obj_dd_icon(colorsDropDownSwitch.color_arrow),
    ddVariants(Fill_variants(variants)),
    win_dd(ZC_WOIData(this->VGetWidth_Obj(), this->GetHeight() * (variants.size() - 1), 0.f, 0.f, ZC_WOIF__X_Right_Pixel | ZC_WOIF__Y_Top_Pixel), ZC_GUI_WF__OutAreaClickClose),
    pBMT_active(&(ddVariants[active_variant]))
{}

ZC_GUI_SwitchDropDown::ZC_GUI_SwitchDropDown(ZC_GUI_SwitchDropDown&& dds)
    : ZC_GUI_ButtonBase(dynamic_cast<ZC_GUI_ButtonBase&&>(dds)),
    ZC_GUI_ButtonMouseText(dynamic_cast<ZC_GUI_ButtonMouseText&&>(dds)),
    obj_dd_icon(std::move(dds.obj_dd_icon)),
    ddVariants(std::move(dds.ddVariants)),
    win_dd(ZC_WOIData(this->VGetWidth_Obj(), this->GetHeight() * (ddVariants.size() - 1), 0.f, 0.f, ZC_WOIF__X_Right_Pixel | ZC_WOIF__Y_Top_Pixel), ZC_GUI_WF__OutAreaClickClose),
    pBMT_active(dds.pBMT_active)
{}

void ZC_GUI_SwitchDropDown::MakeVariantActive(uint index)
{
    if (index >= ddVariants.size()) return;    //  out of range
    ZC_GUI_DDVariant<ZC_GUI_SwitchDropDown>* _pBMT_active = &(ddVariants[index]);
    if (_pBMT_active != pBMT_active) VariantChoosed(_pBMT_active);
}

void ZC_GUI_SwitchDropDown::VSet_pBL_Obj(const ZC_Vec2<float>& _bl)
{       //  make it here to avoid that in move ctr
    this->VAddObj_Obj(&obj_dd_icon, nullptr);
    for (ZC_GUI_DDVariant<ZC_GUI_SwitchDropDown>& bmt : ddVariants)
    {
        bmt.pHolder = this;     //  that not pObjHolder!
        win_dd.AddRow(ZC_GUI_ObjBorder::Row({}, { &bmt }));
    }

    *(this->pBL) = _bl;
    this->objs.front()->VSet_pBL_Obj(_bl);  //  set bl for textButton
    obj_dd_icon.VSet_pBL_Obj(ZC_Vec2<float>(_bl[0] + this->VGetWidth_Obj() - obj_dd_icon.VGetWidth_Obj(), _bl[1] + (this->GetHeight() - obj_dd_icon.GetHeight()) / 2.f));
}

void ZC_GUI_SwitchDropDown::VLeftButtonUp_BM(float time)
{
    if (win_dd.VIsDrawing_Obj()) return;

    SetActiveBMTDrawState(false);

    UpdatePos_win_dd();
    win_dd.VSetDrawState_W(true);   //  start drawing window
}

void ZC_GUI_SwitchDropDown::VMoveBL_Obj(float rel_x, float rel_y, int& update_borders)
{
    MoveVec2(rel_x, rel_y, *(this->pBL));
    for (ZC_GUI_Obj* pObj : objs) pObj->VMoveBL_Obj(rel_x, rel_y, update_borders);
    win_dd.VSetDrawState_W(false);
}

float ZC_GUI_SwitchDropDown::CalculateWidth(const std::vector<std::wstring>& variants, float width)
{
    float total_width = 0.f;
    float text_indent_X = ZC_GUI_DropDownIcon::GetTextIndentX();
    for (auto& var : variants)
    {
        float var_width = text_indent_X + ZC_GUI_TextManager::CalculateWstrWidth(var) + text_indent_X + ZC_GUI_DropDownIcon::width;
        if (total_width < var_width) total_width = var_width;
    }
    return width > total_width ? width : total_width;
}

std::vector<ZC_GUI_DDVariant<ZC_GUI_SwitchDropDown>> ZC_GUI_SwitchDropDown::Fill_variants(const std::vector<std::wstring> variants)
{
    std::vector<ZC_GUI_DDVariant<ZC_GUI_SwitchDropDown>> _bmts;
    _bmts.reserve(variants.size());
    for (const std::wstring& var : variants) _bmts.emplace_back(this, this->VGetWidth_Obj(), this->GetHeight(), var);
    return _bmts;
}

void ZC_GUI_SwitchDropDown::SetActiveBMTDrawState(bool needDraw)
{
    if (pBMT_active->VIsDrawing_Obj() == needDraw) return;
    pBMT_active->VSetDrawState_Obj(needDraw, true);
    if (pBMT_active != &(ddVariants.back()))      //  if active object not last in list, need up y in all object under it in win_dd
    {
        ZC_Vec2<float>* pBL_start = pBMT_active->VGet_pBL_end();
        ZC_Vec2<float>* pBL_end = win_dd.rows.back().objs.front()->VGet_pBL_end();
        static float offset_y = win_dd.rows.front().rowParams.height + win_dd.rows.front().rowParams.indent_y;
        for (auto pBL_cur = ++pBL_start; pBL_cur <= pBL_end; ++pBL_cur) (*pBL_cur)[1] += needDraw ? - offset_y : offset_y;
        win_dd.VSubDataBL_Obj(pBL_start, pBL_end);
    }
}

void ZC_GUI_SwitchDropDown::UpdatePos_win_dd()
{
    ZC_Vec2<float> win_pos = this->Get_bl_Obj();
    win_pos[1] -= win_dd.GetHeight();   //  get win position
    win_pos -= win_dd.Get_bl_Obj();     //  get vector on wich to move win from previous position
    if (win_pos != ZC_Vec2<float>()) win_dd.VCursorMove_Obj(win_pos[0], win_pos[1]);     //  if win_pos x or y not null, move window
}

void ZC_GUI_SwitchDropDown::VariantChoosed(ZC_GUI_DDVariant<ZC_GUI_SwitchDropDown>* _pBMT_active)
{
    SetActiveBMTDrawState(true);
    pBMT_active = _pBMT_active;
    this->UpdateText_BMT(pBMT_active->GetWStr_BMT(), false);
    win_dd.VSetDrawState_W(false);
    VVariantChossed(pBMT_active - ddVariants.data());  //  get index in vector and call VVariantChanged_DD()
}