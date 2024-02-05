#pragma once

#include "ZC_Shader.h"
#include <ZC/Video/OpenGL/VAO/ZC_VAOConfigs.h>
#include <ZC/Tools/ZC_string.h>
#include <ZC/Tools/ZC_uptr.h>
#include <ZC/Tools/Container/ZC_DynamicArray.h>
#include <ZC/Video/OpenGL/Uniform/ZC_Uniform.h>
#include <ZC/Video/OpenGL/Texture/ZC_TexSets.h>

#include <map>
#include <filesystem>

class ZC_ShLoader
{
public:
    enum Name
    {
        colorF,
        colorV,
        pointV,
        lineV,
        stencilV,

        // texV,
        // texF,

        none,
    };

    struct ShData
    {
        ZC_Shader* pV = nullptr;
        ZC_VAOConfig* pVaoCon = nullptr;
        ZC_DA<ZC_uptr<ZC_Uniform>>* pUniformsV = nullptr;
        ZC_Shader* pF = nullptr;
        ZC_TexSets* pTexSets = nullptr;
        ZC_DA<ZC_uptr<ZC_Uniform>>* pUniformsF = nullptr;
        ZC_Shader* pG = nullptr;
    };

    ~ZC_ShLoader();

    ShData Get(Name v, Name f, Name g);

private:
    struct ShLoadSet
    {
        std::string path;
        GLenum type;
        void* subData;     //  vertex - pZC_VaoConfig; fragment - pZC_TexSets
        ZC_DA<ZC_uptr<ZC_Uniform>> uniforms;

        ShLoadSet(std::string _path, GLenum _type, void* _subData, ZC_DA<ZC_uptr<ZC_Uniform>> _uniforms)
            : path(std::move(_path)),
            type(_type),
            subData(_subData),
            uniforms(std::move(_uniforms))
        {}
    };

    typedef typename ZC_VAOConfigs::Index VAOIndex;
    typedef typename ZC_Uniform::Name UName;
    typedef typename ZC_TexSets::Name TName;
    typedef typename std::filesystem::__cxx11::path Path;
    Path startPath = std::filesystem::current_path().append("assets").append("shaders");

    ZC_DA<ShLoadSet> shLoadSets
    { 
        new ShLoadSet[]
        {
            {
                Path(startPath).append("color.fs").string(),
                GL_FRAGMENT_SHADER,
                nullptr,
                nullptr
            },
            {
                Path(startPath).append("color.vs").string(),
                GL_VERTEX_SHADER,
                &ZC_VAOConfigs::configs[VAOIndex::F_3__UB_3__I_2_10_10_10_REV_1],
                { new ZC_uptr<ZC_Uniform>[]{ new ZC_UMatrix4fv(UName::model, 1, false) }, 1 }
            },
            {
                Path(startPath).append("point.vs").string(),
                GL_VERTEX_SHADER,
                &ZC_VAOConfigs::configs[VAOIndex::F_3__UB_3],
                { new ZC_uptr<ZC_Uniform>[]{ new ZC_UMatrix4fv(UName::model, 1, false) }, 1 }
            },
            {
                Path(startPath).append("line.vs").string(),
                GL_VERTEX_SHADER,
                &ZC_VAOConfigs::configs[VAOIndex::F_3__F_3],
                { new ZC_uptr<ZC_Uniform>[]{ new ZC_UMatrix4fv(UName::model, 1, false) }, 1 }
            },
            {
                Path(startPath).append("stencil.vs").string(),
                GL_VERTEX_SHADER,
                nullptr,
                { new ZC_uptr<ZC_Uniform>[]{ new ZC_UMatrix4fv(UName::model, 1, false), new ZC_U3fv(UName::color, 1) }, 2 }
            },
            // {
            //     Path(startPath).append("tex.vs").string(),
            //     GL_VERTEX_SHADER,
            //     &ZC_VAOConfigs::configs[VAOIndex::F_3__F_2],
            //     { new ZC_uptr<ZC_Uniform>[]{ new ZC_UMatrix4fv(UName::model, 1, false) }, 1 }
            // },
            // {
            //     Path(startPath).append("tex.fs").string(),
            //     GL_FRAGMENT_SHADER,
            //     new ZC_TexSets{ { new TName[]{ TName::color }, 1 } },
            //     nullptr
            // },
            
        },
        Name::none + 1
    };

    struct Set
    {
        ZC_Shader shader;
        void* subData;
        ZC_DA<ZC_uptr<ZC_Uniform>>* pUniforms;

        Set(ZC_Shader&& _shader, void* _subData, ZC_DA<ZC_uptr<ZC_Uniform>>* _pUniforms);
        Set(Set&& s);
    };

    std::map<Name, Set> loaded;

    template<typename TSubData>
    void Load(Name shName, ZC_Shader*& rpShader, TSubData& subData, ZC_DA<ZC_uptr<ZC_Uniform>>*& rpUniforms);

    static ZC_DA<char> ReadShaderFile(const char* path, GLenum type);
    static void FillShaderStart(char* shaderData, const std::string& shaderStart) noexcept;
};

template<typename TSubData>
void ZC_ShLoader::Load(Name shName, ZC_Shader*& rpShader, TSubData& subData, ZC_DA<ZC_uptr<ZC_Uniform>>*& rpUniforms)
{
    auto loadedI = loaded.find(shName);
    if (loadedI == loaded.end())
    {
        auto& shaderSet = shLoadSets[shName];
        subData = static_cast<TSubData>(shaderSet.subData);
        rpUniforms = &shaderSet.uniforms;
        rpShader = &loaded.emplace(shName, Set(ZC_Shader(ReadShaderFile(shaderSet.path.c_str(), shaderSet.type).pHead, shaderSet.type),
            subData, rpUniforms)).first->second.shader;
    }
    else
    {
        rpShader = &loadedI->second.shader;
        subData = static_cast<TSubData>(loadedI->second.subData);
        rpUniforms = loadedI->second.pUniforms;
    }
}