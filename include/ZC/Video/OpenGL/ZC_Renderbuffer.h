#pragma once

struct ZC_Renderbuffer
{
    ZC_Renderbuffer() = default;
    ZC_Renderbuffer(int samples, unsigned int internalFormat, int width, int height);
    ZC_Renderbuffer(unsigned int internalFormat, int width, int height);

    ZC_Renderbuffer(ZC_Renderbuffer&& rb);
    ZC_Renderbuffer& operator = (ZC_Renderbuffer&& rb);

    ~ZC_Renderbuffer();

    unsigned int GetId() const noexcept;

private:
    unsigned int id = 0;
};