#include <ZC/Objects/Camera/Matrix/ZC_Perspective.h>

#include <ZC/Tools/Math/ZC_Mat.h>
#include <ZC/Video/OpenGL/Buffer/ZC_UBOs.h>

ZC_Perspective::ZC_Perspective(float _fovy, float _near, float _far)
    : fovy(_fovy),
    near(_near),
    far(_far)
{}

void ZC_Perspective::SetFovy(float _fovy) noexcept
{
    fovy = _fovy;
    update = true;
}

void ZC_Perspective::SetPlane(float _near, float _far) noexcept
{
    near = _near;
    far = _far;
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
        perspective = ZC_Mat::Perspective(fovy, aspect, near, far);
        update = false;
        return true;
    }
    update = false;
    return false;
}