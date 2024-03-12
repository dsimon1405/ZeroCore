#include <ZC_IGWindow.h>

#include <ZC/Tools/Container/ZC_ContFunc.h>
#include "ZC_ImGui.h"
#include <ZC/Events/ZC_Events.h>

#include <imgui.h>
#include <algorithm>

ZC_IGWindow::~ZC_IGWindow()
{
    ZC_ForwardListErase(unicNames, name);
    if (isDrawing) EraseFromRenderer(Level::ImGui);
    // sconZC_WindowResized.Disconnect();
}

void ZC_IGWindow::NeedDraw(bool _needDraw)
{
    if (_needDraw == isDrawing) return;
    isDrawing = !isDrawing;
    sconChangeDrawingState = ZC_Events::ConnectHandleEventsEnd({ &ZC_IGWindow::ChangeDrawingState, this });
}

bool ZC_IGWindow::IsCursorInOneOfWindows() noexcept
{
    return isCursorInOneOfWindows;
}

void ZC_IGWindow::Make_isCursorInOneOfWindows_false(float time) noexcept
{
    isCursorInOneOfWindows = false;
}

ZC_IGWindow::ZC_IGWindow(std::string&& unicName, bool needDraw, float _width, float _height,
        float _indentX, float _indentY, ZC_WindowOrthoIndentFlags _indentFlags, bool _mayClose, ImGuiWindowFlags _igwf)
    : ZC_WindowOrthoIndent(true, _width, _height, _indentX, _indentY, _indentFlags, { &ZC_IGWindow::ZC_WindowResized, this }),
    name(AddName(std::move(unicName))),
    isDrawing(needDraw),
    // width(_width),
    // height(_height),
    // indentX(_indentX),
    // indentY(_indentY),
    // sconZC_WindowResized(SetIndentData(_indentX, _indentY, _indentFlags)),
    mayClose(_mayClose),
    igwf(_igwf)
{
    this->CalculateCurrentIndents();
    ZC_ImGui::FrameStart();
    SetPosition();
    ImGui::SetNextWindowSize(ImVec2(_width, _height));
    ImGui::Begin(name, NULL);
    ImGui::End();
    ZC_ImGui::FrameEnd();

    if (isDrawing) AddToRenderer(Level::ImGui);
}

void ZC_IGWindow::Draw(Level lvl)
{
    SetPosition();
    if (mayClose)
    {
        bool needDrawInNextFrame = true;
        if (ImGui::Begin(name, &needDrawInNextFrame, igwf))
        {
            if (ImGui::IsWindowHovered()) isCursorInOneOfWindows = true;
            DrawWindow();
        }
        if (!needDrawInNextFrame && isDrawing) NeedDraw(needDrawInNextFrame);
    }
    else
    {
        ImGui::Begin(name, NULL, igwf);
        if (ImGui::IsWindowHovered()) isCursorInOneOfWindows = true;
        DrawWindow();
    }
    ImGui::End();
}

const char* ZC_IGWindow::AddName(std::string&& unicName)
{
    assert(std::find(unicNames.begin(), unicNames.end(), unicName) == unicNames.end());
    return unicNames.emplace_front(std::move(unicName)).c_str();
}

// ZC_SConnection ZC_IGWindow::SetIndentData(float _indentX, float _indentY, ZC_IGWIndentFlags _indentFlags)
// {
//     int _indentFlagX = 0;   //  find flag for X
//     if (_indentFlags & X_Left_Pixel) _indentFlagX = X_Left_Pixel;
//     else if (_indentFlags & X_Left_Percent) _indentFlagX = _indentX >= 0.f && _indentX <= 1.f ? X_Left_Percent : 0; //  indent for percent must be in diaposon [0,1]
//     else if (_indentFlags & X_Right_Pixel) _indentFlagX = X_Right_Pixel;
//     else if (_indentFlags & X_Right_Percent) _indentFlagX = _indentX >= 0.f && _indentX <= 1.f ? X_Right_Percent : 0; //  indent for percent must be in diaposon [0,1]
//     else if (_indentFlags & X_Center) _indentFlagX = X_Center;
//     assert(_indentFlagX != 0);  //  no valid data

//     int _indentFlagY = 0;
//     if (_indentFlags & Y_Top_Pixel) _indentFlagY = Y_Top_Pixel;
//     else if (_indentFlags & Y_Top_Percent) _indentFlagY = _indentY >= 0.f && _indentY <= 1.f ? Y_Top_Percent : 0;
//     else if (_indentFlags & Y_Bottom_Pixel) _indentFlagY = Y_Bottom_Pixel;
//     else if (_indentFlags & Y_Bottom_Percent) _indentFlagY = _indentY >= 0.f && _indentY <= 1.f ? Y_Bottom_Percent : 0;
//     else if (_indentFlags & Y_Center) _indentFlagY = Y_Center;
//     assert(_indentFlagY != 0);

//     indentFlags = static_cast<IndentFlag>(_indentFlagX | _indentFlagY);

//     assert((_indentFlags - indentFlags) == 0);    //  flags have more than one X or more than one Y

//     return ZC_Events::ConnectWindowResize({ &ZC_IGWindow::ZC_WindowResized, this });
// }

void ZC_IGWindow::ZC_WindowResized()
{
    needSetPosition = true;
}

void ZC_IGWindow::SetPosition()
{
    if (!needSetPosition) return;

    // static ImGuiIO& io = ImGui::GetIO();

    // float setIndentX = 0.f;
    // switch (indentFlags & 0x1F) //  remove Y flag (0x1F -> ...00011111)
    // {
    // case X_Left_Pixel: setIndentX = indentX; break;
    // case X_Left_Percent: setIndentX = io.DisplaySize.x * indentX; break;
    // case X_Right_Pixel: setIndentX = io.DisplaySize.x - indentX - width; break;
    // case X_Right_Percent: setIndentX = io.DisplaySize.x - (io.DisplaySize.x * indentX) - width; break;
    // case X_Center: setIndentX = (io.DisplaySize.x / 2.f) - (width / 2.f); break;
    // default: break;
    // }

    // float setIndentY = 0.f;
    // switch (indentFlags >> 5 << 5)   //  remove X flag
    // {
    // case Y_Top_Pixel: setIndentY = indentY; break;
    // case Y_Top_Percent: setIndentY = io.DisplaySize.y * indentY; break;
    // case Y_Bottom_Pixel: setIndentY = io.DisplaySize.y - indentY - height; break;
    // case Y_Bottom_Percent: setIndentY = io.DisplaySize.y - (io.DisplaySize.y * indentY) - height; break;
    // case Y_Center: setIndentY = (io.DisplaySize.y / 2.f) - (height / 2.f); break;
    // default: break;
    // }

    ImGui::SetNextWindowPos({ currentIndentX, currentIndentY });
    needSetPosition = false;
}

void ZC_IGWindow::ChangeDrawingState(float time)
{
    if (isDrawing) AddToRenderer(Level::ImGui);
    else EraseFromRenderer(Level::ImGui);
    sconChangeDrawingState.Disconnect();
}