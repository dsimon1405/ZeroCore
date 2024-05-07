#pragma once

#include <ZC/Tools/Math/ZC_Mat4.h>

class ZC_Perspective
{
public:
    ZC_Perspective(float _fovy, float _aspect, float _nearPlane, float _farPlane);
    virtual ~ZC_Perspective() = default;

    void SetFovy(float _fovy);
    void SetAspect(float _aspect);
    void SetNearPlane(float _nearPlane);
    void SetFarPlane(float _farPlane);
    float GetFovy() const noexcept;
    float GetAspect() const noexcept;
    float GetNearPlane() const noexcept;
    float GetFarPlane() const noexcept;
    const ZC_Mat4<float>* GetPerspective();

protected:
    ZC_Mat4<float> perspective;
    
    //  returns true if where updated.
    bool PerspectiveUpdate();

private:
    float fovy,
        aspect,
        nearPlane,
        farPlane;
    bool perspNeedUpdate = true,
        isPerspectiveActual = false;

    void ActualizePerspective();
};
