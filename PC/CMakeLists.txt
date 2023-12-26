#========================= sources
set(ZC_SRC_FILES ${ZC_SRC_FILES}
	${PROJECT_SOURCE_DIR}/src/Tools/Console/PC/ZC_cout.cpp
	${PROJECT_SOURCE_DIR}/src/File/Read/PC/ZC_PC_FileReader.cpp
	$<$<BOOL:${ZC_SDL_VIDEO}>:
		${PROJECT_SOURCE_DIR}/src/Video/PC/SDL/ZC_SDL_Window.cpp
		${PROJECT_SOURCE_DIR}/src/Video/OpenGL/PC/SDL/ZC_SDL_LoadOpenGLFunctions.cpp>
        ${PROJECT_SOURCE_DIR}/src/Tools/Time/PC/SDL/ZC_SDL_FPS.cpp
        ${PROJECT_SOURCE_DIR}/src/Events/ZC_Mouse.cpp
    $<$<BOOL:${ZC_AUDIO}>:
        $<$<BOOL:${ZC_SDL_AUDIO}>:
		    ${PROJECT_SOURCE_DIR}/src/Audio/PC/SDL/ZC_SDL_AudioStream.cpp>>
    PARENT_SCOPE)

set(ZC_INCLUDE_DIRS ${ZC_INCLUDE_DIRS}
    PRIVATE $<$<BOOL:${ZC_SDL_VIDEO}>:${PROJECT_SOURCE_DIR}/external/SDL3/include>
    PARENT_SCOPE)

set(ZC_LINK_LIBS ${ZC_LINK_LIBS}
    PRIVATE $<$<BOOL:${ZC_SDL_VIDEO}>:$<IF:${ZC_MY_DEBUG},/home/dmitry/projects/external/SDL3/lib/libSDL3.so,SDL3::SDL3>>
    PARENT_SCOPE)