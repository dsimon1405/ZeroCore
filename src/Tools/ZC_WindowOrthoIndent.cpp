#include <ZC/Tools/ZC_WindowOrthoIndent.h>

#include <ZC/Events/ZC_Events.h>
#include <ZC/Video/ZC_Window.h>

#include <cassert>

ZC_WindowOrthoIndent::~ZC_WindowOrthoIndent()
{
    sconZC_WindowResized.Disconnect();
}

ZC_WindowOrthoIndent::ZC_WindowOrthoIndent(bool _is_Y_ZeroPointOnTop, float _width, float _height, float _indentX, float _indentY,
        ZC_WindowOrthoIndentFlags _indentFlags, ZC_Function<void()>&& pFuncWindowResized)
    : is_Y_ZeroPointOnTop(_is_Y_ZeroPointOnTop),
    width(_width),
    height(_height),
    indentX(_indentX),
    indentY(_indentY),
    sconZC_WindowResized(SetIndentData(_indentX, _indentY, _indentFlags)),
    funcWindowResized(std::move(pFuncWindowResized))
{}

void ZC_WindowOrthoIndent::CalculateCurrentIndents()
{
    int windowWidth, windowHeight;
    ZC_Window::GetSize(windowWidth, windowHeight);
    CalculateIndents(windowWidth, windowHeight);
}

void ZC_WindowOrthoIndent::SetNewSize(float _width, float _height)
{
    width = _width;
    height = _height;
    CalculateCurrentIndents();
}

void ZC_WindowOrthoIndent::GetSize(float& _width, float& _height)
{
    _width = width;
    _height = height;
}

void ZC_WindowOrthoIndent::SetNewIndentParams(float _indentX, float _indentY, ZC_WindowOrthoIndentFlags _indentFlags)
{
    SetIndentData(_indentX, _indentY, _indentFlags);
    CalculateCurrentIndents();
}

void ZC_WindowOrthoIndent::GetIndentParams(float& _indentX, float& _indentY, ZC_WindowOrthoIndentFlags& _indentFlags)
{
    _indentX = indentX;
    _indentY = indentY;
    _indentFlags = _indentFlags;
}

ZC_SConnection ZC_WindowOrthoIndent::SetIndentData(float _indentX, float _indentY, ZC_WindowOrthoIndentFlags _indentFlags)
{
    int _indentFlagX = 0;   //  find flag for X
    if (_indentFlags & X_Left_Pixel) _indentFlagX = X_Left_Pixel;
    else if (_indentFlags & X_Left_Percent) _indentFlagX = _indentX >= 0.f && _indentX <= 1.f ? X_Left_Percent : 0; //  indent for percent must be in diaposon [0,1]
    else if (_indentFlags & X_Right_Pixel) _indentFlagX = X_Right_Pixel;
    else if (_indentFlags & X_Right_Percent) _indentFlagX = _indentX >= 0.f && _indentX <= 1.f ? X_Right_Percent : 0; //  indent for percent must be in diaposon [0,1]
    else if (_indentFlags & X_Center) _indentFlagX = X_Center;
    assert(_indentFlagX != 0);  //  no valid data

    int _indentFlagY = 0;
    if (_indentFlags & Y_Top_Pixel) _indentFlagY = Y_Top_Pixel;
    else if (_indentFlags & Y_Top_Percent) _indentFlagY = _indentY >= 0.f && _indentY <= 1.f ? Y_Top_Percent : 0;
    else if (_indentFlags & Y_Bottom_Pixel) _indentFlagY = Y_Bottom_Pixel;
    else if (_indentFlags & Y_Bottom_Percent) _indentFlagY = _indentY >= 0.f && _indentY <= 1.f ? Y_Bottom_Percent : 0;
    else if (_indentFlags & Y_Center) _indentFlagY = Y_Center;
    assert(_indentFlagY != 0);

    indentFlags = static_cast<ZC_WindowOrthoIndentFlags>(_indentFlagX | _indentFlagY);

    assert((_indentFlags - indentFlags) == 0);    //  flags have more than one X or more than one Y

    return ZC_Events::ConnectWindowResize({ &ZC_WindowOrthoIndent::ZC_WindowResized, this });
}

void ZC_WindowOrthoIndent::ZC_WindowResized(float windowWidth, float windowHeight)
{
    CalculateIndents(windowWidth, windowHeight);
    if (funcWindowResized) funcWindowResized();
}

void ZC_WindowOrthoIndent::CalculateIndents(float windowWidth, float windowHeight)
{
    switch (indentFlags & 0x1F) //  remove Y flag (0x1F -> ...00011111)
    {
    case X_Left_Pixel: currentIndentX = indentX; break;
    case X_Left_Percent: currentIndentX = windowWidth * indentX; break;
    case X_Right_Pixel: currentIndentX = windowWidth - indentX - width; break;
    case X_Right_Percent: currentIndentX = windowWidth - (windowWidth * indentX) - width; break;
    case X_Center: currentIndentX = (windowWidth / 2.f) - (width / 2.f); break;
    default: break;
    }
    switch (indentFlags >> 5 << 5)   //  remove X flag
    {
    case Y_Top_Pixel: currentIndentY = is_Y_ZeroPointOnTop ? indentY : windowHeight - indentY - height; break;
    case Y_Top_Percent: currentIndentY = is_Y_ZeroPointOnTop ? windowHeight * indentY : windowHeight - (windowHeight * indentY) - height; break;
    case Y_Bottom_Pixel: currentIndentY = is_Y_ZeroPointOnTop ? windowHeight - indentY - height : indentY; break;
    case Y_Bottom_Percent: currentIndentY = is_Y_ZeroPointOnTop ? windowHeight - (windowHeight * indentY) - height : windowHeight * indentY; break;
    case Y_Center: currentIndentY = (windowHeight / 2.f) - (height / 2.f); break;
    default: break;
    }
}