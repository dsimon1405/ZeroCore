#include <ZC/Objects/Camera/Matrix/ZC_Ortho.h>

#include <ZC/Tools/Math/ZC_Mat.h>
#include <ZC/Video/OpenGL/Buffer/ZC_UBOs.h>

ZC_Ortho::ZC_Ortho()
    : ubo(ZC_UBOs::Create(ZC_UBO::BindingPoint::Ortho, nullptr))
{
    ubo->BufferData(sizeof(ZC_Mat4<float>), nullptr, GL_DYNAMIC_DRAW);
}

void ZC_Ortho::UpdateUBO(float width, float height) noexcept
{
    auto ortho = ZC_Mat::Ortho(0.f, width, 0.f, height);
    ubo->BufferSubData(0, sizeof(ZC_Mat4<float>), static_cast<void*>(&ortho));
}