#pragma once

#include <ZC/Tools/Math/ZC_Vec4.h>

#include <glad/glad.h>

#include <list>

class ZC_FBOBuffersController
{
public:
    const bool haveColorBuffer,
        haveDepthBuffer,
        haveStencilBuffer; 

    ZC_FBOBuffersController(bool useColorBuffer, bool useDepthBuffer, bool useStencilBuffer);
    ZC_FBOBuffersController(const ZC_FBOBuffersController& bc);
    ~ZC_FBOBuffersController();

    static ZC_FBOBuffersController* GetActiveBufferController();

    //  makes ZC_FBOBuffersController active, all calls of static functions will call that object.
    void MakeActive();

    // /*
    // Call glEnable(cap).

    // Params:
    // cap - GL_DEPTH_TEST or GL_STENCIL_TEST.
    // */
    // static void GLEnabled(GLenum cap);

    // /*
    // Call glDisable(cap).

    // Params:
    // cap - GL_DEPTH_TEST or GL_STENCIL_TEST.
    // */
    // static void GLDisabled(GLenum cap);

    //  Calls in ZC_FBO::Use(). Clears all buffers which have.
    void GlClear();

    /*
    Clears selected buffer(s).

    Params:
    clearMask - any combination of next caps: GL_COLOR_BUFFER_BIT, GL_DEPTH_BUFFER_BIT, GL_STENCIL_BUFFER_BIT.
    */
    static void GlClear(GLbitfield clearMask);

    //  calls ZC_GLDraw to update which buffers need to be cleared
    static void UsingGLDraw();

    void SetClearColor(const ZC_Vec4<GLfloat>& _clearColor);

        //  call in ZC_Renderer on swap buffer, to make all existing buffers need to clean
    static void BufferSwaped();

private:
    static inline ZC_FBOBuffersController* pActiveBufferController;
    static inline std::list<ZC_FBOBuffersController*> all_buffers_controllers;

    bool needClearColor;
    bool needClearDepth;
    bool needClearStencil;
    ZC_Vec4<GLfloat> clearColor { 0.f, 0.f, 0.f, 1.f };

    void UpdateClearColor();
};