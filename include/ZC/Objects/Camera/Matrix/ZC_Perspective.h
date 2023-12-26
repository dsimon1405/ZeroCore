#pragma once

#include <ZC/Tools/Math/ZC_Mat4.h>
#include <ZC/Video/OpenGL/UBO/ZC_UBO.h>

//  Perspective matrix struct wrapper.
struct ZC_Perspective
{
    float aspect = 0.f,
        fovy,
        near,
        far;
    ZC_Mat4<float> perspective {};
    bool update = false;
    /*
    Create perspective matrix. For error check use ZC_ErrorLogger::WasError() and ZC_ErrorLogger::Message() - for more info.

    Params:
    _fovy - viewing angle in degrees.
    _near - length to the top of the frustum.
    _far - length to base of frustum.
    */
    ZC_Perspective(float _fovy = 45.f, float _near = 0.f, float _far = 100.f);
    
    void SetFovy(float _fovy) noexcept;
    void SetPlane(float _near, float _far) noexcept;
    void SetSize(float _aspect) noexcept;
    bool Update();
};