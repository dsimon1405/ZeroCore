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
    ddWindow(ZC_WOIData(this->VGetWidth_Obj(), this->GetHeight() * (variants.size() - 1), 0.f, 0.f, ZC_WOIF__X_Right_Pixel | ZC_WOIF__Y_Top_Pixel), ZC_GUI_WF__OutAreaClickClose),
    pDDVariant_active(&(ddVariants[active_variant]))
{}

ZC_GUI_SwitchDropDown::ZC_GUI_SwitchDropDown(ZC_GUI_SwitchDropDown&& dds)
    : ZC_GUI_ButtonBase(static_cast<ZC_GUI_ButtonBase&&>(dds)),
    ZC_GUI_ButtonMouseText(static_cast<ZC_GUI_ButtonMouseText&&>(dds)),
    obj_dd_icon(std::move(dds.obj_dd_icon)),
    ddVariants(std::move(dds.ddVariants)),
    ddWindow(ZC_WOIData(this->VGetWidth_Obj(), this->GetHeight() * (ddVariants.size() - 1), 0.f, 0.f, ZC_WOIF__X_Right_Pixel | ZC_WOIF__Y_Top_Pixel), ZC_GUI_WF__OutAreaClickClose),
    pDDVariant_active(dds.pDDVariant_active)
{}

void ZC_GUI_SwitchDropDown::MakeVariantActive(uint index)
{
    if (index >= ddVariants.size()) return;    //  out of range
    ZC_GUI_DDVariant<ZC_GUI_SwitchDropDown>* pDDVariant_choosed = &(ddVariants[index]);
    if (pDDVariant_choosed != pDDVariant_active) VariantChoosed(pDDVariant_choosed);
}

void ZC_GUI_SwitchDropDown::VSet_pBL_Obj(const ZC_Vec2<float>& _bl)
{       //  make it here to avoid that in move ctr
    this->VAddObj_Obj(&obj_dd_icon, nullptr);
    for (ZC_GUI_DDVariant<ZC_GUI_SwitchDropDown>& bmt : ddVariants)
    {
        bmt.pHolder = this;     //  that not pObjHolder!
        ddWindow.AddRow(ZC_GUI_ObjBorder::Row({}, { &bmt }));
    }

    *(this->pBL) = _bl;
    this->objs.front()->VSet_pBL_Obj(_bl);  //  set bl for textButton
    obj_dd_icon.VSet_pBL_Obj(ZC_Vec2<float>(_bl[0] + this->VGetWidth_Obj() - obj_dd_icon.VGetWidth_Obj(), _bl[1] + (this->GetHeight() - obj_dd_icon.GetHeight()) / 2.f));
}

void ZC_GUI_SwitchDropDown::VLeftButtonUp_BM(float time)
{
    if (ddWindow.VIsDrawing_Obj()) return;

    SetActiveBMTDrawState(false);

    UpdatePos_ddWindow();
    ddWindow.VSetDrawState_W(true);   //  start drawing window
}

void ZC_GUI_SwitchDropDown::VMoveBL_Obj(float rel_x, float rel_y, int& update_borders)
{
    MoveVec2(rel_x, rel_y, *(this->pBL));
    for (ZC_GUI_Obj* pObj : objs) pObj->VMoveBL_Obj(rel_x, rel_y, update_borders);
    ddWindow.VSetDrawState_W(false);
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
    if (pDDVariant_active->VIsDrawing_Obj() == needDraw) return;
    pDDVariant_active->VSetDrawState_Obj(needDraw, true);
    if (pDDVariant_active != &(ddVariants.back()))      //  if active object not last in list, need up y in all object under it in ddWindow
    {
        ZC_Vec2<float>* pBL_start = pDDVariant_active->VGet_pBL_end();
        ZC_Vec2<float>* pBL_end = ddWindow.rows.back().objs.front()->VGet_pBL_end();
        static float offset_y = ddWindow.rows.front().rowParams.height + ddWindow.rows.front().rowParams.indent_y;
        for (auto pBL_cur = ++pBL_start; pBL_cur <= pBL_end; ++pBL_cur) (*pBL_cur)[1] += needDraw ? - offset_y : offset_y;
        ddWindow.VSubDataBL_Obj(pBL_start, pBL_end);
    }
}

void ZC_GUI_SwitchDropDown::UpdatePos_ddWindow()
{
    ZC_Vec2<float> bl = this->Get_bl_Obj();     //  button's bl
    bl[1] -= ddWindow.GetHeight();   //  ddWindow new bl
    bl -= ddWindow.Get_bl_Obj();     //  vector to move from ddWindow current bl to new bl
    if (bl != ZC_Vec2<float>()) ddWindow.VCursorMove_Obj(bl[0], bl[1]);     //  if bl.x or bl.y not 0 need to move ddWindow
}

void ZC_GUI_SwitchDropDown::VariantChoosed(ZC_GUI_DDVariant<ZC_GUI_SwitchDropDown>* pDDVariant_choosed)
{
    SetActiveBMTDrawState(true);
    pDDVariant_active = pDDVariant_choosed;
    this->UpdateText_BMT(pDDVariant_active->GetWStr_BMT(), false);
    ddWindow.VSetDrawState_W(false);
    VVariantChoosed(pDDVariant_active - ddVariants.data());  //  get index in vector and call VVariantChanged_DD()
}