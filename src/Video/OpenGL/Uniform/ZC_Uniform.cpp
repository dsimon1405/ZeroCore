#include <ZC/Video/OpenGL/Uniform/ZC_Uniform.h>

#include <ZC/Video/OpenGL/ZC_OpenGL.h>
#include <ZC/Tools/Math/ZC_Mat4.h>
#include <ZC/Tools/Math/ZC_Vec2.h>

ZC_Uniform::ZC_Uniform(ZC_UniformName _name, FunctionType _fucntionType)
    : name(_name),
    functionType(_fucntionType)
{}

void ZC_Uniform::GetUniformLocation(ZC_ShProg& shP)  //  add here new
{
    switch (name)
    {
        case ZC_UN_unModel: location = shP.GetUniformLocation("unModel"); break;
        case ZC_UN_unColor: location = shP.GetUniformLocation("unColor"); break;
        case ZC_UN_unPositionWindow: location = shP.GetUniformLocation("unPositionWindow"); break;
        case ZC_UN_unPositionScene: location = shP.GetUniformLocation("unPositionScene"); break;
        case ZC_UN_unAlpha: location = shP.GetUniformLocation("unAlpha"); break;
        case ZCR_UN_unUseLight: location = shP.GetUniformLocation("unUseLight"); break;
    }
}

std::vector<ZC_uptr<ZC_Uniform>> ZC_Uniform::GetUniformVector(typename ZC_Uniform::NameType nameType)
{
    return GetUniformVector(&nameType, 1);
}

