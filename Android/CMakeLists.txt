#========================= external libs
if(ZC_ANDROID_NATIVE_APP_GLUE)
    #   glue lib
    add_library(native_app_glue STATIC ${ANDROID_NDK}/sources/android/native_app_glue/android_native_app_glue.c)
    target_include_directories(native_app_glue PUBLIC ${ANDROID_NDK}/sources/android/native_app_glue)
endif()

#========================= sources
set(ZC_SRC_FILES ${ZC_SRC_FILES}
    ${PROJECT_SOURCE_DIR}/src/Tools/Console/Android/AndroidNativeAppGlue/ZC_cout.cpp
    $<$<BOOL:${ZC_ANDROID_NATIVE_APP_GLUE}>:
        ${PROJECT_SOURCE_DIR}/src/Video/Android/AndroidNativeAppGlue/ZC_android_app.cpp
        ${PROJECT_SOURCE_DIR}/src/Video/Android/AndroidNativeAppGlue/ZC_AndroidNativeAppGlue_Window.cpp
        ${PROJECT_SOURCE_DIR}/src/File/Read/Android/AndroidNativeAppGlue/ZC_AndroidNativeAppGlue_FileReader.cpp>
    $<$<BOOL:${ZC_AUDIO}>:
        $<$<BOOL:${ZC_OBOE}>:
            ${PROJECT_SOURCE_DIR}/src/Audio/Android/oboe/ZC_oboe_AudioStream.cpp>>
    PARENT_SCOPE)

set(ZC_INCLUDE_DIRS ${ZC_INCLUDE_DIRS}
    PUBLIC $<$<BOOL:${ZC_ANDROID_NATIVE_APP_GLUE}>:${PROJECT_SOURCE_DIR}/includeVariable/AndroidNativeAppGlue>
    PARENT_SCOPE)

set(ZC_LINK_LIBS ${ZC_LINK_LIBS}
    PUBLIC android
    PUBLIC EGL
    PUBLIC GLESv3
    PUBLIC log
    PUBLIC $<$<BOOL:${ZC_ANDROID_NATIVE_APP_GLUE}>:native_app_glue>
    PRIVATE $<$<BOOL:${ZC_OBOE}>:oboe>
    PARENT_SCOPE)