#pragma once

#include <ZC/Video/OpenGL/GL/glcorearb.h>

class ZC_RBufferCleaner
{
public:
    /*
    Call glEnable(cap).

    Params:
    cap - GL_DEPTH_TEST or GL_STENCIL_TEST.
    */
    void GlEnable(GLenum cap);

    /*
    Call glDisable(cap).

    Params:
    cap - GL_DEPTH_TEST or GL_STENCIL_TEST.
    */
    void GlDisable(GLenum cap);

    //  Call glClear(...) for all enabled buffers.
    void GlClear();

    /*
    Clears selected buffer(s);

    Params:
    gl_clear_buffer_bit - any combination of next caps: GL_COLOR_BUFFER_BIT, GL_DEPTH_BUFFER_BIT, GL_STENCIL_BUFFER_BIT.
    */
    void GlClear(GLenum gl_clear_buffer_bit);

private:
    bool needClearDepth = false,
        needClearStencil = false,
        isDepthEnable = false,
        isStecncilEnable = false;
};