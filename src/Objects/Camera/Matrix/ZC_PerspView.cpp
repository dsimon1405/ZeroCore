#include <ZC/Objects/Camera/Matrix/ZC_PerspView.h>

#include <ZC/Video/OpenGL/Buffer/ZC_UBOs.h>

ZC_PerspView::ZC_PerspView(const ZC_Perspective& _persp, const ZC_View& _view)
    : persp(_persp),
    view(_view)
{}

void ZC_PerspView::UpdateUBO()
{
    if (persp.Update() || view.Update())
    {
#ifdef ZC_ANDROID
        ubo->BufferSubData(0, sizeof(ZC_Mat4<float>), static_cast<void*>(new ZC_Mat4<float>[](persp.perspective * view.view)));
#else
        auto pv = persp.perspective * view.view;
        ubo->BufferSubData(0, sizeof(ZC_Mat4<float>), static_cast<void*>(&pv));
#endif
    }
}