#pragma once

#include "ZC_Perspective.h"
#include "ZC_View.h"

//  Perspective * View GLSL matrix struct.
struct ZC_PerspView
{
public:
    ZC_Perspective persp;
    ZC_View view;
    ZC_UBO* ubo;

    /*
    Create perspective * view matrix in GLSL.

    Params:
    binding - GLSL binding (use uniform std140 store type). Example GLSL code: layout (std140, binding = 0) uniform Persp { mat4 perspView; };
    _persp - perspective matrix.
    */
    ZC_PerspView(GLuint binding, const ZC_Perspective& _persp, const ZC_View& _view);

    void UpdateUBO();

};