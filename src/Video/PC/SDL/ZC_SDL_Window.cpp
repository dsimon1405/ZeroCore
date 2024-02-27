#include "ZC_SDL_Window.h"

#include <Video/OpenGL/ZC_OpenGL.h>
#include <ZC/ErrorLogger/ZC_ErrorLogger.h>
#ifdef ZC_IMGUI
#include <Video/imgui/ZC_ImGui.h>
#include <ZC_IGWindow.h>
#endif

#include <SDL3/SDL_init.h>
#include <SDL3/SDL_events.h>

ZC_SDL_Window::ZC_SDL_Window(bool border, int _width, int _height, const char* name)
	: fps(ZC_FPS::Seconds)
{
	static bool sdlVideoInited = false;
	if (!sdlVideoInited)
	{
    	if (SDL_Init(SDL_INIT_VIDEO) != 0)
    	{
        	ZC_ErrorLogger::Err("SDL_Init(SDL_INIT_VIDEO) fail: " + std::string(SDL_GetError()), __FILE__, __LINE__);
			return;
    	}
		sdlVideoInited = true;
	}

	if (!SetOpenGLAttributes()) return;

	pWindow = !border ? SDL_CreateWindow(name, 0, 0, SDL_WINDOW_OPENGL | SDL_WINDOW_FULLSCREEN)
		: _width <= 0 || _height <= 0 ? SDL_CreateWindow(name, _width, _height, SDL_WINDOW_OPENGL | SDL_WINDOW_RESIZABLE | SDL_WINDOW_MAXIMIZED)
			: SDL_CreateWindow(name, _width, _height, SDL_WINDOW_OPENGL | SDL_WINDOW_RESIZABLE);
		
	if (!pWindow)
	{
		ZC_ErrorLogger::Err("SDL_CreateWindow() fail: " + std::string(SDL_GetError()), __FILE__, __LINE__);
		return;
	}

    glContext = SDL_GL_CreateContext(pWindow);
	if (!glContext)
	{
		ZC_ErrorLogger::Err("SDL_GL_CreateContext() fail: " + std::string(SDL_GetError()), __FILE__, __LINE__);
		return;
	}
    
	if (SDL_GL_MakeCurrent(pWindow, glContext) != 0)
	{
		ZC_ErrorLogger::Err("SDL_GL_MakeCurrent() fail: " + std::string(SDL_GetError()), __FILE__, __LINE__);
		return;
	}

	if (SDL_GL_SetSwapInterval(0) != 0)	//	unlimit fps
	{
		ZC_ErrorLogger::Err("SDL_GL_SetSwapInterval(0) fail: " + std::string(SDL_GetError()), __FILE__, __LINE__);
		return;
	}

    if (!LoadOpenGLFunctions()) return;

	ZC_OpenGLAssigneErrorCallback();

#ifdef ZC_IMGUI
	ZC_ImGui::Init(pWindow, glContext);
#endif
}

ZC_SDL_Window::~ZC_SDL_Window()
{
    SDL_GL_DeleteContext(glContext);
    SDL_DestroyWindow(pWindow);
    SDL_Quit();
#ifdef ZC_IMGUI
	ZC_ImGui::Destroy();
#endif
}

bool ZC_SDL_Window::HandleEvents()
{
	float previousFrameTime = fps.StartNewFrame();

    static SDL_Event event;
	while (SDL_PollEvent(&event) != 0)
    {
#ifdef ZC_IMGUI
	ZC_ImGui::PollEvents(&event);
#endif
		switch (event.type)
		{
		case SDL_EVENT_QUIT: return false;
		case SDL_EVENT_WINDOW_RESIZED: Resize(); break;
		case SDL_EVENT_KEY_DOWN: AddActiveDownButton(static_cast<ZC_ButtonID>(event.key.keysym.scancode)); break;
		case SDL_EVENT_KEY_UP: CallUpButton(static_cast<ZC_ButtonID>(event.key.keysym.scancode), previousFrameTime); break;
		case SDL_EVENT_MOUSE_BUTTON_DOWN:
#ifdef ZC_IMGUI		//	if mouse cursor is in one of the ImGui windows, don't poll mouse button down event (same for mouse wheel events)
			if (!ZC_IGWindow::IsCursorInOneOfWindows()) AddActiveDownButton(static_cast<ZC_ButtonID>(event.button.button + 512));
			break;
#else
			AddActiveDownButton(static_cast<ZC_ButtonID>(event.button.button + 512)); break;
#endif
		case SDL_EVENT_MOUSE_BUTTON_UP: CallUpButton(static_cast<ZC_ButtonID>(event.button.button + 512), previousFrameTime); break;
		case SDL_EVENT_MOUSE_MOTION: Move(event.motion.x, event.motion.y, event.motion.xrel, event.motion.yrel, previousFrameTime); break;
		case SDL_EVENT_MOUSE_WHEEL:
#ifdef ZC_IMGUI
			if (!ZC_IGWindow::IsCursorInOneOfWindows()) sScroll.CallLastConnected(event.wheel.x, event.wheel.y, previousFrameTime);
			break;
#else
			sScroll(event.wheel.x, event.wheel.y, previousFrameTime); break;
#endif
		}
    }
	CallActiveButtons(previousFrameTime);
#ifdef ZC_IMGUI
	ZC_IGWindow::PollEventEnds();	//	if some one else need PollEventEnds need create a signal for it  !!!
#endif
    return true;
}

