#include <ZC/Video/OpenGL/Shader/ZC_ShaderCode.h>

#include <utility>

ZC_ShaderCode::ZC_ShaderCode(ZC_DynamicArray<char>&& _vertexCode, ZC_DynamicArray<char>&& _fragmentCode, ZC_DynamicArray<char>&& _geometryCode) noexcept
        : vertexCode(std::move(_vertexCode)),
          fragmentCode(std::move(_fragmentCode)),
          geometryCode(std::move(_geometryCode))
{}

ZC_ShaderCode::ZC_ShaderCode(ZC_ShaderCode&& shaderCode) noexcept
        : ZC_ShaderCode(std::move(shaderCode.vertexCode), std::move(shaderCode.fragmentCode), std::move(shaderCode.geometryCode))
{}

ZC_ShaderCode& ZC_ShaderCode::operator = (ZC_ShaderCode&& shaderCode) noexcept
{
    vertexCode = std::move(shaderCode.vertexCode);
    fragmentCode = std::move(shaderCode.fragmentCode);
    geometryCode = std::move(shaderCode.geometryCode);
    return *this;
}