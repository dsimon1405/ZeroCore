#include <ZC/Video/OpenGL/ZC_GLStencil.h>

#include <ZC/Video/OpenGL/ZC_FBOBuffersController.h>

#include <cassert>

ZC_GLStencil::ZC_GLStencil(bool use_stencil,
    GLenum sms_face, GLuint sms_mask,
    GLenum sos_face, GLenum sos_sfail, GLenum sos_dpfail, GLenum sos_dppass,
    GLenum sfs_face, GLenum sfs_func, GLint sfs_ref, GLuint sfs_mask)
    : state{ .use_stencil = use_stencil,
        .sms_face = sms_face, .sms_mask = sms_mask,
        .sos_face = sos_face, .sos_sfail = sos_sfail, .sos_dpfail = sos_dpfail, .sos_dppass = sos_dppass,
        .sfs_face = sfs_face, .sfs_func = sfs_func, .sfs_ref = sfs_ref, .sfs_mask = sfs_mask }
{
    AssertSMS(sms_face, sms_mask);
    AssertSOS(sos_face, sos_sfail, sos_dpfail, sos_dppass);
    AssertSFS(sfs_face, sfs_func, sfs_ref, sfs_mask);
}

void ZC_GLStencil::GLEnableDisable(bool use_stencil)
{
    assert(global_state.use_stencil != use_stencil);
    if (global_state.use_stencil == use_stencil) return;

    if (use_stencil)
    {
        if (!IsFBOHaveStencilBuffer()) return;
        glEnable(GL_STENCIL_TEST);
    }
    else glDisable(GL_STENCIL_TEST);
    global_state.use_stencil = use_stencil;
}

void ZC_GLStencil::GLStencilMaskSeparate(GLenum sms_face, GLuint sms_mask)
{
    if (!IsFBOHaveStencilBuffer()) return;
    AssertSMS(sms_face, sms_mask);
    assert(global_state.sms_face != sms_face || global_state.sms_mask != sms_mask);
    
    CallSMS(sms_face, sms_mask);
}

void ZC_GLStencil::GLStencilOpSeparate(GLenum sos_face, GLenum sos_sfail, GLenum sos_dpfail, GLenum sos_dppass)
{
    if (!IsFBOHaveStencilBuffer()) return;
    AssertSOS(sos_face, sos_sfail, sos_dpfail, sos_dppass);
    assert(global_state.sos_face != sos_face || global_state.sos_sfail != sos_sfail || global_state.sos_dpfail != sos_dpfail || global_state.sos_dppass != sos_dppass);
    
    CallSOS(sos_face, sos_sfail, sos_dpfail, sos_dppass);
}

void ZC_GLStencil::GLStencilFuncSeparate(GLenum sfs_face, GLenum sfs_func, GLint sfs_ref, GLuint sfs_mask)
{
    if (!IsFBOHaveStencilBuffer()) return;
    AssertSFS(sfs_face, sfs_func, sfs_ref, sfs_mask);
    assert(global_state.sfs_face != sfs_face || global_state.sfs_func != sfs_func || global_state.sfs_ref != sfs_ref || global_state.sfs_mask != sfs_mask);

    CallSFS(sfs_face, sfs_func, sfs_ref, sfs_mask);
}

bool ZC_GLStencil::IsGLDrawWtireToDepthBuffer()
{
    return global_state.use_stencil;
}

void ZC_GLStencil::Use()
{
    auto lamb_update_params = [this]()
    {
        CallSMS(state.sms_face, state.sms_mask);
        CallSOS(state.sos_face, state.sos_sfail, state.sos_dpfail, state.sos_dppass);
        CallSFS(state.sfs_face, state.sfs_func, state.sfs_ref, state.sfs_mask);
    };

    if (global_state.use_stencil != state.use_stencil)
    {
        if (state.use_stencil)
        {
            if (!IsFBOHaveStencilBuffer()) return;
            
            glEnable(GL_STENCIL_TEST);
            lamb_update_params();
        }
        else glDisable(GL_STENCIL_TEST);

        global_state.use_stencil = state.use_stencil;
    }
    else if (global_state.use_stencil) lamb_update_params();
}

