#include "ZC_SDL_Window.h"

#include <ZC/Video/OpenGL/ZC_OpenGL.h>
#include <ZC/ErrorLogger/ZC_ErrorLogger.h>

bool ZC_SDL_Window::LoadOpenGLFunctions()
{
    // if (SDL_GL_LoadLibrary(NULL) != 0) { ZC_ErrorLogger::Err("SDL_GL_LoadLibrary() faild! " + std::string(SDL_GetError()), __FILE__, __LINE__); return false;}

    //  window
    pglViewport = (PFNGLVIEWPORTPROC)SDL_GL_GetProcAddress("glViewport");
    if (!pglViewport) { ZC_ErrorLogger::Err("glViewPort SDL_GL_GetProcAddress faild!", __FILE__, __LINE__); return false;}
    pglClear = (PFNGLCLEARPROC)SDL_GL_GetProcAddress("glClear");
    if (!pglClear) { ZC_ErrorLogger::Err("glClear SDL_GL_GetProcAddress faild!", __FILE__, __LINE__); return false;}
    pglClearColor = (PFNGLCLEARCOLORPROC)SDL_GL_GetProcAddress("glClearColor");
    if (!pglClearColor) { ZC_ErrorLogger::Err("glClearColor SDL_GL_GetProcAddress faild!", __FILE__, __LINE__); return false;}
    //  get/read
    pglGetString = (PFNGLGETSTRINGPROC)SDL_GL_GetProcAddress("glGetString");
    if (!pglGetString) { ZC_ErrorLogger::Err("glGetString SDL_GL_GetProcAddress faild!", __FILE__, __LINE__); return false;}
    pglGetIntegerv = (PFNGLGETINTEGERVPROC)SDL_GL_GetProcAddress("glGetIntegerv");
    if (!pglGetIntegerv) { ZC_ErrorLogger::Err("glGetIntegerv SDL_GL_GetProcAddress faild!", __FILE__, __LINE__); return false;}
    pglReadPixels = (PFNGLREADPIXELSPROC)SDL_GL_GetProcAddress("glReadPixels");
    if (!pglReadPixels) { ZC_ErrorLogger::Err("glReadPixels SDL_GL_GetProcAddress faild!", __FILE__, __LINE__); return false;}
    //  enble
    pglEnable = (PFNGLENABLEPROC)SDL_GL_GetProcAddress("glEnable");
    if (!pglEnable) { ZC_ErrorLogger::Err("glEnable SDL_GL_GetProcAddress faild!", __FILE__, __LINE__); return false;}
    pglDisable = (PFNGLDISABLEPROC)SDL_GL_GetProcAddress("glDisable");
    if (!pglDisable) { ZC_ErrorLogger::Err("glDisable SDL_GL_GetProcAddress faild!", __FILE__, __LINE__); return false;}
    pglBlendFunc = (PFNGLBLENDFUNCPROC)SDL_GL_GetProcAddress("glBlendFunc");
    if (!pglBlendFunc) { ZC_ErrorLogger::Err("glBlendFunc SDL_GL_GetProcAddress faild!", __FILE__, __LINE__); return false;}
    //  debug
    pglDebugMessageCallback = (PFNGLDEBUGMESSAGECALLBACKPROC)SDL_GL_GetProcAddress("glDebugMessageCallback");
    if (!pglDebugMessageCallback) { ZC_ErrorLogger::Err("glDebugMessageCallback SDL_GL_GetProcAddress faild!", __FILE__, __LINE__); return false;}
    pglDebugMessageControl = (PFNGLDEBUGMESSAGECONTROLPROC)SDL_GL_GetProcAddress("glDebugMessageControl");
    if (!pglDebugMessageControl) { ZC_ErrorLogger::Err("glDebugMessageControl SDL_GL_GetProcAddress faild!", __FILE__, __LINE__); return false;}
    //  shader
    pglCreateProgram = (PFNGLCREATEPROGRAMPROC)SDL_GL_GetProcAddress("glCreateProgram");
    if (!pglCreateProgram) { ZC_ErrorLogger::Err("glCreateProgram SDL_GL_GetProcAddress faild!", __FILE__, __LINE__); return false;}
    pglAttachShader = (PFNGLATTACHSHADERPROC)SDL_GL_GetProcAddress("glAttachShader");
    if (!pglAttachShader) { ZC_ErrorLogger::Err("glAttachShader SDL_GL_GetProcAddress faild!", __FILE__, __LINE__); return false;}
    pglLinkProgram = (PFNGLLINKPROGRAMPROC)SDL_GL_GetProcAddress("glLinkProgram");
    if (!pglLinkProgram) { ZC_ErrorLogger::Err("glLinkProgram SDL_GL_GetProcAddress faild!", __FILE__, __LINE__); return false;}
    pglGetProgramiv = (PFNGLGETPROGRAMIVPROC)SDL_GL_GetProcAddress("glGetProgramiv");
    if (!pglGetProgramiv) { ZC_ErrorLogger::Err("glGetProgramiv SDL_GL_GetProcAddress faild!", __FILE__, __LINE__); return false;}
    pglGetProgramInfoLog = (PFNGLGETPROGRAMINFOLOGPROC)SDL_GL_GetProcAddress("glGetProgramInfoLog");
    if (!pglGetProgramInfoLog) { ZC_ErrorLogger::Err("glGetProgramInfoLog SDL_GL_GetProcAddress faild!", __FILE__, __LINE__); return false;}
    pglDeleteShader = (PFNGLDELETESHADERPROC)SDL_GL_GetProcAddress("glDeleteShader");
    if (!pglDeleteShader) { ZC_ErrorLogger::Err("glDeleteShader SDL_GL_GetProcAddress faild!", __FILE__, __LINE__); return false;}
    pglCreateShader = (PFNGLCREATESHADERPROC)SDL_GL_GetProcAddress("glCreateShader");
    if (!pglCreateShader) { ZC_ErrorLogger::Err("glCreateShader SDL_GL_GetProcAddress faild!", __FILE__, __LINE__); return false;}
    pglShaderSource = (PFNGLSHADERSOURCEPROC)SDL_GL_GetProcAddress("glShaderSource");
    if (!pglShaderSource) { ZC_ErrorLogger::Err("glShaderSource SDL_GL_GetProcAddress faild!", __FILE__, __LINE__); return false;}
    pglCompileShader = (PFNGLCOMPILESHADERPROC)SDL_GL_GetProcAddress("glCompileShader");
    if (!pglCompileShader) { ZC_ErrorLogger::Err("glCompileShader SDL_GL_GetProcAddress faild!", __FILE__, __LINE__); return false;}
    pglGetShaderiv = (PFNGLGETSHADERIVPROC)SDL_GL_GetProcAddress("glGetShaderiv");
    if (!pglGetShaderiv) { ZC_ErrorLogger::Err("glGetShaderiv SDL_GL_GetProcAddress faild!", __FILE__, __LINE__); return false;}
    pglGetShaderInfoLog = (PFNGLGETSHADERINFOLOGPROC)SDL_GL_GetProcAddress("glGetShaderInfoLog");
    if (!pglGetShaderInfoLog) { ZC_ErrorLogger::Err("glGetShaderInfoLog SDL_GL_GetProcAddress faild!", __FILE__, __LINE__); return false;}
    pglDeleteProgram = (PFNGLDELETEPROGRAMPROC)SDL_GL_GetProcAddress("glDeleteProgram");
    if (!pglDeleteProgram) { ZC_ErrorLogger::Err("glDeleteProgram SDL_GL_GetProcAddress faild!", __FILE__, __LINE__); return false;}
    pglUseProgram = (PFNGLUSEPROGRAMPROC)SDL_GL_GetProcAddress("glUseProgram");
    if (!pglUseProgram) { ZC_ErrorLogger::Err("glUseProgram SDL_GL_GetProcAddress faild!", __FILE__, __LINE__); return false;}
    pglGetUniformLocation = (PFNGLGETUNIFORMLOCATIONPROC)SDL_GL_GetProcAddress("glGetUniformLocation");
    if (!pglGetUniformLocation) { ZC_ErrorLogger::Err("glGetUniformLocation SDL_GL_GetProcAddress faild!", __FILE__, __LINE__); return false;}
    pglUniform1f = (PFNGLUNIFORM1FPROC)SDL_GL_GetProcAddress("glUniform1f");
    if (!pglUniform1f) { ZC_ErrorLogger::Err("glUniform1f SDL_GL_GetProcAddress faild!", __FILE__, __LINE__); return false;}
    pglUniform1i = (PFNGLUNIFORM1IPROC)SDL_GL_GetProcAddress("glUniform1i");
    if (!pglUniform1i) { ZC_ErrorLogger::Err("glUniform1i SDL_GL_GetProcAddress faild!", __FILE__, __LINE__); return false;}
    pglUniform1ui = (PFNGLUNIFORM1UIPROC)SDL_GL_GetProcAddress("glUniform1ui");
    if (!pglUniform1ui) { ZC_ErrorLogger::Err("glUniform1ui SDL_GL_GetProcAddress faild!", __FILE__, __LINE__); return false;}
    pglUniform1fv = (PFNGLUNIFORM1FVPROC)SDL_GL_GetProcAddress("glUniform1fv");
    if (!pglUniform1fv) { ZC_ErrorLogger::Err("glUniform1fv SDL_GL_GetProcAddress faild!", __FILE__, __LINE__); return false;}
    pglUniform2fv = (PFNGLUNIFORM2FVPROC)SDL_GL_GetProcAddress("glUniform2fv");
    if (!pglUniform2fv) { ZC_ErrorLogger::Err("glUniform2fv SDL_GL_GetProcAddress faild!", __FILE__, __LINE__); return false;}
    pglUniform3fv = (PFNGLUNIFORM3FVPROC)SDL_GL_GetProcAddress("glUniform3fv");
    if (!pglUniform3fv) { ZC_ErrorLogger::Err("glUniform3fv SDL_GL_GetProcAddress faild!", __FILE__, __LINE__); return false;}
    pglUniform4fv = (PFNGLUNIFORM4FVPROC)SDL_GL_GetProcAddress("glUniform4fv");
    if (!pglUniform4fv) { ZC_ErrorLogger::Err("glUniform4fv SDL_GL_GetProcAddress faild!", __FILE__, __LINE__); return false;}
    pglUniform1iv = (PFNGLUNIFORM1IVPROC)SDL_GL_GetProcAddress("glUniform1iv");
    if (!pglUniform1iv) { ZC_ErrorLogger::Err("glUniform1iv SDL_GL_GetProcAddress faild!", __FILE__, __LINE__); return false;}
    pglUniform2iv = (PFNGLUNIFORM2IVPROC)SDL_GL_GetProcAddress("glUniform2iv");
    if (!pglUniform2iv) { ZC_ErrorLogger::Err("glUniform2iv SDL_GL_GetProcAddress faild!", __FILE__, __LINE__); return false;}
    pglUniform3iv = (PFNGLUNIFORM3IVPROC)SDL_GL_GetProcAddress("glUniform3iv");
    if (!pglUniform3iv) { ZC_ErrorLogger::Err("glUniform3iv SDL_GL_GetProcAddress faild!", __FILE__, __LINE__); return false;}
    pglUniform4iv = (PFNGLUNIFORM4IVPROC)SDL_GL_GetProcAddress("glUniform4iv");
    if (!pglUniform4iv) { ZC_ErrorLogger::Err("glUniform4iv SDL_GL_GetProcAddress faild!", __FILE__, __LINE__); return false;}
    pglUniform1uiv = (PFNGLUNIFORM1UIVPROC)SDL_GL_GetProcAddress("glUniform1uiv");
    if (!pglUniform1uiv) { ZC_ErrorLogger::Err("glUniform1uiv SDL_GL_GetProcAddress faild!", __FILE__, __LINE__); return false;}
    pglUniform2uiv = (PFNGLUNIFORM2UIVPROC)SDL_GL_GetProcAddress("glUniform2uiv");
    if (!pglUniform2uiv) { ZC_ErrorLogger::Err("glUniform2uiv SDL_GL_GetProcAddress faild!", __FILE__, __LINE__); return false;}
    pglUniform3uiv = (PFNGLUNIFORM3UIVPROC)SDL_GL_GetProcAddress("glUniform3uiv");
    if (!pglUniform3uiv) { ZC_ErrorLogger::Err("glUniform3uiv SDL_GL_GetProcAddress faild!", __FILE__, __LINE__); return false;}
    pglUniform4uiv = (PFNGLUNIFORM4UIVPROC)SDL_GL_GetProcAddress("glUniform4uiv");
    if (!pglUniform4uiv) { ZC_ErrorLogger::Err("glUniform4uiv SDL_GL_GetProcAddress faild!", __FILE__, __LINE__); return false;}
    pglUniformMatrix2fv = (PFNGLUNIFORMMATRIX2FVPROC)SDL_GL_GetProcAddress("glUniformMatrix2fv");
    if (!pglUniformMatrix2fv) { ZC_ErrorLogger::Err("glUniformMatrix2fv SDL_GL_GetProcAddress faild!", __FILE__, __LINE__); return false;}
    pglUniformMatrix3fv = (PFNGLUNIFORMMATRIX3FVPROC)SDL_GL_GetProcAddress("glUniformMatrix3fv");
    if (!pglUniformMatrix3fv) { ZC_ErrorLogger::Err("glUniformMatrix3fv SDL_GL_GetProcAddress faild!", __FILE__, __LINE__); return false;}
    pglUniformMatrix4fv = (PFNGLUNIFORMMATRIX4FVPROC)SDL_GL_GetProcAddress("glUniformMatrix4fv");
    if (!pglUniformMatrix4fv) { ZC_ErrorLogger::Err("glUniformMatrix4fv SDL_GL_GetProcAddress faild!", __FILE__, __LINE__); return false;}
    pglUniformMatrix2x3fv = (PFNGLUNIFORMMATRIX2X3FVPROC)SDL_GL_GetProcAddress("glUniformMatrix2x3fv");
    if (!pglUniformMatrix2x3fv) { ZC_ErrorLogger::Err("glUniformMatrix2x3fv SDL_GL_GetProcAddress faild!", __FILE__, __LINE__); return false;}
    pglUniformMatrix3x2fv = (PFNGLUNIFORMMATRIX3X2FVPROC)SDL_GL_GetProcAddress("glUniformMatrix3x2fv");
    if (!pglUniformMatrix3x2fv) { ZC_ErrorLogger::Err("glUniformMatrix3x2fv SDL_GL_GetProcAddress faild!", __FILE__, __LINE__); return false;}
    pglUniformMatrix2x4fv = (PFNGLUNIFORMMATRIX2X4FVPROC)SDL_GL_GetProcAddress("glUniformMatrix2x4fv");
    if (!pglUniformMatrix2x4fv) { ZC_ErrorLogger::Err("glUniformMatrix2x4fv SDL_GL_GetProcAddress faild!", __FILE__, __LINE__); return false;}
    pglUniformMatrix4x2fv = (PFNGLUNIFORMMATRIX4X2FVPROC)SDL_GL_GetProcAddress("glUniformMatrix4x2fv");
    if (!pglUniformMatrix4x2fv) { ZC_ErrorLogger::Err("glUniformMatrix4x2fv SDL_GL_GetProcAddress faild!", __FILE__, __LINE__); return false;}
    pglUniformMatrix3x4fv = (PFNGLUNIFORMMATRIX3X4FVPROC)SDL_GL_GetProcAddress("glUniformMatrix3x4fv");
    if (!pglUniformMatrix3x4fv) { ZC_ErrorLogger::Err("glUniformMatrix3x4fv SDL_GL_GetProcAddress faild!", __FILE__, __LINE__); return false;}
    pglUniformMatrix4x3fv = (PFNGLUNIFORMMATRIX4X3FVPROC)SDL_GL_GetProcAddress("glUniformMatrix4x3fv");
    if (!pglUniformMatrix4x3fv) { ZC_ErrorLogger::Err("glUniformMatrix4x3fv SDL_GL_GetProcAddress faild!", __FILE__, __LINE__); return false;}
    //  buffer
    pglGenBuffers = (PFNGLGENBUFFERSPROC)SDL_GL_GetProcAddress("glGenBuffers");
    if (!pglGenBuffers) { ZC_ErrorLogger::Err("glGenBuffers SDL_GL_GetProcAddress faild!", __FILE__, __LINE__); return false;}
    pglDeleteBuffers = (PFNGLDELETEBUFFERSPROC)SDL_GL_GetProcAddress("glDeleteBuffers");
    if (!pglDeleteBuffers) { ZC_ErrorLogger::Err("glDeleteBuffers SDL_GL_GetProcAddress faild!", __FILE__, __LINE__); return false;}
    pglBindBuffer = (PFNGLBINDBUFFERPROC)SDL_GL_GetProcAddress("glBindBuffer");
    if (!pglBindBuffer) { ZC_ErrorLogger::Err("glBindBuffer SDL_GL_GetProcAddress faild!", __FILE__, __LINE__); return false;}
    pglBufferData = (PFNGLBUFFERDATAPROC)SDL_GL_GetProcAddress("glBufferData");
    if (!pglBufferData) { ZC_ErrorLogger::Err("glBufferData SDL_GL_GetProcAddress faild!", __FILE__, __LINE__); return false;}
    pglBufferSubData = (PFNGLBUFFERSUBDATAPROC)SDL_GL_GetProcAddress("glBufferSubData");
    if (!pglBufferSubData) { ZC_ErrorLogger::Err("glBufferSubData SDL_GL_GetProcAddress faild!", __FILE__, __LINE__); return false;}
    pglBindBufferRange = (PFNGLBINDBUFFERRANGEPROC)SDL_GL_GetProcAddress("glBindBufferRange");
    if (!pglBindBufferRange) { ZC_ErrorLogger::Err("glBindBufferRange SDL_GL_GetProcAddress faild!", __FILE__, __LINE__); return false;}
    //  vbo
    pglBindVertexBuffer = (PFNGLBINDVERTEXBUFFERPROC)SDL_GL_GetProcAddress("glBindVertexBuffer");
    if (!pglBindVertexBuffer) { ZC_ErrorLogger::Err("glBindVertexBuffer SDL_GL_GetProcAddress faild!", __FILE__, __LINE__); return false;}
    //  vao
    pglGenVertexArrays = (PFNGLGENVERTEXARRAYSPROC)SDL_GL_GetProcAddress("glGenVertexArrays");
    if (!pglGenVertexArrays) { ZC_ErrorLogger::Err("glGenVertexArrays SDL_GL_GetProcAddress faild!", __FILE__, __LINE__); return false;}
    pglDeleteVertexArrays = (PFNGLDELETEVERTEXARRAYSPROC)SDL_GL_GetProcAddress("glDeleteVertexArrays");
    if (!pglDeleteVertexArrays) { ZC_ErrorLogger::Err("glDeleteVertexArrays SDL_GL_GetProcAddress faild!", __FILE__, __LINE__); return false;}
    pglBindVertexArray = (PFNGLBINDVERTEXARRAYPROC)SDL_GL_GetProcAddress("glBindVertexArray");
    if (!pglBindVertexArray) { ZC_ErrorLogger::Err("glBindVertexArray SDL_GL_GetProcAddress faild!", __FILE__, __LINE__); return false;}
    pglEnableVertexAttribArray = (PFNGLENABLEVERTEXATTRIBARRAYPROC)SDL_GL_GetProcAddress("glEnableVertexAttribArray");
    if (!pglEnableVertexAttribArray) { ZC_ErrorLogger::Err("glEnableVertexAttribArray SDL_GL_GetProcAddress faild!", __FILE__, __LINE__); return false;}
    pglDisableVertexAttribArray = (PFNGLDISABLEVERTEXATTRIBARRAYPROC)SDL_GL_GetProcAddress("glDisableVertexAttribArray");
    if (!pglDisableVertexAttribArray) { ZC_ErrorLogger::Err("glDisableVertexAttribArray SDL_GL_GetProcAddress faild!", __FILE__, __LINE__); return false;}
    pglVertexAttribPointer = (PFNGLVERTEXATTRIBPOINTERPROC)SDL_GL_GetProcAddress("glVertexAttribPointer");
    if (!pglVertexAttribPointer) { ZC_ErrorLogger::Err("glVertexAttribPointer SDL_GL_GetProcAddress faild!", __FILE__, __LINE__); return false;}
    pglDrawArrays = (PFNGLDRAWARRAYSPROC)SDL_GL_GetProcAddress("glDrawArrays");
    if (!pglDrawArrays) { ZC_ErrorLogger::Err("glDrawArrays SDL_GL_GetProcAddress faild!", __FILE__, __LINE__); return false;}
    pglDrawElements = (PFNGLDRAWELEMENTSPROC)SDL_GL_GetProcAddress("glDrawElements");
    if (!pglDrawElements) { ZC_ErrorLogger::Err("glDrawElements SDL_GL_GetProcAddress faild!", __FILE__, __LINE__); return false;}
    pglVertexAttribFormat = (PFNGLVERTEXATTRIBFORMATPROC)SDL_GL_GetProcAddress("glVertexAttribFormat");
    if (!pglVertexAttribFormat) { ZC_ErrorLogger::Err("glVertexAttribFormat SDL_GL_GetProcAddress faild!", __FILE__, __LINE__); return false;}
    pglVertexAttribBinding = (PFNGLVERTEXATTRIBBINDINGPROC)SDL_GL_GetProcAddress("glVertexAttribBinding");
    if (!pglVertexAttribBinding) { ZC_ErrorLogger::Err("glVertexAttribBinding SDL_GL_GetProcAddress faild!", __FILE__, __LINE__); return false;}
    // //  va
    // pglCreateVertexArrays = (PFNGLCREATEVERTEXARRAYSPROC)SDL_GL_GetProcAddress("glCreateVertexArrays");
    // if (!pglCreateVertexArrays) { ZC_ErrorLogger::Err("glCreateVertexArrays SDL_GL_GetProcAddress faild!", __FILE__, __LINE__); return false;}
    // pglEnableVertexArrayAttrib = (PFNGLENABLEVERTEXARRAYATTRIBPROC)SDL_GL_GetProcAddress("glEnableVertexArrayAttrib");
    // if (!pglEnableVertexArrayAttrib) { ZC_ErrorLogger::Err("glEnableVertexArrayAttrib SDL_GL_GetProcAddress faild!", __FILE__, __LINE__); return false;}
    // pglVertexArrayAttribFormat = (PFNGLVERTEXARRAYATTRIBFORMATPROC)SDL_GL_GetProcAddress("glVertexArrayAttribFormat");
    // if (!pglVertexArrayAttribFormat) { ZC_ErrorLogger::Err("glVertexArrayAttribFormat SDL_GL_GetProcAddress faild!", __FILE__, __LINE__); return false;}
    // pglVertexArrayAttribBinding = (PFNGLVERTEXARRAYATTRIBBINDINGPROC)SDL_GL_GetProcAddress("glVertexArrayAttribBinding");
    // if (!pglVertexArrayAttribBinding) { ZC_ErrorLogger::Err("glVertexArrayAttribBinding SDL_GL_GetProcAddress faild!", __FILE__, __LINE__); return false;}
    // pglVertexArrayVertexBuffer = (PFNGLVERTEXARRAYVERTEXBUFFERPROC)SDL_GL_GetProcAddress("glVertexArrayVertexBuffer");
    // if (!pglVertexArrayVertexBuffer) { ZC_ErrorLogger::Err("glVertexArrayVertexBuffer SDL_GL_GetProcAddress faild!", __FILE__, __LINE__); return false;}
    //  texture
    pglGenTextures = (PFNGLGENTEXTURESPROC)SDL_GL_GetProcAddress("glGenTextures");
    if (!pglGenTextures) { ZC_ErrorLogger::Err("glGenTextures SDL_GL_GetProcAddress faild!", __FILE__, __LINE__); return false;}
    pglDeleteTextures = (PFNGLDELETETEXTURESPROC)SDL_GL_GetProcAddress("glDeleteTextures");
    if (!pglDeleteTextures) { ZC_ErrorLogger::Err("glDeleteTextures SDL_GL_GetProcAddress faild!", __FILE__, __LINE__); return false;}
    pglBindTexture = (PFNGLBINDTEXTUREPROC)SDL_GL_GetProcAddress("glBindTexture");
    if (!pglBindTexture) { ZC_ErrorLogger::Err("glBindTexture SDL_GL_GetProcAddress faild!", __FILE__, __LINE__); return false;}
    pglTexParameteri = (PFNGLTEXPARAMETERIPROC)SDL_GL_GetProcAddress("glTexParameteri");
    if (!pglTexParameteri) { ZC_ErrorLogger::Err("glTexParameteri SDL_GL_GetProcAddress faild!", __FILE__, __LINE__); return false;}
    pglTexImage2D = (PFNGLTEXIMAGE2DPROC)SDL_GL_GetProcAddress("glTexImage2D");
    if (!pglTexImage2D) { ZC_ErrorLogger::Err("glTexImage2D SDL_GL_GetProcAddress faild!", __FILE__, __LINE__); return false;}
    pglGenerateMipmap = (PFNGLGENERATEMIPMAPPROC)SDL_GL_GetProcAddress("glGenerateMipmap");
    if (!pglGenerateMipmap) { ZC_ErrorLogger::Err("glGenerateMipmap SDL_GL_GetProcAddress faild!", __FILE__, __LINE__); return false;}
    pglActiveTexture = (PFNGLACTIVETEXTUREPROC)SDL_GL_GetProcAddress("glActiveTexture");
    if (!pglActiveTexture) { ZC_ErrorLogger::Err("glActiveTexture SDL_GL_GetProcAddress faild!", __FILE__, __LINE__); return false;}
    pglPixelStorei = (PFNGLPIXELSTOREIPROC)SDL_GL_GetProcAddress("glPixelStorei");
    if (!pglPixelStorei) { ZC_ErrorLogger::Err("glPixelStorei SDL_GL_GetProcAddress faild!", __FILE__, __LINE__); return false;}
    pglTexSubImage2D = (PFNGLTEXSUBIMAGE2DPROC)SDL_GL_GetProcAddress("glTexSubImage2D");
    if (!pglTexSubImage2D) { ZC_ErrorLogger::Err("glTexSubImage2D SDL_GL_GetProcAddress faild!", __FILE__, __LINE__); return false;}
    pglTexImage2DMultisample = (PFNGLTEXIMAGE2DMULTISAMPLEPROC)SDL_GL_GetProcAddress("glTexImage2DMultisample");
    if (!pglTexImage2DMultisample) { ZC_ErrorLogger::Err("glTexImage2DMultisample SDL_GL_GetProcAddress faild!", __FILE__, __LINE__); return false;}
    //  stencil
    pglStencilMask = (PFNGLSTENCILMASKPROC)SDL_GL_GetProcAddress("glStencilMask");
    if (!pglStencilMask) { ZC_ErrorLogger::Err("glStencilMask SDL_GL_GetProcAddress faild!", __FILE__, __LINE__); return false;}
    pglStencilFunc = (PFNGLSTENCILFUNCPROC)SDL_GL_GetProcAddress("glStencilFunc");
    if (!pglStencilFunc) { ZC_ErrorLogger::Err("glStencilFunc SDL_GL_GetProcAddress faild!", __FILE__, __LINE__); return false;}
    pglStencilOp = (PFNGLSTENCILOPPROC)SDL_GL_GetProcAddress("glStencilOp");
    if (!pglStencilOp) { ZC_ErrorLogger::Err("glStencilOp SDL_GL_GetProcAddress faild!", __FILE__, __LINE__); return false;}
    //  depth
    pglDepthFunc = (PFNGLDEPTHFUNCPROC)SDL_GL_GetProcAddress("glDepthFunc");
    if (!pglDepthFunc) { ZC_ErrorLogger::Err("glDepthFunc SDL_GL_GetProcAddress faild!", __FILE__, __LINE__); return false;}
    pglDepthMask = (PFNGLDEPTHMASKPROC)SDL_GL_GetProcAddress("glDepthMask");
    if (!pglDepthMask) { ZC_ErrorLogger::Err("glDepthMask SDL_GL_GetProcAddress faild!", __FILE__, __LINE__); return false;}
    //  framebuffer
    pglGenFramebuffers = (PFNGLGENFRAMEBUFFERSPROC)SDL_GL_GetProcAddress("glGenFramebuffers");
    if (!pglGenFramebuffers) { ZC_ErrorLogger::Err("glGenFramebuffers SDL_GL_GetProcAddress faild!", __FILE__, __LINE__); return false;}
    pglDeleteFramebuffers = (PFNGLDELETEFRAMEBUFFERSPROC)SDL_GL_GetProcAddress("glDeleteFramebuffers");
    if (!pglDeleteFramebuffers) { ZC_ErrorLogger::Err("glDeleteFramebuffers SDL_GL_GetProcAddress faild!", __FILE__, __LINE__); return false;}
    pglBindFramebuffer = (PFNGLBINDFRAMEBUFFERPROC)SDL_GL_GetProcAddress("glBindFramebuffer");
    if (!pglBindFramebuffer) { ZC_ErrorLogger::Err("glBindFramebuffer SDL_GL_GetProcAddress faild!", __FILE__, __LINE__); return false;}
    pglCheckFramebufferStatus = (PFNGLCHECKFRAMEBUFFERSTATUSPROC)SDL_GL_GetProcAddress("glCheckFramebufferStatus");
    if (!pglCheckFramebufferStatus) { ZC_ErrorLogger::Err("glCheckFramebufferStatus SDL_GL_GetProcAddress faild!", __FILE__, __LINE__); return false;}
    pglFramebufferTexture2D = (PFNGLFRAMEBUFFERTEXTURE2DPROC)SDL_GL_GetProcAddress("glFramebufferTexture2D");
    if (!pglFramebufferTexture2D) { ZC_ErrorLogger::Err("glFramebufferTexture2D SDL_GL_GetProcAddress faild!", __FILE__, __LINE__); return false;}
    pglFramebufferRenderbuffer = (PFNGLFRAMEBUFFERRENDERBUFFERPROC)SDL_GL_GetProcAddress("glFramebufferRenderbuffer");
    if (!pglFramebufferRenderbuffer) { ZC_ErrorLogger::Err("glFramebufferRenderbuffer SDL_GL_GetProcAddress faild!", __FILE__, __LINE__); return false;}
    pglDrawBuffer = (PFNGLDRAWBUFFERPROC)SDL_GL_GetProcAddress("glDrawBuffer");
    if (!pglDrawBuffer) { ZC_ErrorLogger::Err("glDrawBuffer SDL_GL_GetProcAddress faild!", __FILE__, __LINE__); return false;}
    pglReadBuffer = (PFNGLREADBUFFERPROC)SDL_GL_GetProcAddress("glReadBuffer");
    if (!pglReadBuffer) { ZC_ErrorLogger::Err("glReadBuffer SDL_GL_GetProcAddress faild!", __FILE__, __LINE__); return false;}
    pglBlitFramebuffer = (PFNGLBLITFRAMEBUFFERPROC)SDL_GL_GetProcAddress("glBlitFramebuffer");
    if (!pglBlitFramebuffer) { ZC_ErrorLogger::Err("glBlitFramebuffer SDL_GL_GetProcAddress faild!", __FILE__, __LINE__); return false;}
    //  renderbuffer
    pglGenRenderbuffers = (PFNGLGENRENDERBUFFERSPROC)SDL_GL_GetProcAddress("glGenRenderbuffers");
    if (!pglGenRenderbuffers) { ZC_ErrorLogger::Err("glGenRenderbuffers SDL_GL_GetProcAddress faild!", __FILE__, __LINE__); return false;}
    pglDeleteRenderbuffers = (PFNGLDELETERENDERBUFFERSPROC)SDL_GL_GetProcAddress("glDeleteRenderbuffers");
    if (!pglDeleteRenderbuffers) { ZC_ErrorLogger::Err("glDeleteRenderbuffers SDL_GL_GetProcAddress faild!", __FILE__, __LINE__); return false;}
    pglBindRenderbuffer = (PFNGLBINDRENDERBUFFERPROC)SDL_GL_GetProcAddress("glBindRenderbuffer");
    if (!pglBindRenderbuffer) { ZC_ErrorLogger::Err("glBindRenderbuffer SDL_GL_GetProcAddress faild!", __FILE__, __LINE__); return false;}
    pglRenderbufferStorage = (PFNGLRENDERBUFFERSTORAGEPROC)SDL_GL_GetProcAddress("glRenderbufferStorage");
    if (!pglRenderbufferStorage) { ZC_ErrorLogger::Err("glRenderbufferStorage SDL_GL_GetProcAddress faild!", __FILE__, __LINE__); return false;}
    pglRenderbufferStorageMultisample = (PFNGLRENDERBUFFERSTORAGEMULTISAMPLEPROC)SDL_GL_GetProcAddress("glRenderbufferStorageMultisample");
    if (!pglRenderbufferStorageMultisample) { ZC_ErrorLogger::Err("glRenderbufferStorageMultisample SDL_GL_GetProcAddress faild!", __FILE__, __LINE__); return false;}
    
    // auto q = glGetString(GL_VERSION);
    // std::string a(reinterpret_cast<const char *>(glGetString(GL_VERSION)));
    // std::string b(reinterpret_cast<const char *>(glGetString(GL_VENDOR)));
    // std::string c(reinterpret_cast<const char *>(glGetString(GL_EXTENSIONS)));

    return true;
}

