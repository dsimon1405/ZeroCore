#pragma once

#include <ZC/Events/ZC_EC.h>
#include <ZC/Tools/Function/ZC_Function.h>
#include <ZC/Tools/Math/ZC_Vec2.h>

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
    
    //  Return objects width and height.
    void GetSize(float& _width, float& _height);
    //  Set new indent params and calls CalculateCurrentIndents().
    void SetNewIndentParams(float _indentX, float _indentY, ZC_WindowOrthoIndentFlags _indentFlags);
    //  Return indent params.
    void GetIndentParams(float& _indentX, float& _indentY, ZC_WindowOrthoIndentFlags& _indentFlags);
    //  Returns const reference on vector of X, Y indent.
    const ZC_Vec2<float>& GetIndents();

protected:
    ZC_Vec2<float> currentIndents;   //  indents into window from bottom left corner[0,0] -> currentIndents[0] - X, currentIndents[1] - Y 

    /*
    Params:
    - is_Y_ZeroPointOnTop - must be true for next Y axis object position into the window shema, if Y axis inverted - false:
        x_0,  y_0  --------  x_max, y_0
            |                     |
            |                     |
            |                     |
        x_0, y_max -------- x_max, y_max 
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
    */
    ZC_WindowOrthoIndent(bool _is_Y_ZeroPointOnTop, float _width, float _height, float _indentX, float _indentY, ZC_WindowOrthoIndentFlags _indentFlags);
    //  Makes copy of ZC_WindowOrthoIndent with all params, except ZC_upEC ecZC_WindowResized (connects for new copy).
    ZC_WindowOrthoIndent(const ZC_WindowOrthoIndent& woi);
    // ZC_WindowOrthoIndent(ZC_WindowOrthoIndent&& woi);

    //  Set new objects width and height. Returns true, if data was changed, otherwise false.
    bool SetNewSize(float _width, float _height);
    //  Calculate currentIndentX, currentIndentY. Must be called to calculate indents onstart.
    void CalculateCurrentIndents();

private:
    bool is_Y_ZeroPointOnTop;
    float width,
        height,
        indentX,
        indentY;
    ZC_WindowOrthoIndentFlags indentFlags;
    ZC_EC ecZC_WindowResized;

    //  override heirs who need
    virtual void VCallAfterZC_WindowResizedWOI() {}

    void CheckAndSetIndentData(float _indentX, float _indentY, ZC_WindowOrthoIndentFlags _indents);
    void ZC_WindowResized(float width, float height);
    void CalculateIndents(float width, float height);
};