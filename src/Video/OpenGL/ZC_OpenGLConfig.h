#pragma once

#include <ZC_Config.h>

#define ZC_OPEN_GL_COLLOR_BUFFER_SIZE 8
#define ZC_OPEN_GL_DEPTH_BUFFER_SIZE 24
#define ZC_OPEN_GL_STENCIL_BUFFER_SIZE 8

void ZC_OpenGLAssigneErrorCallback();

#ifdef ZC_ANDROID
#include <GLES3/gl32.h>
#define ZC_OPEN_GL_MAJOR_VERSION 3
#define ZC_OPEN_GL_MINOR_VERSION 2
#elif defined ZC_PC
// #include <ZC/Video/OpenGL/GL/glcorearb.h>
#define ZC_OPEN_GL_MAJOR_VERSION 4
#define ZC_OPEN_GL_MINOR_VERSION 6

// //  window

// extern PFNGLVIEWPORTPROC pglViewport;
// #define glViewport pglViewport

// extern PFNGLCLEARPROC pglClear;
// #define glClear pglClear

// extern PFNGLCLEARCOLORPROC pglClearColor;
// #define glClearColor pglClearColor

// //  get/read

// extern PFNGLGETSTRINGPROC pglGetString;
// #define glGetString pglGetString

// extern PFNGLGETINTEGERVPROC pglGetIntegerv;
// #define glGetIntegerv pglGetIntegerv

// extern PFNGLREADPIXELSPROC pglReadPixels;
// #define glReadPixels pglReadPixels

// //  enable

// extern PFNGLENABLEPROC pglEnable;
// #define glEnable pglEnable

// extern PFNGLDISABLEPROC pglDisable;
// #define glDisable pglDisable

// //  blend

// extern PFNGLBLENDFUNCPROC pglBlendFunc;
// #define glBlendFunc pglBlendFunc

// extern PFNGLBLENDCOLORPROC pglBlendColor;
// #define glBlendColor pglBlendColor

// //  debug

// extern PFNGLDEBUGMESSAGECALLBACKPROC pglDebugMessageCallback;
// #define glDebugMessageCallback pglDebugMessageCallback

// extern PFNGLDEBUGMESSAGECONTROLPROC pglDebugMessageControl;
// #define glDebugMessageControl pglDebugMessageControl

// //  shader

// extern PFNGLCREATEPROGRAMPROC pglCreateProgram;
// #define glCreateProgram pglCreateProgram

// extern PFNGLATTACHSHADERPROC pglAttachShader;
// #define glAttachShader pglAttachShader

// extern PFNGLLINKPROGRAMPROC pglLinkProgram;
// #define glLinkProgram pglLinkProgram

// extern PFNGLGETPROGRAMIVPROC pglGetProgramiv;
// #define glGetProgramiv pglGetProgramiv

// extern PFNGLGETPROGRAMINFOLOGPROC pglGetProgramInfoLog;
// #define glGetProgramInfoLog pglGetProgramInfoLog

// extern PFNGLDELETESHADERPROC pglDeleteShader;
// #define glDeleteShader pglDeleteShader

// extern PFNGLCREATESHADERPROC pglCreateShader;
// #define glCreateShader pglCreateShader

// extern PFNGLSHADERSOURCEPROC pglShaderSource;
// #define glShaderSource pglShaderSource

// extern PFNGLCOMPILESHADERPROC pglCompileShader;
// #define glCompileShader pglCompileShader

// extern PFNGLGETSHADERIVPROC pglGetShaderiv;
// #define glGetShaderiv pglGetShaderiv

// extern PFNGLGETSHADERINFOLOGPROC pglGetShaderInfoLog;
// #define glGetShaderInfoLog pglGetShaderInfoLog

// extern PFNGLDELETEPROGRAMPROC pglDeleteProgram;
// #define glDeleteProgram pglDeleteProgram

// extern PFNGLUSEPROGRAMPROC pglUseProgram;
// #define glUseProgram pglUseProgram

// extern PFNGLGETUNIFORMLOCATIONPROC pglGetUniformLocation;
// #define glGetUniformLocation pglGetUniformLocation

// extern PFNGLUNIFORM1FPROC pglUniform1f;
// #define glUniform1f pglUniform1f

