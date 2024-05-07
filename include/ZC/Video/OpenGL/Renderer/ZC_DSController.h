#pragma once

#include <ZC/Video/OpenGL/ZC_GLDraw.h>
#include <ZC/Video/OpenGL/Texture/ZC_Texture.h>
#include <ZC/Video/OpenGL/VAO/ZC_VAO.h>
#include "ZC_RSPersonalData.h"
#include "ZC_RenderLevel.h"

#include <forward_list>

struct ZC_TexturesHolder
{
    const ZC_Texture* pTexture = nullptr;   //  pointer on texture or array of textures
    uint texturesCount = 0;   //  pTextures count

    bool operator == (const ZC_TexturesHolder& th) const noexcept;
    void ActivateOpenGL() const;
};


class ZC_Render;

struct ZC_DSController   //  stores data of object of ZC_DrawerSet for search in ZC_Renderer
{
    struct RenderSet
    {
        ZC_Render* pRender;
        ZC_DrawerLevel drawerLevel;

        RenderSet(ZC_RenderLevel renderLevel);

        bool operator == (ZC_RenderLevel renderLevel);

        void SwitchToDrawLevel(ZC_DrawerLevel _drawLevel, ZC_DSController* pRSController);
    };

    // ZC_DSController() = default;

    /*
    Params:
    _pTexture - in ZC_RSTextured pointer on the start of "textures" into "texSets"; in ZC_RSNotTextured nullptr.
    _texturesCount - count of textures in _pTexture array.
    */
    ZC_DSController(const ZC_ShProg* _pShProg, const ZC_GLDraw* _pGLDraw, const ZC_VAO* _pVAO, const ZC_TexturesHolder& _texturesHolder,
        std::forward_list<ZC_uptr<ZC_RSPersonalData>>&& _personalData, std::forward_list<RenderSet> _renderSets);

    /*
    Switchs to choosen draw level in choosen render level. In one render level could only at one draw level at time.

    Paramas:
    - renderLevel - level of render for switching.
    - drawerLevel - where to switch in ZC_Render. If ZC_DL_None, just remove from ZC_Render.
    */
    void SwitchToDrawLvl(ZC_RenderLevel renderLevel, ZC_DrawerLevel drawerLevel);
    void SetData(ZC_RSPDCategory category, ZC_RSPDStoredData* pData);
    void SetUniformsData(ZC_UniformName unName, void* pData);
    const void* GetPersonalData(ZC_RSPDCategory category) const;
    const void* GetDataFromUniforms(ZC_UniformName unName) const;
    ZC_DSController MakeCopy() const;
    bool IsDrawing(ZC_RenderLevel renderLevel);
    void AddRender(ZC_RenderLevel renderLevel);
    void SetTexturesHolder(const ZC_TexturesHolder& _texturesHolder);

    template<typename T>
    auto GetByType();

    const ZC_ShProg* pShProg = nullptr;
    const ZC_GLDraw* pGLDraw = nullptr;
    const ZC_VAO* pVAO = nullptr;
    ZC_TexturesHolder texturesHolder;
    std::forward_list<ZC_uptr<ZC_RSPersonalData>> personalData;
    std::forward_list<RenderSet> renderSets;
};


struct ZC_RLDData_Uniforms_GLDraw
{
    const ZC_Uniforms* pUniforms;
    const ZC_GLDraw* pGLDraw;
    
    bool operator == (const ZC_RLDData_Uniforms_GLDraw& unifAndGLDraw) const noexcept;
    void Draw();
};

struct ZC_RLDData_Uniforms_GLDraw_StencilBorder
{
    const ZC_Uniforms* pUniforms;
    const ZC_GLDraw* pGLDraw;
    const ZC_RSPDStencilBorderData* pStencilBorderData;
    bool isFirstDraw = true;
    
    bool operator == (const ZC_RLDData_Uniforms_GLDraw_StencilBorder& unifAndGLDraw) const noexcept;
    void Draw();
};


template<typename T>
auto ZC_DSController::GetByType()
{
    if constexpr (std::same_as<T, const ZC_ShProg*>) return pShProg;
    else if constexpr (std::same_as<T, const ZC_VAO*>) return pVAO;
    else if constexpr (std::same_as<T, ZC_TexturesHolder>) return texturesHolder;
    else if constexpr (std::same_as<T, ZC_RLDData_Uniforms_GLDraw>)
        return ZC_RLDData_Uniforms_GLDraw{ static_cast<const ZC_Uniforms*>(GetPersonalData(ZC_RSPDC_uniforms)), pGLDraw };
    else if constexpr (std::same_as<T, ZC_RLDData_Uniforms_GLDraw_StencilBorder>)
        return ZC_RLDData_Uniforms_GLDraw_StencilBorder{ static_cast<const ZC_Uniforms*>(GetPersonalData(ZC_RSPDC_uniforms)),
            pGLDraw, static_cast<const ZC_RSPDStencilBorderData*>(GetPersonalData(ZC_RSPDC_stencilBorder)) };
}