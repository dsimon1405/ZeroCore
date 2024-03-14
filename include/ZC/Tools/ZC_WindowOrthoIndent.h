#pragma once

#include <ZC/Tools/Signal/ZC_SConnection.h>
#include <ZC/Tools/Function/ZC_Function.h>

typedef int ZC_WindowOrthoIndentFlags;  //  look ZC_WindowOrthoIndentFlag

//  must be only one X and only one Y
enum ZC_WindowOrthoIndentFlag
{
    ZC_WOIF__X_Left_Pixel              = 1,
    ZC_WOIF__X_Left_Percent            = 1 << 1,
    ZC_WOIF__X_Right_Pixel             = 1 << 2,
    ZC_WOIF__X_Right_Percent           = 1 << 3,
    ZC_WOIF__X_Center                  = 1 << 4,
    ZC_WOIF__Y_Top_Pixel               = 1 << 5,
    ZC_WOIF__Y_Top_Percent             = 1 << 6,
    ZC_WOIF__Y_Bottom_Pixel            = 1 << 7,
    ZC_WOIF__Y_Bottom_Percent          = 1 << 8,
    ZC_WOIF__Y_Center                  = 1 << 9,
};

class ZC_WindowOrthoIndent
{
public:
    virtual ~ZC_WindowOrthoIndent();

protected:
    float currentIndents[2] { 0, 0 };   //  first x, second y - array to set in ZC_Text classes into uniform as pointer.

    /*
    Params:
    is_Y_ZeroPointOnTop - must be true for next Y axis object position into the window shema, if Y axis inverted - false:
        x_0,  y_0  --------  x_max, y_0
            |                     |
            |                     |
            |                     |
        x_0, y_max -------- x_max, y_max 
    unicName - unic name for window.
    needDraw - window must be draw after creation.
    _width - objects width in pixels.
    _height - objects height in pixels.
    _indentX - value of horizontal indent from border of global window. If used IndentFlag:
        ZC_WOIF__X_Left_Pixel, ZC_WOIF__X_Right_Pixel -> value must be not negative, otherwise sets 0.f;
        ZC_WOIF__X_Left_Percent, ZC_WOIF__X_Right_Percent -> value must be 0.0f - 1.f (where 1.f is 100%);
        ZC_WOIF__X_Center -> value no metter.
    _indentY - value of vertival indent from border of global window. If used IndentFlag: 
        ZC_WOIF__Y_Top_Pixel, ZC_WOIF__Y_Bottom_Pixel -> value must be not negative, otherwise sets 0.f
        ZC_WOIF__Y_Top_Percent, ZC_WOIF__Y_Bottom_Percent -> value must be 0.0f - 1.f (where 1.f is 100%);
        ZC_WOIF__Y_Center -> value no metter.
    _indentFlags - flags of indent horizontal(X) and vertical(Y) from border of global window to IGWindow. Must be set one flag for X and one flag for Y. Example: X_Left_Pixel | Y_Top_Pixel.
    */
    ZC_WindowOrthoIndent(bool _is_Y_ZeroPointOnTop, float _width, float _height, float _indentX, float _indentY, ZC_WindowOrthoIndentFlags _indentFlags);
    //  Makes copy of ZC_WindowOrthoIndent with all params, except ZC_SConnection sconZC_WindowResized (connects for new copy).
    ZC_WindowOrthoIndent(const ZC_WindowOrthoIndent& woi);

    //  Calculate currentIndentX, currentIndentY. Must be called to calculate indents onstart.
    void CalculateCurrentIndents();
    //  Set new objects width and height and calls CalculateCurrentIndents().
    void SetNewSize(float _width, float _height);
    //  Return objects width and size.
    void GetSize(float& _width, float& _height);
    //  Set new indent params and calls CalculateCurrentIndents().
    void SetNewIndentParams(float _indentX, float _indentY, ZC_WindowOrthoIndentFlags _indentFlags);
    //  Return indent params.
    void GetIndentParams(float& _indentX, float& _indentY, ZC_WindowOrthoIndentFlags& _indentFlags);

private:
    bool is_Y_ZeroPointOnTop;
    float width,
        height,
        indentX,
        indentY;
    ZC_WindowOrthoIndentFlags indentFlags;
    ZC_SConnection sconZC_WindowResized;

    //  override heirs who need
    virtual void CallAfterZC_WindowResized() {}

    void CheckAndSetIndentData(float _indentX, float _indentY, ZC_WindowOrthoIndentFlags _indents);
    void ZC_WindowResized(float width, float height);
    void CalculateIndents(float width, float height);
};