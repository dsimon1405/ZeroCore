#pragma once

#include "ZC_ShProg.h"
#include <ZC/Video/OpenGL/Uniform/ZC_Uniforms.h>
#include <ZC/Video/OpenGL/Texture/ZC_TexSets.h>
#include "ZC_ShVertex.h"
#include "ZC_ShFragment.h"
#include "ZC_ShGeometry.h"

#include <forward_list>

enum ZC_ShPName
{
    ShPN_ZCR_ColorFigure,
    ShPN_ZCR_Point,
    ShPN_ZCR_LineFigure,
    ShPN_ZCR_Stencil,
    ShPN_ZCR_Texture_Vertex_TexCoord,
    ShPN_ZCR_LineMesh,
    ShPN_ZCR_LineOrientation3D,
    ShPN_ZCR_QuadOrientation3D,
    ShPN_ZC_TextWindow,
    ShPN_ZC_TextScene,
    ShPN_ZC_TextWindowIntoScene,
    
    ShPN_LoadAll,    //  must be last
};

class ZC_ShProgs
{
public:
    typedef typename ZC_VAOConfig::ConfigData VAOConfigData;
    struct ShPInitSet
    {
        ZC_ShPName name;
        ZC_ShProg shProg;
        VAOConfigData vaoConfigData;
        ZC_Uniforms uniforms;
        ZC_TexSets texSets;

        ShPInitSet(ZC_ShPName _name, ZC_ShProg&& _shProg, VAOConfigData _vaoData,
            ZC_Uniforms&& _uNameLocs, ZC_TexSets&& pTexSets);
        ShPInitSet(ShPInitSet&& shPIS);

        bool operator == (ZC_ShPName _name) const noexcept;
    };

    void Load(ZC_ShPName* pShPName, size_t ShPNameCount);
    static ShPInitSet* Get(ZC_ShPName shNames);

private:
    static inline std::forward_list<ShPInitSet> shProgs;
    
    ZC_ShVertex shVertex;
    ZC_ShFragment shFragment;
    ZC_ShGeometry shGeometry;

    typedef typename ZC_ShVertex::Name VName;
    typedef typename ZC_ShFragment::Name FName;
    typedef typename ZC_ShGeometry::Name GName;
    struct ShNames
    {
        VName vName;
        VAOConfigData vaoConfigData;
        FName fName;
        GName gName;
    };

    ShNames GetShNames(ZC_ShPName name) const noexcept;
};