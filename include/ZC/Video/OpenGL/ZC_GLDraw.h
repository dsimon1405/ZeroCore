#pragma once

#include <glad/glad.h>

struct ZC_GLDraw
{
    virtual ~ZC_GLDraw() = default;
    void Draw() const;

private:
    virtual void VDraw() const = 0;
};

struct ZC_DrawElements : public ZC_GLDraw
{
    GLenum mode;
    GLsizei count;
    GLenum type;
    GLuint startByteOffset;

    /*
    Params:
    - _mode -  GL_POINTS, GL_LINE_STRIP, GL_LINE_LOOP, GL_LINES, GL_LINE_STRIP_ADJACENCY, GL_LINES_ADJACENCY, GL_TRIANGLE_STRIP, GL_TRIANGLE_FAN, GL_TRIANGLES, GL_TRIANGLE_STRIP_ADJACENCY, GL_TRIANGLES_ADJACENCY and GL_PATCHES.
    - _count - number of vertices.
    - _type - type of values in indices: GL_UNSIGNED_BYTE, GL_UNSIGNED_SHORT, or GL_UNSIGNED_INT.
    - _startByteOffset - start offset in indices buffer.
    */
    ZC_DrawElements(GLenum _mode, GLsizei _count, GLenum _type, GLuint _startByteOffset);

    void VDraw() const override;
};

struct ZC_DrawArrays : public ZC_GLDraw
{
    GLenum mode;
    GLint first;
    GLsizei count;

    /*
    Params:
    - _mode - GL_POINTS, GL_LINE_STRIP, GL_LINE_LOOP, GL_LINES, GL_LINE_STRIP_ADJACENCY, GL_LINES_ADJACENCY, GL_TRIANGLE_STRIP, GL_TRIANGLE_FAN, GL_TRIANGLES, GL_TRIANGLE_STRIP_ADJACENCY, GL_TRIANGLES_ADJACENCY and GL_PATCHES.
    - _first - starting index in the enabled arrays.
    - _count - number of vertices.
    */
    ZC_DrawArrays(GLenum _mode, GLint _first, GLsizei _count);

    void VDraw() const override;
};

// struct ZC_DrawArraysInstanced : public ZC_GLDraw
// {
//     unsigned int mode;
//     int first,
//         count,
//         instancecount;

//     /*
//     Params:
//     - _mode - GL_POINTS, GL_LINE_STRIP, GL_LINE_LOOP, GL_LINES, GL_LINE_STRIP_ADJACENCY, GL_LINES_ADJACENCY, GL_TRIANGLE_STRIP, GL_TRIANGLE_FAN, GL_TRIANGLES, GL_TRIANGLE_STRIP_ADJACENCY, GL_TRIANGLES_ADJACENCY and GL_PATCHES.
//     - _first - starting index in the enabled arrays.
//     - _count - number of vertices.
//     - _instancecount - number of instanced object to render.
//     */
//     ZC_DrawArraysInstanced(unsigned int _mode, int _first, int _count, int _instancecount);

//     void VDraw() const override;
// };

