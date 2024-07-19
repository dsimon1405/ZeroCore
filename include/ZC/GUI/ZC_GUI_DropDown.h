#pragma once

#include <ZC/GUI/ZC_GUI_ButtonMouseText.h>
#include <ZC/GUI/ZC_GUI_WinImmutable.h>
#include "ZC_GUI_IconUV.h"

#include <ZC/GUI/ZC_GUI.h>
#include <iostream>
struct ZC_GUI_DropDown : public ZC_GUI_ButtonMouseText
{
    struct BMT : public ZC_GUI_ButtonMouseText
    {
        ZC_GUI_DropDown* pDropDown;

        BMT(ZC_GUI_DropDown* _pDropDown, float width, float height, const std::wstring& _wstr)
            : ZC_GUI_ButtonBase(ZC_GUI_ObjData{ .width = width, .height = height, .uv = ZC_GUI_IconUV::button, .tex_binding = ZC_GUI_Bindings::bind_tex_Icons }),
            ZC_GUI_ButtonMouseText(width, height, ZC_GUI_MB__None, ZC_GUI_Text(_wstr, true, 0)),
            pDropDown(_pDropDown)
        {}

        void VLeftButtonUp_BM(float time) override
        {
            pDropDown->VariantChanged(this);
        }
    };

    ZC_GUI_DropDown(const std::vector<std::wstring>& variants, int active_variant)
        : ZC_GUI_ButtonBase(ZC_GUI_ObjData{ .width = CalculateWidth(variants), .height = float(ZC_GUI_TextManager::GetFontHeight()), .uv = ZC_GUI_IconUV::button, .tex_binding = ZC_GUI_Bindings::bind_tex_Icons }),
        ZC_GUI_ButtonMouseText(this->GetWidth(), this->GetHeight(), ZC_GUI_MB__None, ZC_GUI_Text(variants.front(), false, this->GetWidth() - drop_icon_size)),
        drop_icon(drop_icon_size, drop_icon_size, 0.f, 0, ZC_GUI_IconUV::checkBox, 0, ZC_GUI_Bindings::bind_tex_Icons),
        bmts(Fill_bmts(variants)),
        win_dd(ZC_WOIData(this->GetWidth(), this->GetHeight() * bmts.size() - 1, 0.f, 0.f, ZC_WOIF__X_Left_Pixel | ZC_WOIF__Y_Bottom_Pixel), ZC_GUI_WF__None),
        pBMT_active(&(bmts[active_variant]))
    {
        this->VAddObj_Obj(&drop_icon, nullptr);

        for (BMT& bmt : bmts) win_dd.AddRow(ZC_GUI_ObjBorder::Row({}, { &bmt }));
    }

    virtual void VVariantChanged_DD(int new_variant)
    {
        std::cout<<"variant "<<new_variant<<std::endl;
    }
    //  = 0;

private:
    static inline const float drop_icon_size = 6.f;     //  4x4 pixels
    ZC_GUI_Obj drop_icon;
    std::vector<BMT> bmts;
    ZC_GUI_WinImmutable win_dd;
    BMT* pBMT_active = nullptr;

    float CalculateWidth(const std::vector<std::wstring>& variants)
    {
        float total_width = 0.f;
        for (auto& var : variants)
        {
            float var_width = ZC_GUI_TextManager::CalculateWstrWidth(var) + drop_icon_size;
            if (total_width < var_width) total_width = var_width;
        }
        return total_width;
    }

    void VSet_pBL_Obj(const ZC_Vec2<float>& _bl) override
    {
        this->Conf_Set_bl_BMT(_bl);
        ZC_Vec2<float> bl_icon(_bl[0] + this->GetWidth() - drop_icon.GetWidth(), _bl[1] + (this->GetHeight() - this->GetHeight()) / 2.f);
        drop_icon.VSet_pBL_Obj(bl_icon);
    }

    std::vector<BMT> Fill_bmts(const std::vector<std::wstring> variants)
    {
        std::vector<BMT> _bmts;
        _bmts.reserve(variants.size());
        for (const std::wstring& var : variants) _bmts.emplace_back(this, this->GetWidth(), this->GetHeight(), var);
        return _bmts;
    }

    void VLeftButtonUp_BM(float time) override
    {
        if (win_dd.VIsDrawing_Obj()) return;
        if (pBMT_active->VIsDrawing_Obj()) win_dd.ChangeObjsDrawState(false, pBMT_active, pBMT_active);

        ZC_Vec2<float> win_pos = this->Get_bl_Obj();
        win_pos[1] -= win_dd.GetHeight();
        win_pos -= win_dd.Get_bl_Obj();
        if (win_pos != ZC_Vec2<float>()) win_dd.VCursorMove_Obj(win_pos[0], win_pos[1]);     //  if win_pos x or y not null, move window
        win_dd.VSetDrawState_W(true);   //  start drawing window

        ZC_GUI::pGUI->eventManager.SetMouseButtonDownWatcherObj(this);
    }

    void VMouseButtonLeftOrRightDown_Obj() override
    {
        if (!(win_dd.CheckCursorCollision_Obj())) CollapseDropDown();  //  cursor not in win_dd stop it drowing
    }

    void VariantChanged(BMT* pBMT)
    {
        win_dd.ChangeObjsDrawState(true, pBMT_active, pBMT_active);
        pBMT_active = pBMT;
        this->UpdateText_BMT(pBMT_active->GetText_BMT(), false);
        CollapseDropDown();
        VVariantChanged_DD(pBMT_active - bmts.data());  //  get index in vector and call VVariantChanged_DD()
    }

    void CollapseDropDown()
    {
        win_dd.VSetDrawState_W(false);
        ZC_GUI::pGUI->eventManager.SetMouseButtonDownWatcherObj(nullptr);
        //  add changing uv in drop_icon
    }
};