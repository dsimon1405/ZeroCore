#include <ZC/Video/OpenGL/ZC_GLDepth.h>

#include <ZC/Video/OpenGL/ZC_FBOBuffersController.h>

#include <cassert>

ZC_GLDepth::ZC_GLDepth(bool use_depth, GLboolean depth_mask, GLenum depth_func)
    : state{ .use_depth = use_depth,  .depth_mask = depth_mask, .depth_func = depth_func }
{
    assert(depth_func == GL_ALWAYS || depth_func == GL_NEVER || depth_func == GL_LESS || depth_func == GL_EQUAL || depth_func == GL_LEQUAL
        || depth_func == GL_GREATER || depth_func == GL_NOTEQUAL || depth_func == GL_GEQUAL);
}

void ZC_GLDepth::GLEnableDisable(bool use_depth)
{
    assert(global_state.use_depth != use_depth);
    if (global_state.use_depth == use_depth) return;

    if (use_depth)
    {
        if (!IsFBOHaveDepthBuffer()) return;
        glEnable(GL_DEPTH_TEST);
    }
    else glDisable(GL_DEPTH_TEST);

    global_state.use_depth = use_depth;
}

void ZC_GLDepth::GLDepthMask(GLboolean depth_mask)
{
    if (!IsFBOHaveDepthBuffer()) return;
    CallDepthMask(depth_mask);
}

void ZC_GLDepth::GLDepthFunc(GLenum depth_func)
{
    if (!IsFBOHaveDepthBuffer()) return;
    assert(depth_func == GL_ALWAYS || depth_func == GL_NEVER || depth_func == GL_LESS || depth_func == GL_EQUAL || depth_func == GL_LEQUAL
        || depth_func == GL_GREATER || depth_func == GL_NOTEQUAL || depth_func == GL_GEQUAL);
    assert(global_state.depth_func != depth_func);
    CallDepthFunc(depth_func);
}

bool ZC_GLDepth::IsGLDrawWtireToDepthBuffer()
{
    return global_state.use_depth && global_state.depth_mask;   //  depth test enabled, and mask is GL_TRUE
}

void ZC_GLDepth::Use()
{
    auto lamb_update_params = [this]()
    {
        CallDepthMask(state.depth_mask);
        CallDepthFunc(state.depth_func);
    };

    if (global_state.use_depth != state.use_depth)
    {
        if (state.use_depth)
        {
            if (!IsFBOHaveDepthBuffer()) return;

            glEnable(GL_DEPTH_TEST);
            lamb_update_params();
        }
        else glDisable(GL_DEPTH_TEST);

        global_state.use_depth = state.use_depth;
    }
    else if (global_state.use_depth) lamb_update_params();
}

void ZC_GLDepth::CallDepthMask(GLboolean depth_mask)
{
    if (global_state.depth_mask == depth_mask) return;

    glDepthMask(depth_mask);
    global_state.depth_mask = depth_mask;
}

void ZC_GLDepth::CallDepthFunc(GLenum depth_func)
{
    if (global_state.depth_func == depth_func) return;

    glDepthFunc(depth_func);
    global_state.depth_func = depth_func;
}

bool ZC_GLDepth::IsFBOHaveDepthBuffer()
{
    assert(ZC_FBOBuffersController::GetActiveBufferController()->haveDepthBuffer);   //  fbo haven't stencil buffer
    return ZC_FBOBuffersController::GetActiveBufferController()->haveDepthBuffer;   //  fbo haven't stencil buffer
}