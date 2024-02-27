#pragma once

#include <ZC/Tools/ZC_string.h>
#include <ZC/Video/OpenGL/Renderer/ZC_RendererSet.h>
#include <ZC/Tools/Signal/ZC_SConnection.h>

#include <forward_list>

class ZC_IGWindow : protected ZC_RendererSet
{
public:
    ~ZC_IGWindow();

    /*
    Allows to draw window or not.

    Params:
    _needDraw - if true window will draw until user don't close it (using X on TitleBar if it exists), or calls this function with false.
    */
    void NeedDraw(bool _needDraw);

    static bool IsCursorInOneOfWindows() noexcept;
    //  set isCursorInOneOfWindows in false
    static void PollEventEnds() noexcept;

protected:
    enum WindowIndentFlags
    {
        XLeft_YTop          = 0,
        X_Right             = 1,
        Y_Bottom            = 1 << 1,
        Center              = 1 << 2,
        XRight_YBottom      = X_Right | Y_Bottom
    };

    ZC_IGWindow(std::string&& unicName, bool _needDraw, float _width, float _height,
        float _indentX, float _indentY, WindowIndentFlags _wif, bool _mayClose, int _igwf);

private:
    static inline std::forward_list<std::string> unicNames;

    const char* name;
    bool isDrawing;
    float width,
        height,
        indentX,
        indentY;
    WindowIndentFlags wif;
    ZC_SConnection sconZC_WindowResized;
    bool mayClose;
    int igwf;
    bool needSetPosition = true;
    static inline bool isCursorInOneOfWindows = false;

    void Draw(Level lvl) override;

    virtual void DrawWindow() = 0;

    const char* AddName(std::string&& unicName);
    void ZC_WindowResized(float width, float height);
    void SetPosition();
};