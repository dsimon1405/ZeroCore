#pragma once

#include <ZC/Tools/Math/ZC_Mat4.h>
#include <ZC/Video/OpenGL/Buffer/ZC_UBOs.h>

//  View matrix struct wrapper.
struct ZC_View
{
    ZC_Vec3<float> camPos,
                lookOn,
                up;
    ZC_Mat4<float> view;
    bool update = false;
    ZC_UBO* uboCamPos;

    /*
    Create view matrix.

    Params:
    _camPos - camera location coordinates (your eye).
    _lookOn - coordinates where the camera is looking (the place where your eye looks).
    _wUp - a normalized direction pointing to the top. Typically, it is assumed to be (0,0,1) in the world space.
    */
    ZC_View(const ZC_Vec3<float>& _camPos, const ZC_Vec3<float>& _lookOn, const ZC_Vec3<float>& _up, bool camPosUBO);

    void SetCamPos(const ZC_Vec3<float>& _camPos);
    void SetLookOn(const ZC_Vec3<float>& _lookOn);
    void SetUp(const ZC_Vec3<float>& _wUp);

    bool Update();
};