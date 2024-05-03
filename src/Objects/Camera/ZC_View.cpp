#include <ZC/Objects/Camera/ZC_View.h>

#include <ZC/Tools/Math/ZC_Mat.h>

ZC_View::ZC_View(ZC_Vec3<float>* _position, const ZC_Vec3<float>& _lookOn, const ZC_Vec3<float>& _up)
    : position(_position),
    lookOn(_lookOn),
    up(_up)
{}

ZC_View::ZC_View(ZC_View&& v)
    : position(v.position),
    lookOn(v.lookOn),
    up(v.up),
    viewNeedUpdate(v.viewNeedUpdate),
    sigChangeCamPos(std::move(v.sigChangeCamPos))
{}

void ZC_View::SetPosition(const ZC_Vec3<float>& _position)
{
    if (*position == _position) return;
    *position = _position;
    viewNeedUpdate = true;
    isViewActual = false;

    sigChangeCamPos(*position);
}

void ZC_View::SetLookOn(const ZC_Vec3<float>& _lookOn)
{
    if (lookOn == _lookOn) return;
    lookOn = _lookOn;
    viewNeedUpdate = true;
    isViewActual = false;
}

void ZC_View::SetUp(const ZC_Vec3<float>& _up)
{
    if (up == _up) return;
    up = _up;
    viewNeedUpdate = true;
    isViewActual = false;
}

ZC_Vec3<float> ZC_View::GetPosition() const noexcept
{
    return *position;
}

ZC_Vec3<float> ZC_View::GetLookOn() const noexcept
{
    return lookOn;
}

ZC_Vec3<float> ZC_View::GetUp() const noexcept
{
    return up;
}

const ZC_Mat4<float>* ZC_View::GetView()
{
    ActualizeViewMatrix();
    return &view;
}

bool ZC_View::ViewUpdate()
{
    if (!viewNeedUpdate) return false;

    viewNeedUpdate = false;
    ActualizeViewMatrix();
    return true;
}

ZC_EC ZC_View::ConnectChangeCameraPosition(ZC_Function<void(const ZC_Vec3<float>&)>&& func)
{
    return sigChangeCamPos.Connect(std::move(func));
}

void ZC_View::ActualizeViewMatrix()
{
    if (isViewActual) return;
    
    isViewActual = true;
    view = ZC_Mat::LookAt(*position, lookOn, up);
}