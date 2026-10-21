// Leaked By Cry Service & Siraj
#include "component.h"
#include "../include/xorstr/xorstr.hpp"
#include "../include/pattern_scanner.h"
#include <Windows.h>

namespace fx
{
    ComponentRegistryImpl* ComponentRegistry::get_impl()
    {
        static uintptr_t funcAddr = 0;
        if (!funcAddr)
        {

            const char* modules[] = { "CoreRT.dll", "citizen-resources-core.dll", "citizen-scripting-lua.dll", "FiveM_GTAProcess.exe", "FiveM_Process.exe" };

            for (const char* modName : modules) {
                HMODULE hMod = GetModuleHandleA(modName);
                if (!hMod) continue;

                const char* mangledNames[] = {
                    "?get_impl@ComponentRegistry@fx@@SAPEAVComponentRegistryImpl@2@XZ",
                    "?get_impl@ComponentRegistry@fx@@SAPEAV12@XZ",
                    "?Instance@ComponentRegistry@fx@@SAPEAV12@XZ",
                    "?Instance@?$Singleton@VComponentRegistry@fx@@@fx@@SAPEAVComponentRegistry@2@XZ"
                };

                for (const char* name : mangledNames) {
                    funcAddr = (uintptr_t)GetProcAddress(hMod, name);
                    if (funcAddr) break;
                }
                if (funcAddr) break;

                const char* patterns[] = {
                    "48 83 EC 28 48 8B 05 ? ? ? ? 48 85 C0 74 11",
                    "48 8B 05 ? ? ? ? 48 85 C0 74 ? C3",
                    "48 8D 0D ? ? ? ? E8 ? ? ? ? 48 8B D8 48 85 C0 74 2E",
                    "48 8B 05 ? ? ? ? 48 8B F1 48 85 C0 74 ? 48 8B 00",
                    "48 83 EC 28 48 8D 0D ? ? ? ? E8 ? ? ? ? 48 8B D8",
                    "E8 ? ? ? ? 48 8B 0D ? ? ? ? 48 85 C9 74 20"
                };

                for (const char* pattern : patterns) {
                    funcAddr = PatternScanner::FindPattern((uintptr_t)hMod, pattern);
                    if (funcAddr) break;
                }
                if (funcAddr) break;
            }
        }

        if (!funcAddr) return nullptr;

        auto CoreGetComponentRegistry = reinterpret_cast<ComponentRegistryImpl* (*)()>(funcAddr);
        return CoreGetComponentRegistry();
    }
}