// extern PFNGLUNIFORM1IPROC pglUniform1i;
// #define glUniform1i pglUniform1i

// extern PFNGLUNIFORM1UIPROC pglUniform1ui;
// #define glUniform1ui pglUniform1ui

// extern PFNGLUNIFORM1FVPROC pglUniform1fv;
// #define glUniform1fv pglUniform1fv

// extern PFNGLUNIFORM2FVPROC pglUniform2fv;
// #define glUniform2fv pglUniform2fv

// extern PFNGLUNIFORM3FVPROC pglUniform3fv;
// #define glUniform3fv pglUniform3fv

// extern PFNGLUNIFORM4FVPROC pglUniform4fv;
// #define glUniform4fv pglUniform4fv

// extern PFNGLUNIFORM1IVPROC pglUniform1iv;
// #define glUniform1iv pglUniform1iv

// extern PFNGLUNIFORM2IVPROC pglUniform2iv;
// #define glUniform2iv pglUniform2iv

// extern PFNGLUNIFORM3IVPROC pglUniform3iv;
// #define glUniform3iv pglUniform3iv

// extern PFNGLUNIFORM4IVPROC pglUniform4iv;
// #define glUniform4iv pglUniform4iv

// extern PFNGLUNIFORM1UIVPROC pglUniform1uiv;
// #define glUniform1uiv pglUniform1uiv

// extern PFNGLUNIFORM2UIVPROC pglUniform2uiv;
// #define glUniform2uiv pglUniform2uiv

// extern PFNGLUNIFORM3UIVPROC pglUniform3uiv;
// #define glUniform3uiv pglUniform3uiv

// extern PFNGLUNIFORM4UIVPROC pglUniform4uiv;
// #define glUniform4uiv pglUniform4uiv

// extern PFNGLUNIFORMMATRIX2FVPROC pglUniformMatrix2fv;
// #define glUniformMatrix2fv pglUniformMatrix2fv

// extern PFNGLUNIFORMMATRIX3FVPROC pglUniformMatrix3fv;
// #define glUniformMatrix3fv pglUniformMatrix3fv

// extern PFNGLUNIFORMMATRIX4FVPROC pglUniformMatrix4fv;
// #define glUniformMatrix4fv pglUniformMatrix4fv

// extern PFNGLUNIFORMMATRIX2X3FVPROC pglUniformMatrix2x3fv;
// #define glUniformMatrix2x3fv pglUniformMatrix2x3fv

// extern PFNGLUNIFORMMATRIX3X2FVPROC pglUniformMatrix3x2fv;
// #define glUniformMatrix3x2fv pglUniformMatrix3x2fv

// extern PFNGLUNIFORMMATRIX2X4FVPROC pglUniformMatrix2x4fv;
// #define glUniformMatrix2x4fv pglUniformMatrix2x4fv

// extern PFNGLUNIFORMMATRIX4X2FVPROC pglUniformMatrix4x2fv;
// #define glUniformMatrix4x2fv pglUniformMatrix4x2fv

// extern PFNGLUNIFORMMATRIX3X4FVPROC pglUniformMatrix3x4fv;
// #define glUniformMatrix3x4fv pglUniformMatrix3x4fv

// extern PFNGLUNIFORMMATRIX4X3FVPROC pglUniformMatrix4x3fv;
// #define glUniformMatrix4x3fv pglUniformMatrix4x3fv

// //  buffer

// extern PFNGLGENBUFFERSPROC pglGenBuffers;
// #define glGenBuffers pglGenBuffers

// extern PFNGLDELETEBUFFERSPROC pglDeleteBuffers;
// #define glDeleteBuffers pglDeleteBuffers

// extern PFNGLBINDBUFFERPROC pglBindBuffer;
// #define glBindBuffer pglBindBuffer

// extern PFNGLBUFFERSUBDATAPROC pglBufferSubData;
// #define glBufferSubData pglBufferSubData

// extern PFNGLBINDBUFFERRANGEPROC pglBindBufferRange;
// #define glBindBufferRange pglBindBufferRange

// //  vbo

// extern PFNGLBINDVERTEXBUFFERPROC pglBindVertexBuffer; 
// #define glBindVertexBuffer pglBindVertexBuffer

// //  vao

