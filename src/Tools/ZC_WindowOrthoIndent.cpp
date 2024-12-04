#include <ZC/Tools/ZC_WindowOrthoIndent.h>

#include <ZC/Events/ZC_Events.h>
#include <ZC/ZC__System.h>

#include <cassert>

ZC_WOIData::ZC_WOIData(float _width, float _height, float _indentX, float _indentY, ZC_WindowOrthoIndentFlags _indentFlags)
    : width(_width),
    height(_height),
    indentX(_indentX),
    indentY(_indentY),
    indentFlags(_indentFlags)
{}

ZC_WindowOrthoIndent1::~ZC_WindowOrthoIndent1()
{
    ecZC_WindowResized.Disconnect();
}

void ZC_WindowOrthoIndent1::SetNewSize_WOI(float _width, float _height)
{
    if (woiData.width == _width && woiData.height == _height) return;   //  same data

    woiData.width = _width;
    woiData.height = _height;
    ZC_Vec2<float> prevBL = bl_WOI;
    CalculateCurrentIndents_WOI();
    if (prevBL != bl_WOI) VChanged_bl_WOI();
}

void ZC_WindowOrthoIndent1::GetSize(float& _width, float& _height)
{
    _width = woiData.width;
    _height = woiData.height;
}

void ZC_WindowOrthoIndent1::SetNewIndentParams_WOI(float _indentX, float _indentY, ZC_WindowOrthoIndentFlags _indentFlags)
{
    if (_indentX == woiData.indentX && _indentY == woiData.indentY && _indentFlags == woiData.indentFlags) return;      //  same data

    CheckAndSetIndentData(_indentX, _indentY, _indentFlags);
    if (woiData.indentFlags & ZC_WOIF__X_Left_Pixel     //  no need for ZC_SWindow resize callback for objects with same indent and draw angle. Example: object is drawn from corner bl and has indent bl, when window resizing the object's position will not change
        && (is_Y_ZeroPointOnTop ? woiData.indentFlags & ZC_WOIF__Y_Top_Pixel : woiData.indentFlags & ZC_WOIF__Y_Bottom_Pixel)) ecZC_WindowResized.Disconnect();
    ZC_Vec2<float> prevBL = bl_WOI;
    CalculateCurrentIndents_WOI();
    if (prevBL != bl_WOI) VChanged_bl_WOI();
}

void ZC_WindowOrthoIndent1::GetIndentParams_WOI(float& _indentX, float& _indentY, ZC_WindowOrthoIndentFlags& _indentFlags)
{
    _indentX = woiData.indentX;
    _indentY = woiData.indentY;
    _indentFlags = _indentFlags;
}

ZC_Vec2<float> ZC_WindowOrthoIndent1::GetBL_WOI()
{
    return bl_WOI;
}

void ZC_WindowOrthoIndent1::CalculateCenterPixelIndentsFromBL_WOI(const ZC_Vec2<float>& bl)
{
    assert(((woiData.indentFlags & ZC_WOIF__X_Center_Pixel) == ZC_WOIF__X_Center_Pixel) && ((woiData.indentFlags & ZC_WOIF__Y_Center_Pixel) == ZC_WOIF__Y_Center_Pixel));
    if (((woiData.indentFlags & ZC_WOIF__X_Center_Pixel) != ZC_WOIF__X_Center_Pixel) || ((woiData.indentFlags & ZC_WOIF__Y_Center_Pixel) != ZC_WOIF__Y_Center_Pixel)) return;
    if (bl_WOI == bl) return;
    bl_WOI = bl;

    int win_width = 0;
    int win_height = 0;
    ZC__Window::GetSize(win_width, win_height);

    woiData.indentX = (bl[0] + (woiData.width / 2.f)) - (win_width / 2.f);
    woiData.indentY = is_Y_ZeroPointOnTop ? (bl[1] - (woiData.height / 2.f)) - (win_height / 2.f) : (bl[1] + (woiData.height / 2.f)) - (win_height / 2.f);
}

ZC_WindowOrthoIndent1::ZC_WindowOrthoIndent1(bool _is_Y_ZeroPointOnTop, const ZC_WOIData& _woiData)
    : woiData(_woiData),
    is_Y_ZeroPointOnTop(_is_Y_ZeroPointOnTop),
    ecZC_WindowResized((woiData.indentFlags & ZC_WOIF__X_Left_Pixel && woiData.indentFlags & ZC_WOIF__Y_Bottom_Pixel) ? ZC_EC()     //  bl corner will allways have correct indents
        : ZC_Events::ConnectWindowResize({ &ZC_WindowOrthoIndent1::ZC_WindowResized, this }))
{
    CheckAndSetIndentData(woiData.indentX, woiData.indentY, woiData.indentFlags);
    CalculateCurrentIndents_WOI();
}

