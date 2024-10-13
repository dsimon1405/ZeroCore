#include <ZC/Collision/ZC_CO_FigureSphere.h>

ZC_CO_FigureSphere::ZC_CO_FigureSphere(const ZC_Vec3<float>& _center, float _radius)
    : radius(_radius),
    center_src(_center)
{}

void ZC_CO_FigureSphere::UpdateCenter(const ZC_Mat4<float>& mat_model)
{
    if (is_center_actual) return;
    
    center_fact = MultiplyWithModel(mat_model, center_src);
    is_center_actual = true;
}

ZC_Vec3<float> ZC_CO_FigureSphere::MultiplyWithModel(const ZC_Mat4<float>& model, const ZC_Vec3<float>& src)
{
    return ZC_Vec::Vec4_to_Vec3(model * ZC_Vec4<float>(src, 1.f));
}