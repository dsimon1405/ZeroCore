#include <ZC_IGWindow.h>

#include <ZC/Tools/Container/ZC_ContFunc.h>
#include "ZC_ImGui.h"
#include <ZC/Events/ZC_Events.h>
#include <ZC/ErrorLogger/ZC_ErrorLogger.h>

#include <imgui.h>

ZC_IGWindow::~ZC_IGWindow()
{
    ZC_ForwardListErase(unicNames, name);
    if (isDrawing)
        if (!ZC_ForwardListErase(rendererWindows, this)) ZC_ErrorLogger::Err("Can't find to delete!", __FILE__, __LINE__);  
}

void ZC_IGWindow::NeedDraw(bool _needDraw)
{
    if (_needDraw == isDrawing) return;
    isDrawing = !isDrawing;
    sconChangeDrawingState = ZC_Events::ConnectHandleEventsEnd({ &ZC_IGWindow::UpadteRendererState, this });
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

    if (isDrawing) rendererWindows.emplace_front(this);
}

void ZC_IGWindow::UpdateAndDraw()
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

void ZC_IGWindow::CallAfterZC_WindowResized()
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

void ZC_IGWindow::UpadteRendererState(float time)
{
    if (isDrawing) rendererWindows.emplace_front(this);
    else
    {
        if (!ZC_ForwardListErase(rendererWindows, this)) ZC_ErrorLogger::Err("Can't find to delete!", __FILE__, __LINE__);  
    }
    sconChangeDrawingState.Disconnect();
}

void ZC_IGWindow::Draw()
{
    ZC_ImGui::FrameStart();
    for (auto pIGWindow : rendererWindows) pIGWindow->UpdateAndDraw();
    ZC_ImGui::FrameEnd();
}