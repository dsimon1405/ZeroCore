#include <ZC/Video/OpenGL/Shader/ZC_ShProgs.h>

#include <ZC/Tools/Container/ZC_ContFunc.h>

void ZC_ShProgs::Load(Name* pShPName, size_t shPNameCount)
{
    if (pShPName[0] == Name::LoadAll) shPNameCount = Name::LoadAll;
    for (size_t i = 0; i < shPNameCount; ++i)
    {
        ShNames shNames = GetShNames(pShPName[0] != Name::LoadAll ? pShPName[i] : static_cast<Name>(i));
        auto vertexData = shVertex.GetSet(shNames.vName);
        auto fragmentData = shFragment.GetSet(shNames.fName);
        auto pGeometryShader = shGeometry.GetShader(shNames.gName);

        ZC_ShProg shProg(vertexData.shader->id, fragmentData.shader->id, pGeometryShader ? pGeometryShader->id : 0);

        shProg.ActivateOpenGL();
        
        std::vector<ZC_uptr<ZC_Uniform>> uniforms;
        uniforms.reserve(vertexData.uniforms.size() + fragmentData.uniforms.size());
        for (size_t i = 0; i < vertexData.uniforms.size(); ++i)
            uniforms.emplace_back(std::move(vertexData.uniforms[i]))->GetUniformLocation(shProg);
        for (size_t i = 0; i < fragmentData.uniforms.size(); ++i)
            uniforms.emplace_back(std::move(fragmentData.uniforms[i]))->GetUniformLocation(shProg);
        
        fragmentData.texSets.Uniformli(shProg);     //  activate samplers

        shProgs.emplace_front(ShPInitSet(pShPName[0] != Name::LoadAll ? pShPName[i] : static_cast<Name>(i), std::move(shProg),
            shNames.vaoConfigData, ZC_Uniforms{ std::move(uniforms) }, std::move(fragmentData.texSets)));
    }
}

typename ZC_ShProgs::ShPInitSet* ZC_ShProgs::Get(Name name)
{
    return ZC_Find(shProgs, name);
}

typename ZC_ShProgs::ShNames ZC_ShProgs::GetShNames(Name name) const noexcept
{
    switch (name)
    {
    case ZCR_ColorFigure:
    {
        uchar layoutUsed[] { 0, 2 };
        return {VName::colorFigure, ZC_VAOConfig::CreateConfig(ZC_VAOL_F_3_0__UB_3_1_N__I_2_10_10_10_REV_1_2_N, layoutUsed, 2), FName::color, GName::none};
    }
    case ZCR_Point:
    {
        uchar layoutUsed[] { 0, 1 };
        return {VName::point, ZC_VAOConfig::CreateConfig(ZC_VAOL_F_3_0__UB_3_1_N__I_2_10_10_10_REV_1_2_N, layoutUsed, 2), FName::color, GName::none};
    }
    case ZCR_LineFigure:
    {
        uchar layoutUsed[] { 0, 1 };
        return {VName::lineFigure, ZC_VAOConfig::CreateConfig(ZC_VAOL_F_3_0__UB_3_1_N__I_2_10_10_10_REV_1_2_N, layoutUsed, 2), FName::color, GName::none};
    }
    case ZCR_Stencil: return {VName::stencil, ZC_VAOConfig::CreateConfig(ZC_VAOL_None, nullptr, 0), FName::color, GName::none};
    case ZCR_Texture_Vertex_TexCoord:
    {
        uchar layoutUsed[] { 0, 1 };
        return {VName::texture, ZC_VAOConfig::CreateConfig(ZC_VAOL_F_3_0__F_2_3, layoutUsed, 2), FName::colorTex, GName::none};
    }
    case ZCR_LineMesh:
    {
        uchar layoutUsed[] { 0 };
        return {VName::lineMesh, ZC_VAOConfig::CreateConfig(ZC_VAOL_F_3_0, layoutUsed, 1), FName::color, GName::none};
    }
    case ZCR_LineOrientation3D:
    {
        uchar layoutUsed[] { 0, 1 };
        return {VName::lineOrientation3D, ZC_VAOConfig::CreateConfig(ZC_VAOL_F_4_0__UB_3_1_N, layoutUsed, 2), FName::color, GName::lineOrientation3D};
    }
    case ZCR_QuadOrientation3D:
    {
        uchar layoutUsed[] { 0 , 1 };
        return {VName::quadOrientation3D, ZC_VAOConfig::CreateConfig(ZC_VAOL_F_3_0__F_2_1, layoutUsed, 2), FName::colorTex, GName::none};
    }
    case ZC_TextWindow:
    {
        uchar layoutUsed[] { 0, 1 };
        return {VName::textWindow, ZC_VAOConfig::CreateConfig(ZC_VAOL_F_2_0__US_2_1_N, layoutUsed, 2), FName::text, GName::none};
    }
    case ZC_TextScene:
    {
        uchar layoutUsed[] { 0, 1 };
        return {VName::textScene, ZC_VAOConfig::CreateConfig(ZC_VAOL_F_2_0__US_2_1_N, layoutUsed, 2), FName::text, GName::none};
    }
    default: return {};
    }
}


//  ShPInitSet

ZC_ShProgs::ShPInitSet::ShPInitSet(Name _name, ZC_ShProg&& _shProg, VAOConfigData _vaoData,
    ZC_Uniforms&& _uNameLocs, ZC_TexSets&& _texSets)
    : name(_name),
    shProg(std::move(_shProg)),
    vaoConfigData(_vaoData),
    uniforms(std::move(_uNameLocs)),
    texSets(std::move(_texSets))
{}

ZC_ShProgs::ShPInitSet::ShPInitSet(ShPInitSet&& shPIS)
    : name(shPIS.name),
    shProg(std::move(shPIS.shProg)),
    vaoConfigData(shPIS.vaoConfigData),
    uniforms(std::move(shPIS.uniforms)),
    texSets(std::move(shPIS.texSets))
{}

bool ZC_ShProgs::ShPInitSet::operator == (Name _name) const noexcept
{
    return name == _name;
}