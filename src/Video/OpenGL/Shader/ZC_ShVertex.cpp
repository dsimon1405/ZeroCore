#include <ZC/Video/OpenGL/Shader/ZC_ShVertex.h>

#include <ZC/File/ZC_File.h>
#include <ZC/ErrorLogger/ZC_ErrorLogger.h>

typename ZC_ShVertex::Set ZC_ShVertex::GetSet(Name name)
{
    Set set { GetShader(name) };
    GetUniformData(name, set);
    return set;
}

ZC_Shader* ZC_ShVertex::GetShader(Name name)
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
    }

    return &(shaders.emplace(name, ZC_Shader(ZC_Shader::ReadShaderFile(path.c_str(), GL_VERTEX_SHADER).pHead, GL_VERTEX_SHADER)).first->second);
}

void ZC_ShVertex::GetUniformData(Name name, Set& rSet)
{
    typedef typename ZC_Uniform::NameType UnNT;
    switch (name)
    {
    case Name::colorFigure:
    {
        UnNT unoforms[]{{ZC_UN_unModel, true}};
        rSet.uniforms = std::move(ZC_Uniform::GetUniformsDA(unoforms, 1));
    } break;
    case Name::point:
    {
        UnNT unoforms[]{{ZC_UN_unModel, true}};
        rSet.uniforms = std::move(ZC_Uniform::GetUniformsDA(unoforms, 1));
    } break;
    case Name::lineFigure:
    {
        UnNT unoforms[]{{ZC_UN_unModel, true}};
        rSet.uniforms = std::move(ZC_Uniform::GetUniformsDA(unoforms, 1));
    } break;
    case Name::stencil:
    {
        UnNT unoforms[]{{ZC_UN_unModel, true}, {ZC_UN_unColor, false}};
        rSet.uniforms = std::move(ZC_Uniform::GetUniformsDA(unoforms, 2));
    } break;
    case Name::texture:
    {
        UnNT unoforms[]{{ZC_UN_unModel, true}};
        rSet.uniforms = std::move(ZC_Uniform::GetUniformsDA(unoforms, 1));
    } break;
    case Name::lineMesh:
    {
        UnNT unoforms[]{{ZC_UN_unModel, false}};
        rSet.uniforms = std::move(ZC_Uniform::GetUniformsDA(unoforms, 1));
    } break;
    case Name::quadOrientation3D:
    {
        UnNT unoforms[]{{ZC_UN_unModel, false}};
        rSet.uniforms = std::move(ZC_Uniform::GetUniformsDA(unoforms, 1));
    } break;
    case Name::textWindow:
    {
        UnNT unoforms[]{{ZC_UN_unPosition, true}};
        rSet.uniforms = std::move(ZC_Uniform::GetUniformsDA(unoforms, 1));
    } break;
    case Name::textScene:
    {
        UnNT unoforms[]{{ZC_UN_unModel, true, 1, false}};
        rSet.uniforms = std::move(ZC_Uniform::GetUniformsDA(unoforms, 1));
    } break;
    default: break;
    }
}
