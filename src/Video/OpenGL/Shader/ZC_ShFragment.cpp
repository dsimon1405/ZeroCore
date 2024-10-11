#include <ZC/Video/OpenGL/Shader/ZC_ShFragment.h>

#include <ZC/File/ZC_File.h>

typename ZC_ShFragment1::Set ZC_ShFragment1::GetSet(Name name)
{
    return GetVAOAndUniformData(name);
}

ZC_Shader* ZC_ShFragment1::GetShader(Name name)    //  add here new
{
    auto shadersIter = shaders.find(name);
    if (shadersIter != shaders.end()) return &(shadersIter->second);

    static const ZC_FSPath shadersPath = ZC_FSPath(ZC_ZCDirPath).append("shaders");
    std::string path;
    switch (name)
    {
    case Name::color: path = ZC_FSPath(shadersPath).append("color.fs").string(); break;
    case Name::colorTex: path = ZC_FSPath(shadersPath).append("colorTex.fs").string(); break;
    case Name::text: path = ZC_FSPath(shadersPath).append("text.fs").string(); break;
    case Name::orthoTexture: path = ZC_FSPath(shadersPath).append("orthoTexture.fs").string(); break;

    case Name::game_sphere: path = ZC_FSPath(shadersPath).append("Game/sphere.fs").string(); break;
    case Name::game_cubeMap: path = ZC_FSPath(shadersPath).append("Game/cubeMap.fs").string(); break;
    default: break;
    }

    return &(shaders.emplace(name, ZC_Shader(ZC_Shader::ReadShaderFile(path.c_str(), GL_FRAGMENT_SHADER).pHead, GL_FRAGMENT_SHADER)).first->second);
}

typename ZC_ShFragment1::Set ZC_ShFragment1::GetVAOAndUniformData(Name name)    //  add here new
{
    typedef typename ZC_TexSets::TextureName TName;
    typedef typename ZC_Uniform::NameType UnNT;
    switch (name)
    {
    case Name::color: return { GetShader(name), {}, {} };
    case Name::colorTex: return { GetShader(name), { { new TName[]{ TName::texColor }, 1 } }, {} };
    case Name::text: return { GetShader(name), { { new TName[]{ TName::texColor }, 1 } }, ZC_Uniform::GetUniformVector({ZC_UN_unColor, false}) };
    case Name::textWindow:
    {
        UnNT unoforms[]{ { ZC_UN_unColor, false }, { ZC_UN_unAlpha, false } };
        return { GetShader(Name::text), { { new TName[]{ TName::texColor }, 1 } }, ZC_Uniform::GetUniformVector(unoforms, 2) };      //  shader same as text, but different uniform configuration
    }
    case Name::orthoTexture: return { GetShader(name), { { new TName[]{ TName::texColor }, 1 } }, ZC_Uniform::GetUniformVector({ ZC_UN_unAlpha, false }) };

    case Name::game_sphere:
    {
        UnNT unoforms[]{ { .name = ZC_UN_unColor, .isPointer = true }, { .name = ZC_UN_unAlpha, .isPointer = true } };
        return { .shader = GetShader(name), .texSets = { { new TName[]{ TName::texColor }, 1 } }, .uniforms = ZC_Uniform::GetUniformVector(unoforms, 2) };
    }
    case Name::game_cubeMap: return { GetShader(name), { { new TName[]{ TName::game_cubeMap }, 1 } }, {} };
    default: return {};
    }
}
