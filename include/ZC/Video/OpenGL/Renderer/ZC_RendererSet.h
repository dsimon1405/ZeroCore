#pragma once

#include "ZC_RSController.h"
#include <ZC/Video/OpenGL/Shader/ZC_ShProgs.h>
#include <ZC/Video/OpenGL/VAO/ZC_VAO.h>
#include <ZC/Tools/ZC_sptr.h>

struct ZC_TexturesSet
{
    int id;
    std::vector<ZC_Texture> textures;
};

struct ZC_RendererSet
{
    ZC_ShProg* pShP;
    ZC_Uniforms* pBaseUniforms;   //  for creatiion of activeUniforms
    ZC_VAO vao;
    ZC_uptr<ZC_GLDraw> upGLDraw;
    std::forward_list<ZC_Buffer> buffers;
    std::forward_list<ZC_TexturesSet> texSets;

    ZC_RendererSet(typename ZC_ShProgs::ShPInitSet* pShPInitSet, ZC_VAO&& _vao, ZC_uptr<ZC_GLDraw>&& _upDraw,
        std::forward_list<ZC_Buffer>&& _buffers, std::forward_list<ZC_TexturesSet>&& _texSets = {});

    static ZC_uptr<ZC_RendererSet> CreateUptr(typename ZC_ShProgs::ShPInitSet* pShPInitSet, ZC_VAO&& _vao, ZC_uptr<ZC_GLDraw>&& _upGLDraw,
        std::forward_list<ZC_Buffer>&& _buffers, std::forward_list<ZC_TexturesSet>&& _texSets = {});

    static ZC_sptr<ZC_RendererSet> CreateShptr(typename ZC_ShProgs::ShPInitSet* pShPInitSet, ZC_VAO&& _vao, ZC_uptr<ZC_GLDraw>&& _upGLDraw,
        std::forward_list<ZC_Buffer>&& _buffers, std::forward_list<ZC_TexturesSet>&& _texSets = {});

    /*
    Params:
    texSetId - ZC_TexturesSet.id or -1 if there's no textures sets were added in ZC_RendererSet.
    personalData - forward list of ZC_RSPersonalData heirs (all heir among ZC_RSPDUniformData can be added by user!)

    Return:
    Renderer set controller.
    */
    ZC_RSController MakeZC_RSController(int texSetId = -1, std::forward_list<ZC_uptr<ZC_RSPersonalData>>&& personalData = {});
};