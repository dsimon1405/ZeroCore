#pragma once

#include <ZC/Video/OpenGL/Renderer/ZC_Draw.h>
#include <ZC/Video/OpenGL/Texture/ZC_Texture.h>
#include <ZC/Video/OpenGL/VAO/ZC_VAO.h>
#include "ZC_RSPersonalData.h"

#include <forward_list>

enum ZC_RendererLevels
{
    None = -1,
    Drawing = 0,
    TextScene = 1000,
    StencilBorder = 3000,        //  must be after scene objects!
    TextWindowIntoScene = 4000,     //  in ZC_RLDText.h
    TextWindow = 4001,
};

typedef int ZC_RendererLevel;   //  Renderer Levels for user
#define ZC_RL_None ZC_RendererLevels::None
#define ZC_RL_Drawing ZC_RendererLevels::Drawing
#define ZC_RL_StencilBorder ZC_RendererLevels::StencilBorder

struct ZC_RSController   //  stores data of object of ZC_RendererSet for search in ZC_Renderer
{
    // ZC_RSController() = default;

    /*
    Params:
    _pTexture - in ZC_RSTextured pointer on the start of "textures" into "texSets"; in ZC_RSNotTextured nullptr.
    _pTextureCount - in ZC_RSTextured textures.size(); in ZC_RSNotTextured 0.
    */
    ZC_RSController(const ZC_ShProg* _pShProg, const ZC_GLDraw* _pGLDraw, const ZC_VAO* _pVAO, const ZC_Texture* _pTexture,
        size_t _texturesCount, std::forward_list<ZC_uptr<ZC_RSPersonalData>>&& _personalData);

    ~ZC_RSController();

    //  lvl - where to switch in ZC_Renderer. If lvl - None, just remove from ZC_Renderer.
    void SwitchToLvl(ZC_RendererLevel lvl);
    void SetData(ZC_RSPDCategory category, ZC_RSPDStoredData* pData);
    void SetUniformsData(ZC_UniformName unNmae, void* pData);
    const void* GetPersonalData(ZC_RSPDCategory category) const;
    const void* GetDataFromUniforms(ZC_UniformName name) const;
    ZC_RSController MakeCopy() const;
    bool IsDrawing() const noexcept;

    ZC_RendererLevel rendererLevel = ZC_RL_None;
    const ZC_ShProg* pShProg = nullptr;
    const ZC_GLDraw* pGLDraw = nullptr;
    const ZC_VAO* pVAO = nullptr;
    const ZC_Texture* pTexture = nullptr;   //  in ZC_RSTextured pointer on the start of "textures" into "texSets"; in ZC_RSNotTextured nullptr; in ZC_RSCommonTexture pointer on texture.
    size_t texturesCount = 0;   //  in ZC_RSTextured textures.size(); in ZC_RSNotTextured 0; in ZC_RSCommonTexture 1.
    std::forward_list<ZC_uptr<ZC_RSPersonalData>> personalData;
};