// Leaked By Cry Service & Siraj
#include "Core.h"
#include <cmath>

Core* Core::s_pSingleton = nullptr;

namespace Utils {

    uintptr_t CalibrateViewport(uintptr_t viewportPtr, uintptr_t worldAddr, uintptr_t visualPosOffset, float resX, float resY) {
        auto addr = Address::Get();
        uintptr_t offsets[] = { 0x270, 0x24C, 0x250, 0x240, 0x70, 0x258, 0x260 };

        uintptr_t world = addr->SafeRead<uintptr_t>(worldAddr);
        uintptr_t localPlayer = addr->SafeRead<uintptr_t>(world + 0x8);
        if (!addr->IsValid(localPlayer)) return 0;

        Vector3 localPos = addr->SafeRead<Vector3>(localPlayer + visualPosOffset);
        if (localPos.x == 0) {
            Matrix4x4 localMatrix = addr->SafeRead<Matrix4x4>(localPlayer + 0x60);
            localPos = Vector3(localMatrix._41, localMatrix._42, localMatrix._43);
        }

        for (uintptr_t off : offsets) {
            Matrix4x4 m = addr->SafeRead<Matrix4x4>(viewportPtr + off);
            m.Transpose();

            float screenW = (m._41 * localPos.x) + (m._42 * localPos.y) + (m._43 * localPos.z) + m._44;
            if (screenW > 0.0001f) {
                float screenX = (m._21 * localPos.x) + (m._22 * localPos.y) + (m._23 * localPos.z) + m._24;
                float screenY = (m._31 * localPos.x) + (m._32 * localPos.y) + (m._33 * localPos.z) + m._34;
                float invW = 1.0f / screenW;
                float x = (resX * 0.5f) + (0.5f * (screenX * invW) * resX + 0.5f);
                float y = (resY * 0.5f) - (0.5f * (screenY * invW) * resY + 0.5f);

                if (x > 0 && x < resX && y > 0 && y < resY) {
                    return off;
                }
            }
        }
        return 0;
    }
}


bool g_ResetESP = false;

ImVec2 Core::W2S(Vector3 pos) {
    auto addr = Address::Get();
    if (!addr) return ImVec2(0, 0);

    uintptr_t viewportPtr = addr->SafeRead<uintptr_t>(addr->m_ViewPort);
    if (!addr->IsValid(viewportPtr)) return ImVec2(0, 0);

    ImGuiIO& io = ImGui::GetIO();
    float resX = io.DisplaySize.x;
    float resY = io.DisplaySize.y;

    static uintptr_t viewportOffset = 0;
    static bool calibrated = false;
    static int calibrationTicks = 0;
    static uintptr_t lastLocalPlayer = 0;
    static uintptr_t lastViewportPtr = 0;

    static float lastResX = 0, lastResY = 0;
    if (resX != lastResX || resY != lastResY) {
        calibrated = false;
        lastResX = resX;
        lastResY = resY;
    }

    if (g_ResetESP) {
        calibrated = false;
        viewportOffset = 0;
        g_ResetESP = false;
    }

    uintptr_t world = addr->SafeRead<uintptr_t>(addr->m_World);
    uintptr_t localPlayer = addr->IsValid(world) ? addr->SafeRead<uintptr_t>(world + 0x8) : 0;

    if (viewportPtr != lastViewportPtr || localPlayer != lastLocalPlayer) {
        calibrated = false;
        lastViewportPtr = viewportPtr;
        lastLocalPlayer = localPlayer;
    }

    if (!calibrated || viewportOffset == 0) {
        viewportOffset = 0x24C;
        calibrated = true;
    }

    Matrix4x4 matrix = addr->SafeRead<Matrix4x4>(viewportPtr + viewportOffset);
    matrix.Transpose();

    float screenX = (matrix._21 * pos.x) + (matrix._22 * pos.y) + (matrix._23 * pos.z) + matrix._24;
    float screenY = (matrix._31 * pos.x) + (matrix._32 * pos.y) + (matrix._33 * pos.z) + matrix._34;
    float screenW = (matrix._41 * pos.x) + (matrix._42 * pos.y) + (matrix._43 * pos.z) + matrix._44;

    if (!std::isfinite(screenX) || !std::isfinite(screenY) || !std::isfinite(screenW) || screenW < 0.1f)
        return ImVec2(0, 0);

    float invW = 1.0f / screenW;
    screenX *= invW;
    screenY *= invW;

    ImVec2 screen;
    screen.x = (resX * 0.5f) + (0.5f * screenX * resX + 0.5f);
    screen.y = (resY * 0.5f) - (0.5f * screenY * resY + 0.5f);

    if (!std::isfinite(screen.x) || !std::isfinite(screen.y))
        return ImVec2(0, 0);

    return screen;
}

Vector3 Core::BoneVec(const uintptr_t cPed, int boneid) {
    auto addr = Address::Get();
    if (!addr || !addr->IsValid(cPed)) return Vector3();

    Matrix4x4 matrix = addr->SafeRead<Matrix4x4>(cPed + 0x60);
    Vector3 bone = addr->SafeRead<Vector3>(cPed + addr->m_BoneOffset + (boneid * 0x10));

    Vector3 transform;

    transform.x = (bone.x * matrix._11) + (bone.y * matrix._21) + (bone.z * matrix._31) + matrix._41;
    transform.y = (bone.x * matrix._12) + (bone.y * matrix._22) + (bone.z * matrix._32) + matrix._42;
    transform.z = (bone.x * matrix._13) + (bone.y * matrix._23) + (bone.z * matrix._33) + matrix._43;

    return transform;
}

Vector3 Core::GetBone(const uintptr_t cPed, int boneid) {
    auto addr = Address::Get();
    if (!addr || !addr->m_GetBonePos || !addr->IsValid(cPed)) return Vector3();

    auto GetBonePos = reinterpret_cast<void(__fastcall*)(uintptr_t, float*, int)>(addr->m_GetBonePos);

    alignas(16) float bone[4] = { 0 };
    GetBonePos(cPed, bone, boneid);

    return Vector3{ bone[0], bone[1], bone[2] };
}
