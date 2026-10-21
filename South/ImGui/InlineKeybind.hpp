#pragma once
#include "imgui_internal.h"
#include <Windows.h>

namespace InlineKeybind {
    inline ImGuiID captureId = 0;
    inline int startedFrame = -1;
    inline int seenFrame = -1;
    inline int completedFrame = -3;

    inline bool IsCapturing() {
        return GImGui && captureId && GImGui->ActiveId == captureId && seenFrame >= GImGui->FrameCount - 1;
    }

    inline bool BlocksWindowHotkeys() {
        return IsCapturing() || (GImGui && completedFrame >= GImGui->FrameCount - 1);
    }

    inline int VirtualKey(ImGuiKey key) {
        if (key >= ImGuiKey_A && key <= ImGuiKey_Z) return 'A' + key - ImGuiKey_A;
        if (key >= ImGuiKey_0 && key <= ImGuiKey_9) return '0' + key - ImGuiKey_0;
        if (key >= ImGuiKey_F1 && key <= ImGuiKey_F24) return VK_F1 + key - ImGuiKey_F1;
        if (key >= ImGuiKey_Keypad0 && key <= ImGuiKey_Keypad9) return VK_NUMPAD0 + key - ImGuiKey_Keypad0;
        switch (key) {
        case ImGuiKey_Tab: return VK_TAB;
        case ImGuiKey_LeftArrow: return VK_LEFT;
        case ImGuiKey_RightArrow: return VK_RIGHT;
        case ImGuiKey_UpArrow: return VK_UP;
        case ImGuiKey_DownArrow: return VK_DOWN;
        case ImGuiKey_PageUp: return VK_PRIOR;
        case ImGuiKey_PageDown: return VK_NEXT;
        case ImGuiKey_Home: return VK_HOME;
        case ImGuiKey_End: return VK_END;
        case ImGuiKey_Insert: return VK_INSERT;
        case ImGuiKey_Delete: return VK_DELETE;
        case ImGuiKey_Backspace: return VK_BACK;
        case ImGuiKey_Space: return VK_SPACE;
        case ImGuiKey_Enter: case ImGuiKey_KeypadEnter: return VK_RETURN;
        case ImGuiKey_LeftCtrl: return VK_LCONTROL;
        case ImGuiKey_RightCtrl: return VK_RCONTROL;
        case ImGuiKey_LeftShift: return VK_LSHIFT;
        case ImGuiKey_RightShift: return VK_RSHIFT;
        case ImGuiKey_LeftAlt: return VK_LMENU;
        case ImGuiKey_RightAlt: return VK_RMENU;
        case ImGuiKey_LeftSuper: return VK_LWIN;
        case ImGuiKey_RightSuper: return VK_RWIN;
        case ImGuiKey_Menu: return VK_APPS;
        case ImGuiKey_CapsLock: return VK_CAPITAL;
        case ImGuiKey_ScrollLock: return VK_SCROLL;
        case ImGuiKey_NumLock: return VK_NUMLOCK;
        case ImGuiKey_PrintScreen: return VK_SNAPSHOT;
        case ImGuiKey_Pause: return VK_PAUSE;
        case ImGuiKey_Apostrophe: return VK_OEM_7;
        case ImGuiKey_Comma: return VK_OEM_COMMA;
        case ImGuiKey_Minus: return VK_OEM_MINUS;
        case ImGuiKey_Period: return VK_OEM_PERIOD;
        case ImGuiKey_Slash: return VK_OEM_2;
        case ImGuiKey_Semicolon: return VK_OEM_1;
        case ImGuiKey_Equal: return VK_OEM_PLUS;
        case ImGuiKey_LeftBracket: return VK_OEM_4;
        case ImGuiKey_Backslash: return VK_OEM_5;
        case ImGuiKey_RightBracket: return VK_OEM_6;
        case ImGuiKey_GraveAccent: return VK_OEM_3;
        case ImGuiKey_KeypadDecimal: return VK_DECIMAL;
        case ImGuiKey_KeypadDivide: return VK_DIVIDE;
        case ImGuiKey_KeypadMultiply: return VK_MULTIPLY;
        case ImGuiKey_KeypadSubtract: return VK_SUBTRACT;
        case ImGuiKey_KeypadAdd: return VK_ADD;
        default: return 0;
        }
    }