void ZC_SDL_Window::SwapBuffer()
{
    SDL_GL_SwapWindow(pWindow);
}

void ZC_SDL_Window::VSetFPS(long limit) noexcept
{
	fps.SetLimit(limit);
}

float ZC_SDL_Window::VGetPreviousFrameTime() const noexcept
{
	return fps.PreviousFrameTime();
}

void ZC_SDL_Window::VGetSize(int& width, int& height) const noexcept
{
	SDL_GetWindowSize(pWindow, &width, &height);
}

void ZC_SDL_Window::VHideCursor()
{
	SDL_SetRelativeMouseMode(SDL_TRUE);
}

void ZC_SDL_Window::VShowCursor()
{
	SDL_SetRelativeMouseMode(SDL_FALSE);
}

void ZC_SDL_Window::VLimitCursor()
{
	SDL_SetWindowMouseGrab(pWindow, SDL_TRUE);
}

void ZC_SDL_Window::VUnlimitCursor()
{
	SDL_SetWindowMouseGrab(pWindow, SDL_FALSE);
}

bool ZC_SDL_Window::SetOpenGLAttributes()
{
	if (SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE) != 0)
	{
		ZC_ErrorLogger::Err("SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE) fail: " + std::string(SDL_GetError()), __FILE__, __LINE__);
		return false;
	}
	if (SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, ZC_OPEN_GL_MAJOR_VERSION) != 0)
	{
		ZC_ErrorLogger::Err("SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, ZC_OPEN_GL_MAJOR_VERSION) fail: " + std::string(SDL_GetError()), __FILE__, __LINE__);
		return false;
	}
	if (SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, ZC_OPEN_GL_MINOR_VERSION) != 0)
	{
		ZC_ErrorLogger::Err("SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, ZC_OPEN_GL_MINOR_VERSION) fail: " + std::string(SDL_GetError()), __FILE__, __LINE__);
		return false;
	}
	if (SDL_GL_SetAttribute(SDL_GL_RED_SIZE, ZC_OPEN_GL_COLLOR_BUFFER_SIZE) != 0)
	{
		ZC_ErrorLogger::Err("SDL_GL_SetAttribute(SDL_GL_BLUE_SIZE) fail: " + std::string(SDL_GetError()), __FILE__, __LINE__);
		return false;
	}
	if (SDL_GL_SetAttribute(SDL_GL_GREEN_SIZE, ZC_OPEN_GL_COLLOR_BUFFER_SIZE) != 0)
	{
		ZC_ErrorLogger::Err("SDL_GL_SetAttribute(SDL_GL_BLUE_SIZE) fail: " + std::string(SDL_GetError()), __FILE__, __LINE__);
		return false;
	}
	if (SDL_GL_SetAttribute(SDL_GL_BLUE_SIZE, ZC_OPEN_GL_COLLOR_BUFFER_SIZE) != 0)
	{
		ZC_ErrorLogger::Err("SDL_GL_SetAttribute(SDL_GL_BLUE_SIZE) fail: " + std::string(SDL_GetError()), __FILE__, __LINE__);
		return false;
	}
	if (SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, ZC_OPEN_GL_DEPTH_BUFFER_SIZE) != 0)
	{
		ZC_ErrorLogger::Err("SDL_GL_SetAttribute(SDL_GL_BLUE_SIZE) fail: " + std::string(SDL_GetError()), __FILE__, __LINE__);
		return false;
	}

	return true;
}

