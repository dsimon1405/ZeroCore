#include <ZC/Video/OpenGL/Buffer/ZC_Buffer.h>

#include <Video/OpenGL/ZC_OpenGL.h>
#include <ZC/ErrorLogger/ZC_ErrorLogger.h>

ZC_Buffer::~ZC_Buffer()
{
    glDeleteBuffers(1, &id);
}

ZC_Buffer::ZC_Buffer(GLenum _type)
    : type(_type)
{
    glGenBuffers(1, &id);
}

bool ZC_Buffer::BuffData(long bytesSize, void* pData, GLenum _usage)
{
    ZC_ErrorLogger::Clear();
    glBindBuffer(type, id);
    glBufferData(type, bytesSize, const_cast<const void*>(pData), _usage);
    glBindBuffer(type, 0);
    if (ZC_ErrorLogger::WasError()) return false;
#ifdef ZC_ANDROID
    usage = _usage;
    ClearDatas();
    char* pDataChar = reinterpret_cast<char*>(pData);
    datas.emplace_back(bytesSize, pDataChar, pDataChar);
#endif
    return true;
}

bool ZC_Buffer::BuffSubData(long offset, long bytesSize, void* pData)
{
    ZC_ErrorLogger::Clear();
    glBindBuffer(type, id);
    glBufferSubData(type, offset, bytesSize, const_cast<const void*>(pData));
    glBindBuffer(type, 0);
    if (ZC_ErrorLogger::WasError()) return false;
#ifdef ZC_ANDROID
    AddData(offset, bytesSize, reinterpret_cast<char *>(pData));
#endif
    return true;
}
#ifdef ZC_PC
ZC_Buffer::ZC_Buffer(ZC_Buffer&& buf) noexcept
    : id(buf.id),
    type(buf.type)
{
    buf.id = 0;
}

ZC_Buffer& ZC_Buffer::operator = (ZC_Buffer&& buf)
{
    if (id != 0) glDeleteBuffers(1, &id);
    id = buf.id;
    type = buf.type;

    buf.id = 0;
    return *this;
}
#elif defined ZC_ANDROID
ZC_Buffer::ZC_Buffer(ZC_Buffer&& buf) noexcept
    : id(buf.id),
      type(buf.type),
      datas(std::move(buf.datas)),
      usage(buf.usage)
{
    buf.id = 0;
}

ZC_Buffer& ZC_Buffer::operator = (ZC_Buffer&& buf)
{
    if (id != 0) glDeleteBuffers(1, &id);
    id = buf.id;
    type = buf.type;
    datas = std::move(buf.datas);
    usage = buf.usage;

    buf.id = 0;
    return *this;
}

void ZC_Buffer::ClearDatas() noexcept
{
    for (auto& vboData : datas)
    {
        vboData.size = 0;
    }
    datas.clear();
}

void ZC_Buffer::AddData(long offset, long size, char* pData)
{
    auto emplacePosition = datas.end();
    long endPos = offset + size;
    for (auto vboDatasIter = datas.begin()->pData ? datas.begin() : ++datas.begin(); vboDatasIter != datas.end(); ++vboDatasIter)
    {
        long endPosIter = vboDatasIter->offset + vboDatasIter->size;
        if (offset < vboDatasIter->offset && endPos >= endPosIter)
        {
            vboDatasIter->size = 0;
            continue;
        }
        if (offset >= vboDatasIter->offset && offset < endPosIter)
        {
            vboDatasIter->size = offset - vboDatasIter->offset;
            if (endPos < endPosIter)
            {
                long startPosOffset = size + vboDatasIter->size;
                vboDatasIter = datas.emplace(++vboDatasIter,
                                             Data(endPosIter - endPos,
                                                         vboDatasIter->pData + startPosOffset,
                                                         vboDatasIter->pDataHead,
                                                         vboDatasIter->offset + startPosOffset,
                                                         &*vboDatasIter));
                emplacePosition = vboDatasIter;
                (--vboDatasIter)->pSameNext = &*emplacePosition;
                break;
            }
            if (endPos == endPosIter)
            {
                emplacePosition = ++vboDatasIter;
                break;
            }
        }
        if (endPos > vboDatasIter->offset && endPos <= endPosIter)
        {
            long startOffset = offset + size - vboDatasIter->offset;
            vboDatasIter->offset += startOffset;
            vboDatasIter->size -= startOffset;
            emplacePosition = vboDatasIter;
            break;
        }
    }
    datas.emplace(emplacePosition, Data(size, pData, pData, offset));
    for (auto vboDatasIter = datas.begin()->pData ? datas.begin() : ++datas.begin(); vboDatasIter != datas.end();)
    {
        vboDatasIter = vboDatasIter->size == 0 ? datas.erase(vboDatasIter) : ++vboDatasIter;
    }
}

long ZC_Buffer::Size() noexcept
{
    long result = 0;
    for (auto& data : datas) result += data.size;
    return result;
}

void ZC_Buffer::Reload(GLuint _id)
{
    id = _id;
    if (datas.empty()) return;
    glBindBuffer(type, id);
    auto vboDatasIter = datas.begin();
    glBufferData(type, vboDatasIter->size, vboDatasIter->pData, usage);
    for (++vboDatasIter; vboDatasIter != datas.end(); ++vboDatasIter)
    {
        glBufferSubData(type, vboDatasIter->offset, vboDatasIter->size, vboDatasIter->pData);
    }
}

//  Data
ZC_Buffer::Data::Data(long _size, char* _pData, char* _pDataHead,
                        long _offset, Data* _pSamePrevious) noexcept
    : offset(_offset),
      pData(_pData),
      size(_size),
      pDataHead(_pDataHead),
      pSamePrevious(_pSamePrevious)
{}

ZC_Buffer::Data::Data(Data&& vboData) noexcept
    : Data(vboData.size, vboData.pData, vboData.pDataHead, vboData.offset, vboData.pSamePrevious)
{
    pSameNext = vboData.pSameNext;

    vboData.pSameNext = nullptr;
    vboData.pSamePrevious = nullptr;
    vboData.pDataHead = nullptr;
}

ZC_Buffer::Data& ZC_Buffer::Data::operator = (Data&& vboData) noexcept
{
    size = vboData.size;
    pData = vboData.pData;
    pDataHead = vboData.pDataHead;
    offset = vboData.offset;
    pSamePrevious = vboData.pSamePrevious;
    pSameNext = vboData.pSameNext;

    vboData.pSameNext = nullptr;
    vboData.pSamePrevious = nullptr;
    vboData.pDataHead = nullptr;
    return *this;
}

ZC_Buffer::Data::~Data() noexcept
{
    if (pSameNext)
    {
        pSameNext->pSamePrevious = pSamePrevious;
        pDataHead = nullptr;
    }
    if (pSamePrevious)
    {
        pSamePrevious->pSameNext = pSameNext;
        pDataHead = nullptr;
    }

    delete[] pDataHead;
}
#endif