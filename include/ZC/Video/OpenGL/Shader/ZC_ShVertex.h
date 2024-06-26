#pragma once

#include <ZC/Video/OpenGL/VAO/ZC_VAOConfig.h>
#include <ZC/Video/OpenGL/Shader/ZC_Shader.h>
#include <ZC/Video/OpenGL/Uniform/ZC_Uniform.h>

#include <map>

struct ZC_ShVertex1
{
    enum Name
    {
        colorFigure,
        point,
        lineFigure,
        stencilBorder,
        texture_Vertex_TexCoord,
        lineMesh,
        textWindow,
        textScene,
        textWindowIntoScene,
    };

    struct Set
    {
        ZC_Shader* shader;
        std::vector<ZC_uptr<ZC_Uniform>> uniforms;
    };

    Set GetSet(Name name);

private:
    std::map<Name, ZC_Shader> shaders;

    ZC_Shader* GetShader(Name name);
    std::vector<ZC_uptr<ZC_Uniform>> GetUniformData(Name name);
};