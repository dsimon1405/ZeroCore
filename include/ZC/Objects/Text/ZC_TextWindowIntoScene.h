#pragma once

#include "ZC_TextData.h"

/*
Class for rendering text in the window(ortho matrix), with position in the scene.
The text will have the same pixel height at different distances between the text in the scene and the camera.
*/
class ZC_TextWindowIntoScene : public ZC_TextData
{
public:
    /*
    Params:
    fontData - fonts name and height in pixels.
    _text - text for rendering (supports'\n' new line symbol).
    _alignment - alignment across text, consisting of several lines of different lengths.
    position - position in the scene.
    */
    ZC_TextWindowIntoScene(ZC_FontNameHeight fontData, const std::string &_text, ZC_TextAlignment _alignment, const ZC_Vec3<float>& position);

    //  Changes position.
    void SetPosition(const ZC_Vec3<float>& position) noexcept;

    /*
    Makes a copy of the text.
    The methods affect only the current copy: SetPosition(), NeedDraw(), SetColor(), SetRendererLevel().
    Methods that include effects on all copies: SetText() SetAlignment() SetTextAndAlignment().
    */
    ZC_TextWindowIntoScene MakeCopy() const;

private:
    ZC_Vec3<float> positionIntoScene;

    ZC_TextWindowIntoScene(const ZC_TextWindowIntoScene& twis);
};