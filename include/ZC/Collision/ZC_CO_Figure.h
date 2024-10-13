#pragma once

#include "ZC_CO_FigureSphere.h"

struct ZC_CO_Figure : public ZC_CO_FigureSphere
{
    // bool is_center_actual = false;
    // bool is_points_normals_actual = false;

    // float radius;   //  radius from figure's center to farest point of figure (include variant when figure points move in animation)

    // ZC_Vec3<float> center_src;  //  center of a figure
    std::vector<ZC_Vec3<float>> all_points_src;     //  all points of the figure
    std::vector<ZC_Vec3<float>> all_normals_scr;    //  all normals

    // ZC_Vec3<float> center_fact;  //  center updated with model matrix
    std::vector<ZC_Vec3<float>> all_points_fact;     //  all points updated with model matrix
    std::vector<ZC_Vec3<float>> all_normals_fact;    //  all normals updated with model matrix
    std::vector<ZC_CO_Surface<ZC_Vec3<float>*>> surfaces_fact;  //  surfaces store pinters to all_points_fact and normals_fact

    /*
    Data for collision. for example cube with center and 6 surfaces.

    Params:
    - _center - center of a figure.
    - _radius - radius from figure's center to farest point of figure (include variant when figure points move in animation).
    - _surfaces - surfaces of a figure.
    */
    ZC_CO_Figure(const ZC_Vec3<float>& _center, float _radius, const std::vector<ZC_CO_Surface<ZC_Vec3<float>>>& _surfaces);

        //  multiply vec3 with model matrix
    // static ZC_Vec3<float> MultiplyWithModel(const ZC_Mat4<float>& model, const ZC_Vec3<float>& src);
    
        //  take src data of center, multiply with mat_model and store to fact
    // void UpdateCenter(const ZC_Mat4<float>& mat_model);
        //  take src data of points and normals, multiply with mat_model and store to fact
    void UpdatePointsAndNormals(const ZC_Mat4<float>& mat_model);

        //  return closest surface to a point
    const ZC_CO_Surface<ZC_Vec3<float>*>* GetClosesSurface(const ZC_Vec3<float>& point) override;
        //  return pointer on src point of fact point, if can't find nullptr
    // const ZC_Vec3<float>* GetSourcePoint(const ZC_Vec3<float>* pPoint_fact) const;
    const std::vector<ZC_Vec3<float>>* GetAllPointsFact() const;
    const std::vector<ZC_Vec3<float>>* GetAllNormalsFact() const;
    const std::vector<ZC_CO_Surface<ZC_Vec3<float>*>>* GetSurfacesFact() const;
};