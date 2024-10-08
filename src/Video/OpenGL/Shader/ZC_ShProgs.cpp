#include <ZC/Video/OpenGL/Shader/ZC_ShProgs.h>

#include <ZC/Tools/Container/ZC_ContFunc.h>

void ZC_ShProgs::Load(ZC_ShPName* pShPName, ulong shPNameCount)
{
    if (pShPName[0] == ShPN_LoadAll) shPNameCount = ShPN_LoadAll;
    for (ulong i = 0; i < shPNameCount; ++i)
    {
        ShNames shNames = GetShNames(pShPName[0] != ShPN_LoadAll ? pShPName[i] : static_cast<ZC_ShPName>(i));
        auto vertexData = shVertex.GetSet(shNames.vName);
        auto fragmentData = shFragment.GetSet(shNames.fName);
        auto pGeometryShader = shGeometry.GetShader(shNames.gName);

        ZC_ShProg shProg(vertexData.shader->id, fragmentData.shader->id, pGeometryShader ? pGeometryShader->id : 0);

        shProg.ActivateOpenGL();
        
        std::vector<ZC_uptr<ZC_Uniform>> uniforms;
        uniforms.reserve(vertexData.uniforms.size() + fragmentData.uniforms.size());
        for (ulong i = 0; i < vertexData.uniforms.size(); ++i)
            uniforms.emplace_back(std::move(vertexData.uniforms[i]))->GetUniformLocation(shProg);
        for (ulong i = 0; i < fragmentData.uniforms.size(); ++i)
            uniforms.emplace_back(std::move(fragmentData.uniforms[i]))->GetUniformLocation(shProg);
        
        fragmentData.texSets.Uniformli(shProg);     //  activate samplers

        shProgs.emplace_front(ShPInitSet(pShPName[0] != ShPN_LoadAll ? pShPName[i] : static_cast<ZC_ShPName>(i), std::move(shProg),
            shNames.vaoConfigData, ZC_Uniforms{ std::move(uniforms) }, std::move(fragmentData.texSets)));
    }
}

typename ZC_ShProgs::ShPInitSet* ZC_ShProgs::Get(ZC_ShPName name)
{
    return ZC_Find(shProgs, name);
}

typename ZC_ShProgs::ShNames ZC_ShProgs::GetShNames(ZC_ShPName name) const noexcept    //  add here new
{
    typedef ZC_VAOConfig::LayoutPacker VAOPack;
    switch (name)
    {
    case ShPN_ZCR_ColorFigure: return { VName::colorFigure, { ZC_VAOL__F_3_0__UB_3_1_N__I_2_10_10_10_REV_1_2_N, VAOPack(0).Pack(2) }, FName::color, GName::none };
    case ShPN_ZCR_Point: return { VName::point, { ZC_VAOL__F_3_0__UB_3_1_N__I_2_10_10_10_REV_1_2_N, VAOPack(0).Pack(1) }, FName::color, GName::none };
    case ShPN_ZCR_LineFigure: return { VName::lineFigure, { ZC_VAOL__F_3_0__UB_3_1_N__I_2_10_10_10_REV_1_2_N, VAOPack(0).Pack(1) }, FName::color, GName::none };
    case ShPN_ZCR_StencilBorder: return { VName::stencilBorder, { ZC_VAOL__None }, FName::color, GName::none };
    case ShPN_ZCR_Texture_Vertex_TexCoord: return { VName::texture_Vertex_TexCoord, { ZC_VAOL__F_3_0__F_2_1, VAOPack(0).Pack(1) }, FName::colorTex, GName::none };
    case ShPN_ZCR_LineMesh: return { VName::lineMesh, { ZC_VAOL__F_3_0__UB_3_1_N, VAOPack(0).Pack(1) }, FName::color, GName::none };
    case ShPN_ZC_TextWindow: return { VName::textWindow, { ZC_VAOL__F_2_0__US_2_1_N, VAOPack(0).Pack(1) }, FName::textWindow, GName::none };
    case ShPN_ZC_TextScene: return { VName::textScene, { ZC_VAOL__F_2_0__US_2_1_N, VAOPack(0).Pack(1) }, FName::text, GName::none };
    case ShPN_ZC_TextWindowIntoScene: return { VName::textWindowIntoScene, { ZC_VAOL__F_2_0__US_2_1_N, VAOPack(0).Pack(1) }, FName::text, GName::none };
    case ShPN_ZC_OrthoTexture: return { VName::textWindow, { ZC_VAOL__F_2_0__US_2_1_N, VAOPack(0).Pack(1) }, FName::orthoTexture, GName::none };

    case ShPN_Game_PlayerSphere: return ShNames{ .vName = VName::Game_PlayerSphere, .vaoConfigData = VAOConfigData{ .formatShVLayout = ZC_VAOL__F_3_0__I_2_10_10_10_REV_1_1_N__US_2_2_N,
        .usingFormatsPacker = VAOPack(0).Pack(1).Pack(2) }, .fName = FName::game_sphere, .gName = GName::none };
    default: return {};
    }
}


//  ShPInitSet

ZC_ShProgs::ShPInitSet::ShPInitSet(ZC_ShPName _name, ZC_ShProg&& _shProg, VAOConfigData _vaoData,
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

bool ZC_ShProgs::ShPInitSet::operator == (ZC_ShPName _name) const noexcept
{
    return name == _name;
}