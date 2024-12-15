#pragma once

#include <compare>
#include <utility>
#include <ZC/ZC_Types.h>

template<typename T>
class ZC_sptr
{
public:
    ZC_sptr(T* _pData = nullptr);
    ZC_sptr(T* _pData, ul_zc* _pUseCount);
    
    ZC_sptr(const ZC_sptr<T>& sptr);
    ZC_sptr<T>& operator = (const ZC_sptr<T>& sptr);
    
    ZC_sptr(ZC_sptr<T>&& sptr);
    ZC_sptr<T>& operator = (ZC_sptr<T>&& sptr);

    ~ZC_sptr();

    T* operator -> () noexcept;
    const T* operator -> () const noexcept;

    T& operator * ();
    const T& operator * () const;

    operator bool () const noexcept;
    auto operator <=> (const ZC_sptr<T>& uptr) const noexcept;

    T* Get() noexcept;
    const T* Get() const noexcept;

    template<typename TParant>
    ZC_sptr<TParant> DynamicCast() const;
    ul_zc UseCount() const noexcept;

private:
    T* pData = nullptr;
    ul_zc* pUseCount = nullptr;

    void Delete();
};

template<typename T, typename... TParams>
ZC_sptr<T> ZC_sptrMake(TParams&&... params) noexcept
{
    return ZC_sptr<T>(new T(std::forward<TParams>(params)...));
}

template<typename T, typename TChild, typename... TParams>
ZC_sptr<T> ZC_sptrMakeFromChild(TParams&&... params) noexcept
{
    return ZC_sptr<T>(dynamic_cast<T*>(new TChild(std::forward<TParams>(params)...)));
}

template<typename T>
ZC_sptr<T>::ZC_sptr(T* _pData)
    : pData(_pData),
    pUseCount(pData ? new ul_zc(1) : nullptr)
{}

template<typename T>
ZC_sptr<T>::ZC_sptr(T* _pData, ul_zc* _pUseCount)
    : pData(_pData && _pUseCount ? _pData : nullptr),
    pUseCount(_pData && _pUseCount ? _pUseCount : nullptr)
{}
    
template<typename T>
ZC_sptr<T>::ZC_sptr(const ZC_sptr<T>& sptr)
    : pData(sptr.pData),
    pUseCount(pData ? &++*sptr.pUseCount : nullptr)
{}

template<typename T>
ZC_sptr<T>& ZC_sptr<T>::operator = (const ZC_sptr<T>& sptr)
{
    if (*this != sptr)
    {
        Delete();
        pUseCount = &++*sptr.pUseCount;
        pData = sptr.pData;
    }
    return *this;
}

template<typename T>
ZC_sptr<T>::ZC_sptr(ZC_sptr<T>&& sptr)
    : pData(sptr.pData),
    pUseCount(sptr.pUseCount)
{
    sptr.pData = nullptr;
    sptr.pUseCount = nullptr;
}

template<typename T>
ZC_sptr<T>& ZC_sptr<T>::operator = (ZC_sptr<T>&& sptr)
{
    if (this != &sptr)
    {
        Delete();
        pData = sptr.pData;
        pUseCount = sptr.pUseCount;

        sptr.pData = nullptr;
        sptr.pUseCount = nullptr;
    }
    return *this;
}

template<typename T>
ZC_sptr<T>::~ZC_sptr()
{
    Delete();
}

template<typename T>
T* ZC_sptr<T>::operator -> () noexcept
{
    return pData;
}

template<typename T>
const T* ZC_sptr<T>::operator -> () const noexcept
{
    return static_cast<const T*>(pData);
}

template<typename T>
T& ZC_sptr<T>::operator * ()
{
    return *pData;
}

template<typename T>
const T& ZC_sptr<T>::operator * () const
{
    return *pData;
}

template<typename T>
ZC_sptr<T>::operator bool () const noexcept
{
    return pData != nullptr;
}

template<typename T>
auto ZC_sptr<T>::operator <=> (const ZC_sptr<T>& uptr) const noexcept
{
    return pData <=> uptr.pData;
}

template<typename T>
T* ZC_sptr<T>::Get() noexcept
{
    return pData;
}

template<typename T>
const T* ZC_sptr<T>::Get() const noexcept
{
    return static_cast<const T*>(pData);
}

template<typename T>
template<typename TParant>
ZC_sptr<TParant> ZC_sptr<T>::DynamicCast() const
{
    if (pData) return ZC_sptr<TParant>(dynamic_cast<TParant*>(pData), &++*pUseCount);
    return nullptr;
}

template<typename T>
ul_zc ZC_sptr<T>::UseCount() const noexcept
{
    return pUseCount ? *pUseCount : 0;
}

template<typename T>
void ZC_sptr<T>::Delete()
{
    if (!pUseCount) return;
    if (--*pUseCount == 0)
    {
        delete pData;
        delete pUseCount;
    }
}