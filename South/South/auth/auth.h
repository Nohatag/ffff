#pragma once
#include <windows.h>
#include <wininet.h>
#include <iostream>
#include <string>
#include <fstream>
#include <filesystem>
#include <ShlObj.h>

#pragma comment(lib, "wininet.lib")

namespace AuthSystem {
    inline std::string GetHWID() {
        HW_PROFILE_INFOA hwProfileInfo;
        if (GetCurrentHwProfileA(&hwProfileInfo)) {
            std::string guid = hwProfileInfo.szHwProfileGuid;
            // Remove braces if present
            if (!guid.empty() && guid.front() == '{') guid = guid.substr(1);
            if (!guid.empty() && guid.back() == '}') guid.pop_back();
            return guid;
        }
        return "unknown";
    }

    inline std::string GetKeyPath() {
        // Tenta salvar em %APPDATA%/South/key.txt, fallback para ./key.txt
        wchar_t path[MAX_PATH] = {0};
        std::filesystem::path p;
        if (SHGetFolderPathW(NULL, CSIDL_APPDATA, NULL, 0, path) == S_OK) {
            p = std::filesystem::path(path) / L"South" / L"key.txt";
        } else {
            p = std::filesystem::path(L"key.txt");
        }
        // Return as string (narrow) - use wide to handle unicode, but convert
        std::string s = p.string();
        return s;
    }

    inline std::filesystem::path GetKeyPathW() {
        wchar_t path[MAX_PATH] = {0};
        if (SHGetFolderPathW(NULL, CSIDL_APPDATA, NULL, 0, path) == S_OK) {
            auto p = std::filesystem::path(path) / L"South" / L"key.txt";
            std::error_code ec;
            std::filesystem::create_directories(p.parent_path(), ec);
            return p;
        }
        auto p = std::filesystem::path(L"key.txt");
        return p;
    }

    inline void SaveKey(const std::string& key) { (void)key; }
    inline std::string LoadKey() { return ""; }

    inline bool Authenticate(std::string key, std::string& out_message) {
        // Bypass auth - sempre sucesso
        out_message = "OK";
        return true;
    }
}
