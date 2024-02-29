#include <ZC/Video/OpenGL/Shader/ZC_ShProgs.h>

#include <Video/OpenGL/ZC_OpenGL.h>

void ZC_ShProgs::Load(Name* pShPName, size_t ShPNameCount)
{
    for (size_t i = 0; i < ShPNameCount; ++i)
    {
        ShNames shNames = GetShNames(pShPName[i]);
        auto vertexData = shVertex.GetSet(shNames.vName, shNames.vaoConFSVL);
        auto fragmentData = shFragment.GetSet(shNames.fName);

        ZC_ShProg shProg(vertexData.shader->id, fragmentData.shader->id, 0);

        shProg.UseProgram();
        
        std::vector<ZC_uptr<ZC_Uniform>> uniforms;
        uniforms.reserve(vertexData.uniforms.size + fragmentData.uniforms.size);
        for (size_t i = 0; i < vertexData.uniforms.size; ++i)
            uniforms.emplace_back(std::move(vertexData.uniforms[i]))->GetUniformLocation(shProg);
        for (size_t i = 0; i < fragmentData.uniforms.size; ++i)
            uniforms.emplace_back(std::move(fragmentData.uniforms[i]))->GetUniformLocation(shProg);
        
        fragmentData.texSets.Uniformli(shProg);     //  activate samplers

        shProgs.emplace_front(ShPInitSet(pShPName[i], std::move(shProg),
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
    case Name::ZCR_Color: return {VName::color, VAOConFSVL::F_3_0__UB_3_1__I_2_10_10_10_REV_1_2, FName::color};
    case Name::ZCR_Point: return {VName::point, VAOConFSVL::F_3_0__UB_3_1__I_2_10_10_10_REV_1_2, FName::color};
    case Name::ZCR_Line: return {VName::line, VAOConFSVL::F_3_0__UB_3_1__I_2_10_10_10_REV_1_2, FName::color};
    case Name::ZCR_Stencil: return {VName::stencil, VAOConFSVL::None, FName::color};
    case Name::ZCR_Texture_Vertex_TexCoord: return {VName::texture, VAOConFSVL::F_3_0__F_2_3, FName::texture};
    case Name::ZCR_Mesh: return {VName::mesh, VAOConFSVL::F_3_0, FName::color};
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