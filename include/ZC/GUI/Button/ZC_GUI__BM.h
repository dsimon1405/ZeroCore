#pragma once

#include <ZC/GUI/Backend/Button/ZC_GUI_ButtonBase.h>

template <typename TBM, typename THolder>
struct ZC_GUI__BM : public TBM
{
    THolder* pHolder;

    ZC_GUI__BM(TBM&& bm, THolder* _pHolder);

    void VScroll_Obj(float vertical, float time) override;
    void VCursorMove_Obj(float rel_x, float rel_y) override;
    void VLeftButtonDown_BM(float time) override;
    void VLeftButtonDoubleClick_BM(float time) override;
    void VLeftButtonPressed_BM(float time) override;
    void VLeftButtonUp_BM(float time) override;
};


template <typename TBM, typename THolder>
ZC_GUI__BM<TBM, THolder>::ZC_GUI__BM(TBM&& bm, THolder* _pHolder)
    : ZC_GUI_ButtonBase(dynamic_cast<ZC_GUI_ButtonBase&&>(bm)),
    TBM(std::move(bm)),
    pHolder(_pHolder)
{}

template <typename TBM, typename THolder>
void ZC_GUI__BM<TBM, THolder>::VScroll_Obj(float vertical, float time)
{
    pHolder->VScroll(vertical, time);
}

template <typename TBM, typename THolder>
void ZC_GUI__BM<TBM, THolder>::VCursorMove_Obj(float rel_x, float rel_y)
{
    pHolder->VCursorMove(rel_x, rel_y);
}

template <typename TBM, typename THolder>
void ZC_GUI__BM<TBM, THolder>::VLeftButtonDown_BM(float time)
{
    pHolder->VLeftButtonDown(time);
}

template <typename TBM, typename THolder>
void ZC_GUI__BM<TBM, THolder>::VLeftButtonDoubleClick_BM(float time)
{
    pHolder->VLeftButtonDoubleClick(time);
}

template <typename TBM, typename THolder>
void ZC_GUI__BM<TBM, THolder>::VLeftButtonPressed_BM(float time)
{
    pHolder->VLeftButtonPressed(time);
}

template <typename TBM, typename THolder>
void ZC_GUI__BM<TBM, THolder>::VLeftButtonUp_BM(float time)
{
    pHolder->VLeftButtonUp(time);
}