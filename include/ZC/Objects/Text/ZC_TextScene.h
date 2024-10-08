#pragma once

#include "ZC_TextData.h"
#include <ZC/Tools/Math/ZC_Mat4.h>

//  Class for rendering text in the scene(perspective * view -> matrix).
class ZC_TextScene : public ZC_TextData
{
public:
    /*
    Params:
    fontData - fonts name and height in pixels (recomended to use a height greater than 150 pixels).
    _text - text for rendering (supports'\n' new line symbol).
    _alignment - alignment across text, consisting of several lines of different lengths.
    - needDraw - if true, adds to renderer after creatioin.
    */
    ZC_TextScene(ZC_FontData fontData, const std::wstring& _text, ZC_TextAlignment _alignment, bool needDraw);

    //  Changes position.
    void SetPosition(const ZC_Vec3<float>& pos) noexcept;
    /*
    Sets rotation.

    Params:
    angle - rotations angle in degrees.
    axises - axises who rotation, usual is [0,1].
    */
    void SetRotation(float angle, const ZC_Vec3<float>& axises) noexcept;

    //  Changes scale. Default scaling creates letters with height near 2.f.
    void SetScale(float _scale);

    /*
    This function must be used if more than one parameter of the model matrix(position, rotaion, scale) needs to be changed.

    Params is pointers on data, which need to change. If parameter = nullptr, data does not changes.
    */
    void SetModelMatrixData(ZC_Vec3<float>* pPosition, float* pRotationAngle, const ZC_Vec3<float>* pRotationAxises, float* pScale);

    /*
    Makes a copy of the text.
    The methods affect only the current copy: SetPosition(), SetRotation(), SetScale(), SetModelMatrixData(), NeedDraw(), SetColor(), SetRendererLevel().
    Methods that include effects on all copies: SetText() SetAlignment() SetTextAndAlignment().

    Params is pointers on data, which need to change. If parameter = nullptr, the data is copied.
    */
    ZC_TextScene MakeCopy(ZC_Vec3<float>* pPosition, float* pRotationAngle, const ZC_Vec3<float>* pRotationAxises, float* pScale) const;

private:
    ZC_Mat4<float> model { 1.f };
    float scale;
    ZC_Vec3<float> position{ 0.f, 0.f, 0.f };
    float rotationAngle = 0.f;
    ZC_Vec3<float> rotationAxises{ 0.f, 0.f, 0.f };

    ZC_TextScene(const ZC_TextScene& ts);

    void RecalculateModelMatrix();
};