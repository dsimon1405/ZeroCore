#pragma once

#include <ZC/GUI/Backend/Button/ZC_GUI_ButtonBase.h>

    //  struct for classes with (void VVariantChossed(uint)) virtual method for TVar.
template <typename THolder, typename TVar>
struct ZC_GUI__VariantChanged : public TVar
{
    THolder* pHolder;

    ZC_GUI__VariantChanged(THolder* _pHolder, TVar&& var);
    
    void VVariantChossed(uint variant_index) override;
};

template <typename THolder, typename TVar>
ZC_GUI__VariantChanged<THolder, TVar>::ZC_GUI__VariantChanged(THolder* _pHolder, TVar&& var)
    : ZC_GUI_ButtonBase(dynamic_cast<ZC_GUI_ButtonBase&&>(var)),
    TVar(std::move(var)),
    pHolder(_pHolder)
{}

template <typename THolder, typename TVar>
void ZC_GUI__VariantChanged<THolder, TVar>::VVariantChossed(unsigned int variant_index)
{
    pHolder->VVariantChossed(variant_index);
}
