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

    /*
    Params:
    - _mode -  GL_POINTS, GL_LINE_STRIP, GL_LINE_LOOP, GL_LINES, GL_LINE_STRIP_ADJACENCY, GL_LINES_ADJACENCY, GL_TRIANGLE_STRIP, GL_TRIANGLE_FAN, GL_TRIANGLES, GL_TRIANGLE_STRIP_ADJACENCY, GL_TRIANGLES_ADJACENCY and GL_PATCHES.
    - _count - number of vertices.
    - _type - type of values in indices: GL_UNSIGNED_BYTE, GL_UNSIGNED_SHORT, or GL_UNSIGNED_INT.
    - _startByteOffset - start offset in indices buffer.
    */
    ZC_DrawElements(GLenum _mode, int _count, GLenum _type, GLuint _startByteOffset);

    void Draw() const override;
    // void UpdateData(ZC_GLDraw* pGLDraw);
};

struct ZC_DrawArrays : public ZC_GLDraw
{
    GLenum mode;
    int first;
    int count;

    /*
    Params:
    - _mode - GL_POINTS, GL_LINE_STRIP, GL_LINE_LOOP, GL_LINES, GL_LINE_STRIP_ADJACENCY, GL_LINES_ADJACENCY, GL_TRIANGLE_STRIP, GL_TRIANGLE_FAN, GL_TRIANGLES, GL_TRIANGLE_STRIP_ADJACENCY, GL_TRIANGLES_ADJACENCY and GL_PATCHES.
    - _first - starting index in the enabled arrays.
    - _count - number of vertices.
    */
    ZC_DrawArrays(GLenum _mode, int _first, int _count);

    void Draw() const override;
    // void UpdateData(ZC_GLDraw* pGLDraw);
};

