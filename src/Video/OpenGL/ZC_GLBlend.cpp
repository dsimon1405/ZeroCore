#include <ZC/Video/OpenGL/ZC_GLBlend.h>

#include <cassert>

ZC_GLBlend::ZC_GLBlend(bool use_blend, GLenum sfactor, GLenum dfactor, GLfloat red, GLfloat green, GLfloat blue, GLfloat alpha)
    : state{ .use_blend = use_blend, .sfactor = sfactor, .dfactor = dfactor, .blend_color = { red, green, blue, alpha } }
{
    AssertBF(sfactor, dfactor);
    AssertBC(red, green, blue, alpha);
}

void ZC_GLBlend::GLEnableDisable(bool enable)
{
    assert(global_state.use_blend != enable);   //  state is actual
    if (global_state.use_blend == enable) return;   //  state is actual

    enable ? glEnable(GL_BLEND) : glDisable(GL_BLEND);
    global_state.use_blend = enable;
}

void ZC_GLBlend::GLBendFunc(GLenum sfactor, GLenum dfactor)
{
    AssertBF(sfactor, dfactor);
    assert(global_state.sfactor != sfactor || global_state.dfactor != dfactor);     //  state is actual
    CallBendFunc(sfactor, dfactor);
}

void ZC_GLBlend::GLBlendColor(GLfloat red, GLfloat green, GLfloat blue, GLfloat alpha)
{
    AssertBC(red, green, blue, alpha);
    assert(global_state.blend_color[0] != red || global_state.blend_color[1] != green
        || global_state.blend_color[2] != blue || global_state.blend_color[3] != alpha);    //  state is actia;
    CallBlendColor(red, green, blue, alpha);
}

void ZC_GLBlend::Use()
{
    auto lamb_update_params = [this]()
    {
        CallBendFunc(state.sfactor, state.dfactor);
        CallBlendColor(state.blend_color[0], state.blend_color[1], state.blend_color[2], state.blend_color[3]);
    };

    if (global_state.use_blend != state.use_blend)  //  need change global blending state
    {
        if (state.use_blend)    //  blend disabled, enable and reset needed params
        {
            glEnable(GL_BLEND);
            lamb_update_params();
        }
        else glDisable(GL_BLEND);   //  just disable blending

        global_state.use_blend = state.use_blend;
    }
    else if (global_state.use_blend) lamb_update_params();
}

void ZC_GLBlend::CallBendFunc(GLenum sfactor, GLenum dfactor)
{
    if (global_state.sfactor == sfactor && global_state.dfactor == dfactor) return;     //  state is actual

    glBlendFunc(sfactor, dfactor);
    global_state.sfactor = sfactor;
    global_state.dfactor = dfactor;
}

void ZC_GLBlend::CallBlendColor(GLfloat red, GLfloat green, GLfloat blue, GLfloat alpha)
{
    if (global_state.blend_color[0] == red && global_state.blend_color[1] == green
        && global_state.blend_color[2] == blue, global_state.blend_color[3] == alpha) return;    //  state os actia;

    glBlendColor(red, green, blue, alpha);
    global_state.blend_color = { red, green, blue, alpha };
}

void ZC_GLBlend::AssertBF(GLenum sfactor, GLenum dfactor)
{
    assert(sfactor == GL_ZERO || sfactor == GL_ONE || sfactor == GL_SRC_COLOR || sfactor == GL_ONE_MINUS_SRC_COLOR || sfactor == GL_DST_COLOR ||
        sfactor == GL_ONE_MINUS_DST_COLOR || sfactor == GL_SRC_ALPHA || sfactor == GL_ONE_MINUS_SRC_ALPHA || sfactor == GL_DST_ALPHA ||
        sfactor ==  GL_ONE_MINUS_DST_ALPHA || sfactor == GL_CONSTANT_COLOR || sfactor == GL_ONE_MINUS_CONSTANT_COLOR || sfactor == GL_CONSTANT_ALPHA ||
        sfactor == GL_ONE_MINUS_CONSTANT_ALPHA || sfactor == GL_SRC_ALPHA_SATURATE || sfactor == GL_SRC1_COLOR || sfactor == GL_ONE_MINUS_SRC1_COLOR ||
        sfactor == GL_SRC1_ALPHA || sfactor == GL_ONE_MINUS_SRC1_ALPHA);
    assert(dfactor == GL_ZERO || dfactor == GL_ONE || dfactor == GL_SRC_COLOR || dfactor == GL_ONE_MINUS_SRC_COLOR || dfactor == GL_DST_COLOR ||
        dfactor == GL_ONE_MINUS_DST_COLOR || dfactor == GL_SRC_ALPHA || dfactor == GL_ONE_MINUS_SRC_ALPHA || dfactor == GL_DST_ALPHA ||
        dfactor ==  GL_ONE_MINUS_DST_ALPHA || dfactor == GL_CONSTANT_COLOR || dfactor == GL_ONE_MINUS_CONSTANT_COLOR || dfactor == GL_CONSTANT_ALPHA ||
        dfactor == GL_ONE_MINUS_CONSTANT_ALPHA || dfactor == GL_SRC_ALPHA_SATURATE || dfactor == GL_SRC1_COLOR || dfactor == GL_ONE_MINUS_SRC1_COLOR ||
        dfactor == GL_SRC1_ALPHA || dfactor == GL_ONE_MINUS_SRC1_ALPHA);
}

void ZC_GLBlend::AssertBC(GLfloat red, GLfloat green, GLfloat blue, GLfloat alpha)
{
    assert((red == -1.f || (red >= 0.f && red <= 1.f)) && (green == -1.f || (green >= 0.f && green <= 1.f))
        && (blue == -1.f || (blue >= 0.f && blue <= 1.f)) && (alpha == -1.f || (alpha >= 0.f && alpha <= 1.f)));
}