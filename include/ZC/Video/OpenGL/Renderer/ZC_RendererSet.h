#pragma once

#include <ZC/Tools/Math/Limits.h>

class ZC_RendererSet
{
public:
    ZC_RendererSet(ZC_RendererSet&& rs);
    virtual ~ZC_RendererSet();

    bool operator == (ZC_RendererSet* pRS) const noexcept;

    virtual void Draw() = 0;

    unsigned char GetLevel() const noexcept;

protected:
    ZC_RendererSet(unsigned char _rendererLevel);

    void AddToRenderer();   //  use in heir Add function that returning ZC_Uniforms*
    void EraseFromRenderer();   //  use in heirs Erase function

private:
    unsigned char rendererLevel;
    size_t uniformsCount = 0;
};

#include <ZC/Video/OpenGL/Shader/ZC_ShProgs.h>
#include <ZC/Video/OpenGL/VAO/ZC_VAO.h>
#include "ZC_Draw.h"

#include <forward_list>

struct ZC_RS : public ZC_RendererSet
{
    ZC_ShProg* pShP;
    ZC_Uniforms* pBaseUniforms;   //  for creatiion of activeUniforms
    ZC_VAO vao;
    ZC_uptr<ZC_Draw> upDraw;
    std::forward_list<ZC_Buffer> buffers;

    ZC_RS(typename ZC_ShProgs::ShPInitSet* pShPInitSet, ZC_VAO&& _vao, ZC_uptr<ZC_Draw>&& _upDraw,
        std::forward_list<ZC_Buffer>&& _buffers, unsigned char _rendererLevel);
    ZC_RS(ZC_RS&&);

    static void SetStencilShaderProgData(typename ZC_ShProgs::ShPInitSet* pShPInitSet) noexcept;

protected:
    static inline ZC_ShProg* pShPStencil;
    static inline ZC_Uniforms* pActiveUniformsStencil;

};


#include <ZC/Video/OpenGL/Uniform/ZC_Uniforms.h>

struct ZC_RSNonTex : public ZC_RS
{
    struct StencilSet
    {
        ZC_Uniforms uniforms;
        ZC_Vec3<float> color;
    };

    std::forward_list<ZC_Uniforms> activeUniforms;
    std::forward_list<StencilSet> stencilUniforms;

    ZC_RSNonTex(typename ZC_ShProgs::ShPInitSet* pShPInitSet, ZC_VAO&& _vao, ZC_uptr<ZC_Draw>&& _upDraw,
        std::forward_list<ZC_Buffer>&& _buffers, unsigned char _rendererLevel = 0);
    ZC_RSNonTex(ZC_RSNonTex&& rs);

    void Draw() override;
    ZC_Uniforms* Add();
    void Erase(ZC_Uniforms* pUniforms);
    void AddStencil(ZC_Uniforms* pUniforms, const ZC_Vec3<float>& color);
    void EraseStencil(ZC_Uniforms* pUniforms);
};

// struct ZC_RSNTStencil : public ZC_RSNonTex
// {
//     struct Uniforms
//     {
//         ZC_Uniforms* pUniforms,
//             * pUniformsStencil;
//     };

//     ZC_ShProg* pShPStencil;
//     ZC_Uniforms* baseUniformsStencil;
//     std::forward_list<ZC_Uniforms> activeUniformsStencil;
//     bool stencilMode = false;

//     ZC_RSNTStencil(typename ZC_ShProgs::ShPInitSet* pShPIS, typename ZC_ShProgs::ShPInitSet* pShPISStencil,
//         ZC_VAO&& _vao, ZC_uptr<ZC_Draw>&& _upDraw, std::forward_list<ZC_Buffer>&& _buffers, unsigned char _rendererLevel = 0);
//     ZC_RSNTStencil(ZC_RSNonTex&& rs);

//     void Draw() override;
//     Uniforms Add();
//     void Erase(Uniforms& uniforms);
//     void SetStencilMode(bool _stencilMode);
// };


#include <ZC/Video/OpenGL/Texture/ZC_Texture.h>
#include <ZC/Tools/ZC_string.h>

#include <vector>

struct ZC_RSTexs : public ZC_RS
{
    struct TexSet
    {
        std::string name;
        std::vector<ZC_Texture> textures;
        std::forward_list<ZC_Uniforms> activeUniforms = {};

        TexSet(std::string&& _name, std::vector<ZC_Texture>&& _textures);

        TexSet(TexSet&& ts);

        bool operator == (const char* _name) const;

        void Draw(ZC_uptr<ZC_Draw>& upDraw);
    };

    std::vector<TexSet> texSets;

    ZC_RSTexs(typename ZC_ShProgs::ShPInitSet* pShPInitSet, ZC_VAO&& _vao, ZC_uptr<ZC_Draw>&& _upDraw,
            std::forward_list<ZC_Buffer>&& _buffers, std::vector<TexSet>&& _texSets = {}, unsigned char _rendererLevel = 0);

    ZC_RSTexs(ZC_RSTexs&& rs);

    void Draw() override;
    ZC_Uniforms* Add(const char* texSetName);
    void Erase(ZC_Uniforms* uniforms, const char* texSetName);
};