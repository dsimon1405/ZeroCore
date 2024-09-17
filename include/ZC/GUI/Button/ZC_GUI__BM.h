#pragma once

#include <ZC/GUI/Backend/Button/ZC_GUI_ButtonBase.h>
#include "ZC_GUI__Button.h"

template <typename TBM>
struct ZC_GUI__BM : public TBM
{
    ZC_GUI__Button<ZC_GUI__BM<TBM>>* pHolder = nullptr;

    ZC_GUI__BM(ZC_GUI__Button<ZC_GUI__BM<TBM>>* _pHolder, TBM&& bm);

    void VScroll_Obj(float vertical, float time) override;
    void VCursorMove_Obj(float rel_x, float rel_y) override;
    void VLeftButtonDown_BM(float time) override;
    void VLeftButtonDoubleClick_BM(float time) override;
    void VLeftButtonPressed_BM(float time) override;
    void VLeftButtonUp_BM(float time) override;
};


template <typename TBM>
ZC_GUI__BM<TBM>::ZC_GUI__BM(ZC_GUI__Button<ZC_GUI__BM<TBM>>* _pHolder, TBM&& bm)
    : ZC_GUI_ButtonBase(static_cast<ZC_GUI_ButtonBase&&>(bm)),
    TBM(std::move(bm)),
    pHolder(_pHolder)
{}

template <typename TBM>
void ZC_GUI__BM<TBM>::VScroll_Obj(float vertical, float time)
{
    pHolder->VScroll(vertical, time);
}

template <typename TBM>
void ZC_GUI__BM<TBM>::VCursorMove_Obj(float rel_x, float rel_y)
{
    pHolder->VCursorMove(rel_x, rel_y);
}

template <typename TBM>
void ZC_GUI__BM<TBM>::VLeftButtonDown_BM(float time)
{
    pHolder->VLeftButtonDown(time);
}

template <typename TBM>
void ZC_GUI__BM<TBM>::VLeftButtonDoubleClick_BM(float time)
{
    pHolder->VLeftButtonDoubleClick(time);
}

template <typename TBM>
void ZC_GUI__BM<TBM>::VLeftButtonPressed_BM(float time)
{
    pHolder->VLeftButtonPressed(time);
}

template <typename TBM>
void ZC_GUI__BM<TBM>::VLeftButtonUp_BM(float time)
{
    pHolder->VLeftButtonUp(time);
}