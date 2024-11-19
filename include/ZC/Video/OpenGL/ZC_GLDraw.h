#pragma once

#include <ZC/Video/OpenGL/Buffer/ZC_Buffer.h>

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

struct ZC_DrawArraysInstanced : public ZC_GLDraw
{
    unsigned int mode;
    int first,
        count,
        instancecount;

    /*
    Params:
    - _mode - GL_POINTS, GL_LINE_STRIP, GL_LINE_LOOP, GL_LINES, GL_LINE_STRIP_ADJACENCY, GL_LINES_ADJACENCY, GL_TRIANGLE_STRIP, GL_TRIANGLE_FAN, GL_TRIANGLES, GL_TRIANGLE_STRIP_ADJACENCY, GL_TRIANGLES_ADJACENCY and GL_PATCHES.
    - _first - starting index in the enabled arrays.
    - _count - number of vertices.
    - _instancecount - number of instanced object to render.
    */
    ZC_DrawArraysInstanced(unsigned int _mode, int _first, int _count, int _instancecount);

    void VDraw() const override;
};

struct ZC_MultiDrawArraysIndirect : public ZC_GLDraw
{
    struct DrawArraysIndirectCommand
    {
        GLuint count = 0;   //  count of drawing vertices (in that case GL_POINTS, one point on one element).
        GLuint instanceCount = 0;   //  if 1 instnce is drawn, if 0 not.
        GLuint first = 0;   //  index of the first drawn vertex (in window case 0 is background, if first 0 background drawn, if more than 0 not).
        GLuint baseInstance = 0;   //  may be used for some usefull information. Default unused.
    };

    ZC_Buffer* pBuf_daic;
    GLenum mode;
    void* indirect;
    GLsizei drawcount;
    GLsizei stride;

    /*
    Params:
    - _pBuf_daic - pointer to buffer (GL_DRAW_INDIRECT_BUFFER) of structs ZC_MultiDrawArraysIndirect::DrawArraysIndirectCommand.
    - _mode - GL_POINTS, GL_LINE_STRIP, GL_LINE_LOOP, GL_LINES, GL_LINE_STRIP_ADJACENCY, GL_LINES_ADJACENCY, GL_TRIANGLE_STRIP, GL_TRIANGLE_FAN, GL_TRIANGLES, GL_TRIANGLE_STRIP_ADJACENCY, GL_TRIANGLES_ADJACENCY and GL_PATCHES.
    - _indirect - SET 0! Must be pointer to array of daics, but don't work in my implimitations, so use bind of _pBuf_daic.
    - _drawcount - cont of DrawArraysIndirectCommand in _pBuf_daic.
    - stride - Set 0! Must be stride of DrawArraysIndirectCommand buffer.
    */
    ZC_MultiDrawArraysIndirect(ZC_Buffer* _pBuf_daic, GLenum _mode, void* _indirect, GLsizei _drawcount, GLsizei _stride);
    ZC_MultiDrawArraysIndirect() = default;

    void VDraw() const override;
};