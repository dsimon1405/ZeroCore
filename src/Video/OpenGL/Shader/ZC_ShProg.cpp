#include <ZC/Video/OpenGL/Shader/ZC_ShProg.h>

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
    // int uniforms = 0;
    // glGetProgramiv(id, GL_ACTIVE_UNIFORMS, &uniforms);

    glDetachShader(id, idV);
    glDetachShader(id, idF);
    if (idG != 0) glDetachShader(id, idG);
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

void ZC_ShProg::ActivateOpenGL() const
{    
    if (activeId == id || id == 0) return;
    activeId = id;
    
    glUseProgram(id);
}

void ZC_ShProg::SetDefault()
{
    activeId = 0;
    glUseProgram(activeId);
}

GLint ZC_ShProg::GetUniformLocation(const char* name)
{
    GLint result = glGetUniformLocation(id, name);
    if (result == -1) ZC_ErrorLogger::Err("glGetUniformLocation() fail!", __FILE__, __LINE__);
    return result;
}