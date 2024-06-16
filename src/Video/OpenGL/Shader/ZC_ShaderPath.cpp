#include <ZC/Video/OpenGL/Shader/ZC_ShaderPath.h>

#include <cassert>

std::string ZC_ShaderPath::GetPath_Vertex(ZC_ShVertex shV)
{
    std::string path;
    switch (shV)
    {
    case ZC_SV_gui: return ZC_FSPath(shadersPath).append("GUI/gui.vs").string();
    default: assert(false); return {};
    }
}

std::string ZC_ShaderPath::GetPath_TessControl(ZC_ShTessControl shTC)
{
    return {};
}

std::string ZC_ShaderPath::GetPath_TessEvaluation(ZC_ShTessEvaluation shTE)
{
    return {};
}

std::string ZC_ShaderPath::GetPath_Geometry(ZC_ShGeometry shG)
{
    switch (shG)
    {
    case ZC_SG_gui: return ZC_FSPath(shadersPath).append("GUI/gui.gs").string();
    default: assert(false); return {};
    }
}

std::string ZC_ShaderPath::GetPath_Fragment(ZC_ShFragment shF)
{
    switch (shF)
    {
    case ZC_SF_gui: return ZC_FSPath(shadersPath).append("GUI/gui.fs").string();
    default: assert(false); return {};
    }
}