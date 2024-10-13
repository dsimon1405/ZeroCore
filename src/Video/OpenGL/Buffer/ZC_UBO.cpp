#include <ZC/Video/OpenGL/Buffer/ZC_UBO.h>

ZC_UBO::ZC_UBO(ZC_UBO_BindingPoint _bindingPoint)
	: buffer(GL_UNIFORM_BUFFER),
	bindingPoint(static_cast<GLuint>(_bindingPoint))
{}

ZC_UBO::ZC_UBO(ZC_UBO&& ubo) noexcept
    : buffer(std::move(ubo.buffer)),
    bindingPoint(ubo.bindingPoint)
{}

ZC_UBO& ZC_UBO::operator = (ZC_UBO&& ubo)
{
    buffer = std::move(ubo.buffer);
    bindingPoint = ubo.bindingPoint;
    return *this;
}

void ZC_UBO::GLNamedBufferData(GLsizeiptr bytesSize, const void* pData, GLenum usage)
{
	buffer.GLNamedBufferData(bytesSize, pData, usage);
	glBindBufferRange(GL_UNIFORM_BUFFER, bindingPoint, buffer.id, 0, bytesSize);
}

void ZC_UBO::GLNamedBufferStorage(GLsizeiptr bytesSize, const void* pData, GLbitfield flags)
{
	buffer.GLNamedBufferStorage(bytesSize, pData, flags);
	glBindBufferRange(GL_UNIFORM_BUFFER, bindingPoint, buffer.id, 0, bytesSize);
}

void ZC_UBO::GLNamedBufferSubData(GLintptr offset, GLsizeiptr bytesSize, const void* pData)
{
    buffer.GLNamedBufferSubData(offset, bytesSize, pData);
}