#include "ZC_GUI__ColorManipulator.h"

ZC_GUI__ColorManipulator::ZC_GUI__ColorManipulator(const ZC_GUI_Font* pFont, ZC_Function<void(float,float,float,float)>&& _callback, bool _range_255)
    : ZC_GUI__Obj<ZC_GUI_ColorManipulator>(ZC_GUI_ColorManipulator(pFont, std::move(_callback), _range_255))
{}

void ZC_GUI__ColorManipulator::SetAlpha(float alpha, bool use_callback)
{
    this->obj.SetAlpha(alpha, use_callback);
}