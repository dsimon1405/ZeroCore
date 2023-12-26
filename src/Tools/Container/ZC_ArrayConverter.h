#pragma once

#include <ZC_Config.h>
#include <ZC/Tools/Math/ZC_Mat4.h>
#include <ZC/Tools/Container/ZC_DynamicArray.h>
#include <ZC/Tools/Container/ZC_StackArray.h>

namespace ZC_ArrayConverter
{
    template<ZC_cVecTypes T>
    auto Convert(const ZC_Mat4<T>& mat)
    {
#ifdef ZC_PC
        return ZC_StackArray(const_cast<T*>(&mat[0][0]), 16);
#elif defined ZC_ANDROID
        ZC_DynamicArray<T> result(16);
        for (size_t i = 0, resultIndex = 0; i < 4; i++)
        {
            for (size_t j = 0; j < 4; j++)
            {
                projViewDynamic[resultIndex++] = projView[i][j];
            }
        }
        return result;
#endif
    }

    ZC_StackArray<signed char> Convert(unsigned long bytesSize) noexcept;
}