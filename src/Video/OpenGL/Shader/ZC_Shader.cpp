#include <ZC/Video/OpenGL/Shader/ZC_Shader.h>

#include <Video/OpenGL/ZC_OpenGL.h>
#include <ZC/ErrorLogger/ZC_ErrorLogger.h>
    
ZC_Shader::ZC_Shader(const char* code, GLenum type)
{
    id = glCreateShader(type);
    glShaderSource(id, 1, &code, nullptr);
    glCompileShader(id);
    int success = 0;
    glGetShaderiv(id, GL_COMPILE_STATUS, &success);
    if (!success)
    {
        char infoLog[1024];
        glGetProgramInfoLog(id, 1024, NULL, infoLog);
        ZC_ErrorLogger::Err("glCompileShader() fail! => " + std::string(infoLog));
    }
}

ZC_Shader::ZC_Shader(ZC_Shader&& sh)
    : id(sh.id)
{
    sh.id = 0;
}

ZC_Shader::~ZC_Shader()
{
    glDeleteShader(id);
}