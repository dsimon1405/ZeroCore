#pragma once

#include <glad/glad.h>

struct ZC_ShaderProgram
{
    GLuint id = 0;
    int customID = -1;    //  id wich set user, uses to find shader for pipeline creation

    ZC_ShaderProgram(GLenum type, const char* code, int _shaderEnum);
    ZC_ShaderProgram(ZC_ShaderProgram&& s);
    ~ZC_ShaderProgram();

    bool operator == (int _shaderEnum) const noexcept;
};