//  window
PFNGLVIEWPORTPROC pglViewport = nullptr;
PFNGLCLEARPROC pglClear = nullptr;
PFNGLCLEARCOLORPROC pglClearColor = nullptr;

//  get/read
PFNGLGETSTRINGPROC pglGetString = nullptr;
PFNGLGETINTEGERVPROC pglGetIntegerv = nullptr;
PFNGLREADPIXELSPROC pglReadPixels = nullptr;

//  enble
PFNGLENABLEPROC pglEnable = nullptr;
PFNGLDISABLEPROC pglDisable = nullptr;
PFNGLBLENDFUNCPROC pglBlendFunc = nullptr;

//  debug
PFNGLDEBUGMESSAGECALLBACKPROC pglDebugMessageCallback = nullptr;
PFNGLDEBUGMESSAGECONTROLPROC pglDebugMessageControl = nullptr;

//  shader
PFNGLCREATEPROGRAMPROC pglCreateProgram = nullptr;
PFNGLATTACHSHADERPROC pglAttachShader = nullptr;
PFNGLLINKPROGRAMPROC pglLinkProgram = nullptr;
PFNGLGETPROGRAMIVPROC pglGetProgramiv = nullptr;
PFNGLGETPROGRAMINFOLOGPROC pglGetProgramInfoLog = nullptr;
PFNGLDELETESHADERPROC pglDeleteShader = nullptr;
PFNGLCREATESHADERPROC pglCreateShader = nullptr;
PFNGLSHADERSOURCEPROC pglShaderSource = nullptr;
PFNGLCOMPILESHADERPROC pglCompileShader = nullptr;
PFNGLGETSHADERIVPROC pglGetShaderiv = nullptr;
PFNGLGETSHADERINFOLOGPROC pglGetShaderInfoLog = nullptr;
PFNGLDELETEPROGRAMPROC pglDeleteProgram = nullptr;
PFNGLUSEPROGRAMPROC pglUseProgram = nullptr;
PFNGLGETUNIFORMLOCATIONPROC pglGetUniformLocation = nullptr;
PFNGLUNIFORM1FPROC pglUniform1f = nullptr;
PFNGLUNIFORM1IPROC pglUniform1i = nullptr;
PFNGLUNIFORM1UIPROC pglUniform1ui = nullptr;
PFNGLUNIFORM1FVPROC pglUniform1fv = nullptr;
PFNGLUNIFORM2FVPROC pglUniform2fv = nullptr;
PFNGLUNIFORM3FVPROC pglUniform3fv = nullptr;
PFNGLUNIFORM4FVPROC pglUniform4fv = nullptr;
PFNGLUNIFORM1IVPROC pglUniform1iv = nullptr;
PFNGLUNIFORM2IVPROC pglUniform2iv = nullptr;
PFNGLUNIFORM3IVPROC pglUniform3iv = nullptr;
PFNGLUNIFORM4IVPROC pglUniform4iv = nullptr;
PFNGLUNIFORM1UIVPROC pglUniform1uiv = nullptr;
PFNGLUNIFORM2UIVPROC pglUniform2uiv = nullptr;
PFNGLUNIFORM3UIVPROC pglUniform3uiv = nullptr;
PFNGLUNIFORM4UIVPROC pglUniform4uiv = nullptr;
PFNGLUNIFORMMATRIX2FVPROC pglUniformMatrix2fv = nullptr;
PFNGLUNIFORMMATRIX3FVPROC pglUniformMatrix3fv = nullptr;
PFNGLUNIFORMMATRIX4FVPROC pglUniformMatrix4fv = nullptr;
PFNGLUNIFORMMATRIX2X3FVPROC pglUniformMatrix2x3fv = nullptr;
PFNGLUNIFORMMATRIX3X2FVPROC pglUniformMatrix3x2fv = nullptr;
PFNGLUNIFORMMATRIX2X4FVPROC pglUniformMatrix2x4fv = nullptr;
PFNGLUNIFORMMATRIX4X2FVPROC pglUniformMatrix4x2fv = nullptr;
PFNGLUNIFORMMATRIX3X4FVPROC pglUniformMatrix3x4fv = nullptr;
PFNGLUNIFORMMATRIX4X3FVPROC pglUniformMatrix4x3fv = nullptr;

