#include <ZC/Tools/ZC_WindowOrthoIndent.h>

#include <ZC/Events/ZC_Events.h>
#include <ZC/Video/ZC_Window.h>

#include <cassert>

ZC_WindowOrthoIndent::~ZC_WindowOrthoIndent()
{
    sconZC_WindowResized.Disconnect();
}

ZC_WindowOrthoIndent::ZC_WindowOrthoIndent(bool _is_Y_ZeroPointOnTop, float _width, float _height, float _indentX, float _indentY, ZC_WindowOrthoIndentFlags _indentFlags)
    : is_Y_ZeroPointOnTop(_is_Y_ZeroPointOnTop),
    width(_width),
    height(_height),
    sconZC_WindowResized(ZC_Events::ConnectWindowResize({ &ZC_WindowOrthoIndent::ZC_WindowResized, this }))
{
    CheckAndSetIndentData(_indentX, _indentY, _indentFlags);
}

ZC_WindowOrthoIndent::ZC_WindowOrthoIndent(const ZC_WindowOrthoIndent& woi)
    : currentIndents{ woi.currentIndents[0], woi.currentIndents[1] },
    is_Y_ZeroPointOnTop(woi.is_Y_ZeroPointOnTop),
    width(woi.width),
    height(woi.height),
    indentX(woi.indentX),
    indentY(woi.indentY),
    indentFlags(woi.indentFlags),
    sconZC_WindowResized(ZC_Events::ConnectWindowResize({ &ZC_WindowOrthoIndent::ZC_WindowResized, this }))
{}

void ZC_WindowOrthoIndent::CalculateCurrentIndents()
{
    int windowWidth, windowHeight;
    ZC_Window::GetSize(windowWidth, windowHeight);
    CalculateIndents(static_cast<float>(windowWidth), static_cast<float>(windowHeight));
}

void ZC_WindowOrthoIndent::SetNewSize(float _width, float _height)
{
    if (width == _width && height == _height) return;
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
    CheckAndSetIndentData(_indentX, _indentY, _indentFlags);
    CalculateCurrentIndents();
}

void ZC_WindowOrthoIndent::GetIndentParams(float& _indentX, float& _indentY, ZC_WindowOrthoIndentFlags& _indentFlags)
{
    _indentX = indentX;
    _indentY = indentY;
    _indentFlags = _indentFlags;
}

void ZC_WindowOrthoIndent::CheckAndSetIndentData(float _indentX, float _indentY, ZC_WindowOrthoIndentFlags _indentFlags)
{
    int _indentFlagX = 0;   //  find flag for X
    if (_indentFlags & ZC_WOIF__X_Left_Pixel) _indentFlagX = ZC_WOIF__X_Left_Pixel;
    else if (_indentFlags & ZC_WOIF__X_Left_Percent) _indentFlagX = _indentX >= 0.f && _indentX <= 1.f ? ZC_WOIF__X_Left_Percent : 0; //  indent for percent must be in diaposon [0,1]
    else if (_indentFlags & ZC_WOIF__X_Right_Pixel) _indentFlagX = ZC_WOIF__X_Right_Pixel;
    else if (_indentFlags & ZC_WOIF__X_Right_Percent) _indentFlagX = _indentX >= 0.f && _indentX <= 1.f ? ZC_WOIF__X_Right_Percent : 0; //  indent for percent must be in diaposon [0,1]
    else if (_indentFlags & ZC_WOIF__X_Center) _indentFlagX = ZC_WOIF__X_Center;
    assert(_indentFlagX != 0);  //  no valid data

    int _indentFlagY = 0;
    if (_indentFlags & ZC_WOIF__Y_Top_Pixel) _indentFlagY = ZC_WOIF__Y_Top_Pixel;
    else if (_indentFlags & ZC_WOIF__Y_Top_Percent) _indentFlagY = _indentY >= 0.f && _indentY <= 1.f ? ZC_WOIF__Y_Top_Percent : 0;
    else if (_indentFlags & ZC_WOIF__Y_Bottom_Pixel) _indentFlagY = ZC_WOIF__Y_Bottom_Pixel;
    else if (_indentFlags & ZC_WOIF__Y_Bottom_Percent) _indentFlagY = _indentY >= 0.f && _indentY <= 1.f ? ZC_WOIF__Y_Bottom_Percent : 0;
    else if (_indentFlags & ZC_WOIF__Y_Center) _indentFlagY = ZC_WOIF__Y_Center;
    assert(_indentFlagY != 0);

    indentFlags = static_cast<ZC_WindowOrthoIndentFlags>(_indentFlagX | _indentFlagY);

    assert((_indentFlags - indentFlags) == 0);    //  flags have more than one X or more than one Y
    
    indentX = _indentX;
    indentY = _indentY;
}

void ZC_WindowOrthoIndent::ZC_WindowResized(float windowWidth, float windowHeight)
{
    CalculateIndents(windowWidth, windowHeight);
    CallAfterZC_WindowResized();
}

void ZC_WindowOrthoIndent::CalculateIndents(float windowWidth, float windowHeight)
{
    switch (indentFlags & 0x1F) //  remove Y flag (0x1F -> ...00011111)
    {   //  x indent
    case ZC_WOIF__X_Left_Pixel: currentIndents[0] = indentX; break;
    case ZC_WOIF__X_Left_Percent: currentIndents[0] = windowWidth * indentX; break;
    case ZC_WOIF__X_Right_Pixel: currentIndents[0] = windowWidth - indentX - width; break;
    case ZC_WOIF__X_Right_Percent: currentIndents[0] = windowWidth - (windowWidth * indentX) - width; break;
    case ZC_WOIF__X_Center: currentIndents[0] = (windowWidth / 2.f) - (width / 2.f); break;
    default: break;
    }
    switch (indentFlags >> 5 << 5)   //  remove X flag
    {   //  y indent
    case ZC_WOIF__Y_Top_Pixel: currentIndents[1] = is_Y_ZeroPointOnTop ? indentY : windowHeight - indentY - height; break;
    case ZC_WOIF__Y_Top_Percent: currentIndents[1] = is_Y_ZeroPointOnTop ? windowHeight * indentY : windowHeight - (windowHeight * indentY) - height; break;
    case ZC_WOIF__Y_Bottom_Pixel: currentIndents[1] = is_Y_ZeroPointOnTop ? windowHeight - indentY - height : indentY; break;
    case ZC_WOIF__Y_Bottom_Percent: currentIndents[1] = is_Y_ZeroPointOnTop ? windowHeight - (windowHeight * indentY) - height : windowHeight * indentY; break;
    case ZC_WOIF__Y_Center: currentIndents[1] = (windowHeight / 2.f) - (height / 2.f); break;
    default: break;
    }
}