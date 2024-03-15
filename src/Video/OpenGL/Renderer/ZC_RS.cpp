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
        if (pShPog) LevelController::DrawingStyleStencilBorder::pActiveUniformsStencil = &(pShPog->uniforms);
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


//  DrawingStyleSimple

ZC_RS::LevelController::DrawingStyleSimple::DrawingStyleSimple(Level _lvl)
    : lvl(_lvl)
{}

bool ZC_RS::LevelController::DrawingStyleSimple::operator == (Level _lvl) const
{
    return lvl == _lvl;
}

void ZC_RS::LevelController::DrawingStyleSimple::Add(DrawingSet* pDrSet)
{
    drawingSets.emplace_front(pDrSet);
}

bool ZC_RS::LevelController::DrawingStyleSimple::Erase(DrawingSet* pDrSet)
{
    if (!ZC_ForwardListErase(drawingSets, pDrSet)) ZC_ErrorLogger::Err("Can't find DrawingSet in drawingSets!", __FILE__, __LINE__);
    return drawingSets.empty();
}

void ZC_RS::LevelController::DrawingStyleSimple::Draw(ZC_uptr<ZC_GLDraw>& upDraw, ZC_Texture* pTextures, size_t texturesCount)
{
    SimpleDraw(upDraw, pTextures, texturesCount);
}

void ZC_RS::LevelController::DrawingStyleSimple::SimpleDraw(ZC_uptr<ZC_GLDraw>& upDraw, ZC_Texture* pTextures, size_t texturesCount)
{
    if (pTextures)
        for (size_t i = 0; i < texturesCount; ++i) (pTextures + i)->ActiveTexture(i);
    for (auto pDrSet : drawingSets)
    {
        pDrSet->uniforms.Activate();
        upDraw->Draw();
    }
}


//  DrawingStyleStencil

ZC_RS::LevelController::DrawingStyleStencilBorder::DrawingStyleStencilBorder(Level lvl)
    : DrawingStyleSimple(lvl)
{}

void ZC_RS::LevelController::DrawingStyleStencilBorder::Draw(ZC_uptr<ZC_GLDraw>& upDraw, ZC_Texture* pTextures, size_t texturesCount)
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
            auto model = *static_cast<const ZC_Mat4<float>*>(pDrSet->uniforms.Get(UName::unModel));
            model.Scale(pDrSet->stencilScale, pDrSet->stencilScale, pDrSet->stencilScale);
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
    : drawingStyles(std::move(lc.drawingStyles))
{}

void ZC_RS::LevelController::Draw(Level lvl, ZC_uptr<ZC_GLDraw>& upDraw, ZC_Texture* pTextures, size_t texturesCount)
{
    for (auto& upLvlDrawing : drawingStyles)
        if (upLvlDrawing->lvl == lvl) upLvlDrawing->Draw(upDraw, pTextures, texturesCount);
}

bool ZC_RS::LevelController::Add(DrawingSet* pDS)
{
    bool isEmpty = drawingStyles.empty();
    for (auto& upDrawingStyle : drawingStyles)
    {
        if (upDrawingStyle->lvl == pDS->lvl)
        {
            upDrawingStyle->Add(pDS);
            return isEmpty;
        }
    }
    drawingStyles.emplace_front(GetLvl(pDS->lvl))->Add(pDS);
    return isEmpty;
}

bool ZC_RS::LevelController::Erase(DrawingSet* pDS)
{ 
    auto prevIter = drawingStyles.before_begin();
    for (auto lvlDrIter = drawingStyles.begin(); lvlDrIter != drawingStyles.end(); )
    {
        if ((*lvlDrIter)->lvl == pDS->lvl)
        {
            if ((*lvlDrIter)->Erase(pDS))
            {
                drawingStyles.erase_after(prevIter);
                return true;
            }
            return false;
        }
        prevIter = lvlDrIter++;
    }
    ZC_ErrorLogger::Err("Can't find DrawingSet in lvlDrawings!", __FILE__, __LINE__);
    return true;
}

ZC_uptr<ZC_RS::LevelController::DrawingStyleSimple> ZC_RS::LevelController::GetLvl(Level lvl)
{
    switch (lvl)
    {
    case Level::StencilBorder: return { new DrawingStyleStencilBorder(lvl) };   //  at now only for stencil need some special draawing prepares in separate class
    default: return { new DrawingStyleSimple(lvl) };
    }
}