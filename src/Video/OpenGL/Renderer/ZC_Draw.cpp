#include <ZC/Video/OpenGL/Renderer/ZC_Draw.h>

#include <ZC/Video/OpenGL/ZC_OpenGL.h>

ZC_DrawElements::ZC_DrawElements(GLenum _mode, int _count, GLenum _type, GLuint _startByteOffset)
    : mode(_mode),
    count(_count),
    type(_type),
    startByteOffset(_startByteOffset)
{}

void ZC_DrawElements::Draw() const
{
    glDrawElements(mode, count, type, reinterpret_cast<void*>(startByteOffset));
}


//  ZC_DrawArrays

ZC_DrawArrays::ZC_DrawArrays(GLenum _mode, int _first, int _count)
    : mode(_mode),
    first(_first),
    count(_count)
{}

void ZC_DrawArrays::Draw() const
{
    glDrawArrays(mode, first, count);
}