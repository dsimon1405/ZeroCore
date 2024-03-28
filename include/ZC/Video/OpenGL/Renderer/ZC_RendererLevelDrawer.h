#pragma once

#include "ZC_RBufferCleaner.h"
#include <ZC/Tools/Function/ZC_Function.h>
#include "ZC_RSController.h"

struct ZC_RendererLevelDrawer
{
    virtual ~ZC_RendererLevelDrawer() = default;

    static ZC_uptr<ZC_RendererLevelDrawer> GetRendererLevelDrawer(ZC_RendererLevel lvl);
    static void SetUsersFunction_GetRendererLevelDrawer(ZC_Function<ZC_uptr<ZC_RendererLevelDrawer>(ZC_RendererLevel)>&& func);

    virtual void Draw(ZC_RBufferCleaner& rBufferCleaner) = 0;
    virtual void Add(ZC_RSController* pRSController) = 0;
    //  return true if empty, otherwise false
    virtual bool Erase(ZC_RSController* pRSController) = 0;

private:
    static inline ZC_Function<ZC_uptr<ZC_RendererLevelDrawer>(ZC_RendererLevel)> funcUsers_GetRendererLevelDrawer;
};


//  ZC_TexturesHolder

struct ZC_TexturesHolder
{
    const ZC_Texture* pTexture;   //  pointer on texture or array of textures
    size_t texturesCount;   //  pTextures count

    bool operator < (const ZC_TexturesHolder& th) const noexcept;
    bool operator == (const ZC_TexturesHolder& th) const noexcept;
    void ActivateOpenGL() const;
};


//  ZC_RLDSet_Uniforms_GLDraw

struct ZC_RLDData_Uniforms_GLDraw
{
    const ZC_Uniforms* pUniforms;
    const ZC_GLDraw* pGLDraw;
    
    bool operator == (const ZC_RLDData_Uniforms_GLDraw& unifAndGLDraw) const noexcept;
    void Draw() const;
};