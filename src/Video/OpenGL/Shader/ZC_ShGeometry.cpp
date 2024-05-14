#include <ZC/Video/OpenGL/Shader/ZC_ShGeometry.h>

#include <ZC/File/ZC_File.h>

ZC_Shader* ZC_ShGeometry::GetShader(Name name)    //  add here new
{
    auto shadersIter = shaders.find(name);
    if (shadersIter != shaders.end()) return &(shadersIter->second);

    static const ZC_FSPath shadersPath = ZC_FSPath(ZC_ZCDirPath).append("shaders");
    std::string path;
    switch (name)
    {
    // case Name::lineOrientation3D: path = ZC_FSPath(shadersPath).append("lineOrientation3D.gs").string(); break;
    case Name::none: return nullptr;
    }

    return &(shaders.emplace(name, ZC_Shader(ZC_Shader::ReadShaderFile(path.c_str(), GL_GEOMETRY_SHADER).pHead, GL_GEOMETRY_SHADER)).first->second);
}