std::vector<ZC_uptr<ZC_Uniform>> ZC_Uniform::GetUniformVector(typename ZC_Uniform::NameType* pNameType, ulong nameTypeCount)  //  add here new
{
    std::vector<ZC_uptr<ZC_Uniform>> uniforms;
    uniforms.reserve(nameTypeCount);
    for (ulong i = 0; i < nameTypeCount; ++i)
    {
        switch (pNameType[i].name)
        {
            case ZC_UN_unModel: uniforms.emplace_back(GetUpUniform(FT_glUniformMatrix4fv, pNameType[i])); break;
            case ZC_UN_unColor: uniforms.emplace_back(GetUpUniform(FT_glUniform1ui, pNameType[i])); break;
            case ZC_UN_unPositionWindow: uniforms.emplace_back(GetUpUniform(FT_glUniform2fv, pNameType[i])); break;
            case ZC_UN_unPositionScene: uniforms.emplace_back(GetUpUniform(FT_glUniform3fv, pNameType[i])); break;
            case ZC_UN_unAlpha: uniforms.emplace_back(GetUpUniform(FT_glUniform1f, pNameType[i])); break;
            case ZCR_UN_unUseLight: uniforms.emplace_back(GetUpUniform(FT_glUniform1i, pNameType[i])); break;
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

ZC_uptr<ZC_Uniform> ZC_Uniform::GetUpUniform(FunctionType functionType, NameType& nameType)
{
    switch (functionType)
    {
    case FT_glUniform1f: return nameType.isPointer ? ZC_uptrMakeFromChild<ZC_Uniform, ZC_UniformData<float*>>(nameType.name, functionType)
                : ZC_uptrMakeFromChild<ZC_Uniform, ZC_UniformData<float>>(nameType.name, functionType);
    case FT_glUniform1i: return nameType.isPointer ? ZC_uptrMakeFromChild<ZC_Uniform, ZC_UniformData<int*>>(nameType.name, functionType)
                : ZC_uptrMakeFromChild<ZC_Uniform, ZC_UniformData<int>>(nameType.name, functionType);
    case FT_glUniform1ui: return nameType.isPointer ? ZC_uptrMakeFromChild<ZC_Uniform, ZC_UniformData<uint*>>(nameType.name, functionType)
                : ZC_uptrMakeFromChild<ZC_Uniform, ZC_UniformData<uint>>(nameType.name, functionType);
    // case FT_glUniform1fv: return nameType.isPointer ? ZC_uptrMakeFromChild<ZC_Uniform, ZC_UDCount<float*>>(nameType.name, functionType, nameType.count)  //  don't have .Begin()!
    //             : ZC_uptrMakeFromChild<ZC_Uniform, ZC_UDCount<float>>(nameType.name, functionType, nameType.count);
    case FT_glUniform2fv: return nameType.isPointer ? ZC_uptrMakeFromChild<ZC_Uniform, ZC_UDCount<ZC_Vec2<float>*>>(nameType.name, functionType, nameType.count)
                : ZC_uptrMakeFromChild<ZC_Uniform, ZC_UDCount<ZC_Vec2<float>>>(nameType.name, functionType, nameType.count);
    case FT_glUniform3fv: return nameType.isPointer ? ZC_uptrMakeFromChild<ZC_Uniform, ZC_UDCount<ZC_Vec3<float>*>>(nameType.name, functionType, nameType.count)
                : ZC_uptrMakeFromChild<ZC_Uniform, ZC_UDCount<ZC_Vec3<float>>>(nameType.name, functionType, nameType.count);
    case FT_glUniform4fv: return nameType.isPointer ? ZC_uptrMakeFromChild<ZC_Uniform, ZC_UDCount<ZC_Vec4<float>*>>(nameType.name, functionType, nameType.count)
                : ZC_uptrMakeFromChild<ZC_Uniform, ZC_UDCount<ZC_Vec4<float>>>(nameType.name, functionType, nameType.count);
    // case FT_glUniform1iv: return nameType.isPointer ? ZC_uptrMakeFromChild<ZC_Uniform, ZC_UDCount<int*>>(nameType.name, functionType, nameType.count)
    //             : ZC_uptrMakeFromChild<ZC_Uniform, ZC_UDCount<int>>(nameType.name, functionType, nameType.count);
    case FT_glUniform2iv: return nameType.isPointer ? ZC_uptrMakeFromChild<ZC_Uniform, ZC_UDCount<ZC_Vec2<int>*>>(nameType.name, functionType, nameType.count)
                : ZC_uptrMakeFromChild<ZC_Uniform, ZC_UDCount<ZC_Vec2<int>>>(nameType.name, functionType, nameType.count);
    case FT_glUniform3iv: return nameType.isPointer ? ZC_uptrMakeFromChild<ZC_Uniform, ZC_UDCount<ZC_Vec3<int>*>>(nameType.name, functionType, nameType.count)
                : ZC_uptrMakeFromChild<ZC_Uniform, ZC_UDCount<ZC_Vec3<int>>>(nameType.name, functionType, nameType.count);
    case FT_glUniform4iv: return nameType.isPointer ? ZC_uptrMakeFromChild<ZC_Uniform, ZC_UDCount<ZC_Vec4<int>*>>(nameType.name, functionType, nameType.count)
                : ZC_uptrMakeFromChild<ZC_Uniform, ZC_UDCount<ZC_Vec4<int>>>(nameType.name, functionType, nameType.count);
    // case FT_glUniform1uiv: return nameType.isPointer ? ZC_uptrMakeFromChild<ZC_Uniform, ZC_UDCount<uint*>>(nameType.name, functionType, nameType.count)
    //             : ZC_uptrMakeFromChild<ZC_Uniform, ZC_UDCount<uint>>(nameType.name, functionType, nameType.count);
    case FT_glUniform2uiv: return nameType.isPointer ? ZC_uptrMakeFromChild<ZC_Uniform, ZC_UDCount<ZC_Vec2<uint>*>>(nameType.name, functionType, nameType.count)
                : ZC_uptrMakeFromChild<ZC_Uniform, ZC_UDCount<ZC_Vec2<uint>>>(nameType.name, functionType, nameType.count);
    case FT_glUniform3uiv: return nameType.isPointer ? ZC_uptrMakeFromChild<ZC_Uniform, ZC_UDCount<ZC_Vec3<uint>*>>(nameType.name, functionType, nameType.count)
                : ZC_uptrMakeFromChild<ZC_Uniform, ZC_UDCount<ZC_Vec3<uint>>>(nameType.name, functionType, nameType.count);
    case FT_glUniform4uiv: return nameType.isPointer ? ZC_uptrMakeFromChild<ZC_Uniform, ZC_UDCount<ZC_Vec4<uint>*>>(nameType.name, functionType, nameType.count)
                : ZC_uptrMakeFromChild<ZC_Uniform, ZC_UDCount<ZC_Vec4<uint>>>(nameType.name, functionType, nameType.count);
    // case FT_glUniformMatrix2fv: 
    // case FT_glUniformMatrix3fv: 
    case FT_glUniformMatrix4fv: return nameType.isPointer ? ZC_uptrMakeFromChild<ZC_Uniform, ZC_UDCTransponse<ZC_Mat4<float>*>>(nameType.name, functionType, nameType.count, nameType.transponse)
                : ZC_uptrMakeFromChild<ZC_Uniform, ZC_UDCTransponse<ZC_Mat4<float>>>(nameType.name, functionType, nameType.count, nameType.transponse);
    // case FT_glUniformMatrix2x3fv: 
    // case FT_glUniformMatrix3x2fv: 
    // case FT_glUniformMatrix2x4fv: 
    // case FT_glUniformMatrix4x2fv: 
    // case FT_glUniformMatrix3x4fv: 
    // case FT_glUniformMatrix4x3fv: 
    default: return {};
    }
}