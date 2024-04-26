#pragma once

#include "ZC_DSController.h"
#include <ZC/Video/OpenGL/Shader/ZC_ShProgs.h>
#include <ZC/Video/OpenGL/VAO/ZC_VAO.h>
#include <ZC/Tools/ZC_sptr.h>
#include <ZC/Tools/Math/ZC_Vec3.h>
#include "ZC_Renders.h"

struct ZC_TexturesSet
{
    int id;
    std::vector<ZC_Texture> textures;

    bool operator == (int _id) const noexcept;
};

struct ZC_DrawerSet
{
    typedef typename ZC_DSController::RenderSet ZC_RSC_RenderSet;
    typedef typename ZC_ShProgs::ShPInitSet ZC_ShP_ShPInitSet;

    ZC_ShProg* pShP;
    ZC_Uniforms* pBaseUniforms;   //  base, for creatiion of activeUniforms
    ZC_VAO vao;
    ZC_uptr<ZC_GLDraw> upGLDraw;
    std::forward_list<ZC_Buffer> buffers;
    std::forward_list<ZC_TexturesSet> texSets;
    std::forward_list<ZC_RSC_RenderSet> renderSets;      //  base, for copy in ZC_DSController

    ZC_DrawerSet(ZC_ShP_ShPInitSet* pShPInitSet, ZC_VAO&& _vao, ZC_uptr<ZC_GLDraw>&& _upDraw, std::forward_list<ZC_Buffer>&& _buffers,
        std::forward_list<ZC_TexturesSet>&& _texSets = {}, std::forward_list<ZC_RSC_RenderSet> _renderSets = { { ZC_RL_Default } });

    static ZC_uptr<ZC_DrawerSet> CreateUptr(ZC_ShP_ShPInitSet* pShPInitSet, ZC_VAO&& _vao, ZC_uptr<ZC_GLDraw>&& _upDraw, std::forward_list<ZC_Buffer>&& _buffers,
        std::forward_list<ZC_TexturesSet>&& _texSets = {}, std::forward_list<ZC_RSC_RenderSet> _renderSets = { { ZC_RL_Default } });

    static ZC_sptr<ZC_DrawerSet> CreateShptr(ZC_ShP_ShPInitSet* pShPInitSet, ZC_VAO&& _vao, ZC_uptr<ZC_GLDraw>&& _upDraw, std::forward_list<ZC_Buffer>&& _buffers,
        std::forward_list<ZC_TexturesSet>&& _texSets = {}, std::forward_list<ZC_RSC_RenderSet> _renderSets = { { ZC_RL_Default } });

    /*
    Params:
    - texSetId - id of ZC_TexturesSet, if no texture must be -1.
    - personalData - forward list of ZC_RSPersonalData heirs (default nothing).

    Return:
    Renderer set controller.
    */
    ZC_DSController MakeZC_DSController(int texSetId = -1, std::forward_list<ZC_uptr<ZC_RSPersonalData>> personalData = {});
    // The following ZC_RSControllers will be created with the ability to be added to the new rendering layer.
    void AddRenderLevel(ZC_RenderLevel renderLevel);
};