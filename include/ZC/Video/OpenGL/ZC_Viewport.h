#pragma once

class ZC_Viewport
{
public:
    //  Create a viewport with window size.
    static ZC_Viewport CreateStandardWindowViewport();
    
    ZC_Viewport(int _startX, int _startY, int _width, int _height);

    void Use();
    void SetSize(int _width, int _height);
    void GetSize(int& rWidth, int& rHeight) const noexcept;
    int GetWidth() const noexcept;
    int GetHeight() const noexcept;

private:
    static inline ZC_Viewport* pActiveViewport = nullptr;

    int startX,
        startY,
        width,
        height;
};