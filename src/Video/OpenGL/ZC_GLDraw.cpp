#include <ZC/Video/OpenGL/ZC_GLDraw.h>

#include <ZC/Video/OpenGL/ZC_OpenGL.h>
#include <ZC/Video/OpenGL/ZC_FBOBuffersController.h>

void ZC_GLDraw::Draw() const
{
    ZC_FBOBuffersController::UsingGLDraw();
    VDraw();
}

ZC_DrawElements::ZC_DrawElements(GLenum _mode, int _count, GLenum _type, GLuint _startByteOffset)
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

ZC_DrawArrays::ZC_DrawArrays(GLenum _mode, int _first, int _count)
    : mode(_mode),
    first(_first),
    count(_count)
{}

void ZC_DrawArrays::VDraw() const
{
    glDrawArrays(mode, first, count);
}