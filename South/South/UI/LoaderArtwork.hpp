#pragma once
#include "../../ImGui/imgui_internal.h"
#include "../../ImGui/InterfaceScale.hpp"

namespace LoaderArtwork {
    inline void Login(ImDrawList* dl, ImTextureID texture, const ImVec2& pos, const ImVec2& size, int alpha) {
        const ImVec2 min = pos + ImVec2(InterfaceScale::Px(196), 0);
        const ImVec2 max = pos + size;
        const ImVec2 extent = max - min;
        // Exclude the reference window border and taskbar; cover without stretching.
        ImVec2 cropMin(201, 5), cropMax(419, 248);
        const float cropRatio = (cropMax.x - cropMin.x) / (cropMax.y - cropMin.y);
        const float ratio = extent.x / extent.y;
        if (ratio < cropRatio) {
            const float inset = ((cropMax.x - cropMin.x) - (cropMax.y - cropMin.y) * ratio) * 0.5f;
            cropMin.x += inset; cropMax.x -= inset;
        } else {
            const float inset = ((cropMax.y - cropMin.y) - (cropMax.x - cropMin.x) / ratio) * 0.5f;
            cropMin.y += inset; cropMax.y -= inset;
        }
        const ImVec2 uvMin(cropMin.x / 432, cropMin.y / 265);
        const ImVec2 uvMax(cropMax.x / 432, cropMax.y / 265);
        const float featherWidth = ImMin(InterfaceScale::Px(72), extent.x * 0.5f);
        const float middleU = ImLerp(uvMin.x, uvMax.x, featherWidth / extent.x);
        const float overlapWidth = featherWidth + 1.0f;
        const float overlapU = ImLerp(uvMin.x, uvMax.x, overlapWidth / extent.x);
        const int firstVertex = dl->VtxBuffer.Size;
        // Cover the antialiased inner edge of the rounded image with the same texels.
        dl->AddImage(texture, min, ImVec2(min.x + overlapWidth, max.y), uvMin, ImVec2(overlapU, uvMax.y));
        for (int i = firstVertex; i < dl->VtxBuffer.Size; ++i) {
            const float coverage = ImClamp((dl->VtxBuffer[i].pos.x - min.x) / featherWidth, 0.0f, 1.0f);
            dl->VtxBuffer[i].col = IM_COL32(255, 255, 255, (int)(alpha * coverage));
        }
        dl->AddImageRounded(texture, ImVec2(min.x + featherWidth, min.y), max, ImVec2(middleU, uvMin.y),
            uvMax, IM_COL32(255, 255, 255, alpha),
            InterfaceScale::Px(10.0f), ImDrawFlags_RoundCornersRight);
    }

    inline void Brand(ImDrawList* dl, ImTextureID texture, const ImVec2& min, const ImVec2& max, int alpha) {
        dl->AddImage(texture, min, max, ImVec2(0.0f, 0.0f),
            ImVec2(1.0f, 1.0f), IM_COL32(255, 255, 255, alpha));
    }

    inline void ProductMark(ImDrawList* dl, const ImVec2& min, const ImVec2& max, int alpha) {
        // FiveM's original vector mark: https://fivem.net/ (logo-img, 50.949 x 60).
        const float scale = ImMin((max.x - min.x) / 50.949f, (max.y - min.y) / 60.0f);
        const ImVec2 origin = (min + max - ImVec2(50.949f, 60) * scale) * 0.5f;
        auto p = [&](float x, float y) { return origin + ImVec2(x, y) * scale; };
        const ImU32 color = IM_COL32(235, 235, 235, alpha);
        dl->PathLineTo(p(10.959f, 27.123f));
        dl->PathLineTo(p(19.509f, 1.473f));
        dl->PathBezierCubicCurveTo(p(19.803f, 0.591f), p(20.625f, 0), p(21.552f, 0));
        dl->PathLineTo(p(30.99f, 0));
        dl->PathLineTo(p(32.787f, 5.403f));
        dl->PathFillConvex(color);
        const ImVec2 right[] = { p(35.67f, 14.07f), p(50.949f, 60), p(38.817f, 60), p(27.624f, 22.107f) };
        const ImVec2 left[] = { p(5.148f, 44.553f), p(21.588f, 28.137f), p(12.123f, 60), p(0, 60) };
        dl->AddConvexPolyFilled(right, IM_ARRAYSIZE(right), color);
        dl->AddConvexPolyFilled(left, IM_ARRAYSIZE(left), color);
    }
}
