#include "ZC_ImGui.h"

#include <ZC/ErrorLogger/ZC_ErrorLogger.h>
#include <Video/OpenGL/ZC_OpenGL.h>
#include <ZC/Events/ZC_Events.h>
#include <ZC_IGWindow.h>

#include "imgui_impl_opengl3.h"
#ifdef ZC_SDL_VIDEO
// #include <Video/PC/SDL/ZC_SDL_Window.h>
#include "imgui_impl_sdl3.h"
#endif

bool ZC_ImGui::Init(void* pWindow, void* pGlContext)
{
    if (!IMGUI_CHECKVERSION())
	{
		ZC_ErrorLogger::Err("IMGUI_CHECKVERSION() fail!", __FILE__, __LINE__);
        return false;
	}
    if (!ImGui::CreateContext())
	{
		ZC_ErrorLogger::Err("CreateContext() fail!", __FILE__, __LINE__);
        return false;
	}
    ImGui::StyleColorsDark();
#ifdef ZC_SDL_VIDEO
    if (!ImGui_ImplSDL3_InitForOpenGL(static_cast<SDL_Window*>(pWindow), pGlContext))
	{
		ZC_ErrorLogger::Err("ImGui_ImplSDL3_InitForOpenGL() fail!", __FILE__, __LINE__);
        return false;
	}
#endif
    if (!ImGui_ImplOpenGL3_Init(("#version " + std::to_string(ZC_OPEN_GL_MAJOR_VERSION) + std::to_string(ZC_OPEN_GL_MINOR_VERSION) + "0").c_str()))
	{
		ZC_ErrorLogger::Err("ImGui_ImplOpenGL3_Init() fail!", __FILE__, __LINE__);
        return false;
	}

    ZC_Events::ConnectHandleEventsEnd({ &ZC_IGWindow::HandleEventsEnd });

    return true;
}

void ZC_ImGui::Destroy()
{
	ImGui_ImplOpenGL3_Shutdown();
#ifdef ZC_SDL_VIDEO
    ImGui_ImplSDL3_Shutdown();
#endif
    ImGui::DestroyContext();
}

void ZC_ImGui::FrameStart()
{
    //  new frame
    ImGui_ImplOpenGL3_NewFrame();
#ifdef ZC_SDL_VIDEO
    ImGui_ImplSDL3_NewFrame();
#endif
	ImGui::NewFrame();
}

void ZC_ImGui::FrameEnd()
{
    ImGui::Render();	//	may be before glClear()
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());	//	only before swapBuffer
}

void ZC_ImGui::PollEvents(void* event)
{
#ifdef ZC_SDL_VIDEO
    ImGui_ImplSDL3_ProcessEvent(static_cast<SDL_Event*>(event));
#endif
}