//  buffer
PFNGLGENBUFFERSPROC pglGenBuffers = nullptr;
PFNGLDELETEBUFFERSPROC pglDeleteBuffers = nullptr;
PFNGLBINDBUFFERPROC pglBindBuffer = nullptr;
PFNGLBUFFERDATAPROC pglBufferData = nullptr;
PFNGLBUFFERSUBDATAPROC pglBufferSubData = nullptr;
PFNGLBINDBUFFERRANGEPROC pglBindBufferRange = nullptr;

//  vbo
PFNGLBINDVERTEXBUFFERPROC pglBindVertexBuffer = nullptr;

//  vao
PFNGLGENVERTEXARRAYSPROC pglGenVertexArrays = nullptr;
PFNGLDELETEVERTEXARRAYSPROC pglDeleteVertexArrays = nullptr;
PFNGLBINDVERTEXARRAYPROC pglBindVertexArray = nullptr;
PFNGLENABLEVERTEXATTRIBARRAYPROC pglEnableVertexAttribArray = nullptr;
PFNGLDISABLEVERTEXATTRIBARRAYPROC pglDisableVertexAttribArray = nullptr;
PFNGLVERTEXATTRIBPOINTERPROC pglVertexAttribPointer = nullptr;
PFNGLDRAWARRAYSPROC pglDrawArrays = nullptr;
PFNGLDRAWELEMENTSPROC pglDrawElements = nullptr;
PFNGLVERTEXATTRIBFORMATPROC pglVertexAttribFormat = nullptr;
PFNGLVERTEXATTRIBBINDINGPROC pglVertexAttribBinding = nullptr;

