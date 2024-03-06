#pragma once

#include <ZC/Video/OpenGL/VAO/ZC_VAOConfig.h>
#include <ZC/Video/OpenGL/Shader/ZC_Shader.h>
#include <ZC/Video/OpenGL/Uniform/ZC_Uniform.h>

#include <map>

struct ZC_ShGeometry
{
    enum Name
    {
        none,
        lineOrientation3D,  //  draw as POINTS!!!
    };

    ZC_Shader* GetShader(Name name);

private:
    std::map<Name, ZC_Shader> shaders;
};