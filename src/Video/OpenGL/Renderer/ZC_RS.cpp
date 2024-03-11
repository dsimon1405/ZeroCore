#include <ZC/Video/OpenGL/Renderer/ZC_RS.h>

#include <ZC/Tools/Container/ZC_ContFunc.h>
#include "ZC_Renderer.h"
#include <ZC/ErrorLogger/ZC_ErrorLogger.h>

#include <algorithm>

ZC_RS::ZC_RS(typename ZC_ShProgs::ShPInitSet* pShPInitSet, ZC_VAO&& _vao, ZC_uptr<ZC_GLDraw>&& _upDraw,
        std::forward_list<ZC_Buffer>&& _buffers)
    : pShP(&pShPInitSet->shProg),
    pBaseUniforms(&pShPInitSet->uniforms),
    vao(std::move(_vao)),
    upGLDraw(std::move(_upDraw)),
    buffers(std::move(_buffers))
{
    static bool isFirstCall = true;
    if (isFirstCall)
    {
        auto pShPog = ZC_ShProgs::Get(ZC_ShProgs::Name::ZCR_Stencil);
        if (pShPog) LevelController::LevelStencil::pActiveUniformsStencil = &(pShPog->uniforms);
        isFirstCall = false;
    }
}

ZC_RS::ZC_RS(ZC_RS&& rs)
    : ZC_RendererSet(dynamic_cast<ZC_RendererSet&&>(rs)),
    pShP(rs.pShP),
    pBaseUniforms(std::move(rs.pBaseUniforms)),
    vao(std::move(rs.vao)),
    upGLDraw(std::move(rs.upGLDraw)),
    buffers(std::move(rs.buffers))
{}


//  LevelDrawing

ZC_RS::LevelController::LevelDrawing::LevelDrawing(Level _lvl)
    : lvl(_lvl)
{}

bool ZC_RS::LevelController::LevelDrawing::operator == (Level _lvl)
{
    return lvl == _lvl;
}

void ZC_RS::LevelController::LevelDrawing::Add(DrawingSet* pDrSet)
{
    drawingSets.emplace_front(pDrSet);
}

bool ZC_RS::LevelController::LevelDrawing::Erase(DrawingSet* pDrSet)
{
    if (!ZC_ForwardListErase(drawingSets, pDrSet)) ZC_ErrorLogger::Err("Can't find DrawingSet in drawingSets!", __FILE__, __LINE__);
    return drawingSets.empty();
}

void ZC_RS::LevelController::LevelDrawing::Draw(ZC_uptr<ZC_GLDraw>& upDraw, ZC_Texture* pTextures, size_t texturesCount)
{
    SimpleDraw(upDraw, pTextures, texturesCount);
}

void ZC_RS::LevelController::LevelDrawing::SimpleDraw(ZC_uptr<ZC_GLDraw>& upDraw, ZC_Texture* pTextures, size_t texturesCount)
{
    if (pTextures)
        for (size_t i = 0; i < texturesCount; ++i) (pTextures + i)->ActiveTexture(i);
    for (auto pDrSet : drawingSets)
    {
        pDrSet->uniforms.Activate();
        upDraw->Draw();
    }
}


//  LvlStencil

ZC_RS::LevelController::LevelStencil::LevelStencil()
    : LevelDrawing(Level::Stencil)
{}

void ZC_RS::LevelController::LevelStencil::Draw(ZC_uptr<ZC_GLDraw>& upDraw, ZC_Texture* pTextures, size_t texturesCount)
{
    if (isFirstDrawing)
    {
        SimpleDraw(upDraw, pTextures, texturesCount);
        isFirstDrawing = false;
    }
    else 
    {
        for (auto pDrSet : drawingSets)
        {
            typedef typename ZC_Uniform::Name UName;
            auto model = *static_cast<ZC_Mat4<float>*>(pDrSet->uniforms.Get(UName::unModel));
            model.Scale({ pDrSet->stencilScale, pDrSet->stencilScale, pDrSet->stencilScale});
            pActiveUniformsStencil->Set(UName::unModel, &model);
            pActiveUniformsStencil->Set(UName::unColor, &(pDrSet->stencilColor));
            pActiveUniformsStencil->Activate();
            upDraw->Draw();
        }
        isFirstDrawing = true;
    }
}


//  LevelController

ZC_RS::LevelController::LevelController(LevelController&& lc)
    : lvlDrawings(std::move(lc.lvlDrawings))
{}

void ZC_RS::LevelController::Draw(Level lvl, ZC_uptr<ZC_GLDraw>& upDraw, ZC_Texture* pTextures, size_t texturesCount)
{
    for (auto& upLvlDrawing : lvlDrawings)
        if (upLvlDrawing->lvl == lvl) upLvlDrawing->Draw(upDraw, pTextures, texturesCount);
}

bool ZC_RS::LevelController::Add(DrawingSet* pDS)
{
    bool isEmpty = lvlDrawings.empty();
    auto lvlDrawingsIter = std::find(lvlDrawings.begin(), lvlDrawings.end(), pDS->lvl);
    if (lvlDrawingsIter != lvlDrawings.end()) lvlDrawingsIter->Get()->Add(pDS);
    else lvlDrawings.emplace_front(GetLvl(pDS->lvl))->Add(pDS);
    return isEmpty;
}

bool ZC_RS::LevelController::Erase(DrawingSet* pDS)
{ 
    auto prevIter = lvlDrawings.before_begin();
    for (auto lvlDrIter = lvlDrawings.begin(); lvlDrIter != lvlDrawings.end(); )
    {
        if ((*lvlDrIter)->lvl == pDS->lvl)
        {
            if ((*lvlDrIter)->Erase(pDS))
            {
                lvlDrawings.erase_after(prevIter);
                return true;
            }
            return false;
        }
        prevIter = lvlDrIter++;
    }
    ZC_ErrorLogger::Err("Can't find DrawingSet in lvlDrawings!", __FILE__, __LINE__);
    return true;
}

ZC_uptr<ZC_RS::LevelController::LevelDrawing> ZC_RS::LevelController::GetLvl(Level lvl)
{
    switch (lvl)
    {
    case Level::Drawing: return { new LevelDrawing{} };
    case Level::Stencil: return { new LevelStencil{} };
    case Level::None: return nullptr;
    default: return nullptr;
    }
}