// //  va
// PFNGLCREATEVERTEXARRAYSPROC pglCreateVertexArrays = nullptr;
// PFNGLENABLEVERTEXARRAYATTRIBPROC pglEnableVertexArrayAttrib = nullptr;
// PFNGLVERTEXARRAYATTRIBFORMATPROC pglVertexArrayAttribFormat = nullptr;
// PFNGLVERTEXARRAYATTRIBBINDINGPROC pglVertexArrayAttribBinding = nullptr;
// PFNGLVERTEXARRAYVERTEXBUFFERPROC pglVertexArrayVertexBuffer = nullptr;

//  texture
PFNGLGENTEXTURESPROC pglGenTextures = nullptr;
PFNGLDELETETEXTURESPROC pglDeleteTextures = nullptr;
PFNGLBINDTEXTUREPROC pglBindTexture = nullptr;
PFNGLTEXPARAMETERIPROC pglTexParameteri = nullptr;
PFNGLTEXIMAGE2DPROC pglTexImage2D = nullptr;
PFNGLGENERATEMIPMAPPROC pglGenerateMipmap = nullptr;
PFNGLACTIVETEXTUREPROC pglActiveTexture = nullptr;
PFNGLPIXELSTOREIPROC pglPixelStorei = nullptr;
PFNGLTEXSUBIMAGE2DPROC pglTexSubImage2D = nullptr;
PFNGLTEXIMAGE2DMULTISAMPLEPROC pglTexImage2DMultisample = nullptr;

