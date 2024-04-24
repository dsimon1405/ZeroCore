#pragma once

#include "ZC_Ortho.h"
#include "ZC_Perspective.h"
#include "ZC_View.h"
#include <ZC/Video/OpenGL/Renderer/ZC_FrameBuffer.h>
#include <ZC/Video/OpenGL/Buffer/ZC_UBO.h>

class ZC_Camera : public ZC_View, public ZC_Perspective, public ZC_Ortho
{
public:
    ZC_Camera(const ZC_Vec3<float>& _camPos, const ZC_Vec3<float>& _lookOn, const ZC_Vec3<float>& _up,
            const ZC_Perspective& persp, const ZC_Ortho& ortho, bool useWindowSize, ZC_FrameBuffer frameBuffer = ZC_FB_Default);

    ZC_Camera(ZC_Camera&& c);

    ~ZC_Camera();

    //  Make camera active, next call GetActiveCamera() retun pointer on that camera.
    void MakeActive();
    //  Returns the camera that last called MakeActive(). Used to get the camera status.
    static ZC_Camera* GetActiveCamera();
    const ZC_Mat4<float>* GetPerspectiveView();

private:
    struct UboSet   //  structure for ubo sub data
    {
        ZC_Mat4<float> ortho;
        ZC_Mat4<float> perspView;
        ZC_Vec3<float> position;
    };

    static inline ZC_uptr<ZC_UBO> ubo;
    static inline const ZC_Camera* activeUBO = nullptr;      //  pointer on camera wich uboSet was activated last in ZC_Render or ZC_Renderer (GPU data)
    static inline ZC_Camera* activeCamera = nullptr;   //  pointer on active camera for getting camera information throught GetActiveCamera() (CPU data)

    UboSet uboSet;
    ZC_SConnection sConWindowResize;

    void UboUpdate();
    void WindowResize(float width, float height);
};