// extern PFNGLGENVERTEXARRAYSPROC pglGenVertexArrays;
// #define glGenVertexArrays pglGenVertexArrays

// extern PFNGLDELETEVERTEXARRAYSPROC pglDeleteVertexArrays;
// #define glDeleteVertexArrays pglDeleteVertexArrays

// extern PFNGLBINDVERTEXARRAYPROC pglBindVertexArray;
// #define glBindVertexArray pglBindVertexArray

// extern PFNGLBUFFERDATAPROC pglBufferData;
// #define glBufferData pglBufferData

// extern PFNGLENABLEVERTEXATTRIBARRAYPROC pglEnableVertexAttribArray;
// #define glEnableVertexAttribArray pglEnableVertexAttribArray

// extern PFNGLDISABLEVERTEXATTRIBARRAYPROC pglDisableVertexAttribArray;
// #define glDisableVertexAttribArray pglDisableVertexAttribArray

// extern PFNGLVERTEXATTRIBPOINTERPROC pglVertexAttribPointer;
// #define glVertexAttribPointer pglVertexAttribPointer

// extern PFNGLVERTEXATTRIBFORMATPROC pglVertexAttribFormat;
// #define glVertexAttribFormat pglVertexAttribFormat

// extern PFNGLVERTEXATTRIBBINDINGPROC pglVertexAttribBinding;
// #define glVertexAttribBinding pglVertexAttribBinding

// // //  va gles3.2.0 no support https://itecnote.com/tecnote/c-the-role-of-glbindvertexarrays-vs-glbindbuffer-and-what-is-their-relationship/
// // extern PFNGLCREATEVERTEXARRAYSPROC pglCreateVertexArrays;
// // #define glCreateVertexArrays pglCreateVertexArrays

// // extern PFNGLENABLEVERTEXARRAYATTRIBPROC pglEnableVertexArrayAttrib;
// // #define glEnableVertexArrayAttrib pglEnableVertexArrayAttrib

// // extern PFNGLVERTEXARRAYATTRIBFORMATPROC pglVertexArrayAttribFormat;
// // #define glVertexArrayAttribFormat pglVertexArrayAttribFormat

// // extern PFNGLVERTEXARRAYATTRIBBINDINGPROC pglVertexArrayAttribBinding;
// // #define glVertexArrayAttribBinding pglVertexArrayAttribBinding

// // extern PFNGLVERTEXARRAYVERTEXBUFFERPROC pglVertexArrayVertexBuffer;
// // #define glVertexArrayVertexBuffer pglVertexArrayVertexBuffer

// //  draw

// extern PFNGLDRAWARRAYSPROC pglDrawArrays;
// #define glDrawArrays pglDrawArrays

// extern PFNGLDRAWELEMENTSPROC pglDrawElements;
// #define glDrawElements pglDrawElements

// extern PFNGLDRAWARRAYSINSTANCEDPROC pglDrawArraysInstanced;
// #define glDrawArraysInstanced pglDrawArraysInstanced

// extern PFNGLDRAWELEMENTSINSTANCEDPROC pglDrawElementsInstanced;
// #define glDrawElementsInstanced pglDrawElementsInstanced

// //  texture

// extern PFNGLGENTEXTURESPROC pglGenTextures;
// #define glGenTextures pglGenTextures

// extern PFNGLDELETETEXTURESPROC pglDeleteTextures;
// #define glDeleteTextures pglDeleteTextures

// extern PFNGLBINDTEXTUREPROC pglBindTexture;
// #define glBindTexture pglBindTexture

// extern PFNGLTEXPARAMETERIPROC pglTexParameteri;
// #define glTexParameteri pglTexParameteri

// extern PFNGLTEXIMAGE2DPROC pglTexImage2D;
// #define glTexImage2D pglTexImage2D

// extern PFNGLGENERATEMIPMAPPROC pglGenerateMipmap;
// #define glGenerateMipmap pglGenerateMipmap

// extern PFNGLACTIVETEXTUREPROC pglActiveTexture;
// #define glActiveTexture pglActiveTexture

// extern PFNGLPIXELSTOREIPROC pglPixelStorei;
// #define glPixelStorei pglPixelStorei

// extern PFNGLTEXSUBIMAGE2DPROC pglTexSubImage2D;
// #define glTexSubImage2D pglTexSubImage2D

