#pragma once
#include <Windows.h>
#include <string>
#include <vector>
#include <filesystem>
#include <thread>
#include <functional>
#include <iostream>
#include "../UI/LoaderNotification.hpp"

namespace fs = std::filesystem;

class HardwareCleaner {
public:
    static int CleanDirectory(const fs::path& dirPath, bool recursive = true) {
        int count = 0;
        try {
            if (!fs::exists(dirPath)) return 0;
            for (const auto& entry : fs::directory_iterator(dirPath)) {
                try {
                    if (fs::is_directory(entry.status()) && recursive) {
                        count += CleanDirectory(entry.path(), true);
                        fs::remove(entry.path());
                        count++;
                    } else if (fs::is_regular_file(entry.status())) {
                        if (fs::remove(entry.path())) {
                            count++;
                        }
                    }
                } catch (...) {
                    // Ignora arquivos bloqueados em uso
                }
            }
        } catch (...) {}
        return count;
    }

    static void FlushNetworkCache() {
        // 1. Tenta DnsFlushResolverCache via dnsapi.dll
        HMODULE hDns = LoadLibraryA("dnsapi.dll");
        if (hDns) {
            typedef BOOL(WINAPI* PDnsFlushResolverCache)();
            PDnsFlushResolverCache pfnFlush = (PDnsFlushResolverCache)GetProcAddress(hDns, "DnsFlushResolverCache");
            if (pfnFlush) {
                pfnFlush();
            }
            FreeLibrary(hDns);
        }

        // 2. Executa ipconfig /flushdns e arp de forma totalmente oculta (CREATE_NO_WINDOW)
        STARTUPINFOA si = { sizeof(si) };
        si.dwFlags = STARTF_USESHOWWINDOW;
        si.wShowWindow = SW_HIDE;
        PROCESS_INFORMATION pi = { 0 };

        char cmd1[] = "cmd.exe /c ipconfig /flushdns";
        if (CreateProcessA(NULL, cmd1, NULL, NULL, FALSE, CREATE_NO_WINDOW, NULL, NULL, &si, &pi)) {
            WaitForSingleObject(pi.hProcess, 2000);
            CloseHandle(pi.hProcess);
            CloseHandle(pi.hThread);
        }

        char cmd2[] = "cmd.exe /c arp -d *";
        if (CreateProcessA(NULL, cmd2, NULL, NULL, FALSE, CREATE_NO_WINDOW, NULL, NULL, &si, &pi)) {
            WaitForSingleObject(pi.hProcess, 1500);
            CloseHandle(pi.hProcess);
            CloseHandle(pi.hThread);
        }
    }

    static bool ExecuteClean(int& outFilesCleaned) {
        int files = 0;

        // Fase 1: Network flush
        FlushNetworkCache();

        // Fase 2: Windows Temp & Crash Dumps
        char tempPath[MAX_PATH] = { 0 };
        if (GetTempPathA(MAX_PATH, tempPath)) {
            files += CleanDirectory(fs::path(tempPath), false);
        }

        char* localAppData = nullptr;
        size_t len = 0;
        if (_dupenv_s(&localAppData, &len, "LOCALAPPDATA") == 0 && localAppData) {
            fs::path localPath(localAppData);
            free(localAppData);

            // Crash Dumps do WER
            fs::path crashDumps = localPath / "CrashDumps";
            files += CleanDirectory(crashDumps, true);

            // FiveM / CFX Caches
            fs::path fivemPath = localPath / "FiveM" / "FiveM.app";
            if (fs::exists(fivemPath)) {
                files += CleanDirectory(fivemPath / "data" / "cache", true);
                files += CleanDirectory(fivemPath / "data" / "server-cache", true);
                files += CleanDirectory(fivemPath / "data" / "server-cache-priv", true);
                files += CleanDirectory(fivemPath / "data" / "nui-storage", true);
                files += CleanDirectory(fivemPath / "crashes", true);
                files += CleanDirectory(fivemPath / "logs", true);
            }
        }

        char* appData = nullptr;
        if (_dupenv_s(&appData, &len, "APPDATA") == 0 && appData) {
            fs::path ropPath(appData);
            free(appData);

            fs::path cfxPath = ropPath / "CitizenFX";
            if (fs::exists(cfxPath)) {
                files += CleanDirectory(cfxPath, false);
            }
        }

        outFilesCleaned = files;
        return true;
    }

    static void ExecuteAsync(std::function<void(bool success, int filesRemoved)> onComplete = nullptr) {
        std::thread([onComplete]() {
            LoaderNotification::Show("Cleaner", "Hardware Cleaner in progress...", 2.5f);
            std::this_thread::sleep_for(std::chrono::milliseconds(1200));

            int cleaned = 0;
            bool ok = ExecuteClean(cleaned);

            std::this_thread::sleep_for(std::chrono::milliseconds(800));
            if (onComplete) {
                onComplete(ok, cleaned);
            }
        }).detach();
    }
};
