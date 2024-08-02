#pragma once

#include <ZC/GUI/ZC_GUI_ButtonMouseText.h>
#include <ZC/GUI/ZC_GUI_WinImmutable.h>
#include "ZC_GUI_IconUV.h"
#include <ZC/Events/ZC_Events.h>

#include <ZC/GUI/ZC_GUI.h>
#include <iostream>
struct ZC_GUI_DropDown : public ZC_GUI_ButtonMouseText
{
    struct BMT : public ZC_GUI_ButtonMouseText
    {
        ZC_GUI_DropDown* pDropDown;

        BMT(ZC_GUI_DropDown* _pDropDown, float width, float height, const std::wstring& _wstr)
            : ZC_GUI_ButtonBase(ZC_GUI_ObjData(width, height, 0, ZC_GUI_IconUV::button, ZC_GUI_Bindings::bind_tex_Icons), ZC_GUI_BF__None),
            ZC_GUI_ButtonMouseText(width, height, ZC_GUI_BF__None, ZC_GUI_TextForButton(ZC_GUI_TextForButton::Indent(textIndent_x, ZC_GUI_TextForButton::Indent::Left),_wstr, true, 0)),
            pDropDown(_pDropDown)
        {}

        void VLeftButtonUp_BM(float time) override
        {
            pDropDown->VariantChanged(this);
        }
    };

    ZC_GUI_DropDown(const std::vector<std::wstring>& variants, int active_variant)
        : ZC_GUI_ButtonBase(ZC_GUI_ObjData(CalculateWidth(variants), float(ZC_GUI_TextManager::GetFontHeight()), 0, ZC_GUI_IconUV::button, ZC_GUI_Bindings::bind_tex_Icons), ZC_GUI_BF__None),
        ZC_GUI_ButtonMouseText(this->VGetWidth_Obj(), this->GetHeight(), ZC_GUI_BF__None,
            ZC_GUI_TextForButton(ZC_GUI_TextForButton::Indent(textIndent_x, ZC_GUI_TextForButton::Indent::Left), variants.front(), false, this->VGetWidth_Obj() - textIndent_x - drop_icon_size)),
        obj_dd_icon(drop_icon_size, drop_icon_size, 0.f, 0, ZC_GUI_IconUV::checkBox, 0, ZC_GUI_Bindings::bind_tex_Icons),
        bmts(Fill_bmts(variants)),
        win_dd(ZC_WOIData(this->VGetWidth_Obj(), this->GetHeight() * (bmts.size() - 1), 0.f, 0.f, ZC_WOIF__X_Left_Pixel | ZC_WOIF__Y_Bottom_Pixel), ZC_GUI_WF__None),
        pBMT_active(&(bmts[active_variant]))
    {
        this->VAddObj_Obj(&obj_dd_icon, nullptr);

        for (BMT& bmt : bmts) win_dd.AddRow(ZC_GUI_ObjBorder::Row({}, { &bmt }));
    }
    
    virtual void VVariantChanged_DD(int new_variant)
    {
        std::cout<<"variant "<<new_variant<<std::endl;
    }
    //  = 0;

private:
    static inline const float drop_icon_size = 8.f;     //  4x4 pixels
    static inline const float textIndent_x = 4.f;       //  indent from button's left border to text
    ZC_GUI_Obj obj_dd_icon;
    std::vector<BMT> bmts;
    ZC_GUI_WinImmutable win_dd;
    BMT* pBMT_active = nullptr;
    ZC_EC ecWinResize;

    void VSet_pBL_Obj(const ZC_Vec2<float>& _bl) override
    {
        *(this->pBL) = _bl;
        this->objs.front()->VSet_pBL_Obj(_bl);  //  set bl for textButotn
        obj_dd_icon.VSet_pBL_Obj(ZC_Vec2<float>(_bl[0] + this->VGetWidth_Obj() - obj_dd_icon.VGetWidth_Obj(), _bl[1] + (this->GetHeight() - obj_dd_icon.GetHeight()) / 2.f));
    }

