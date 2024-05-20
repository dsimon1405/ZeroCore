#include <ZC/Video/OpenGL/Renderer/ZC_Renderer.h>

#include <ZC/Tools/Container/ZC_ContFunc.h>
#include <ZC/Video/OpenGL/ZC_GLBlend.h>
#include <ZC_Config.h>
#ifdef ZC_IMGUI
#include <ZC_IGWindow.h>
#endif

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






//  try smoothstep  !!!


const char shVCode[] = 
"#version 460 core\n"
"layout (location = 0) in vec2 inPosition;\n"
"layout (location = 1) in float r;\n"
"layout (location = 2) in float g;\n"
"layout (location = 3) in float b;\n"

"out vec4 vColor;\n"

"void main()\n"
"{\n"
    "gl_Position = vec4(inPosition, 0.f, 1.f);\n"
    "vColor = vec4(r, g, b, 1.f);\n"
"}\n";

const char* shFCode = 
"#version 460 core\n"

"in vec4 vColor;\n"
"out vec4 FragColor;\n"

"void main()\n"
"{\n"
    "FragColor = vColor;\n"
"}\n";

GLuint shV = 0,
    shF = 0;
#include <ZC/Video/OpenGL/Shader/ZC_Shader.h>
#include <ZC/Video/OpenGL/Shader/ZC_ShProg.h>
#include <ZC/Video/OpenGL/Buffer/ZC_Buffer.h>
ZC_ShProg* pShP = nullptr;
ZC_Buffer* pb1 = nullptr,
    * pb2el = nullptr,
    * pb2ver = nullptr,
    * pb2r = nullptr,
    * pb2g = nullptr,
    * pb2b = nullptr;
ZC_VAO* pVao = nullptr;

    GLuint vbufIndex_pos = 0,
        vbufIndex_r = 1,
        vbufIndex_g = 2,
        vbufIndex_b = 3;


    GLubyte el[] { 0,1,2,  0,2,3};    //  elements
    float ver[] =
    {
        0.f, 0.f,    //  bl
        0.4f, 0.f,    //  br
        0.4f, 0.4f,     //  tr
        0.0, 0.4,       //  tl
    };  //  quad
    float r[] = { 0.f, 0.f, 0.f, 0.f, },    //  r
        g[] = { 1.f, 1.f, 1.f, 1.f, },    //  g
        b[] = { 1.f, 1.f, 1.f, 1.f };    //  b

void Create1()
{
    ZC_Shader vsh(shVCode, GL_VERTEX_SHADER);
    ZC_Shader fsh(shFCode, GL_FRAGMENT_SHADER);
    pShP = new ZC_ShProg(vsh.id, fsh.id, 0);

    GLuint shP_atrib_pos = 0,
        shP_atrib_r = 1,
        shP_atrib_g = 2,
        shP_atrib_b = 3;

    pVao = new ZC_VAO;
    GLuint id1 = pVao->id;
    glEnableVertexArrayAttrib(id1, shP_atrib_pos);
    glVertexArrayAttribFormat(id1, shP_atrib_pos, 2, GL_FLOAT, false, 0);
    glVertexArrayAttribBinding(id1, shP_atrib_pos, vbufIndex_pos);

    glEnableVertexArrayAttrib(id1, shP_atrib_r);
    glVertexArrayAttribFormat(id1, shP_atrib_r, 1, GL_FLOAT, false, 0);
    glVertexArrayAttribBinding(id1, shP_atrib_r, vbufIndex_r);

    glEnableVertexArrayAttrib(id1, shP_atrib_g);
    glVertexArrayAttribFormat(id1, shP_atrib_g, 1, GL_FLOAT, false, 0);
    glVertexArrayAttribBinding(id1, shP_atrib_g, vbufIndex_g);

    glEnableVertexArrayAttrib(id1, shP_atrib_b);
    glVertexArrayAttribFormat(id1, shP_atrib_b, 1, GL_FLOAT, false, 0);
    glVertexArrayAttribBinding(id1, shP_atrib_b, vbufIndex_b);

    float data1[]
    {
        //  quad
        -0.4, 0.f,  //  bl
        0.f, 0.f,   //  br
        0.f, 0.4,   //  tr
        -0.4, 0.4,  //  tl

        1.f, 1.f, 1.f, 1.f,  //  r
        1.f, 1.f, 1.f, 1.f,  //  g
        0.f, 0.f, 0.f, 0.f,  //  b
    };
    pb1 = new ZC_Buffer(GL_ARRAY_BUFFER);
    pb1->GLNamedBufferStorage(sizeof(el) + sizeof(data1), el, GL_DYNAMIC_STORAGE_BIT);
    pb1->GLNamedBufferSubData(sizeof(el), sizeof(data1), data1);


    pb2el = new ZC_Buffer(GL_ARRAY_BUFFER);
    pb2el->GLNamedBufferStorage(sizeof(el), el, 0);
    pb2ver = new ZC_Buffer(GL_ARRAY_BUFFER);
    pb2ver->GLNamedBufferStorage(sizeof(ver), ver, 0);
    pb2r = new ZC_Buffer(GL_ARRAY_BUFFER);
    pb2r->GLNamedBufferStorage(sizeof(r), r, 0);
    pb2g = new ZC_Buffer(GL_ARRAY_BUFFER);
    pb2g->GLNamedBufferStorage(sizeof(g), g, 0);
    pb2b = new ZC_Buffer(GL_ARRAY_BUFFER);
    pb2b->GLNamedBufferStorage(sizeof(b), b, 0);
}

void ZC_Renderer::Draw()
{
    this->UpdateUBO();

    auto prevIter = renders.before_begin();
    for (auto curIter = renders.begin(); curIter != renders.end(); )
        curIter = (*curIter)->Draw() ? ++curIter : renders.erase_after(prevIter);

glClearColor(0.3f,0.3f,0.3f,1);
glClear(GL_COLOR_BUFFER_BIT);
static bool first = true;
if (first) Create1();
    first = false;


    pShP->ActivateOpenGL();
    pVao->ActivateOpenGL();

    GLuint vboId = pVao->id;

    glVertexArrayVertexBuffer(vboId, vbufIndex_pos, pb1->id, sizeof(el), 8);
    glVertexArrayVertexBuffer(vboId, vbufIndex_r, pb1->id, sizeof(el) + sizeof(ver), 4);
    glVertexArrayVertexBuffer(vboId, vbufIndex_g, pb1->id, sizeof(el) + sizeof(ver) + sizeof(r), 4);
    glVertexArrayVertexBuffer(vboId, vbufIndex_b, pb1->id, sizeof(el) + sizeof(ver) + sizeof(r) + sizeof(g), 4);
    glVertexArrayElementBuffer(vboId, pb1->id);
    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_BYTE, 0);

    glVertexArrayVertexBuffer(vboId, vbufIndex_pos, pb2ver->id, 0, 8);
    glVertexArrayVertexBuffer(vboId, vbufIndex_r, pb2r->id, 0, 4);
    glVertexArrayVertexBuffer(vboId, vbufIndex_g, pb2g->id, 0, 4);
    glVertexArrayVertexBuffer(vboId, vbufIndex_b, pb2b->id, 0, 4);
    glVertexArrayElementBuffer(vboId, pb2el->id);
    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_BYTE, 0);

    glBindVertexArray(0);
    glUseProgram(0);


    //  my rendering finished, make openGL state default
    ZC_VAO::UnbindVertexArray();
    ZC_GLBlend::Disable();
    ZC_Framebuffer::Unbind();

#ifdef ZC_IMGUI
    ZC_IGWindow::Draw();
#endif

    funcSwapBuffer();
}