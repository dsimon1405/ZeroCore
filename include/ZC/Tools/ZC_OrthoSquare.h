#pragma once

#include "ZC_WindowOrthoIndent.h"
#include <ZC/Video/OpenGL/Renderer/ZC_DrawerSet.h>

#include <concepts>

template<typename T>
concept ZC_cTexture = std::same_as<T, ZC_Texture> || std::same_as<T, ZC_Texture*> || std::same_as<T, const ZC_Texture*>;

class ZC_OrthoSquare : public ZC_WindowOrthoIndent
{
public:
    /*
    Params:
    - _texture - texture or pointer on texture.
    - _width - objects width in pixels.
    - _height - objects height in pixels.
    - _indentX - value of horizontal indent from border of global window. If used IndentFlag:
        ZC_WOIF__X_Left_Pixel, ZC_WOIF__X_Right_Pixel -> value must be not negative, otherwise sets 0.f;
        ZC_WOIF__X_Left_Percent, ZC_WOIF__X_Right_Percent -> value must be 0.0f - 1.f (where 1.f is 100%);
        ZC_WOIF__X_Center -> value no metter.
    - _indentY - value of vertival indent from border of global window. If used IndentFlag: 
        ZC_WOIF__Y_Top_Pixel, ZC_WOIF__Y_Bottom_Pixel -> value must be not negative, otherwise sets 0.f
        ZC_WOIF__Y_Top_Percent, ZC_WOIF__Y_Bottom_Percent -> value must be 0.0f - 1.f (where 1.f is 100%);
        ZC_WOIF__Y_Center -> value no metter.
    - _indentFlags - flags of indent horizontal(X) and vertical(Y) from border of global window to IGWindow. Must be set one flag for X and one flag for Y. Example: X_Left_Pixel | Y_Top_Pixel.
    - renderLevel - which framebuffer to add to.
    - needDraw - if true, adds to renderer after creatioin.
    - funcIndentChange - function for call after changing indents into window after window resize, if don't need nullptr.
    */
    template<ZC_cTexture TTexture>
    ZC_OrthoSquare(TTexture _texture, float _width, float _height, float _indentX, float _indentY,
        ZC_WindowOrthoIndentFlags _indentFlags, ZC_RenderLevel _frameBuffer, bool needDraw, ZC_Function<void(float,float)>&& _funcIndentChange);

    void SetSize(float _width, float _height);
    void SetAlpha(float alpha);
    void NeedDraw(bool needDraw);
    bool IsDrawing();

private:
    ZC_RenderLevel renderLevel;
    ZC_DrawerSet drawerSet;
    ZC_DSController dsController;
    ZC_Texture texture;
    ZC_Function<void(float,float)> funcIndentChange;

    ZC_DrawerSet CreateDrawerSet(float _width, float _height);
    void VCallAfterZC_WindowResizedWOI() override;
};


template<ZC_cTexture TTexture>
ZC_OrthoSquare::ZC_OrthoSquare(TTexture _texture, float _width, float _height, float _indentX, float _indentY,
        ZC_WindowOrthoIndentFlags _indentFlags, ZC_RenderLevel _frameBuffer, bool needDraw, ZC_Function<void(float,float)>&& _funcIndentChange)
    : ZC_WindowOrthoIndent(false, _width, _height, _indentX, _indentY, _indentFlags),
    renderLevel(_frameBuffer),
    drawerSet(CreateDrawerSet(_width, _height)),
    dsController(drawerSet.MakeZC_DSController()),
    funcIndentChange(std::move(_funcIndentChange))
{
    if constexpr (std::is_pointer<TTexture>()) dsController.SetTexturesHolder(ZC_TexturesHolder{ _texture, 1 });
    else
    {
        texture = std::move(_texture);
        dsController.SetTexturesHolder(ZC_TexturesHolder{ &texture, 1 });
    }

    dsController.SetUniformsData(ZC_UN_unPositionWindow, &this->currentIndents);
    SetAlpha(1.f);
    if (needDraw) NeedDraw(true);
}