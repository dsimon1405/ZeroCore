#pragma once

#include "ZC_ShProg.h"
#include <ZC/Video/OpenGL/Uniform/ZC_Uniforms.h>
#include <ZC/Video/OpenGL/Texture/ZC_TexSets.h>
#include "ZC_ShVertex.h"
#include "ZC_ShFragment.h"
#include "ZC_ShGeometry.h"

#include <forward_list>

class ZC_ShProgs
{
public:
    enum Name
    {
        ZCR_ColorFigure,
        ZCR_Point,
        ZCR_LineFigure,
        ZCR_Stencil,
        ZCR_Texture_Vertex_TexCoord,
        ZCR_LineMesh,
        ZCR_LineOrientation3D
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
    ZC_ShGeometry shGeometry;

    typedef typename ZC_ShVertex::Name VName;
    typedef typename ZC_ShFragment::Name FName;
    typedef typename ZC_ShGeometry::Name GName;
    typedef typename ZC_VAOConfig::FormatShVLayout VAOConFSVL;
    struct ShNames
    {
        VName vName;
        VAOConFSVL vaoConFSVL;
        FName fName;
        GName gName;
    };

    ShNames GetShNames(Name name) const noexcept;
};