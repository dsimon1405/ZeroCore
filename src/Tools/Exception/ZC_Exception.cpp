#include <ZC/Tools/Exception/ZC_Exception.h>

ZC_Exception::ZC_Exception(const std::string& _msg)
    : message(_msg)
{}

const char* ZC_Exception::what() const noexcept
{
    return message.c_str();
}