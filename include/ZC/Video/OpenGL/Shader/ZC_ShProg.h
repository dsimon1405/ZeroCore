#pragma once

#include <ZC/Video/OpenGL/GL/glcorearb.h>

struct ZC_ShProg
{
    GLuint id;

    ZC_ShProg(GLuint idV, GLuint idF, GLuint idG);

	ZC_ShProg(const ZC_ShProg&) = delete;
	ZC_ShProg& operator = (const ZC_ShProg&) = delete;
	
	ZC_ShProg(ZC_ShProg&& shader) noexcept;
	ZC_ShProg& operator = (ZC_ShProg&& shader);

    ~ZC_ShProg();

	/*
	Activate shader program.
	*/
    void Use() const;

	GLint GetUniformLocation(const char* name);

	/*
	Finds the uniform of Mat4X4 with type float in the shader and writes data to it.

	Params:
	name - uniforms name.
	pData - pointer to the first element of matrix.

	Return:
	If uniform is found - true, otherwise - false.
	*/
	bool UniformMatrix4fv(const char* name, const float* pData);
	bool Uniform1i(const char* name, int data);
	bool Uniform3fv(const char* name, const float* pData);
};