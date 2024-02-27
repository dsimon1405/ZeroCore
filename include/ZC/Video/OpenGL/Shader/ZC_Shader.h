#pragma once

#include <ZC/Video/OpenGL/GL/glcorearb.h>
#include <ZC/Tools/Container/ZC_DA.h>

struct ZC_Shader
{
    GLuint id;
    
    ZC_Shader(const char* code, GLenum type);
    ZC_Shader(ZC_Shader&& sh);

    ~ZC_Shader();

    static ZC_DA<char> ReadShaderFile(const char* path, GLenum type);
private:
    static void FillShaderStart(char* shaderData, const std::string& shaderStart) noexcept;
};