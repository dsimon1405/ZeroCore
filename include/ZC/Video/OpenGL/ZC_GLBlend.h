#pragma once

#include <ZC/Tools/Math/ZC_Vec4.h>

#include <glad/glad.h>

//  Class creates for every ZC_DrawerFL (ZC_Drawer heir) on each draw level. Changes blending global state if it not equal to current state, before rendering.
//  To change global blending state while rendering into the call ZC_Drawer::Draw() of custom heir of ZC_Drawer, use static methods.
class ZC_GLBlend
{
private:
    static inline const float unused_color = -1.f;

    struct State
    {
        bool use_blend;
        GLenum sfactor;
        GLenum dfactor;
        ZC_Vec4<GLfloat> blend_color;
    } static inline global_state { .use_blend = false, .sfactor = GL_SRC_ALPHA, .dfactor = GL_ONE_MINUS_SRC_ALPHA,
        .blend_color { unused_color, unused_color, unused_color, unused_color }};
public:
    static inline const State& global_state_const = global_state;

    /*
    Params:
    - use_blend - glEnable/glDisable(GL_BLEND).
    - sfactor, dfactor - glBlendColor() params, each can be one of next params:  GL_ZERO, GL_ONE, GL_SRC_COLOR, GL_ONE_MINUS_SRC_COLOR, GL_DST_COLOR,
        GL_ONE_MINUS_DST_COLOR, GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA, GL_DST_ALPHA, GL_ONE_MINUS_DST_ALPHA, GL_CONSTANT_COLOR, GL_ONE_MINUS_CONSTANT_COLOR,
        GL_CONSTANT_ALPHA, GL_ONE_MINUS_CONSTANT_ALPHA, GL_SRC_ALPHA_SATURATE, GL_SRC1_COLOR, GL_ONE_MINUS_SRC1_COLOR, GL_SRC1_ALPHA, GL_ONE_MINUS_SRC1_ALPHA.
        Default sfactor = GL_SRC_ALPHA, dfactor = GL_ONE_MINUS_SRC_ALPHA.
    - red, green, blue, alpha - glBlendColor() parama, each must be in range [0, 1]. If don't need, must stay default!
    */
    ZC_GLBlend(bool use_blend, GLenum sfactor = GL_SRC_ALPHA, GLenum dfactor = GL_ONE_MINUS_SRC_ALPHA,
        GLfloat red = unused_color, GLfloat green = unused_color, GLfloat blue = unused_color, GLfloat alpha = unused_color);

        // - use_blend - glEnable/glDisable(GL_BLEND).
    static void GLEnableDisable(bool enable);
        // - sfactor, dfactor - glBlendColor() params, each can be one of next params:  GL_ZERO, GL_ONE, GL_SRC_COLOR, GL_ONE_MINUS_SRC_COLOR, GL_DST_COLOR,
        //     GL_ONE_MINUS_DST_COLOR, GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA, GL_DST_ALPHA, GL_ONE_MINUS_DST_ALPHA, GL_CONSTANT_COLOR, GL_ONE_MINUS_CONSTANT_COLOR,
        //     GL_CONSTANT_ALPHA, GL_ONE_MINUS_CONSTANT_ALPHA, GL_SRC_ALPHA_SATURATE, GL_SRC1_COLOR, GL_ONE_MINUS_SRC1_COLOR, GL_SRC1_ALPHA, GL_ONE_MINUS_SRC1_ALPHA.
    static void GLBendFunc(GLenum sfactor, GLenum dfactor);
        // - red, green, blue, alpha - glBlendColor() parama, each must be in range [0, 1].
    static void GLBlendColor(GLfloat red, GLfloat green, GLfloat blue, GLfloat alpha);

    void Use();

private:
    State state;

    static void CallBendFunc(GLenum sfactor, GLenum dfactor);
    static void CallBlendColor(GLfloat red, GLfloat green, GLfloat blue, GLfloat alpha);

    static void AssertBF(GLenum sfactor, GLenum dfactor);
    static void AssertBC(GLfloat red, GLfloat green, GLfloat blue, GLfloat alpha);
};