#include <ZC_IGWindow.h>

#include <ZC/Tools/Container/ZC_ContFunc.h>
#include "ZC_ImGui.h"
#include <ZC/Events/ZC_Events.h>
#include <ZC/ErrorLogger/ZC_ErrorLogger.h>

#include <imgui.h>

ZC_IGWindow::~ZC_IGWindow()
{
    ZC_ForwardListErase(unicNames, name);
    if (isDrawing) std::erase(drawingWindows, this);
}

void ZC_IGWindow::NeedDrawIGW(bool _needDraw)
{
    if (_needDraw == isDrawing) return;
    isDrawing = !isDrawing;
    
    if (isDrawingInProcess) AddAfterDrawEvent({ &ZC_IGWindow::Update_drawingWindows, this });
    else if (isDrawing) drawingWindows.emplace_back(this);
    else std::erase(drawingWindows, this);

    UpdateFocusState(isDrawing, false);
}

bool ZC_IGWindow::IsDrawingIGW() const noexcept
{
    return isDrawing;
}

void ZC_IGWindow::MakeFocusedIGW()
{
    if (!isFocused) ImGui::SetWindowFocus(name);
}

void ZC_IGWindow::ActivateIGW()
{
    NeedDrawIGW(true);
    MakeFocusedIGW();
}

void ZC_IGWindow::NeedImGuiDraw(bool _needDraw) noexcept
{
    needDrawImGui = _needDraw;
}

bool ZC_IGWindow::IsCursorInOneOfWindows() noexcept
{
    return isCursorInOneOfWindows;
}

ZC_IGWindow::ZC_IGWindow(std::string&& unicName, bool needDraw, float _width, float _height,
        float _indentX, float _indentY, ZC_WindowOrthoIndentFlags _indentFlags, bool _mayClose, ImGuiWindowFlags _igwf)
    : ZC_WindowOrthoIndent(true, _width, _height, _indentX, _indentY, _indentFlags),
    name(AddName(std::move(unicName))),
    isDrawing(needDraw),
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

    if (isDrawing) drawingWindows.emplace_back(this);
}

// ZC_IGWindow::ZC_IGWindow(ZC_IGWindow&& igw)
//     : ZC_WindowOrthoIndent(dynamic_cast<ZC_WindowOrthoIndent&&>(igw)),
//     name(igw.name),
//     isDrawing(igw.isDrawing),
//     mayClose(igw.mayClose),
//     igwf(igw.igwf),
//     needSetPosition(igw.needSetPosition)
// {
//     igw.name = nullptr;
//     igw.isDrawing = false;
// }

void ZC_IGWindow::AddAfterDrawEvent(ZC_Function<void()>&& func)
{
    afterDrawEvents.emplace_front(std::move(func));
}

void ZC_IGWindow::UpdateAndDraw()
{
    SetPosition();
    if (mayClose)
    {
        bool needDrawInNextFrame = true;
        if (ImGui::Begin(name, &needDrawInNextFrame, igwf)) UpdateCursorCollisionState();
        if (!needDrawInNextFrame && isDrawing) NeedDrawIGW(needDrawInNextFrame);
    }
    else
    {
        ImGui::Begin(name, NULL, igwf);
        UpdateCursorCollisionState();
    }
    ImGui::End();
}

void ZC_IGWindow::VCallAfterZC_WindowResizedWOI()
{
    needSetPosition = true;
}

const char* ZC_IGWindow::AddName(std::string&& unicName)
{
    assert(!ZC_Find(unicNames, unicName));
    return unicNames.emplace_front(std::move(unicName)).c_str();
}

void ZC_IGWindow::SetPosition()
{
    if (!needSetPosition) return;
    ImGui::SetNextWindowPos({ currentIndents[0], currentIndents[1] });
    needSetPosition = false;
}

void ZC_IGWindow::Update_drawingWindows()
{
    if (isDrawing) drawingWindows.emplace_back(this);
    else std::erase(drawingWindows, this);
}

void ZC_IGWindow::UpdateCursorCollisionState()
{
    if (ImGui::IsWindowHovered()) isCursorInOneOfWindows = true;
    
    UpdateFocusState(ImGui::IsWindowFocused(), true);

    VDrawWindowIGW();
}

void ZC_IGWindow::UpdateFocusState(bool _isFocused, bool needUpdate_drawingWindows)
{
    if (_isFocused == isFocused) return;
    //  window came in focus and need update order of drawing windows, and window not last for drawing now, make it last
    if (needUpdate_drawingWindows && isFocused && *(--(drawingWindows.end())) != this) AddAfterDrawEvent({ &ZC_IGWindow::MakeLastIn_drawingWindows, this });

    isFocused = _isFocused;
    VFocusStateChangedIGW(isFocused);
}

void ZC_IGWindow::MakeLastIn_drawingWindows()
{
    std::erase(drawingWindows, this);
    drawingWindows.emplace_back(this);
}

void ZC_IGWindow::Make_isCursorInOneOfWindows_false(float time) noexcept
{
    isCursorInOneOfWindows = false;
}

void ZC_IGWindow::Draw()
{
    if (needDrawImGui)
    {
        isDrawingInProcess = true;
        ZC_ImGui::FrameStart();
        for (auto pIGWindow : drawingWindows) pIGWindow->UpdateAndDraw();
        ZC_ImGui::FrameEnd();
        isDrawingInProcess = false;
    }

    if (afterDrawEvents.empty()) return;
    //  call after draw events, added while draw
    for (auto& func : afterDrawEvents) func();
    afterDrawEvents.clear();
}