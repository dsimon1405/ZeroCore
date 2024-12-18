#include <ZC/Video/OpenGL/Buffer/ZC_Buffer.h>

#include <cstring>

ZC_Buffer::ZC_Buffer(GLenum _type, GLuint _binding)
    : type(_type),
    binding(_binding)
{
    static const ushort buffersSize = 2000;
    static GLuint buffers[buffersSize];
    static ulong buffersIndex = 0;
    if (buffersIndex == 0 || buffersIndex == buffersSize)
    {
        glCreateBuffers(buffersSize, buffers);
        buffersIndex = 0;
    }
    id = buffers[buffersIndex++];
}

ZC_Buffer::ZC_Buffer(ZC_Buffer&& b) noexcept
    : id(b.id),
    type(b.type),
    binding(b.binding)
{
    b.id = 0;
}

ZC_Buffer& ZC_Buffer::operator = (ZC_Buffer&& b)
{
    if (id != 0) glDeleteBuffers(1, &id);
    id = b.id;
    type = b.type;
    binding = b.binding;

    b.id = 0;
    return *this;
}

ZC_Buffer::~ZC_Buffer()
{
    glDeleteBuffers(1, &id);
}

void ZC_Buffer::BindBuffer()
{
    glBindBuffer(type, id);
}

void ZC_Buffer::UnbindBuffer()
{
    glBindBuffer(type, 0);
}

GLenum ZC_Buffer::GetType() const noexcept
{
    return type;
}

ZC_Buffer ZC_Buffer::CreateAndFillStorage(GLuint _binding, GLsizeiptr bytesSize, const void* pData, GLbitfield flags)
{
    ZC_Buffer buf(GL_SHADER_STORAGE_BUFFER, _binding);
    buf.GLNamedBufferStorage(bytesSize, pData, flags);
    return buf;
}

void ZC_Buffer::GLNamedBufferData(GLsizeiptr bytesSize, const void* pData, GLenum _usage)
{
    glNamedBufferData(id, bytesSize, pData, _usage);
}

void ZC_Buffer::GLNamedBufferStorage(GLsizeiptr bytesSize, const void* pData, GLbitfield flags)
{
    glNamedBufferStorage(id, bytesSize, pData, flags);
}

void ZC_Buffer::GLNamedBufferSubData(GLintptr offset, GLsizeiptr bytesSize, const void* pData)
{
    glNamedBufferSubData(id, offset, bytesSize, pData);
}

void ZC_Buffer::GLBindBufferBase() const
{
    assert(type == GL_ATOMIC_COUNTER_BUFFER || type == GL_TRANSFORM_FEEDBACK_BUFFER || type == GL_UNIFORM_BUFFER || type == GL_SHADER_STORAGE_BUFFER);
    glBindBufferBase(type, binding, id);
}

bool ZC_Buffer::GLMapNamedBufferRange_Write(GLintptr offset, GLsizeiptr length, void* pData)
{
    void* pMap = glMapNamedBufferRange(id, offset, length, GL_MAP_WRITE_BIT);
    if (pData) std::memcpy(pMap, pData, length);
    glUnmapNamedBuffer(id);
    return pMap != nullptr;
}

void ZC_Buffer::GetElementsData(ul_zc maxElementsIndex, ul_zc& storingTypeSize, GLenum& rElementsType) noexcept
{
    if (maxElementsIndex <= UCHAR_MAX)
    {
        storingTypeSize = sizeof(uch_zc);
        rElementsType = GL_UNSIGNED_BYTE;
    }
    else if (maxElementsIndex <= USHRT_MAX)
    {
        storingTypeSize = sizeof(ush_zc);
        rElementsType = GL_UNSIGNED_SHORT;
    }
    else
    {
        storingTypeSize = sizeof(ui_zc);
        rElementsType = GL_UNSIGNED_INT;
    }
}

ZC_DA<uchar> ZC_Buffer::GetTriangleElements(ul_zc& rElementsCount, GLenum& rElementsType, ul_zc quadsCount, ul_zc trianglesCount)
{
    ul_zc quadsElementsCount = quadsCount * 6,     //  6 elements in ebo on one quad
        trianglesElementsCount = trianglesCount * 3;     //  3 elements in ebo on one triangle
    rElementsCount = quadsElementsCount + trianglesElementsCount;  
    ul_zc verticesInVBO = (quadsCount * 4) + trianglesElementsCount,     //  4 vertices in vbo on one quad
        storingTypeSize = 0;
    ZC_Buffer::GetElementsData(verticesInVBO - 1, storingTypeSize, rElementsType);
    ZC_DA<uch_zc> elements(storingTypeSize * rElementsCount);
    switch (storingTypeSize)
    {
    case 1: FillTriangleElements<uch_zc>(elements.pHead, elements.size, quadsElementsCount); break;
    case 2: FillTriangleElements<ush_zc>(reinterpret_cast<ush_zc*>(elements.pHead), elements.size / sizeof(ush_zc), quadsElementsCount); break;
    case 4: FillTriangleElements<ui_zc>(reinterpret_cast<ui_zc*>(elements.pHead), elements.size / sizeof(ui_zc), quadsElementsCount); break;
    }
    return elements;
}