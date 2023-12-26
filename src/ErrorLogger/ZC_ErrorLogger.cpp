#include <ZC/ErrorLogger/ZC_ErrorLogger.h>

// constexpr bool ZC_ErrorLogger::WasError() noexcept
// {
//     return !errorMessage.empty();
// }

const char* ZC_ErrorLogger::ErrorMessage() noexcept
{
    return errorMessage.empty() ? nullptr : errorMessage.c_str();
}

void ZC_ErrorLogger::Clear() noexcept
{
    if (!errorMessage.empty()) errorMessage = "";
}