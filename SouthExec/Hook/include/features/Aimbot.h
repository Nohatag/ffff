// Leaked By Cry Service & Siraj
#pragma once
#include <windows.h>
#include <vector>
#include "../Address.h"
#include "../Core.h"

extern bool aim_Enable;
extern int aim_Key;
extern bool aim_ShowFov;
extern float col_aim_Fov[4];
extern float aim_Fov;
extern float aim_Smoothing;
extern int aim_BoneIdx;
extern bool aim_VisibleCheck;
extern bool aim_ExcludeDeads;
extern bool aim_TargetNpcs;
extern bool aim_TargetAdmins;

extern bool silent_Enable;
extern int silent_Key;
extern bool silent_MagicBullet;
extern bool silent_ShowFov;
extern float col_silent_Fov[4];
extern float silent_Fov;

namespace Aimbot {
    void Tick();
}
