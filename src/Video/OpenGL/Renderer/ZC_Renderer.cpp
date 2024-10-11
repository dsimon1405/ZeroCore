#include <ZC/Video/OpenGL/Renderer/ZC_Renderer.h>

#include <ZC/Tools/Container/ZC_ContFunc.h>
#include <ZC/Video/OpenGL/ZC_GLBlend.h>
#include <ZC_Config.h>
// #ifdef ZC_IMGUI
// #include <ZC_IGWindow.h>
// #endif
#include <ZC/GUI/Backend/System/ZC_GUI.h>

#include <cassert>

ZC_Renderer::ZC_Renderer(ZC_Function<void()>&& _funcSwapBuffer)
    : funcSwapBuffer(std::move(_funcSwapBuffer))
{
    assert(funcSwapBuffer);
    pRenderer = this;
}

ZC_Renderer::~ZC_Renderer()
{
    pRenderer = nullptr;
}

void ZC_Renderer::AddUBOs(ZC_UBO* ubo, ZC_Function<void()>&& fUpdate)
{
    if (pRenderer) pRenderer->AddUBO(ubo, std::move(fUpdate));
}

void ZC_Renderer::EraseUBOs(ZC_UBO* pUbo)
{
    if (pRenderer) pRenderer->EraseUBO(pUbo);
}

void ZC_Renderer::Add(ZC_Render* pRender)
{
    if (!pRenderer || !pRender) return;

    auto prevIter = pRenderer->renders.before_begin();
    for (auto curIter = pRenderer->renders.begin(); curIter != pRenderer->renders.end(); )
    {
        if (**curIter < (*pRender).renderLevel)
        {
            prevIter = curIter;
            ++curIter;
            continue;
        }
        else
        {
            assert(**curIter != (*pRender).renderLevel);     //  double adding
            break;
        }
    }
    pRenderer->renders.emplace_after(prevIter, pRender);
}

void ZC_Renderer::Erase(ZC_Render* pRender)
{
    if (!pRenderer || !pRender) return;
    ZC_ForwardListErase(pRenderer->renders, pRender);
}

// void ZC_Renderer::Draw()
// {
//     this->UpdateUBO();

//     auto prevIter = renders.before_begin();
//     for (auto curIter = renders.begin(); curIter != renders.end(); )
//         curIter = (*curIter)->Draw() ? ++curIter : renders.erase_after(prevIter);

//     //  my rendering finished, make openGL state default
//     ZC_VAO::UnbindVertexArray();
//     ZC_GLBlend::Disable();
//     ZC_Framebuffer::Unbind();

// #ifdef ZC_IMGUI
//     ZC_IGWindow::Draw();
// #endif

//     funcSwapBuffer();
// }


// #include <ZC/GUI/ZC_GUI_TextManager.h>
// #include <ZC/Video/OpenGL/Shader/ZC_ShProgs.h>
// #include <ZC/Video/ZC_SWindow.h>
// ZC_GUI_TextManager tm;

// ZC_ShProg* pShP;
// void Create()
// {
//     tm.Init();
//     tm.GetText(L"jk");
//     tm.Configure();

//     ZC_Shader vs(ZC_Shader::ReadShaderFile("/home/dmitry/projects/ZCreator/build/assets/ZC/shaders/GUI/gui_text_test.vs", GL_VERTEX_SHADER).pHead, GL_VERTEX_SHADER);
//     ZC_Shader gs(ZC_Shader::ReadShaderFile("/home/dmitry/projects/ZCreator/build/assets/ZC/shaders/GUI/gui_text_test.gs", GL_GEOMETRY_SHADER).pHead, GL_GEOMETRY_SHADER);
//     ZC_Shader fs(ZC_Shader::ReadShaderFile("/home/dmitry/projects/ZCreator/build/assets/ZC/shaders/GUI/gui_text_test.fs", GL_FRAGMENT_SHADER).pHead, GL_FRAGMENT_SHADER);
//     pShP = new ZC_ShProg(vs.id, fs.id, gs.id);
// }

