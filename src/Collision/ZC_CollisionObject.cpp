#include <ZC/Collision/ZC_CollisionObject.h>

#include <Collision/ZC_CollisionManager.h>
#include <ZC/Tools/Math/ZC_Math.h>
#include <ZC/Video/ZC_SWindow.h>

ZC_CollisionObject::ZC_CollisionObject(ZC_uptr<ZC_CO_FigureSphere>&& _figure, ZC_C0_Type _collision_type, void* _pHolder,
        ZC_Function<void(const ZC_CO_CollisionResult&)>&& _collision_callback, const ZC_Mat4<float>& mat_model)
    : upFigSphere(std::move(_figure)),
    collision_type(_collision_type),
    pHolder(_pHolder),
    collision_callback(std::move(_collision_callback)),
    mat_model_actual(mat_model)
{
    ZC_CollisionManager::AddFigure(this);
}

ZC_CollisionObject::~ZC_CollisionObject()
{
    ZC_CollisionManager::EraseFigure(this);
}

ZC_Mat4<float>* ZC_CollisionObject::GetModelMatrix()
{
    return &mat_model_actual;
}

void ZC_CollisionObject::UpdateModelMatrix(const ZC_Mat4<float>& mat)
{
    mat_model_previous = mat_model_actual;
    mat_model_actual = mat;
    upFigSphere->is_center_actual = false;
    upFigSphere->is_points_normals_actual = false;
    if (ZC_CollisionManager::IsCollisionInProcess())   //  collision in process need update data for correct collisions with rest objects
    {
        UpdateCenterWithModelMatrix();
        UpdatePointsAndNormalsWithModelMatrix();
    }
}

void ZC_CollisionObject::UpdateRadius(float radius)
{
    upFigSphere->radius = radius;
}

const ZC_CO_Surface<ZC_Vec3<float>*>* ZC_CollisionObject::GetClosestSurface(const ZC_Vec3<float>& point)
{
    return upFigSphere->GetClosesSurface(point);
}

// const ZC_Vec3<float>* ZC_CollisionObject::GetSourcePoint(const ZC_Vec3<float>* pPoint_fact) const
// {
//     return upFigSphere.GetSourcePoint(pPoint_fact);
// }

const ZC_CO_FigureSphere& ZC_CollisionObject::GetFigure() const
{
    return *upFigSphere;
}

bool ZC_CollisionObject::IsCurrentFrameCollision() const
{
    return ZC_SWindow::GetCurrentFrameNumber() == last_collision_frame_number;
}

void* ZC_CollisionObject::GetHolder()
{
    return pHolder;
}

void ZC_CollisionObject::UpdateCenterWithModelMatrix()
{
    upFigSphere->UpdateCenter(mat_model_actual);
}

void ZC_CollisionObject::UpdatePointsAndNormalsWithModelMatrix()
{
    upFigSphere->UpdatePointsAndNormals(mat_model_actual);
}

