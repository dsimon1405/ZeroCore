#pragma once

#include <ZC/Tools/Math/ZC_Math.h>
#include <ZC/Tools/Container/ZC_DA.h>

#include <glad/glad.h>

//	Wrapper OpenGL buffer.
struct ZC_Buffer
{
	ZC_Buffer() = default;
	ZC_Buffer(GLenum _type, GLuint _binding = 0);

	ZC_Buffer(ZC_Buffer&& vbo) noexcept;
	ZC_Buffer& operator = (ZC_Buffer&& vbo);

	~ZC_Buffer();

	void BindBuffer();
	void UnbindBuffer();

	static ZC_Buffer CreateAndFillStorage(GLuint _binding, GLsizeiptr bytesSize, const void* pData, GLbitfield flags);

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
	- offset - offset in bytes before editing starts.
	- bytesSize - data bytes size.
	- pData - data to save.
	
	Return:
	On success true, otherwise false (in second case ZC_ErrorLogger::ErrorMessage() - for more information).
	*/
	void GLNamedBufferSubData(GLintptr offset, GLsizeiptr bytesSize, const void* pData);

	void GLBindBufferBase();
	bool GLMapNamedBufferRange_Write(GLintptr offset, GLsizeiptr length, void* pData);

	/*
	Determines, from the maximum index of an element, the size of the element's type and the type of that element from open gl enum.

	Params:
	- maxElementsIndex - maximum index of an element.
	- storingTypeSize - here places the size of the element's type.
	- rElementsType - here places the type of that element from open gl enum.
	*/
    static void GetElementsData(ulong maxElementsIndex, ulong& storingTypeSize, GLenum& rElementsType) noexcept;

	/*
	Create dynamic array of elements for drawing quads and(or) triangles. Im vbo vertices(corners) of quad must be in order [bl -> tr -> tl -> br].

	Params:
	rElementsCount - set count of elements in ebo (ZC_DrawElements>(..., elementsCount, ..., ...);)
	rElementsType - set elements type in ebo (ZC_DrawElements>(..., ..., elementsType, ...);)
	quadsCount - count of quads in vbo.
	trianglesCount - count of triangles in vbo.

	Return:
	Array of element in uchar format. Using:
		ZC_Buffer ebo(GL_ELEMENT_ARRAY_BUFFER);
		ebo.GLNamedBufferData(elements.size, elements.Begin(), ...);
	*/
	static ZC_DA<uchar> GetTriangleElements(ulong& rElementsCount, GLenum& rElementsType, ulong quadsCount, ulong trianglesCount);

	GLuint id = 0;
	GLenum type;
	GLuint binding = 0;
	
private:
	template<typename T>
	static void FillTriangleElements(T* pElementsHead, ulong elementsSize, ulong quadsElementsCount);
};

template<typename T>
void ZC_Buffer::FillTriangleElements(T* pElementsHead, ulong elementsSize, ulong quadsElementsCount)
{
	ulong elementsI = 0,
		index = 0;
	for ( ; elementsI < quadsElementsCount; elementsI += 6)
	{
		pElementsHead[elementsI] = pElementsHead[elementsI + 3] = static_cast<T>(index++);
		pElementsHead[elementsI + 1] = pElementsHead[elementsI + 5] = static_cast<T>(index++);
		pElementsHead[elementsI + 2] = static_cast<T>(index++);
		pElementsHead[elementsI + 4] = static_cast<T>(index++);
	}
	while (elementsI < elementsSize)
	{
		pElementsHead[elementsI++] = static_cast<T>(index++);
		pElementsHead[elementsI++] = static_cast<T>(index++);
		pElementsHead[elementsI++] = static_cast<T>(index++);
	}
}