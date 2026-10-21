// Leaked By Cry Service & Siraj
#pragma once
#include "Address.h"
#include <Psapi.h>
#include <tlhelp32.h>
#include <regex>
#include "Scanner.h"

Address* Address::s_pSingleton = nullptr;

namespace Utils {
    inline std::pair<DWORD, std::wstring> GetProcessPidByName(const std::wstring& ProcessNamePattern) {
        HANDLE hSnapshot = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
        if (!hSnapshot || hSnapshot == INVALID_HANDLE_VALUE) return { 0, L"" };

        DWORD Pid = 0;
        std::wstring processName;
        PROCESSENTRY32W ProcessEntry;
        ProcessEntry.dwSize = sizeof(ProcessEntry);

        std::wregex pattern(ProcessNamePattern);
        if (Process32FirstW(hSnapshot, &ProcessEntry)) {
            do {
                std::wstring exeFile(ProcessEntry.szExeFile);
                if (std::regex_match(exeFile, pattern)) {
                    Pid = ProcessEntry.th32ProcessID;
                    processName = exeFile;
                    break;
                }
            } while (Process32NextW(hSnapshot, &ProcessEntry));
        }

        CloseHandle(hSnapshot);
        return { Pid, processName };
    }

    inline int ExtractBuildNumber(const std::wstring& processName) {
        std::wregex buildRegex(L"FiveM_b(\\d{4})_");
        std::wsmatch match;
        if (std::regex_search(processName, match, buildRegex) && match.size() > 1) {
            return std::stoi(match[1].str());
        }
        return -1;
    }

    static inline bool SafeReadDate(uintptr_t dateAddr, char* outDate) {
        __try {
            int32_t rel = *(int32_t*)dateAddr;
            if (rel != 0) {
                char* strPtr = (char*)(dateAddr + rel + 4);
                for (int i = 0; i < 24; ++i) {
                    outDate[i] = strPtr[i];
                    if (strPtr[i] == '\0') break;
                }
                return true;
            }
        } __except (EXCEPTION_EXECUTE_HANDLER) {
            return false;
        }
        return false;
    }

    inline int GetBuildFromDate(uintptr_t image_base) {
        uintptr_t dateAddr = image_base + 0x13ee1a;
        char tempDate[32] = { 0 };

        if (SafeReadDate(dateAddr, tempDate)) {
            std::string buildString(tempDate);
            static const std::unordered_map<std::string, int> buildMap = {
                {"Dec  5 2024", 3407},
                {"Sep 10 2024", 3323},
                {"Jun 20 2024", 3258},
                {"Dec  8 2023", 3095},
                {"Jun  8 2023", 2944},
                {"Dec  8 2022", 2802},
                {"Jul 21 2022", 2699},
                {"Apr 18 2022", 2612},
                {"Dec 14 2021", 2545},
                {"Jul 15 2021", 2372},
                {"Dec 10 2020", 2189},
                {"Aug  5 2020", 2060},
                {"Dec 11 2019", 1868},
                {"Jul 23 2019", 1737},
                {"Dec  5 2018", 1604},
                {"Oct 14 2018", 1493},
                {"Mar 14 2018", 1365},
                {"Dec  6 2017", 1290},
                {"Jun  9 2017", 1103},
                {"Mar 31 2017", 1032},
                {"Oct 13", 505},
                {"Jun 30", 393},
                {"Jun  9 2015", 372}
            };

            auto it = buildMap.find(buildString);
            if (it != buildMap.end()) return it->second;
        }
        return -1;
    }

    uintptr_t FindNameList(uintptr_t image_base) {
        __try {

            return Scanner::Get()->Scan(NULL, "48 8B 05 ?? ?? ?? ?? 48 8B 0C C8 48 85 C9 74 30", 0, 7);
        } __except (EXCEPTION_EXECUTE_HANDLER) {}
        return 0;
    }

