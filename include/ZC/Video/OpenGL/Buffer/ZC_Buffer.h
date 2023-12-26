#pragma once

#include <ZC_Config.h>
#include <ZC/Video/OpenGL/GL/glcorearb.h>
#include <ZC/Tools/Container/ZC_DynamicArray.h>
#include <ZC/Tools/Container/ZC_StackArray.h>
#ifdef ZC_ANDROID
#include <list>
#endif

#include <concepts>

template <typename T>
concept ZC_cOpenGLType = std::same_as<T, GLbyte>
					  || std::same_as<T, GLubyte>
					  || std::same_as<T, GLshort>
					  || std::same_as<T, GLushort>
					  || std::same_as<T, GLhalf>
					  || std::same_as<T, GLint>
					  || std::same_as<T, GLuint>
					  || std::same_as<T, GLfloat>;


template <typename TCont>
concept ZC_cOpenGLContainer = std::same_as<TCont, ZC_DynamicArray<GLbyte>>
						   || std::same_as<TCont, ZC_DynamicArray<GLubyte>>
						   || std::same_as<TCont, ZC_DynamicArray<GLshort>>
						   || std::same_as<TCont, ZC_DynamicArray<GLushort>>
						   || std::same_as<TCont, ZC_DynamicArray<GLhalf>>
						   || std::same_as<TCont, ZC_DynamicArray<GLint>>
						   || std::same_as<TCont, ZC_DynamicArray<GLuint>>
						   || std::same_as<TCont, ZC_DynamicArray<GLfloat>>
						   || std::same_as<TCont, ZC_StackArray<GLbyte>>
						   || std::same_as<TCont, ZC_StackArray<GLubyte>>
						   || std::same_as<TCont, ZC_StackArray<GLshort>>
						   || std::same_as<TCont, ZC_StackArray<GLushort>>
						   || std::same_as<TCont, ZC_StackArray<GLhalf>>
						   || std::same_as<TCont, ZC_StackArray<GLint>>
						   || std::same_as<TCont, ZC_StackArray<GLuint>>
						   || std::same_as<TCont, ZC_StackArray<GLfloat>>;

//	Wrapper OpenGL buffer.
class ZC_Buffer
{
public:

	ZC_Buffer(const ZC_Buffer&) = delete;
	ZC_Buffer& operator = (const ZC_Buffer&) = delete;

	ZC_Buffer(ZC_Buffer&& vbo) noexcept;
	ZC_Buffer& operator = (ZC_Buffer&& vbo);

	virtual ~ZC_Buffer();

	/*
	Saves data to a buffer, or reserve place.

	Params:
	data - data to save (if nullptr, reserve place).
	_usage - style of using stored data (GL_STREAM_DRAW, GL_STREAM_READ, GL_STREAM_COPY, GL_STATIC_DRAW, GL_STATIC_READ, GL_STATIC_COPY, GL_DYNAMIC_DRAW, GL_DYNAMIC_READ, or GL_DYNAMIC_COPY).
	
	Return:
	On success true, otherwise false (in second case ZC_ErrorLogger::ErrorMessage() - for more information).
	*/
	template<ZC_cOpenGLContainer TOpenGLCont>
	bool BufferData(TOpenGLCont&& data, GLenum _usage);

	/*
	Save new data in buffer.

	Params:
	offset - offset in bytes before editing starts.
	data - data to save.
	
	Return:
	On success true, otherwise false (in second case ZC_ErrorLogger::ErrorMessage() - for more information).
	*/
	template<ZC_cOpenGLContainer TOpenGLCont>
	bool BufferSubData(long offset, TOpenGLCont&& data);

protected:
	GLuint id;
	GLenum type;

	ZC_Buffer(GLenum _type);

	bool BuffData(long bytesSize, void* pData, GLenum _usage);
	bool BuffSubData(long offset, long bytesSize, void* pData);

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

template<ZC_cOpenGLContainer TOpenGLCont>
bool ZC_Buffer::BufferData(TOpenGLCont&& data, GLenum _usage)
{
	if (!BuffData(static_cast<long>(data.BytesSize()), static_cast<void*>(data.pHead), _usage)) return false;
#ifdef ZC_ANDROID
	data.pHead = nullptr;
#endif
	return true;
}

template<ZC_cOpenGLContainer TOpenGLCont>
bool ZC_Buffer::BufferSubData(long offset, TOpenGLCont&& data)
{
	if (!BuffSubData(offset, static_cast<long>(data.BytesSize()), static_cast<void*>(data.pHead))) return false;
#ifdef ZC_ANDROID
	data.pHead = nullptr;
#endif
	return true;
}