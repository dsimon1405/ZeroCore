#include <ZC/Objects/Camera/Matrix/ZC_Perspective.h>

#include <ZC/Tools/Math/ZC_Mat.h>
#include <ZC/Video/OpenGL/Buffer/ZC_UBOs.h>

ZC_Perspective::ZC_Perspective(float _fovy, float _nearPlane, float _farPlane)
    : fovy(_fovy),
    nearPlane(_nearPlane),
    farPlane(_farPlane)
{}

void ZC_Perspective::SetFovy(float _fovy) noexcept
{
    fovy = _fovy;
    update = true;
}

void ZC_Perspective::SetPlane(float _nearPlane, float _farPlane) noexcept
{
    nearPlane = _nearPlane;
    farPlane = _farPlane;
    update = true;
}

void ZC_Perspective::SetSize(float _aspect) noexcept
{
    aspect = _aspect;
    update = true;
}

bool ZC_Perspective::Update()
{
    if (update)
    {
        perspective = ZC_Mat::Perspective(fovy, aspect, nearPlane, farPlane);
        update = false;
        return true;
    }
    update = false;
    return false;
}