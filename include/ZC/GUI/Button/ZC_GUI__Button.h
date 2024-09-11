#pragma once

#include <ZC/GUI/ZC_GUI__Obj.h>

template <typename T>
struct ZC_GUI__Button : public ZC_GUI__Obj<T>
{
    ZC_GUI__Button(T&& t);

    virtual void VScroll(float vertical, float time) {}
    virtual void VCursorMove(float rel_x, float rel_y) {}
    virtual void VLeftButtonDown(float time) {}
    virtual void VLeftButtonDoubleClick(float time) {}
    virtual void VLeftButtonPressed(float time) {}
    virtual void VLeftButtonUp(float time) {}
    virtual void VKeyboardButtonDown(float time) {}
    virtual void VKeyboardButtonUp(float time) {}
    virtual void VKeyboardButtonPressed(float time) {}
};


template <typename T>
ZC_GUI__Button<T>::ZC_GUI__Button(T&& t)
    : ZC_GUI__Obj<T>(std::move(t))
{}