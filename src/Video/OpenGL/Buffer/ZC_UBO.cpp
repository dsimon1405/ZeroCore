#include <ZC/Video/OpenGL/Buffer/ZC_UBO.h>

#include <Video/OpenGL/ZC_OpenGL.h>
#include <ZC/ErrorLogger/ZC_ErrorLogger.h>

ZC_UBO::ZC_UBO(BindingPoint _bindingPoint)
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

void ZC_UBO::BufferData(long bytesSize, void* pData, GLenum usage)
{
	buffer.BufferData(bytesSize, pData, usage);
	glBindBufferRange(GL_UNIFORM_BUFFER, bindingPoint, buffer.id, 0, bytesSize);
}

void ZC_UBO::BufferSubData(long offset, long bytesSize, void* pData)
{
    buffer.BufferSubData(offset, bytesSize, pData);
}