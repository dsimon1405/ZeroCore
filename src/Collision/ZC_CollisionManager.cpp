#include "ZC_CollisionManager.h"


ZC_CollisionManager::ZC_CollisionManager()
{
    pCM = this;
}

void ZC_CollisionManager::AddFigure(ZC_CollisionObject* pFig)
{
    switch (pFig->collision_type)
    {
    case ZC_COT__Dynamic: pCM->dynamic_figures.emplace_back(pFig); break;
    case ZC_COT__DynamicPushback: pCM->dynamic_figures.emplace_back(pFig); break;
    case ZC_COT__Static: pCM->static_figures.emplace_back(pFig); break;
    case ZC_COT__StaticSoloCollision: pCM->static_solo_figures.emplace_back(pFig); break;
    }
}

void ZC_CollisionManager::EraseFigure(ZC_CollisionObject* pFig)
{
    switch (pFig->collision_type)
    {
    case ZC_COT__Dynamic: std::erase(pCM->dynamic_figures, pFig); break;
    case ZC_COT__DynamicPushback: std::erase(pCM->dynamic_figures, pFig); break;
    case ZC_COT__Static: std::erase(pCM->static_figures, pFig); break;
    case ZC_COT__StaticSoloCollision: std::erase(pCM->static_solo_figures, pFig); break;
    }
}

bool ZC_CollisionManager::IsCollisionInProcess()
{
    return pCM->collision_in_process;
}

void ZC_CollisionManager::Configure()
{
    for (ZC_CollisionObject* pCF : static_figures) pCF->UpdateDataWithModelMatrix();
    for (ZC_CollisionObject* pCF : static_solo_figures) pCF->UpdateDataWithModelMatrix();
    for (ZC_CollisionObject* pCF : dynamic_figures) pCF->UpdateDataWithModelMatrix();
}

void ZC_CollisionManager::MakeCollision()
{
    collision_in_process = true;

    for (ZC_CollisionObject* pCF : dynamic_figures) pCF->UpdateDataWithModelMatrix();  //  update position and normal need only for dynamic objects

        //  make collision each dynamic object with all others
    for (auto cur_iter = dynamic_figures.begin(); cur_iter != dynamic_figures.end(); ++cur_iter)
    {
        auto iter = cur_iter;
        for (++iter; iter != dynamic_figures.end(); ++iter)
            (*cur_iter)->MakeCollision(*iter);
    }
        //  collision with static objects
    for (ZC_CollisionObject* pCF_dynaic : dynamic_figures)
    {
        for (ZC_CollisionObject* pCF_static : static_figures)
            pCF_dynaic->MakeCollision(pCF_static);
    }
        //   collision with solo stati cobjects
    for (ZC_CollisionObject* pCF_dynaic : dynamic_figures)
    {
        for (ZC_CollisionObject* pCF_solo_static : static_solo_figures)
            if (pCF_dynaic->MakeCollision(pCF_solo_static)) break;    //  had collision with solo static object may finish
    }

    collision_in_process = false;
}