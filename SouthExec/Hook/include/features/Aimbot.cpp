// Leaked By Cry Service & Siraj
#include "../../includes.h"
#include "Aimbot.h"
#include "../Graphics.h"
#include "../../kiero/minhook/include/MinHook.h"
#include "../Scanner.h"

namespace Aimbot {

    typedef bool(__fastcall* OriginalGetIsAccurate)(uintptr_t, uintptr_t, Vector3*, Vector3*, float);
    static OriginalGetIsAccurate o_GetIsAccurate = nullptr;
    static Vector3 silent_HitPos = Vector3{ 0, 0, 0 };

    static bool __fastcall hkGetIsAccurate(uintptr_t thisPtr, uintptr_t autiste, Vector3* vEnd, Vector3* vAdjustedEnd, float zgeg) {
        if (silent_Enable && silent_HitPos.x != 0.0f) {
            if (vEnd && vAdjustedEnd) {
                if (silent_MagicBullet) {
                    Vector3 dir = silent_HitPos - *vEnd;
                    float len = sqrtf(dir.x * dir.x + dir.y * dir.y + dir.z * dir.z);
                    if (len > 0.001f) {
                        dir = dir * (1.0f / len);
                        *vEnd = silent_HitPos + dir * 5.0f;
                    }
                }
                *vAdjustedEnd = silent_HitPos;
            }
        }
        return o_GetIsAccurate(thisPtr, autiste, vEnd, vAdjustedEnd, zgeg);
    }

    static bool IsOnScreen(ImVec2 coords) {
        return !(coords.x < 0.1f || coords.y < 0.1 || coords.x > ImGui::GetIO().DisplaySize.x || coords.y > ImGui::GetIO().DisplaySize.y);
    }

    static float GetDistance2D(ImVec2 p1, ImVec2 p2) {
        return sqrtf(powf(p2.x - p1.x, 2.0f) + powf(p2.y - p1.y, 2.0f));
    }

    static bool IsPlayer(uintptr_t ped) {
        auto addr = Address::Get();
        uint32_t pedType = addr->SafeRead<uint32_t>(ped + addr->m_EntityType);
        return ((pedType << 11) >> 25) == 2;
    }

    static bool IsAdmin(uintptr_t ped) {
        auto addr = Address::Get();
        BYTE nativeVis = addr->SafeRead<BYTE>(ped + 0x2C, 0xFF);
        float alpha = addr->SafeRead<float>(ped + 0xD0, 255.0f);
        return ((nativeVis & 0x01) == 0) && (alpha < 1.0f);
    }

    static bool IsVisible(uintptr_t targetPed) {
        auto addr = Address::Get();
        uintptr_t world = addr->SafeRead<uintptr_t>(addr->m_World);
        uintptr_t localPlayer = addr->SafeRead<uintptr_t>(world + 0x8);
        if (!addr->IsValid(localPlayer) || !addr->IsValid(targetPed)) return false;

        auto GetHandle = [](uintptr_t ptr) -> int {
            static auto fn = (int(*)(uintptr_t))GetProcAddress(GetModuleHandleA("fivem-mode-venice.dll"), "?PointerToHandle@five@@YAHPEAX@Z");
            if (!fn) return 0;
            return fn(ptr);
        };

        int lHandle = GetHandle(localPlayer);
        int tHandle = GetHandle(targetPed);

        return Invoker::Call<bool>(0xEE4335606471CB7A, lHandle, tHandle, 17);
    }

