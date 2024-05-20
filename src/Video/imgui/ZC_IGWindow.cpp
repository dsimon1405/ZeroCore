#include <ZC_IGWindow.h>

#include <ZC/Tools/Container/ZC_ContFunc.h>
#include "ZC_ImGui.h"
#include <ZC/Events/ZC_Events.h>
#include <ZC/ErrorLogger/ZC_ErrorLogger.h>

#include <imgui.h>

ZC_IGWindow::~ZC_IGWindow()
{
    ZC_ForwardListErase(unicNames, pName);
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
    if (!isFocused) ImGui::SetWindowFocus(pName);
}

void ZC_IGWindow::ActivateIGW()
{
    NeedDrawIGW(true);
    MakeFocusedIGW();
}

const char* ZC_IGWindow::GetName() const noexcept
{
    return pName;
}

bool ZC_IGWindow::IsFocusedIGW() const noexcept
{
    return isFocused;
}

bool ZC_IGWindow::IsImGuiDrawing() noexcept
{
    return needDrawImGui;
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
        //  if uses auto resize (size can be changed in each frame, depends on window's content), on start take some random width, height 100.f for example
    : ZC_WindowOrthoIndent(true, _igwf & ImGuiWindowFlags_AlwaysAutoResize ? 100.f : _width, _igwf & ImGuiWindowFlags_AlwaysAutoResize ? 100.f : _height,
        _indentX, _indentY, _indentFlags),
    pName(AddName(std::move(unicName))),
    isDrawing(needDraw),
    mayClose(_mayClose),
    igwf(_igwf)
{
    this->CalculateCurrentIndents();
    ZC_ImGui::FrameStart();
    SetPosition();
    ImGui::SetNextWindowSize(ImVec2(_width, _height));
    ImGui::Begin(pName, NULL);
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
        if (ImGui::Begin(pName, &needDrawInNextFrame, igwf)) UpdateCursorCollisionStateAndDraw();
        if (!needDrawInNextFrame && isDrawing) NeedDrawIGW(needDrawInNextFrame);
    }
    else
    {
        ImGui::Begin(pName, NULL, igwf);
        UpdateCursorCollisionStateAndDraw();
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

void ZC_IGWindow::UpdateCursorCollisionStateAndDraw()
{
    if (ImGui::IsWindowHovered()) isCursorInOneOfWindows = true;
    
    UpdateFocusState(ImGui::IsWindowFocused(), true);

    VDrawWindowIGW();

    if (igwf & ImGuiWindowFlags_AlwaysAutoResize)
    {
        auto windowSize = ImGui::GetWindowSize();
        needSetPosition = this->SetNewSize(windowSize[0], windowSize[1]);
    }
}

void ZC_IGWindow::UpdateFocusState(bool _isFocused, bool needUpdate_drawingWindows)
{
    if (_isFocused == isFocused) return;
    //  window came in focus and need update order of drawing windows, and window not last for drawing now, make it last
    if (needUpdate_drawingWindows && isFocused && *(--(drawingWindows.end())) != this) AddAfterDrawEvent({ &ZC_IGWindow::MakeLastInto_drawingWindows, this });

    isFocused = _isFocused;
    VFocusStateChangedIGW(isFocused);
}

void ZC_IGWindow::MakeLastInto_drawingWindows()
{
    std::erase(drawingWindows, this);
    drawingWindows.emplace_back(this);
}

void ZC_IGWindow::Draw()
{
    isCursorInOneOfWindows = false;
    if (needDrawImGui)
    {
        isDrawingInProcess = true;
        ZC_ImGui::FrameStart();
        for (auto pIGWindow : drawingWindows)
            pIGWindow->UpdateAndDraw();
        ZC_ImGui::FrameEnd();
        isDrawingInProcess = false;
    }

    if (afterDrawEvents.empty()) return;
    //  call after draw events, added while draw
    for (auto& func : afterDrawEvents) func();
    afterDrawEvents.clear();
}