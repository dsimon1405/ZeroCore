#pragma once

#include <ZC/Collision/ZC_CollisionObject.h>

#include <list>

class ZC_CollisionManager
{
public:
    ZC_CollisionManager();

    static void AddFigure(ZC_CollisionObject* pFig);
    static void EraseFigure(ZC_CollisionObject* pFig);
        //  return true if collision in process
    static bool IsCollisionInProcess();
        //  make collision dynamic objects with static objects and with other dynamic objects
    void MakeCollision();

private:
    static inline ZC_CollisionManager* pCM;

    std::list<ZC_CollisionObject*> dynamic_figures; //  movable
    std::list<ZC_CollisionObject*> static_figures;  //  not movable
    std::list<ZC_CollisionObject*> static_solo_figures; //  dynamic figures may have collision only with one of those not movable object at a time (for exaple parts of the earth)
    bool collision_in_process;
};