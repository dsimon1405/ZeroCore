#pragma once

#include <ZC/GUI/Backend/ZC_GUI_Obj.h>

template <typename TObj>
struct ZC_GUI__Obj
{
    ZC_GUI__Obj(TObj&& _obj);

    ZC_GUI__Obj(ZC_GUI__Obj&& _obj);
    
    virtual ~ZC_GUI__Obj() = default;

    ZC_GUI_Obj* GetObj();
    float GetWidth();
    float GetHeight();
        //  return objects drawing state.
    bool IsDrawing();
        //  start/stop draw object into the window.
    void SetDrawState(bool needDraw);

protected:
    TObj obj;
};

template <typename TObj>
ZC_GUI__Obj<TObj>::ZC_GUI__Obj(TObj&& _obj)
    : obj(std::move(_obj))
{}

template <typename TObj>
ZC_GUI__Obj<TObj>::ZC_GUI__Obj(ZC_GUI__Obj&& obj)
    : obj(std::move(obj.obj))
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

template <typename TObj>
bool ZC_GUI__Obj<TObj>::IsDrawing()
{
    return obj.VIsDrawing_Obj();
}

template <typename TObj>
void ZC_GUI__Obj<TObj>::SetDrawState(bool needDraw)
{
    obj.VSetDrawState_Obj(needDraw);
}