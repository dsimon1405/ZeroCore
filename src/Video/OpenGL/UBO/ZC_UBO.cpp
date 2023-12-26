#include <ZC/Video/OpenGL/UBO/ZC_UBO.h>

#include <Video/OpenGL/ZC_OpenGL.h>
#include <ZC/ErrorLogger/ZC_ErrorLogger.h>

ZC_UBO::ZC_UBO(ZC_UBO&& ubo) noexcept
    : ZC_Buffer(dynamic_cast<ZC_Buffer&&>(ubo)),
    bindingPoint(ubo.bindingPoint)
{}

ZC_UBO& ZC_UBO::operator = (ZC_UBO&& vbo)
{
    dynamic_cast<ZC_Buffer&>(*this) = dynamic_cast<ZC_Buffer&&>(vbo);
    bindingPoint = vbo.bindingPoint;
    return *this;
}

bool ZC_UBO::BindBufferRange(long bytesSize)
{
	glBindBufferRange(this->type, bindingPoint, this->id, 0, bytesSize);
    return !ZC_ErrorLogger::WasError();
}

ZC_UBO::ZC_UBO(GLuint _type, GLuint _bindingPoint)
	: ZC_Buffer(_type),
	bindingPoint(_bindingPoint)
{}