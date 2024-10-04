#pragma once

#include "ZC_CO_Figure.h"
#include <ZC/Tools/Function/ZC_Function.h>
#include <ZC/Tools/Math/ZC_Mat4.h>

#include <list>

enum ZC_C0_Type
{
    ZC_COT__Dynamic,             //  object dynamic (movable)
    ZC_COT__DynamicPushback,     //  object dynamic (movable) on collision will be calculated distance-vector to move out of other object
    ZC_COT__Static,              //  static object (don't movable)
    ZC_COT__StaticSoloCollision  //  dynamic object coud make collision only with one of those static objects
};

struct ZC_CollisionObject;

struct ZC_CO_CollisionResult
{
    ZC_CollisionObject* pObj = nullptr;                 //  object with wich were collision
    ZC_Vec3<float> pushback;                            //  distance-vector to move out of other object (calculates only for ZC_COT__DynamicPushback flag)
    ZC_CO_Surface<ZC_Vec3<float>*>* pSurf = nullptr;    //  pointer on collisioned surface (calculates exacly if ZC_COT__DynamicPushback flag)
    bool is_your_surface;                               //  If pSurf from your object - true, otherwise false. For collision takes points from object with smaller radius and they collisions with other object surfaces.
};

class ZC_CollisionManager;
struct ZC_CO_Figure;

struct ZC_CollisionObject
{
    friend class ZC_CollisionManager;
    friend struct ZC_CO_Figure;

    //  may be added set (vector) of ZC_CF_CollisionData(s) (eacch must be have own model matrics) on cases of scelet animated characters to complement rude world collisions with humanable collisions like attacs (those must be updated only if for example bullet in radius of our object)

    /*
    Params:
    - _figure - store collision model.
    - _collision_type - look ZC_C0_Type.
    - pHolder - pointer on object (holder) wich will be return on call GetHolder()
    - _func_callision - callback calls on collision, if don't need may be missed. Params: first - pointer on object with wich were happened collision.
                                                                                          second - pointer on surface on wich where happend collision.
    - mat_model - model matrix. For static ZC_COT__Static and ZC_COT__StaticSoloCollision object actual model matrix must be seted in ctr or with UpdateModelMatrix() before call ZC_SWindow::RunMainCycle()!
    */
    ZC_CollisionObject(ZC_CO_Figure&& _figure, ZC_C0_Type _collision_type, void* _pHolder, ZC_Function<void(const ZC_CO_CollisionResult&)>&& _func_callision, const ZC_Mat4<float>& mat_model = ( 1.f ));

    ~ZC_CollisionObject();
    
        //  don't change matrix! Use UpdateModelMatrix() for changing
    ZC_Mat4<float>* GetModelMatrix();
        //  update model matrix and data dependent on it
    void UpdateModelMatrix(const ZC_Mat4<float>& mat);
        //  update radius
    void UpdateRadius(float radius);
        //  return closest surface to a point
    const ZC_CO_Surface<ZC_Vec3<float>*>* GetClosestSurface(const ZC_Vec3<float>& point);
        //  return pointer on src point of fact point, if can't find nullptr
    const ZC_Vec3<float>* GetSourcePoint(const ZC_Vec3<float>* pPoint_fact) const;
        //  return const reference on figure (collision model)
    const ZC_CO_Figure& GetFigure() const;
        //  Return true if object has collision with other object in current frame
    bool IsCurrentFrameCollision() const;
        //  return pointer on object's holder
    void* GetHolder();

private:
    ZC_CO_Figure figure;  //  sets in ctr
    ZC_C0_Type collision_type;
    void* pHolder;  //  holder of the object
    ZC_Function<void(const ZC_CO_CollisionResult&)> collision_callback;
    ZC_Mat4<float> mat_model_actual;   //  model matrix of global position stores in that class and all changes makes throught access methods
    ZC_Mat4<float> mat_model_previous;   //  model matrix fom previous move
    bool mat_model_was_updated = true;
    unsigned long long last_collision_frame_number;     //  number of the frame in wich was last collision

        //  updates fact data with model matrix
    void UpdateCenterWithModelMatrix();
    void UpdatePointsAndNormalsWithModelMatrix();
        //  make collision with other object and call collision_callback for both objects
    bool MakeCollision(ZC_CollisionObject* pCO);
    bool SimpleCollision(std::vector<ZC_Vec3<float>>& points, std::vector<ZC_CO_Surface<ZC_Vec3<float>*>>& surfaces);
    bool HardCollision(ZC_CollisionObject* pCO);
    std::list<ZC_Vec3<float>*> FindPointsInsideFigure(std::vector<ZC_Vec3<float>>& points, std::vector<ZC_CO_Surface<ZC_Vec3<float>*>>& surfaces);
};