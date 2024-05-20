#pragma once

#include <ZC/Tools/Math/ZC_Vec4.h>

#include <glad/glad.h>

struct ZC_GLBlend
{
    ZC_GLBlend() = default;

    /*
    Params:
    - _sfactor, _dfactor - glBlendColor() params, each can be one of next params:  GL_ZERO, GL_ONE, GL_SRC_COLOR, GL_ONE_MINUS_SRC_COLOR, GL_DST_COLOR,
        GL_ONE_MINUS_DST_COLOR, GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA, GL_DST_ALPHA, GL_ONE_MINUS_DST_ALPHA, GL_CONSTANT_COLOR, GL_ONE_MINUS_CONSTANT_COLOR,
        GL_CONSTANT_ALPHA, GL_ONE_MINUS_CONSTANT_ALPHA, GL_SRC_ALPHA_SATURATE, GL_SRC1_COLOR, GL_ONE_MINUS_SRC1_COLOR, GL_SRC1_ALPHA, GL_ONE_MINUS_SRC1_ALPHA
    - red, green, blue, alpha - glBlendColor() parama, each must be in range [0, 1]. If don't need, must stay default!
    */
    ZC_GLBlend(GLenum _sfactor, GLenum _dfactor, GLfloat red = -1.f, GLfloat green = -1.f, GLfloat blue = -1.f, GLfloat alpha = -1.f);
    ~ZC_GLBlend();

    //  Don't calls glDisable, just sets pAcviteBlend = nullptr. Calling in ZC_Renderer::Draw()
    static void Disable();

    void Use();

private:
    static inline ZC_GLBlend* pActiveGLBlend = nullptr;
    static const GLenum defaultValue = 2;   //  2 not used in blend open gl, make it default

    GLenum sfactor = defaultValue,
        dfactor = defaultValue;
    ZC_Vec4<GLfloat> blendColor;
};