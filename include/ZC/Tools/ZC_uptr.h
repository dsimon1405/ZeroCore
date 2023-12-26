#pragma once

#include <compare>

template<typename T>
class ZC_uptr
{
public:
    ZC_uptr(T* _type = nullptr) noexcept;

    ZC_uptr(const ZC_uptr<T>&) = delete;
    ZC_uptr<T>& operator = (const ZC_uptr<T>&) = delete;

    ZC_uptr(ZC_uptr<T>&& uptr) noexcept;
    ZC_uptr<T>& operator = (ZC_uptr<T>&& uptr) noexcept;

    ~ZC_uptr() noexcept;

    T* operator -> () noexcept;
    const T* operator -> () const noexcept;

    T& operator * () noexcept;
    const T& operator * () const noexcept;
    
    operator bool () const noexcept;
    auto operator <=> (const ZC_uptr<T>& uptr) const noexcept = default;

    T* Get() noexcept;
    const T* Get() const noexcept;

private:
    T* pData = nullptr;
};

template<typename T, typename... TParams>
ZC_uptr<T> ZC_uptrMake(TParams&&... params) noexcept
{
    return ZC_uptr<T>(new T(std::forward<TParams>(params)...));
}

template<typename T, typename TChild, typename... TParams>
ZC_uptr<T> ZC_uptrMakeFromChild(TParams&&... params) noexcept
{
    return ZC_uptr<T>(dynamic_cast<T*>(new TChild(std::forward<TParams>(params)...)));
}

template<typename T>
ZC_uptr<T>::ZC_uptr(T* _type) noexcept
    : pData(_type)
{}

template<typename T>
ZC_uptr<T>::ZC_uptr(ZC_uptr<T>&& uptr) noexcept
    : pData(uptr.pData)
{
    uptr.pData = nullptr;
}

template<typename T>
ZC_uptr<T>& ZC_uptr<T>::operator = (ZC_uptr<T>&& uptr) noexcept
{
    if(this != &uptr)
    {
        if(pData) delete pData;
        pData = uptr.pData;
        uptr.pData = nullptr;
    }
    return *this;
}

template<typename T>
ZC_uptr<T>::~ZC_uptr() noexcept
{
    delete pData;
}

template<typename T>
T* ZC_uptr<T>::operator -> () noexcept
{
    return pData;
}

template<typename T>
const T* ZC_uptr<T>::operator -> () const noexcept
{
    return const_cast<const T*>(pData);
}

template<typename T>
T& ZC_uptr<T>::operator * () noexcept
{
    return *pData;
}

template<typename T>
const T& ZC_uptr<T>::operator * () const noexcept
{
    return const_cast<const T&>(pData);
}

template<typename T>
ZC_uptr<T>::operator bool () const noexcept
{
    return pData != nullptr;
}

template<typename T>
T* ZC_uptr<T>::Get() noexcept
{
    return pData;
}

template<typename T>
const T* ZC_uptr<T>::Get() const noexcept
{
    return static_cast<const T*>(pData);
}