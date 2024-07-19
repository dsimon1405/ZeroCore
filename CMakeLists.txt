cmake_minimum_required(VERSION 3.22)
project(ZeroCore)

#========================= options
#	only one could be ON
option(ZC_PC "ON - build for PC version" ON)
# option(ZC_ANDROID "ON - build for Android version" OFF)

# set ZC_PARANT_BIN_DIR for copy assets

option(ZC_AUDIO "ON - audio support" ON)
option(ZC_IMGUI "ON - build with imgui lib files (only for ZC_PC)" OFF)
option(ZC_STATIC "ON - build static library, else build shared" ON)

message(STATUS "========================= ZERO CORE START ========================")

if(ZC_MY_DEBUG)		#	top project must set preferred ZC_MY_DEBUG, or will be set ZC_MU_DEBUG 0
	set(ZC_MY_DEBUG 1)     #	1 true, 0 false   =>  use if(ZCR_MY_DEBUG EQUAL 1) - true; if(NOT ZCR_MY_DEBUG) - false
else()
	set(ZC_MY_DEBUG 0)
endif()

#========================= c++ standard
set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

#	for windows build dll + lib for linking
if(ZC_PC AND WIN32)
	set(CMAKE_WINDOWS_EXPORT_ALL_SYMBOLS ON)
endif()

#========================= set configuration
if((ZC_PC AND ZC_ANDROID) OR (NOT ZC_PC AND NOT ZC_ANDROID))
	message(FATAL_ERROR "Options ZC_PC and ZC_ANDROID can't have the same values.")
endif()

#	lib type
if(ZC_STATIC)
	set(ZC_LIB_TYPE STATIC)
else()
	set(ZC_LIB_TYPE SHARED)
endif()

#	all bool used values set off
	#	video
set(ZC_SDL_VIDEO OFF)
set(ZC_ANDROID_NATIVE_APP_GLUE OFF)
	#	audio
set(ZC_OBOE OFF)
set(ZC_SDL_AUDIO OFF)

if(ZC_PC)
	#	video
	set(ZC_SDL_VIDEO ON)
	#	audio
	if(ZC_AUDIO)
		set(ZC_SDL_AUDIO ON)
	endif()
# elseif(ZC_ANDROID)
# 	#	video
# 	set(ZC_ANDROID_NATIVE_APP_GLUE ON)
# 	set(ZC_IMGUI OFF)
# 	#	audio
# 	if(ZC_AUDIO)
# 		set(ZC_OBOE ON)
# 		list(APPEND GIT_EXTERNAL_LIBS "oboe" "https://github.com/google/oboe.git" "cb4e5d2")
# 	endif()
endif()

#========================= external libs
if(NOT ZC_MY_DEBUG)
	add_subdirectory(cmakeGitSubmodules)
endif()

if(ZC_PC)
	if (NOT ZC_MY_DEBUG)
		#	sdl3
		if(ZC_SDL_VIDEO)
			set(SDL_TEST_LIBRARY OFF)
			set(SDL_STATIC ON)
			set(SDL_SHARED OFF)
			add_subdirectory(${PROJECT_SOURCE_DIR}/external/SDL)
		endif()
		#	freetype
		set(FT_DISABLE_ZLIB ON)
		set(FT_DISABLE_BZIP2 ON)
		set(FT_DISABLE_PNG ON)
		set(FT_DISABLE_HARFBUZZ ON)
		set(FT_DISABLE_BROTLI ON)
		set(CMAKE_POLICY_DEFAULT_CMP0077 NEW)	# sets for including subdirectory, new policy -> option()
		add_subdirectory(${PROJECT_SOURCE_DIR}/external/freetype)
		# #	sqlite3
		# add_subdirectory(${PROJECT_SOURCE_DIR}/external/sqlite3)
	endif()
endif()

# if(ZC_ANDROID)
# 	#	android_native_app_glue
# 	if(ZC_ANDROID_NATIVE_APP_GLUE)
# 		#   set link flag for calling CMakeLists.txt
# 		set(CMAKE_SHARED_LINKER_FLAGS "${CMAKE_SHARED_LINKER_FLAGS} -u ANativeActivity_onCreate" PARENT_SCOPE)
# 	endif()

# 	#	oboe
# 	if(ZC_OBOE)
# 		add_subdirectory(${PROJECT_SOURCE_DIR}/external/oboe)
# 	endif()
# endif()

#========================= ZERO CORE =========================

#========================= make files
add_subdirectory(config)

#========================= sources
set(ZC_SRC_FILES)
set(ZC_INCLUDE_DIRS)
set(ZC_LINK_LIBS)

