#include <ZC_IGTextCentered.h>

#include <imgui.h>

ZC_IGTextCentered::ZC_IGTextCentered(std::string&& _text)
    : text(std::move(_text)),
    indentX((ImGui::GetWindowWidth() - ImGui::CalcTextSize(text.c_str()).x) * 0.5f)
{}

void ZC_IGTextCentered::RecalculateIndent()
{
    indentX = (ImGui::GetWindowWidth() - ImGui::CalcTextSize(text.c_str()).x) * 0.5f;
}

void ZC_IGTextCentered::SetText(std::string&& _text)
{
    text = std::move(_text);
    indentX = (ImGui::GetWindowWidth() - ImGui::CalcTextSize(text.c_str()).x) * 0.5f;
}

void ZC_IGTextCentered::Draw()
{
    ImGui::SetCursorPosX(indentX);
    ImGui::TextUnformatted(text.c_str());
}