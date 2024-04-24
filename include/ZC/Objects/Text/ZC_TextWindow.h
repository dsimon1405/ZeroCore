#pragma once

#include <ZC/Tools/ZC_WindowOrthoIndent.h>
#include "ZC_TextData.h"

//  Class for rendering text in the window(ortho matrix).
class ZC_TextWindow : protected ZC_WindowOrthoIndent, public ZC_TextData
{
public:
    /*
    Params:
    - fontData - fonts name and height in pixels.
    - _text - text for rendering (supports'\n' new line symbol).
    - _alignment - alignment across text consisting of several lines of different lengths.
    - color - texts color.
    - windowIndentX - value of horizontal indent from border of global window. If used IndentFlag:
        ZC_WOIF__X_Left_Pixel, ZC_WOIF__X_Right_Pixel -> value must be not negative, otherwise sets 0.f;
        ZC_WOIF__X_Left_Percent, ZC_WOIF__X_Right_Percent -> value must be 0.0f - 1.f (where 1.f is 100%);
        ZC_WOIF__X_Center -> value no metter.
    - windowIndentY - value of vertival indent from border of global window. If used IndentFlag: 
        ZC_WOIF__Y_Top_Pixel, ZC_WOIF__Y_Bottom_Pixel -> value must be not negative, otherwise sets 0.f
        ZC_WOIF__Y_Top_Percent, ZC_WOIF__Y_Bottom_Percent -> value must be 0.0f - 1.f (where 1.f is 100%);
        ZC_WOIF__Y_Center -> value no metter.
    - indentFlags - flags of indent horizontal(X) and vertical(Y) from border of global window to IGWindow. Must be set one flag for X and one flag for Y. Example: X_Left_Pixel | Y_Top_Pixel.
    - needDraw - if true, adds to renderer after creatioin.
    */
    ZC_TextWindow(ZC_FontData fontData, const std::string& _text, ZC_TextAlignment _alignment,
            float windowIndentX, float windowIndentY, ZC_WindowOrthoIndentFlags indentFlags, bool needDraw);

    /*
    Params:
    - windowIndentX - value of horizontal indent from border of global window. If used IndentFlag:
        ZC_WOIF__X_Left_Pixel, ZC_WOIF__X_Right_Pixel -> value must be not negative, otherwise sets 0.f;
        ZC_WOIF__X_Left_Percent, ZC_WOIF__X_Right_Percent -> value must be 0.0f - 1.f (where 1.f is 100%);
        ZC_WOIF__X_Center -> value no metter.
    - windowIndentY - value of vertival indent from border of global window. If used IndentFlag: 
        ZC_WOIF__Y_Top_Pixel, ZC_WOIF__Y_Bottom_Pixel -> value must be not negative, otherwise sets 0.f
        ZC_WOIF__Y_Top_Percent, ZC_WOIF__Y_Bottom_Percent -> value must be 0.0f - 1.f (where 1.f is 100%);
        ZC_WOIF__Y_Center -> value no metter.
    - indentFlags - flags of indent horizontal(X) and vertical(Y) from border of global window to IGWindow. Must be set one flag for X and one flag for Y. Example: X_Left_Pixel | Y_Top_Pixel.
    */
    void SetIndentData(float windowIndentX, float windowIndentY, ZC_WindowOrthoIndentFlags indentFlags);

    /*
    Makes a copy of the text.
    The methods affect only the current copy: SetIndentData(), NeedDraw(), SetColor(), SetRendererLevel().
    Methods that include effects on all copies: SetText(), SetAlignment(), SetTextAndAlignment().
    */
    ZC_TextWindow MakeCopy(float windowIndentX, float windowIndentY, ZC_WindowOrthoIndentFlags indentFlags) const;

    /*
    Sets alpha channel (transparent).

    Params:
    - alpha - must be in range 0.f - 1.f. Closer to 0.f more transparent.
    */
    void SetAlpha(float alpha);

private:
    ZC_TextWindow(const ZC_TextWindow& tw);

    void SetNewTextSize() override;
};