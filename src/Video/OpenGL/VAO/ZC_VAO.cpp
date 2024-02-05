#include <ZC/Video/OpenGL/VAO/ZC_VAO.h>

#include <Video/OpenGL/ZC_OpenGL.h>

ZC_VAO::ZC_VAO()
{
    glGenVertexArrays(1, &id);
}

ZC_VAO::ZC_VAO(ZC_VAO&& vao) noexcept
    : id(vao.id)
{
    vao.id = 0;
}

ZC_VAO& ZC_VAO::operator = (ZC_VAO&& vao)
{
    if (id != 0) glDeleteVertexArrays(1, &id);
    id = vao.id;

    vao.id = 0;
    return *this;
}

ZC_VAO::~ZC_VAO()
{
    glDeleteVertexArrays(1, &id);
}

void ZC_VAO::Config(ZC_VAOConfig* config, ZC_Buffer& vbo, ZC_Buffer& ebo, GLuint startOffset, GLuint verticesCount)
{
    glBindVertexArray(id);
    vbo.BindBuffer();
    config->Config(startOffset, verticesCount);
    ebo.BindBuffer();
    glBindVertexArray(0);
    vbo.UnbindBuffer();
    ebo.UnbindBuffer();
}

void ZC_VAO::BindVertexArray()
{
    glBindVertexArray(id);
}


// void ZC_VAO::DrawArrays(const ZC_VBO* vbo, long bufferOffset, GLenum mode, int count) const
// {
//     BindVertexArray();
//     vbo->BindVertexBuffer(configID, bufferOffset, stride);
//     glDrawArrays(mode, 0, count);
//     UnbindVertexArray();
// }