ZC_WindowOrthoIndent1::ZC_WindowOrthoIndent1(const ZC_WindowOrthoIndent1& woi)
    : woiData(woi.woiData),
    bl_WOI{ woi.bl_WOI },
    is_Y_ZeroPointOnTop(woi.is_Y_ZeroPointOnTop),
    ecZC_WindowResized(ZC_Events::ConnectWindowResize({ &ZC_WindowOrthoIndent1::ZC_WindowResized, this }))
{}

// ZC_WindowOrthoIndent::ZC_WindowOrthoIndent(ZC_WindowOrthoIndent&& woi)
//     : currentIndents{ woi.currentIndents[0], woi.currentIndents[1] },
//     is_Y_ZeroPointOnTop(woi.is_Y_ZeroPointOnTop),
//     width(woi.width),
//     height(woi.height),
//     indentX(woi.indentX),
//     indentY(woi.indentY),
//     indentFlags(woi.indentFlags)
// {
//     ecZC_WindowResized.NewConnection(std::move(woi.ecZC_WindowResized));
// }

void ZC_WindowOrthoIndent1::CalculateCurrentIndents_WOI()
{
    int windowWidth, windowHeight;
    ZC__Window::GetSize(windowWidth, windowHeight);
    CalculateIndents(static_cast<float>(windowWidth), static_cast<float>(windowHeight));
}

void ZC_WindowOrthoIndent1::CheckAndSetIndentData(float _indentX, float _indentY, ZC_WindowOrthoIndentFlags _indentFlags)
{
    int _indentFlagX = 0;   //  find flag for X
    if (_indentFlags & ZC_WOIF__X_Left_Pixel) _indentFlagX = ZC_WOIF__X_Left_Pixel;
    else if (_indentFlags & ZC_WOIF__X_Left_Percent) _indentFlagX = _indentX >= 0.f && _indentX <= 1.f ? ZC_WOIF__X_Left_Percent : 0; //  indent for percent must be in diaposon [0,1]
    else if (_indentFlags & ZC_WOIF__X_Right_Pixel) _indentFlagX = ZC_WOIF__X_Right_Pixel;
    else if (_indentFlags & ZC_WOIF__X_Right_Percent) _indentFlagX = _indentX >= 0.f && _indentX <= 1.f ? ZC_WOIF__X_Right_Percent : 0; //  indent for percent must be in diaposon [0,1]
    else if (_indentFlags & ZC_WOIF__X_Center) _indentFlagX = ZC_WOIF__X_Center;
    else if (_indentFlags & ZC_WOIF__X_Center_Pixel) _indentFlagX = ZC_WOIF__X_Center_Pixel;
    assert(_indentFlagX != 0);  //  no valid data

    int _indentFlagY = 0;
    if (_indentFlags & ZC_WOIF__Y_Top_Pixel) _indentFlagY = ZC_WOIF__Y_Top_Pixel;
    else if (_indentFlags & ZC_WOIF__Y_Top_Percent) _indentFlagY = _indentY >= 0.f && _indentY <= 1.f ? ZC_WOIF__Y_Top_Percent : 0;
    else if (_indentFlags & ZC_WOIF__Y_Bottom_Pixel) _indentFlagY = ZC_WOIF__Y_Bottom_Pixel;
    else if (_indentFlags & ZC_WOIF__Y_Bottom_Percent) _indentFlagY = _indentY >= 0.f && _indentY <= 1.f ? ZC_WOIF__Y_Bottom_Percent : 0;
    else if (_indentFlags & ZC_WOIF__Y_Center) _indentFlagY = ZC_WOIF__Y_Center;
    else if (_indentFlags & ZC_WOIF__Y_Center_Pixel) _indentFlagY = ZC_WOIF__Y_Center_Pixel;
    assert(_indentFlagY != 0);

    woiData.indentFlags = static_cast<ZC_WindowOrthoIndentFlags>(_indentFlagX | _indentFlagY);

    assert((_indentFlags - woiData.indentFlags) == 0);    //  flags have more than one X or more than one Y
    
    woiData.indentX = _indentX;
    woiData.indentY = _indentY;
}

void ZC_WindowOrthoIndent1::ZC_WindowResized(float windowWidth, float windowHeight)
{
    CalculateIndents(windowWidth, windowHeight);
    VChanged_bl_WOI();
}

