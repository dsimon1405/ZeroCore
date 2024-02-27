#pragma once

#include <ZC/Tools/Exception/ZC_Exception.h>

#include <cstdlib>

//  Wrapper for dynamic array with open data.
template<typename T>
struct ZC_DA
{
    T* pHead = nullptr;
    size_t size = 0;

    ZC_DA(T* _pArray = nullptr, size_t _size = 0);
    ZC_DA(size_t _size);

    ZC_DA(const ZC_DA&) = delete;
    ZC_DA& operator = (const ZC_DA&) = delete;

    ZC_DA(ZC_DA&& charArray) noexcept;
    ZC_DA& operator = (ZC_DA&& charArray);

    ~ZC_DA();

    T& operator [] (size_t index);
    const T& operator [] (size_t index) const;

    unsigned long BytesSize() noexcept;
    T* Begin() noexcept;
    const T* Begin() const noexcept;
    T* End() noexcept;
    const T* End() const noexcept;

    ZC_DA<T> GetCopy() const;
};

template<typename T>
ZC_DA<T>::ZC_DA(T* _pArray, size_t _size)
    : pHead(_pArray),
    size(_size)
{}

template<typename T>
ZC_DA<T>::ZC_DA(size_t _size)
    : pHead(static_cast<T*>(malloc(sizeof(T) * _size))),
    size(_size)
{}

template<typename T>
ZC_DA<T>::ZC_DA(ZC_DA&& charArray) noexcept
    : pHead(charArray.pHead),
    size(charArray.size)
{
    charArray.pHead = nullptr;
    charArray.size = 0;
}

template<typename T>
ZC_DA<T>& ZC_DA<T>::operator = (ZC_DA&& charArray)
{
    if(pHead) delete[] pHead;
    pHead = charArray.pHead;
    size = charArray.size;

    charArray.pHead = nullptr;
    charArray.size = 0;
    return *this;
}

template<typename T>
ZC_DA<T>::~ZC_DA()
{
    delete[] pHead;
    pHead = nullptr;
}

template<typename T>
T& ZC_DA<T>::operator [] (size_t index)
{
    if (index >= size) ZC_Exception("Index out of range!");
    return pHead[index];
}

template<typename T>
const T& ZC_DA<T>::operator [] (size_t index) const
{
    if (index >= size) ZC_Exception("Index out of range!");
    return const_cast<T&>(pHead[index]);
}

template<typename T>
unsigned long ZC_DA<T>::BytesSize() noexcept
{
    return size * sizeof(T);
}

template<typename T>
T* ZC_DA<T>::Begin() noexcept
{
    return pHead;
}

template<typename T>
const T* ZC_DA<T>::Begin() const noexcept
{
    return pHead;
}

template<typename T>
T* ZC_DA<T>::End() noexcept
{
    return pHead + size;
}

template<typename T>
const T* ZC_DA<T>::End() const noexcept
{
    return pHead + size;
}

template<typename T>
ZC_DA<T> ZC_DA<T>::GetCopy() const
{
    ZC_DA<T> copy(size);
    for (size_t i = 0; i < size; ++i) copy[i] = pHead[i]; 
    return copy;
}