#pragma once
#include <Windows.h>

struct Settings {
    bool b_MenuVisible = true;
    bool b_IsLoggedIn = false;
    int i_LoginStage = 0;
    char sz_Username[64] = "Cry-Service";
    char sz_Password[64] = "Cry-Service";
    bool b_IsRegistering = false;
    float f_SplashProgress = 0.0f;
};

extern Settings g_Settings;
