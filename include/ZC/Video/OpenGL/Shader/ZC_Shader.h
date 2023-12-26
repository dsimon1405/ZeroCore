#pragma once

#include <ZC/Video/OpenGL/GL/glcorearb.h>
#include "ZC_ShaderCode.h"

class ZC_Shaders;

//	OpenGL shader program class.
class ZC_Shader
{
	friend class ZC_Shaders;
public:
	ZC_Shader() = delete;

	ZC_Shader(const ZC_Shader&) = delete;
	ZC_Shader& operator = (const ZC_Shader&) = delete;
	
	ZC_Shader(ZC_Shader&& shader) noexcept;
	ZC_Shader& operator = (ZC_Shader&& shader);

    ~ZC_Shader();

	/*
	Activate shader program.
	*/
    void Use() const;

	/*
	Finds the uniform of Mat4X4 with type float in the shader and writes data to it.

	Params:
	name - uniforms name.
	pData - pointer to the first element of matrix.

	Return:
	If uniform is found - true, otherwise - false.
	*/
	bool SetUniformMatrix4fv(const char* name, const float* pData) const;
	
private:
	GLuint id = 0;

	ZC_Shader(const ZC_ShaderCode& shaderCode);

	static GLuint CreateShader(const char* shaderCode, GLenum shaderType);

	void CreateShaderProgram(const ZC_ShaderCode& shaderCode);
};