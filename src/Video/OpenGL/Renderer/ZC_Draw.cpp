#include <ZC/Video/OpenGL/Renderer/ZC_Draw.h>

#include <Video/OpenGL/ZC_OpenGL.h>

ZC_DrawElements::ZC_DrawElements(GLenum _mode, int _count, GLenum _type, GLuint _startByteOffset)
    : mode(_mode),
    count(_count),
    type(_type),
    startByteOffset(_startByteOffset)
{}

void ZC_DrawElements::Draw()
{
    glDrawElements(mode, count, type, reinterpret_cast<void*>(startByteOffset));
}