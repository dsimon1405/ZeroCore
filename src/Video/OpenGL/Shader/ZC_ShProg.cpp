#include <ZC/Video/OpenGL/Shader/ZC_ShProg.h>

#include <Video/OpenGL/ZC_OpenGL.h>
#include <ZC/ErrorLogger/ZC_ErrorLogger.h>

ZC_ShProg::ZC_ShProg(GLuint idV, GLuint idF, GLuint idG)
{
    id = glCreateProgram();
    glAttachShader(id, idV);
    glAttachShader(id, idF);
    if (idG != 0) glAttachShader(id, idG);
    
    glLinkProgram(id);
    //	link check
    int success = 0;
    glGetProgramiv(id, GL_LINK_STATUS, &success);
    if (!success)
    {
        char infoLog[1024];
        glGetProgramInfoLog(id, 1024, NULL, infoLog);
        ZC_ErrorLogger::Err("glLinkProgram() fail! => " + std::string(infoLog));
    }
}
   
ZC_ShProg::ZC_ShProg(ZC_ShProg&& shader) noexcept
    : id(shader.id)
{
    shader.id = 0;
}

ZC_ShProg& ZC_ShProg::operator = (ZC_ShProg&& shader)
{
    if (id != 0) glDeleteProgram(id);
    id = shader.id;
    shader.id = 0;
    return *this;
}

ZC_ShProg::~ZC_ShProg()
{
    glDeleteProgram(id);
}

void ZC_ShProg::Use() const
{
    if (id != 0) glUseProgram(id);
}

GLint ZC_ShProg::GetUniformLocation(const char* name)
{
    GLint result = glGetUniformLocation(id, name);
    if (result == -1) ZC_ErrorLogger::Err("glGetUniformLocation() fail!", __FILE__, __LINE__);
    return result;
}

bool ZC_ShProg::UniformMatrix4fv(const char* name, const float* pData)
{
    GLint location = glGetUniformLocation(id, name);
    if(location == -1) return false;
    glUniformMatrix4fv(location, 1, GL_FALSE, pData);
    return true;
}

bool ZC_ShProg::Uniform1i(const char* name, int data)
{
    GLint location = glGetUniformLocation(id, name);
    if(location == -1) return false;
    glUniform1i(location, data);
    return true;
}

bool ZC_ShProg::Uniform3fv(const char* name, const float* pData)
{
    GLint location = glGetUniformLocation(id, name);
    if(location == -1) return false;
    glUniform3fv(location, 1, pData);
    return true;
}