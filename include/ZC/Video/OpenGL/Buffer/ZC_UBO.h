#pragma once

#include <ZC/Video/OpenGL/Buffer/ZC_Buffer.h>

struct ZC_UBO
{
	enum BindingPoint
	{
		ProjView = 0,
		Ortho = 1,
		CamPos = 2,
	};

	ZC_Buffer buffer;
	GLuint bindingPoint;

    ZC_UBO(BindingPoint _bindingPoint);

	ZC_UBO(ZC_UBO&& vbo) noexcept;
	ZC_UBO& operator = (ZC_UBO&& vbo);

	/*
	Saves data to a ubo.

	Params:
	data - data to save.
	_usage - style of using stored data (GL_STREAM_DRAW, GL_STREAM_READ, GL_STREAM_COPY, GL_STATIC_DRAW, GL_STATIC_READ, GL_STATIC_COPY, GL_DYNAMIC_DRAW, GL_DYNAMIC_READ, or GL_DYNAMIC_COPY).
	
	Return:
	On success true, otherwise false (in second case ZC_ErrorLogger::ErrorMessage() - for more information).
	*/
	void BufferData(long bytesSize, void* pData, GLenum usage);

	/*
	Save new data in buffer.

	Params:
	offset - offset in bytes before editing starts.
	bytesSize - data bytes size.
	pData - data to save.
	
	Return:
	On success true, otherwise false (in second case ZC_ErrorLogger::ErrorMessage() - for more information).
	*/
	void BufferSubData(long offset, long bytesSize, void* pData);
};