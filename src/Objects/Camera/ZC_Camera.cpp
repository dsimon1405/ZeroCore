#include <ZC/Objects/Camera/ZC_Camera.h>

#include <ZC_Config.h>
#include <ZC/Video/ZC_Window.h>
#include <ZC/Events/ZC_Events.h>

ZC_upCamera ZC_Camera::CreateCamera(const ZC_PerspView& _perspView, const ZC_Ortho& _ortho)
{
    ZC_upCamera upCamera(new ZC_Camera(_perspView, _ortho));
    pCurrentCamera = upCamera.Get();
    return upCamera;
}

ZC_Camera::~ZC_Camera()
{
    sconWindowResize.Disconnect();
}

ZC_Vec3<float> ZC_Camera::GetCamPos() noexcept
{
    if (pCurrentCamera) return pCurrentCamera->perspView.view.camPos;
}

ZC_Vec3<float> ZC_Camera::GetLookOn() noexcept
{
    if (pCurrentCamera) return pCurrentCamera->perspView.view.lookOn;
}

ZC_Vec3<float> ZC_Camera::GetUp() noexcept
{
    if (pCurrentCamera) return pCurrentCamera->perspView.view.up;
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

float ZC_Camera::GetWindowAspect() const noexcept
{
    return perspView.persp.aspect;
}

ZC_Camera::ZC_Camera(const ZC_PerspView& _perspView, const ZC_Ortho& _ortho)
    : perspView(_perspView),
    ortho(_ortho)
{
    perspView.ubo = ZC_UBOs::Create(ZC_UBO::BindingPoint::ProjView, { &ZC_Camera::Update, this });
    perspView.ubo->BufferData(sizeof(ZC_Mat4<float>), nullptr, GL_DYNAMIC_DRAW);
    
    sconWindowResize = ZC_Events::ConnectWindowResize({ &ZC_Camera::ResizeCallBack, this });
    
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