// extern PFNGLTEXIMAGE2DMULTISAMPLEPROC pglTexImage2DMultisample;
// #define glTexImage2DMultisample pglTexImage2DMultisample


// // extern PFNGLCREATETEXTURESPROC pglCreateTextures;
// // #define glCreateTextures pglCreateTextures

// // extern PFNGLTEXTUREPARAMETERIPROC pglTextureParameteri;
// // #define glTextureParameteri pglTextureParameteri

// // extern PFNGLTEXTURESTORAGE2DPROC pglTextureStorage2D;
// // #define glTextureStorage2D pglTextureStorage2D

// // extern PFNGLTEXTURESUBIMAGE2DPROC pglTextureSubImage2D;
// // #define glTextureSubImage2D pglTextureSubImage2D

// //  stencil

// extern PFNGLSTENCILMASKPROC pglStencilMask; 
// #define glStencilMask pglStencilMask

// extern PFNGLSTENCILFUNCPROC pglStencilFunc; 
// #define glStencilFunc pglStencilFunc

// extern PFNGLSTENCILOPPROC pglStencilOp; 
// #define glStencilOp pglStencilOp

// //  depth

// extern PFNGLDEPTHFUNCPROC pglDepthFunc; 
// #define glDepthFunc pglDepthFunc

// extern PFNGLDEPTHMASKPROC pglDepthMask;
// #define glDepthMask pglDepthMask

// //  framebuffer

// extern PFNGLGENFRAMEBUFFERSPROC pglGenFramebuffers;
// #define glGenFramebuffers pglGenFramebuffers

// extern PFNGLDELETEFRAMEBUFFERSPROC pglDeleteFramebuffers;
// #define glDeleteFramebuffers pglDeleteFramebuffers

// extern PFNGLBINDFRAMEBUFFERPROC pglBindFramebuffer;
// #define glBindFramebuffer pglBindFramebuffer

// extern PFNGLCHECKFRAMEBUFFERSTATUSPROC pglCheckFramebufferStatus;
// #define glCheckFramebufferStatus pglCheckFramebufferStatus

// extern PFNGLFRAMEBUFFERTEXTURE2DPROC pglFramebufferTexture2D;
// #define glFramebufferTexture2D pglFramebufferTexture2D

// extern PFNGLFRAMEBUFFERRENDERBUFFERPROC pglFramebufferRenderbuffer;
// #define glFramebufferRenderbuffer pglFramebufferRenderbuffer

// extern PFNGLDRAWBUFFERPROC pglDrawBuffer;
// #define glDrawBuffer pglDrawBuffer

// extern PFNGLREADBUFFERPROC pglReadBuffer;
// #define glReadBuffer pglReadBuffer

// extern PFNGLBLITFRAMEBUFFERPROC pglBlitFramebuffer;
// #define glBlitFramebuffer pglBlitFramebuffer

// //  renderbuffer

// extern PFNGLGENRENDERBUFFERSPROC pglGenRenderbuffers;
// #define glGenRenderbuffers pglGenRenderbuffers

// extern PFNGLDELETERENDERBUFFERSPROC pglDeleteRenderbuffers;
// #define glDeleteRenderbuffers pglDeleteRenderbuffers

// extern PFNGLBINDRENDERBUFFERPROC pglBindRenderbuffer;
// #define glBindRenderbuffer pglBindRenderbuffer

// extern PFNGLRENDERBUFFERSTORAGEPROC pglRenderbufferStorage;
// #define glRenderbufferStorage pglRenderbufferStorage

// extern PFNGLRENDERBUFFERSTORAGEMULTISAMPLEPROC pglRenderbufferStorageMultisample;
// #define glRenderbufferStorageMultisample pglRenderbufferStorageMultisample

#endif

// // extern PFNGL ;
// // #define 




//                                                        speed test

// #include <ZC/Tools/Time/ZC_Clock.h>
// #include <iostream>
// {
//     ZCR_ShaderKeeper sm;
//     auto p = ZC_ShaderManager::GetPipeLine(ZCR_SV_colorFigure, ZCR_STC_None, ZCR_STE_None, ZCR_SG_None, ZCR_SF_color);

//     GLuint buf = 0;
//     glCreateBuffers(1, &buf);
//     glNamedBufferStorage(buf, 100, nullptr, 0);

