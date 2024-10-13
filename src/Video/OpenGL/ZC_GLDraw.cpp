#include <ZC/Video/OpenGL/ZC_GLDraw.h>

#include <ZC/Video/OpenGL/ZC_FBOBuffersController.h>

void ZC_GLDraw::Draw() const
{
    ZC_FBOBuffersController::UsingGLDraw();
    VDraw();
}

ZC_DrawElements::ZC_DrawElements(GLenum _mode, GLsizei _count, GLenum _type, GLuint _startByteOffset)
    : mode(_mode),
    count(_count),
    type(_type),
    startByteOffset(_startByteOffset)
{}

void ZC_DrawElements::VDraw() const
{
    glDrawElements(mode, count, type, (void*)(unsigned long long)startByteOffset);
}


//  ZC_DrawArrays

ZC_DrawArrays::ZC_DrawArrays(GLenum _mode, GLint _first, GLsizei _count)
    : mode(_mode),
    first(_first),
    count(_count)
{}

void ZC_DrawArrays::VDraw() const
{
    glDrawArrays(mode, first, count);
}


//  ZC_DrawArraysInstanced

ZC_DrawArraysInstanced::ZC_DrawArraysInstanced(GLenum _mode, int _first, int _count, int _instancecount)
    : mode(_mode),
    first(_first),
    count(_count),
    instancecount(_instancecount)
{}

void ZC_DrawArraysInstanced::VDraw() const
{
    glDrawArraysInstanced(mode, first, count, instancecount);
}