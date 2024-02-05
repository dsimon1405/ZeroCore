#pragma once

#include <ZC/Video/OpenGL/GL/glcorearb.h>

struct ZC_Draw
{
    virtual ~ZC_Draw() = default;
    virtual void Draw() = 0;
};

struct ZC_DrawElements : public ZC_Draw
{
    GLenum mode;
    int count;
    GLenum type;
    GLuint startByteOffset;

    ZC_DrawElements(GLenum _mode, int _count, GLenum _type, GLuint _startByteOffset);

    virtual void Draw();
};

