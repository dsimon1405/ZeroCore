#pragma once

#include <ZC/Video/OpenGL/ZC_FBOBuffersController.h>
#include <ZC/Tools/Function/ZC_Function.h>
#include "ZC_RSController.h"

struct ZC_RenderLevelDrawer
{
    virtual ~ZC_RenderLevelDrawer() = default;

    static ZC_uptr<ZC_RenderLevelDrawer> GetRendererLevelDrawer(ZC_DrawLevel lvl);
    static void SetUsersFunction_GetRendererLevelDrawer(ZC_Function<ZC_uptr<ZC_RenderLevelDrawer>(ZC_DrawLevel)>&& func);

    virtual void VAdd(ZC_RSController* pRSController) = 0;
    //  return true if empty, otherwise false
    virtual bool VErase(ZC_RSController* pRSController) = 0;
    virtual void VDraw(ZC_FBOBuffersController& rBuffersController) = 0;

private:
    static inline ZC_Function<ZC_uptr<ZC_RenderLevelDrawer>(ZC_DrawLevel)> funcUsers_GetRendererLevelDrawer;
};


//  ZC_TexturesHolder

struct ZC_TexturesHolder
{
    const ZC_Texture* pTexture;   //  pointer on texture or array of textures
    ulong texturesCount;   //  pTextures count

    // bool operator < (const ZC_TexturesHolder& th) const noexcept;
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