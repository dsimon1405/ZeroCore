#include <ZC/Video/OpenGL/Shader/ZC_Shader.h>

#include <Video/OpenGL/ZC_OpenGL.h>
#include <ZC/ErrorLogger/ZC_ErrorLogger.h>

ZC_Shader::ZC_Shader(ZC_Shader&& shader) noexcept
    : id(shader.id)
{
    shader.id = 0;
}

ZC_Shader& ZC_Shader::operator = (ZC_Shader&& shader)
{
	if (id != 0) glDeleteProgram(id);
    id = shader.id;
    shader.id = 0;
	return *this;
}

ZC_Shader::~ZC_Shader()
{
	glDeleteProgram(id);
}

void ZC_Shader::Use() const
{
	if (id != 0) glUseProgram(id);
}

bool ZC_Shader::SetUniformMatrix4fv(const char* name, const float* pData) const
{
    GLint location = glGetUniformLocation(id, name);
	if(location == -1) return false;
	glUniformMatrix4fv(location, 1, GL_FALSE, pData);
	return true;
}

ZC_Shader::ZC_Shader(const ZC_ShaderCode& shaderCode)
{
	if (!shaderCode.vertexCode.pHead)
	{
		ZC_ErrorLogger::Err("ZC_ShaderCode::vertexCode = nullptr!", __FILE__, __LINE__);
		return;
	}
	if (!shaderCode.fragmentCode.pHead)
	{
		ZC_ErrorLogger::Err("ZC_ShaderCode::fragmentCode = nullptr!", __FILE__, __LINE__);
		return;
	}
	CreateShaderProgram(shaderCode);
}

void ZC_Shader::CreateShaderProgram(const ZC_ShaderCode& shaderCode)
{
	GLuint vertexID = CreateShader(shaderCode.vertexCode.Begin(), GL_VERTEX_SHADER);
	if (vertexID == 0) return;

	GLuint fragmentID = CreateShader(shaderCode.fragmentCode.Begin(), GL_FRAGMENT_SHADER);
	if (fragmentID == 0)
	{
		glDeleteShader(vertexID);
		return;
	}

	GLuint geometryID = 0;
	if (shaderCode.geometryCode.pHead)
	{
		geometryID = CreateShader(shaderCode.geometryCode.pHead, GL_GEOMETRY_SHADER);
		if (geometryID == 0)
		{
			glDeleteShader(vertexID);
			glDeleteShader(fragmentID);
			return;
		}
	}

	id = glCreateProgram();
	glAttachShader(id, vertexID);
	glAttachShader(id, fragmentID);
	if (geometryID != 0) glAttachShader(id, geometryID);

	glLinkProgram(id);

	glDeleteShader(vertexID);
	glDeleteShader(fragmentID);
	if (geometryID != 0) glDeleteShader(geometryID);
}

GLuint ZC_Shader::CreateShader(const char* shaderCode, GLenum shaderType)
{
	GLuint shaderID = glCreateShader(shaderType);
	glShaderSource(shaderID, 1, &shaderCode, nullptr);
	glCompileShader(shaderID);
	return ZC_ErrorLogger::WasError() ? 0 : shaderID;
}