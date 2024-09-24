#include <ZC/Collision/ZC_CollisionObject.h>

#include <Collision/ZC_CollisionManager.h>
#include <ZC/Tools/Math/ZC_Math.h>
#include <ZC/Video/ZC_SWindow.h>

ZC_CollisionObject::ZC_CollisionObject(ZC_CO_Figure&& _figure, ZC_C0_Type _collision_type, void* _pHolder, ZC_Function<void(const ZC_CO_CollisionResult&)>&& _collision_callback, const ZC_Mat4<float>& mat_model)
    : figure(std::move(_figure)),
    collision_type(_collision_type),
    collision_callback(std::move(_collision_callback)),
    pHolder(_pHolder),
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
    mat_model_was_updated = true;
    if (ZC_CollisionManager::IsCollisionInProcess()) UpdateDataWithModelMatrix();   //  collision in process need update data for correct collisions with rest objects
}

const ZC_CO_Surface<ZC_Vec3<float>*>* ZC_CollisionObject::GetClosestSurface(const ZC_Vec3<float>& point)
{
    return figure.GetClosesSurface(point);
}

const ZC_Vec3<float>* ZC_CollisionObject::GetSourcePoint(const ZC_Vec3<float>* pPoint_fact) const
{
    return figure.GetSourcePoint(pPoint_fact);
}

const ZC_CO_Figure& ZC_CollisionObject::GetFigure() const
{
    return figure;
}

bool ZC_CollisionObject::IsCurrentFrameCollision() const
{
    return ZC_SWindow::GetCurrentFrameNumber() == last_collision_frame_number;
}

void* ZC_CollisionObject::GetHolder()
{
    return pHolder;
}

void ZC_CollisionObject::UpdateDataWithModelMatrix()
{
    if (!mat_model_was_updated) return;
    figure.UpdateWithModelMatrix(mat_model_actual);
    mat_model_was_updated = false;
}

