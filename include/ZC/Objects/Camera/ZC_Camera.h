#pragma once

#include "Matrix/ZC_PerspView.h"
#include "Matrix/ZC_Ortho.h"

/*
Camera matrix control class. Must be created after ZC_Window!
The product of the (perspective * view) and ortho matrices is passed to GLSL.
Use the global (for all shaders that have this uniform) std140 uniform storage in GLSL code.
Example GLSL:
layout (std140, binding = 0) uniform Persp { mat4 perspView; };
layout (std140, binding = 1) uniform Ortho { mat4 ortho; };
*/
class ZC_Camera
{
public:
    ZC_Camera(const ZC_PerspView& _perspView, const ZC_Ortho& _ortho);

    ZC_Camera(const ZC_Camera& cm) noexcept;
    ZC_Camera& operator = (const ZC_Camera& cm) noexcept;

    ZC_Vec3<float> GetCamPos() const noexcept;
    ZC_Vec3<float> GetLookOn() const noexcept;
    ZC_Vec3<float> GetUp() const noexcept;
    ZC_Camera& SetCamPos(const ZC_Vec3<float>& _camPos) noexcept;
    ZC_Camera& SetLookOn(const ZC_Vec3<float>& _lookOn) noexcept;
    ZC_Camera& SetUp(const ZC_Vec3<float>& _wUp) noexcept;

private:
    ZC_PerspView perspView;
    ZC_Ortho ortho;
    
    void Update();
    void ResizeCallBack(float width, float height);
};