//     ZC_Clock cl;
//     size_t count = 1000000;

//     GLuint vaoShared;
//     glCreateVertexArrays(1, &vaoShared);

//     cl.Start();
//     glBindVertexArray(vaoShared);     //  vao don't uses (bind only on frame start)
//     for (size_t i = 0; i < count; i++)
//     {
//         //  first object
//         glBindProgramPipeline(1);
//         glVertexArrayElementBuffer(vaoShared, buf);
//         glBindBufferBase(GL_UNIFORM_BUFFER, 0, buf);    //  next 2 lines comments is now all data in one buffer (VNCVNC)
//         // glBindBufferBase(GL_UNIFORM_BUFFER, 1, ubo);
//         // glBindBufferBase(GL_UNIFORM_BUFFER, 2, ubo);
//         //  draw triangle
//         //  second object
//         glVertexArrayElementBuffer(vaoShared, buf);
//         glBindBufferBase(GL_UNIFORM_BUFFER, 0, buf);    //  next 2 lines comments is now all data in one buffer (VNCVNC)
//         // glBindBufferBase(GL_UNIFORM_BUFFER, 1, ubo);
//         // glBindBufferBase(GL_UNIFORM_BUFFER, 2, ubo);
//         //  draw triangle


//         //  first object
//         glBindProgramPipeline(1);
//         glVertexArrayElementBuffer(vaoShared, buf);
//         glBindBufferBase(GL_UNIFORM_BUFFER, 0, buf);    //  next 2 lines comments is now all data in one buffer (VNCVNC)
//         // glBindBufferBase(GL_UNIFORM_BUFFER, 1, ubo);
//         // glBindBufferBase(GL_UNIFORM_BUFFER, 2, ubo);
//         //  draw line
//         //  second object
//         glVertexArrayElementBuffer(vaoShared, buf);
//         glBindBufferBase(GL_UNIFORM_BUFFER, 0, buf);    //  next 2 lines comments is now all data in one buffer (VNCVNC)
//         // glBindBufferBase(GL_UNIFORM_BUFFER, 1, ubo);
//         // glBindBufferBase(GL_UNIFORM_BUFFER, 2, ubo);
//         //  draw line
//     }
//     std::cout<<cl.Time<ZC_Nanoseconds>()<<"             1"<<std::endl;

//     cl.Start();
//     glBindVertexArray(vaoShared);     //  vao don't uses (bind only on frame start)
//     for (size_t i = 0; i < count; i++)
//     {
//         //  first object
//         glBindProgramPipeline(1);
//         glVertexArrayElementBuffer(vaoShared, buf);
//         glBindBufferBase(GL_UNIFORM_BUFFER, 0, buf);    //  next 2 lines comments is now all data in one buffer (VNCVNC)
//         // glBindBufferBase(GL_UNIFORM_BUFFER, 1, ubo);
//         // glBindBufferBase(GL_UNIFORM_BUFFER, 2, ubo);
//         //  draw triangle
//         glBindProgramPipeline(1);
//         //  draw line


//         //  second object
//         glBindProgramPipeline(1);
//         glVertexArrayElementBuffer(vaoShared, buf);
//         glBindBufferBase(GL_UNIFORM_BUFFER, 0, buf);    //  next 2 lines comments is now all data in one buffer (VNCVNC)
//         // glBindBufferBase(GL_UNIFORM_BUFFER, 1, ubo);
//         // glBindBufferBase(GL_UNIFORM_BUFFER, 2, ubo);
//         //  draw triangle
//         glBindProgramPipeline(1);
//         //  draw line
//     }
//     std::cout<<cl.Time<ZC_Nanoseconds>()<<"             2"<<std::endl;

//     glEnableVertexArrayAttrib(vaoShared, 0);
//     glVertexArrayAttribFormat(vaoShared, 0, 3, GL_FLOAT, GL_FALSE, 0);
//     glVertexArrayAttribBinding(vaoShared, 0, 0);
//     glEnableVertexArrayAttrib(vaoShared, 1);
//     glVertexArrayAttribFormat(vaoShared, 1, 3, GL_FLOAT, GL_FALSE, 12);
//     glVertexArrayAttribBinding(vaoShared, 1, 1);
//     glEnableVertexArrayAttrib(vaoShared, 2);
//     glVertexArrayAttribFormat(vaoShared, 2, 3, GL_FLOAT, GL_FALSE, 24);
//     glVertexArrayAttribBinding(vaoShared, 2, 2);

