#pragma once

#include "ZC_ShProg.h"
#include <ZC/Video/OpenGL/Uniform/ZC_Uniforms.h>
#include <ZC/Video/OpenGL/Texture/ZC_TexSets.h>
#include "ZC_ShVertex.h"
#include "ZC_ShFragment.h"

#include <forward_list>

class ZC_ShProgs
{
public:
    enum Name
    {
        ZCR_Color,
        ZCR_Point,
        ZCR_Line,
        ZCR_Stencil,
        ZCR_Texture_Vertex_TexCoord,
        ZCR_Mesh,
    };

    typedef typename ZC_VAOConfig::FormatShVLayoutAndUsingFormatsPacker VAOConData;
    struct ShPInitSet
    {
        Name name;
        ZC_ShProg shProg;
        VAOConData vaoConData;
        ZC_Uniforms uniforms;
        ZC_TexSets texSets;

        ShPInitSet(Name _name, ZC_ShProg&& _shProg, VAOConData _vaoData,
            ZC_Uniforms&& _uNameLocs, ZC_TexSets&& pTexSets);
        ShPInitSet(ShPInitSet&& shPIS);

        bool operator == (Name _name) const noexcept;
    };

    void Load(Name* pShPName, size_t ShPNameCount);
    static ShPInitSet* Get(Name shNames);

private:
    static inline std::forward_list<ShPInitSet> shProgs;
    
    ZC_ShVertex shVertex;
    ZC_ShFragment shFragment;

    typedef typename ZC_ShVertex::Name VName;
    typedef typename ZC_ShFragment::Name FName;
    typedef typename ZC_VAOConfig::FormatShVLayout VAOConFSVL;
    struct ShNames
    {
        VName vName;
        VAOConFSVL vaoConFSVL;
        FName fName;
    };

    ShNames GetShNames(Name name) const noexcept;
};