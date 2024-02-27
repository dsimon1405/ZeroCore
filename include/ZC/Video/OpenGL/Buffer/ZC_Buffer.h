#pragma once

#include <ZC_Config.h>
#include <ZC/Video/OpenGL/GL/glcorearb.h>
#include <ZC/Tools/Math/ZC_Limits.h>
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

    static void GetElementsData(size_t maxElementsIndex, size_t& storingTypeSize, GLenum& rElementsType)
    {
        if (maxElementsIndex <= ZC_UCHAR_MAX)
        {
            storingTypeSize = sizeof(uchar);
            rElementsType = GL_UNSIGNED_BYTE;
        }
        else if (maxElementsIndex <= ZC_USHRT_MAX)
        {
            storingTypeSize = sizeof(ushort);
            rElementsType = GL_UNSIGNED_SHORT;
        }
        else
        {
            storingTypeSize = sizeof(uint);
            rElementsType = GL_UNSIGNED_INT;
        }
    }

	static constexpr int Pack_INT_2_10_10_10_REV(float x, float y, float z)
	{
		//  pack float in signed byte array[10]:
		//  array[0] - sign (0 is pluss, 1 is minus);
		//  array[1 - 9] - number;
		//  512(min), 511(max) signed byte[9] values.
		auto packIn10Bytes = [](float val) -> int
		{
			return  val < 0 ?
			512 | static_cast<int>(ZC_ROUND(512.f + val * 512.f))
			: static_cast<int>(ZC_ROUND(val * 511.f));
		};
		return ((packIn10Bytes(z) << 20) | (packIn10Bytes(y) << 10)) | packIn10Bytes(x);
	};

	GLuint id = 0;
	GLenum type;

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