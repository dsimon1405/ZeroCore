#include <ZC/Video/OpenGL/Uniform/ZC_Uniform.h>

#include <ZC/Video/OpenGL/ZC_OpenGL.h>
#include <ZC/Tools/Math/ZC_Mat4.h>
#include <ZC/Tools/Math/ZC_Vec2.h>

ZC_Uniform::ZC_Uniform(ZC_UniformName _name)
    : name(_name)
{
    switch (name)
    {
        case ZC_UN_unModel: functionType = FT_glUniformMatrix4fv; break;
        case ZC_UN_unColor: functionType = FT_glUniform1ui; break;
        case ZC_UN_unPosition: functionType = FT_glUniform2fv; break;
    }
}

void ZC_Uniform::GetUniformLocation(ZC_ShProg& shP)
{
    switch (name)
    {
        case ZC_UN_unModel: location = shP.GetUniformLocation("unModel"); break;
        case ZC_UN_unColor: location = shP.GetUniformLocation("unColor"); break;
        case ZC_UN_unPosition: location = shP.GetUniformLocation("unPosition"); break;
    }
}

std::vector<ZC_uptr<ZC_Uniform>> ZC_Uniform::GetUniformsDA(typename ZC_Uniform::NameType* pNameType, size_t nameTypeCount)
{
    std::vector<ZC_uptr<ZC_Uniform>> uniforms;
    uniforms.reserve(nameTypeCount);
    for (size_t i = 0; i < nameTypeCount; ++i)
    {
        switch (pNameType[i].name)
        {
            case ZC_UN_unModel: uniforms.emplace_back(pNameType[i].isPointer ?
                ZC_uptrMakeFromChild<ZC_Uniform, ZC_UDCTransponse<ZC_Mat4<float>*>>(pNameType[i].name, pNameType[i].count, pNameType[i].transponse)
                : ZC_uptrMakeFromChild<ZC_Uniform, ZC_UDCTransponse<ZC_Mat4<float>>>(pNameType[i].name, pNameType[i].count, pNameType[i].transponse)); break;
            case ZC_UN_unColor: uniforms.emplace_back(pNameType[i].isPointer ?
                ZC_uptrMakeFromChild<ZC_Uniform, ZC_UniformData<uint*>>(pNameType[i].name)
                : ZC_uptrMakeFromChild<ZC_Uniform, ZC_UniformData<uint>>(pNameType[i].name)); break;
            case ZC_UN_unPosition: uniforms.emplace_back(pNameType[i].isPointer ?
                ZC_uptrMakeFromChild<ZC_Uniform, ZC_UDCount<ZC_Vec2<float>*>>(pNameType[i].name, pNameType[i].count)
                : ZC_uptrMakeFromChild<ZC_Uniform, ZC_UDCount<ZC_Vec2<float>>>(pNameType[i].name, pNameType[i].count)); break;
        }
    }
    return uniforms;
}

void ZC_Uniform::GLUniform(const void* pData, int count, bool transponse) const
{
    switch (functionType)
    {
    case FT_glUniform1f: glUniform1f(location, *static_cast<const float*>(pData)); break;
    case FT_glUniform1i: glUniform1i(location, *static_cast<const int*>(pData)); break;
    case FT_glUniform1ui: glUniform1ui(location, *static_cast<const uint*>(pData)); break;
    case FT_glUniform1fv: glUniform1fv(location, count, static_cast<const float*>(pData)); break;
    case FT_glUniform2fv: glUniform2fv(location, count, static_cast<const float*>(pData)); break;
    case FT_glUniform3fv: glUniform3fv(location, count, static_cast<const float*>(pData)); break;
    case FT_glUniform4fv: glUniform4fv(location, count, static_cast<const float*>(pData)); break;
    case FT_glUniform1iv: glUniform1iv(location, count, static_cast<const int*>(pData)); break;
    case FT_glUniform2iv: glUniform2iv(location, count, static_cast<const int*>(pData)); break;
    case FT_glUniform3iv: glUniform3iv(location, count, static_cast<const int*>(pData)); break;
    case FT_glUniform4iv: glUniform4iv(location, count, static_cast<const int*>(pData)); break;
    case FT_glUniform1uiv: glUniform1uiv(location, count, static_cast<const uint*>(pData)); break;
    case FT_glUniform2uiv: glUniform2uiv(location, count, static_cast<const uint*>(pData)); break;
    case FT_glUniform3uiv: glUniform3uiv(location, count, static_cast<const uint*>(pData)); break;
    case FT_glUniform4uiv: glUniform4uiv(location, count, static_cast<const uint*>(pData)); break;
    case FT_glUniformMatrix2fv: glUniformMatrix2fv(location, count, transponse, static_cast<const float*>(pData)); break;
    case FT_glUniformMatrix3fv: glUniformMatrix3fv(location, count, transponse, static_cast<const float*>(pData)); break;
    case FT_glUniformMatrix4fv: glUniformMatrix4fv(location, count, transponse, static_cast<const float*>(pData)); break;
    case FT_glUniformMatrix2x3fv: glUniformMatrix2x3fv(location, count, transponse, static_cast<const float*>(pData)); break;
    case FT_glUniformMatrix3x2fv: glUniformMatrix3x2fv(location, count, transponse, static_cast<const float*>(pData)); break;
    case FT_glUniformMatrix2x4fv: glUniformMatrix2x4fv(location, count, transponse, static_cast<const float*>(pData)); break;
    case FT_glUniformMatrix4x2fv: glUniformMatrix4x2fv(location, count, transponse, static_cast<const float*>(pData)); break;
    case FT_glUniformMatrix3x4fv: glUniformMatrix3x4fv(location, count, transponse, static_cast<const float*>(pData)); break;
    case FT_glUniformMatrix4x3fv: glUniformMatrix4x3fv(location, count, transponse, static_cast<const float*>(pData)); break;
    }
}