    inline const char* KeyName(int key) {
        switch (key) {
        case 0: return "None";
        case VK_LBUTTON: return "M1";
        case VK_RBUTTON: return "M2";
        case VK_MBUTTON: return "M3";
        case VK_XBUTTON1: return "M4";
        case VK_XBUTTON2: return "M5";
        case VK_CONTROL: case VK_LCONTROL: return "Ctrl";
        case VK_RCONTROL: return "RCtrl";
        case VK_SHIFT: case VK_LSHIFT: return "Shift";
        case VK_RSHIFT: return "RShift";
        case VK_MENU: case VK_LMENU: return "Alt";
        case VK_RMENU: return "RAlt";
        }
        for (int i = ImGuiKey_NamedKey_BEGIN; i < ImGuiKey_NamedKey_END; ++i)
            if (VirtualKey((ImGuiKey)i) == key) return ImGui::GetKeyName((ImGuiKey)i);
        return "?";
    }

    struct Result { bool capturingClick = false; bool changed = false; };

    inline Result Update(int* key, bool leftClickStarts = false) {
        if (!key) return {};
        ImGuiContext& g = *GImGui;
        const ImGuiID id = g.LastItemData.ID ^ 0x6b657962;
        const bool hovered = ImGui::IsItemHovered(ImGuiHoveredFlags_AllowWhenBlockedByActiveItem);
        bool active = IsCapturing() && captureId == id;
        Result result;
        if (!active && hovered && (ImGui::IsMouseClicked(1) || (leftClickStarts && ImGui::IsMouseClicked(0)))) {
            captureId = id;
            startedFrame = g.FrameCount;
            ImGui::SetActiveID(id, ImGui::GetCurrentWindow());
            ImGui::FocusWindow(ImGui::GetCurrentWindow());
            active = true;
        }
        if (!active) return result;
        seenFrame = g.FrameCount;
        ImGui::KeepAliveID(id);
        ImGui::SetActiveIdUsingAllKeyboardKeys();
        result.capturingClick = true;
        // The click that starts capture must never become the assigned mouse button.
        if (g.FrameCount == startedFrame) return result;
        int next = -1;
        if (ImGui::IsKeyPressed(ImGuiKey_Escape, false)) next = 0;
        const int mouseKeys[] = {VK_LBUTTON, VK_RBUTTON, VK_MBUTTON, VK_XBUTTON1, VK_XBUTTON2};
        for (int i = 0; next < 0 && i < 5; ++i) {
            if (ImGui::IsMouseClicked(i)) {
                next = mouseKeys[i];
                ImGui::SetKeyOwner(ImGui::MouseButtonToKey(i), id, ImGuiInputFlags_LockUntilRelease);
            }
        }
        for (int i = ImGuiKey_NamedKey_BEGIN; next < 0 && i < ImGuiKey_NamedKey_END; ++i) {
            const int vk = VirtualKey((ImGuiKey)i);
            if (vk && ImGui::IsKeyPressed((ImGuiKey)i, false)) {
                next = vk;
                ImGui::SetKeyOwner((ImGuiKey)i, id, ImGuiInputFlags_LockUntilRelease);
            }
        }
        if (next >= 0) {
            completedFrame = g.FrameCount;
            result.changed = *key != next;
            *key = next;
            ImGui::ClearActiveID();
            captureId = 0;
            if (result.changed) ImGui::MarkItemEdited(g.LastItemData.ID);
        }
        return result;
    }

    inline void Label(ImDrawList* dl, ImFont* font, float fontSize, const ImVec2& pos,
        float right, const char* label, int* key, ImU32 color, ImU32 bindColor, bool showEmpty = false) {
        const char* labelEnd = ImGui::FindRenderedTextEnd(label);
        const bool capturing = key && IsCapturing() && captureId == (GImGui->LastItemData.ID ^ 0x6b657962);
        char suffix[48] = {};
        if (capturing) ImFormatString(suffix, sizeof(suffix), "(...)");
        else if (key && (*key || showEmpty)) ImFormatString(suffix, sizeof(suffix), "(%s)", KeyName(*key));
        const float suffixWidth = font->CalcTextSizeA(fontSize, FLT_MAX, 0, suffix).x;
        const float labelRight = ImMax(pos.x, right - (suffixWidth ? suffixWidth + 5 : 0));
        const ImVec4 clip(pos.x, pos.y, labelRight, pos.y + fontSize + 2);
        dl->AddText(font, fontSize, pos, color, label, labelEnd, 0, &clip);
        if (suffixWidth) {
            const float width = font->CalcTextSizeA(fontSize, FLT_MAX, 0, label, labelEnd).x;
            const ImVec2 suffixPos(ImMin(pos.x + width + 5, labelRight + 5), pos.y);
            const ImVec4 suffixClip(pos.x, pos.y, right, pos.y + fontSize + 2);
            dl->AddText(font, fontSize, suffixPos, bindColor, suffix, nullptr, 0, &suffixClip);
        }
    }
}
