#pragma once

#include <ZC/Video/OpenGL/Buffer/ZC_Buffer.h>

#include <utility>

class ZC_UBOs;

class ZC_UBO : public ZC_Buffer
{
    friend class ZC_UBOs;
public:
	ZC_UBO(ZC_UBO&& vbo) noexcept;
	ZC_UBO& operator = (ZC_UBO&& vbo);

    ~ZC_UBO() override = default;

	/*
	Saves data to a ubo.

	Params:
	data - data to save.
	_usage - style of using stored data (GL_STREAM_DRAW, GL_STREAM_READ, GL_STREAM_COPY, GL_STATIC_DRAW, GL_STATIC_READ, GL_STATIC_COPY, GL_DYNAMIC_DRAW, GL_DYNAMIC_READ, or GL_DYNAMIC_COPY).
	
	Return:
	On success true, otherwise false (in second case ZC_ErrorLogger::ErrorMessage() - for more information).
	*/
	template<ZC_cOpenGLContainer TOpenGLCont>
	bool BufferData(TOpenGLCont&& data, GLenum _usage);

private:
	GLuint bindingPoint;

    ZC_UBO(GLuint _type, GLuint _bindingPoint);

	bool BindBufferRange(long bytesSize);
};

template<ZC_cOpenGLContainer TOpenGLCont>
bool ZC_UBO::BufferData(TOpenGLCont&& data, GLenum _usage)
{
	long bytesSize = static_cast<long>(data.BytesSize());
	if (!BuffData(bytesSize, static_cast<void*>(data.pHead), _usage)) return false;
#ifdef ZC_ANDROID
	data.pHead = nullptr;
#endif
	return BindBufferRange(bytesSize);
}