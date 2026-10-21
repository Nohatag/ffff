// Leaked By Cry Service & Siraj
#pragma once
#include "../imgui/imgui.h"
#include <string>

class Graphics {
public:
    static void DrawText(ImVec2 pos, ImColor color, const char* text, float size = 15.0f) {
        ImGui::GetBackgroundDrawList()->AddText(nullptr, size, pos, color, text);
    }

    static void DrawOutlinedText(ImVec2 pos, ImColor color, const char* text, float size = 15.0f) {
        auto draw = ImGui::GetBackgroundDrawList();
        draw->AddText(nullptr, size, ImVec2(pos.x - 1, pos.y - 1), ImColor(0, 0, 0, 255), text);
        draw->AddText(nullptr, size, ImVec2(pos.x + 1, pos.y - 1), ImColor(0, 0, 0, 255), text);
        draw->AddText(nullptr, size, ImVec2(pos.x - 1, pos.y + 1), ImColor(0, 0, 0, 255), text);
        draw->AddText(nullptr, size, ImVec2(pos.x + 1, pos.y + 1), ImColor(0, 0, 0, 255), text);
        draw->AddText(nullptr, size, pos, color, text);
    }

    static void DrawText2(const char* text, float x, float y, float size, ImColor color, bool outline = true) {
        if (outline) DrawOutlinedText(ImVec2(x, y), color, text, size);
        else DrawText(ImVec2(x, y), color, text, size);
    }

    static void DrawLine(ImVec2 from, ImVec2 to, ImColor color, float thickness = 1.0f) {
        ImGui::GetBackgroundDrawList()->AddLine(from, to, color, thickness);
    }

    static void DrawLine(float x1, float y1, float x2, float y2, ImColor color, float thickness = 1.0f) {
        ImGui::GetBackgroundDrawList()->AddLine(ImVec2(x1, y1), ImVec2(x2, y2), color, thickness);
    }

    static void DrawRect(ImVec2 pos, ImVec2 size, ImColor color, float thickness = 1.0f) {
        ImGui::GetBackgroundDrawList()->AddRect(pos, ImVec2(pos.x + size.x, pos.y + size.y), color, 0.0f, 0, thickness);
    }

    static void DrawCircle(ImVec2 center, float radius, ImColor color, float thickness = 1.0f) {
        ImGui::GetBackgroundDrawList()->AddCircle(center, radius, color, 100, thickness);
    }

    static void DrawCircle(float x, float y, float radius, ImColor color, float thickness = 1.0f) {
        ImGui::GetBackgroundDrawList()->AddCircle(ImVec2(x, y), radius, color, 100, thickness);
    }

    static void DrawBox(ImVec2 pos, ImVec2 size, ImColor color, float thickness = 1.0f) {
        DrawRect(pos, size, color, thickness);
    }

    static void DrawCornerBox(float x, float y, float w, float h, ImColor color, float thickness = 1.0f) {
        auto draw = ImGui::GetBackgroundDrawList();
        float lineW = w / 8.5f;
        float lineH = h / 14.0f;

        draw->AddLine(ImVec2(x, y), ImVec2(x + lineW, y), color, thickness);
        draw->AddLine(ImVec2(x, y), ImVec2(x, y + lineH), color, thickness);

        draw->AddLine(ImVec2(x + w, y), ImVec2(x + w - lineW, y), color, thickness);
        draw->AddLine(ImVec2(x + w, y), ImVec2(x + w, y + lineH), color, thickness);

        draw->AddLine(ImVec2(x, y + h), ImVec2(x + lineW, y + h), color, thickness);
        draw->AddLine(ImVec2(x, y + h), ImVec2(x, y + h - lineH), color, thickness);

        draw->AddLine(ImVec2(x + w, y + h), ImVec2(x + w - lineW, y + h), color, thickness);
        draw->AddLine(ImVec2(x + w, y + h), ImVec2(x + w, y + h - lineH), color, thickness);
    }

    static void DrawHealthBar(ImVec2 pos, ImVec2 size, float health, float maxHealth, ImColor color) {
        auto draw = ImGui::GetBackgroundDrawList();
        float healthRatio = health / maxHealth;
        if (healthRatio < 0.0f) healthRatio = 0.0f;
        if (healthRatio > 1.0f) healthRatio = 1.0f;

        draw->AddRect(ImVec2(pos.x - 1, pos.y - 1), ImVec2(pos.x + size.x + 1, pos.y + size.y + 1), ImColor(0, 0, 0, 255), 0.0f, 0, 1.5f);

        draw->AddRectFilled(pos, ImVec2(pos.x + size.x, pos.y + size.y), ImColor(0, 0, 0, 150));

        float barHeight = size.y * healthRatio;
        draw->AddRectFilled(ImVec2(pos.x, pos.y + size.y - barHeight), ImVec2(pos.x + size.x, pos.y + size.y), color);
    }
};