    uintptr_t FindBonePos(uintptr_t image_base) {
        __try {
            const unsigned char pattern[] = { 0x48, 0x89, 0x5C, 0x24, 0x00, 0x48, 0x89, 0x6C, 0x24, 0x00, 0x48, 0x89, 0x74, 0x24, 0x00, 0x57, 0x48, 0x83, 0xEC, 0x60, 0x48, 0x8B, 0x01, 0x41, 0x8B, 0xE8, 0x48, 0x8B, 0xF2, 0x48, 0x8B, 0xF9, 0x33, 0xDB };
            const char mask[] = "xxxx?xxxx?xxxx?xxxxxxxxxxxxxxxxxxx";

            for (uintptr_t i = 0x100000; i < 0x2000000; i++) {
                bool found = true;
                for (int k = 0; k < 34; k++) {
                    if (mask[k] == 'x' && *(unsigned char*)(image_base + i + k) != pattern[k]) {
                        found = false; break;
                    }
                }
                if (found) return image_base + i;
            }
        } __except (EXCEPTION_EXECUTE_HANDLER) {}
        return 0;
    }
    uintptr_t PatternScan(uintptr_t moduleBase, const char* signature) {
        static auto patternToByte = [](const char* pattern) {
            auto bytes = std::vector<int>{};
            auto start = const_cast<char*>(pattern);
            auto end = const_cast<char*>(pattern) + strlen(pattern);

            for (auto curr = start; curr < end; ++curr) {
                if (*curr == '?') {
                    ++curr;
                    if (*curr == '?') ++curr;
                    bytes.push_back(-1);
                }
                else {
                    bytes.push_back(strtoul(curr, &curr, 16));
                }
            }
            return bytes;
        };

        auto dosHeader = (PIMAGE_DOS_HEADER)moduleBase;
        auto ntHeaders = (PIMAGE_NT_HEADERS)((std::uint8_t*)moduleBase + dosHeader->e_lfanew);

        auto sizeOfImage = ntHeaders->OptionalHeader.SizeOfImage;
        auto patternBytes = patternToByte(signature);
        auto scanBytes = reinterpret_cast<std::uint8_t*>(moduleBase);

        auto s = patternBytes.size();
        auto d = patternBytes.data();

        for (auto i = 0ul; i < sizeOfImage - s; ++i) {
            bool found = true;
            for (auto j = 0ul; j < s; ++j) {
                if (scanBytes[i + j] != d[j] && d[j] != -1) {
                    found = false;
                    break;
                }
            }
            if (found) {
                return (uintptr_t)&scanBytes[i];
            }
        }
        return 0;
    }
}

