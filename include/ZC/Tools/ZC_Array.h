// #pragma once

// #include <ZC/Tools/Exception/ZC_Exception.h>

// namespace ZC_ArrayMethods
// {
//     template<typename TValue>
//     TValue& OperatorGet(const unsigned long& index, const unsigned long& currentSize, TValue* const& pArray)
//     {
//         if (index >= currentSize) throw ZC_Exception(ZC_MakeErrorString("Call ZC_Array[index] out of range!", __FILE__, __LINE__));
//         return *(pArray + index);
//     }

//     template<typename TValue>
//     bool EmplaceBack(TValue& value, const unsigned long& size, unsigned long& currentSize, TValue* pArray) noexcept
//     {
//         if (size == currentSize) return false;
//         *(pArray + currentSize++) = std::move(value);
//         return true;
//     }

//     template<typename TValue>
//     bool Erase(unsigned long& index, unsigned long& currentSize, TValue* const& pArray) noexcept
//     {
//         if (index >= currentSize) return false;
//         TValue* pArrayIndex = pArray + index;
//         for (; index < currentSize; ++index)
//         {
//             if (index == currentSize - 1) break;
//             *pArrayIndex++ = std::move(*(pArrayIndex + 1));
//         }
//         --currentSize;
//         return true;
//     }
// };

// struct ZC_Stack;
// struct ZC_Heap;

// // #include <concepts>
// // template<typename T>
// // concept ZC_cMemoryType = std::same_as<T, ZC_Stack> || std::same_as<T, ZC_Heap>;

// // template<typename TValue, ZC_cMemoryType TMemoryType, unsigned long Size>
// template<typename TValue, typename TMemoryType, unsigned long TSize>
// class ZC_Array;

// template<typename TValue, unsigned long TSize>
// class ZC_Array<TValue, ZC_Stack, TSize>
// {
// public:
//     ZC_Array() noexcept;

//     TValue& operator [] (const unsigned long& index) noexcept;

//     bool EmplaceBack(TValue&& value) noexcept;
//     bool Erase(unsigned long index) noexcept;
//     ulong Size() const noexcept;

// private:
//     TValue array[TSize];
//     unsigned long currentSize = 0;
// };

// template<typename TValue, unsigned long TSize>
// class ZC_Array<TValue, ZC_Heap, TSize>
// {
// public:
//     ZC_Array() noexcept;
//     ~ZC_Array() noexcept;

//     TValue& operator [] (const unsigned long& index) noexcept;

//     bool EmplaceBack(TValue&& value) noexcept;
//     bool Erase(unsigned long index) noexcept;
//     ulong Size() const noexcept;

// private:
//     TValue* pValue;
//     unsigned long currentSize = 0;
// };

// //  ZC_Stack start

// template<typename TValue, unsigned long TSize>
// ZC_Array<TValue, ZC_Stack, TSize>::ZC_Array() noexcept
// {}

// template<typename TValue, unsigned long TSize>
// TValue& ZC_Array<TValue, ZC_Stack, TSize>::operator [] (const unsigned long& index) noexcept
// {
//     return ZC_ArrayMethods::OperatorGet<TValue>(index, currentSize, array);
// }

// template<typename TValue, unsigned long TSize>
// bool ZC_Array<TValue, ZC_Stack, TSize>::EmplaceBack(TValue&& value) noexcept
// {
//     return ZC_ArrayMethods::EmplaceBack(value, TSize, currentSize, array);
// }

// template<typename TValue, unsigned long TSize>
// bool ZC_Array<TValue, ZC_Stack, TSize>::Erase(unsigned long index) noexcept
// {
//     return ZC_ArrayMethods::Erase<TValue>(index, currentSize, array);
// }

// template<typename TValue, unsigned long TSize>
// unsigned long ZC_Array<TValue, ZC_Stack, TSize>::Size() const noexcept
// {
//     return currentSize;
// }
// //  ZC_Stack end

// //  ZC_Heap start

// template<typename TValue, unsigned long TSize>
// ZC_Array<TValue, ZC_Heap, TSize>::ZC_Array() noexcept
//     : pValue(new TValue[TSize])
// {}

// template<typename TValue, unsigned long TSize>
// ZC_Array<TValue, ZC_Heap, TSize>::~ZC_Array() noexcept
// {
//     delete[] pValue;
// }

// template<typename TValue, unsigned long TSize>
// TValue& ZC_Array<TValue, ZC_Heap, TSize>::operator [] (const unsigned long& index) noexcept
// {
//     return ZC_ArrayMethods::OperatorGet<TValue>(index, currentSize, pValue);
// }

// template<typename TValue, unsigned long TSize>
// bool ZC_Array<TValue, ZC_Heap, TSize>::EmplaceBack(TValue&& value) noexcept
// {
//     return ZC_ArrayMethods::EmplaceBack(value, TSize, currentSize, pValue);
// }

// template<typename TValue, unsigned long TSize>
// bool ZC_Array<TValue, ZC_Heap, TSize>::Erase(unsigned long index) noexcept
// {
//     return ZC_ArrayMethods::Erase<TValue>(index, currentSize, pValue);
// }

// template<typename TValue, unsigned long TSize>
// unsigned long ZC_Array<TValue, ZC_Heap, TSize>::Size() const noexcept
// {
//     return currentSize;
// }
// //  ZC_Heap end