void ZC_GLStencil::CallSMS(GLenum sms_face, GLuint sms_mask)
{
    if (global_state.sms_face == sms_face && global_state.sms_mask == sms_mask) return;

    glStencilMaskSeparate(sms_face, sms_mask);
    global_state.sms_face = sms_face;
    global_state.sms_mask = sms_mask;
}

void ZC_GLStencil::CallSOS(GLenum sos_face, GLenum sos_sfail, GLenum sos_dpfail, GLenum sos_dppass)
{
    if (global_state.sos_face == sos_face && global_state.sos_sfail == sos_sfail && global_state.sos_dpfail == sos_dpfail && global_state.sos_dppass == sos_dppass) return;

    glStencilOpSeparate(sos_face, sos_sfail, sos_dpfail, sos_dppass);
    global_state.sos_face = sos_face;
    global_state.sos_sfail = sos_sfail;
    global_state.sos_dpfail = sos_dpfail;
    global_state.sos_dppass = sos_dppass;
}

void ZC_GLStencil::CallSFS(GLenum sfs_face, GLenum sfs_func, GLint sfs_ref, GLuint sfs_mask)
{
    if (global_state.sfs_face == sfs_face && global_state.sfs_func == sfs_func && global_state.sfs_ref == sfs_ref && global_state.sfs_mask == sfs_mask) return;

    glStencilFuncSeparate(sfs_face, sfs_func, sfs_ref, sfs_mask);
    global_state.sfs_face = sfs_face;
    global_state.sfs_func = sfs_func;
    global_state.sfs_ref = sfs_ref;
    global_state.sfs_mask = sfs_mask;
}

void ZC_GLStencil::AssertSMS(GLenum sms_face, GLuint sms_mask)
{
    assert(sms_face == GL_FRONT || sms_face == GL_BACK || sms_face == GL_FRONT_AND_BACK);
    assert(sms_mask >= 0u && sms_mask <= 255u);
}

void ZC_GLStencil::AssertSOS(GLenum sos_face, GLenum sos_sfail, GLenum sos_dpfail, GLenum sos_dppass)
{
    assert(sos_face == GL_FRONT || sos_face == GL_BACK || sos_face == GL_FRONT_AND_BACK);
    assert(sos_sfail == GL_KEEP || sos_sfail == GL_ZERO || sos_sfail == GL_REPLACE || sos_sfail == GL_INCR || sos_sfail == GL_INCR_WRAP || sos_sfail == GL_DECR
        || sos_sfail == GL_DECR_WRAP || sos_sfail == GL_INVERT);
    assert(sos_dpfail == GL_KEEP || sos_dpfail == GL_ZERO || sos_dpfail == GL_REPLACE || sos_dpfail == GL_INCR || sos_dpfail == GL_INCR_WRAP || sos_dpfail == GL_DECR
        || sos_dpfail == GL_DECR_WRAP || sos_dpfail == GL_INVERT);
    assert(sos_dppass == GL_KEEP || sos_dppass == GL_ZERO || sos_dppass == GL_REPLACE || sos_dppass == GL_INCR || sos_dppass == GL_INCR_WRAP || sos_dppass == GL_DECR
        || sos_dppass == GL_DECR_WRAP || sos_dppass == GL_INVERT);
}

void ZC_GLStencil::AssertSFS(GLenum sfs_face, GLenum sfs_func, GLint sfs_ref, GLuint sfs_mask)
{
    assert(sfs_face == GL_FRONT || sfs_face ==  GL_BACK || sfs_face ==  GL_FRONT_AND_BACK);
    assert(sfs_func == GL_NEVER || sfs_func == GL_LESS || sfs_func == GL_LEQUAL || sfs_func == GL_GREATER || sfs_func == GL_GEQUAL || sfs_func == GL_EQUAL
        || sfs_func == GL_NOTEQUAL || sfs_func == GL_ALWAYS);
    assert(sfs_ref >= 0u && sfs_ref <= 255u);
    assert(sfs_mask >= 0u && sfs_mask <= 255u);
}

bool ZC_GLStencil::IsFBOHaveStencilBuffer()
{
    assert(ZC_FBOBuffersController::GetActiveBufferController()->haveStencilBuffer);   //  fbo haven't stencil buffer
    return ZC_FBOBuffersController::GetActiveBufferController()->haveStencilBuffer;   //  fbo haven't stencil buffer
}