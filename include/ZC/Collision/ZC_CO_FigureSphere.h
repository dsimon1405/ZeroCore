#pragma once

#include "ZC_CO_Surface.h"
#include <ZC/Tools/Math/ZC_Mat4.h>

struct ZC_CO_FigureSphere
{
    bool is_center_actual = false;
    bool is_points_normals_actual = false;
    float radius;   //  radius from figure's center to farest point of figure (include variant when figure points move in animation)
    ZC_Vec3<float> center_src;  //  center of a figure
    ZC_Vec3<float> center_fact;  //  center updated with model matrix

    ZC_CO_FigureSphere(const ZC_Vec3<float>& _center, float _radius);
    
    virtual ~ZC_CO_FigureSphere() = default;

    static ZC_Vec3<float> MultiplyWithModel(const ZC_Mat4<float>& model, const ZC_Vec3<float>& src);
    
        //  take src data of center, multiply with mat_model and store to fact
    virtual void UpdateCenter(const ZC_Mat4<float>& mat_model);
        //  take src data of points and normals, multiply with mat_model and store to fact
    virtual void UpdatePointsAndNormals(const ZC_Mat4<float>& mat_model) {}
    virtual const ZC_CO_Surface<ZC_Vec3<float>*>* GetClosesSurface(const ZC_Vec3<float>& point) { return nullptr; }
    virtual const std::vector<ZC_Vec3<float>>* GetAllPointsFact() const { return nullptr; }
    virtual const std::vector<ZC_Vec3<float>>* GetAllNormalsFact() const { return nullptr; }
    virtual const std::vector<ZC_CO_Surface<ZC_Vec3<float>*>>* GetSurfacesFact() const { return nullptr; }
};