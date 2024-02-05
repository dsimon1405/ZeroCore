#include <ZC/Video/OpenGL/Shader/ZC_ShProgs.h>

#include <Video/OpenGL/ZC_OpenGL.h>

ZC_ShProgs::ZC_ShProgs(bool freeVaoConfigs)
    : vaoConfigs(freeVaoConfigs)
{}

typename ZC_ShProgs::ShPInitSet* ZC_ShProgs::Get(Name name)
{
    auto shProgsI = std::find(shProgs.begin(), shProgs.end(), name);
    return shProgsI != shProgs.end() ? &shProgsI->shPInitSet : nullptr;
}

void ZC_ShProgs::Load(Name* pShPName, size_t ShPNameCount)
{
    for (size_t i = 0; i < ShPNameCount; ++i)
    {
        ShNames shNames = shaderNames[pShPName[i]];
        auto shData = shLoader.Get(shNames.vName, shNames.fName, shNames.gName);
        ZC_ShProg shProg(shData.pV->id, shData.pF->id, shData.pG ? shData.pG->id : 0);

        shProg.Use();
        
        std::vector<ZC_uptr<ZC_Uniform>> uniforms;
        uniforms.reserve(shData.pUniformsV->size + shData.pUniformsF->size);
        for (size_t i = 0; i < shData.pUniformsV->size; ++i)
            uniforms.emplace_back((*shData.pUniformsV)[i]->GetCopy())->GetUniformLocation(shProg);
        for (size_t i = 0; i < shData.pUniformsF->size; ++i)
            uniforms.emplace_back((*shData.pUniformsF)[i]->GetCopy())->GetUniformLocation(shProg);
        
        if (shData.pTexSets) shData.pTexSets->UniformI(shProg); //  activate samplers

        shProgs.emplace_front(ShPSet(pShPName[i], ShPInitSet(std::move(shProg), shData.pVaoCon, ZC_Uniforms{ std::move(uniforms) }, shData.pTexSets)));
    }
}


//  ShPInitSet start

ZC_ShProgs::ShPInitSet::ShPInitSet(ZC_ShProg&& _shProg, ZC_VAOConfig* _pVaoCon,
    ZC_Uniforms&& _uNameLocs, ZC_TexSets* _pTexSets)
    : shProg(std::move(_shProg)),
    pVaoCon(_pVaoCon),
    uniforms(std::move(_uNameLocs)),
    pTexSets(_pTexSets)
{}

ZC_ShProgs::ShPInitSet::ShPInitSet(ShPInitSet&& shPIS)
    : shProg(std::move(shPIS.shProg)),
    pVaoCon(shPIS.pVaoCon),
    uniforms(std::move(shPIS.uniforms)),
    pTexSets(shPIS.pTexSets)
{}


//  UNameLoc start

ZC_ShProgs::ShPInitSet::UNameLoc::UNameLoc(std::string&& _name, GLint _location)
    : name(std::move(_name)),
    location(_location)
{}

ZC_ShProgs::ShPInitSet::UNameLoc::UNameLoc(UNameLoc&& unl)
    : name(std::move(unl.name)),
    location(unl.location)
{}

bool ZC_ShProgs::ShPInitSet::UNameLoc::operator == (const char* _name) const
{
    return name == _name;
}


//  ShPSet start

ZC_ShProgs::ShPSet::ShPSet(Name _name, ShPInitSet&& _shPInitSet)
    : name(_name),
    shPInitSet(std::move(_shPInitSet))
{}

ZC_ShProgs::ShPSet::ShPSet(ShPSet&& shPS)
    : name(shPS.name),
    shPInitSet(std::move(shPS.shPInitSet))
{}

bool ZC_ShProgs::ShPSet::operator == (Name _name) const noexcept
{
    return name == _name;
}