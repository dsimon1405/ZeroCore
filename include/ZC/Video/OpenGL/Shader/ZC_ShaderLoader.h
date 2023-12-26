#pragma once

#include <ZC_Config.h>
#include "ZC_ShaderCode.h"
#include <ZC/Tools/Container/ZC_DynamicArray.h>
#include <ZC/Tools/ZC_string.h>

//  Class for load shader code from files.
class ZC_ShaderLoader
{
public:
    enum ShaderType
    {
        Vertex,
        Fragment,
        Geometry
    };

    ZC_ShaderLoader() = delete;

    /*
    Load shader code from file.

    Params:
    name - shader name.
    vertexPath - path to the vertex shader file.
    fragmentPath - path to the fragment shader file.
    geometryPath - path to the geometry shader file.

    Return:
    If success ZC_ShaderCode with code data, otherwise ZC_ShaderCode will consider nullptr data (use ZC_ErrorLogger::WasError() for check)(ZC_ErrorLogger::ErrorMessage() - for more information).
    */
    static ZC_ShaderCode LoadShaderCode(const char* vertexPath, const char* fragmentPath, const char* geometryPath = nullptr);

    /*
    Read shader code from file.

    Params:
    path - the path to the file.
    shaderType - shader type to fill the shader header.

    Return:
    If success struct dynamic array with code, otherwise empty struct (ZC_ErrorLogger::ErrorMessage() - for more information).
    */
    static ZC_DynamicArray<char> ReadShaderFile(const char* path, ShaderType shaderType);

private:
    static void FillShaderStart(char* shaderData, const std::string& shaderStart) noexcept;
};