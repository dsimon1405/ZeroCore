#include "ZC_ArrayConverter.h"

namespace ZC_ArrayConverter
{
    ZC_StackArray<signed char> Convert(unsigned long bytesSize) noexcept
    {
        return ZC_StackArray<signed char>(nullptr, bytesSize);
    }
}