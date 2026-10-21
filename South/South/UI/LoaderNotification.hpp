#pragma once
#include <string>
#include <algorithm>
#include "../../ImGui/imgui.h"
#include "../../ImGui/imgui_internal.h"
#include "../../ImGui/InterfaceScale.hpp"

extern ImFont* InterSemiBold;

namespace LoaderNotification {
    using InterfaceScale::Px;
    inline std::string title;
    inline std::string message;
    inline double startedAt = -1.0;
    inline float duration = 0.0f;

    inline void Show(const char* heading, const char* text, float seconds = 3.0f) {
        title = heading;
        message = text;
        startedAt = ImGui::GetTime();
        duration = seconds;
    }

    inline bool IsVisible() {
        return startedAt >= 0.0 && ImGui::GetTime() - startedAt < duration;
    }

    inline void Draw(const ImVec2& windowPos, const ImVec2& windowSize) {
        if (!IsVisible()) return;
        const float elapsed = (float)(ImGui::GetTime() - startedAt);
        const float fadeIn = ImClamp(elapsed / 0.18f, 0.0f, 1.0f);
        const float fadeOut = ImClamp((duration - elapsed) / 0.25f, 0.0f, 1.0f);
        const float visibility = ImMin(fadeIn, fadeOut);
        const float eased = 1.0f - (1.0f - visibility) * (1.0f - visibility);

        float textW1 = InterSemiBold ? InterSemiBold->CalcTextSizeA(Px(12.0f), FLT_MAX, 0.0f, message.c_str()).x : 100.0f;
        float textW2 = InterSemiBold ? InterSemiBold->CalcTextSizeA(Px(12.0f), FLT_MAX, 0.0f, title.c_str()).x : 100.0f;
        const float notifW = ImMin(Px(250.0f), ImMax(Px(150.0f), ImMax(textW1, textW2) + Px(26.0f)));

        const ImVec2 notifMin(windowPos.x + windowSize.x - notifW - Px(9.0f) + (1.0f - eased) * Px(28.0f), windowPos.y + Px(30.0f));
        const ImVec2 notifMax(notifMin.x + notifW, notifMin.y + Px(44.0f));
        const int alpha = (int)(eased * ImGui::GetStyle().Alpha * 255.0f);

        ImDrawList* dl = ImGui::GetForegroundDrawList();
        const ImVec2 clipMax(windowPos.x + windowSize.x, windowPos.y + windowSize.y);
        dl->PushClipRect(windowPos, clipMax, true);

        dl->AddRectFilled(notifMin, notifMax, IM_COL32(10, 10, 11, alpha), Px(4.0f));
        dl->AddRect(notifMin, notifMax, IM_COL32(255, 255, 255, (int)(alpha * 0.025f)), Px(4.0f));
        dl->AddRectFilled(ImVec2(notifMin.x, notifMin.y + Px(6.0f)), ImVec2(notifMin.x + Px(2.0f), notifMin.y + Px(34.0f)), IM_COL32(230, 45, 55, alpha), Px(1.5f));

        if (InterSemiBold) {
            dl->AddText(InterSemiBold, Px(12.0f), ImVec2(notifMin.x + Px(14.0f), notifMin.y + Px(7.0f)), IM_COL32(208, 208, 211, alpha), title.c_str());
            const ImVec4 clip(notifMin.x + Px(13.0f), notifMin.y, notifMax.x - Px(7.0f), notifMax.y);
            dl->AddText(InterSemiBold, Px(12.0f), ImVec2(notifMin.x + Px(14.0f), notifMin.y + Px(23.0f)), IM_COL32(202, 202, 205, alpha),
                message.c_str(), nullptr, 0.0f, &clip);
        } else {
            dl->AddText(ImVec2(notifMin.x + Px(14.0f), notifMin.y + Px(7.0f)), IM_COL32(208, 208, 211, alpha), title.c_str());
            dl->AddText(ImVec2(notifMin.x + Px(14.0f), notifMin.y + Px(23.0f)), IM_COL32(202, 202, 205, alpha), message.c_str());
        }

        dl->PopClipRect();
    }
}
