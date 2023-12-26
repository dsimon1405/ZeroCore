#pragma once

//  Wrapper for dynamic array with open data.
template<typename T>
struct ZC_DynamicArray
{
    T* pHead = nullptr;
    unsigned long size = 0;

    ZC_DynamicArray(T* _pArray = nullptr, unsigned long _size = 0) noexcept;
    ZC_DynamicArray(unsigned long _size) noexcept;

    ZC_DynamicArray(const ZC_DynamicArray&) = delete;
    ZC_DynamicArray& operator = (const ZC_DynamicArray&) = delete;

    ZC_DynamicArray(ZC_DynamicArray&& charArray) noexcept;
    ZC_DynamicArray& operator = (ZC_DynamicArray&& charArray);

    ~ZC_DynamicArray();

    T& operator [] (unsigned long index);
    const T& operator [] (unsigned long index) const;

    unsigned long BytesSize() noexcept;
    const T* Begin() const noexcept;
};

template<typename T>
ZC_DynamicArray<T>::ZC_DynamicArray(T* _pArray, unsigned long _size) noexcept
    : pHead(_pArray),
    size(_size)
{}

template<typename T>
ZC_DynamicArray<T>::ZC_DynamicArray(unsigned long _size) noexcept
        : pHead(new T[_size]),
        size(_size)
{}

template<typename T>
ZC_DynamicArray<T>::ZC_DynamicArray(ZC_DynamicArray&& charArray) noexcept
        : pHead(charArray.pHead),
        size(charArray.size)
{
    charArray.pHead = nullptr;
}

template<typename T>
ZC_DynamicArray<T>& ZC_DynamicArray<T>::operator = (ZC_DynamicArray&& charArray)
{
    delete[] pHead;
    pHead = charArray.pHead;
    size = charArray.size;

    charArray.pHead = nullptr;
    charArray.size = 0;
    return *this;
}

template<typename T>
ZC_DynamicArray<T>::~ZC_DynamicArray()
{
    delete[] pHead;
    pHead = nullptr;
}

template<typename T>
T& ZC_DynamicArray<T>::operator [] (unsigned long index)
{
    return pHead[index];
}

template<typename T>
const T& ZC_DynamicArray<T>::operator [] (unsigned long index) const
{
    return const_cast<T&>(pHead[index]);
}

template<typename T>
unsigned long ZC_DynamicArray<T>::BytesSize() noexcept
{
    return size * sizeof(T);
}

template<typename T>
const T* ZC_DynamicArray<T>::Begin() const noexcept
{
    return pHead;
}