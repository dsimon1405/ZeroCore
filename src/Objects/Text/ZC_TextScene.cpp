#include <ZC/Objects/Text/ZC_TextScene.h>

ZC_TextScene::ZC_TextScene(ZC_FontData fontData, const std::string& _text, ZC_TextAlignment _alignment, bool needDraw)
    : ZC_TextData(ZC_ShProgs::Get(ShPN_ZC_TextScene), ZC_FO_center, fontData, _text, _alignment, ZC_DrawerLevels::TextScene, needDraw),
    scale(startHeightInScene / fontData.pixelsHeight)
{
    dsController.SetUniformsData(ZC_UN_unModel, &(model.Scale(scale, scale, scale)));
}

void ZC_TextScene::SetPosition(const ZC_Vec3<float>& pos) noexcept
{
    if (position == pos) return;
    position = pos;
    RecalculateModelMatrix();
}

void ZC_TextScene::SetRotation(float angle, const ZC_Vec3<float>& axises) noexcept
{
    if (rotationAngle == angle && rotationAxises == axises) return;
    rotationAngle = angle;
    rotationAxises = axises;
    RecalculateModelMatrix();
}

void ZC_TextScene::SetScale(float _scale)
{
    if (scale == _scale) return;
    scale = _scale;
    RecalculateModelMatrix();
}

void ZC_TextScene::SetModelMatrixData(ZC_Vec3<float>* pPosition, float* pRotationAngle, const ZC_Vec3<float>* pRotationAxises, float* pScale)
{
    if (pPosition) position = *pPosition;
    if (pRotationAngle && pRotationAxises)
    {
        rotationAngle = *pRotationAngle;
        rotationAxises = *pRotationAxises;
    }
    if (pScale) scale = *pScale;
    RecalculateModelMatrix();
}

ZC_TextScene ZC_TextScene::MakeCopy(ZC_Vec3<float>* pPosition, float* pRotationAngle, const ZC_Vec3<float>* pRotationAxises, float* pScale) const
{
    ZC_TextScene copy = { *this };
    copy.SetModelMatrixData(pPosition, pRotationAngle, pRotationAxises, pScale);
    return copy;
} 

ZC_TextScene::ZC_TextScene(const ZC_TextScene& ts)
    : ZC_TextData(dynamic_cast<const ZC_TextData&>(ts)),
    model(ts.model),
    scale(ts.scale),
    position(ts.position),
    rotationAngle(ts.rotationAngle),
    rotationAxises(ts.rotationAxises)
{
    dsController.SetUniformsData(ZC_UN_unModel, &model);
}

void ZC_TextScene::RecalculateModelMatrix()
{
    model = ZC_Mat4<float>(1.f);
    model.Translate(position);
    if (rotationAngle != 0.f) model.Rotate(rotationAngle, rotationAxises);
    model.Scale(scale, scale, scale);
}