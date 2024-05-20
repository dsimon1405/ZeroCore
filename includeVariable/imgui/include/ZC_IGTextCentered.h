#pragma once

#include <ZC/Tools/ZC_string.h>

/*
Class for drawing text with window horizontal center alignment.
How to use:
- If ImGui window not resizable, ZC_IGTextCentered object can be created in static variable, into method that draw ImGui window (indext will calculated once).
- If window resizable:  object can be recreated, in not static vaiable, in each frame, into method that draw ImGui window; or still create in static variable and call
    RecalculateIndent() when size is changed.
*/
class ZC_IGTextCentered
{
public:
    ZC_IGTextCentered(std::string&& _text);

    //  recalculate text indent (use after ImGui window resize)
    void RecalculateIndent();
    //  reset text
    void SetText(std::string&& _text);
    //  draws text into window's center
    void Draw();

private:
    std::string text;
    float indentX;
};