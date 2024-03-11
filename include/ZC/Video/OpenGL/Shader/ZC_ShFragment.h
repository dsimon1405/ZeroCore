#pragma once

#include <ZC/Video/OpenGL/Shader/ZC_Shader.h>
#include <ZC/Video/OpenGL/Uniform/ZC_Uniform.h>
#include <ZC/Video/OpenGL/Texture/ZC_TexSets.h>

#include <map>

struct ZC_ShFragment
{
    enum Name
    {
        color,
        colorTex,
        text,
    };

    struct Set
    {
        ZC_Shader* shader;
        ZC_TexSets texSets {};
        ZC_DA<ZC_uptr<ZC_Uniform>> uniforms {};
    };

    Set GetSet(Name name);

private:
    std::map<Name, ZC_Shader> shaders;
    
    ZC_Shader* GetShader(Name name);
    Set GetVAOAndUniformData(Name name);
};