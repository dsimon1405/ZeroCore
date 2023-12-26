#include <ZC/Video/OpenGL/VAO/ZC_VAO.h>

#include <Video/OpenGL/ZC_OpenGL.h>

ZC_VAO::ZC_VAO(ZC_VAO&& vao) noexcept
    : id(vao.id),
    configID(vao.configID),
    stride(vao.stride)
{
    vao.id = 0;
}

ZC_VAO& ZC_VAO::operator = (ZC_VAO&& vao)
{
    if (id != 0) glDeleteVertexArrays(1, &id);
    id = vao.id;
    configID = vao.configID;
    stride = vao.stride;

    vao.id = 0;
    return *this;
}

ZC_VAO::~ZC_VAO()
{
    glDeleteVertexArrays(1, &id);
}
void ZC_VAO::UnbindVertexArray()
{
    glBindVertexArray(0);
}

void ZC_VAO::BindVertexArray() const
{
    glBindVertexArray(id);
}

void ZC_VAO::DrawArrays(const ZC_VBO* vbo, long bufferOffset, GLenum mode, int count) const
{
    BindVertexArray();
    vbo->BindVertexBuffer(configID, bufferOffset, stride);
    glDrawArrays(mode, 0, count);
    UnbindVertexArray();
}

ZC_VAO::ZC_VAO(const GLuint& _configID)
    : configID(_configID)
{
    glGenVertexArrays(1, &id);
}