    void VLeftButtonUp_BM(float time) override
    {
        if (win_dd.VIsDrawing_Obj()) return;

        SetActiveBMTDrawState(false);

        UpdatePos_win_dd();
        win_dd.VSetDrawState_W(true);   //  start drawing window

        ZC_GUI::pGUI->eventManager.SetMouseButtonDownWatcherObj(this);
        ecWinResize.NewConnection(ZC_Events::ConnectWindowResize({ &ZC_GUI_DropDown::SWindowResized, this }));

        obj_dd_icon.pObjData->uv = ZC_GUI_UV{ .bl{ ZC_GUI_IconUV::checkBox.tr[0], ZC_GUI_IconUV::checkBox.bl[1] }, .tr{ ZC_GUI_IconUV::checkBox.bl[0], ZC_GUI_IconUV::checkBox.tr[1] } };
        VMapObjData_Obj(obj_dd_icon.pObjData, offsetof(ZC_GUI_ObjData, uv), sizeof(ZC_GUI_ObjData::uv), &(obj_dd_icon.pObjData->uv));
    }

    void VMouseButtonLeftOrRightDown_Obj() override
    {
        if (!(win_dd.CheckCursorCollision_Obj())) CollapseDropDown();  //  cursor not in win_dd stop it drowing
    }

    void VMoveBL_Obj(float rel_x, float rel_y, int& update_borders) override
    {
        MoveVec2(rel_x, rel_y, *(this->pBL));
        for (ZC_GUI_Obj* pObj : objs) pObj->VMoveBL_Obj(rel_x, rel_y, update_borders);
        if (win_dd.VIsDrawing_Obj()) CollapseDropDown();
    }

    float CalculateWidth(const std::vector<std::wstring>& variants)
    {
        float total_width = 0.f;
        for (auto& var : variants)
        {
            float var_width = textIndent_x + ZC_GUI_TextManager::CalculateWstrWidth(var) + drop_icon_size;
            if (total_width < var_width) total_width = var_width;
        }
        return total_width;
    }

    std::vector<BMT> Fill_bmts(const std::vector<std::wstring> variants)
    {
        std::vector<BMT> _bmts;
        _bmts.reserve(variants.size());
        for (const std::wstring& var : variants) _bmts.emplace_back(this, this->VGetWidth_Obj(), this->GetHeight(), var);
        return _bmts;
    }

    void SetActiveBMTDrawState(bool needDraw)
    {
        if (pBMT_active->VIsDrawing_Obj() == needDraw) return;
        pBMT_active->VSetDrawState_Obj(needDraw, true);
        if (pBMT_active != &(bmts.back()))      //  if active object not last in list, need up y in all object under it in win_dd
        {
            ZC_Vec2<float>* pBL_start = pBMT_active->VGet_pBL_end();
            ZC_Vec2<float>* pBL_end = win_dd.rows.back().objs.front()->VGet_pBL_end();
            static float offset_y = win_dd.rows.front().rowParams.height + win_dd.rows.front().rowParams.indent_y;
            for (auto pBL_cur = ++pBL_start; pBL_cur <= pBL_end; ++pBL_cur) (*pBL_cur)[1] += needDraw ? - offset_y : offset_y;
            win_dd.VSubDataBL_Obj(pBL_start, pBL_end);
        }
    }

    void UpdatePos_win_dd()
    {
        ZC_Vec2<float> win_pos = this->Get_bl_Obj();
        win_pos[1] -= win_dd.GetHeight();   //  get win position
        win_pos -= win_dd.Get_bl_Obj();     //  get vector on wich to move win from previous position
        if (win_pos != ZC_Vec2<float>()) win_dd.VCursorMove_Obj(win_pos[0], win_pos[1]);     //  if win_pos x or y not null, move window
    }

    void VariantChanged(BMT* pBMT)
    {
        SetActiveBMTDrawState(true);
        pBMT_active = pBMT;
        this->UpdateText_BMT(pBMT_active->GetWStr_BMT(), false);
        CollapseDropDown();
        VVariantChanged_DD(pBMT_active - bmts.data());  //  get index in vector and call VVariantChanged_DD()
    }

    void CollapseDropDown()
    {
        win_dd.VSetDrawState_W(false);
        ZC_GUI::pGUI->eventManager.SetMouseButtonDownWatcherObj(nullptr);
        ecWinResize.Disconnect();
        
        obj_dd_icon.pObjData->uv = ZC_GUI_IconUV::checkBox;
        VMapObjData_Obj(obj_dd_icon.pObjData, offsetof(ZC_GUI_ObjData, uv), sizeof(ZC_GUI_ObjData::uv), &(obj_dd_icon.pObjData->uv));
    }

    void SWindowResized(float, float)
    {
        if (win_dd.VIsDrawing_Obj()) UpdatePos_win_dd();
    }
};