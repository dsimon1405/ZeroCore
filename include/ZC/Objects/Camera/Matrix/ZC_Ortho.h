#pragma once

#include <ZC/Tools/Math/ZC_Mat4.h>
#include <ZC/Video/OpenGL/Buffer/ZC_UBO.h>

//  Ortho matrix struct wrapper.
struct ZC_Ortho
{
    ZC_UBO* ubo;

    /*
    Create ortho matrix. For error check use ZC_ErrorLogger::WasError() and ZC_ErrorLogger::Message() - for more info.

    Params:
    binding - GLSL binding (uses uniform std140 store type). Example GLSL code: layout (std140, binding = 1) uniform Ortho { mat4 ortho; };
    */
    ZC_Ortho();

    void UpdateUBO(float width, float height) noexcept;
};