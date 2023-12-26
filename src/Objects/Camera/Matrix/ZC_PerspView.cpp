#include <ZC/Objects/Camera/Matrix/ZC_PerspView.h>

#include <ZC/Video/OpenGL/UBO/ZC_UBOs.h>
#include <Tools/Container/ZC_ArrayConverter.h>

ZC_PerspView::ZC_PerspView(GLuint binding, const ZC_Perspective& _persp, const ZC_View& _view)
    : persp(_persp),
    view(_view),
    ubo(ZC_UBOs::Create(binding))
{
    ubo->BufferData(ZC_ArrayConverter::Convert(sizeof(ZC_Mat4<float>)), GL_DYNAMIC_DRAW);
}

void ZC_PerspView::UpdateUBO()
{
    if (persp.Update() || view.Update()) ubo->BufferSubData(0, ZC_ArrayConverter::Convert(persp.perspective * view.view));
}