#pragma once

#include <glad/glad.h>

//  Class creates for every ZC_DrawerFL (ZC_Drawer heir) on each draw level. Changes blending global state if it not equal to current state, before rendering.
//  To change global face cull state while rendering into the call ZC_Drawer::Draw() of custom heir of ZC_Drawer, use static methods.
class ZC_GLCullFace
{
public:
    /*
    Params:
    - use_cull_face - glEnable/glDisable(GL_CULL_FACE).
    - cull_face - wich face to cull glCullFace(GL_BACK/GL_FRONT/GL_FRONT_AND_BACK). Default is GL_BACK.
    - front_face - defines the order in which triangle points are drawn for the face glFrontFace(GL_CCW(counter clockwise)/GL_CW(clockwise)). Default is GL_CCW.
    */
    ZC_GLCullFace(bool use_cull_face, GLenum cull_face = GL_BACK, GLenum front_face = GL_CCW);

        // - use_cull_face - glEnable/glDisable(GL_CULL_FACE).
    static void GLEnableDisable(bool use_cull_face);
        // - cull_face - wich face to cull glCullFace(GL_BACK/GL_FRONT/GL_FRONT_AND_BACK).
    static void GLCullFace(GLenum cull_face);
        // - front_face - defines the order in which triangle points are drawn for the face glFrontFace(GL_CCW(counter clockwise)/GL_CW(clockwise)).
    static void GLFrontFace(GLenum front_face);

    void Use();

private:
    struct State
    {
        bool use_cull_face;
        GLenum cull_face;
        GLenum front_face;
    } static inline global_state { .use_cull_face = false, .cull_face = GL_BACK, .front_face = GL_CCW };

    State state;

    static void CallCullFace(GLenum cull_face);
    static void CallFrontFace(GLenum front_face);
};