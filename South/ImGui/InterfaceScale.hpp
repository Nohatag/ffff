#pragma once
#include "imgui.h"

namespace InterfaceScale {
    constexpr float Factor = 1.30f;
    constexpr float Px(float value) { return value * Factor; }

    inline ImVec2 InitialPosition(const ImVec2& size, bool executor) {
        const ImVec2 display = ImGui::GetIO().DisplaySize;
        const float pairWidth = Px(1320);
        const float x = display.x >= pairWidth + 16
            ? (display.x - pairWidth) * 0.5f + (executor ? 0 : Px(712))
            : executor ? (display.x - size.x) * 0.5f : display.x - size.x - 12;
        return ImVec2(x < 8 ? 8 : x, display.y > size.y + 16 ? (display.y - size.y) * 0.5f : 8);
    }
}
