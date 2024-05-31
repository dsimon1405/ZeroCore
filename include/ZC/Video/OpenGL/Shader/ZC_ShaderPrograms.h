#pragma once

#include <ZC/Tools/Container/ZC_DA.h>
#include "ZC_ShaderProgram.h"

#include <glad/glad.h>

#include <forward_list>
#include <string>

struct ZC_ShaderInput
{
    std::string path;
    int customID;   //  id wich set user, uses to find shader for pipeline creation
};

class ZC_ShaderPrograms
{
public:
    void Clear();

    ZC_ShaderProgram* GetShader(GLenum type, int customID);
    bool AddShaderProgram(GLenum type, const ZC_ShaderInput& shaderInput);
    //  takes pathes to files, read files, compile in shaders and link shader program. On success return true otherwise false, in Debug mode thow exaptions.
    static bool CompilationTest(const char* v, const char* tc, const char* te, const char* g, const char* f);

private:
    std::forward_list<ZC_ShaderProgram> vertex,
        tessControl,
        tessEvaluation,
        geometry,
        fragment;

    static ZC_DA<char> ReadShaderFile(const char* path);
    static bool CompileShader(GLuint& id, GLenum type, const char* code);
};