#pragma once

#include "ZC_RendererSet.h"
#include <ZC/Video/OpenGL/Shader/ZC_ShProgs.h>
#include <ZC/Video/OpenGL/VAO/ZC_VAO.h>
#include "ZC_Draw.h"

struct ZC_RS : public ZC_RendererSet
{
    ZC_ShProg* pShP;
    ZC_Uniforms* pBaseUniforms;   //  for creatiion of activeUniforms
    ZC_VAO vao;
    ZC_uptr<ZC_GLDraw> upGLDraw;
    std::forward_list<ZC_Buffer> buffers;

    ZC_RS(typename ZC_ShProgs::ShPInitSet* pShPInitSet, ZC_VAO&& _vao, ZC_uptr<ZC_GLDraw>&& _upDraw,
        std::forward_list<ZC_Buffer>&& _buffers);
    ZC_RS(ZC_RS&&);

    ZC_ShProg* GetShProg() const noexcept override { return pShP; }
    
protected:
    struct LevelDrawing;

    struct LevelController
    {
        LevelController() = default;
        LevelController(LevelController&& lc);

        void Draw(Level lvl, ZC_uptr<ZC_GLDraw>& upDraw, ZC_Texture* pTextures, size_t texturesCount);
        //  return true if was empty
        bool Add(DrawingSet* pDS);
        //  return true if became empty
        bool Erase(DrawingSet* pDS);
    
        struct DrawingStyleSimple
        {
            Level lvl;
            std::forward_list<DrawingSet*> drawingSets;

            DrawingStyleSimple(Level _lvl);

            virtual ~DrawingStyleSimple() = default;

            bool operator == (Level _lvl) const;
            void Add(DrawingSet* pDrSet);
            //  if empty return - true
            bool Erase(DrawingSet* pDrSet);
            virtual void Draw(ZC_uptr<ZC_GLDraw>& upDraw, ZC_Texture* pTextures, size_t texturesCount);
            void SimpleDraw(ZC_uptr<ZC_GLDraw>& upDraw, ZC_Texture* pTextures, size_t texturesCount);
        };
        
        struct DrawingStyleStencil : public DrawingStyleSimple
        {
            static inline ZC_Uniforms* pActiveUniformsStencil;
            bool isFirstDrawing = true;

            DrawingStyleStencil(Level lvl);
            
            void Draw(ZC_uptr<ZC_GLDraw>& upDraw, ZC_Texture* pTextures, size_t texturesCount) override;
        };

    private:
        std::forward_list<ZC_uptr<DrawingStyleSimple>> drawingStyles {};

        ZC_uptr<DrawingStyleSimple> GetLvl(Level lvl);
    };
};