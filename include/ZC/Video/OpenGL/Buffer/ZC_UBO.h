#pragma once

#include <ZC/Video/OpenGL/Buffer/ZC_Buffer.h>

enum ZC_UBO_BindingPoint
{
	ZC_UBO_BP__Camera = 0,
	ZC_UBO_BP__
};

struct ZC_UBO
{
    ZC_UBO(ZC_UBO_BindingPoint _bindingPoint);

	ZC_UBO(ZC_UBO&& vbo) noexcept;
	ZC_UBO& operator = (ZC_UBO&& vbo);

	/*
	Stores data in a buffer or reserves space.

	Params:
	- bytesSize - data bytes size.
	- data - data to save (if nullptr, reserve place).
	- _usage - style of using stored data (GL_STREAM_DRAW, GL_STREAM_READ, GL_STREAM_COPY, GL_STATIC_DRAW, GL_STATIC_READ, GL_STATIC_COPY,
		GL_DYNAMIC_DRAW, GL_DYNAMIC_READ, or GL_DYNAMIC_COPY).
	
	Return:
	On success true, otherwise false (in second case ZC_ErrorLogger::ErrorMessage() - for more information).
	*/
	void GLNamedBufferData(GLsizeiptr bytesSize, const void* pData, GLenum _usage);

	/*
	Stores data in a buffer or reserves space. Make buffer no resizable. May be called only once, double call or call GLNamedBufferData(...) returns exception!

	Params:
	- bytesSize - data bytes size.
	- data - data to save (if nullptr, reserve place).
	- flags - 0, GL_DYNAMIC_STORAGE_BIT, GL_MAP_COHERENT_BIT (must include GL_MAP_PERSISTENT_BIT (must include GL_MAP_READ_BIT or GL_MAP_WRITE_BIT)).
	*/
	void GLNamedBufferStorage(GLsizeiptr bytesSize, const void* pData, GLbitfield flags);

	/*
	Save new data in buffer.

	Params:
	offset - offset in bytes before editing starts.
	bytesSize - data bytes size.
	pData - data to save.
	
	Return:
	On success true, otherwise false (in second case ZC_ErrorLogger::ErrorMessage() - for more information).
	*/
	void GLNamedBufferSubData(GLintptr offset, GLsizeiptr bytesSize, const void* pData);

private:
	ZC_Buffer buffer;
	GLuint bindingPoint;
};