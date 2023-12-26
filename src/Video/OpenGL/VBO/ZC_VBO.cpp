#include <ZC/Video/OpenGL/VBO/ZC_VBO.h>

#include <Video/OpenGL/ZC_OpenGL.h>

ZC_VBO::ZC_VBO(ZC_VBO&& vbo) noexcept
    : ZC_Buffer(dynamic_cast<ZC_Buffer&&>(vbo))
{}

ZC_VBO& ZC_VBO::operator = (ZC_VBO&& vbo)
{
    dynamic_cast<ZC_Buffer&>(*this) = dynamic_cast<ZC_Buffer&&>(vbo);
    return *this;
}

void ZC_VBO::BindVertexBuffer(GLuint vaoConfig, long offset, int stride) const
{
    glBindVertexBuffer(vaoConfig, this->id, offset, stride);
}

ZC_VBO::ZC_VBO(GLenum type)
    : ZC_Buffer(type)
{}

// bool ZC_VBO::BufferData(long bytesSize, GLenum usage)
// {
//     return buffer.BufferData(GL_ARRAY_BUFFER, bytesSize, usage);
// }

// #ifdef ZC_PC
// bool ZC_VBO::BufferData(long bytesSize, void* pData, GLenum usage)
// {
//     buffer.BufferData(GL_ARRAY_BUFFER, bytesSize, pData, usage);
// }

// bool ZC_VBO::BufferSubData(long offset, long bytesSize, void* pData)
// {
//     buffer.BufferSubData(GL_ARRAY_BUFFER, offset, bytesSize, pData);
// }
// #endif