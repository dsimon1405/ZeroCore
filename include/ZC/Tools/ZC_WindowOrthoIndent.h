#pragma once

#include <ZC/Tools/Signal/ZC_SConnection.h>
#include <ZC/Tools/Function/ZC_Function.h>

typedef int ZC_WindowOrthoIndentFlags;  //  look ZC_WindowOrthoIndentFlag

//  must be only one X and only one Y
enum ZC_WindowOrthoIndentFlag
{
    X_Left_Pixel              = 1,
    X_Left_Percent            = 1 << 1,
    X_Right_Pixel             = 1 << 2,
    X_Right_Percent           = 1 << 3,
    X_Center                  = 1 << 4,
    Y_Top_Pixel               = 1 << 5,
    Y_Top_Percent             = 1 << 6,
    Y_Bottom_Pixel            = 1 << 7,
    Y_Bottom_Percent          = 1 << 8,
    Y_Center                  = 1 << 9,
};

class ZC_WindowOrthoIndent
{
public:
    virtual ~ZC_WindowOrthoIndent();    //  signald siconnect !!!!!!!!

protected:
    float currentIndentX = 0,
        currentIndentY = 0;

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
        X_Left_Pixel, X_Right_Pixel -> value must be not negative, otherwise sets 0.f;
        X_Left_Percent, X_Right_Percent -> value must be 0.0f - 1.f (where 1.f is 100%);
        X_Center -> value no metter.
    _indentX - value of vertival indent from border of global window. If used IndentFlag: 
        Y_Top_Pixel, Y_Bottom_Pixel -> value must be not negative, otherwise sets 0.f
        Y_Top_Percent, Y_Bottom_Percent -> value must be 0.0f - 1.f (where 1.f is 100%);
        Y_Center -> value no metter.
    _indentFlags - flags of indent horizontal(X) and vertical(Y) from border of global window to IGWindow. Must be set one flag for X and one flag for Y. Example: X_Left_Pixel | Y_Top_Pixel.
    pFuncWindowResized - if need call some function after resize of the window set it, if not make constructor call {}.
    */
    ZC_WindowOrthoIndent(bool _is_Y_ZeroPointOnTop, float _width, float _height, float _indentX, float _indentY,
        ZC_WindowOrthoIndentFlags _indentFlags, ZC_Function<void()>&& pFuncWindowResized);
    
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
    ZC_Function<void()> funcWindowResized;

    ZC_SConnection SetIndentData(float _indentX, float _indentY, ZC_WindowOrthoIndentFlags _indents);
    void ZC_WindowResized(float width, float height);
    void CalculateIndents(float width, float height);
};