void ZC_SDL_Window::Resize()
{
	int width = 0, height = 0;
	SDL_GetWindowSize(pWindow, &width,&height);
	glViewport(0, 0, width, height);										//	в ZC_Renderer ????????????????????????
	sigResize(static_cast<float>(width), static_cast<float>(height));
}

	// int red = 0;
    // int a999 = SDL_GL_GetAttribute(SDL_GL_RED_SIZE, &red);	//	8
	// int green = 0;
    // int a888 = SDL_GL_GetAttribute(SDL_GL_GREEN_SIZE, &green);	//	8
	// int blue = 0;
    // int a777 = SDL_GL_GetAttribute(SDL_GL_BLUE_SIZE, &blue);	//	8
	// int alpha = 0;
    // int a666 = SDL_GL_GetAttribute(SDL_GL_ALPHA_SIZE, &alpha);	//	8
	// int buffer = 0;
    // int a555 = SDL_GL_GetAttribute(SDL_GL_BUFFER_SIZE, &buffer);	//	32
	// int boublebuffer = 0;
    // int a444 = SDL_GL_GetAttribute(SDL_GL_DOUBLEBUFFER, &boublebuffer);	//	1 on
	// int depth = 0;
    // int a333 = SDL_GL_GetAttribute(SDL_GL_DEPTH_SIZE, &depth);	//	24
	// int stencil = 0;
    // int a222 = SDL_GL_GetAttribute(SDL_GL_STENCIL_SIZE, &stencil);	//	8
	// int accumRed = 0;
    // int a111 = SDL_GL_GetAttribute(SDL_GL_ACCUM_RED_SIZE, &accumRed);
	// int accumGreen = 0;
    // int a99 = SDL_GL_GetAttribute(SDL_GL_ACCUM_GREEN_SIZE, &accumGreen);
	// int accumBlue = 0;
    // int a88 = SDL_GL_GetAttribute(SDL_GL_ACCUM_BLUE_SIZE, &accumBlue);
	// int accumAlpha = 0;
    // int a77 = SDL_GL_GetAttribute(SDL_GL_ACCUM_ALPHA_SIZE, &accumAlpha);
	// int stereo = 0;
    // int a66 = SDL_GL_GetAttribute(SDL_GL_STEREO, &stereo);
	// int multisamplebuffers = 0;
    // int a55 = SDL_GL_GetAttribute(SDL_GL_MULTISAMPLEBUFFERS, &multisamplebuffers);
	// int multisampleSamples = 0;
    // int a44 = SDL_GL_GetAttribute(SDL_GL_MULTISAMPLESAMPLES, &multisampleSamples);
	// int acceleratedVisual = 0;
    // int a33 = SDL_GL_GetAttribute(SDL_GL_ACCELERATED_VISUAL, &acceleratedVisual);
	// int retainedBacking = 0;
    // int a22 = SDL_GL_GetAttribute(SDL_GL_RETAINED_BACKING, &retainedBacking);
	// int majorVersion = 0;
    // int a11 = SDL_GL_GetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, &majorVersion);
	// int minorVersion = 0;
    // int a0 = SDL_GL_GetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, &minorVersion);
	// int contextFlags = 0;
    // int a9 = SDL_GL_GetAttribute(SDL_GL_CONTEXT_FLAGS, &contextFlags);
	// int contextProfileMask = 0;
    // int a8 = SDL_GL_GetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, &contextProfileMask);
	// int shareWithCurrentContext = 0;
    // int a7 = SDL_GL_GetAttribute(SDL_GL_SHARE_WITH_CURRENT_CONTEXT, &shareWithCurrentContext);
	// int framebufferSRGBcapable = 0;
    // int a6 = SDL_GL_GetAttribute(SDL_GL_FRAMEBUFFER_SRGB_CAPABLE, &framebufferSRGBcapable);
	// int contextReleaseBehavior = 0;
    // int a5 = SDL_GL_GetAttribute(SDL_GL_CONTEXT_RELEASE_BEHAVIOR, &contextReleaseBehavior);
	// int contextResetNotofocation = 0;
    // int a4 = SDL_GL_GetAttribute(SDL_GL_CONTEXT_RESET_NOTIFICATION, &contextResetNotofocation);
	// int contextNoError = 0;
    // int a3 = SDL_GL_GetAttribute(SDL_GL_CONTEXT_NO_ERROR, &contextNoError);
	// int floatBuffers = 0;
    // int a2 = SDL_GL_GetAttribute(SDL_GL_FLOATBUFFERS, &floatBuffers);
	// int eglPlatform = 0;
    // int a1 = SDL_GL_GetAttribute(SDL_GL_EGL_PLATFORM, &eglPlatform);