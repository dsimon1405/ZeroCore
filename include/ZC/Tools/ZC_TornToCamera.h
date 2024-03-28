#pragma once

#include <ZC/Tools/Signal/ZC_SConnection.h>
#include <ZC/Tools/Math/ZC_Mat4.h>

#include <forward_list>

/*
Parent for classes that need to constantly rotate the texture to face the camera, every where into the scene.
Conditions:
- textures origin must be in position vec3(0,0,0).
- texture must ley on x,z axises face to -y.
*/
class ZC_TurnToCamera
{
public:
    virtual ~ZC_TurnToCamera();

    //  Changes position.
    void SetPosition(const ZC_Vec3<float>& pos);
    //  Changes scale. Default scaling creates letters with height near 2.f.
    void SetScale(float _scale);

protected:
    ZC_Mat4<float> model;
    ZC_Vec3<float> position;
    bool isNeedUpdate = true;
    float scale = 0.f;

    ZC_TurnToCamera(const ZC_Vec3<float>& _position, float _scale);

    ZC_TurnToCamera(const ZC_TurnToCamera& ttc);

private:
    static inline ZC_SConnection sconEventsEnd; //  must be connected to event, which calls after all camera manipulations  !!!!!!!!!!!!
    static inline std::forward_list<ZC_TurnToCamera*> allHeirs;
    static inline std::forward_list<ZC_TurnToCamera*> needUpdate;

    static void Update(float time);
    void CalculateModel(const ZC_Vec3<float>& camPos, bool isNormalVerticalOrientation);
    static void Rotate(ZC_Mat4<float>& model, float cos, float sin, const ZC_Vec3<float>& axise);
};