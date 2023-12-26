#pragma once

#include <ZC/ErrorLogger/ZC_MakeErrorString.h>

#include <exception>

class ZC_Exception : public std::exception
{
public:
    ZC_Exception(const std::string& _msg);
    ~ZC_Exception() override = default;

    const char* what() const noexcept override;

private:
    std::string message;
};