if(ZC_PC)
	add_subdirectory(PC)
elseif(ZC_ANDROID)
	add_subdirectory(Android)
endif()

#========================= add lib
add_library(${PROJECT_NAME} ${ZC_LIB_TYPE} ${ZC_SRC_FILES}
	$<$<NOT:${ZC_MY_DEBUG}>:/external/glad/src/glad.c>
	$<IF:$<CONFIG:Debug>,
		${PROJECT_SOURCE_DIR}/src/ErrorLogger/Exception/ZC_ErrorLogger.cpp,
		${PROJECT_SOURCE_DIR}/src/ErrorLogger/Message/ZC_ErrorLogger.cpp>
	$<$<BOOL:${ZC_AUDIO}>:
		${PROJECT_SOURCE_DIR}/src/Audio/ZC_Audio.cpp
		${PROJECT_SOURCE_DIR}/src/Audio/ZC_AudioSet.cpp
		${PROJECT_SOURCE_DIR}/src/Audio/ZC_AudioStream.cpp
		${PROJECT_SOURCE_DIR}/src/Audio/ZC_Sound.cpp
		${PROJECT_SOURCE_DIR}/src/Audio/ZC_SoundData.cpp
		${PROJECT_SOURCE_DIR}/src/Audio/ZC_Sounds.cpp
		${PROJECT_SOURCE_DIR}/src/Audio/ZC_StreamSound.cpp>
    ${PROJECT_SOURCE_DIR}/src/Video/ZC_SWindow.cpp
    ${PROJECT_SOURCE_DIR}/src/Video/ZC_SWindowHolder.cpp
    ${PROJECT_SOURCE_DIR}/src/Video/OpenGL/ZC_FBO.cpp
    ${PROJECT_SOURCE_DIR}/src/Video/OpenGL/ZC_FBOBuffersController.cpp
    ${PROJECT_SOURCE_DIR}/src/Video/OpenGL/ZC_Framebuffer.cpp
    ${PROJECT_SOURCE_DIR}/src/Video/OpenGL/ZC_OpenGLConfig.cpp
    ${PROJECT_SOURCE_DIR}/src/Video/OpenGL/ZC_Renderbuffer.cpp
    ${PROJECT_SOURCE_DIR}/src/Video/OpenGL/ZC_Viewport.cpp
    ${PROJECT_SOURCE_DIR}/src/Video/OpenGL/ZC_GLBlend.cpp
    ${PROJECT_SOURCE_DIR}/src/Video/OpenGL/ZC_GLDraw.cpp
	
    ${PROJECT_SOURCE_DIR}/src/Video/OpenGL/Shader/ZC_Pipeline.cpp
    ${PROJECT_SOURCE_DIR}/src/Video/OpenGL/Shader/ZC_ShaderManager.cpp
    ${PROJECT_SOURCE_DIR}/src/Video/OpenGL/Shader/ZC_ShaderProgram.cpp
    ${PROJECT_SOURCE_DIR}/src/Video/OpenGL/Shader/ZC_ShaderPrograms.cpp
    ${PROJECT_SOURCE_DIR}/src/Video/OpenGL/Shader/ZC_ShaderPath.cpp
    
	${PROJECT_SOURCE_DIR}/src/Video/OpenGL/Shader/ZC_Shader.cpp
	${PROJECT_SOURCE_DIR}/src/Video/OpenGL/Shader/ZC_ShProg.cpp
    ${PROJECT_SOURCE_DIR}/src/Video/OpenGL/Shader/ZC_ShProgs.cpp
    ${PROJECT_SOURCE_DIR}/src/Video/OpenGL/Shader/ZC_ShVertex.cpp
    ${PROJECT_SOURCE_DIR}/src/Video/OpenGL/Shader/ZC_ShFragment.cpp
    ${PROJECT_SOURCE_DIR}/src/Video/OpenGL/Shader/ZC_ShGeometry.cpp
    
	${PROJECT_SOURCE_DIR}/src/Video/OpenGL/Buffer/ZC_Buffer.cpp
    ${PROJECT_SOURCE_DIR}/src/Video/OpenGL/Buffer/ZC_Buffers.cpp
    ${PROJECT_SOURCE_DIR}/src/Video/OpenGL/Buffer/ZC_UBO.cpp
    ${PROJECT_SOURCE_DIR}/src/Video/OpenGL/Buffer/ZC_UBOs.cpp
    ${PROJECT_SOURCE_DIR}/src/Video/OpenGL/Uniform/ZC_Uniform.cpp
    ${PROJECT_SOURCE_DIR}/src/Video/OpenGL/Uniform/ZC_Uniforms.cpp
    ${PROJECT_SOURCE_DIR}/src/Video/OpenGL/VAO/ZC_VAO.cpp
    ${PROJECT_SOURCE_DIR}/src/Video/OpenGL/VAO/ZC_VAOs.cpp
    ${PROJECT_SOURCE_DIR}/src/Video/OpenGL/VAO/ZC_VAOConfig.cpp
    ${PROJECT_SOURCE_DIR}/src/Video/OpenGL/Texture/ZC_Texture.cpp
    ${PROJECT_SOURCE_DIR}/src/Video/OpenGL/Texture/ZC_TexSets.cpp
    ${PROJECT_SOURCE_DIR}/src/Video/OpenGL/Renderer/ZC_Render.cpp
    ${PROJECT_SOURCE_DIR}/src/Video/OpenGL/Renderer/ZC_Renderer.cpp
    ${PROJECT_SOURCE_DIR}/src/Video/OpenGL/Renderer/ZC_Drawer.cpp
    ${PROJECT_SOURCE_DIR}/src/Video/OpenGL/Renderer/ZC_DrawerSet.cpp
    ${PROJECT_SOURCE_DIR}/src/Video/OpenGL/Renderer/ZC_Renders.cpp
    ${PROJECT_SOURCE_DIR}/src/Video/OpenGL/Renderer/ZC_DSController.cpp
    ${PROJECT_SOURCE_DIR}/src/Video/OpenGL/Renderer/ZC_RSPersonalData.cpp
    ${PROJECT_SOURCE_DIR}/src/Video/OpenGL/Renderer/ZC_DSStencilBorder.cpp
    ${PROJECT_SOURCE_DIR}/src/Tools/Signal/ZC_SConnection.cpp
	${PROJECT_SOURCE_DIR}/src/Tools/Exception/ZC_Exception.cpp
    ${PROJECT_SOURCE_DIR}/src/Tools/Time/ZC_Clock.cpp
	${PROJECT_SOURCE_DIR}/src/Tools/Time/ZC_FPS.cpp
	${PROJECT_SOURCE_DIR}/src/Tools/ZC_OrthoSquare.cpp
	${PROJECT_SOURCE_DIR}/src/Tools/ZC_TornToCamera.cpp
	${PROJECT_SOURCE_DIR}/src/Tools/ZC_WindowOrthoIndent.cpp
	${PROJECT_SOURCE_DIR}/src/Tools/ZC_ButtonManipulator.cpp
	${PROJECT_SOURCE_DIR}/src/ErrorLogger/ZC_MakeErrorString.cpp
	${PROJECT_SOURCE_DIR}/src/File/Read/ZC_FileReader.cpp
	${PROJECT_SOURCE_DIR}/src/ErrorLogger/ZC_ErrorLogger.cpp
	${PROJECT_SOURCE_DIR}/src/Events/Button/ZC_ButtonHolder.cpp
	${PROJECT_SOURCE_DIR}/src/Events/Button/ZC_ButtonPressedDown.cpp
	${PROJECT_SOURCE_DIR}/src/Events/Button/ZC_ButtonClick.cpp
	${PROJECT_SOURCE_DIR}/src/Events/Button/ZC_ButtonUp.cpp
    ${PROJECT_SOURCE_DIR}/src/Events/ZC_EC.cpp
    ${PROJECT_SOURCE_DIR}/src/Events/ZC_Events.cpp
    ${PROJECT_SOURCE_DIR}/src/Events/ZC_EventsHolder.cpp
	${PROJECT_SOURCE_DIR}/src/Events/ZC_Mouse.cpp
	${PROJECT_SOURCE_DIR}/src/Objects/Camera/ZC_Camera.cpp
	${PROJECT_SOURCE_DIR}/src/Objects/Camera/ZC_Ortho.cpp
	${PROJECT_SOURCE_DIR}/src/Objects/Camera/ZC_Perspective.cpp
	${PROJECT_SOURCE_DIR}/src/Objects/Camera/ZC_View.cpp
	${PROJECT_SOURCE_DIR}/src/Objects/Text/ZC_Font.cpp
	${PROJECT_SOURCE_DIR}/src/Objects/Text/ZC_Fonts.cpp
	${PROJECT_SOURCE_DIR}/src/Objects/Text/ZC_TextData.cpp
	${PROJECT_SOURCE_DIR}/src/Objects/Text/ZC_TextScene.cpp
	${PROJECT_SOURCE_DIR}/src/Objects/Text/ZC_TextSceneTurnedToCamera.cpp
	${PROJECT_SOURCE_DIR}/src/Objects/Text/ZC_TextWindow.cpp
	${PROJECT_SOURCE_DIR}/src/Objects/Text/ZC_TextWindowIntoScene.cpp
	${PROJECT_SOURCE_DIR}/src/Collision/ZC_MouseCollisionWindow.cpp
	${PROJECT_SOURCE_DIR}/src/Collision/ZC_MouseCollisionWindowController.cpp
	
	${PROJECT_SOURCE_DIR}/include/ZC/GUI/ZC_GUI_ButtonMouse.cpp
	${PROJECT_SOURCE_DIR}/include/ZC/GUI/ZC_GUI_EventManager.cpp
	${PROJECT_SOURCE_DIR}/include/ZC/GUI/ZC_GUI_Obj.cpp
	${PROJECT_SOURCE_DIR}/include/ZC/GUI/ZC_GUI_ObjBorder.cpp
	${PROJECT_SOURCE_DIR}/include/ZC/GUI/ZC_GUI_Window.cpp
	${PROJECT_SOURCE_DIR}/include/ZC/GUI/ZC_GUI_WinMutable.cpp
	${PROJECT_SOURCE_DIR}/include/ZC/GUI/ZC_GUI.cpp
	${PROJECT_SOURCE_DIR}/include/ZC/GUI/ZC_GUI_DrawManager.cpp
	${PROJECT_SOURCE_DIR}/include/ZC/GUI/ZC_GUI_WinImmutable.cpp
	${PROJECT_SOURCE_DIR}/include/ZC/GUI/ZC_GUI_ObjData.cpp
	${PROJECT_SOURCE_DIR}/include/ZC/GUI/ZC_GUI_ButtonKeyboard.cpp
	${PROJECT_SOURCE_DIR}/include/ZC/GUI/ZC_GUI_ButtonMouseAndKeyboard.cpp
	${PROJECT_SOURCE_DIR}/include/ZC/GUI/ZC_GUI_ButtonBase.cpp
	${PROJECT_SOURCE_DIR}/include/ZC/GUI/Text/ZC_GUI_Font.cpp
	${PROJECT_SOURCE_DIR}/include/ZC/GUI/Text/ZC_GUI_FontLoader.cpp
	${PROJECT_SOURCE_DIR}/include/ZC/GUI/Text/ZC_GUI_TextManager.cpp
	${PROJECT_SOURCE_DIR}/include/ZC/GUI/Text/ZC_GUI_TextInputWindow.cpp
	)

