#pragma once

#include <ZC/Tools/Math/ZC_Vec4.h>

class ZC_FBOBuffersController
{
public:
    const bool haveColorBuffer,
        haveDepthBuffer,
        haveStencilBuffer; 

    ZC_FBOBuffersController(bool useColorBuffer, bool useDepthBuffer, bool useStencilBuffer);

    /*
    Call glEnable(cap).

    Params:
    cap - GL_DEPTH_TEST or GL_STENCIL_TEST.
    */
    void GlEnable(unsigned int cap);

    /*
    Call glDisable(cap).

    Params:
    cap - GL_DEPTH_TEST or GL_STENCIL_TEST.
    */
    void GlDisable(unsigned int cap);

    //  Clear all enabled buffers (GL_COLOR_BUFFER_BIT, GL_DEPTH_BUFFER_BIT, GL_STENCIL_BUFFER_BIT) or those of them, that was disabled, but wasn't cleaned.
    void GlClear();

    /*
    Clears selected buffer(s). That function must be called before GLEnable, if need clear some buffer(s) before filling that buffer(s).

    Params:
    gl_clear_buffer_bit - any combination of next caps: GL_COLOR_BUFFER_BIT, GL_DEPTH_BUFFER_BIT, GL_STENCIL_BUFFER_BIT.
    */
    void GlClear(unsigned int gl_clear_buffer_bit);

    void SetClearColor(const ZC_Vec4<float>& _clearColor);

private:
    bool needClearDepth,
        needClearStencil,
        isDepthEnable = false,
        isStecncilEnable = false;
    ZC_Vec4<float> clearColor { 0.f, 0.f, 0.f, 1.f };

    void CheckClearColor();
};