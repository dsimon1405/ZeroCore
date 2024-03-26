#include <ZC/Video/OpenGL/Shader/ZC_ShVertex.h>

#include <ZC/File/ZC_File.h>
#include <ZC/ErrorLogger/ZC_ErrorLogger.h>

typename ZC_ShVertex::Set ZC_ShVertex::GetSet(Name name)
{
    return { GetShader(name), GetUniformData(name) };
}

ZC_Shader* ZC_ShVertex::GetShader(Name name)    //  add here new
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
    case Name::stencil: path = ZC_FSPath(shadersPath).append("stencil.vs").string(); break;
    case Name::texture: path = ZC_FSPath(shadersPath).append("tex.vs").string(); break;
    case Name::lineMesh: path = ZC_FSPath(shadersPath).append("lineMesh.vs").string(); break;
    case Name::lineOrientation3D: path = ZC_FSPath(shadersPath).append("lineOrientation3D.vs").string(); break;
    case Name::quadOrientation3D: path = ZC_FSPath(shadersPath).append("quadOrientation3D.vs").string(); break;
    case Name::textWindow: path = ZC_FSPath(shadersPath).append("textWindow.vs").string(); break;
    case Name::textScene: path = ZC_FSPath(shadersPath).append("textScene.vs").string(); break;
    case Name::textWindowIntoScene: path = ZC_FSPath(shadersPath).append("textWindowIntoScene.vs").string(); break;
    }

    return &(shaders.emplace(name, ZC_Shader(ZC_Shader::ReadShaderFile(path.c_str(), GL_VERTEX_SHADER).pHead, GL_VERTEX_SHADER)).first->second);
}

std::vector<ZC_uptr<ZC_Uniform>> ZC_ShVertex::GetUniformData(Name name)    //  add here new
{
    typedef typename ZC_Uniform::NameType UnNT;
    switch (name)
    {
    case Name::colorFigure: return ZC_Uniform::GetUniformVector({ ZC_UN_unModel, true });
    case Name::point: return ZC_Uniform::GetUniformVector({ ZC_UN_unModel, true });
    case Name::lineFigure: return ZC_Uniform::GetUniformVector({ ZC_UN_unModel, true });
    case Name::stencil:
    {
        UnNT unoforms[]{ { ZC_UN_unModel, true }, { ZC_UN_unColor, false }};
        return ZC_Uniform::GetUniformVector(unoforms, 2);
    }
    case Name::texture: return ZC_Uniform::GetUniformVector({ ZC_UN_unModel, true });
    case Name::lineMesh: return ZC_Uniform::GetUniformVector({ ZC_UN_unModel, false });
    case Name::quadOrientation3D: return ZC_Uniform::GetUniformVector({ ZC_UN_unModel, false });
    case Name::textWindow: return ZC_Uniform::GetUniformVector({ ZC_UN_unPositionWindow, true });
    case Name::textScene: return ZC_Uniform::GetUniformVector({ ZC_UN_unModel, true });
    case Name::textWindowIntoScene: return ZC_Uniform::GetUniformVector({ ZC_UN_unPositionScene, true });
    default: return std::vector<ZC_uptr<ZC_Uniform>>{};
    }
}
