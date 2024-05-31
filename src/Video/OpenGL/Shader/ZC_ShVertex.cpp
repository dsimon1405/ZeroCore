#include <ZC/Video/OpenGL/Shader/ZC_ShVertex.h>

#include <ZC/File/ZC_File.h>
#include <ZC/ErrorLogger/ZC_ErrorLogger.h>

typename ZC_ShVertex1::Set ZC_ShVertex1::GetSet(Name name)
{
    return { GetShader(name), GetUniformData(name) };
}

ZC_Shader* ZC_ShVertex1::GetShader(Name name)    //  add here new
{
    auto shadersIter = shaders.find(name);
    if (shadersIter != shaders.end()) return &(shadersIter->second);

    static const ZC_FSPath shadersPath = ZC_FSPath(ZC_ZCDirPath).append("shaders");
    std::string path;
    switch (name)
    {
    case Name::colorFigure: path = ZC_FSPath(shadersPath).append("colorFigure.vs").string(); break;
    case Name::point: path = ZC_FSPath(shadersPath).append("point.vs").string(); break;
    case Name::lineFigure: path = ZC_FSPath(shadersPath).append("lineFigure.vs").string(); break;
    case Name::stencilBorder: path = ZC_FSPath(shadersPath).append("stencilBorder.vs").string(); break;
    case Name::texture_Vertex_TexCoord: path = ZC_FSPath(shadersPath).append("texture_Vertex_TexCoord.vs").string(); break;
    case Name::lineMesh: path = ZC_FSPath(shadersPath).append("lineMesh.vs").string(); break;
    case Name::textWindow: path = ZC_FSPath(shadersPath).append("textWindow.vs").string(); break;
    case Name::textScene: path = ZC_FSPath(shadersPath).append("textScene.vs").string(); break;
    case Name::textWindowIntoScene: path = ZC_FSPath(shadersPath).append("textWindowIntoScene.vs").string(); break;
    }

    return &(shaders.emplace(name, ZC_Shader(ZC_Shader::ReadShaderFile(path.c_str(), GL_VERTEX_SHADER).pHead, GL_VERTEX_SHADER)).first->second);
}

std::vector<ZC_uptr<ZC_Uniform>> ZC_ShVertex1::GetUniformData(Name name)    //  add here new
{
    typedef typename ZC_Uniform::NameType UnNT;
    switch (name)
    {
    case Name::colorFigure:
    {
        UnNT uniforms[]{ { ZC_UN_unModel, true }, { ZC_UN_unAlpha, true }, { ZCR_UN_unUseLight, true } };
        return ZC_Uniform::GetUniformVector(uniforms, 3);
    }
    case Name::point: return ZC_Uniform::GetUniformVector({ ZC_UN_unModel, true });
    case Name::lineFigure: return ZC_Uniform::GetUniformVector({ ZC_UN_unModel, true });
    case Name::stencilBorder:
    {
        UnNT uniforms[]{ { ZC_UN_unModel, true }, { ZC_UN_unColor, false } };
        return ZC_Uniform::GetUniformVector(uniforms, 2);
    }
    case Name::texture_Vertex_TexCoord: return ZC_Uniform::GetUniformVector({ ZC_UN_unModel, true });
    case Name::lineMesh: return ZC_Uniform::GetUniformVector({ ZC_UN_unModel, false });
    case Name::textWindow: return ZC_Uniform::GetUniformVector({ ZC_UN_unPositionWindow, true });
    case Name::textScene: return ZC_Uniform::GetUniformVector({ ZC_UN_unModel, true });
    case Name::textWindowIntoScene: return ZC_Uniform::GetUniformVector({ ZC_UN_unPositionScene, true });
    default: return std::vector<ZC_uptr<ZC_Uniform>>{};
    }
}