void ZC_Renderer::Draw(ZC_GUI* pGUI)
{
    this->UpdateUBO();

    auto prevIter = renders.before_begin();
    for (auto curIter = renders.begin(); curIter != renders.end(); )
        curIter = (*curIter)->Draw() ? ++curIter : renders.erase_after(prevIter);

    // static bool isFirst = true;
    // if (isFirst)
    // {
    //     Create();
    //     isFirst = false;
    // }

    // tm.texture.GLBindTextureUnit();
    // pShP->ActivateOpenGL();
    // glDrawArrays(GL_POINTS, 0, 1);

    // int w,h;
    // ZC_SWindow::GetSize(w,h);
// glDisable(GL_DEPTH_TEST);



    if (pGUI)
    {
            //  ZC_GUI uses modern openGL rendering, while total render system uses not fresh openGL, so no such
        ZC_FBOBuffersController::GlClear(GL_DEPTH_BUFFER_BIT);
        ZC_FBOBuffersController::GlEnable(GL_DEPTH_TEST);
        glDepthFunc(GL_LEQUAL);

        static ZC_GLBlend blend(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
        blend.Use();

        pGUI->Draw();
    }

    //  my rendering finished, make openGL state default
    ZC_ShProg::SetDefault();
    ZC_VAO::UnbindVertexArray();
    ZC_GLBlend::Disable();
    ZC_Framebuffer::Unbind();
    
    funcSwapBuffer();
}




//                         //                      draw arrays indirect
// #include <ZC/Video/OpenGL/Shader/ZC_ShProgs.h>
// struct Data
// {
//     float width, height;
//     ZC_Vec2<float> uv[4];
// };

// ZC_ShProg* pShP;
// GLuint vaoEmpty = 0,
//     bufVertex = 0,  //  with point coords (bl corner of triangle)
//     bufData = 0;    //  buffer for structs Data
// ZC_Texture tex;

// struct DrawArraysIndirectCommand
// {
//     GLuint count,
//         instanceCount,
//         first,
//         baseInstance;
// };
// GLuint cmdBuf = 0;
// // GLuint testBuf = 0;

//         #include <ZC/Tools/Time/ZC_Clock.h>
//         #include <iostream>

// void Create()
// {
//         //  sh prog
//     ZC_Shader vs(ZC_Shader::ReadShaderFile("/home/dmitry/projects/ZCreator/build/assets/ZC/shaders/GUI/gui.vs", GL_VERTEX_SHADER).pHead, GL_VERTEX_SHADER);
//                     //      vert + geon + frag
//     ZC_Shader gs(ZC_Shader::ReadShaderFile("/home/dmitry/projects/ZCreator/build/assets/ZC/shaders/GUI/gui.gs", GL_GEOMETRY_SHADER).pHead, GL_GEOMETRY_SHADER);
//     ZC_Shader fs(ZC_Shader::ReadShaderFile("/home/dmitry/projects/ZCreator/build/assets/ZC/shaders/GUI/gui.fs", GL_FRAGMENT_SHADER).pHead, GL_FRAGMENT_SHADER);
//     pShP = new ZC_ShProg(vs.id, fs.id, gs.id);
//     // pShP = new ZC_ShProg(vs.id, fs.id, 0);

//         //  vao empty
//     glCreateVertexArrays(1, &vaoEmpty);

//         //  buffers
//             //  vertex
//     float length = 50.f,
//         indent = 100;
//     ZC_Vec2<float> verts[]
//     {
//         //  first
//         {indent, indent}, {indent + length + 5, indent},
//         //  second
//         {indent, indent + 200}, {indent + length + 5, indent + 200},
//         //  second
//         {0, 0}, {length + 5, 0}
//     };
//     glCreateBuffers(1, &bufVertex);
//     glNamedBufferStorage(bufVertex, sizeof(verts), verts, 0);
//             //  data
//     Data data[]
//     {
//         //  first
//         {.width = length, .height = length, .uv{ {0.f, 0.f}, {.5f, 0.f}, {0.f, 1.f}, {.5f, 1.f} }},  //  bl, br, tl, tr  geom -> triangle_strip
//         {.width = length, .height = length, .uv{ {.5f, 0.f}, {1.f, 0.f}, {.5f, 1.f}, {1.f, 1.f} }},
//         //  second
//         {.width = length, .height = length, .uv{ {0.f, 0.f}, {.5f, 0.f}, {0.f, 1.f}, {.5f, 1.f} }},  //  bl, br, tl, tr  geom -> triangle_strip
//         {.width = length, .height = length, .uv{ {.5f, 0.f}, {1.f, 0.f}, {.5f, 1.f}, {1.f, 1.f} }},
//         //  second
//         {.width = length, .height = length, .uv{ {0.f, 0.f}, {.5f, 0.f}, {0.f, 1.f}, {.5f, 1.f} }},  //  bl, br, tl, tr  geom -> triangle_strip
//         {.width = length, .height = length, .uv{ {.5f, 0.f}, {1.f, 0.f}, {.5f, 1.f}, {1.f, 1.f} }},
//     };
//     glCreateBuffers(1, &bufData);
//     glNamedBufferStorage(bufData, sizeof(data), data, 0);
        
//         //  texture
//     tex = ZC_Texture::LoadTexture2D("/home/dmitry/Загрузки/awesomeface.png");

//     DrawArraysIndirectCommand daic[]
//     {
//         //     //      vert + frag
//         // {
//         //     .count = 4,                 //  for (int i = 0; i < instanceCount; ++i) {  
//         //     .instanceCount = 2,         //      instanceID = i;
//         //     .first = 0,                 //      glDrawArrays(mode, first, count); }
//         //     .baseInstance = 0   //  unused
//         // },
//             //      vert + geon + frag
//         {
//             //  draw points throught count
//             .count = 2,                 //  for (int i = 0; i < instanceCount; ++i) {  
//             .instanceCount = 1,         //      instanceID = i;
//             .first = 0,                 //      glDrawArrays(mode, first, count); }
//             .baseInstance = 0   //  use that unused variable to store start index in buffers: data and vertex
//         },
//         {
//             .count = 1,                 //  for (int i = 0; i < instanceCount; ++i) {  
//             .instanceCount = 0,         //      instanceID = i;
//             .first = 1,                 //      glDrawArrays(mode, first, count); }
//             .baseInstance = 2   //  use that unused variable to store start index in buffers: data and vertex
//         },
//         {
//             .count = 2,                 //  for (int i = 0; i < instanceCount; ++i) {  
//             .instanceCount = 1,         //      instanceID = i;
//             .first = 0,                 //      glDrawArrays(mode, first, count); }
//             .baseInstance = 4   //  use that unused variable to store start index in buffers: data and vertex
//         }
//     };
//     glCreateBuffers(1, &cmdBuf);
//     glNamedBufferStorage(cmdBuf, sizeof(daic), daic, GL_DYNAMIC_STORAGE_BIT);
// };

// //  geom count:                 1520    920
// //  geom instanceCount:         1530    910
// //  non geom count:             900     450
// //  non geom instanceCount:     1500    900

// void ZC_Renderer::Draw()
// {
//     this->UpdateUBO();

//     auto prevIter = renders.before_begin();
//     for (auto curIter = renders.begin(); curIter != renders.end(); )
//         curIter = (*curIter)->Draw() ? ++curIter : renders.erase_after(prevIter);

//     //  my rendering finished, make openGL state default
//     ZC_VAO::UnbindVertexArray();
//     ZC_GLBlend::Disable();
//     ZC_Framebuffer::Unbind();
    

//     static bool first = true;
//     if (first)
//     {
//         Create();
//         first = false;
//     }

//     glBindVertexArray(vaoEmpty);

//     glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 10, bufVertex);
//     glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 11, bufData);
//     pShP->ActivateOpenGL();
//     tex.GLBindTextureUnit(0);   //  0 -> binding from GLSL, not location

//     glBindBuffer(GL_DRAW_INDIRECT_BUFFER, cmdBuf);

//     // glMultiDrawArraysIndirect(GL_TRIANGLE_STRIP, 0, 2, 0);
//     glMultiDrawArraysIndirect(GL_POINTS, 0, 3, 0);

//     // glMultiDrawArraysIndirect(GL_POINTS, (const void*)sizeof(DrawArraysIndirectCommand), 1, 0);
    

//     funcSwapBuffer();
// }







// //                          //      glMultiDrawElementsIndirect

// #include <ZC/Video/OpenGL/Shader/ZC_ShProgs.h>
// struct Vertex
// {
//     float x, y;
//     float u, v;
// };
// struct Data
// {
//     float posX, posY;
// };

// ZC_ShProg* pShP;
// GLuint vaoEmpty = 0,
//     bufVertex = 0,  //  with point coords (bl corner of triangle)
//     bufData = 0,    //  buffer for structs Data
//     bufElements = 0;
// ZC_Texture tex;

// struct DrawElementsIndirectCommand
// {
//     GLuint count,
//         instanceCount,
//         firstIndex,
//         baseVertex,
//         baseInstance;
// };
// GLuint cmdBuf = 0;

// void Create()
// {
//         //  sh prog
//     ZC_Shader vs(ZC_Shader::ReadShaderFile("/home/dmitry/projects/ZCreator/ZeroCore/src/Video/OpenGL/Renderer/test.vs", GL_VERTEX_SHADER).pHead, GL_VERTEX_SHADER);
//     ZC_Shader fs(ZC_Shader::ReadShaderFile("/home/dmitry/projects/ZCreator/ZeroCore/src/Video/OpenGL/Renderer/test.fs", GL_FRAGMENT_SHADER).pHead, GL_FRAGMENT_SHADER);
//     pShP = new ZC_ShProg(vs.id, fs.id, 0);

//         //  vao empty
//     glCreateVertexArrays(1, &vaoEmpty);
    
//         //  buffers
//             //  vertex
//     Vertex verts[]
//     { 
//         0.f,      0.f,      0.f, 0.f,            //  bl
//         100.f,    0.f,      1.f, 0.f,            //  br
//         100.f,  100.f,      1.f, 1.f,            //  tr
//         0.f,    100.f,      0.f, 1.f             //  tl
//     };
//     glCreateBuffers(1, &bufVertex);
//     glNamedBufferStorage(bufVertex, sizeof(verts), verts, 0);
//             //  elements
//     GLubyte elmts[]
//     {
//         0,1,2,
//         0,2,3
//     };
//     glCreateBuffers(1, &bufElements);
//     glNamedBufferStorage(bufElements, sizeof(elmts), elmts, 0);
//             //  data
//     float indent = 100;
//     Data data[]
//     {
//         indent, indent, //  first
//     };
//     glCreateBuffers(1, &bufData);
//     glNamedBufferStorage(bufData, sizeof(data), data, 0);
        
//         //  texture
//     tex = ZC_Texture::LoadTexture2D("/home/dmitry/Загрузки/awesomeface.png");

//     DrawElementsIndirectCommand deic[]
//     {
//         {
//             .count = 6,     //  elements count
//             .instanceCount = 1,
//             .firstIndex = 0,
//             .baseVertex = 0,
//             .baseInstance = 0
//         }
//     };
//     glCreateBuffers(1, &cmdBuf);
//     glNamedBufferStorage(cmdBuf, sizeof(deic), deic, 0);
// };

// void ZC_Renderer::Draw()
// {
//     this->UpdateUBO();

//     auto prevIter = renders.before_begin();
//     for (auto curIter = renders.begin(); curIter != renders.end(); )
//         curIter = (*curIter)->Draw() ? ++curIter : renders.erase_after(prevIter);

//     //  my rendering finished, make openGL state default
//     ZC_VAO::UnbindVertexArray();
//     ZC_GLBlend::Disable();
//     ZC_Framebuffer::Unbind();
    

//     static bool first = true;
//     if (first)
//     {
//         Create();
//         first = false;
//     }

//     glBindVertexArray(vaoEmpty);


//     glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 0, bufVertex);
//     glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 1, bufData);
//     pShP->ActivateOpenGL();
//     tex.GLBindTextureUnit(0);   //  0 -> binding from GLSL, not location

//     glVertexArrayElementBuffer(vaoEmpty, bufElements);
//     glBindBuffer(GL_DRAW_INDIRECT_BUFFER, cmdBuf);

//     glMultiDrawElementsIndirect(GL_TRIANGLES, GL_UNSIGNED_BYTE, 0, 1, 0);

//     funcSwapBuffer();
// }









//                                  read pixel storie
// #include <ZC/Video/ZC_SWindow.h>
// #include <ZC/Tools/Console/ZC_cout.h>
// void ZC_Renderer::Draw()
// {
//     this->UpdateUBO();

//     auto prevIter = renders.before_begin();
//     for (auto curIter = renders.begin(); curIter != renders.end(); )
//         curIter = (*curIter)->Draw() ? ++curIter : renders.erase_after(prevIter);

//     // uchar data[4];
//     float data;
//     float x, y;
//     ZC_SWindow::GetCursorPosition(x, y);
//     static float _x, _y;
//     if (x != _x || y != _y)
//     {
//         _x = x;
//         _y = y;
//         glReadPixels(x,y,1,1,  GL_DEPTH_COMPONENT,  GL_FLOAT, &data);
//         // glReadPixels(x,y,1,1,  GL_DEPTH_STENCIL,  GL_UNSIGNED_INT_24_8, &data);
//         ZC_cout(std::to_string(data));
//         // int data1 = ((*(int*)data) >> 8) & 16777215;
//         // ZC_cout(std::to_string(data1));
//         // ZC_cout(std::to_string(data[0]) + "     " + std::to_string(data[1]) + "     " + std::to_string(data[2]));
//     }

// #ifdef ZC_IMGUI
//     glBindFramebuffer(GL_FRAMEBUFFER, 0);
//     ZC_IGWindow::Draw();
// #endif
//     glBindVertexArray(0);

//     funcSwapBuffer();
// }






//              buffer testing
//  block vs unifom:
//      - one mat4 -> block faster with glNamedBufferSubData + glBinBufferBase, then glUniformMatrix4fv.
//      - this is funny, four vec4 -> block slower, with glBinBufferBase (and without glNamedBufferSubData), then glUniform4fv is called four times.
//      - maping faster for some part of buffer then subbufffer


// //                                      instancing + sso st430

// #include <ZC/Video/OpenGL/Shader/ZC_Shader.h>
// #include <ZC/Video/OpenGL/Shader/ZC_ShProg.h>
// #include <ZC/Video/OpenGL/Buffer/ZC_Buffer.h>
// #include <ZC/Tools/Math/ZC_Mat4.h>
// #include <ZC/Tools/Time/ZC_Clock.h>
// #include <iostream>

// ZC_ShProg* pShP;

// GLuint vao,
//     vbo,
//     bind_point = 0,
//     indexVBO,
    
//     ubo1;

// struct UniData
// {
//     ZC_Vec4<float> color;
//     ZC_Mat4<float> model {1.f};
// };

// void Create1()
// {
//     //  sh prog
//     ZC_Shader vs(ZC_Shader::ReadShaderFile("/home/dmitry/projects/ZCreator/ZeroCore/src/Video/OpenGL/Renderer/test.vs", GL_VERTEX_SHADER).pHead, GL_VERTEX_SHADER);
//     ZC_Shader fs(ZC_Shader::ReadShaderFile("/home/dmitry/projects/ZCreator/ZeroCore/src/Video/OpenGL/Renderer/test.fs", GL_FRAGMENT_SHADER).pHead, GL_FRAGMENT_SHADER);
//     pShP = new ZC_ShProg(vs.id, fs.id, 0);
//     int uniforms = 0;
//     glGetProgramiv(pShP->id, GL_ACTIVE_UNIFORMS, &uniforms);

//     //  vao
//     glCreateVertexArrays(1, &vao);
//     GLuint lay_loc_inPosition = 0;
//     glEnableVertexArrayAttrib(vao, lay_loc_inPosition);
//     glVertexArrayAttribFormat(vao, lay_loc_inPosition, 2, GL_FLOAT, false, 0);
//     glVertexArrayAttribBinding(vao, lay_loc_inPosition, bind_point);
    
//     //  vbo
//     float leg = 0.3;
//     float vrts[]
//     {
//         -leg, -leg,     //  bl
//         leg, -leg,      //  br
//         leg, leg,       //  tr
//         -leg, leg,      //  tl
//     };
//     glCreateBuffers(1, &vbo);
//     glNamedBufferStorage(vbo, sizeof(vrts), vrts, 0);
//     //  index vbo
//     uchar indexes[]
//     {
//         0, 1, 2,
//         0, 2, 3
//     };
//     glCreateBuffers(1, &indexVBO);
//     glNamedBufferStorage(indexVBO, sizeof(indexes), indexes, 0);

//     //  personal data for rendering objects
//     UniData ud[2];
//         //  first object data
//     ud[0].color = ZC_Vec4<float>(1,1,0,1);
//     ud[0].model.Translate({-0.5f, 0.f, 0.f});
//         //  second object data
//     ud[1].color = ZC_Vec4<float>(0,1,1,1);
//     ud[1].model.Translate({0.5f, 0.f, 0.f});

//     glCreateBuffers(1, &ubo1);
//     glNamedBufferStorage(ubo1, sizeof(ud), &ud, GL_DYNAMIC_STORAGE_BIT);
// }

// void ZC_Renderer::Draw()
// {
//     this->UpdateUBO();

//     auto prevIter = renders.before_begin();
//     for (auto curIter = renders.begin(); curIter != renders.end(); )
//         curIter = (*curIter)->Draw() ? ++curIter : renders.erase_after(prevIter);

//     static bool first = true;
//     if (first)
//     {
//         Create1();
//         first = false;
//     }   

//     pShP->ActivateOpenGL();

//     glBindVertexArray(vao);
//     glVertexArrayVertexBuffer(vao, bind_point, vbo, 0, 8);
//     glVertexArrayElementBuffer(vao, indexVBO);

//     glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 0, ubo1);

//     //  6 is indexes amount, 2 count of object to repeat
//     glDrawElementsInstanced(GL_TRIANGLES, 6, GL_UNSIGNED_BYTE, 0, 2);

//     //  my rendering finished, make openGL state default
//     ZC_VAO::UnbindVertexArray();
//     ZC_GLBlend::Disable();
//     ZC_Framebuffer::Unbind();

//     funcSwapBuffer();
// }







//                         //  shader pipeline + (tes evaluation shader + tes control from cpu (need make tes con shader))

// #include <ZC/Video/OpenGL/Shader/ZC_Shader.h>

// GLuint vp = 0,
//     fp = 0,
//     tep = 0,
//     pipeline = 0,
//     vao,
//     vbo,
    
//     bind_point = 0,    //  use pos and color so name just bind_point, PCPC shema, not PPCC
    
//     shP;
// // #include <iostream>
// // #include <ZC/Tools/Time/ZC_Clock.h>
// void Create1()
// {
//     auto da_vs = ZC_Shader::ReadShaderFile("/home/dmitry/projects/ZCreator/ZeroCore/src/Video/OpenGL/Renderer/test.vs", GL_VERTEX_SHADER);
//     auto da_tes = ZC_Shader::ReadShaderFile("/home/dmitry/projects/ZCreator/ZeroCore/src/Video/OpenGL/Renderer/test.tes", GL_GEOMETRY_SHADER);
//     auto da_fs = ZC_Shader::ReadShaderFile("/home/dmitry/projects/ZCreator/ZeroCore/src/Video/OpenGL/Renderer/test.fs", GL_FRAGMENT_SHADER);
    
//         //  pipe
//     vp = glCreateShaderProgramv(GL_VERTEX_SHADER, 1, &da_vs.pHead);
//     tep = glCreateShaderProgramv(GL_TESS_EVALUATION_SHADER, 1, &da_tes.pHead);
//     fp = glCreateShaderProgramv(GL_FRAGMENT_SHADER, 1, &da_fs.pHead);
//     glCreateProgramPipelines(1, &pipeline);
//     glUseProgramStages(pipeline, GL_VERTEX_SHADER_BIT, vp);
//     glUseProgramStages(pipeline, GL_TESS_EVALUATION_SHADER_BIT, tep);
//     glUseProgramStages(pipeline, GL_FRAGMENT_SHADER_BIT, fp);

//     //     //  prog
//     // ZC_Shader vSh(da_vs.pHead, GL_VERTEX_SHADER);
//     // ZC_Shader teSh(da_tes.pHead, GL_TESS_EVALUATION_SHADER);
//     // ZC_Shader fSh(da_fs.pHead, GL_FRAGMENT_SHADER);

//     // shP = glCreateProgram();
//     // glAttachShader(shP, vSh.id);
//     // glAttachShader(shP, teSh.id);
//     // glAttachShader(shP, fSh.id);

//     // glLinkProgram(shP);
//     // //	link check
//     // int success = 0;
//     // glGetProgramiv(shP, GL_LINK_STATUS, &success);
//     // if (!success)
//     // {
//     //     char infoLog[1024];
//     //     glGetProgramInfoLog(shP, 1024, NULL, infoLog);
//     //     assert(false);
//     // }
//     // glDetachShader(shP, vSh.id);
//     // glDetachShader(shP, teSh.id);
//     // glDetachShader(shP, fSh.id);

//     // using namespace std;
//     // ZC_Clock cl;
//     // size_t count = 1000000;

//     // cl.Start();
//     // for (size_t i = 0; i < count; i++)
//     // {
//     //     glUseProgram(shP);
//     // }
//     // cout<<cl.Time<ZC_Nanoseconds>()<<endl;

//     // cl.Start();
//     // for (size_t i = 0; i < count; i++)
//     // {
//     //     glUseProgramStages(pipeline, GL_TESS_EVALUATION_SHADER_BIT, tep);
//     //     glUseProgramStages(pipeline, GL_VERTEX_SHADER_BIT, vp);
//     //     glBindProgramPipeline(pipeline);
//     // }
//     // cout<<cl.Time<ZC_Nanoseconds>()<<endl;



//     glCreateVertexArrays(1, &vao);
//     GLuint lay_loc_inPosition = 0;
//     glEnableVertexArrayAttrib(vao, lay_loc_inPosition);
//     glVertexArrayAttribFormat(vao, lay_loc_inPosition, 2, GL_FLOAT, false, 0);
//     glVertexArrayAttribBinding(vao, lay_loc_inPosition, bind_point);
//     GLuint lay_loc_inColor = 1;
//     glEnableVertexArrayAttrib(vao, lay_loc_inColor);
//     glVertexArrayAttribFormat(vao, lay_loc_inColor, 3, GL_FLOAT, false, 8);
//     glVertexArrayAttribBinding(vao, lay_loc_inColor, bind_point);

//     float vrts[]
//     {
//         -0.5f, -0.5f,       1.f, 0.f, 0.f,      //  bl
//         0.5f, -0.5f,        0.f, 1.f, 0.f,      //  br
//         0.5f, 0.5f,         0.f, 0.f, 1.f,      //  tr
//         -0.5f, 0.5f,        0.5f, 0.5f, 0.5f,   //  tl
//     };
//     glCreateBuffers(1, &vbo);
//     glNamedBufferStorage(vbo, sizeof(vrts), vrts, 0);

//     glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
// }

//     //    //  activate deactivate teseletion evaluation shader program
// // ZC_Clock clo;
// // int currState = 2;

// void ZC_Renderer::Draw()
// {
//     this->UpdateUBO();
//     auto prevIter = renders.before_begin();
//     for (auto curIter = renders.begin(); curIter != renders.end(); )
//         curIter = (*curIter)->Draw() ? ++curIter : renders.erase_after(prevIter);

//     static bool first = true;
//     if (first)
//     {
//         Create1();
//         first = false;
//         //    //  activate deactivate teseletion evaluation shader program
//         // clo.Start();
//     }   

//     glBindVertexArray(vao);
//     glVertexArrayVertexBuffer(vao, bind_point, vbo, 0, 8 + 12); //  8 + 12 -> position + color
//     //    //  activate deactivate teseletion evaluation shader program
//     // if (clo.Time<ZC_Seconds>() >= 2)
//     // {
//     //     if (currState == 1)
//     //     {
//     //         glUseProgramStages(pipeline, GL_TESS_EVALUATION_SHADER_BIT, tep);
//     //         currState = 2;
//     //     }
//     //     else
//     //     {
//     //         glUseProgramStages(pipeline, GL_TESS_EVALUATION_SHADER_BIT, 0);
//     //         currState = 1;
//     //     }
//     //     clo.Start();
//     // }
//     glUseProgram(0);
//     glBindProgramPipeline(pipeline);

//     // glUseProgram(shP);

// //        //  activate deactivate teseletion evaluation shader program
// // if (currState == 2)
// // {
//     int patches = 4;
//     glPatchParameteri(GL_PATCH_VERTICES, patches);
//     float out = 1;
//     float outer[]{out, out, out, out};
//     glPatchParameterfv(GL_PATCH_DEFAULT_OUTER_LEVEL, outer);
//     float in = 0;
//     float iner[]{in, in};
//     glPatchParameterfv(GL_PATCH_DEFAULT_INNER_LEVEL, iner);

//     glDrawArrays(GL_PATCHES, 0, patches);
// // }
// // else
// // {
// //     glDrawArrays(GL_TRIANGLE_FAN, 0, 4);
// // }
//     glBindProgramPipeline(0);


//     //  my rendering finished, make openGL state default
//     ZC_VAO::UnbindVertexArray();
//     ZC_GLBlend::Disable();
//     ZC_Framebuffer::Unbind();

//     funcSwapBuffer();
// }






//          //  texture arrays

// #include <ZC/Video/OpenGL/Shader/ZC_Shader.h>
// #include <ZC/Video/OpenGL/Shader/ZC_ShProg.h>
// #include <ZC/Video/OpenGL/Buffer/ZC_Buffer.h>

// ZC_ShProg* pShP = nullptr;
// ZC_Buffer* pb1 = nullptr,
//     * pb2el = nullptr,
//     * pb2ver = nullptr,
//     * pb2tex = nullptr;
// ZC_VAO* pVao = nullptr;

// GLuint bind_inPosition = 0,
//     bind_inTexCoord = 1;

// GLubyte el[] { 0,1,2,  0,2,3};    //  elements
// float ver[] =
// {  //  quad
//     0.f, 0.f,    //  bl
//     0.4f, 0.f,    //  br
//     0.4f, 0.4f,     //  tr
//     0.0, 0.4,       //  tl
// };
// float texCoord[]
// {
//     //  tex coord
//     0.f, 0.f,   //  bl
//     1.f, 0.f,   //  br
//     1.f, 1.f,   //  tr
//     0.f, 1.f    //  tl
// };

// GLuint texArr1;

// struct Data
// {
//     ZC_Vec2<float> position;
//     ZC_Vec4<float> color;
// };
// #include <ZC/Tools/Time/ZC_Clock.h>
// #include <iostream>
// void Create1()
// {
//     ZC_Shader vs(ZC_Shader::ReadShaderFile("/home/dmitry/projects/ZCreator/ZeroCore/src/Video/OpenGL/Renderer/test.vs", GL_VERTEX_SHADER).pHead, GL_VERTEX_SHADER);
//     ZC_Shader fs(ZC_Shader::ReadShaderFile("/home/dmitry/projects/ZCreator/ZeroCore/src/Video/OpenGL/Renderer/test.fs", GL_FRAGMENT_SHADER).pHead, GL_FRAGMENT_SHADER);
//     pShP = new ZC_ShProg(vs.id, fs.id, 0);

//     GLuint lay_loc_inPosition = 0,
//         lay_loc_r = 1,
//         lay_loc_g = 2,
//         lay_loc_b = 3;
    
//     pVao = new ZC_VAO;
//     GLuint vaoID = pVao->id;
//     glEnableVertexArrayAttrib(vaoID, lay_loc_inPosition);
//     glVertexArrayAttribFormat(vaoID, lay_loc_inPosition, 2, GL_FLOAT, false, 0);
//     glVertexArrayAttribBinding(vaoID, lay_loc_inPosition, bind_inPosition);

//         //  uses -> VertexAtribFormat, no metter call glVertexArrayVertexBuffers(...) or glVertexArrayVertexBuffer(...)
//     glEnableVertexArrayAttrib(vaoID, lay_loc_r);
//     glVertexArrayAttribFormat(vaoID, lay_loc_r, 2, GL_FLOAT, false, 0);
//     glVertexArrayAttribBinding(vaoID, lay_loc_r, bind_inTexCoord);

//     //  buf1
//     float data1[]
//     {
//         //  quad
//         -0.4, 0.f,  //  bl
//         0.f, 0.f,   //  br
//         0.f, 0.4,   //  tr
//         -0.4, 0.4,  //  tl

//         //  tex coord
//         0.f, 0.f,   //  bl
//         1.f, 0.f,   //  br
//         1.f, 1.f,   //  tr
//         0.f, 1.f    //  tl
//     };
//     pb1 = new ZC_Buffer(GL_ARRAY_BUFFER);
//     pb1->GLNamedBufferStorage(sizeof(el) + sizeof(data1), el, GL_DYNAMIC_STORAGE_BIT);
//     pb1->GLNamedBufferSubData(sizeof(el), sizeof(data1), data1);

//     //  buf2
//     pb2el = new ZC_Buffer(GL_ARRAY_BUFFER);
//     pb2el->GLNamedBufferStorage(sizeof(el), el, 0);
//     pb2ver = new ZC_Buffer(GL_ARRAY_BUFFER);
//     pb2ver->GLNamedBufferStorage(sizeof(ver), ver, 0);
//     pb2tex = new ZC_Buffer(GL_ARRAY_BUFFER);
//     pb2tex->GLNamedBufferStorage(sizeof(texCoord), texCoord, 0);

//     //  texture array
//     GLuint w = 512, h = 512;

//     glCreateTextures(GL_TEXTURE_2D_ARRAY, 1, &texArr1);
//     glTextureStorage3D(texArr1, 1, GL_RGB8, w, h, 2);
    
//     uint texSize = w * h;
//     ZC_Vec3<uchar> colors[texSize];
//     std::fill(colors, colors + texSize, ZC_Vec3<uchar>(225,225,0));
//         // glTextureSubImage3D(texArr1, 0, 0, 0, 0 -> index of 2d texture in 2D array
//     glTextureSubImage3D(texArr1, 0, 0, 0, 0, w, h, 1, GL_RGB, GL_UNSIGNED_BYTE, colors);

//     auto tex = ZC_Texture::LoadTexture2D("/home/dmitry/Загрузки//wall.jpg");
//     glCopyImageSubData(tex.GetId(), GL_TEXTURE_2D, 0, 0, 0, 0, texArr1, GL_TEXTURE_2D_ARRAY, 0, 0, 0, 1, w, h, 1);  //  last is number of 2d texture, now second(starts from 0)

//     // glProgramUniform1i(pShP->id, 0, 11);    //  !!!!
// }

// void ZC_Renderer::Draw()
// {
//     this->UpdateUBO();

//     auto prevIter = renders.before_begin();
//     for (auto curIter = renders.begin(); curIter != renders.end(); )
//         curIter = (*curIter)->Draw() ? ++curIter : renders.erase_after(prevIter);

//     glClearColor(0.3f,0.3f,0.3f,1);
//     glClear(GL_COLOR_BUFFER_BIT);
    
//     static bool first = true;
//     if (first)
//     {
//         Create1();
//         first = false;
//     }

//     glUseProgram(pShP->id);
//     pVao->ActivateOpenGL();

//     GLuint vaoId = pVao->id;

//     GLuint binding_texArray = 11,
//         loc_layer = 0;

//     glBindTextureUnit(binding_texArray, texArr1);

//             //  first draw
    
//     GLuint layyer1 = 0;
//     glUniform1ui(loc_layer, layyer1);

//     glVertexArrayVertexBuffer(vaoId, bind_inPosition, pb1->id, sizeof(el), 8);
//     glVertexArrayVertexBuffer(vaoId, bind_inTexCoord, pb1->id, sizeof(el) + sizeof(ver), 8);
    
//     glVertexArrayElementBuffer(vaoId, pb1->id);
    
//     glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_BYTE, 0);

//             //  second draw

//     GLuint layyer2 = 1;
//     glUniform1ui(loc_layer, layyer2);

//     glVertexArrayVertexBuffer(vaoId, bind_inPosition, pb2ver->id, 0, 8);
//     glVertexArrayVertexBuffer(vaoId, bind_inTexCoord, pb2tex->id, 0, 8);

//     glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_BYTE, 0);

//     glBindVertexArray(0);
//     glUseProgram(0);


//     //  my rendering finished, make openGL state default
//     ZC_VAO::UnbindVertexArray();
//     ZC_GLBlend::Disable();
//     ZC_Framebuffer::Unbind();

//     funcSwapBuffer();
// }




//         //  uses -> VertexAtribFormat with bind all buffers throught one call to -> glVertexArrayVertexBuffers(...)

// #include <ZC/Video/OpenGL/Shader/ZC_Shader.h>
// #include <ZC/Video/OpenGL/Shader/ZC_ShProg.h>
// #include <ZC/Video/OpenGL/Buffer/ZC_Buffer.h>
// ZC_ShProg* pShP = nullptr;
// ZC_Buffer* pb1 = nullptr,
//     * pb2el = nullptr,
//     * pb2ver = nullptr,
//     * pb2r = nullptr,
//     * pb2g = nullptr,
//     * pb2b = nullptr;
// ZC_VAO* pVao = nullptr;

    // GLuint bind_inPosition = 0,
    //     bind_r = 1,
    //     bind_g = 2,
    //     bind_b = 3;

//     GLubyte el[] { 0,1,2,  0,2,3};    //  elements
//     float ver[] =
//     {
//         0.f, 0.f,    //  bl
//         0.4f, 0.f,    //  br
//         0.4f, 0.4f,     //  tr
//         0.0, 0.4,       //  tl
//     };  //  quad
//     float r[] = { 0.f, 0.f, 0.f, 0.f, },    //  r
//         g[] = { 1.f, 1.f, 1.f, 1.f, },    //  g
//         b[] = { 1.f, 1.f, 1.f, 1.f };    //  b

// struct Data
// {
//     ZC_Vec2<float> position;
//     ZC_Vec4<float> color;
// };

// void Create1()
// {
//     ZC_Shader vs(ZC_Shader::ReadShaderFile("/home/dmitry/projects/ZCreator/ZeroCore/src/Video/OpenGL/Renderer/test.vs", GL_VERTEX_SHADER).pHead, GL_VERTEX_SHADER);
//     ZC_Shader fs(ZC_Shader::ReadShaderFile("/home/dmitry/projects/ZCreator/ZeroCore/src/Video/OpenGL/Renderer/test.fs", GL_FRAGMENT_SHADER).pHead, GL_FRAGMENT_SHADER);
//     pShP = new ZC_ShProg(vs.id, fs.id, 0);

//     GLuint lay_loc_inPosition = 0,
//         lay_loc_r = 1,
//         lay_loc_g = 2,
//         lay_loc_b = 3;

//     pVao = new ZC_VAO;
//     GLuint id1 = pVao->id;
//     glEnableVertexArrayAttrib(id1, lay_loc_inPosition);
//     glVertexArrayAttribFormat(id1, lay_loc_inPosition, 2, GL_FLOAT, false, 0);
//     glVertexArrayAttribBinding(id1, lay_loc_inPosition, bind_inPosition);

//         //  uses -> VertexAtribFormat, no metter call glVertexArrayVertexBuffers(...) or glVertexArrayVertexBuffer(...)
//     glEnableVertexArrayAttrib(id1, lay_loc_r);
//     glVertexArrayAttribFormat(id1, lay_loc_r, 1, GL_FLOAT, false, 0);
//     glVertexArrayAttribBinding(id1, lay_loc_r, bind_r);

//     glEnableVertexArrayAttrib(id1, lay_loc_g);
//     glVertexArrayAttribFormat(id1, lay_loc_g, 1, GL_FLOAT, false, 0);
//     glVertexArrayAttribBinding(id1, lay_loc_g, bind_g);

//     glEnableVertexArrayAttrib(id1, lay_loc_b);
//     glVertexArrayAttribFormat(id1, lay_loc_b, 1, GL_FLOAT, false, 0);
//     glVertexArrayAttribBinding(id1, lay_loc_b, bind_b);

//     //  buf1
//     float data1[]
//     {
//         //  quad
//         -0.4, 0.f,  //  bl
//         0.f, 0.f,   //  br
//         0.f, 0.4,   //  tr
//         -0.4, 0.4,  //  tl

//         1.f, 1.f, 1.f, 1.f,  //  r
//         1.f, 1.f, 1.f, 1.f,  //  g
//         0.f, 0.f, 0.f, 0.f,  //  b
//     };
//     pb1 = new ZC_Buffer(GL_ARRAY_BUFFER);
//     pb1->GLNamedBufferStorage(sizeof(el) + sizeof(data1), el, GL_DYNAMIC_STORAGE_BIT);
//     pb1->GLNamedBufferSubData(sizeof(el), sizeof(data1), data1);

//     //  buf2
//     pb2el = new ZC_Buffer(GL_ARRAY_BUFFER);
//     pb2el->GLNamedBufferStorage(sizeof(el), el, 0);
//     pb2ver = new ZC_Buffer(GL_ARRAY_BUFFER);
//     pb2ver->GLNamedBufferStorage(sizeof(ver), ver, 0);
//     pb2r = new ZC_Buffer(GL_ARRAY_BUFFER);
//     pb2r->GLNamedBufferStorage(sizeof(r), r, 0);
//     pb2g = new ZC_Buffer(GL_ARRAY_BUFFER);
//     pb2g->GLNamedBufferStorage(sizeof(g), g, 0);
//     pb2b = new ZC_Buffer(GL_ARRAY_BUFFER);
//     pb2b->GLNamedBufferStorage(sizeof(b), b, 0);
// }

// void ZC_Renderer::Draw()
// {
//     this->UpdateUBO();

//     auto prevIter = renders.before_begin();
//     for (auto curIter = renders.begin(); curIter != renders.end(); )
//         curIter = (*curIter)->Draw() ? ++curIter : renders.erase_after(prevIter);

    // glClearColor(0.3f,0.3f,0.3f,1);
    // glClear(GL_COLOR_BUFFER_BIT);
    // static bool first = true;
    // if (first) Create1();
    //     first = false;


//     pShP->ActivateOpenGL();
//     pVao->ActivateOpenGL();

//     GLuint vaoId = pVao->id;


//             //  first draw

//         //  uses -> VertexAtribFormat with bind all buffers throught one call to -> glVertexArrayVertexBuffers(...)
//     // GLuint buffers1[]{ pb1->id, pb1->id, pb1->id, pb1->id };    //  all data in same buffer
//     // GLintptr offsets1[]{ sizeof(el), sizeof(el) + sizeof(ver), sizeof(el) + sizeof(ver) + sizeof(r), sizeof(el) + sizeof(ver) + sizeof(r) + sizeof(g) };
//     // GLsizei strides1[]{ 8, 4, 4, 4 };
//     // glVertexArrayVertexBuffers(vaoId, 0, 4, buffers1, offsets1, strides1);

//         //  uses -> VertexAtribFormat with a separate binding of each buffer -> glVertexArrayVertexBuffer(...)
//     glVertexArrayVertexBuffer(vaoId, bind_inPosition, pb1->id, sizeof(el), 8);
//     glVertexArrayVertexBuffer(vaoId, bind_r, pb1->id, sizeof(el) + sizeof(ver), 4);
//     glVertexArrayVertexBuffer(vaoId, bind_g, pb1->id, sizeof(el) + sizeof(ver) + sizeof(r), 4);
//     glVertexArrayVertexBuffer(vaoId, bind_b, pb1->id, sizeof(el) + sizeof(ver) + sizeof(r) + sizeof(g), 4);

//     glVertexArrayElementBuffer(vaoId, pb1->id);
    
//     glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_BYTE, 0);


//             //  second draw

//         //  uses -> VertexAtribFormat with bind all buffers throught one call to -> glVertexArrayVertexBuffers(...)
//     // GLuint buffers2[]{ pb2ver->id, pb2r->id, pb2g->id, pb2b->id };    //  data in separate buffers
//     // GLintptr offsets2[]{ 0, 0, 0, 0 };  //  no offsets in separate buffers
//     // GLsizei strides2[]{ 8, 4, 4, 4 };
//     // glVertexArrayVertexBuffers(vaoId, 0, 4, buffers2, offsets2, strides2);

//         //  uses -> VertexAtribFormat with a separate binding of each buffer -> glVertexArrayVertexBuffer(...)
//     glVertexArrayVertexBuffer(vaoId, bind_inPosition, pb2ver->id, 0, 8);
//     glVertexArrayVertexBuffer(vaoId, bind_r, pb2r->id, 0, 4);
//     glVertexArrayVertexBuffer(vaoId, bind_g, pb2g->id, 0, 4);
//     glVertexArrayVertexBuffer(vaoId, bind_b, pb2b->id, 0, 4);

//     glVertexArrayElementBuffer(vaoId, pb2el->id);

//     glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_BYTE, 0);


//     glBindVertexArray(0);
//     glUseProgram(0);


//     //  my rendering finished, make openGL state default
//     ZC_VAO::UnbindVertexArray();
//     ZC_GLBlend::Disable();
//     ZC_Framebuffer::Unbind();

// #ifdef ZC_IMGUI
//     ZC_IGWindow::Draw();
// #endif

//     funcSwapBuffer();
// }





//                             //  uses -> std430 buffer

// #include <ZC//Video/OpenGL/Shader/ZC_Shader.h>
// #include <ZC/Video/OpenGL/Shader/ZC_ShProg.h>
// #include <ZC/Video/OpenGL/Buffer/ZC_Buffer.h>
// ZC_ShProg* pShP = nullptr;
// ZC_Buffer* pb1 = nullptr,
//     * pb2el = nullptr,
//     * pb2ver = nullptr;
// ZC_VAO* pVao = nullptr;

// GLuint bind_inPosition = 0;


// GLubyte el[] { 0,1,2,  0,2,3 };    //  elements
// float ver[] =
// {  //  quad
//     0.f, 0.f,    //  bl
//     0.4f, 0.f,    //  br
//     0.4f, 0.4f,     //  tr
//     0.0, 0.4,       //  tl
// };

// GLuint ubo;  //  wrong name uses as -> buffer storage

// struct Data
// {
//     ZC_Vec2<float> position;
//     ZC_Vec4<float> color;
// };

// void Create1()
// {
//     ZC_Shader vs(ZC_Shader::ReadShaderFile("/home/dmitry/projects/ZCreator/ZeroCore/src/Video/OpenGL/Renderer/test.vs", GL_VERTEX_SHADER).pHead, GL_VERTEX_SHADER);
//     ZC_Shader fs(ZC_Shader::ReadShaderFile("/home/dmitry/projects/ZCreator/ZeroCore/src/Video/OpenGL/Renderer/test.fs", GL_FRAGMENT_SHADER).pHead, GL_FRAGMENT_SHADER);
//     pShP = new ZC_ShProg(vs.id, fs.id, 0);

//     GLuint lay_loc_inPosition = 0;
    
//     //  vao
//     pVao = new ZC_VAO;
//     GLuint id1 = pVao->id;
//     glEnableVertexArrayAttrib(id1, lay_loc_inPosition);
//     glVertexArrayAttribFormat(id1, lay_loc_inPosition, 2, GL_FLOAT, false, 0);
//     glVertexArrayAttribBinding(id1, lay_loc_inPosition, bind_inPosition);

//     //      vbo
//     //  buf1
//     float data1[]
//     {
//         //  quad
//         -0.4, 0.f,  //  bl
//         0.f, 0.f,   //  br
//         0.f, 0.4,   //  tr
//         -0.4, 0.4,  //  tl
//     };
//     pb1 = new ZC_Buffer(GL_ARRAY_BUFFER);
//     pb1->GLNamedBufferStorage(sizeof(el) + sizeof(data1), el, GL_DYNAMIC_STORAGE_BIT);
//     pb1->GLNamedBufferSubData(sizeof(el), sizeof(data1), data1);
    
//     //  buf2
//     pb2el = new ZC_Buffer(GL_ARRAY_BUFFER);
//     pb2el->GLNamedBufferStorage(sizeof(el), el, 0);
//     pb2ver = new ZC_Buffer(GL_ARRAY_BUFFER);
//     pb2ver->GLNamedBufferStorage(sizeof(ver), ver, 0);

//     //  ubo
//     Data dat1
//     {
//         ZC_Vec2<float>(-0.2f, -0.2f),
//         ZC_Vec4<float>(1.f, 1.f, 0.f, 1.f)
//     };
//     Data dat2
//     {
//         ZC_Vec2<float>(0.2f, 0.2f),
//         ZC_Vec4<float>(0.f, 1.f, 1.f, 1.f)
//     };

//     //     //  uses -> std430 buffer
//     glCreateBuffers(1, &ubo);
//     uint objsCount = 2;
//     glNamedBufferStorage(ubo, sizeof(Data) * objsCount, &dat1, GL_DYNAMIC_STORAGE_BIT);
//     glNamedBufferSubData(ubo, sizeof(Data), sizeof(Data), &dat2);
//     glBindBuffer(GL_SHADER_STORAGE_BUFFER, ubo);
//     glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 5, ubo);
//     glBindBuffer(GL_SHADER_STORAGE_BUFFER, 0);
// }

// void ZC_Renderer::Draw()
// {
//     this->UpdateUBO();

//     auto prevIter = renders.before_begin();
//     for (auto curIter = renders.begin(); curIter != renders.end(); )
//         curIter = (*curIter)->Draw() ? ++curIter : renders.erase_after(prevIter);

//     glClearColor(0.3f,0.3f,0.3f,1);
//     glClear(GL_COLOR_BUFFER_BIT);
//     static bool first = true;
//     if (first) Create1();
//         first = false;


//     pShP->ActivateOpenGL();
//     pVao->ActivateOpenGL();

//     GLuint vaoId = pVao->id;

//             //  first draw


//     GLuint locIndex = 11;
//         //  uses -> std430 buffer
//     glUniform1ui(locIndex, 0);

//     glVertexArrayVertexBuffer(vaoId, bind_inPosition, pb1->id, sizeof(el), 8);
//     glVertexArrayElementBuffer(vaoId, pb1->id);
    
//     glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_BYTE, 0);


//             //  second draw

//         //  uses -> std430 buffer
//     glUniform1ui(locIndex, 1);

//     glVertexArrayVertexBuffer(vaoId, bind_inPosition, pb2ver->id, 0, 8);
//     glVertexArrayElementBuffer(vaoId, pb2el->id);

//     glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_BYTE, 0);

//     glBindVertexArray(0);
//     glUseProgram(0);


//     //  my rendering finished, make openGL state default
//     ZC_VAO::UnbindVertexArray();
//     ZC_GLBlend::Disable();
//     ZC_Framebuffer::Unbind();

// #ifdef ZC_IMGUI
//     ZC_IGWindow::Draw();
// #endif

//     funcSwapBuffer();
// }







//         //  uses -> uniform block

// #include <ZC//Video/OpenGL/Shader/ZC_Shader.h>
// #include <ZC/Video/OpenGL/Shader/ZC_ShProg.h>
// #include <ZC/Video/OpenGL/Buffer/ZC_Buffer.h>
// ZC_ShProg* pShP = nullptr;
// ZC_Buffer* pb1 = nullptr,
//     * pb2el = nullptr,
//     * pb2ver = nullptr;
// ZC_VAO* pVao = nullptr;

// GLuint bind_inPosition = 0;

// GLubyte el[] { 0,1,2,  0,2,3 };    //  elements
// float ver[] =
// {  //  quad
//     0.f, 0.f,    //  bl
//     0.4f, 0.f,    //  br
//     0.4f, 0.4f,     //  tr
//     0.0, 0.4,       //  tl
// };

// GLuint ubo1, ubo2;

// struct Data
// {
//     ZC_Vec2<float> position;
//     ZC_Vec4<float> color;
// };

// void Create1()
// {
//     ZC_Shader vs(ZC_Shader::ReadShaderFile("/home/dmitry/projects/ZCreator/ZeroCore/src/Video/OpenGL/Renderer/test.vs", GL_VERTEX_SHADER).pHead, GL_VERTEX_SHADER);
//     ZC_Shader fs(ZC_Shader::ReadShaderFile("/home/dmitry/projects/ZCreator/ZeroCore/src/Video/OpenGL/Renderer/test.fs", GL_FRAGMENT_SHADER).pHead, GL_FRAGMENT_SHADER);
//     pShP = new ZC_ShProg(vs.id, fs.id, 0);

//     GLuint lay_loc_inPosition = 0;
    
//     //  vao
//     pVao = new ZC_VAO;
//     GLuint id1 = pVao->id;
//     glEnableVertexArrayAttrib(id1, lay_loc_inPosition);
//     glVertexArrayAttribFormat(id1, lay_loc_inPosition, 2, GL_FLOAT, false, 0);
//     glVertexArrayAttribBinding(id1, lay_loc_inPosition, bind_inPosition);

//     //      vbo
//     //  buf1
//     float data1[]
//     {
//         //  quad
//         -0.4, 0.f,  //  bl
//         0.f, 0.f,   //  br
//         0.f, 0.4,   //  tr
//         -0.4, 0.4,  //  tl
//     };
//     pb1 = new ZC_Buffer(GL_ARRAY_BUFFER);
//     pb1->GLNamedBufferStorage(sizeof(el) + sizeof(data1), el, GL_DYNAMIC_STORAGE_BIT);
//     pb1->GLNamedBufferSubData(sizeof(el), sizeof(data1), data1);
    
//     //  buf2
//     pb2el = new ZC_Buffer(GL_ARRAY_BUFFER);
//     pb2el->GLNamedBufferStorage(sizeof(el), el, 0);
//     pb2ver = new ZC_Buffer(GL_ARRAY_BUFFER);
//     pb2ver->GLNamedBufferStorage(sizeof(ver), ver, 0);

//     //  ubo
//     Data dat1
//     {
//         ZC_Vec2<float>(-0.2f, -0.2f),
//         ZC_Vec4<float>(1.f, 1.f, 0.f, 1.f)
//     };
//     Data dat2
//     {
//         ZC_Vec2<float>(0.2f, 0.2f),
//         ZC_Vec4<float>(0.f, 1.f, 1.f, 1.f)
//     };

//         //  uses -> uniform block
//     glCreateBuffers(1, &ubo1);
//     glNamedBufferStorage(ubo1, sizeof(Data), &dat1, GL_DYNAMIC_STORAGE_BIT);
//     glCreateBuffers(1, &ubo2);
//     glNamedBufferStorage(ubo2, sizeof(Data), &dat2, GL_DYNAMIC_STORAGE_BIT);
// }

// void ZC_Renderer::Draw()
// {
//     this->UpdateUBO();

//     auto prevIter = renders.before_begin();
//     for (auto curIter = renders.begin(); curIter != renders.end(); )
//         curIter = (*curIter)->Draw() ? ++curIter : renders.erase_after(prevIter);

//     glClearColor(0.3f,0.3f,0.3f,1);
//     glClear(GL_COLOR_BUFFER_BIT);
//     static bool first = true;
//     if (first) Create1();
//         first = false;


//     pShP->ActivateOpenGL();
//     pVao->ActivateOpenGL();

//     GLuint vaoId = pVao->id;

//             //  first draw

//         //  uses -> uniform block
//     GLuint uboBinding = 1;
//     glBindBufferBase(GL_UNIFORM_BUFFER, uboBinding, ubo1);

//     glVertexArrayVertexBuffer(vaoId, bind_inPosition, pb1->id, sizeof(el), 8);
//     glVertexArrayElementBuffer(vaoId, pb1->id);
    
//     glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_BYTE, 0);


//             //  second draw

//         //  uses -> uniform block
//     glBindBufferBase(GL_UNIFORM_BUFFER, uboBinding, ubo2);

//     glVertexArrayVertexBuffer(vaoId, bind_inPosition, pb2ver->id, 0, 8);
//     glVertexArrayElementBuffer(vaoId, pb2el->id);

//     glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_BYTE, 0);

//     glBindVertexArray(0);
//     glUseProgram(0);


//     //  my rendering finished, make openGL state default
//     ZC_VAO::UnbindVertexArray();
//     ZC_GLBlend::Disable();
//     ZC_Framebuffer::Unbind();

// #ifdef ZC_IMGUI
//     ZC_IGWindow::Draw();
// #endif

//     funcSwapBuffer();
// }




// //                          ssbo withought vao
// //                               	std 430 alignment (struct in std430 array, alignes by the largest element of that struct, largest of posible is vec4)
// #include <ZC/Video/OpenGL/Shader/ZC_Shader.h>
// #include <ZC/Video/OpenGL/Shader/ZC_ShProg.h>
// #include <ZC/Video/OpenGL/Buffer/ZC_Buffer.h>
// #include <ZC/Video/OpenGL/Texture/ZC_Texture.h>

// GLuint vs = 0,
//     fs = 0;
// ZC_Texture tex;
// ZC_ShProg* pShP;

// GLuint bind_inPosition = 0;

// struct Vertex
// {
//     float v[2];
//     float uv[2];
// };

// GLuint vertBuf = 0;
// GLuint indexBuf = 0;

// void Create1()
// {
//     ZC_Shader vs(ZC_Shader::ReadShaderFile("/home/dmitry/projects/ZCreator/ZeroCore/src/Video/OpenGL/Renderer/test.vs", GL_VERTEX_SHADER).pHead, GL_VERTEX_SHADER);
//     ZC_Shader fs(ZC_Shader::ReadShaderFile("/home/dmitry/projects/ZCreator/ZeroCore/src/Video/OpenGL/Renderer/test.fs", GL_FRAGMENT_SHADER).pHead, GL_FRAGMENT_SHADER);
//     pShP = new ZC_ShProg(vs.id, fs.id, 0);
   
//         //  vertex buffer
//     glCreateBuffers(1, &vertBuf);
//     Vertex vertex[]
//     {
//         { .v { -0.2, -0.2 }, .uv { 0, 0 } },    //  bl
//         { .v {  0.2, -0.2 }, .uv { 1, 0 } },    //  br
//         { .v {  0.2,  0.2 }, .uv { 1, 1 } },    //  tr
//         { .v { -0.2,  0.2 }, .uv { 0, 1 } },    //  tl
//     };
//     glNamedBufferStorage(vertBuf, sizeof(vertex), vertex, 0);

//         //  index buffer
//     glCreateBuffers(1, &indexBuf);
//     uint indexes[]
//     {
//         0,1,2,
//         0,2,3
//     };
//     glNamedBufferStorage(indexBuf, sizeof(indexes), indexes, 0);

//     tex = ZC_Texture::LoadTexture2D("/home/dmitry/Загрузки//wall.jpg");
// }
// GLuint vao;
// void ZC_Renderer::Draw()
// {
//     this->UpdateUBO();

//     auto prevIter = renders.before_begin();
//     for (auto curIter = renders.begin(); curIter != renders.end(); )
//         curIter = (*curIter)->Draw() ? ++curIter : renders.erase_after(prevIter);

//     glClearColor(0.3f,0.3f,0.3f,1);
//     glClear(GL_COLOR_BUFFER_BIT);
    
//     static bool first = true;
//     if (first)
//     {
//         Create1();
//         first = false;
//         glCreateVertexArrays(1, &vao);
//     }

//     glBindVertexArray(vao);

//     glUseProgram(pShP->id);     //  pay attension!!!!, don't use pShP->ActivateOpenGL(), there is no unbind function, but unbind makes later glUseProgram(0);!

//     tex.GLBindTextureUnit(0);

//     glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 0, vertBuf);
//     // glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 1, indexBuf);

//     glVertexArrayElementBuffer(vao, indexBuf);
//     glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

//     // glDrawArrays(GL_TRIANGLE_FAN, 0, 4);


//     glBindVertexArray(0);
//     glUseProgram(0);


//     //  my rendering finished, make openGL state default
//     ZC_VAO::UnbindVertexArray();
//     ZC_GLBlend::Disable();
//     ZC_Framebuffer::Unbind();

//     funcSwapBuffer();
// }