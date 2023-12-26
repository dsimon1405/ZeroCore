#include <ZC/Objects/Camera/Matrix/ZC_Ortho.h>

#include <ZC/Tools/Math/ZC_Mat.h>
#include <ZC/Video/OpenGL/UBO/ZC_UBOs.h>
#include <Tools/Container/ZC_ArrayConverter.h>

ZC_Ortho::ZC_Ortho(GLuint binding)
    : ubo(ZC_UBOs::Create(binding))
{
    ubo->BufferData(ZC_ArrayConverter::Convert(sizeof(ZC_Mat4<float>)), GL_DYNAMIC_DRAW);
}

void ZC_Ortho::UpdateUBO(float width, float height) noexcept
{
    ubo->BufferSubData(0, ZC_ArrayConverter::Convert(ZC_Mat::Ortho(0.f, width, 0.f, height)));
}