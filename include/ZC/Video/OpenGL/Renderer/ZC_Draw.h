#pragma once

#include <ZC/Video/OpenGL/GL/glcorearb.h>

struct ZC_GLDraw
{
    virtual ~ZC_GLDraw() = default;
    virtual void Draw() = 0;
};

struct ZC_DrawElements : public ZC_GLDraw
{
    GLenum mode;
    int count;
    GLenum type;
    GLuint startByteOffset;

    ZC_DrawElements(GLenum _mode, int _count, GLenum _type, GLuint _startByteOffset);

    virtual void Draw();
};

struct ZC_DrawArrays : public ZC_GLDraw
{
    GLenum mode;
    int first;
    int count;

    ZC_DrawArrays(GLenum _mode, int _first, int _count);

    virtual void Draw();
};

