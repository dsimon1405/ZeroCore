#pragma once

#include <ZC/Events/ZC_EventConnection.h>
#include <ZC/Tools/Math/ZC_Mat4.h>

#include <forward_list>

/*
Parent for classes that need to constantly rotate (calculates model matrix) the texture to face the camera, every where into the scene.
Conditions:
- textures origin must be in position vec3(0,0,0).
- texture must ley on x,z axises face to -y.
*/
class ZC_TornToCamera
{
public:
    virtual ~ZC_TornToCamera();

    //  Changes position.
    void SetPosition(const ZC_Vec3<float>& pos);
    //  Changes scale. Default scaling creates letters with height near 2.f.
    void SetScale(float _scale);

protected:
    ZC_Mat4<float> model;   //  model matrix for heir
    ZC_Vec3<float> position;
    bool isNeedUpdate = true;
    float scale = 0.f;

    ZC_TornToCamera(const ZC_Vec3<float>& _position, float _scale);

    ZC_TornToCamera(const ZC_TornToCamera& ttc);

private:
    static inline ZC_EC ecEventsEnd; //  must be connected to event, which calls after all camera manipulations  !!!!!!!!!!!!
    static inline std::forward_list<ZC_TornToCamera*> allHeirs;
    static inline std::forward_list<ZC_TornToCamera*> needUpdate;

    static void Update(float time);
    void CalculateModel(const ZC_Vec3<float>& camPos, bool isNormalVerticalOrientation);
    static void Rotate(ZC_Mat4<float>& model, float cos, float sin, const ZC_Vec3<float>& axise);
};