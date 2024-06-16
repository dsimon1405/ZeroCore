#include <ZC/Video/OpenGL/Shader/ZC_Pipeline.h>

#include <cassert>

ZC_Pipeline::ZC_Pipeline(ZC_ShaderProgram* v, ZC_ShaderProgram* tc, ZC_ShaderProgram* te, ZC_ShaderProgram* g, ZC_ShaderProgram* f)
    : 
    custID_V(v ? v->customID : ZC_Shader_None),
    custID_TC(tc ? tc->customID : ZC_Shader_None),
    custID_TE(te ? te->customID : ZC_Shader_None),
    custID_G(g ? g->customID : ZC_Shader_None),
    custID_F(f ? f->customID : ZC_Shader_None)
{
    glCreateProgramPipelines(1, &id);
    assert(id != 0);
    if (v && v->id != 0) glUseProgramStages(id, GL_VERTEX_SHADER_BIT, v->id);
    if (tc && tc->id != 0) glUseProgramStages(id, GL_TESS_CONTROL_SHADER_BIT, tc->id);
    if (te && te->id != 0) glUseProgramStages(id, GL_TESS_EVALUATION_SHADER_BIT, te->id);
    if (g && g->id != 0) glUseProgramStages(id, GL_GEOMETRY_SHADER_BIT, g->id);
    if (f && f->id != 0) glUseProgramStages(id, GL_FRAGMENT_SHADER_BIT, f->id);
}

ZC_Pipeline::ZC_Pipeline(ZC_Pipeline&& p)
    : 
    custID_V(p.custID_V),
    custID_TC(p.custID_TC),
    custID_TE(p.custID_TE),
    custID_G(p.custID_G),
    custID_F(p.custID_F),
    id(p.id)
{
    p.id = 0;
}

ZC_Pipeline::~ZC_Pipeline()
{
    if (id != 0) glDeleteProgramPipelines(1, &id);
}

void ZC_Pipeline::GLBindProgramPipeline()
{
    glBindProgramPipeline(id);
}