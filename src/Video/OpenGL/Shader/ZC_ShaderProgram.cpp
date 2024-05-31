#include <ZC/Video/OpenGL/Shader/ZC_ShaderProgram.h>

#include <cassert>

ZC_ShaderProgram::ZC_ShaderProgram(GLenum type, const char* code, int _shaderEnum)
    : customID(_shaderEnum)
{
    id = glCreateShaderProgramv(type, 1, &code);
    assert(id != 0);
}

ZC_ShaderProgram::ZC_ShaderProgram(ZC_ShaderProgram&& s)
    : id(s.id),
    customID(s.customID)
{
    s.id = 0;
}

ZC_ShaderProgram::~ZC_ShaderProgram()
{
    if (id != 0) glDeleteProgram(id);
}

bool ZC_ShaderProgram::operator == (int _shaderEnum) const noexcept
{
    return customID == _shaderEnum;
}