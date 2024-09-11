#pragma once

#include <ZC/GUI/Backend/ZC_GUI_Obj.h>

template <typename TObj>
struct ZC_GUI__Obj
{
    ZC_GUI__Obj(TObj&& _obj);
    
    virtual ~ZC_GUI__Obj() = default;

    ZC_GUI_Obj* GetObj();
    float GetWidth();
    float GetHeight();

protected:
    TObj obj;
};

template <typename TObj>
ZC_GUI__Obj<TObj>::ZC_GUI__Obj(TObj&& _obj)
    : obj(std::move(_obj))
{}

template <typename TObj>
ZC_GUI_Obj* ZC_GUI__Obj<TObj>::GetObj()
{
    return &obj;
}

template <typename TObj>
float ZC_GUI__Obj<TObj>::GetWidth()
{
    return obj.VGetWidthComposite_Obj();
}

template <typename TObj>
float ZC_GUI__Obj<TObj>::GetHeight()
{
    return obj.VGetHeightComposite_Obj();
}