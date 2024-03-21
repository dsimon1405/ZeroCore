#include <ZC/Video/OpenGL/Renderer/ZC_Draw.h>

#include <Video/OpenGL/ZC_OpenGL.h>

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

// void ZC_DrawElements::UpdateData(ZC_GLDraw* pGLDraw)
// {
//     ZC_DrawElements* pDrawElements = dynamic_cast<ZC_DrawElements*>(pGLDraw);
//     mode = pDrawElements->mode;
//     count = pDrawElements->count;
//     type = pDrawElements->type;
//     startByteOffset = pDrawElements->startByteOffset;
// }


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

// void ZC_DrawArrays::UpdateData(ZC_GLDraw* pGLDraw)
// {
//     ZC_DrawArrays* pDrawArrays = dynamic_cast<ZC_DrawArrays*>(pGLDraw);
//     mode = pDrawArrays->mode;
//     first = pDrawArrays->first;
//     count = pDrawArrays->count;
// }