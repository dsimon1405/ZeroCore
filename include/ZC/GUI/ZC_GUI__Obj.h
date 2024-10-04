#pragma once

#include <ZC/GUI/Backend/ZC_GUI_Obj.h>
#include <ZC/Tools/Function/ZC_Function.h>

template <typename TObj>
struct ZC_GUI__Obj
{
    ZC_GUI__Obj(ZC_GUI__Obj&& _obj);
    
    virtual ~ZC_GUI__Obj() = default;

    ZC_GUI_Obj* GetObj();
    float GetWidth();
    float GetHeight();
        //  return objects drawing state.
    bool IsDrawing();
        //  start/stop draw object into the window.
    void SetDrawState(bool needDraw);
        //  return bottom left position of the texture into the window
    ZC_Vec2<float> GetPositionBL();

protected:
    ZC_GUI__Obj(TObj&& _obj);

    TObj obj;
};

template <typename TObj>
struct ZC_GUI__ObjFocuse : public ZC_GUI__Obj<TObj>    //  store focuse callback function
{
    ZC_GUI__ObjFocuse(TObj&& _obj, ZC_Function<void(bool)>&& _callback_focuse);

private:
    ZC_Function<void(bool)> callback_focuse;

    void VFocuseChanged_BM(bool isFocused) override;
};


    //  ZC_GUI__Obj

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
    obj.VSetDrawState_Obj(needDraw, true);
}

template <typename TObj>
ZC_Vec2<float> ZC_GUI__Obj<TObj>::GetPositionBL()
{
    return obj.Get_bl_Obj();
}



    //  ZC_GUI__ObjFocuse

template <typename TObj>
ZC_GUI__ObjFocuse<TObj>::ZC_GUI__ObjFocuse(TObj&& _obj, ZC_Function<void(bool)>&& _callback_focuse)
    : ZC_GUI__Obj<TObj>(std::move(_obj)),
    callback_focuse(std::move(_callback_focuse))
{}

template <typename TObj>
void ZC_GUI__ObjFocuse<TObj>::VFocuseChanged_BM(bool isFocused)
{
    callback_focuse(isFocused);
}