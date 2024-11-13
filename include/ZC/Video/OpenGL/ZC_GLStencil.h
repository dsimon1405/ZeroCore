#pragma once

#include <glad/glad.h>

//  Class creates for every ZC_DrawerFL (ZC_Drawer heir) on each draw level. Changes blending global state if it not equal to current state, before rendering.
//  To change global stencil test state while rendering into the call ZC_Drawer::Draw() of custom heir of ZC_Drawer, use static methods.
class ZC_GLStencil
{
public:
    /*
    Params:
    - use_stencil - glEnable/glDisable(GL_STENCIL_TEST).

    - sms_face - specifier wich face update with mask: GL_FRONT, GL_BACK, GL_FRONT_AND_BACK. Default is GL_FRONT_AND_BACK.
    - sms_mask - mask writing to the buffer [0,255]. With 255 will write to the buffer as it is, with 0 don't write to the buffer. Default is 255.

    - sos_face - specifies face to update: GL_FRONT, GL_BACK, GL_FRONT_AND_BACK. Default is GL_FRONT_AND_BACK.
    - sos_sfail - action for stencil test fail: GL_KEEP, GL_ZERO, GL_REPLACE, GL_INCR, GL_INCR_WRAP, GL_DECR, GL_DECR_WRAP, GL_INVERT. Default is GL_KEEP.
    - sos_dpfail - action for stencil test pass, depth test fail: GL_KEEP, GL_ZERO, GL_REPLACE, GL_INCR, GL_INCR_WRAP, GL_DECR, GL_DECR_WRAP, GL_INVERT. Default is GL_KEEP.
    - sos_dppass - action for stencil pass, depth path or not exists: GL_KEEP, GL_ZERO, GL_REPLACE, GL_INCR, GL_INCR_WRAP, GL_DECR, GL_DECR_WRAP, GL_INVERT. Default is GL_KEEP.

    - sfs_face - specifies updated face: GL_FRONT, GL_BACK, GL_FRONT_AND_BACK. Default is GL_FRONT_AND_BACK.
    - sfs_func - test function: GL_NEVER, GL_LESS, GL_LEQUAL, GL_GREATER, GL_GEQUAL, GL_EQUAL, GL_NOTEQUAL, and GL_ALWAYS. Default is GL_ALWAYS.
    - sfs_ref - reference value for the stencil test [0,225]. Default is 0.
    - sfs_mask - mask that is ANDed with both the reference value and the stored stencil value when the test is done [0,225]. Default is 255.
    */
    ZC_GLStencil(bool use_stencil,
        GLenum sms_face = GL_FRONT_AND_BACK, GLuint sms_mask = 255u,
        GLenum sos_face = GL_FRONT_AND_BACK, GLenum sos_sfail = GL_KEEP, GLenum sos_dpfail = GL_KEEP, GLenum sos_dppass = GL_KEEP,
        GLenum sfs_face = GL_FRONT_AND_BACK, GLenum sfs_func = GL_ALWAYS, GLint sfs_ref = 0u, GLuint sfs_mask = 255u);

        // - use_depht - glEnable/glDisable(GL_STENCIL_TEST).
    static void GLEnableDisable(bool use_stencil);

    /*
    Params:
    - sms_face - specifier wich face update with mask: GL_FRONT, GL_BACK, GL_FRONT_AND_BACK.
    - sms_mask - mask writing to the buffer [0,255]. With 255 will write to the buffer as it is, with 0 don't write to the buffer. Default is 255.
    */
    static void GLStencilMaskSeparate(GLenum sms_face, GLuint sms_mask);

    /*
    Params:
    - sos_face - specifies face to update: GL_FRONT, GL_BACK, GL_FRONT_AND_BACK.
    - sos_sfail - action for stencil test fail: GL_KEEP, GL_ZERO, GL_REPLACE, GL_INCR, GL_INCR_WRAP, GL_DECR, GL_DECR_WRAP, GL_INVERT. Default is GL_KEEP.
    - sos_dpfail - action for stencil test pass, depth test fail: GL_KEEP, GL_ZERO, GL_REPLACE, GL_INCR, GL_INCR_WRAP, GL_DECR, GL_DECR_WRAP, GL_INVERT. Default is GL_KEEP.
    - sos_dppass - action for stencil pass, depth path or not exists: GL_KEEP, GL_ZERO, GL_REPLACE, GL_INCR, GL_INCR_WRAP, GL_DECR, GL_DECR_WRAP, GL_INVERT. Default is GL_KEEP.
    */
    static void GLStencilOpSeparate(GLenum sos_face, GLenum sos_sfail, GLenum sos_dpfail, GLenum sos_dppass);

    /*
    Params:
    - sfs_face - specifies updated face: GL_FRONT, GL_BACK, GL_FRONT_AND_BACK.
    - sfs_func - test function: GL_NEVER, GL_LESS, GL_LEQUAL, GL_GREATER, GL_GEQUAL, GL_EQUAL, GL_NOTEQUAL, and GL_ALWAYS.
    - sfs_ref - reference value for the stencil test [0,225].
    - sfs_mask - mask that is ANDed with both the reference value and the stored stencil value when the test is done [0,225].
    */
    static void GLStencilFuncSeparate(GLenum sfs_face, GLenum sfs_func, GLint sfs_ref, GLuint sfs_mask);

        //  return true if glDraw... operation will write in stencil buffer.
    static bool IsGLDrawWtireToDepthBuffer();

    void Use();

private:
    struct State
    {
        bool use_stencil;
            //  glStencilMaskSeparate
        GLenum sms_face;
        GLuint sms_mask;
            //  glStencilOpSeparate
        GLenum sos_face;
        GLenum sos_sfail;
        GLenum sos_dpfail;
        GLenum sos_dppass;
            //  glStencilFuncSeparate
        GLenum sfs_face;
        GLenum sfs_func;
        GLint sfs_ref;
        GLuint sfs_mask;
    } static inline global_state { .use_stencil = false,
        .sms_face = GL_FRONT_AND_BACK, .sms_mask = 255u,
        .sos_face = GL_FRONT_AND_BACK, .sos_sfail = GL_KEEP, .sos_dpfail = GL_KEEP, .sos_dppass = GL_KEEP,
        .sfs_face = GL_FRONT_AND_BACK, .sfs_func = GL_ALWAYS, .sfs_ref = 0u, .sfs_mask = 255u };

    State state;

    static void CallSMS(GLenum sms_face, GLuint sms_mask);
    static void CallSOS(GLenum sos_face, GLenum sos_sfail, GLenum sos_dpfail, GLenum sos_dppass);
    static void CallSFS(GLenum sfs_face, GLenum sfs_func, GLint sfs_ref, GLuint sfs_mask);

    static void AssertSMS(GLenum sms_face, GLuint sms_mask);
    static void AssertSOS(GLenum sos_face, GLenum sos_sfail, GLenum sos_dpfail, GLenum sos_dppass);
    static void AssertSFS(GLenum sfs_face, GLenum sfs_func, GLint sfs_ref, GLuint sfs_mask);
        //  return true if fbo have stencil buffer
    static bool IsFBOHaveStencilBuffer();
};