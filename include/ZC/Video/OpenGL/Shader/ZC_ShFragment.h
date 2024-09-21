#pragma once

#include <ZC/Video/OpenGL/Shader/ZC_Shader.h>
#include <ZC/Video/OpenGL/Uniform/ZC_Uniform.h>
#include <ZC/Video/OpenGL/Texture/ZC_TexSets.h>

#include <map>

struct ZC_ShFragment1
{
    enum Name
    {
        color,
        colorTex,
        text,
        textWindow,     //  shader same as text, but different uniform configuration
        orthoTexture,

        game_sphere
    };

    struct Set
    {
        ZC_Shader* shader;
        ZC_TexSets texSets;
        std::vector<ZC_uptr<ZC_Uniform>> uniforms {};
    };

    Set GetSet(Name name);

private:
    std::map<Name, ZC_Shader> shaders;
    
    ZC_Shader* GetShader(Name name);
    Set GetVAOAndUniformData(Name name);
};