//  stencil
PFNGLSTENCILMASKPROC pglStencilMask = nullptr;
PFNGLSTENCILFUNCPROC pglStencilFunc = nullptr;
PFNGLSTENCILOPPROC pglStencilOp = nullptr;

//  depth
PFNGLDEPTHFUNCPROC pglDepthFunc = nullptr;
PFNGLDEPTHMASKPROC pglDepthMask = nullptr;

//  framebuffer
PFNGLGENFRAMEBUFFERSPROC pglGenFramebuffers = nullptr;
PFNGLDELETEFRAMEBUFFERSPROC pglDeleteFramebuffers = nullptr;
PFNGLBINDFRAMEBUFFERPROC pglBindFramebuffer = nullptr;
PFNGLCHECKFRAMEBUFFERSTATUSPROC pglCheckFramebufferStatus = nullptr;
PFNGLFRAMEBUFFERTEXTURE2DPROC pglFramebufferTexture2D = nullptr;
PFNGLFRAMEBUFFERRENDERBUFFERPROC pglFramebufferRenderbuffer = nullptr;
PFNGLDRAWBUFFERPROC pglDrawBuffer = nullptr;
PFNGLREADBUFFERPROC pglReadBuffer = nullptr;
PFNGLBLITFRAMEBUFFERPROC pglBlitFramebuffer = nullptr;

//  renderbuffer
PFNGLGENRENDERBUFFERSPROC pglGenRenderbuffers = nullptr;
PFNGLDELETERENDERBUFFERSPROC pglDeleteRenderbuffers = nullptr;
PFNGLBINDRENDERBUFFERPROC pglBindRenderbuffer = nullptr;
PFNGLRENDERBUFFERSTORAGEPROC pglRenderbufferStorage = nullptr;
PFNGLRENDERBUFFERSTORAGEMULTISAMPLEPROC pglRenderbufferStorageMultisample = nullptr;