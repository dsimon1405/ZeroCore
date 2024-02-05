#include <ZC/Objects/Camera/Matrix/ZC_View.h>

#include <ZC/Tools/Math/ZC_Mat.h>

ZC_View::ZC_View(const ZC_Vec3<float>& _camPos, const ZC_Vec3<float>& _lookOn, const ZC_Vec3<float>& _up, bool camPosUBO)
    : camPos(_camPos),
    lookOn(_lookOn),
    up(_up),
    view(ZC_Mat::LookAt(camPos, lookOn, up)),
    uboCamPos(camPosUBO ? ZC_UBOs::Create(ZC_UBO::BindingPoint::CamPos, nullptr) : nullptr)
{
    uboCamPos->BufferData(sizeof(ZC_Vec3<float>), &camPos[0], GL_DYNAMIC_DRAW);
}

void ZC_View::SetCamPos(const ZC_Vec3<float>& _camPos)
{
    camPos = _camPos;
    update = true;
}

void ZC_View::SetLookOn(const ZC_Vec3<float>& _lookOn)
{
    lookOn = _lookOn;
    update = true;
}

void ZC_View::SetUp(const ZC_Vec3<float>& _up)
{
    up = _up;
    update = true;
}

bool ZC_View::Update()
{
    if (update)
    {
        view = ZC_Mat::LookAt(camPos, lookOn, up);
        uboCamPos->BufferSubData(0, sizeof(ZC_Vec3<float>), &camPos[0]);
        update = false;
        return true;
    }
    update = false;
    return false;
}