bool ZC_CollisionObject::MakeCollision(ZC_CollisionObject* pCO)
{
    ZC_CO_FigureSphere& figure_other = *(pCO->upFigSphere);
        //  check fugures distances
    float length_between_figures = ZC_Vec::Length(upFigSphere->center_fact - figure_other.center_fact);
    float radiuses_sum = upFigSphere->radius + figure_other.radius;
    if (radiuses_sum < length_between_figures) return false;     //  figures are to far apart

    if (!(upFigSphere->GetAllPointsFact()) || !(figure_other.GetAllPointsFact()))    //  one of the object have only radius fo collision, so collision happens only by radius (WRONG WAY)
    {
        float push_back_dist = radiuses_sum - length_between_figures;   //  if equal 0, need recalculate previous poses and make pushback from them (HAVE NO TIME TO DO...)
        last_collision_frame_number = ZC_SWindow::GetCurrentFrameNumber();
        pCO->last_collision_frame_number = last_collision_frame_number;
        if (collision_callback)
        {
            ZC_Vec3<float> dir_dist = upFigSphere->center_fact - pCO->upFigSphere->center_fact;
            if (dir_dist != ZC_Vec3<float>()) collision_callback(ZC_CO_CollisionResult{ .pObj = pCO, .pushback = (collision_type != ZC_COT__DynamicPushback ? ZC_Vec3<float>()
            :  ZC_Vec::MoveByLength({}, dir_dist, push_back_dist)) });
        }
        if (pCO->collision_callback) pCO->collision_callback(ZC_CO_CollisionResult{ .pObj = this, .pushback = (pCO->collision_type != ZC_COT__DynamicPushback ? ZC_Vec3<float>()
            : ZC_Vec::MoveByLength({}, pCO->upFigSphere->center_fact - upFigSphere->center_fact, push_back_dist)) });
        return true;
    }

        //  centers collision passed, update points and normals for collisoin if need for both objects (centers updates on start of collision in ZC_CollisionManager::MakeCollision())
    UpdatePointsAndNormalsWithModelMatrix();
    pCO->UpdatePointsAndNormalsWithModelMatrix();
        
        //  checks collision of the figures
    auto lamb_CallSimpleCollision = [this, pCO](const std::vector<ZC_Vec3<float>>* points, const std::vector<ZC_CO_Surface<ZC_Vec3<float>*>>* surfaces)
    {
        assert(points && surfaces);
        if (SimpleCollision(points, surfaces))   //  both object need simple collision
        {
            last_collision_frame_number = ZC_SWindow::GetCurrentFrameNumber();
            pCO->last_collision_frame_number = last_collision_frame_number;
            if (collision_callback) collision_callback(ZC_CO_CollisionResult{ .pObj = pCO, });
            if (pCO->collision_callback) collision_callback(ZC_CO_CollisionResult{ .pObj = this });
            return true;
        }
        return false;
    };

    return collision_type != ZC_COT__DynamicPushback && collision_type != ZC_COT__DynamicPushback ?   //  both object need simple collision
        upFigSphere->radius <= figure_other.radius ? lamb_CallSimpleCollision(upFigSphere->GetAllPointsFact(), figure_other.GetSurfacesFact())
            : lamb_CallSimpleCollision(figure_other.GetAllPointsFact(), upFigSphere->GetSurfacesFact())  //  simple collision
        : HardCollision(pCO);

    return false;
}

bool ZC_CollisionObject::SimpleCollision(const std::vector<ZC_Vec3<float>>* points, const std::vector<ZC_CO_Surface<ZC_Vec3<float>*>>* surfaces)
{
    for (const ZC_Vec3<float>& point : *points)
    {
        bool point_into_figure = true;
        for (const ZC_CO_Surface<ZC_Vec3<float>*>& surf : *surfaces)
        {
            if (ZC_DistanceFromPointToPlane(*(surf.points[0]), *(surf.normal), point) >= 0)
            {
                point_into_figure = false;
                break;
            }
        }
        if (point_into_figure) return true;
    }
    return false;
};

