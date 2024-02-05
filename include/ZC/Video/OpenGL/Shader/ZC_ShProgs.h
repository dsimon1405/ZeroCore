#pragma once

#include "ZC_ShProg.h"
#include "ZC_ShLoader.h"
#include <ZC/Video/OpenGL/Uniform/ZC_Uniforms.h>
#include <ZC/Video/OpenGL/Texture/ZC_TexSets.h>

class ZC_ShProgs
{
public:
    enum Name
    {
        Color = 0,
        Point,
        Line,
        Stencil,

        none,
    };  //  Point must be last element

    struct ShPInitSet
    {
        struct UNameLoc
        {
            std::string name;
            GLint location;

            UNameLoc(std::string&& _name, GLint _location);
            UNameLoc(UNameLoc&& unl);
            bool operator == (const char* _name) const;
        };

        ZC_ShProg shProg;
        ZC_VAOConfig* pVaoCon;
        ZC_Uniforms uniforms;
        ZC_TexSets* pTexSets;

        ShPInitSet(ZC_ShProg&& _shProg, ZC_VAOConfig* _pVaoCon,
            ZC_Uniforms&& _uNameLocs, ZC_TexSets* pTexSets);
        ShPInitSet(ShPInitSet&& shPIS);
    };

    ZC_ShProgs(bool freeVaoConfigs = true);

    static ShPInitSet* Get(Name shNames);
    void Load(Name* pShPName, size_t ShPNameCount);

private:
    typedef typename ZC_ShLoader::Name ShName;
    struct ShNames
    {
        ShName vName;
        ShName fName;
        ShName gName;
    };

    struct ShPSet
    {
        Name name;
        ShPInitSet shPInitSet;

        ShPSet(Name _name, ShPInitSet&& _shPInitSet);
        ShPSet(ShPSet&& shPS);

        bool operator == (Name _name) const noexcept;
    };

    static inline std::forward_list<ShPSet> shProgs;

    ZC_VAOConfigs vaoConfigs;
    ZC_ShLoader shLoader {};
    static inline ShNames shaderNames[Name::none + 1]    //  Point last element
    {
        {ShName::colorV, ShName::colorF, ShName::none},
        {ShName::pointV, ShName::colorF, ShName::none},
        {ShName::lineV, ShName::colorF, ShName::none},
        {ShName::stencilV, ShName::colorF, ShName::none},
    };
};