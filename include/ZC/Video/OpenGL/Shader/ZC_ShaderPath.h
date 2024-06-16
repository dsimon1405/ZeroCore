#pragma once

#include <ZC/File/ZC_File.h>

enum ZC_ShVertex
{
    ZC_SV_gui = 1000000,
};

enum ZC_ShTessControl
{
};

enum ZC_ShTessEvaluation
{
};

enum ZC_ShGeometry
{
    ZC_SG_gui = 1000000,
};

enum ZC_ShFragment
{
    ZC_SF_gui = 1000000,
};


class ZC_ShaderPath  //  class for loading all shaders
{
public:
    static std::string GetPath_Vertex(ZC_ShVertex shV);
    static std::string GetPath_TessControl(ZC_ShTessControl shTC);
    static std::string GetPath_TessEvaluation(ZC_ShTessEvaluation shTE);
    static std::string GetPath_Geometry(ZC_ShGeometry shG);
    static std::string GetPath_Fragment(ZC_ShFragment shF);

private:
    static inline const ZC_FSPath shadersPath = ZC_FSPath(ZC_ZCDirPath).append("shaders");
};