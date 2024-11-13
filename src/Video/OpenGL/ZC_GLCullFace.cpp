#include <ZC/Video/OpenGL/ZC_GLCullFace.h>

#include <cassert>

ZC_GLCullFace::ZC_GLCullFace(bool use_cull_face, GLenum cull_face, GLenum front_face)
    : state{ .use_cull_face = use_cull_face, .cull_face = cull_face, .front_face = front_face }
{
    assert(cull_face == GL_BACK || cull_face == GL_FRONT || cull_face == GL_FRONT_AND_BACK);
    assert(front_face == GL_CCW || front_face == GL_CW);
}

void ZC_GLCullFace::GLEnableDisable(bool use_cull_face)
{
    assert(global_state.use_cull_face != use_cull_face);
    if (global_state.use_cull_face == use_cull_face) return;

    use_cull_face ? glEnable(GL_CULL_FACE) : glDisable(GL_CULL_FACE);
    global_state.use_cull_face = use_cull_face;
}

void ZC_GLCullFace::GLCullFace(GLenum cull_face)
{
    assert(cull_face == GL_BACK || cull_face == GL_FRONT || cull_face == GL_FRONT_AND_BACK);
    assert(global_state.cull_face != cull_face);
    CallCullFace(cull_face);
}

void ZC_GLCullFace::GLFrontFace(GLenum front_face)
{
    assert(front_face == GL_CCW || front_face == GL_CW);
    assert(global_state.front_face != front_face);
    CallFrontFace(front_face);
}

void ZC_GLCullFace::Use()
{
    auto lamb_update_params = [this]()
    {
        CallCullFace(state.cull_face);
        CallFrontFace(state.front_face);
    };

    if (global_state.use_cull_face != state.use_cull_face)
    {
        if (state.use_cull_face)    //  enable cull face and change additional params, if not equal to global state
        {
            glEnable(GL_CULL_FACE);
            lamb_update_params();
        }
        else glDisable(GL_CULL_FACE);    //  just disable cull face

        global_state.use_cull_face = state.use_cull_face;
    }
    else if (global_state.use_cull_face) lamb_update_params();  //  if face culling are allready used, updates the parameters
}

void ZC_GLCullFace::CallCullFace(GLenum cull_face)
{
    if (global_state.cull_face == cull_face) return;

    glCullFace(cull_face);
    global_state.cull_face = cull_face;
}

void ZC_GLCullFace::CallFrontFace(GLenum front_face)
{
    if (global_state.front_face == front_face) return;

    glFrontFace(front_face);
    global_state.front_face = front_face;
}