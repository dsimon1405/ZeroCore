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

    //  Clear all enabled buffers (GL_COLOR_BUFFER_BIT, GL_DEPTH_BUFFER_BIT, GL_STENCIL_BUFFER_BIT) or those of them, that was disabled, but wasn't cleaned.
    void GlClear();

    /*
    Clears selected buffer(s). That function must be called before GLEnable, if need clear some buffer(s) before filling that buffer(s).

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