#pragma once
#include "../Address.h"
#include "../../imgui/imgui.h"

extern bool wep_NoRecoil;
extern bool wep_NoSpread;
extern bool wep_InfRange;

namespace Weapon {
    inline void Tick() {
        if (!wep_NoRecoil && !wep_NoSpread && !wep_InfRange) return;

        auto addr = Address::Get();
        uintptr_t world = addr->SafeRead<uintptr_t>(addr->m_World);
        uintptr_t localPed = addr->SafeRead<uintptr_t>(world + 0x8);
        if (!addr->IsValid(localPed)) return;

        uintptr_t weaponMgr = addr->SafeRead<uintptr_t>(localPed + addr->m_WeaponManager);
        if (!addr->IsValid(weaponMgr)) return;

        uintptr_t weaponInfo = addr->SafeRead<uintptr_t>(weaponMgr + 0x20);
        if (!addr->IsValid(weaponInfo)) return;

        if (wep_NoRecoil) {
            *(float*)(weaponInfo + addr->m_Recoil) = 0.0f;
        }
        if (wep_NoSpread) {
            *(float*)(weaponInfo + addr->m_Spread) = 0.0f;
        }
        if (wep_InfRange) {
            *(float*)(weaponInfo + addr->m_Range) = 99999.0f;
        }
    }
}