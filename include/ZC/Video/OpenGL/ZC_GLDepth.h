#pragma once

#include <glad/glad.h>

//  Class creates for every ZC_DrawerFL (ZC_Drawer heir) on each draw level. Changes blending global state if it not equal to current state, before rendering.
//  To change global depth test state while rendering into the call ZC_Drawer::Draw() of custom heir of ZC_Drawer, use static methods.
class ZC_GLDepth
{
public:
    /*
    Params:
    - use_depht - glEnable/glDisable(GL_DEPTH_TEST).
    - depth_mask - write to depth buffer or not glDepthMask(GL_TRUE/GL_FALSE). Default is GL_TRUE.
    - depth_func - wich comparison to use to pass depht test glDepthMask(GL_ALWAYS/GL_NEVER/GL_LESS/GL_EQUAL/GL_LEQUAL/GL_GREATER/GL_NOTEQUAL/GL_GEQUAL). Default is GL_LESS.
    */
    ZC_GLDepth(bool use_depth, GLboolean depth_mask = GL_TRUE, GLenum depth_func = GL_LESS);

        // - use_depht - glEnable/glDisable(GL_DEPTH_TEST).
    static void GLEnableDisable(bool use_depth);
        // - depth_mask - write to depth buffer or not glDepthMask(GL_TRUE/GL_FALSE).
    static void GLDepthMask(GLboolean depth_mask);
        // - depth_func - wich comparison to use to pass depht test glDepthMask(GL_ALWAYS/GL_NEVER/GL_LESS/GL_EQUAL/GL_LEQUAL/GL_GREATER/GL_NOTEQUAL/GL_GEQUAL).
    static void GLDepthFunc(GLenum depth_func);
        //  return true if glDraw... operation will write in depth buffer.
    static bool IsGLDrawWtireToDepthBuffer();

    void Use();

private:
    struct State
    {
        bool use_depth;
        GLboolean depth_mask;
        GLenum depth_func;
    } static inline global_state { .use_depth = false, .depth_mask = GL_TRUE, .depth_func = GL_LESS };

    State state;

    static void CallDepthMask(GLboolean depth_mask);
    static void CallDepthFunc(GLenum depth_func);

        //  return true if fbo have stencil buffer
    static bool IsFBOHaveDepthBuffer();
};