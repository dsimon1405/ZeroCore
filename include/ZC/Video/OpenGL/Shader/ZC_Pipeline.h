#pragma once

#include "ZC_ShaderProgram.h"

#define ZC_Shader_None -1

class ZC_Pipeline
{
public:
    const int custID_V;
    const int custID_TC;
    const int custID_TE;
    const int custID_G;
    const int custID_F;

    ZC_Pipeline(ZC_ShaderProgram* v, ZC_ShaderProgram* tc, ZC_ShaderProgram* te, ZC_ShaderProgram* g, ZC_ShaderProgram* f);
    ZC_Pipeline(ZC_Pipeline&& p);
    ~ZC_Pipeline();

    void GLBindProgramPipeline();

private:
    GLuint id = 0;
};



