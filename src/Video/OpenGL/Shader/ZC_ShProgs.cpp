#include <ZC/Video/OpenGL/Shader/ZC_ShProgs.h>

#include <Video/OpenGL/ZC_OpenGL.h>

#include <algorithm>

void ZC_ShProgs::Load(Name* pShPName, size_t shPNameCount)
{
    if (pShPName[0] == Name::LoadAll) shPNameCount = Name::LoadAll;
    for (size_t i = 0; i < shPNameCount; ++i)
    {
        ShNames shNames = GetShNames(pShPName[0] != Name::LoadAll ? pShPName[i] : static_cast<Name>(i));
        auto vertexData = shVertex.GetSet(shNames.vName, shNames.vaoConFSVL);
        auto fragmentData = shFragment.GetSet(shNames.fName);
        auto pGeometryShader = shGeometry.GetShader(shNames.gName);

        ZC_ShProg shProg(vertexData.shader->id, fragmentData.shader->id, pGeometryShader ? pGeometryShader->id : 0);

        shProg.UseProgram();
        
        std::vector<ZC_uptr<ZC_Uniform>> uniforms;
        uniforms.reserve(vertexData.uniforms.size + fragmentData.uniforms.size);
        for (size_t i = 0; i < vertexData.uniforms.size; ++i)
            uniforms.emplace_back(std::move(vertexData.uniforms[i]))->GetUniformLocation(shProg);
        for (size_t i = 0; i < fragmentData.uniforms.size; ++i)
            uniforms.emplace_back(std::move(fragmentData.uniforms[i]))->GetUniformLocation(shProg);
        
        fragmentData.texSets.Uniformli(shProg);     //  activate samplers

        shProgs.emplace_front(ShPInitSet(pShPName[0] != Name::LoadAll ? pShPName[i] : static_cast<Name>(i), std::move(shProg),
            vertexData.vaoConSets, ZC_Uniforms{ std::move(uniforms) }, std::move(fragmentData.texSets)));
    }
}

typename ZC_ShProgs::ShPInitSet* ZC_ShProgs::Get(Name name)
{
    auto shProgsI = std::find(shProgs.begin(), shProgs.end(), name);
    return shProgsI != shProgs.end() ? &(*shProgsI) : nullptr;
}

typename ZC_ShProgs::ShNames ZC_ShProgs::GetShNames(Name name) const noexcept
{
    switch (name)
    {
    case Name::ZCR_ColorFigure: return {VName::colorFigure, VAOConFSVL::F_3_0__UB_3_1__I_2_10_10_10_REV_1_2, FName::color, GName::none};
    case Name::ZCR_Point: return {VName::point, VAOConFSVL::F_3_0__UB_3_1__I_2_10_10_10_REV_1_2, FName::color, GName::none};
    case Name::ZCR_LineFigure: return {VName::lineFigure, VAOConFSVL::F_3_0__UB_3_1__I_2_10_10_10_REV_1_2, FName::color, GName::none};
    case Name::ZCR_Stencil: return {VName::stencil, VAOConFSVL::None, FName::color, GName::none};
    case Name::ZCR_Texture_Vertex_TexCoord: return {VName::texture, VAOConFSVL::F_3_0__F_2_3, FName::colorTex, GName::none};
    case Name::ZCR_LineMesh: return {VName::lineMesh, VAOConFSVL::F_3_0, FName::color, GName::none};
    case Name::ZCR_LineOrientation3D: return {VName::lineOrientation3D, VAOConFSVL::F_4_0__UB_3_1, FName::color, GName::lineOrientation3D};
    case Name::ZCR_QuadOrientation3D: return {VName::quadOrientation3D, VAOConFSVL::F_3_0__F_2_1, FName::colorTex, GName::none};
    case Name::ZC_TextWindow: return {VName::textWindow, VAOConFSVL::F_4_0, FName::text, GName::none};
    default: return {};
    }
}


//  ShPInitSet

ZC_ShProgs::ShPInitSet::ShPInitSet(Name _name, ZC_ShProg&& _shProg, VAOConData _vaoData,
    ZC_Uniforms&& _uNameLocs, ZC_TexSets&& _texSets)
    : name(_name),
    shProg(std::move(_shProg)),
    vaoConData(_vaoData),
    uniforms(std::move(_uNameLocs)),
    texSets(std::move(_texSets))
{}

ZC_ShProgs::ShPInitSet::ShPInitSet(ShPInitSet&& shPIS)
    : name(shPIS.name),
    shProg(std::move(shPIS.shProg)),
    vaoConData(shPIS.vaoConData),
    uniforms(std::move(shPIS.uniforms)),
    texSets(std::move(shPIS.texSets))
{}

bool ZC_ShProgs::ShPInitSet::operator == (Name _name) const noexcept
{
    return name == _name;
}