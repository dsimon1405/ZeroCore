#pragma once

#include "ZC_TextData.h"
#include <ZC/Tools/ZC_TornToCamera.h>

//  Class for rendering text to face the camera, in the scene(perspective * view -> matrix).
class ZC_TextSceneTurnedToCamera : public ZC_TextData, public ZC_TornToCamera
{
public:
    /*
    Params:
    - fontData - fonts name and height in pixels (recomended to use a height greater than 150 pixels).
    - _text - text for rendering (supports'\n' new line symbol).
    - _alignment - alignment across text, consisting of several lines of different lengths.
    - position - position in the scene.
    - scale - default scaling(parameter = 0.f) creates letters with height near 2.f.
    - needDraw - if true, adds to renderer after creatioin.
    */
    ZC_TextSceneTurnedToCamera(ZC_FontData fontData, const std::string& _text, ZC_TextAlignment _alignment,
            const ZC_Vec3<float>& _position, bool needDraw, float _scale = 0.f);

    /*
    Makes a copy of the text.
    The methods affect only the current copy: SetPosition(), SetScale(), NeedDraw(), SetColor(), SetRendererLevel().
    Methods that include effects on all copies: SetText() SetAlignment() SetTextAndAlignment().
    */
    ZC_TextSceneTurnedToCamera MakeCopy() const;

private:
    ZC_TextSceneTurnedToCamera(const ZC_TextSceneTurnedToCamera& tsttc);
};