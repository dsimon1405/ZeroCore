#pragma once

#include <ZC_Config.h>
#include "ZC_ShaderCode.h"
#include <ZC/Tools/Container/ZC_DynamicArray.h>
#include <ZC/Tools/ZC_string.h>

#include <filesystem>
#include <map>

enum ZC_ShaderPath
{
    testVS,
    testFS
};

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
    static ZC_ShaderCode LoadShaderCode(const std::string& vertexPath, const std::string& fragmentPath, const std::string& geometryPath = "");
    static ZC_ShaderCode LoadShaderCode(const char* vertexPath, const char* fragmentPath, const char* geometryPath = nullptr);
    static ZC_ShaderCode LoadShaderCode(ZC_ShaderPath vertexPath, ZC_ShaderPath fragmentPath, ZC_ShaderPath geometryPath);
    static ZC_ShaderCode LoadShaderCode(ZC_ShaderPath vertexPath, ZC_ShaderPath fragmentPath);

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
    static const char* GetPath(ZC_ShaderPath code);

    static inline std::map<ZC_ShaderPath,std::string> shCodePaths
    {
        { ZC_ShaderPath::testVS, std::filesystem::current_path().append("assets").append("shaders").append("ZC_test.vs").string() },
        { ZC_ShaderPath::testFS, std::filesystem::current_path().append("assets").append("shaders").append("ZC_test.fs").string() }
    };
};