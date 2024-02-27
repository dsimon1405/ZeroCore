#include <ZC/Video/OpenGL/Shader/ZC_ShFragment.h>

#include <ZC/File/ZC_File.h>
#include <ZC/ErrorLogger/ZC_ErrorLogger.h>

typename ZC_ShFragment::Set ZC_ShFragment::GetSet(Name name)
{
    return GetVAOAndUniformData(name);
}

ZC_Shader* ZC_ShFragment::GetShader(Name name)
{
    auto shadersIter = shaders.find(name);
    if (shadersIter != shaders.end()) return &(shadersIter->second);

    static const ZC_FSPath shadersPath = ZC_FSPath(ZC_ZCDirPath).append("shaders");
    std::string path;
    switch (name)
    {
    case Name::color: path = ZC_FSPath(shadersPath).append("color.fs").string(); break;
    case Name::texture: path = ZC_FSPath(shadersPath).append("tex.fs").string(); break;
    }

    return &(shaders.emplace(name, ZC_Shader(ZC_Shader::ReadShaderFile(path.c_str(), GL_FRAGMENT_SHADER).pHead, GL_FRAGMENT_SHADER)).first->second);
}

typename ZC_ShFragment::Set ZC_ShFragment::GetVAOAndUniformData(Name name)
{
    // typedef typename ZC_Uniform::Name UName;
    typedef typename ZC_TexSets::TextureName TName;
    switch (name)
    {
    case Name::color: return
    {
        GetShader(name),
        {},
        {}
    };
    case Name::texture: return
    {
        GetShader(name),
        { { new TName[]{ TName::texColor }, 1 } },
        {}
    };
    default: return {};
    }
}
