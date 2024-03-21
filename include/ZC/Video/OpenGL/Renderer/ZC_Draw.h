#pragma once

#include <ZC/Video/OpenGL/GL/glcorearb.h>

struct ZC_GLDraw
{
    virtual ~ZC_GLDraw() = default;
    virtual void Draw() const = 0;
    // virtual void UpdateData(ZC_GLDraw* pGLDraw) = 0;
};

struct ZC_DrawElements : public ZC_GLDraw
{
    GLenum mode;
    int count;
    GLenum type;
    GLuint startByteOffset;

    ZC_DrawElements(GLenum _mode, int _count, GLenum _type, GLuint _startByteOffset);

    void Draw() const override;
    // void UpdateData(ZC_GLDraw* pGLDraw);
};

struct ZC_DrawArrays : public ZC_GLDraw
{
    GLenum mode;
    int first;
    int count;

    ZC_DrawArrays(GLenum _mode, int _first, int _count);

    void Draw() const override;
    // void UpdateData(ZC_GLDraw* pGLDraw);
};

