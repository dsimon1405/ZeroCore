#include <ZC/Tools/ZC_TornToCamera.h>

#include <ZC/Tools/Container/ZC_ContFunc.h>
#include <ZC/Events/ZC_Events.h>
#include <ZC/Objects/Camera/ZC_Camera.h>
#include <ZC/ErrorLogger/ZC_ErrorLogger.h>

#include <cassert>

ZC_TornToCamera::~ZC_TornToCamera()
{
    if (!ZC_ForwardListErase(allHeirs, this)) ZC_ErrorLogger::Err("Can't find to delete!", __FILE__, __LINE__);  
    if (isNeedUpdate) ZC_ForwardListErase(needUpdate, this);  
    if (allHeirs.empty()) sconEventsEnd.Disconnect();
}

void ZC_TornToCamera::SetPosition(const ZC_Vec3<float>& pos)
{
    if (position == pos) return;
    position = pos;
    if (!isNeedUpdate)
    {
        isNeedUpdate = true;
        needUpdate.emplace_front(this);
    }
}

void ZC_TornToCamera::SetScale(float _scale)
{
    if (scale == _scale) return;
    if (scale != 0.f)   //  need return to 1.0 -> 100%
    {   //  (model[0] / scale) -> return 100% scale, if new scale not null make scale -> (* _scale)
        auto column0 = _scale == 0.f ? model[0] / scale : model[0] / scale * _scale,
            column1 = _scale == 0.f ? model[1] / scale : model[1] / scale * _scale,
            column2 = _scale == 0.f ? model[2] / scale : model[2] / scale * _scale;
        model[0] = column0;
        model[1] = column1;
        model[2] = column2;
    }
    scale = _scale;
}

ZC_TornToCamera::ZC_TornToCamera(const ZC_Vec3<float>& _position, float _scale)
    : position(_position),
    scale(_scale)
{
    if (allHeirs.empty()) sconEventsEnd = ZC_Events::ConnectHandleEventsEnd({ &ZC_TornToCamera::Update });
    allHeirs.emplace_front(this);
    auto pCamera = ZC_Camera::GetActiveCamera();
    this->CalculateModel(pCamera->GetPosition(), pCamera->GetUp()[2] == 1.f);
}

ZC_TornToCamera::ZC_TornToCamera(const ZC_TornToCamera& ttc)
    : model(ttc.model),
    position(ttc.position),
    isNeedUpdate(ttc.isNeedUpdate),
    scale(ttc.scale)
{
    allHeirs.emplace_front(this);
    if (isNeedUpdate) needUpdate.emplace_front(this);
}

void ZC_TornToCamera::Update(float time)
{
    static ZC_Vec3<float> previousCameraPosition;

    auto pCamera = ZC_Camera::GetActiveCamera();
    auto currentCamPos = pCamera->GetPosition();
    bool isNormalVerticalOrientation = pCamera->GetUp()[2] == 1.f;
    if (previousCameraPosition == currentCamPos)
    {
        for (auto pHeir : needUpdate)
        {
            pHeir->CalculateModel(previousCameraPosition, isNormalVerticalOrientation);
        }
    }
    else
    {
        previousCameraPosition = currentCamPos;
        for (auto pHeir : allHeirs)
        {
            pHeir->CalculateModel(previousCameraPosition, isNormalVerticalOrientation);
        }
    }
    needUpdate.clear();
}

void ZC_TornToCamera::CalculateModel(const ZC_Vec3<float>& camPos, bool isNormalVerticalOrientation)
{
    auto posToCam = position - camPos;
    auto normPosToCam = ZC_Vec::Normalize(posToCam);
    ZC_Vec3<float> vertivalStartPoint = ZC_Vec::Normalize(ZC_Vec3<float>(posToCam[0], posToCam[1], 0.f));    //  also end point for horizontal rotation
    auto cosVertical = ZC_Vec::Dot(normPosToCam, vertivalStartPoint);
    auto sinVertical = sqrt(1.f - (cosVertical * cosVertical));
    if (posToCam[2] > 0) sinVertical *= -1.f;
    if (!isNormalVerticalOrientation) cosVertical *= -1.f;

    ZC_Vec3<float> horizontalStartPoint(0.f, 1.f, 0.f);
    auto cosHorizontal = ZC_Vec::Dot(horizontalStartPoint, vertivalStartPoint);   //  vertivalStartPoint is normalized end point for horizontal angle
    auto sinHorizontal = sqrt(1.f - (cosHorizontal * cosHorizontal));
    if (posToCam[0] < 0) sinHorizontal *= -1.f;
    if (!isNormalVerticalOrientation)
    {
        cosHorizontal *= -1.f;
        sinHorizontal *= -1.f;
    }

    model = ZC_Mat4<float>(1.f);
    model.Translate(position);
    Rotate(model, cosHorizontal, sinHorizontal, { 0, 0, -1.f });
    Rotate(model, cosVertical, sinVertical, { -1.f, 0, 0 });
    if (scale != 0.f) model.Scale(scale, scale, scale);

    isNeedUpdate = false;
}

void ZC_TornToCamera::Rotate(ZC_Mat4<float>& model, float cos, float sin, const ZC_Vec3<float>& axise)
{
    ZC_Vec3<float> temp(axise * (static_cast<float>(1) - cos));

    ZC_Vec3<float> rotateX;
    rotateX[0] = cos + temp[0] * axise[0];
    rotateX[1] = temp[0] * axise[1] + sin * axise[2];
    rotateX[2] = temp[0] * axise[2] - sin * axise[1];
    ZC_Vec4<float> columnX = model[0] * rotateX[0] + model[1] * rotateX[1] + model[2] * rotateX[2];

    ZC_Vec3<float> rotateY;
    rotateY[0] = temp[1] * axise[0] - sin * axise[2];
    rotateY[1] = cos + temp[1] * axise[1];
    rotateY[2] = temp[1] * axise[2] + sin * axise[0];
    ZC_Vec4<float> columnY = model[0] * rotateY[0] + model[1] * rotateY[1] + model[2] * rotateY[2];

    ZC_Vec3<float> rotateZ;
    rotateZ[0] = temp[2] * axise[0] + sin * axise[1];
    rotateZ[1] = temp[2] * axise[1] - sin * axise[0];
    rotateZ[2] = cos + temp[2] * axise[2];
    ZC_Vec4<float> columnZ = model[0] * rotateZ[0] + model[1] * rotateZ[1] + model[2] * rotateZ[2];

    model[0] = columnX;
    model[1] = columnY;
    model[2] = columnZ;
}