#pragma once

#include <ZC_Config.h>
#include <ZC/Video/OpenGL/GL/glcorearb.h>
#include <ZC/Tools/Math/ZC_Math.h>
#include <ZC/Tools/Container/ZC_DA.h>
#ifdef ZC_ANDROID
#include <list>
#endif

//	Wrapper OpenGL buffer.
struct ZC_Buffer
{
	ZC_Buffer(GLenum _type);

	ZC_Buffer(ZC_Buffer&& vbo) noexcept;
	ZC_Buffer& operator = (ZC_Buffer&& vbo);

	~ZC_Buffer();

	void BindBuffer();
	void UnbindBuffer();

	/*
	Saves data to a buffer, or reserve place.

	Params:
	bytesSize - data bytes size.
	data - data to save (if nullptr, reserve place).
	_usage - style of using stored data (GL_STREAM_DRAW, GL_STREAM_READ, GL_STREAM_COPY, GL_STATIC_DRAW, GL_STATIC_READ, GL_STATIC_COPY, GL_DYNAMIC_DRAW, GL_DYNAMIC_READ, or GL_DYNAMIC_COPY).
	
	Return:
	On success true, otherwise false (in second case ZC_ErrorLogger::ErrorMessage() - for more information).
	*/
	void BufferData(long bytesSize, const void* pData, GLenum _usage);

	/*
	Save new data in buffer.

	Params:
	offset - offset in bytes before editing starts.
	bytesSize - data bytes size.
	pData - data to save.
	
	Return:
	On success true, otherwise false (in second case ZC_ErrorLogger::ErrorMessage() - for more information).
	*/
	void BufferSubData(long offset, long bytesSize, const void* pData);

    static void GetElementsData(size_t maxElementsIndex, size_t& storingTypeSize, GLenum& rElementsType) noexcept;

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
		ebo.BufferData(elements.size, elements.Begin(), ...);
	*/
	static ZC_DA<uchar> GetTriangleElements(size_t& rElementsCount, GLenum& rElementsType, size_t quadsCount, size_t trianglesCount);

	GLuint id = 0;
	GLenum type;
	
private:
	template<typename TpHead>
	static void FillTriangleElements(TpHead pElementsHead, size_t elementsSize, size_t quadsElementsCount);

#ifdef ZC_ANDROID
	struct Data
	{
		long offset;
		char* pData;
		long size;

		char* pDataHead;
		Data* pSamePrevious = nullptr;
		Data* pSameNext = nullptr;

        Data() = delete;
		Data(long _size, char* _pData, char* _pDataHead,
				long _offset = 0, Data* _pSamePrevious = nullptr) noexcept;

		Data(const Data&) = delete;
		ZC_Buffer::Data& operator = (const Data&) = delete;

		Data(Data&& vboData) noexcept;
		ZC_Buffer::Data& operator = (Data&& vboData) noexcept;

		~Data() noexcept;
	};

	std::list<Data> datas;
	GLenum usage = 0;

	void ClearDatas() noexcept;
    void AddData(long offset, long size, char* pData);
	long Size() noexcept;

    void Reload(GLuint _id);
#endif
};

template<typename TpHead>
void ZC_Buffer::FillTriangleElements(TpHead pElementsHead, size_t elementsSize, size_t quadsElementsCount)
{
	size_t elementsI = 0,
		index = 0;
	for ( ; elementsI < quadsElementsCount; elementsI += 6)
	{
		pElementsHead[elementsI] = pElementsHead[elementsI + 3] = index++;
		pElementsHead[elementsI + 1] = pElementsHead[elementsI + 5] = index++;
		pElementsHead[elementsI + 2] = index++;
		pElementsHead[elementsI + 4] = index++;
	}
	while (elementsI < elementsSize)
	{
		pElementsHead[elementsI++] = index++;
		pElementsHead[elementsI++] = index++;
		pElementsHead[elementsI++] = index++;
	}
}