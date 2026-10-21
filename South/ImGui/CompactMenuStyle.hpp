#pragma once
#include "imgui.h"
#include "InterfaceScale.hpp"

extern ImFont* MainMenuFont;

namespace CompactMenuStyle {
    inline bool enabled = false;
    inline float Px(float value) { return enabled ? InterfaceScale::Px(value) : value; }

    inline ImFont* Font(ImFont* regular) {
        return enabled && MainMenuFont ? MainMenuFont : regular;
    }

    struct Scope {
        bool previous;
        Scope() : previous(enabled) {
            enabled = true;
            ImGui::PushFont(MainMenuFont);
            ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2(Px(4), Px(1)));
            ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2(Px(4), Px(3)));
            ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0, 0));
            ImGui::PushStyleVar(ImGuiStyleVar_FrameBorderSize, 0.0f);
            ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.0f);
            ImGui::PushStyleVar(ImGuiStyleVar_ChildRounding, Px(2.0f));
            ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, Px(4.0f));
            ImGui::PushStyleVar(ImGuiStyleVar_ScrollbarSize, Px(3.0f));
            ImGui::PushStyleVar(ImGuiStyleVar_ScrollbarRounding, Px(1.5f));
            ImGui::PushStyleColor(ImGuiCol_WindowBg, IM_COL32(0, 0, 0, 0));
            ImGui::PushStyleColor(ImGuiCol_ChildBg, IM_COL32(0, 0, 0, 0));
            ImGui::PushStyleColor(ImGuiCol_PopupBg, IM_COL32(18, 18, 24, 255));
            ImGui::PushStyleColor(ImGuiCol_Text, IM_COL32(193, 194, 205, 255));
            ImGui::PushStyleColor(ImGuiCol_TextDisabled, IM_COL32(89, 89, 101, 255));
            ImGui::PushStyleColor(ImGuiCol_FrameBg, IM_COL32(18, 18, 24, 255));
            ImGui::PushStyleColor(ImGuiCol_FrameBgHovered, IM_COL32(22, 22, 29, 255));
            ImGui::PushStyleColor(ImGuiCol_FrameBgActive, IM_COL32(25, 26, 34, 255));
            ImGui::PushStyleColor(ImGuiCol_SliderGrab, IM_COL32(230, 45, 55, 255));
            ImGui::PushStyleColor(ImGuiCol_SliderGrabActive, IM_COL32(255, 70, 80, 255));
            ImGui::PushStyleColor(ImGuiCol_Header, IM_COL32(65, 20, 25, 255));
            ImGui::PushStyleColor(ImGuiCol_HeaderHovered, IM_COL32(85, 25, 32, 255));
            ImGui::PushStyleColor(ImGuiCol_HeaderActive, IM_COL32(110, 30, 38, 255));
            ImGui::PushStyleColor(ImGuiCol_ScrollbarGrab, IM_COL32(230, 45, 55, 200));
            ImGui::PushStyleColor(ImGuiCol_ScrollbarGrabHovered, IM_COL32(255, 70, 80, 240));
            ImGui::PushStyleColor(ImGuiCol_ScrollbarGrabActive, IM_COL32(255, 70, 80, 255));
            ImGui::PushStyleColor(ImGuiCol_TextSelectedBg, IM_COL32(230, 45, 55, 90));
            ImGui::PushStyleColor(ImGuiCol_NavHighlight, IM_COL32(255, 70, 80, 120));
        }
        ~Scope() {
            ImGui::PopStyleColor(18);
            ImGui::PopStyleVar(9);
            ImGui::PopFont();
            enabled = previous;
        }
        Scope(const Scope&) = delete;
        Scope& operator=(const Scope&) = delete;
    };
}
