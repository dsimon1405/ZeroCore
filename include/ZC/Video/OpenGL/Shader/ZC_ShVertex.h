#pragma once

#include <ZC/Video/OpenGL/VAO/ZC_VAOConfig.h>
#include <ZC/Video/OpenGL/Shader/ZC_Shader.h>
#include <ZC/Video/OpenGL/Uniform/ZC_Uniform.h>

#include <map>

struct ZC_ShVertex
{
    enum Name
    {
        colorFigure,
        point,
        lineFigure,
        stencil,
        texture,
        lineMesh,
        lineOrientation3D,  //  draw as POINTS!!!
        quadOrientation3D,
        textWindow,
    };

    typedef typename ZC_VAOConfig::FormatShVLayoutAndUsingFormatsPacker VAO_FSVLaUFP; 
    struct Set
    {
        ZC_Shader* shader;
        VAO_FSVLaUFP vaoConSets {};
        ZC_DA<ZC_uptr<ZC_Uniform>> uniforms {};
    };

    typedef typename ZC_VAOConfig::FormatShVLayout VAOConFSVL;
    Set GetSet(Name name, VAOConFSVL vaoConFSVL);

private:
    std::map<Name, ZC_Shader> shaders;

    ZC_Shader* GetShader(Name name);
    void GetVAOAndUniformData(Name name, VAOConFSVL vaoConFSVL, Set& rSet);
};