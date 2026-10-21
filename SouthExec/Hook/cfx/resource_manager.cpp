// Leaked By Cry Service & Siraj
#include "resource_manager.h"
#include "resource.h"
#include "../include/xorstr/xorstr.hpp"
#include "../include/pattern_scanner.h"
#include <Windows.h>

#include "../scanner/scanner.h"

namespace fx
{
    ResourceManager* ResourceManager::getCurrent(bool fb)
    {
        static void* resourceManager = nullptr;
        if (resourceManager)
            return (ResourceManager*)resourceManager;

        static uint64_t lastScanTime = 0;
        uint64_t now = GetTickCount64();
        if (now - lastScanTime < 5000) {
            return nullptr;
        }
        lastScanTime = now;

        const char* modules[] = {
            "citizen-resources-gta.dll",
            "citizen-resources-core.dll",
            "citizen-game-main.dll"
        };

        const char* patterns[] = {
            "48 8B 0D ? ? ? ? 48 85 C9 74 06 48 8B 01 FF",
            "48 8B 05 ? ? ? ? 48 85 C0 74 ? 48 8B 08",
            "48 8B 0D ? ? ? ? E8 ? ? ? ? 48 8B F8"
        };

        for (const char* modName : modules) {
            HMODULE mod = GetModuleHandleA(modName);
            if (!mod) continue;

            for (const char* pattern : patterns) {
                uintptr_t addr = scan(mod, pattern);
                if (!addr) continue;

                struct RefContainer { void* ptr; };
                auto* ref = reinterpret_cast<RefContainer*>(ResolveRelativeAddress(addr + 3, 4));
                if (ref && ref->ptr) {
                    resourceManager = ref->ptr;
                    break;
                }
            }
            if (resourceManager) break;
        }

        return (ResourceManager*)resourceManager;
    }

    ResourceManagerImpl* ResourceManager::getCurrentIl(bool fb)
    {
        ResourceManager* rm = getCurrent(fb);
        if (!rm) return nullptr;

        return (ResourceManagerImpl*)&((uint64_t*)rm)[2];
    }

    fwRefContainer<Resource> ResourceManagerImpl::gResByName(const std::string& name)
    {
        std::unique_lock<std::recursive_mutex> lock(m_resourcesMutex);

        auto it = m_resources.find(name);
        if (it != m_resources.end())
            return (fwRefContainer<Resource>)(it->second);

        return nullptr;
    }

    std::vector<fwRefContainer<Resource>> ResourceManagerImpl::getAllResources()
    {
        std::vector<fwRefContainer<Resource>> resources;

        try {
            for (auto& resource : m_resources) {
                if (resource.second.GetRef()) {
                    resources.push_back(resource.second);
                }
            }
        } catch (...) { }

        return resources;
    }
}
