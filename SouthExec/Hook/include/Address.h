// Leaked By Cry Service & Siraj
#pragma once
#include <windows.h>
#include <vector>
#include <string>
#include <iostream>
#include <unordered_map>
#include <cmath>

struct Vector3 {
    float x, y, z;
    Vector3() : x(0), y(0), z(0) {}
    Vector3(float x, float y, float z) : x(x), y(y), z(z) {}

    Vector3 operator+(const Vector3& v) const { return Vector3(x + v.x, y + v.y, z + v.z); }
    Vector3 operator-(const Vector3& v) const { return Vector3(x - v.x, y - v.y, z - v.z); }
    Vector3 operator*(float f) const { return Vector3(x * f, y * f, z * f); }

    float Distance(const Vector3& v) const {
        float dx = x - v.x, dy = y - v.y, dz = z - v.z;
        return sqrtf(dx * dx + dy * dy + dz * dz);
    }
};

class Address {
public:
    static Address* Get() {
        if (s_pSingleton == nullptr)
            s_pSingleton = new Address;
        return s_pSingleton;
    }

    void Load();

    template <typename T>
    inline T SafeRead(uintptr_t ptr, T defVal = T()) {
        if (ptr < 0x100000 || ptr > 0x7FFFFFFFFFFF) return defVal;
        __try { return *(T*)ptr; }
        __except (EXCEPTION_EXECUTE_HANDLER) { return defVal; }
    }

    inline bool IsValid(uintptr_t ptr) {
        return (ptr > 0x100000 && ptr < 0x7FFFFFFFFFFF);
    }

    typedef const char* (__fastcall* GetPlayerNameFn)(void*);
    GetPlayerNameFn m_GetPlayerName = nullptr;

    uintptr_t m_World = 0;
    uintptr_t m_ReplayInterface = 0;
    uintptr_t m_ViewPort = 0;
    uintptr_t m_CamGameplayDirector = 0; 
    uintptr_t m_HandleBullet = 0;         

    uintptr_t m_PedInterface = 0x18; 
    uintptr_t m_PedList = 0x100;     
    uintptr_t m_PedMaximum = 0x108;  
    uintptr_t m_VehicleInterface = 0x10;
    uintptr_t m_VehicleList = 0x180;
    uintptr_t m_VehicleMaximum = 0x188;

    uintptr_t m_EntityType = 0x10B8;
    uintptr_t m_PlayerInfo = 0x10C8;
    uintptr_t m_Armor = 0x1530;
    uintptr_t m_Health = 0x280;
    uintptr_t m_MaxHealth = 0x2A0;
    uintptr_t m_WeaponManager = 0x10D8;
    uintptr_t m_Recoil = 0x2F4;
    uintptr_t m_Spread = 0x84;
    uintptr_t m_Range = 0x28C;
    uintptr_t m_PlayerId = 0x88;
    uintptr_t m_BoneOffset = 0x430;
    uintptr_t m_VisualPos = 0x90;
    uintptr_t m_VisibleFlag = 0x145C;

    uintptr_t m_VehEngineHealth = 0x908;
    uintptr_t m_VehBodyHealth = 0x840;
    uintptr_t m_VehLockStatus = 0x138C;
    uintptr_t m_VehInvincible = 0x189; 
    uintptr_t m_VehModelInfo = 0x20;
    uintptr_t m_NetIdToNamesEntry = 0; 
    uintptr_t m_GetBonePos = 0;       

    int CurrentBuild = 0;

private:
    static Address* s_pSingleton;
    Address() {}
};

class scrNativeCallContext {
public:
    void* m_pReturn;
    uint32_t m_nArgCount;
    void* m_pArgs;
    uint32_t m_nDataCount;
    alignas(uintptr_t) uint8_t m_vectorSpace[192];

    template<typename T> inline T GetArgument(int idx) { return ((T*)m_pArgs)[idx]; }
    template<typename T> inline void SetResult(T value) { *(T*)m_pReturn = value; }
};

class NativeContext : public scrNativeCallContext {
    uint64_t m_TempStack[32];
public:
    inline NativeContext() {
        m_pArgs = &m_TempStack;
        m_pReturn = &m_TempStack;
        m_nArgCount = 0;
        m_nDataCount = 0;
        memset(m_TempStack, 0, sizeof(m_TempStack));
    }
    template <typename T> inline void Push(T value) {
        *(T*)(m_TempStack + m_nArgCount) = value;
        m_nArgCount++;
    }
    template <typename T> inline T GetResult() { return *(T*)m_pReturn; }
};

typedef void(__cdecl* NativeHandler)(scrNativeCallContext* context);
typedef NativeHandler(__fastcall* GetNativeHandlerFn)(uint64_t hash);

class Invoker {
public:
    template<typename R, typename... Args>
    static inline R Call(uint64_t hash, Args... args) {
        auto addr = Address::Get();
        static auto getHandler = (GetNativeHandlerFn)GetProcAddress(GetModuleHandleA("rage-scripting-five.dll"), "?GetNativeHandler@scrEngine@rage@@SAP6AXPEAVscrNativeCallContext@2@@Z_K@Z");
        if (!getHandler) return R();

        NativeHandler fn = getHandler(hash);
        if (!fn) return R();

        NativeContext ctx;
        (ctx.Push(args), ...);
        fn(&ctx);

        if constexpr (!std::is_void_v<R>) return ctx.GetResult<R>();
    }
};
