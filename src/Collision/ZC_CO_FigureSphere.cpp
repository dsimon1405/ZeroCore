#include <ZC/Collision/ZC_CO_FigureSphere.h>

ZC_CO_FigureSphere::ZC_CO_FigureSphere(const ZC_Vec3<float>& _center, float _radius)
    : radius(_radius),
    center_src(_center)
{}

void ZC_CO_FigureSphere::UpdateCenter(const ZC_Mat4<float>& mat_model)
{
    if (is_center_actual) return;
    
    center_fact = ZC_Vec3<float>(center_src[0] + mat_model[3][0], center_src[1] + mat_model[3][1], center_src[2] + mat_model[3][2]);
    is_center_actual = true;
}

ZC_Vec3<float> ZC_CO_FigureSphere::MultiplyWithModel(const ZC_Mat4<float>& model, const ZC_Vec3<float>& src)
{
    return ZC_Vec::Vec4_to_Vec3(model * ZC_Vec4<float>(src, 1.f));
}