#========================= include dirs
target_include_directories(${PROJECT_NAME} ${ZC_INCLUDE_DIRS}
	PUBLIC ${PROJECT_SOURCE_DIR}/external/glad/include
	PUBLIC ${PROJECT_SOURCE_DIR}/include
	PRIVATE ${PROJECT_SOURCE_DIR}/src
	PUBLIC ${PROJECT_BINARY_DIR}/config
	PUBLIC $<$<BOOL:${ZC_AUDIO}>:${PROJECT_SOURCE_DIR}/includeVariable/Audio>
	)

#========================= link libs
target_link_libraries(${PROJECT_NAME} ${ZC_LINK_LIBS}
	PUBLIC $<$<BOOL:${ZC_MY_DEBUG}>:/home/dmitry/projects/external/glad/lib/libglad.so>
	)
	
#========================= compiler params
target_compile_options(${PROJECT_NAME}
	PRIVATE	$<IF:$<BOOL:${WIN32}>,-W3,-Wall>
			$<$<CONFIG:Release>:-O3>
	)

#========================= copy assets
add_custom_target(ZC_COPY_ASSETS ALL
	COMMAND ${CMAKE_COMMAND} -E copy_directory ${PROJECT_SOURCE_DIR}/assets ${ZC_PARANT_BIN_DIR}
	COMMENT "Copy assets ${PROJECT_SOURCE_DIR}/assets => ${ZC_PARANT_BIN_DIR}")
add_dependencies(${PROJECT_NAME} ZC_COPY_ASSETS)

message(STATUS "======================== ZERO CORE CONFIG ========================")
message(STATUS "ZC_PC: ${ZC_PC}")
# message(STATUS "ZC_ANDROID: ${ZC_ANDROID}")
message(STATUS "ZC_STATIC: ${ZC_STATIC}")
message(STATUS "ZC_AUDIO: ${ZC_AUDIO}")
message(STATUS "========================= ZERO CORE END ==========================")