#pragma once

#include <ZC/Video/OpenGL/GL/glcorearb.h>

struct ZC_Shader
{
    GLuint id;
    
    ZC_Shader(const char* code, GLenum type);

	ZC_Shader(const ZC_Shader&) = delete;
	ZC_Shader& operator = (const ZC_Shader&) = delete;

    ZC_Shader(ZC_Shader&& sh);

    ~ZC_Shader();
};