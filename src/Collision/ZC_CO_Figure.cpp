#include <ZC/Collision/ZC_CO_Figure.h>

#include <ZC/Tools/Container/ZC_ContFunc.h>
#include <ZC/Tools/Math/ZC_Math.h>
#include <ZC/Collision/ZC_CollisionObject.h>

#include <list>

ZC_CO_Figure::ZC_CO_Figure(ZC_Vec3<float> _center, float _radius, std::vector<ZC_CO_Surface<ZC_Vec3<float>>> _surfaces)
    : radius(_radius),
    center_src(_center)
{
    all_normals_scr.reserve(_surfaces.size());  //  each durface have only one normal, normals of collision figure can't be dublicated!

    std::list<ZC_Vec3<float>> list_points;    //  will contain non duplicate points (don't know how much non duplicate points in figure, so to avoid lost memory in vector, make list at first)
        //  fill list_points and all_normals_scr
    for (ZC_CO_Surface<ZC_Vec3<float>>& surf : _surfaces)
    {
        all_normals_scr.emplace_back(surf.normal);
        for (ZC_Vec3<float>& point : surf.points)
        {
            if (!ZC_Find(list_points, point)) list_points.emplace_back(point);    //  if point not in list yet add it
        }
    }
    
        //  fill all_points_src, copying from list_points
    all_points_src.reserve(list_points.size());
    for (ZC_Vec3<float>& point : list_points)
        all_points_src.emplace_back(point);
        //  copy from src to fact
    all_normals_fact = all_normals_scr;
    all_points_fact = all_points_src;

        //  fill surfaces with pointer from all_points_fact and all_normals_fact
    surfaces_fact.reserve(_surfaces.size());
    size_t normal_i = 0;
    for (ZC_CO_Surface<ZC_Vec3<float>>& surf : _surfaces)
    {
        std::vector<ZC_Vec3<float>*> points;    //  pointers on points from all_points
        points.reserve(surf.points.size());
        for (ZC_Vec3<float>& point : surf.points)
            points.emplace_back(ZC_Find(all_points_fact, point));
        
        surfaces_fact.emplace_back(ZC_CO_Surface<ZC_Vec3<float>*>(std::move(points), &(all_normals_fact[normal_i++])));    //  normals in surfaces_fact have same order with _surfaces and all_normals_fact, so normal can be taken with index
    }
}

void ZC_CO_Figure::UpdateWithModelMatrix(const ZC_Mat4<float>& mat_model)
{
    auto lamb_updateVec3 = [](const ZC_Mat4<float>& model, const ZC_Vec3<float>& src)
    {
        ZC_Vec4<float> center_4f = model * ZC_Vec4<float>(src, 1.f);
        return ZC_Vec3<float>(center_4f[0], center_4f[1], center_4f[2]);
    };
        //  update center_fact
    center_fact = lamb_updateVec3(mat_model, center_src);
    
    if (surfaces_fact.empty()) return;    //  collision object is sphere, no planes

        //  update all_points_fact
    for (size_t i = 0; i < all_points_fact.size(); i++)
        all_points_fact[i] = lamb_updateVec3(mat_model, all_points_src[i]);
    
    if (mat_model[0][0] == 1.f && mat_model[0][1] == 0.f && mat_model[0][2] == 0.f
        && mat_model[1][0] == 0.f && mat_model[1][1] == 1.f && mat_model[1][2] == 0.f
        && mat_model[2][1] == 0.f && mat_model[2][1] == 0.f && mat_model[2][2] == 1.f) return;      //  scale and rotate dont uses

        //  for normals need matrix withought translation, make translation = 0
    ZC_Mat4<float> mrs = mat_model;     // matrix rotate srcale
    mrs[3][0] = 0.f;
    mrs[3][1] = 0.f;
    mrs[3][2] = 0.f;
        //  update all_normals_fact
    for (size_t i = 0; i < all_normals_fact.size(); i++)
        all_normals_fact[i] = ZC_Vec::Normalize(lamb_updateVec3(mrs, all_normals_scr[i])); //  need normalize on case of scaling
}

const ZC_CO_Surface<ZC_Vec3<float>*>* ZC_CO_Figure::GetClosesSurface(const ZC_Vec3<float>& point)
{
    ZC_CO_Surface<ZC_Vec3<float>*>* pSurf_closest = nullptr;
    float closest_distance = ULLONG_MAX;
    for (ZC_CO_Surface<ZC_Vec3<float>*>& surf : surfaces_fact)
    {
        float distance = std::abs(ZC_DistanceFromPointToPlane(*(surf.points[0]), *(surf.normal), point));
        if (distance < closest_distance)
        {
            closest_distance = distance;
            pSurf_closest = &surf;
        }
    }
    return pSurf_closest;
}

const ZC_Vec3<float>* ZC_CO_Figure::GetSourcePoint(const ZC_Vec3<float>* pPoint_fact) const
{
    size_t index = pPoint_fact - all_points_fact.data();
    return index < all_points_src.size() ? &all_points_src[index] : nullptr;
}