    static uintptr_t GetBestTarget(float maxFov) {
        auto addr = Address::Get();
        auto core = Core::Get();
        uintptr_t world = addr->SafeRead<uintptr_t>(addr->m_World);
        uintptr_t localPlayer = addr->SafeRead<uintptr_t>(world + 0x8);
        if (!addr->IsValid(localPlayer)) return 0;

        Matrix4x4 localMatrix = addr->SafeRead<Matrix4x4>(localPlayer + 0x60);
        Vector3 localPos = Vector3(localMatrix._41, localMatrix._42, localMatrix._43);

        uintptr_t replay = addr->SafeRead<uintptr_t>(addr->m_ReplayInterface);
        uintptr_t pedInterface = addr->SafeRead<uintptr_t>(replay + addr->m_PedInterface);
        if (!addr->IsValid(pedInterface)) return 0;

        uintptr_t pedList = addr->SafeRead<uintptr_t>(pedInterface + addr->m_PedList);
        int pedMaximum = addr->SafeRead<int>(pedInterface + addr->m_PedMaximum);

        uintptr_t bestPed = 0;
        float best3DDist = 999999.0f;
        float fovLimit = (maxFov < 1000.0f) ? maxFov : (maxFov * 0.004f);
        ImVec2 center = ImVec2(ImGui::GetIO().DisplaySize.x / 2.0f, ImGui::GetIO().DisplaySize.y / 2.0f);

        for (int i = 0; i < pedMaximum; i++) {
            uintptr_t ped = addr->SafeRead<uintptr_t>(pedList + (i * 0x10));
            if (!addr->IsValid(ped) || ped == localPlayer) continue;

            bool isPlayer = IsPlayer(ped);
            if (!isPlayer && !aim_TargetNpcs) continue;

            if (isPlayer) {
                bool isAdmin = IsAdmin(ped);
                if (isAdmin && !aim_TargetAdmins) continue;
            }

            if (aim_ExcludeDeads) {
                float health = addr->SafeRead<float>(ped + addr->m_Health);
                if (health <= 0.0f) continue;
            }

            if (aim_VisibleCheck && !IsVisible(ped)) continue;

            int boneId = 0x796e; 
            if (aim_BoneIdx == 1) boneId = 0x9995; 
            if (aim_BoneIdx == 2) boneId = 0x2e28; 

            Vector3 targetPos = core->GetBone(ped, boneId);
            if (targetPos.x == 0) continue;

            ImVec2 screenPos = core->W2S(targetPos);
            if (!IsOnScreen(screenPos)) continue;

            float dist2D = GetDistance2D(center, screenPos);
            if (dist2D < fovLimit) {
                float dist3D = localPos.Distance(targetPos);
                if (dist3D < best3DDist) {
                    best3DDist = dist3D;
                    bestPed = ped;
                }
            }
        }
        return bestPed;
    }

    static void AimAtPos(Vector3 point, float smoothing) {
        auto addr = Address::Get();
        uintptr_t camera = addr->SafeRead<uintptr_t>(addr->m_CamGameplayDirector);
        if (!addr->IsValid(camera)) return;

        Vector3 crosshairPos = addr->SafeRead<Vector3>(camera + 0x60);
        Vector3 direction = point - crosshairPos;
        float distance = sqrtf(direction.x * direction.x + direction.y * direction.y + direction.z * direction.z);
        if (distance == 0.0f) return;

        uintptr_t camera_params = addr->SafeRead<uintptr_t>(camera + 0x10);
        if (addr->IsValid(camera_params)) {
            if (addr->SafeRead<float>(camera_params + 0x130) == 8.0f) {
                *(float*)(camera_params + 0x130) = 111.0f;
                *(float*)(camera_params + 0x134) = 111.0f;
                *(float*)(camera_params + 0x4CC) = 0.0f;
                if (addr->SafeRead<float>(camera_params + 0x49C) == 1.0f)
                    *(float*)(camera_params + 0x49C) = 0.0f;
                *(float*)(camera_params + 0x2AC) = 0.0f;
                *(float*)(camera_params + 0x2B0) = 0.0f;
            }
        }

        Vector3 aim_direction = direction * (1.0f / distance);

        if (smoothing <= 1.0f) {
            *(Vector3*)(camera + 0x40) = aim_direction;  
            *(Vector3*)(camera + 0x3D0) = aim_direction; 
        } else {

            float step = 1.0f / (1.0f + (smoothing * 0.001f));
            if (step > 1.0f) step = 1.0f;
            if (step < 0.0001f) step = 0.0001f;

            auto angles_calc = [&](uintptr_t addr, const Vector3& target_direction) {
                Vector3 current_angles = *(Vector3*)(addr);
                Vector3 new_direction;
                new_direction.x = current_angles.x + (target_direction.x - current_angles.x) * step;
                new_direction.y = current_angles.y + (target_direction.y - current_angles.y) * step;
                new_direction.z = current_angles.z + (target_direction.z - current_angles.z) * step;

                float len = sqrtf(new_direction.x * new_direction.x + new_direction.y * new_direction.y + new_direction.z * new_direction.z);
                if (len > 0.001f) {
                    new_direction = new_direction * (1.0f / len);
                }
                *(Vector3*)(addr) = new_direction;
            };
            angles_calc(camera + 0x40, aim_direction);
            angles_calc(camera + 0x3D0, aim_direction);
        }
    }

