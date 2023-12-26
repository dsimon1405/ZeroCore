#include "ZC_OpenGL.h"

#include "ZC_OpenGL.h"
#include <ZC/ErrorLogger/ZC_ErrorLogger.h>

#include <sstream>

#ifdef ZC_SDL_VIDEO
void APIENTRY ZC_OpenGLErrorCallback(GLenum source, GLenum type, unsigned int id, GLenum severity, GLsizei length, const char* message, const void* userParam)
#elif defined(ZC_ANDROID_NATIVE_APP_GLUE)
void ZC_OpenGLErrorCallback(GLenum source, GLenum type, unsigned int id, GLenum severity, GLsizei length, const char* message, const void* userParam)
#endif
{
    // ignore these non-significant error codes
    if (id == 131169 || id == 131185 || id == 131218 || id == 131204) return;

    std::stringstream stream;
    stream << "Debug message (" << id << "):\n" << message << '\n';

    switch (source)
    {
        case GL_DEBUG_SOURCE_API:             stream << "Source: API"; break;
        case GL_DEBUG_SOURCE_WINDOW_SYSTEM:   stream << "Source: Window System"; break;
        case GL_DEBUG_SOURCE_SHADER_COMPILER: stream << "Source: Shader Compiler"; break;
        case GL_DEBUG_SOURCE_THIRD_PARTY:     stream << "Source: Third Party"; break;
        case GL_DEBUG_SOURCE_APPLICATION:     stream << "Source: Application"; break;
        case GL_DEBUG_SOURCE_OTHER:           stream << "Source: Other"; break;
    } stream << std::endl;

    switch (type)
    {
        case GL_DEBUG_TYPE_ERROR:               stream << "Type: Error"; break;
        case GL_DEBUG_TYPE_DEPRECATED_BEHAVIOR: stream << "Type: Deprecated Behaviour"; break;
        case GL_DEBUG_TYPE_UNDEFINED_BEHAVIOR:  stream << "Type: Undefined Behaviour"; break;
        case GL_DEBUG_TYPE_PORTABILITY:         stream << "Type: Portability"; break;
        case GL_DEBUG_TYPE_PERFORMANCE:         stream << "Type: Performance"; break;
        case GL_DEBUG_TYPE_MARKER:              stream << "Type: Marker"; break;
        case GL_DEBUG_TYPE_PUSH_GROUP:          stream << "Type: Push Group"; break;
        case GL_DEBUG_TYPE_POP_GROUP:           stream << "Type: Pop Group"; break;
        case GL_DEBUG_TYPE_OTHER:               stream << "Type: Other"; break;
    } stream << std::endl;

    switch (severity)
    {
        case GL_DEBUG_SEVERITY_HIGH:         stream << "Severity: high"; break;
        case GL_DEBUG_SEVERITY_MEDIUM:       stream << "Severity: medium"; break;
        case GL_DEBUG_SEVERITY_LOW:          stream << "Severity: low"; break;
        case GL_DEBUG_SEVERITY_NOTIFICATION: stream << "Severity: notification"; break;
    };

    ZC_ErrorLogger::Err(stream.str(), __FILE__, __LINE__);
}

void ZC_OpenGLAssigneErrorCallback()
{
    glEnable(GL_DEBUG_OUTPUT);
    glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS);
    glDebugMessageCallback(ZC_OpenGLErrorCallback, nullptr);
    glDebugMessageControl(GL_DONT_CARE, GL_DONT_CARE, GL_DONT_CARE, 0, nullptr, GL_TRUE);
}