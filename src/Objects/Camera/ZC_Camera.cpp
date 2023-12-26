#include <ZC/Objects/Camera/ZC_Camera.h>

#include <ZC_Config.h>
#ifdef ZC_SDL_VIDEO
#include <Video/PC/SDL/ZC_SDL_Window.h>
#endif

ZC_Camera::ZC_Camera(const ZC_PerspView& _perspView, const ZC_Ortho& _ortho)
    : perspView(_perspView),
    ortho(_ortho)
{
#ifdef ZC_SDL_VIDEO
    ZC_SDL_Window::ConnectResize({ &ZC_Camera::ResizeCallBack, this });
#endif
    ResizeCallBack(static_cast<float>(ZC_Window::GetWidth()), static_cast<float>(ZC_Window::GetHeight()));
}

ZC_Camera::ZC_Camera(const ZC_Camera& cm) noexcept
    : perspView(cm.perspView),
    ortho(cm.ortho)
{
#ifdef ZC_SDL_VIDEO
    ZC_SDL_Window::ConnectResize({ &ZC_Camera::ResizeCallBack, this });
#endif
}

ZC_Camera& ZC_Camera::operator = (const ZC_Camera& cm) noexcept
{   
    perspView = cm.perspView;
    ortho = cm.ortho;
#ifdef ZC_SDL_VIDEO
    ZC_SDL_Window::ConnectResize({ &ZC_Camera::ResizeCallBack, this });
#endif
    return *this;
}

void ZC_Camera::Update()
{
    perspView.UpdateUBO();
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

void ZC_Camera::ResizeCallBack(float width, float height)
{
    perspView.persp.SetSize(width / height);
    ortho.UpdateUBO(width, height);
}