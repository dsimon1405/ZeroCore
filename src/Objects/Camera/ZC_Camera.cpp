#include <ZC/Objects/Camera/ZC_Camera.h>

#include <ZC_Config.h>
#include <ZC/Video/ZC_Window.h>

ZC_upCamera ZC_Camera::CreateCamera(const ZC_PerspView& _perspView, const ZC_Ortho& _ortho)
{
    return { new ZC_Camera(_perspView, _ortho) };
}

ZC_Vec3<float> ZC_Camera::GetCamPos() const noexcept
{
    return perspView.view.camPos;
}

ZC_Vec3<float> ZC_Camera::GetLookOn() const noexcept
{
    return perspView.view.lookOn;
}

ZC_Vec3<float> ZC_Camera::GetUp() const noexcept
{
    return perspView.view.up;
}

ZC_Camera& ZC_Camera::SetCamPos(const ZC_Vec3<float>& _camPos) noexcept
{
    perspView.view.SetCamPos(_camPos);
    return *this;
}

ZC_Camera& ZC_Camera::SetLookOn(const ZC_Vec3<float>& _lookOn) noexcept
{
    perspView.view.SetLookOn(_lookOn);
    return *this;
}

ZC_Camera& ZC_Camera::SetUp(const ZC_Vec3<float>& _up) noexcept
{
    perspView.view.SetUp(_up);
    return *this;
}

ZC_Camera::ZC_Camera(const ZC_PerspView& _perspView, const ZC_Ortho& _ortho)
    : perspView(_perspView),
    ortho(_ortho)
{
    perspView.ubo = ZC_UBOs::Create(ZC_UBO::BindingPoint::ProjView, { &ZC_Camera::Update, this });
    perspView.ubo->BufferData(sizeof(ZC_Mat4<float>), nullptr, GL_DYNAMIC_DRAW);
    
    ZC_Window::ConnectResize({ &ZC_Camera::ResizeCallBack, this });
    
    int width = 0, height = 0;
    ZC_Window::GetSize(width, height);
    ResizeCallBack(static_cast<float>(width), static_cast<float>(height));
}

void ZC_Camera::Update()
{
    perspView.UpdateUBO();
}

void ZC_Camera::ResizeCallBack(float width, float height)
{
    perspView.persp.SetSize(width / height);
    ortho.UpdateUBO(width, height);
}