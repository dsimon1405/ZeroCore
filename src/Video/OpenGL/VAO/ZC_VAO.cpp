#include <ZC/Video/OpenGL/VAO/ZC_VAO.h>

ZC_VAO::ZC_VAO()
{
    glCreateVertexArrays(1, &id);
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

void ZC_VAO::Config(VAOConData vaoConfigData, ZC_Buffer& vbo, ZC_Buffer* ebo, GLuint startOffset, GLuint verticesCount)
{
    glBindVertexArray(id);
    vbo.BindBuffer();

    ZC_VAOConfig config(vaoConfigData);
    config.Config(startOffset, verticesCount);
    
    if (ebo) ebo->BindBuffer();
    
    glBindVertexArray(0);
    vbo.UnbindBuffer();
    if (ebo) ebo->UnbindBuffer();
}

void ZC_VAO::ActivateOpenGL() const
{
    glBindVertexArray(id);
}

void ZC_VAO::UnbindVertexArray()
{
    glBindVertexArray(0);
}


// void ZC_VAO::DrawArrays(const ZC_VBO* vbo, long bufferOffset, GLenum mode, int count) const
// {
//     BindVertexArray();
//     vbo->BindVertexBuffer(configID, bufferOffset, stride);
//     glDrawArrays(mode, 0, count);
//     UnbindVertexArray();
// }