//     GLuint buffers[] {buf, buf, buf};
//     GLintptr offsets[] {0,0,0};
//     GLsizei strides[] {0,0,0};

//     cl.Start();
//     for (size_t i = 0; i < count; i++)
//     {
//         glBindVertexArray(vaoShared);     //  each iteration have own vao

//         //  first object
//         glBindProgramPipeline(1);
//         glVertexArrayElementBuffer(vaoShared, buf);
//         glVertexArrayVertexBuffer(vaoShared, 0, buf, 0, 36);
//         glVertexArrayVertexBuffer(vaoShared, 1, buf, 0, 36);
//         glVertexArrayVertexBuffer(vaoShared, 2, buf, 0, 36);
//         //  draw triangle
//         glBindProgramPipeline(1);
//         glVertexArrayElementBuffer(vaoShared, buf);
//         //  draw line


//         //  second object
//         glBindProgramPipeline(1);
//         glVertexArrayElementBuffer(vaoShared, buf);
//         glVertexArrayVertexBuffer(vaoShared, 0, buf, 0, 36);
//         glVertexArrayVertexBuffer(vaoShared, 1, buf, 0, 36);
//         glVertexArrayVertexBuffer(vaoShared, 2, buf, 0, 36);
//         //  draw triangle
//         glBindProgramPipeline(1);
//         glVertexArrayElementBuffer(vaoShared, buf);
//         //  draw line
//     }
//     std::cout<<cl.Time<ZC_Nanoseconds>()<<"             3"<<std::endl;

//     cl.Start();
//     for (size_t i = 0; i < count; i++)
//     {
//         glBindVertexArray(vaoShared);     //  each iteration have own vao

//         //  first object
//         glBindProgramPipeline(1);
//         glVertexArrayElementBuffer(vaoShared, buf);
//         glVertexArrayVertexBuffers(vaoShared, 0, 3, buffers, offsets, strides);
//         //  draw triangle
//         glBindProgramPipeline(1);
//         glVertexArrayElementBuffer(vaoShared, buf);
//         //  draw line


//         //  second object
//         glBindProgramPipeline(1);
//         glVertexArrayElementBuffer(vaoShared, buf);
//         glVertexArrayVertexBuffers(vaoShared, 0, 3, buffers, offsets, strides);
//         //  draw triangle
//         glBindProgramPipeline(1);
//         glVertexArrayElementBuffer(vaoShared, buf);
//         //  draw line
//     }
//     std::cout<<cl.Time<ZC_Nanoseconds>()<<"             3   glVertexArrayVertexBuffers"<<std::endl;

//     cl.Start();
//     for (size_t i = 0; i < count; i++)
//     {
//         glBindVertexArray(vaoShared);     //  each iteration have own vao

//         //  first object
//         glBindProgramPipeline(1);
//         glVertexArrayElementBuffer(vaoShared, buf);
//         glVertexArrayVertexBuffer(vaoShared, 0, buf, 0, 36);
//         glVertexArrayVertexBuffer(vaoShared, 1, buf, 0, 36);
//         glVertexArrayVertexBuffer(vaoShared, 2, buf, 0, 36);
//         //  draw triangle
//         //  second object
//         glVertexArrayElementBuffer(vaoShared, buf);
//         glVertexArrayVertexBuffer(vaoShared, 0, buf, 0, 36);
//         glVertexArrayVertexBuffer(vaoShared, 1, buf, 0, 36);
//         glVertexArrayVertexBuffer(vaoShared, 2, buf, 0, 36);
//         //  draw triangle


//         //  first object
//         glBindProgramPipeline(1);
//         glVertexArrayElementBuffer(vaoShared, buf);
//         glVertexArrayVertexBuffer(vaoShared, 0, buf, 0, 36);
//         glVertexArrayVertexBuffer(vaoShared, 1, buf, 0, 36);
//         glVertexArrayVertexBuffer(vaoShared, 2, buf, 0, 36);
//         //  draw line
//         //  second object
//         glVertexArrayElementBuffer(vaoShared, buf);
//         glVertexArrayVertexBuffer(vaoShared, 0, buf, 0, 36);
//         glVertexArrayVertexBuffer(vaoShared, 1, buf, 0, 36);
//         glVertexArrayVertexBuffer(vaoShared, 2, buf, 0, 36);
//         //  draw line
//     }
//     std::cout<<cl.Time<ZC_Nanoseconds>()<<"             4"<<std::endl;

