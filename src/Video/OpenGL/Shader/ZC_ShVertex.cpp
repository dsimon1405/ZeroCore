#include <ZC/Video/OpenGL/Shader/ZC_ShVertex.h>

#include <ZC/File/ZC_File.h>
#include <ZC/ErrorLogger/ZC_ErrorLogger.h>

typename ZC_ShVertex::Set ZC_ShVertex::GetSet(Name name, VAOConFSVL vaoConFSVL)
{
    Set set { GetShader(name) };
    GetVAOAndUniformData(name, vaoConFSVL, set);
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
    }

    return &(shaders.emplace(name, ZC_Shader(ZC_Shader::ReadShaderFile(path.c_str(), GL_VERTEX_SHADER).pHead, GL_VERTEX_SHADER)).first->second);
}

void ZC_ShVertex::GetVAOAndUniformData(Name name, VAOConFSVL vaoConFSVL, Set& rSet)
{
    typedef typename ZC_VAOConfig::UsingFormatsPacker VAOUFP;
    typedef typename ZC_Uniform::Name UName;
    switch (name)
    {
    case Name::colorFigure:
    {
        switch (vaoConFSVL)
        {
        case VAOConFSVL::F_3_0__UB_3_1__I_2_10_10_10_REV_1_2: rSet.vaoConSets = { vaoConFSVL, VAOUFP().Pack(0).Pack(2)}; break;
        default: ZC_ErrorLogger::Err("There's no ZC_VAOConfig::FormatShVLayout for that shader!", __FILE__, __LINE__);
        }
        rSet.uniforms = std::move(ZC_DA<ZC_uptr<ZC_Uniform>>(new ZC_uptr<ZC_Uniform>[]{ new ZC_UMatrix4fvPointer(UName::unModel, 1, false) }, 1));
    } break;
    case Name::point:
    {
        switch (vaoConFSVL)
        {
        case VAOConFSVL::F_3_0__UB_3_1__I_2_10_10_10_REV_1_2: rSet.vaoConSets = { vaoConFSVL, VAOUFP().Pack(0).Pack(1) }; break;
        default: ZC_ErrorLogger::Err("There's no ZC_VAOConfig::FormatShVLayout for that shader!", __FILE__, __LINE__);
        }
        rSet.uniforms = std::move(ZC_DA<ZC_uptr<ZC_Uniform>>(new ZC_uptr<ZC_Uniform>[]{ new ZC_UMatrix4fvPointer(UName::unModel, 1, false) }, 1));
    } break;
    case Name::lineFigure:
    {
        switch (vaoConFSVL)
        {
        case VAOConFSVL::F_3_0__UB_3_1__I_2_10_10_10_REV_1_2: rSet.vaoConSets = { vaoConFSVL, VAOUFP().Pack(0).Pack(1) }; break;
        default: ZC_ErrorLogger::Err("There's no ZC_VAOConfig::FormatShVLayout for that shader!", __FILE__, __LINE__);
        }
        rSet.uniforms = std::move(ZC_DA<ZC_uptr<ZC_Uniform>>(new ZC_uptr<ZC_Uniform>[]{ new ZC_UMatrix4fvPointer(UName::unModel, 1, false) }, 1));
    } break;
    case Name::stencil:
    {
        rSet.uniforms = std::move(ZC_DA<ZC_uptr<ZC_Uniform>>(new ZC_uptr<ZC_Uniform>[]{ new ZC_UMatrix4fvPointer(UName::unModel, 1, false), new ZC_U1uiValue(UName::unColor) }, 2));
    } break;
    case Name::texture:
    {
        switch (vaoConFSVL)
        {
        case VAOConFSVL::F_3_0__F_2_3: rSet.vaoConSets = { vaoConFSVL, VAOUFP().Pack(0).Pack(1) }; break;
        default: ZC_ErrorLogger::Err("There's no ZC_VAOConfig::FormatShVLayout for that shader!", __FILE__, __LINE__);
        }
        rSet.uniforms = std::move(ZC_DA<ZC_uptr<ZC_Uniform>>(new ZC_uptr<ZC_Uniform>[]{ new ZC_UMatrix4fvPointer(UName::unModel, 1, false) }, 1));
    } break;
    case Name::lineMesh:
    {
        switch (vaoConFSVL)
        {
        case VAOConFSVL::F_3_0: rSet.vaoConSets = { vaoConFSVL, VAOUFP().Pack(0) }; break;
        default: ZC_ErrorLogger::Err("There's no ZC_VAOConfig::FormatShVLayout for that shader!", __FILE__, __LINE__);
        }
        rSet.uniforms = std::move(ZC_DA<ZC_uptr<ZC_Uniform>>(new ZC_uptr<ZC_Uniform>[]{ new ZC_UMatrix4fvValue(UName::unModel, 1, false) }, 1));
    } break;
    case Name::lineOrientation3D:
    {
        switch (vaoConFSVL)
        {
        case VAOConFSVL::F_4_0__UB_3_1: rSet.vaoConSets = { vaoConFSVL, VAOUFP().Pack(0).Pack(1) }; break;
        default: ZC_ErrorLogger::Err("There's no ZC_VAOConfig::FormatShVLayout for that shader!", __FILE__, __LINE__);
        }
    } break;
    case Name::quadOrientation3D:
    {
        switch (vaoConFSVL)
        {
        case VAOConFSVL::F_3_0__F_2_1: rSet.vaoConSets = { vaoConFSVL, VAOUFP().Pack(0).Pack(1) }; break;
        default: ZC_ErrorLogger::Err("There's no ZC_VAOConfig::FormatShVLayout for that shader!", __FILE__, __LINE__);
        }
        rSet.uniforms = std::move(ZC_DA<ZC_uptr<ZC_Uniform>>(new ZC_uptr<ZC_Uniform>[]{ new ZC_UMatrix4fvValue(UName::unModel, 1, false) }, 1));
    } break;
    case Name::textWindow:
    {
        switch (vaoConFSVL)
        {
        case VAOConFSVL::F_4_0: rSet.vaoConSets = { vaoConFSVL, VAOUFP().Pack(0) }; break;
        default: ZC_ErrorLogger::Err("There's no ZC_VAOConfig::FormatShVLayout for that shader!", __FILE__, __LINE__);
        }
    } break;
    }
}
