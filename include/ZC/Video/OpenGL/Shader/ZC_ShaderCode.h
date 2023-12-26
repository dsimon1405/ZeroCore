#pragma once

#include <ZC/Tools/Container/ZC_DynamicArray.h>

//  Struct keeping shader codes.
struct ZC_ShaderCode
{
    ZC_DynamicArray<char> vertexCode {};
    ZC_DynamicArray<char> fragmentCode {};
    ZC_DynamicArray<char> geometryCode {};

    ZC_ShaderCode() = default;
    ZC_ShaderCode(ZC_DynamicArray<char>&& _vertexCode, ZC_DynamicArray<char>&& _fragmentCode, ZC_DynamicArray<char>&& _geometryCode) noexcept;

    ZC_ShaderCode(const ZC_ShaderCode&) = delete;
    ZC_ShaderCode& operator = (const ZC_ShaderCode&) = delete;

    ZC_ShaderCode(ZC_ShaderCode&& shaderCode) noexcept;
    ZC_ShaderCode& operator = (ZC_ShaderCode&& shaderCode) noexcept;

    ~ZC_ShaderCode() = default;
};