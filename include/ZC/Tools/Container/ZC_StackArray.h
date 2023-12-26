#pragma once

template<typename T>
struct ZC_StackArray
{
    T* pHead;
    unsigned long size;

    ZC_StackArray(T* pHead, unsigned long size) noexcept;

    static ZC_StackArray<char> GetEmpty(unsigned long bytesSize) noexcept;

    unsigned long BytesSize() noexcept;
};

template<typename T>
ZC_StackArray<T>::ZC_StackArray(T* _pHead, unsigned long _size) noexcept
    : pHead(_pHead),
    size(_size)
{}

template<typename T>
unsigned long ZC_StackArray<T>::BytesSize() noexcept
{
    return size * sizeof(T);
}