    static void SilentAim(Vector3 point) {

        silent_HitPos = point;
    }

    void Tick() {
        auto addr = Address::Get();
        if (aim_Enable && (GetAsyncKeyState(aim_Key) & 0x8000)) {
            uintptr_t target = GetBestTarget(aim_Fov);
            if (target) {
                int boneId = 0x796e; 
                if (aim_BoneIdx == 1) boneId = 0x9995; 
                if (aim_BoneIdx == 2) boneId = 0x2e28;

                Vector3 pos = Core::Get()->GetBone(target, boneId);
                if (pos.x != 0) AimAtPos(pos, aim_Smoothing);
            }
        }

        if (silent_Enable && (silent_Key == 0 || (GetAsyncKeyState(silent_Key) & 0x8000))) {

            static bool hook_initialized = false;
            if (!hook_initialized) {

                uintptr_t sig = Scanner::Get()->Scan(NULL, "48 8B C4 48 89 58 ? 48 89 68 ? 48 89 70 ? 48 89 78 ? 41 56 48 83 EC 70 0F 29 70 ? 48 8B F1", 0, 0);
                if (sig) {
                    if (MH_CreateHook((LPVOID)sig, &hkGetIsAccurate, (LPVOID*)&o_GetIsAccurate) == MH_OK) {
                        MH_EnableHook((LPVOID)sig);
                    }
                }
                hook_initialized = true;
            }

            uintptr_t target = GetBestTarget(silent_Fov);
            if (target) {
                int boneId = 0x796e; 
                if (aim_BoneIdx == 1) boneId = 0x9995; 
                if (aim_BoneIdx == 2) boneId = 0x2e28; 

                Vector3 pos = Core::Get()->GetBone(target, boneId);
                if (pos.x != 0) {
                    uintptr_t camera = addr->SafeRead<uintptr_t>(addr->m_CamGameplayDirector);
                    if (addr->IsValid(camera)) {
                        Vector3 cam = addr->SafeRead<Vector3>(camera + 0x60);
                        Vector3 dir = pos - cam;
                        float len = sqrtf(dir.x * dir.x + dir.y * dir.y + dir.z * dir.z);
                        if (len > 0.001f) {
                            dir = dir * (1.0f / len);
                            silent_HitPos = pos + dir * 3.5f; 
                        } else {
                            silent_HitPos = pos;
                        }
                    } else {
                        silent_HitPos = pos;
                    }
                }
            } else {
                silent_HitPos = Vector3{ 0, 0, 0 };
            }
        } else {
            silent_HitPos = Vector3{ 0, 0, 0 };
        }

        if (aim_ShowFov) {
            ImVec2 center = ImVec2(ImGui::GetIO().DisplaySize.x / 2.0f, ImGui::GetIO().DisplaySize.y / 2.0f);
            Graphics::DrawCircle(center, aim_Fov * 0.004f, ImColor(col_aim_Fov[0], col_aim_Fov[1], col_aim_Fov[2], col_aim_Fov[3]), 1.5f);
        }
        if (silent_ShowFov) {
            ImVec2 center = ImVec2(ImGui::GetIO().DisplaySize.x / 2.0f, ImGui::GetIO().DisplaySize.y / 2.0f);
            Graphics::DrawCircle(center, silent_Fov * 0.004f, ImColor(col_silent_Fov[0], col_silent_Fov[1], col_silent_Fov[2], col_silent_Fov[3]), 1.5f);
        }
    }
}

