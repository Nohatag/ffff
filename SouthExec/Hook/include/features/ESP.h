// Leaked By Cry Service & Siraj
#pragma once
#include <windows.h>
#include <string>
#include "../Address.h"
#include "../Core.h"
#include "../Graphics.h"
#include <vector>

extern bool esp_Enable;
extern bool esp_Box;
extern bool esp_Names;
extern bool esp_Weapons;
extern bool esp_Head;
extern bool esp_Skeleton;
extern bool esp_Healthbar;
extern bool esp_Armorbar;
extern bool esp_Lines;
extern float esp_Distance;
extern bool esp_InfDistance;
extern bool esp_ShowDistance;
extern bool esp_TargetNpcs;
extern bool esp_Self;
extern float esp_TextSize;

extern bool admin_List;
extern bool admin_Lines;
extern bool admin_Skeleton;

extern bool veh_Enable;
extern bool veh_Name;
extern bool veh_Marker;
extern bool veh_LockState;
extern bool veh_Lines;
extern float veh_Distance;
extern bool veh_InfDistance;
extern bool veh_ShowDistance;
extern float veh_TextSize;

extern float col_Skeleton[4];
extern float col_Box[4];

extern bool wep_ShowCrosshair;

struct AdminData {
    char name[64];
    float distance;
};

struct PlayerListData {
    char name[64];
    float distance;
    bool isAdmin;
    int serverId;
    uintptr_t ped;
};

struct VehicleListData {
    uintptr_t vehicle;
    char name[64];
    float distance;
    Vector3 pos;
};

extern SRWLOCK g_PlayerListLock;
extern SRWLOCK g_VehicleListLock;

struct SRWLockGuard {
    SRWLOCK* lock;
    SRWLockGuard(SRWLOCK* l) : lock(l) { AcquireSRWLockExclusive(lock); }
    ~SRWLockGuard() { ReleaseSRWLockExclusive(lock); }
};

extern std::vector<AdminData> g_AdminList;
extern std::vector<PlayerListData> g_PlayerList;
extern std::vector<VehicleListData> g_VehicleList;

class ESP {
public:
    static void Render();
    static void DrawEntityESP(uintptr_t entity, Vector3 localPos, bool isAdmin = false);
    static void DrawVehicleESP(uintptr_t vehicle, Vector3 localPos);
    static void GetPlayerName(uintptr_t ped, char* outBuf, int bufLen);
    static void GetWeaponName(uintptr_t ped, char* outBuf, int bufLen);
    static bool IsPlayer(uintptr_t ped);
    static int GetServerIdInternal(uintptr_t ped);
private:
    static void SyncPlayerList(Vector3 localPos);
    static bool GetPlayerNameFromMemory(int serverId, char* outBuf, int bufLen);
};
