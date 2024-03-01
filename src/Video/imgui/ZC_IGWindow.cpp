#include <ZC_IGWindow.h>

#include <ZC/Tools/Container/ZC_ContFunc.h>
#include "ZC_ImGui.h"
#include <ZC/Events/ZC_Events.h>

#include <imgui.h>

ZC_IGWindow::~ZC_IGWindow()
{
    ZC_ForwardListErase(unicNames, name);
    if (isDrawing) EraseFromRenderer(Level::ImGui);
    sconZC_WindowResized.Disconnect();
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

void ZC_IGWindow::Make_isCursorInOneOfWindows_false() noexcept
{
    isCursorInOneOfWindows = false;
}

ZC_IGWindow::ZC_IGWindow(std::string&& unicName, bool _needDraw, float _width, float _height,
        float _indentX, float _indentY, WindowIndentFlags _wif, bool _mayClose, ImGuiWindowFlags _igwf)
    : name(AddName(std::move(unicName))),
    isDrawing(_needDraw),
    width(_width),
    height(_height),
    indentX(_indentX),
    indentY(_indentY),
    wif(_wif),
    sconZC_WindowResized((wif & WindowIndentFlags::Center) || (wif & WindowIndentFlags::X_Right) || (wif & WindowIndentFlags::Y_Bottom) ?
        ZC_Events::ConnectWindowResize({ &ZC_IGWindow::ZC_WindowResized, this }) : ZC_SConnection()),
    mayClose(_mayClose),
    igwf(_igwf)
{
    ZC_ImGui::FrameStart();
    ImGui::Begin(name, NULL);

    ImGui::SetWindowSize(ImVec2(width, height));
    SetPosition();

    ImGui::End();
    ZC_ImGui::FrameEnd();

    if (isDrawing) AddToRenderer(Level::ImGui);
}

void ZC_IGWindow::Draw(Level lvl)
{
    if (mayClose)
    {
        bool needDrawInNextFrame = true;
        if (ImGui::Begin(name, &needDrawInNextFrame, igwf))
        {
            SetPosition();
            if (ImGui::IsWindowHovered()) isCursorInOneOfWindows = true;
            DrawWindow();
        }
        if (!needDrawInNextFrame && isDrawing) NeedDraw(needDrawInNextFrame);
    }
    else
    {
        ImGui::Begin(name, NULL, igwf);
        SetPosition();
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

void ZC_IGWindow::ZC_WindowResized(float width, float height)
{
    needSetPosition = true;
}

void ZC_IGWindow::SetPosition()
{
    if (!needSetPosition) return;

    static ImGuiIO& io = ImGui::GetIO();
    if (wif & WindowIndentFlags::Center)
    {
        indentX = (io.DisplaySize.x / 2.f) - (width / 2.f);
        indentY = (io.DisplaySize.y / 2.f) - (height / 2.f);
    }
    else
    {
        if (wif & WindowIndentFlags::X_Right) indentX = io.DisplaySize.x - indentX;
        if (wif & WindowIndentFlags::Y_Bottom) indentY = io.DisplaySize.y - indentY;
    }
    ImGui::SetWindowPos({indentX, indentY});
    needSetPosition = false;
}

void ZC_IGWindow::ChangeDrawingState()
{
    if (isDrawing) AddToRenderer(Level::ImGui);
    else EraseFromRenderer(Level::ImGui);
    sconChangeDrawingState.Disconnect();
}