void Address::Load() {
    uintptr_t image_base = (uintptr_t)GetModuleHandleA(NULL);
    if (!image_base) return;

    int build = -1;
    char exePath[MAX_PATH] = { 0 };
    DWORD len = GetModuleFileNameA(NULL, exePath, MAX_PATH);
    if (len > 0) {
        std::string exeName(exePath);
        size_t pos = exeName.find_last_of("\\/");
        if (pos != std::string::npos) exeName = exeName.substr(pos + 1);
        size_t bPos = exeName.find("_b");
        if (bPos != std::string::npos) {
            try { build = std::stoi(exeName.substr(bPos + 2)); } catch (...) {}
        }
    }

    if (build == -1) {
        build = Utils::GetBuildFromDate(image_base);
    }

    if (build == -1) build = 3258;
    this->CurrentBuild = build;

    this->m_GetPlayerName = (GetPlayerNameFn)Scanner::Get()->Scan("citizen-playernames-five.dll", "40 57 48 83 EC ? 48 8B 05", 0, 0);

    this->m_NetIdToNamesEntry = Utils::FindNameList(image_base);
    this->m_GetBonePos = Utils::FindBonePos(image_base);

    switch (build) {
    case 3407:
        m_World = image_base + 0x25D7108;
        m_ReplayInterface = image_base + 0x1F9A9D8;
        m_ViewPort = image_base + 0x20431C0;
        m_CamGameplayDirector = image_base + 0x2043DF8;
        m_HandleBullet = image_base + 0x102FF8C;
        m_MaxHealth = 0x284;
        m_Health = 0x280;
        m_WeaponManager = 0x10B8;
        m_EntityType = 0x1098;
        m_PlayerInfo = 0x10A8;
        m_PlayerId = 0xE8;
        m_Armor = 0x150C;
        m_BoneOffset = 0x430;
        m_VehicleInterface = 0x10;
        m_VehicleList = 0x180;
        m_VehicleMaximum = 0x188;
        m_VehBodyHealth = 0x840;
        m_VehEngineHealth = 0x908;
        m_VehLockStatus = 0x13C0;
        break;
    case 3323:
        m_World = image_base + 0x25C15B0;
        m_ReplayInterface = image_base + 0x1F85458;
        m_ViewPort = image_base + 0x202DC50;
        m_CamGameplayDirector = image_base + 0x202EB48;
        m_MaxHealth = 0x284;
        m_Health = 0x280;
        m_WeaponManager = 0x10B8;
        m_EntityType = 0x1098;
        m_PlayerInfo = 0x10A8;
        m_PlayerId = 0xE8;
        m_Armor = 0x150C;
        m_BoneOffset = 0x430;
        m_VehicleInterface = 0x10;
        m_VehicleList = 0x180;
        m_VehicleMaximum = 0x188;
        m_VehBodyHealth = 0x840;
        m_VehEngineHealth = 0x908;
        m_VehLockStatus = 0x13C0;
        break;
    case 3571:
    case 3751:
        m_World = image_base + 0x2603908;
        m_ReplayInterface = image_base + 0x1FC38A8;
        m_ViewPort = image_base + 0x206C060;
        m_CamGameplayDirector = image_base + 0x206D1C0;
        m_MaxHealth = 0x284;
        m_Health = 0x280;
        m_WeaponManager = 0x10B8;
        m_EntityType = 0x1098;
        m_PlayerInfo = 0x10A8;
        m_PlayerId = 0xE8;
        m_Armor = 0x150C;
        m_BoneOffset = 0x430;
        m_VehicleInterface = 0x10;
        m_VehicleList = 0x180;
        m_VehicleMaximum = 0x188;
        m_VehBodyHealth = 0x840;
        m_VehEngineHealth = 0x908;
        m_VehLockStatus = 0x13C0;
        break;
    case 3788:
        m_World = image_base + 0x26068E0;
        m_ReplayInterface = image_base + 0x1FC68A8;
        m_ViewPort = image_base + 0x206F060;
        m_CamGameplayDirector = image_base + 0x20701C0;
        m_HandleBullet = image_base + 0x42F5F86B;
        m_MaxHealth = 0x284;
        m_Health = 0x280;
        m_WeaponManager = 0x10B8;
        m_EntityType = 0x1098;
        m_PlayerInfo = 0x10A8;
        m_PlayerId = 0xE8;
        m_Armor = 0x150C;
        m_BoneOffset = 0x430;
        m_VehicleInterface = 0x10;
        m_VehicleList = 0x180;
        m_VehicleMaximum = 0x188;
        m_VehBodyHealth = 0x840;
        m_VehEngineHealth = 0x908;
        m_VehLockStatus = 0x13C0;
        break;
    case 3570:
        m_World = image_base + 0x25EC580;
        m_ReplayInterface = image_base + 0x1FB0418;
        m_ViewPort = image_base + 0x2058BA0;
        m_CamGameplayDirector = image_base + 0x2059778;
        m_HandleBullet = image_base + 0x102D550;
        m_MaxHealth = 0x284;
        m_Health = 0x280;
        m_WeaponManager = 0x10B8;
        m_EntityType = 0x1098;
        m_PlayerInfo = 0x10A8;
        m_PlayerId = 0xE8;
        m_Armor = 0x150C;
        m_BoneOffset = 0x430;
        m_VehicleInterface = 0x10;
        m_VehicleList = 0x180;
        m_VehicleMaximum = 0x188;
        m_VehBodyHealth = 0x840;
        m_VehEngineHealth = 0x908;
        m_VehLockStatus = 0x13C0;
        break;
    case 3258:
        m_World = image_base + 0x25B14B0;
        m_ReplayInterface = image_base + 0x1FBD4F0;
        m_ViewPort = image_base + 0x201DBA0;
        m_CamGameplayDirector = image_base + 0x201ED50;
        m_HandleBullet = image_base + 0x1026CB0;
        m_MaxHealth = 0x284;
        m_Health = 0x280;
        m_WeaponManager = 0x10B8;
        m_Recoil = 0x2F4;
        m_Spread = 0x84;
        m_Range = 0x28C;
        m_EntityType = 0x1098;
        m_PlayerInfo = 0x10A8;
        m_PlayerId = 0xE8;
        m_Armor = 0x150C;
        m_BoneOffset = 0x430;
        m_VehicleInterface = 0x10;
        m_VehicleList = 0x180;
        m_VehicleMaximum = 0x188;
        m_VehBodyHealth = 0x840;
        m_VehEngineHealth = 0x908;
        break;
    case 3095:
        m_World = image_base + 0x2593320;
        m_ReplayInterface = image_base + 0x1F58B58;
        m_ViewPort = image_base + 0x20019E0;
        m_MaxHealth = 0x284;
        m_Health = 0x280;
        m_WeaponManager = 0x10B8;
        m_EntityType = 0x1098;
        m_PlayerInfo = 0x10A8;
        m_PlayerId = 0xE8;
        m_Armor = 0x150C;
        m_BoneOffset = 0x430;
        break;
    case 2944:
        m_World = image_base + 0x257BEA0;
        m_ReplayInterface = image_base + 0x1F42218;
        m_ViewPort = image_base + 0x1FEAAC0;
        m_MaxHealth = 0x284;
        m_Health = 0x280;
        m_WeaponManager = 0x10B8;
        m_EntityType = 0x1098;
        m_PlayerInfo = 0x10A8;
        m_PlayerId = 0xE8;
        m_Armor = 0x150C;
        m_BoneOffset = 0x410;
        break;
    case 2802:
        m_World = image_base + 0x254D448;
        m_ReplayInterface = image_base + 0x1F30578;
        m_ViewPort = image_base + 0x1FBC100;
        m_MaxHealth = 0x2A0;
        m_Health = 0x280;
        m_WeaponManager = 0x10B8;
        m_EntityType = 0x1098;
        m_PlayerInfo = 0x10A8;
        m_PlayerId = 0x88;
        m_Armor = 0x150C;
        m_BoneOffset = 0x410;
        break;
    case 2612:
        m_World = image_base + 0x2567DB0;
        m_ReplayInterface = image_base + 0x1F77EF0;
        m_ViewPort = image_base + 0x1FD8570;
        m_CamGameplayDirector = image_base + 0x1FD9148;
        m_MaxHealth = 0x2A0;
        m_Health = 0x280;
        m_WeaponManager = 0x10D8;
        m_EntityType = 0x10B8;
        m_PlayerInfo = 0x10C8;
        m_PlayerId = 0x88;
        m_Armor = 0x1530;
        m_BoneOffset = 0x1450;
        m_VisibleFlag = 0x142C;
        m_VehicleInterface = 0x10;
        m_VehicleList = 0x180;
        m_VehicleMaximum = 0x188;
        m_VehBodyHealth = 0x840;
        m_VehEngineHealth = 0x844;
        m_VehLockStatus = 0x1390;
        m_Recoil = 0x2F4;
        m_Spread = 0x84;
        m_Range = 0x28C;
        break;
    case 1604:
        m_World = image_base + 0x247F840;
        m_ReplayInterface = image_base + 0x1EC29F0;
        m_ViewPort = image_base + 0x2087780;
        m_MaxHealth = 0x2A0;
        m_Health = 0x280;
        m_WeaponManager = 0x10C8;
        m_EntityType = 0x10A8;
        m_PlayerInfo = 0x10B8;
        m_PlayerId = 0x60;
        m_Armor = 0x14B8;
        m_BoneOffset = 0x430;
        break;
    default:
        m_World = image_base + 0x25B14B0;
        m_ReplayInterface = image_base + 0x1FBD4F0;
        m_ViewPort = image_base + 0x201DBA0;
        m_MaxHealth = 0x284;
        m_Health = 0x280;
        m_WeaponManager = 0x10B8;
        m_EntityType = 0x1098;
        m_PlayerInfo = 0x10A8;
        m_PlayerId = 0xE8;
        m_Armor = 0x150C;
        m_BoneOffset = 0x430;
        break;
    }

}
