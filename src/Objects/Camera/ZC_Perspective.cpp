#include <ZC/Objects/Camera/ZC_Perspective.h>

#include <ZC/Tools/Math/ZC_Mat.h>

ZC_Perspective::ZC_Perspective(float _fovy, float _aspect, float _nearPlane, float _farPlane)
    : fovy(_fovy),
    aspect(_aspect),
    nearPlane(_nearPlane),
    farPlane(_farPlane)
{}

void ZC_Perspective::SetFovy(float _fovy)
{
    if (fovy == _fovy) return;
    fovy = _fovy;
    perspNeedUpdate = true;
    isPerspectiveActual = false;
}

void ZC_Perspective::SetAspect(float _aspect)
{
    if (aspect == _aspect) return;
    aspect = _aspect;
    perspNeedUpdate = true;
    isPerspectiveActual = false;
}

void ZC_Perspective::SetNearPlane(float _nearPlane)
{
    if (nearPlane == _nearPlane) return;
    nearPlane = _nearPlane;
    perspNeedUpdate = true;
    isPerspectiveActual = false;
}

void ZC_Perspective::SetFarPlane(float _farPlane)
{
    if (farPlane == _farPlane) return;
    farPlane = _farPlane;
    perspNeedUpdate = true;
    isPerspectiveActual = false;
}

float ZC_Perspective::GetFovy() const noexcept
{
    return fovy;
}

float ZC_Perspective::GetAspect() const noexcept
{
    return aspect;
}

float ZC_Perspective::GetNearPlane() const noexcept
{
    return nearPlane;
}

float ZC_Perspective::GetFraPlane() const noexcept
{
    return farPlane;
}

const ZC_Mat4<float>* ZC_Perspective::GetPerspective()
{
    ActualizePerspective();
    return &perspective;
}

bool ZC_Perspective::PerspectiveUpdate()
{
    if (!perspNeedUpdate) return false;

    perspNeedUpdate = false;
    ActualizePerspective();
    return true;
}

void ZC_Perspective::ActualizePerspective()
{
    if (isPerspectiveActual) return;

    isPerspectiveActual = true;
    perspective = ZC_Mat::Perspective(fovy, aspect, nearPlane, farPlane);
}