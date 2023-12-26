cmake_minimum_required(VERSION 3.22)
project(zeroCore)

#========================= options
#	only one could be ON
option(ZC_PC "ON - build for PC version" ON)
option(ZC_ANDROID "ON - build for Android version" OFF)

#	set the relative path from the launch file to your resources (this is necessary for the same behavior of PC/Android file systems)
#	exapmle: your resources directory in build folder (.../build/allResurcesDirName) make next => set(ZC_ASSETS_PATH allResurcesDirName)
set(ZC_ASSETS_PATH ${ZC_ASSETS_PATH})

option(ZC_AUDIO "ON - audio support" ON)

option(ZC_STATIC "ON - build static library, else build shared" ON)

message(STATUS "========================= ZERO CORE START ========================")

set(ZC_MY_DEBUG 1)

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
elseif(ZC_ANDROID)
	#	video
	set(ZC_ANDROID_NATIVE_APP_GLUE ON)
	#	audio
	if(ZC_AUDIO)
		set(ZC_OBOE ON)
	endif()
endif()

#========================= lib type
if(ZC_STATIC)
	set(ZC_LIB_TYPE STATIC)
else()
	set(ZC_LIB_TYPE SHARED)
endif()

#	fatatl error equal function
function(ZC_F_FATAL_EQUAL data1 data2 errorMessage)
	if(NOT ${data1} EQUAL ${data2})
		message(FATAL_ERROR ${errorMessage})
	endif()
endfunction()

#========================= external libs
if(ZC_PC)
	#	sdl3
	if(ZC_SDL_VIDEO AND NOT ZC_MY_DEBUG)
		set(SDL_TEST_LIBRARY OFF)
		set(SDL_STATIC ON)
		set(SDL_SHARED OFF)
		add_subdirectory(${PROJECT_SOURCE_DIR}/external/SDL3)
	endif()
endif()

if(ZC_ANDROID)
	#	android_native_app_glue
	if(ZC_ANDROID_NATIVE_APP_GLUE)
		#   set link flag for calling CMakeLists.txt
		set(CMAKE_SHARED_LINKER_FLAGS "${CMAKE_SHARED_LINKER_FLAGS} -u ANativeActivity_onCreate" PARENT_SCOPE)
	endif()

	#	oboe
	if(ZC_OBOE)
		add_subdirectory(${PROJECT_SOURCE_DIR}/external/oboe)
	endif()
endif()

#========================= ZERO CORE =========================

#========================= make config file
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

set(ZC_SRC_FILES ${ZC_SRC_FILES}
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
    ${PROJECT_SOURCE_DIR}/src/Video/ZC_Video.cpp
    ${PROJECT_SOURCE_DIR}/src/Video/ZC_Window.cpp
    ${PROJECT_SOURCE_DIR}/src/Video/OpenGL/ZC_OpenGL.cpp
    ${PROJECT_SOURCE_DIR}/src/Video/OpenGL/Shader/ZC_Shader.cpp
    ${PROJECT_SOURCE_DIR}/src/Video/OpenGL/Shader/ZC_Shaders.cpp
	${PROJECT_SOURCE_DIR}/src/Video/OpenGL/Shader/ZC_ShaderCode.cpp
    ${PROJECT_SOURCE_DIR}/src/Video/OpenGL/Shader/ZC_ShaderLoader.cpp
    ${PROJECT_SOURCE_DIR}/src/Video/OpenGL/Buffer/ZC_Buffer.cpp
    ${PROJECT_SOURCE_DIR}/src/Video/OpenGL/VBO/ZC_VBO.cpp
    ${PROJECT_SOURCE_DIR}/src/Video/OpenGL/VBO/ZC_VBOs.cpp
    ${PROJECT_SOURCE_DIR}/src/Video/OpenGL/UBO/ZC_UBO.cpp
    ${PROJECT_SOURCE_DIR}/src/Video/OpenGL/UBO/ZC_UBOs.cpp
    ${PROJECT_SOURCE_DIR}/src/Video/OpenGL/VAO/ZC_VAO.cpp
    ${PROJECT_SOURCE_DIR}/src/Video/OpenGL/VAO/ZC_VAOs.cpp
    ${PROJECT_SOURCE_DIR}/src/Video/OpenGL/VAO/ZC_VAOConfig.cpp
    ${PROJECT_SOURCE_DIR}/src/Tools/Signal/ZC_SignalConnection.cpp
	${PROJECT_SOURCE_DIR}/src/Tools/Exception/ZC_Exception.cpp
    ${PROJECT_SOURCE_DIR}/src/Tools/Time/ZC_Clock.cpp
	${PROJECT_SOURCE_DIR}/src/Tools/Container/ZC_ArrayConverter.cpp
	${PROJECT_SOURCE_DIR}/src/ErrorLogger/ZC_MakeErrorString.cpp
	${PROJECT_SOURCE_DIR}/src/File/Read/ZC_FileReader.cpp
	${PROJECT_SOURCE_DIR}/src/ErrorLogger/ZC_ErrorLogger.cpp
	${PROJECT_SOURCE_DIR}/src/Events/Button/ZC_Button.cpp
	${PROJECT_SOURCE_DIR}/src/Events/Button/ZC_ButtonOperator.cpp
	${PROJECT_SOURCE_DIR}/src/Events/ZC_Event.cpp
	${PROJECT_SOURCE_DIR}/src/Objects/Camera/ZC_Camera.cpp
	${PROJECT_SOURCE_DIR}/src/Objects/Camera/Matrix/ZC_Ortho.cpp
	${PROJECT_SOURCE_DIR}/src/Objects/Camera/Matrix/ZC_Perspective.cpp
	${PROJECT_SOURCE_DIR}/src/Objects/Camera/Matrix/ZC_View.cpp
	${PROJECT_SOURCE_DIR}/src/Objects/Camera/Matrix/ZC_PerspView.cpp
    )

add_library(${PROJECT_NAME} ${ZC_LIB_TYPE} ${ZC_SRC_FILES})

#========================= include dirs
set(ZC_INCLUDE_DIRS ${ZC_INCLUDE_DIRS}
	PUBLIC ${PROJECT_SOURCE_DIR}/include
	PRIVATE ${PROJECT_SOURCE_DIR}/src
	PUBLIC ${PROJECT_BINARY_DIR}/config
	PUBLIC $<$<BOOL:${ZC_AUDIO}>:${PROJECT_SOURCE_DIR}/includeVariable/Audio>
	PUBLIC ${PROJECT_SOURCE_DIR}/external/glm/glm	#	delete me
	)

target_include_directories(${PROJECT_NAME} ${ZC_INCLUDE_DIRS})

#========================= link libs
set(ZC_LINK_LIBS ${ZC_LINK_LIBS})

target_link_libraries(${PROJECT_NAME} ${ZC_LINK_LIBS})

#========================= compiler params
target_compile_options(${PROJECT_NAME}
	PRIVATE	$<IF:$<BOOL:${WIN32}>,-W3,-Wall>
			$<$<CONFIG:Release>:-O3>
	)

message(STATUS "======================== ZERO CORE CONFIG ========================")
message(STATUS "ZC_PC: ${ZC_PC}")
message(STATUS "ZC_ANDROID: ${ZC_ANDROID}")
message(STATUS "ZC_STATIC: ${ZC_STATIC}")
message(STATUS "ZC_AUDIO: ${ZC_AUDIO}")
message(STATUS "========================= ZERO CORE END ==========================")