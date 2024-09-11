#pragma once

#include <ZC/GUI/Backend/Button/ZC_GUI_ButtonBase.h>

    //  struct for classes with (void VVariantChoosed(uint)) virtual method for TVar.
template <typename THolder, typename TVar>
struct ZC_GUI__VariantChoosed : public TVar
{
    THolder* pHolder;

    ZC_GUI__VariantChoosed(THolder* _pHolder, TVar&& var);
    
    void VVariantChoosed(uint variant_index) override;
};

template <typename THolder, typename TVar>
ZC_GUI__VariantChoosed<THolder, TVar>::ZC_GUI__VariantChoosed(THolder* _pHolder, TVar&& var)
    : ZC_GUI_ButtonBase(static_cast<ZC_GUI_ButtonBase&&>(var)),
    TVar(std::move(var)),
    pHolder(_pHolder)
{}

template <typename THolder, typename TVar>
void ZC_GUI__VariantChoosed<THolder, TVar>::VVariantChoosed(unsigned int variant_index)
{
    pHolder->VVariantChoosed(variant_index);
}
