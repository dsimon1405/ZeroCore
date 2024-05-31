#pragma once

#include <ZC/Video/OpenGL/Shader/ZC_Shader.h>
#include <ZC/Video/OpenGL/Uniform/ZC_Uniform.h>

#include <map>

struct ZC_ShGeometry1
{
    enum Name
    {
        none,
    };

    ZC_Shader* GetShader(Name name);

private:
    std::map<Name, ZC_Shader> shaders;
};