bool ZC_CollisionObject::MakeCollision(ZC_CollisionObject* pCO)
{
    ZC_CO_Figure& figure_other = pCO->figure;
        //  check fugures distances
    float length_between_figures = ZC_Vec::Length(figure.center_fact - figure_other.center_fact);
    if (figure.radius + figure_other.radius < length_between_figures) return false;     //  figures are to far apart
        
        //  checks collision of the figures
    auto lamb_CallSimpleCollision = [this, pCO](std::vector<ZC_Vec3<float>>& points, std::vector<ZC_CO_Surface<ZC_Vec3<float>*>>& surfaces)
    {
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
        figure.radius <= figure_other.radius ? lamb_CallSimpleCollision(figure.all_points_fact, figure_other.surfaces_fact) : lamb_CallSimpleCollision(figure_other.all_points_fact, figure.surfaces_fact)  //  simple collision
        : HardCollision(pCO);

    return false;
}

bool ZC_CollisionObject::SimpleCollision(std::vector<ZC_Vec3<float>>& points, std::vector<ZC_CO_Surface<ZC_Vec3<float>*>>& surfaces)
{
    for (const ZC_Vec3<float>& point : points)
    {
        bool point_into_figure = true;
        for (const ZC_CO_Surface<ZC_Vec3<float>*>& surf : surfaces)
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
    ZC_CO_Surface<ZC_Vec3<float>*>* pSurf_collisioned = nullptr;
    float farthest_point_distance = 0.f;    //  ditance from pSurf_collisioned to farthest of penetration points 
    auto lamb_FoundSurfaceAndDistance = [&pSurf_collisioned, &farthest_point_distance, this](std::vector<ZC_Vec3<float>>& points, std::vector<ZC_CO_Surface<ZC_Vec3<float>*>>& surfaces)
    {
        std::list<ZC_Vec3<float>*> points_inside = FindPointsInsideFigure(points, surfaces);     //  points inse other figure
        if (points_inside.empty()) return false;    //  no collision points

           //  find central point, summ of all points divide on count of points                                                              MAY BE ADD HERE PREV POS OF POINTS
        ZC_Vec3<float> central_point;
        for (ZC_Vec3<float>* pPoint : points_inside) central_point += *pPoint;                                                               
        central_point /= points_inside.size();
        // for (ZC_Vec3<float>* pPoint : points)
        //     prev_points.emplace_back(mat_model_previous * ZC_Vec4<float>(*(figure.GetSourcePoint(pPoint)), 1.f));   //  get src_point of the point, multiply with previous model matrix,
        
            //  find closest surface to the central point (pSurf_collisioned)
        float dist_to_surf = ULONG_MAX;
        for (ZC_CO_Surface<ZC_Vec3<float>*>& surf : surfaces)
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
        for (ZC_Vec3<float>* pPoint : points_inside)
        {
            float dist = std::abs(ZC_DistanceFromPointToPlane(*(pSurf_collisioned->points[0]), *(pSurf_collisioned->normal), *pPoint));
            if (dist > farthest_point_distance) farthest_point_distance = dist;
        }
        return true;
    };

    ZC_CO_Figure& figure_other = pCO->figure;
    if (figure.radius <= pCO->figure.radius)    //  our radius smaller, take our points for seach into pCO.figure
    {
        if (!lamb_FoundSurfaceAndDistance(figure.all_points_fact, figure_other.surfaces_fact)) return false;    //  no collision
        if (collision_callback)
            collision_callback(ZC_CO_CollisionResult{ .pObj = pCO,
                .pushback = collision_type == ZC_COT__DynamicPushback ? ZC_Vec::MoveByLength({ 0.f, 0.f, 0.f }, *(pSurf_collisioned->normal), farthest_point_distance) : ZC_Vec3<float>(),  //  calculate pushback only if need
                .pSurf = pSurf_collisioned, .is_your_surface = false });
        if (pCO->collision_callback)
            pCO->collision_callback(ZC_CO_CollisionResult{ .pObj = this,                               //  normal from our surface, need inverce for calculation
                .pushback = collision_type == ZC_COT__DynamicPushback ? ZC_Vec::MoveByLength({ 0.f, 0.f, 0.f }, *(pSurf_collisioned->normal) * -1.f, farthest_point_distance) : ZC_Vec3<float>(),
                .pSurf = pSurf_collisioned, .is_your_surface = true });
    }
    else    //  pCO radius smaller take their points for search into our figure
    {
        if (!lamb_FoundSurfaceAndDistance(figure_other.all_points_fact, figure.surfaces_fact)) return false;
        if (collision_callback)
            collision_callback(ZC_CO_CollisionResult{ .pObj = pCO,                                     //  normal from our surface, need inverce for calculation
                .pushback = collision_type == ZC_COT__DynamicPushback ? ZC_Vec::MoveByLength({ 0.f, 0.f, 0.f }, *(pSurf_collisioned->normal) * -1.f, farthest_point_distance) : ZC_Vec3<float>(),  //  calculate pushback only if need
                .pSurf = pSurf_collisioned, .is_your_surface = true });
        if (pCO->collision_callback)
            pCO->collision_callback(ZC_CO_CollisionResult{ .pObj = this,
                .pushback = collision_type == ZC_COT__DynamicPushback ? ZC_Vec::MoveByLength({ 0.f, 0.f, 0.f }, *(pSurf_collisioned->normal), farthest_point_distance) : ZC_Vec3<float>(),
                .pSurf = pSurf_collisioned, .is_your_surface = false });
    }
    return true;
}

std::list<ZC_Vec3<float>*> ZC_CollisionObject::FindPointsInsideFigure(std::vector<ZC_Vec3<float>>& points, std::vector<ZC_CO_Surface<ZC_Vec3<float>*>>& surfaces)
{
    std::list<ZC_Vec3<float>*> points_inside;    //  points inside other figure

    for (ZC_Vec3<float>& point : points)
    {
        bool point_into_figure = true;
        for (const ZC_CO_Surface<ZC_Vec3<float>*>& surf : surfaces)
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