bool ZC_CollisionObject::HardCollision(ZC_CollisionObject* pCO)
{
    const ZC_CO_Surface<ZC_Vec3<float>*>* pSurf_collisioned = nullptr;
    float farthest_point_distance = 0.f;    //  ditance from pSurf_collisioned to farthest of penetration points 
    auto lamb_FoundSurfaceAndDistance = [&pSurf_collisioned, &farthest_point_distance, this](const std::vector<ZC_Vec3<float>>* points,
        const std::vector<ZC_CO_Surface<ZC_Vec3<float>*>>* surfaces)
    {
        std::list<const ZC_Vec3<float>*> points_inside = FindPointsInsideFigure(points, surfaces);     //  points inse other figure
        if (points_inside.empty()) return false;    //  no collision points

           //  find central point, summ of all points divide on count of points                                                              MAY BE ADD HERE PREV POS OF POINTS
        ZC_Vec3<float> central_point;
        for (const ZC_Vec3<float>* pPoint : points_inside) central_point += *pPoint;                                                               
        central_point /= points_inside.size();
        // for (ZC_Vec3<float>* pPoint : points)
        //     prev_points.emplace_back(mat_model_previous * ZC_Vec4<float>(*(upFigSphere.GetSourcePoint(pPoint)), 1.f));   //  get src_point of the point, multiply with previous model matrix,
        
            //  find closest surface to the central point (pSurf_collisioned)
        float dist_to_surf = ULONG_MAX;
        for (const ZC_CO_Surface<ZC_Vec3<float>*>& surf : *surfaces)
        {
            float dist = std::abs(ZC_DistanceFromPointToPlane(*(surf.points[0]), *(surf.normal), central_point));
            if (dist < dist_to_surf)
            {
                pSurf_collisioned = &surf;
                dist_to_surf = dist;
            }
        }

        assert(pSurf_collisioned != nullptr);

            //  find distance to farthest point from found surface (farthest_point_distance)
        for (const ZC_Vec3<float>* pPoint : points_inside)
        {
            float dist = std::abs(ZC_DistanceFromPointToPlane(*(pSurf_collisioned->points[0]), *(pSurf_collisioned->normal), *pPoint));
            if (dist > farthest_point_distance) farthest_point_distance = dist;
        }
        return true;
    };

    ZC_CO_FigureSphere& figure_other = *(pCO->upFigSphere);
    if (upFigSphere->radius <= pCO->upFigSphere->radius)    //  our radius smaller, take our points for seach into pCO.upFigSphere
    {
        if (!lamb_FoundSurfaceAndDistance(upFigSphere->GetAllPointsFact(), figure_other.GetSurfacesFact())) return false;    //  no collision
        if (collision_callback)
            collision_callback(ZC_CO_CollisionResult{ .pObj = pCO,
                .pushback = collision_type == ZC_COT__DynamicPushback ? ZC_Vec::MoveByLength({ 0.f, 0.f, 0.f }, *(pSurf_collisioned->normal), farthest_point_distance)
                    : ZC_Vec3<float>(),  //  calculate pushback only if need
                .pSurf = pSurf_collisioned, .is_your_surface = false });
        if (pCO->collision_callback)
            pCO->collision_callback(ZC_CO_CollisionResult{ .pObj = this,                               //  normal from our surface, need inverce for calculation
                .pushback = collision_type == ZC_COT__DynamicPushback ? ZC_Vec::MoveByLength({ 0.f, 0.f, 0.f }, *(pSurf_collisioned->normal) * -1.f, farthest_point_distance)
                    : ZC_Vec3<float>(),
                .pSurf = pSurf_collisioned, .is_your_surface = true });
    }
    else    //  pCO radius smaller take their points for search into our figure
    {
        if (!lamb_FoundSurfaceAndDistance(figure_other.GetAllPointsFact(), upFigSphere->GetSurfacesFact())) return false;
        if (collision_callback)
            collision_callback(ZC_CO_CollisionResult{ .pObj = pCO,                                     //  normal from our surface, need inverce for calculation
                .pushback = collision_type == ZC_COT__DynamicPushback ? ZC_Vec::MoveByLength({ 0.f, 0.f, 0.f }, *(pSurf_collisioned->normal) * -1.f, farthest_point_distance)
                    : ZC_Vec3<float>(),  //  calculate pushback only if need
                .pSurf = pSurf_collisioned, .is_your_surface = true });
        if (pCO->collision_callback)
            pCO->collision_callback(ZC_CO_CollisionResult{ .pObj = this,
                .pushback = collision_type == ZC_COT__DynamicPushback ? ZC_Vec::MoveByLength({ 0.f, 0.f, 0.f }, *(pSurf_collisioned->normal), farthest_point_distance)
                    : ZC_Vec3<float>(),
                .pSurf = pSurf_collisioned, .is_your_surface = false });
    }
    return true;
}

std::list<const ZC_Vec3<float>*> ZC_CollisionObject::FindPointsInsideFigure(const std::vector<ZC_Vec3<float>>* points, const std::vector<ZC_CO_Surface<ZC_Vec3<float>*>>* surfaces)
{
    std::list<const ZC_Vec3<float>*> points_inside;    //  points inside other figure

    for (const ZC_Vec3<float>& point : *points)
    {
        bool point_into_figure = true;
        for (const ZC_CO_Surface<ZC_Vec3<float>*>& surf : *surfaces)
        {
            if (ZC_DistanceFromPointToPlane(*(surf.points[0]), *(surf.normal), point) >= 0)
            {
                point_into_figure = false;
                break;
            }
        }
        if (point_into_figure)
        {
            points_inside.emplace_back(&point);
        }
    }
    return points_inside;
}