//     cl.Start();
//     for (size_t i = 0; i < count; i++)
//     {
//         glBindVertexArray(vaoShared);     //  each iteration have own vao

//         //  first object
//         glBindProgramPipeline(1);
//         glVertexArrayElementBuffer(vaoShared, buf);
//         glVertexArrayVertexBuffers(vaoShared, 0, 3, buffers, offsets, strides);
//         //  draw triangle
//         //  second object
//         glVertexArrayElementBuffer(vaoShared, buf);
//         glVertexArrayVertexBuffers(vaoShared, 0, 3, buffers, offsets, strides);
//         //  draw triangle


//         //  first object
//         glBindProgramPipeline(1);
//         glVertexArrayElementBuffer(vaoShared, buf);
//         glVertexArrayVertexBuffers(vaoShared, 0, 3, buffers, offsets, strides);
//         //  draw line
//         //  second object
//         glVertexArrayElementBuffer(vaoShared, buf);
//         glVertexArrayVertexBuffers(vaoShared, 0, 3, buffers, offsets, strides);
//         //  draw line
//     }
//     std::cout<<cl.Time<ZC_Nanoseconds>()<<"             4   glVertexArrayVertexBuffers"<<std::endl;


//     GLuint ebo;
//     glCreateBuffers(1, &ebo);
//     glNamedBufferStorage(ebo, 100, nullptr, 0);
    
//     glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
//     glBindBuffer(GL_ARRAY_BUFFER, buf);

//     GLuint vao1;
//     glCreateVertexArrays(1, &vao1);
//     glBindVertexArray(vao1);
//     glEnableVertexAttribArray(0);
//     glVertexAttribPointer(0, 3, GL_FLOAT, 0, 36, 0);
//     glEnableVertexAttribArray(1);
//     glVertexAttribPointer(1, 3, GL_FLOAT, 0, 36, (void*)12);
//     glEnableVertexAttribArray(2);
//     glVertexAttribPointer(2, 3, GL_FLOAT, 0, 36, (void*)24);
//     GLuint vao2;
//     glCreateVertexArrays(1, &vao2);
//     glBindVertexArray(vao2);
//     glEnableVertexAttribArray(0);
//     glVertexAttribPointer(0, 3, GL_FLOAT, 0, 36, 0);
//     glEnableVertexAttribArray(1);
//     glVertexAttribPointer(1, 3, GL_FLOAT, 0, 36, (void*)12);
//     glEnableVertexAttribArray(2);
//     glVertexAttribPointer(2, 3, GL_FLOAT, 0, 36, (void*)24);

//     glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
//     glBindBuffer(GL_ARRAY_BUFFER, 0);
//     glBindVertexArray(0);


//     cl.Start();
//     for (size_t i = 0; i < count; i++)
//     {
//         glBindProgramPipeline(1);
//         //  first object
//         glBindVertexArray(vao1);
//         //  draw triangle
//         //  second object
//         glBindVertexArray(vao2);
//         //  draw triangle
        
//         glBindProgramPipeline(1);
//         //  first object
//         glBindVertexArray(vao1);
//         //  draw line
//         //  second object
//         glBindVertexArray(vao2);
//         //  draw line
//     }
//     std::cout<<cl.Time<ZC_Nanoseconds>()<<"             5"<<std::endl;

//     cl.Start();
//     for (size_t i = 0; i < count; i++)
//     {
//         glBindVertexArray(vao1);
//         //  first object
//         glBindProgramPipeline(1);
//         //  draw triangle
//         glBindProgramPipeline(1);
//         //  draw line

//         glBindVertexArray(vao2);
//         //  second object
//         glBindProgramPipeline(1);
//         //  draw triangle
//         glBindProgramPipeline(1);
//         //  draw line
//     }
//     std::cout<<cl.Time<ZC_Nanoseconds>()<<"             6"<<std::endl;
// }