void ZC_WindowOrthoIndent1::CalculateIndents(float windowWidth, float windowHeight)
{
    switch (woiData.indentFlags & 63) //  remove Y flag (63 -> ...000111111)
    {   //  x indent
    case ZC_WOIF__X_Left_Pixel: bl_WOI[0] = woiData.indentX; break;
    case ZC_WOIF__X_Left_Percent: bl_WOI[0] = windowWidth * woiData.indentX; break;
    case ZC_WOIF__X_Right_Pixel: bl_WOI[0] = windowWidth - woiData.indentX - woiData.width; break;
    case ZC_WOIF__X_Right_Percent: bl_WOI[0] = windowWidth - (windowWidth * woiData.indentX) - woiData.width; break;
    case ZC_WOIF__X_Center: bl_WOI[0] = (windowWidth / 2.f) - (woiData.width / 2.f); break;
    case ZC_WOIF__X_Center_Pixel: bl_WOI[0] = (windowWidth / 2.f) - (woiData.width / 2.f) + woiData.indentX; break;
    default: break;
    }
    switch (woiData.indentFlags & 4032)   //  remove X flag (4032 -> ...111111000000)
    {   //  y indent
    case ZC_WOIF__Y_Top_Pixel: bl_WOI[1] = is_Y_ZeroPointOnTop ? woiData.indentY : windowHeight - woiData.indentY - woiData.height; break;
    case ZC_WOIF__Y_Top_Percent: bl_WOI[1] = is_Y_ZeroPointOnTop ? windowHeight * woiData.indentY : windowHeight - (windowHeight * woiData.indentY) - woiData.height; break;
    case ZC_WOIF__Y_Bottom_Pixel: bl_WOI[1] = is_Y_ZeroPointOnTop ? windowHeight - woiData.indentY - woiData.height : woiData.indentY; break;
    case ZC_WOIF__Y_Bottom_Percent: bl_WOI[1] = is_Y_ZeroPointOnTop ? windowHeight - (windowHeight * woiData.indentY) - woiData.height : windowHeight * woiData.indentY; break;
    case ZC_WOIF__Y_Center: bl_WOI[1] = (windowHeight / 2.f) - (woiData.height / 2.f); break;
    case ZC_WOIF__Y_Center_Pixel: bl_WOI[1] = is_Y_ZeroPointOnTop ? (windowHeight / 2.f) + (woiData.height / 2.f) + woiData.indentY
                                                                    : (windowHeight / 2.f) - (woiData.height / 2.f) + woiData.indentY; break;
    default: break;
    }
}







ZC_WindowOrthoIndent::~ZC_WindowOrthoIndent()
{
    ecZC_WindowResized.Disconnect();
}

bool ZC_WindowOrthoIndent::SetNewSize_WOI(float _width, float _height)
{
    if (width == _width && height == _height) return false;
    width = _width;
    height = _height;
    CalculateCurrentIndents_WOI();
    return true;
}

void ZC_WindowOrthoIndent::GetSize(float& _width, float& _height)
{
    _width = width;
    _height = height;
}

void ZC_WindowOrthoIndent::SetNewIndentParams_WOI(float _indentX, float _indentY, ZC_WindowOrthoIndentFlags _indentFlags)
{
    CheckAndSetIndentData(_indentX, _indentY, _indentFlags);
    CalculateCurrentIndents_WOI();
}

void ZC_WindowOrthoIndent::GetIndentParams_WOI(float& _indentX, float& _indentY, ZC_WindowOrthoIndentFlags& _indentFlags)
{
    _indentX = indentX;
    _indentY = indentY;
    _indentFlags = indentFlags;
}

const ZC_Vec2<float>& ZC_WindowOrthoIndent::GetIndents()
{
    return currentIndents;
}

ZC_WindowOrthoIndent::ZC_WindowOrthoIndent(bool _is_Y_ZeroPointOnTop, float _width, float _height, float _indentX, float _indentY, ZC_WindowOrthoIndentFlags _indentFlags)
    : is_Y_ZeroPointOnTop(_is_Y_ZeroPointOnTop),
    width(_width),
    height(_height),
    ecZC_WindowResized(ZC_Events::ConnectWindowResize({ &ZC_WindowOrthoIndent::ZC_WindowResized, this }))
{
    CheckAndSetIndentData(_indentX, _indentY, _indentFlags);
    CalculateCurrentIndents_WOI();
}

ZC_WindowOrthoIndent::ZC_WindowOrthoIndent(const ZC_WindowOrthoIndent& woi)
    : currentIndents{ woi.currentIndents[0], woi.currentIndents[1] },
    is_Y_ZeroPointOnTop(woi.is_Y_ZeroPointOnTop),
    width(woi.width),
    height(woi.height),
    indentX(woi.indentX),
    indentY(woi.indentY),
    indentFlags(woi.indentFlags),
    ecZC_WindowResized(ZC_Events::ConnectWindowResize({ &ZC_WindowOrthoIndent::ZC_WindowResized, this }))
{}

// ZC_WindowOrthoIndent::ZC_WindowOrthoIndent(ZC_WindowOrthoIndent&& woi)
//     : currentIndents{ woi.currentIndents[0], woi.currentIndents[1] },
//     is_Y_ZeroPointOnTop(woi.is_Y_ZeroPointOnTop),
//     width(woi.width),
//     height(woi.height),
//     indentX(woi.indentX),
//     indentY(woi.indentY),
//     indentFlags(woi.indentFlags)
// {
//     ecZC_WindowResized.NewConnection(std::move(woi.ecZC_WindowResized));
// }

void ZC_WindowOrthoIndent::CalculateCurrentIndents_WOI()
{
    int windowWidth, windowHeight;
    ZC__Window::GetSize(windowWidth, windowHeight);
    CalculateIndents(static_cast<float>(windowWidth), static_cast<float>(windowHeight));
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
    VCallAfterZC_WindowResizedWOI();
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