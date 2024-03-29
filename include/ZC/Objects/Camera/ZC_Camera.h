#pragma once

#include "Matrix/ZC_PerspView.h"
#include "Matrix/ZC_Ortho.h"
#include <ZC/Tools/ZC_uptr.h>
#include <ZC/Tools/Signal/ZC_SConnection.h>

class ZC_Camera;
using ZC_upCamera = ZC_uptr<ZC_Camera>;

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
    static ZC_upCamera CreateCamera(const ZC_PerspView& _perspView, const ZC_Ortho& _ortho);

    ~ZC_Camera();

    static ZC_Vec3<float>* GetCamPos() noexcept;
    static ZC_Vec3<float>* GetLookOn() noexcept;
    static ZC_Vec3<float>* GetUp() noexcept;
    ZC_Camera& SetCamPos(const ZC_Vec3<float>& _camPos) noexcept;
    ZC_Camera& SetLookOn(const ZC_Vec3<float>& _lookOn) noexcept;
    ZC_Camera& SetUp(const ZC_Vec3<float>& _wUp) noexcept;
    float GetWindowAspect() const noexcept;

private:
    static inline ZC_Camera* pCurrentCamera = nullptr;

    ZC_PerspView perspView;
    ZC_Ortho ortho;
    ZC_SConnection sconWindowResize;

    ZC_Camera(const ZC_PerspView& _perspView, const ZC_Ortho& _ortho);
    
    void Update();
    void ResizeCallBack(float width, float height);
};