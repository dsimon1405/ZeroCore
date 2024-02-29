#========================= sources
set(ZC_SRC_FILES ${ZC_SRC_FILES}
	${PROJECT_SOURCE_DIR}/src/Tools/Console/PC/ZC_cout.cpp
	${PROJECT_SOURCE_DIR}/src/File/Read/PC/ZC_PC_FileReader.cpp
    $<$<BOOL:${ZC_IMGUI}>:
        ${PROJECT_SOURCE_DIR}/src/Video/imgui/imgui_demo.cpp
        ${PROJECT_SOURCE_DIR}/src/Video/imgui/imgui_draw.cpp
        ${PROJECT_SOURCE_DIR}/src/Video/imgui/imgui_impl_opengl3.cpp
        ${PROJECT_SOURCE_DIR}/src/Video/imgui/imgui_tables.cpp
        ${PROJECT_SOURCE_DIR}/src/Video/imgui/imgui_widgets.cpp
        ${PROJECT_SOURCE_DIR}/src/Video/imgui/imgui.cpp
        ${PROJECT_SOURCE_DIR}/src/Video/imgui/ZC_ImGui.cpp
        ${PROJECT_SOURCE_DIR}/src/Video/imgui/ZC_IGWindow.cpp
        $<$<BOOL:${ZC_SDL_VIDEO}>:
            ${PROJECT_SOURCE_DIR}/src/Video/imgui/imgui_impl_sdl3.cpp
            ${PROJECT_SOURCE_DIR}/src/Video/imgui/imgui_impl_sdlrenderer3.cpp>>
	$<$<BOOL:${ZC_SDL_VIDEO}>:
		${PROJECT_SOURCE_DIR}/src/Video/PC/SDL/ZC_SDL_Window.cpp
		${PROJECT_SOURCE_DIR}/src/Video/PC/SDL/ZC_SDL_Window_LoadOpenGLFunctions.cpp
		${PROJECT_SOURCE_DIR}/src/Events/PC/SDL/ZC_SDL_EventsHolder.cpp>
    $<$<BOOL:${ZC_AUDIO}>:
        $<$<BOOL:${ZC_SDL_AUDIO}>:
		    ${PROJECT_SOURCE_DIR}/src/Audio/PC/SDL/ZC_SDL_AudioStream.cpp>>
    PARENT_SCOPE)

set(ZC_INCLUDE_DIRS ${ZC_INCLUDE_DIRS}
    PRIVATE $<$<BOOL:${ZC_SDL_VIDEO}>:$<IF:${ZC_MY_DEBUG},/home/dmitry/projects/external/SDL3/include,${PROJECT_SOURCE_DIR}/external/SDL3/include>>
    PUBLIC $<$<BOOL:${ZC_IMGUI}>:${PROJECT_SOURCE_DIR}/includeVariable/imgui/include>
    PRIVATE $<$<BOOL:${ZC_IMGUI}AND${ZC_SDL_VIDEO}>:${PROJECT_SOURCE_DIR}/includeVariable/imgui/SDL3>
    PARENT_SCOPE)

set(ZC_LINK_LIBS ${ZC_LINK_LIBS}
    PRIVATE $<$<BOOL:${ZC_SDL_VIDEO}>:$<IF:${ZC_MY_DEBUG},/home/dmitry/projects/external/SDL3/lib/libSDL3.so,SDL3::SDL3>>
    PARENT_SCOPE)