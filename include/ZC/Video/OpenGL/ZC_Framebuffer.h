#pragma once

struct ZC_Framebuffer
{
    //  _target - must be GL_FRAMEBUFFER
    ZC_Framebuffer(bool create);

    ZC_Framebuffer(ZC_Framebuffer&& fb);
    ZC_Framebuffer& operator = (ZC_Framebuffer&& fb);

    ~ZC_Framebuffer();

    //  posible targets GL_DRAW_FRAMEBUFFER, GL_DRAW_FRAMEBUFFER, GL_FRAMEBUFFER
    void Bind(unsigned int targer) const;
    static void Unbind();
    void BlitTo(const ZC_Framebuffer& fbToDraw, int srcX0, int srcY0, int srcX1, int srcY1, int dstX0, int dstY0, int dstX1, int dstY1, unsigned int mask);
    unsigned int GetId() const noexcept;

private:
    unsigned int id = 0;
};