// Leaked By Cry Service & Siraj
#pragma once
#include <windows.h>
#include "../imgui/imgui.h"
#include "Address.h"

struct Matrix4x4 {
    union {
        struct {
            float _11, _12, _13, _14;
            float _21, _22, _23, _24;
            float _31, _32, _33, _34;
            float _41, _42, _43, _44;
        };
        float m[4][4];
    };

    void Transpose() {
        Matrix4x4 res;
        for (int i = 0; i < 4; i++)
            for (int j = 0; j < 4; j++)
                res.m[i][j] = m[j][i];
        *this = res;
    }
};

class Core {
public:
    static Core* Get() {
        if (s_pSingleton == nullptr)
            s_pSingleton = new Core;
        return s_pSingleton;
    }

    ImVec2 W2S(Vector3 pos);
    Vector3 BoneVec(const uintptr_t cPed, int boneid);
    Vector3 GetBone(const uintptr_t cPed, int boneid);

private:
    static Core* s_pSingleton;
    Core() {}
};
