#pragma once

#include <ZC/Tools/Math/ZC_Mat4.h>
#include <ZC/Tools/Signal/ZC_Signal.h>

class ZC_View
{
public:
    ZC_View(ZC_Vec3<float>* _camPos, const ZC_Vec3<float>& _lookOn, const ZC_Vec3<float>& _up);
    
    ZC_View(ZC_View&& v);

    virtual ~ZC_View() = default;

    void SetPosition(const ZC_Vec3<float>& _camPos);
    void SetLookOn(const ZC_Vec3<float>& _lookOn);
    void SetUp(const ZC_Vec3<float>& _up);
    ZC_Vec3<float> GetPosition() const noexcept;
    ZC_Vec3<float> GetLookOn() const noexcept;
    ZC_Vec3<float> GetUp() const noexcept;
    ZC_SConnection ConnectChangeCameraPosition(ZC_Function<void(const ZC_Vec3<float>&)>&& func);
    const ZC_Mat4<float>* GetView();

protected:
    ZC_Mat4<float> view;

    //  returns true if where updated.
    bool ViewUpdate();

private:
    ZC_Vec3<float>* position;
    ZC_Vec3<float> lookOn,
                up;
    bool viewNeedUpdate = true,
        isViewActual = false;
    ZC_Signal<void(const ZC_Vec3<float>&)> sigChangeCamPos {false};
  
    void ActualizeViewMatrix();
};