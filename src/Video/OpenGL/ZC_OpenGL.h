#pragma once

#include <ZC_Config.h>

#define ZC_OPEN_GL_COLLOR_BUFFER_SIZE 8
#define ZC_OPEN_GL_DEPTH_BUFFER_SIZE 24

void ZC_OpenGLAssigneErrorCallback();

#ifdef ZC_ANDROID
#include <GLES3/gl32.h>
#define ZC_OPEN_GL_MAJOR_VERSION 3
#define ZC_OPEN_GL_MINOR_VERSION 2
#elif defined ZC_PC
#include <ZC/Video/OpenGL/GL/glcorearb.h>
#define ZC_OPEN_GL_MAJOR_VERSION 4
#define ZC_OPEN_GL_MINOR_VERSION 6

//  window

extern PFNGLVIEWPORTPROC pglViewport;
#define glViewport pglViewport

extern PFNGLCLEARPROC pglClear;
#define glClear pglClear

extern PFNGLCLEARCOLORPROC pglClearColor;
#define glClearColor pglClearColor

//  get

extern PFNGLGETSTRINGPROC pglGetString;
#define glGetString pglGetString

extern PFNGLGETINTEGERVPROC pglGetIntegerv;
#define glGetIntegerv pglGetIntegerv

//  enable

extern PFNGLENABLEPROC pglEnable;
#define glEnable pglEnable

//  debug

extern PFNGLDEBUGMESSAGECALLBACKPROC pglDebugMessageCallback;
#define glDebugMessageCallback pglDebugMessageCallback

extern PFNGLDEBUGMESSAGECONTROLPROC pglDebugMessageControl;
#define glDebugMessageControl pglDebugMessageControl

//  shader

extern PFNGLCREATEPROGRAMPROC pglCreateProgram;
#define glCreateProgram pglCreateProgram

extern PFNGLATTACHSHADERPROC pglAttachShader;
#define glAttachShader pglAttachShader

extern PFNGLLINKPROGRAMPROC pglLinkProgram;
#define glLinkProgram pglLinkProgram

extern PFNGLGETPROGRAMIVPROC pglGetProgramiv;
#define glGetProgramiv pglGetProgramiv

extern PFNGLGETPROGRAMINFOLOGPROC pglGetProgramInfoLog;
#define glGetProgramInfoLog pglGetProgramInfoLog

extern PFNGLDELETESHADERPROC pglDeleteShader;
#define glDeleteShader pglDeleteShader

extern PFNGLCREATESHADERPROC pglCreateShader;
#define glCreateShader pglCreateShader

extern PFNGLSHADERSOURCEPROC pglShaderSource;
#define glShaderSource pglShaderSource

extern PFNGLCOMPILESHADERPROC pglCompileShader;
#define glCompileShader pglCompileShader

extern PFNGLGETSHADERIVPROC pglGetShaderiv;
#define glGetShaderiv pglGetShaderiv

extern PFNGLGETSHADERINFOLOGPROC pglGetShaderInfoLog;
#define glGetShaderInfoLog pglGetShaderInfoLog

extern PFNGLDELETEPROGRAMPROC pglDeleteProgram;
#define glDeleteProgram pglDeleteProgram

extern PFNGLUSEPROGRAMPROC pglUseProgram;
#define glUseProgram pglUseProgram

extern PFNGLGETUNIFORMLOCATIONPROC pglGetUniformLocation;
#define glGetUniformLocation pglGetUniformLocation

extern PFNGLUNIFORMMATRIX4FVPROC pglUniformMatrix4fv;
#define glUniformMatrix4fv pglUniformMatrix4fv

//  buffer

extern PFNGLGENBUFFERSPROC pglGenBuffers;
#define glGenBuffers pglGenBuffers

extern PFNGLDELETEBUFFERSPROC pglDeleteBuffers;
#define glDeleteBuffers pglDeleteBuffers

extern PFNGLBINDBUFFERPROC pglBindBuffer;
#define glBindBuffer pglBindBuffer

extern PFNGLBUFFERSUBDATAPROC pglBufferSubData;
#define glBufferSubData pglBufferSubData

extern PFNGLBINDBUFFERRANGEPROC pglBindBufferRange;
#define glBindBufferRange pglBindBufferRange

//  vbo

extern PFNGLBINDVERTEXBUFFERPROC pglBindVertexBuffer; 
#define glBindVertexBuffer pglBindVertexBuffer

//  vao

extern PFNGLGENVERTEXARRAYSPROC pglGenVertexArrays;
#define glGenVertexArrays pglGenVertexArrays

extern PFNGLDELETEVERTEXARRAYSPROC pglDeleteVertexArrays;
#define glDeleteVertexArrays pglDeleteVertexArrays

extern PFNGLBINDVERTEXARRAYPROC pglBindVertexArray;
#define glBindVertexArray pglBindVertexArray

extern PFNGLBUFFERDATAPROC pglBufferData;
#define glBufferData pglBufferData

extern PFNGLENABLEVERTEXATTRIBARRAYPROC pglEnableVertexAttribArray;
#define glEnableVertexAttribArray pglEnableVertexAttribArray

extern PFNGLDISABLEVERTEXATTRIBARRAYPROC pglDisableVertexAttribArray;
#define glDisableVertexAttribArray pglDisableVertexAttribArray

extern PFNGLVERTEXATTRIBPOINTERPROC pglVertexAttribPointer;
#define glVertexAttribPointer pglVertexAttribPointer

extern PFNGLDRAWARRAYSPROC pglDrawArrays;
#define glDrawArrays pglDrawArrays

extern PFNGLVERTEXATTRIBFORMATPROC pglVertexAttribFormat;
#define glVertexAttribFormat pglVertexAttribFormat

extern PFNGLVERTEXATTRIBBINDINGPROC pglVertexAttribBinding;
#define glVertexAttribBinding pglVertexAttribBinding

// //  va gles3.2.0 no support https://itecnote.com/tecnote/c-the-role-of-glbindvertexarrays-vs-glbindbuffer-and-what-is-their-relationship/
// extern PFNGLCREATEVERTEXARRAYSPROC pglCreateVertexArrays;
// #define glCreateVertexArrays pglCreateVertexArrays

// extern PFNGLENABLEVERTEXARRAYATTRIBPROC pglEnableVertexArrayAttrib;
// #define glEnableVertexArrayAttrib pglEnableVertexArrayAttrib

// extern PFNGLVERTEXARRAYATTRIBFORMATPROC pglVertexArrayAttribFormat;
// #define glVertexArrayAttribFormat pglVertexArrayAttribFormat

// extern PFNGLVERTEXARRAYATTRIBBINDINGPROC pglVertexArrayAttribBinding;
// #define glVertexArrayAttribBinding pglVertexArrayAttribBinding

// extern PFNGLVERTEXARRAYVERTEXBUFFERPROC pglVertexArrayVertexBuffer;
// #define glVertexArrayVertexBuffer pglVertexArrayVertexBuffer
#endif

// extern PFNGL PROC ;
// #define 



