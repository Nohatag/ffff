// Leaked By Cry Service & Siraj
#include "api-south.h"
#include <fstream>
#include <algorithm>
#include <random>
#include <iostream>
#include <vector>
#include <string>
#include <unordered_map>
#include <cctype>
#include "../../scanner/scanner.h"
#include "../../Hook/kiero/minhook/include/MinHook.h"
#include "../../Hook/cfx/resource_manager.h"
#include "../../Hook/cfx/resource.h"

std::string g_overrideResourceName;

extern int selectedRuntimeIndex;
extern _LuaClasses::LuaScriptRuntime* selectedRuntime;

#define LUA_STATE_OFFSET 0x58
#define RES_HOST_OFFSET 0x78

inline _LuaClasses::LuaScriptRuntimeHolder* g_currentLuaRuntime;

lua_pcallk_t Original_lua_pcallk = nullptr;

static CRITICAL_SECTION g_RuntimesMutex;
static bool g_RuntimesMutexInitialized = []() {
    InitializeCriticalSection(&g_RuntimesMutex);
    return true;
}();

void LockRuntimes() {
    if (g_RuntimesMutexInitialized) {
        EnterCriticalSection(&g_RuntimesMutex);
    }
}

void UnlockRuntimes() {
    if (g_RuntimesMutexInitialized) {
        LeaveCriticalSection(&g_RuntimesMutex);
    }
}

namespace {
    bool IsReadableMemory(const void* ptr, size_t size);
}

extern void PushNotification(std::string title, std::string msg, float duration = 3.0f);
static bool g_ExecutorActivatedNotificationSent = false;

static void SafeRegisterRuntime(_LuaClasses::LuaScriptRuntime* rt)
{
    LockRuntimes();
    auto& runtimes = _LuaGlobals::_AvailableRuntimes;
    bool found = (std::find(runtimes.begin(), runtimes.end(), rt) != runtimes.end());
    if (!found)
    {
        runtimes.push_back(rt);
    }
    UnlockRuntimes();

    if (!found)
    {
        auto* host = rt->GetResourceHost();
        if (host) {
            char* rName = host->GetResourceName();
            if (rName) {
                std::string name(rName);
                std::string lower = name;
                std::transform(lower.begin(), lower.end(), lower.begin(), [](unsigned char c) { return std::tolower(c); });
                if ((lower == "spawnmanager" || lower == "monitor") && !g_ExecutorActivatedNotificationSent) {
                    g_ExecutorActivatedNotificationSent = true;
                    PushNotification("Executor", name + " detected! Executor is now fully active.", 4.0f);
                }
            }
        }
    }
}

__int64 __fastcall Detour_lua_pcallk(__int64* L, int nargs, int nresults, int errfunc, __int64 ctx, __int64* k)
{
    if (g_currentLuaRuntime && IsReadableMemory(g_currentLuaRuntime, sizeof(void*)))
    {
        _LuaClasses::LuaScriptRuntime* rt = g_currentLuaRuntime->GetLuaScriptRuntime();
        if (rt && IsReadableMemory(rt, 0x100))
        {
            LockRuntimes();
            auto& runtimes = _LuaGlobals::_AvailableRuntimes;
            if (std::find(runtimes.begin(), runtimes.end(), rt) == runtimes.end())
            {
                runtimes.push_back(rt);
                UnlockRuntimes();

                auto* host = rt->GetResourceHost();
                std::string name = host ? host->GetResourceNameStr() : "";
                std::string lower = name;
                std::transform(lower.begin(), lower.end(), lower.begin(), [](unsigned char c) { return std::tolower(c); });
                if ((lower == "spawnmanager" || lower == "monitor") && !g_ExecutorActivatedNotificationSent) {
                    g_ExecutorActivatedNotificationSent = true;
                    PushNotification("Executor", name + " detected! Executor is now fully active.", 4.0f);
                }
            }
            else
            {
                UnlockRuntimes();
            }
        }
    }
    return Original_lua_pcallk ? Original_lua_pcallk(L, nargs, nresults, errfunc, ctx, k) : 0;
}

std::vector<_LuaClasses::LuaScriptRuntime*> _LuaGlobals::_AvailableRuntimes;
std::vector<ResourceEntry> _LuaGlobals::_ServerResources;

bool g_runtimeFound = false;

namespace _LuaFunctions {
    lua_pcallk_t        lua_pcallk = nullptr;
    luaL_loadfilex_t    luaL_loadfilex = nullptr;
    luaL_loadbufferx_t  luaL_loadbufferx = nullptr;
    lua_getglobal_t     lua_getglobal = nullptr;
    lua_tolstring_t     lua_tolstring = nullptr;
    lua_gettop_t        lua_gettop = nullptr;
    lua_settop_t        lua_settop = nullptr;
    luaL_newstate_t     luaL_newstate = nullptr;
    lua_close_t         lua_close = nullptr;
    luaL_openlibs_t     luaL_openlibs = nullptr;
}

namespace {

constexpr char kResourceDelimiter = '\x1e';
static int g_cachedResourcesMapOffset = -1;

bool IsPlausibleResourceName(const std::string& name)
{
    if (name.empty() || name.size() > 96)
        return false;

    for (char c : name) {
        if (std::isalnum(static_cast<unsigned char>(c)) || c == '_' || c == '-' || c == '.' || c == '[' || c == ']' || c == '(' || c == ')' || c == ' ' || c == '+')
            continue;
        return false;
    }
    return true;
}

bool IsReadableMemory(const void* ptr, size_t size)
{
    if (!ptr || !size)
        return false;

    MEMORY_BASIC_INFORMATION mbi{};
    if (!SouthVirtualQuery(ptr, &mbi, sizeof(mbi)))
        return false;

    if (mbi.State != MEM_COMMIT)
        return false;

    if (mbi.Protect & (PAGE_NOACCESS | PAGE_GUARD))
        return false;

    const auto addr = reinterpret_cast<uintptr_t>(ptr);
    const auto regionEnd = reinterpret_cast<uintptr_t>(mbi.BaseAddress) + mbi.RegionSize;
    return addr + size <= regionEnd;
}

bool ReadMsvcString(uintptr_t strObj, std::string& out)
{
    if (!strObj || !IsReadableMemory(reinterpret_cast<void*>(strObj), 0x20))
        return false;

    const size_t size = *reinterpret_cast<size_t*>(strObj + 0x10);
    if (size == 0 || size > 96)
        return false;

    const char* data = size <= 15
        ? reinterpret_cast<const char*>(strObj)
        : *reinterpret_cast<const char**>(strObj);

    if (!data || !IsReadableMemory(data, size))
        return false;

    out.assign(data, size);
    return IsPlausibleResourceName(out);
}

bool ReadPointer(uintptr_t address, uintptr_t& out)
{
    if (!address || !IsReadableMemory(reinterpret_cast<void*>(address), sizeof(uintptr_t)))
        return false;

    out = *reinterpret_cast<uintptr_t*>(address);
    return out != 0;
}

void* GetResourceManager()
{
    static void* resourceManager = nullptr;
    if (resourceManager)
        return resourceManager;

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
        if (!mod)
            continue;

        for (const char* pattern : patterns) {
            uintptr_t addr = scan(mod, pattern);
            if (!addr)
                continue;

            struct RefContainer { void* ptr; };
            auto* ref = reinterpret_cast<RefContainer*>(ResolveRelativeAddress(addr + 3, 4));
            if (ref && ref->ptr) {
                resourceManager = ref->ptr;
                return resourceManager;
            }
        }
    }

    return nullptr;
}

struct ManagerResourceRow {
    std::string name;
    void* fxResource = nullptr;
};

int TryReadResourceState(void* fxResource)
{
    if (!fxResource)
        return -1;

    const uintptr_t base = reinterpret_cast<uintptr_t>(fxResource);
    for (size_t off = 0x30; off <= 0xC0; off += 0x4) {
        if (!IsReadableMemory(reinterpret_cast<void*>(base + off), sizeof(int)))
            continue;

        const int state = *reinterpret_cast<int*>(base + off);
        if (state >= 0 && state <= 5)
            return state;
    }
    return -1;
}

bool TryReadResourceFromNode(uintptr_t node, void*& fxResource)
{
    fxResource = nullptr;
    static const int kValueOffsets[] = { 0x30, 0x38, 0x40, 0x28 };
    for (int valueOff : kValueOffsets) {
        uintptr_t ref = 0;
        if (!ReadPointer(node + valueOff, ref))
            continue;
        if (ref > 0x10000 && IsReadableMemory(reinterpret_cast<void*>(ref), 8)) {
            fxResource = reinterpret_cast<void*>(ref);
            return true;
        }
    }
    return false;
}

int WalkUmapNodes(uintptr_t hashPtr, std::vector<ManagerResourceRow>& rows)
{
    rows.clear();

    uintptr_t sentinel = 0;
    if (!ReadPointer(hashPtr, sentinel) && !ReadPointer(hashPtr + 8, sentinel))
        return 0;

    uintptr_t node = 0;
    if (!ReadPointer(sentinel, node))
        return 0;

    int guard = 0;
    while (node && node != sentinel && guard < 4096) {
        std::string name;
        static const int kKeyOffsets[] = { 0x10, 0x18, 0x20, 0x08 };
        for (int keyOff : kKeyOffsets) {
            if (ReadMsvcString(node + keyOff, name))
                break;
            name.clear();
        }

        void* fxResource = nullptr;
        TryReadResourceFromNode(node, fxResource);

        if (!name.empty()) {
            bool exists = false;
            for (const auto& row : rows) {
                if (row.name == name) {
                    exists = true;
                    break;
                }
            }
            if (!exists)
                rows.push_back({ name, fxResource });
        }

        uintptr_t next = 0;
        if (!ReadPointer(node, next))
            break;

        node = next;
        ++guard;
    }

    return (int)rows.size();
}

int WalkUmapNodes(uintptr_t hashPtr, std::vector<std::string>& names)
{
    std::vector<ManagerResourceRow> rows;
    const int count = WalkUmapNodes(hashPtr, rows);
    names.clear();
    for (const auto& row : rows)
        names.push_back(row.name);
    return count;
}

int ScoreResourceNameList(const std::vector<std::string>& names)
{
    if (names.empty())
        return 0;

    int score = (int)names.size() * 2;
    for (const auto& name : names) {
        if (name == "monitor" || name == "sessionmanager" || name == "spawnmanager" || name == "hardcap")
            score += 8;
    }
    return score;
}

struct ManagerEnumerateResult {
    std::vector<std::string> names;
    std::unordered_map<std::string, void*> fxResources;
};

bool TryEnumerateFromResourceMap(uintptr_t mapPtr, ManagerEnumerateResult& out)
{
    if (!mapPtr)
        return false;

    std::vector<ManagerResourceRow> rows;
    const int count = WalkUmapNodes(mapPtr, rows);
    std::vector<std::string> names;
    for (const auto& row : rows)
        names.push_back(row.name);

    const int score = ScoreResourceNameList(names);
    if (count < 1 || score < 2)
        return false;

    out.names = std::move(names);
    out.fxResources.clear();
    for (const auto& row : rows)
        out.fxResources[row.name] = row.fxResource;

    return true;
}

bool CollectResourceNamesViaManager(ManagerEnumerateResult& out)
{
    static void* resourceManager = nullptr;
    if (!resourceManager) {
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
    }

    if (!resourceManager)
        return false;

    const uintptr_t base = reinterpret_cast<uintptr_t>(resourceManager);

    if (g_cachedResourcesMapOffset >= 0) {
        if (TryEnumerateFromResourceMap(base + g_cachedResourcesMapOffset, out))
            return true;
        return false;
    }

    int bestScore = 0;
    size_t bestOffset = 0;
    ManagerEnumerateResult best;

    for (size_t off = 0x30; off <= 0x200; off += 0x8) {
        ManagerEnumerateResult trial;
        if (!TryEnumerateFromResourceMap(base + off, trial))
            continue;

        const int score = ScoreResourceNameList(trial.names);
        if (score > bestScore) {
            bestScore = score;
            bestOffset = off;
            best = std::move(trial);
        }
    }

    if (bestScore < 2 || best.names.empty())
        return false;

    g_cachedResourcesMapOffset = (int)bestOffset;
    out = std::move(best);
    return true;
}

bool CollectResourceNamesViaManager(std::vector<std::string>& out)
{
    ManagerEnumerateResult result;
    if (!CollectResourceNamesViaManager(result))
        return false;
    out = std::move(result.names);
    return true;
}

static bool NamesMatch(const std::string& a, const std::string& b)
{
    if (a.size() != b.size())
        return false;
    for (size_t i = 0; i < a.size(); ++i) {
        if (std::tolower(static_cast<unsigned char>(a[i])) != std::tolower(static_cast<unsigned char>(b[i])))
            return false;
    }
    return true;
}

ResourceLifecycle DetectResourceLifecycleImpl(
    _LuaClasses::LuaScriptRuntime* runtime,
    void* fxResource,
    bool registeredOnServer) noexcept
{
    if (runtime && runtime->GetLuaState())
        return ResourceLifecycle::Active;

    const int state = TryReadResourceState(fxResource);

    if (state == 1)
        return ResourceLifecycle::Stopped;

    if (state == 0)
        return ResourceLifecycle::Idle;

    if (state == 2 || state == 3 || state == 4)
        return ResourceLifecycle::Running;

    if (registeredOnServer)
        return ResourceLifecycle::Running;

    return ResourceLifecycle::Idle;
}

void ResolveExtraLuaApi()
{
    if (_LuaFunctions::lua_getglobal && _LuaFunctions::lua_tolstring)
        return;

    const char* modules[] = { "lua54.dll", "lua53.dll", "lua52.dll", "citizen-resources-metadata-lua.dll", "citizen-scripting-lua.dll" };
    for (const char* modName : modules) {
        HMODULE mod = GetModuleHandleA(modName);
        if (!mod)
            continue;

        if (!_LuaFunctions::lua_getglobal)
            _LuaFunctions::lua_getglobal = reinterpret_cast<lua_getglobal_t>(GetProcAddress(mod, "lua_getglobal"));
        if (!_LuaFunctions::lua_tolstring)
            _LuaFunctions::lua_tolstring = reinterpret_cast<lua_tolstring_t>(GetProcAddress(mod, "lua_tolstring"));
        if (!_LuaFunctions::lua_gettop)
            _LuaFunctions::lua_gettop = reinterpret_cast<lua_gettop_t>(GetProcAddress(mod, "lua_gettop"));
        if (!_LuaFunctions::lua_settop)
            _LuaFunctions::lua_settop = reinterpret_cast<lua_settop_t>(GetProcAddress(mod, "lua_settop"));
        if (!_LuaFunctions::luaL_newstate)
            _LuaFunctions::luaL_newstate = reinterpret_cast<luaL_newstate_t>(GetProcAddress(mod, "luaL_newstate"));
        if (!_LuaFunctions::lua_close)
            _LuaFunctions::lua_close = reinterpret_cast<lua_close_t>(GetProcAddress(mod, "lua_close"));
        if (!_LuaFunctions::luaL_openlibs)
            _LuaFunctions::luaL_openlibs = reinterpret_cast<luaL_openlibs_t>(GetProcAddress(mod, "luaL_openlibs"));



        if (_LuaFunctions::lua_getglobal && _LuaFunctions::lua_tolstring)
            break;
    }
}

_LuaClasses::LuaScriptRuntime* GetAnyRuntimeWithState()
{
    LockRuntimes();
    for (auto* rt : _LuaGlobals::_AvailableRuntimes) {
        if (rt && rt->GetLuaState()) {
            UnlockRuntimes();
            return rt;
        }
    }
    UnlockRuntimes();

    if (g_currentLuaRuntime && IsReadableMemory(g_currentLuaRuntime, sizeof(void*))) {
        auto* rt = g_currentLuaRuntime->GetLuaScriptRuntime();
        if (rt && rt->GetLuaState())
            return rt;
    }

    return nullptr;
}

static bool IsWritableMemory(const void* ptr, size_t size)
{
    if (!ptr || !size)
        return false;

    MEMORY_BASIC_INFORMATION mbi{};
    if (!SouthVirtualQuery(ptr, &mbi, sizeof(mbi)))
        return false;

    if (mbi.State != MEM_COMMIT)
        return false;

    if (!(mbi.Protect & (PAGE_READWRITE | PAGE_WRITECOPY | PAGE_EXECUTE_READWRITE | PAGE_EXECUTE_WRITECOPY)))
        return false;

    if (mbi.Protect & (PAGE_NOACCESS | PAGE_GUARD))
        return false;

    const auto addr = reinterpret_cast<uintptr_t>(ptr);
    const auto regionEnd = reinterpret_cast<uintptr_t>(mbi.BaseAddress) + mbi.RegionSize;
    return addr + size <= regionEnd;
}

bool RunLuaOnRuntime(_LuaClasses::LuaScriptRuntime* runtime, const char* script)
{
    if (!runtime || !g_currentLuaRuntime)
        return false;

    if (!IsReadableMemory(runtime, 0x100) || !IsReadableMemory(g_currentLuaRuntime, sizeof(void*)))
        return false;

    auto* L = runtime->GetLuaState();
    if (!L || !IsReadableMemory(L, 0x80))
        return false;

    if (!IsWritableMemory(g_currentLuaRuntime, sizeof(void*)))
        return false;

    __int64* originalRuntime = nullptr;
    bool success = false;
    __try {
        originalRuntime = *reinterpret_cast<__int64**>(g_currentLuaRuntime);
        *reinterpret_cast<__int64**>(g_currentLuaRuntime) = reinterpret_cast<__int64*>(runtime);
        success = luaL_dostring(L, script) == 0;
        *reinterpret_cast<__int64**>(g_currentLuaRuntime) = originalRuntime;
    }
    __except (EXCEPTION_EXECUTE_HANDLER) {
        if (originalRuntime)
            *reinterpret_cast<__int64**>(g_currentLuaRuntime) = originalRuntime;
        success = false;
    }

    return success;
}

bool ReadGlobalString(__int64* L, const char* globalName, std::string& out)
{
    if (!L || !_LuaFunctions::lua_getglobal || !_LuaFunctions::lua_tolstring)
        return false;

    const int top = _LuaFunctions::lua_gettop ? _LuaFunctions::lua_gettop(L) : 0;
    _LuaFunctions::lua_getglobal(L, globalName);
    const char* value = _LuaFunctions::lua_tolstring(L, -1, nullptr);
    if (!value || !value[0]) {
        if (_LuaFunctions::lua_settop)
            _LuaFunctions::lua_settop(L, top);
        return false;
    }

    out = value;
    if (_LuaFunctions::lua_settop)
        _LuaFunctions::lua_settop(L, top);
    return true;
}

} 

bool ReadLuaGlobalString(_LuaClasses::LuaScriptRuntime* runtime, const char* globalName, std::string& out) noexcept
{
    out.clear();
    if (!runtime || !globalName || !globalName[0])
        return false;

    ResolveExtraLuaApi();
    auto* L = runtime->GetLuaState();
    if (!L)
        return false;

    return ReadGlobalString(L, globalName, out);
}

namespace {

bool CollectResourceNamesViaLua(std::vector<std::string>& out)
{
    ResolveExtraLuaApi();
    auto* runtime = GetAnyRuntimeWithState();
    if (!runtime)
        return false;

    static const char* kCollectScript = R"(
_G.__se_names_flat = ''
for i = 0, GetNumResources() - 1 do
    local n = GetResourceByFindIndex(i)
    if n and n ~= '' then
        if _G.__se_names_flat ~= '' then
            _G.__se_names_flat = _G.__se_names_flat .. string.char(30)
        end
        _G.__se_names_flat = _G.__se_names_flat .. n
    end
end
)";

    if (!RunLuaOnRuntime(runtime, kCollectScript))
        return false;

    auto* L = runtime->GetLuaState();
    std::string flat;
    if (!ReadGlobalString(L, "__se_names_flat", flat))
        return false;

    std::vector<std::string> names;
    size_t start = 0;
    while (start <= flat.size()) {
        size_t end = flat.find(kResourceDelimiter, start);
        if (end == std::string::npos)
            end = flat.size();

        std::string piece = flat.substr(start, end - start);
        if (IsPlausibleResourceName(piece)) {
            if (std::find(names.begin(), names.end(), piece) == names.end())
                names.push_back(piece);
        }

        if (end == flat.size())
            break;
        start = end + 1;
    }

    if (names.empty())
        return false;

    out = std::move(names);
    return true;
}

} 

const char* GetResourceLifecycleLabel(ResourceLifecycle life) noexcept
{
    switch (life) {
    case ResourceLifecycle::Active: return "active";
    case ResourceLifecycle::Running: return "running";
    case ResourceLifecycle::Stopped: return "stopped";
    default: return "idle";
    }
}

ResourceLifecycle DetectResourceLifecycle(
    _LuaClasses::LuaScriptRuntime* runtime,
    void* fxResource,
    bool registeredOnServer) noexcept
{
    return DetectResourceLifecycleImpl(runtime, fxResource, registeredOnServer);
}

namespace _LuaGlobals {
    HMODULE CitizenResourcesMetadataLua = nullptr;
    HMODULE CitizenScriptingLua = nullptr;
}

namespace _LuaClasses {
    char* ResourceTemp::GetResourceName() noexcept {
        if (!this) return nullptr;
        if (!IsReadableMemory(this, 0xD0 + sizeof(void*))) return nullptr;
        return reinterpret_cast<char*>(&ResourceName);
    }

    char* ResourceHost::GetResourceName() noexcept {
        if (!this) return nullptr;
        if (!IsReadableMemory(this, sizeof(ResourceHost))) return nullptr;
        char* name = nullptr;
        if (Resource2 && IsReadableMemory(Resource2, sizeof(void*))) {
            name = Resource2->GetResourceName();
        }
        if (!name && Resource && IsReadableMemory(Resource, sizeof(void*))) {
            name = Resource->GetResourceName();
        }
        return name;
    }

    std::string ResourceHost::GetResourceNameStr() {
        char* name = GetResourceName();
        return name ? std::string(name) : "Unknown";
    }

    __int64* LuaScriptRuntime::GetLuaState() {
        if (!this)
            return 0;

        ResolveExtraLuaApi();

        const uintptr_t base = reinterpret_cast<uintptr_t>(this);
        static const size_t kStateOffsets[] = { 0x58, 0x50, 0x60, 0x48, 0x68, 0x70, 0x78 };

        for (size_t off : kStateOffsets) {
            auto* L = *reinterpret_cast<__int64**>(base + off);
            if (!L || !IsReadableMemory(L, 0x80))
                continue;

            // Safe read-only structural validation of lua_State to prevent greedy offset matching:
            // 1. Verify l_G (global_State* at offset 0x20) is a valid readable pointer.
            uintptr_t l_G = *reinterpret_cast<uintptr_t*>(reinterpret_cast<uintptr_t>(L) + 0x20);
            if (!l_G || !IsReadableMemory(reinterpret_cast<void*>(l_G), 8))
                continue;

            // 2. Verify stack (StkId at offset 0x40) is a valid readable pointer.
            uintptr_t stack = *reinterpret_cast<uintptr_t*>(reinterpret_cast<uintptr_t>(L) + 0x40);
            if (!stack || !IsReadableMemory(reinterpret_cast<void*>(stack), 8))
                continue;

            return L;
        }
        return 0;
    }

    ResourceHost* LuaScriptRuntime::GetResourceHost() {
        if (!this) return 0;
        return *reinterpret_cast<ResourceHost**>(reinterpret_cast<uintptr_t>(this) + RES_HOST_OFFSET);
    }

    std::string LuaScriptRuntime::GetResourceName() {
        if (!g_overrideResourceName.empty())
            return g_overrideResourceName;

        ResourceHost* host = GetResourceHost();
        return host ? host->GetResourceNameStr() : "Unknown";
    }

    LuaScriptRuntime* LuaScriptRuntimeHolder::GetLuaScriptRuntime() {
        if (!this) return nullptr;
        return *reinterpret_cast<LuaScriptRuntime**>(this);
    }
}

template<typename T>
T ResolveRelative(uintptr_t address, int offset = 1)
{
    if (!address) return nullptr;
    int32_t relativeOffset = *(int32_t*)(address + offset);
    uintptr_t absoluteAddress = address + offset + 4 + relativeOffset;
    return (T)absoluteAddress;
}

void LuaBase::Initialize()
{

    std::cout << "[LUA - BASE] Scans:\n\n";

    _LuaGlobals::CitizenResourcesMetadataLua = GetModuleHandleA("citizen-resources-metadata-lua.dll");
    _LuaGlobals::CitizenScriptingLua = GetModuleHandleA("citizen-scripting-lua.dll");

    if (_LuaGlobals::CitizenResourcesMetadataLua)
    {
        uintptr_t resModuleBase = (uintptr_t)_LuaGlobals::CitizenResourcesMetadataLua;

        uintptr_t pcallkAddr = scan(_LuaGlobals::CitizenResourcesMetadataLua, "48 89 5C 24 ? 33 F6");
        if (pcallkAddr) pcallkAddr -= 7;
        _LuaFunctions::lua_pcallk = reinterpret_cast<lua_pcallk_t>(pcallkAddr);
        printf("  lua_pcallk found at: 0x%llX (offset: 0x%llX)\n", pcallkAddr, pcallkAddr - resModuleBase);



        uintptr_t loadfilexAddr = scan(_LuaGlobals::CitizenResourcesMetadataLua, "41 56 41 57 48 8D AC 24 ? ? ? ? 48 81 EC ? ? ? ? 48 8B 05 ? ? ? ? 48 33 C4 48 89 85 ? ? ? ? 48 8B 41 ? 45 33 ED");
        if (loadfilexAddr) loadfilexAddr -= 8;
        _LuaFunctions::luaL_loadfilex = reinterpret_cast<luaL_loadfilex_t>(loadfilexAddr);
        printf("[DEBUG] luaL_loadfilex found at: 0x%llX (offset: 0x%llX)\n", loadfilexAddr, loadfilexAddr - resModuleBase);

        uintptr_t loadbufferxAddr = scan(_LuaGlobals::CitizenResourcesMetadataLua, "48 83 EC ? 4D 89 43");
        if (loadbufferxAddr) loadbufferxAddr -= 8;
        _LuaFunctions::luaL_loadbufferx = reinterpret_cast<luaL_loadbufferx_t>(loadbufferxAddr);
        printf("[DEBUG] luaL_loadbufferx found at: 0x%llX (offset: 0x%llX)\n", loadbufferxAddr, loadbufferxAddr - resModuleBase);

    }

    if (_LuaGlobals::CitizenScriptingLua)
    {
        uintptr_t scrptModuleBase = (uintptr_t)_LuaGlobals::CitizenScriptingLua;

        uintptr_t runtimeScanResult = scan((HMODULE)scrptModuleBase, "48 8B 0D ? ? ? ? 48 89 4D"); 
        if (runtimeScanResult) {
            g_currentLuaRuntime = reinterpret_cast<_LuaClasses::LuaScriptRuntimeHolder*>(ResolveRelativeAddress(runtimeScanResult + 3, 4));
            printf("  g_currentLuaRuntime found at: 0x%llX (offset: 0x%llX)\n", (uintptr_t)g_currentLuaRuntime, (uintptr_t)g_currentLuaRuntime - scrptModuleBase);
        } else {
            g_currentLuaRuntime = nullptr;
            printf("  g_currentLuaRuntime scan failed!\n");
        }

    }

    std::cout << "[LUA - BASE] Scans done.\n\n";

    ResolveExtraLuaApi();
    g_initialized = true;

    // Background polling thread to safely gather script runtimes as they execute
    CreateThread(nullptr, 0, [](LPVOID) -> DWORD {
        while (g_initialized) {
            Sleep(1000); // Poll 1 time per second instead of 200 times to prevent overhead and crashes
            Collector::CollectScriptRuntimes();
        }
        return 0;
    }, nullptr, 0, nullptr);

    RefreshServerResources(false);
}

_LuaClasses::LuaScriptRuntime* FindRuntimeByName(const std::string& name) noexcept
{
    LockRuntimes();
    for (auto* rt : _LuaGlobals::_AvailableRuntimes) {
        if (rt && NamesMatch(rt->GetResourceName(), name)) {
            UnlockRuntimes();
            return rt;
        }
    }
    UnlockRuntimes();
    return nullptr;
}

static _LuaClasses::LuaScriptRuntime* FindRuntimeForEntryName(
    const std::string& name,
    const std::unordered_map<std::string, _LuaClasses::LuaScriptRuntime*>& runtimeByName) noexcept
{
    auto it = runtimeByName.find(name);
    if (it != runtimeByName.end())
        return it->second;

    for (const auto& [key, rt] : runtimeByName) {
        if (NamesMatch(key, name))
            return rt;
    }

    return FindRuntimeByName(name);
}

_LuaClasses::LuaScriptRuntime* ResolveRuntimeForEntry(const ResourceEntry& entry) noexcept
{
    auto* rt = FindRuntimeByName(entry.name);
    if (!rt)
        return nullptr;

    if (!NamesMatch(rt->GetResourceName(), entry.name))
        return nullptr;

    return rt;
}

void RefreshServerResources(bool allowLua) noexcept
{
    LockRuntimes();

    auto& runtimes = _LuaGlobals::_AvailableRuntimes;
    runtimes.erase(std::remove_if(runtimes.begin(), runtimes.end(), [](auto* rt) {
        if (!rt) return true;

        if (!IsReadableMemory(rt, 0x100)) return true;

        __int64* L = rt->GetLuaState();
        if (!L || !IsReadableMemory(L, 8)) return true;

        auto* host = rt->GetResourceHost();
        if (!host || !IsReadableMemory(host, 8)) return true;

        std::string name = rt->GetResourceName();
        if (!IsPlausibleResourceName(name)) return true;

        return false;
    }), runtimes.end());
    UnlockRuntimes();

    if (selectedRuntime) {
        bool valid = true;
        if (!IsReadableMemory(selectedRuntime, 0x100)) {
            valid = false;
        } else {
            __int64* L = selectedRuntime->GetLuaState();
            auto* host = selectedRuntime->GetResourceHost();
            if (!L || !IsReadableMemory(L, 8) || !host || !IsReadableMemory(host, 8) || !IsPlausibleResourceName(selectedRuntime->GetResourceName())) {
                valid = false;
            }
        }
        if (!valid) {
            selectedRuntime = nullptr;
            selectedRuntimeIndex = 0;
        }
    }

    std::string previousName;
    if (selectedRuntimeIndex >= 0 && selectedRuntimeIndex < (int)_LuaGlobals::_ServerResources.size())
        previousName = _LuaGlobals::_ServerResources[selectedRuntimeIndex].name;
    else if (selectedRuntime)
        previousName = selectedRuntime->GetResourceName();

    Collector::CollectScriptRuntimes();

    if (g_currentLuaRuntime) {
        if (auto* hookRt = g_currentLuaRuntime->GetLuaScriptRuntime()) {
            auto& runtimes = _LuaGlobals::_AvailableRuntimes;
            if (std::find(runtimes.begin(), runtimes.end(), hookRt) == runtimes.end())
                runtimes.push_back(hookRt);
        }
    }

    std::unordered_map<std::string, _LuaClasses::LuaScriptRuntime*> runtimeByName;
    for (auto* rt : _LuaGlobals::_AvailableRuntimes) {
        if (!rt)
            continue;
        const std::string name = rt->GetResourceName();
        if (IsPlausibleResourceName(name))
            runtimeByName[name] = rt;
    }

    ManagerEnumerateResult managerData;
    std::vector<std::string> names;
    bool listedViaManager = CollectResourceNamesViaManager(managerData);
    if (!listedViaManager) {
        if (allowLua) {
            CollectResourceNamesViaLua(names);
        }
        managerData.fxResources.clear();
    }
    else {
        names = std::move(managerData.names);
    }

    for (const auto& [name, rt] : runtimeByName) {
        if (std::find(names.begin(), names.end(), name) == names.end())
            names.push_back(name);
    }

    std::sort(names.begin(), names.end());
    names.erase(std::unique(names.begin(), names.end()), names.end());

    _LuaGlobals::_ServerResources.clear();
    _LuaGlobals::_ServerResources.reserve(names.size());
    for (const auto& name : names) {
        ResourceEntry entry;
        entry.name = name;
        entry.runtime = FindRuntimeForEntryName(name, runtimeByName);

        void* fxResource = nullptr;
        auto fxIt = managerData.fxResources.find(name);
        if (fxIt != managerData.fxResources.end())
            fxResource = fxIt->second;

        const bool onServerList = listedViaManager
            || std::find(names.begin(), names.end(), name) != names.end()
            || fxResource != nullptr;

        entry.lifecycle = DetectResourceLifecycle(entry.runtime, fxResource, onServerList);

        _LuaGlobals::_ServerResources.push_back(entry);
    }

    if (!previousName.empty()) {
        for (int i = 0; i < (int)_LuaGlobals::_ServerResources.size(); ++i) {
            if (_LuaGlobals::_ServerResources[i].name == previousName) {
                selectedRuntimeIndex = i;
                selectedRuntime = _LuaGlobals::_ServerResources[i].runtime;
                return;
            }
        }
    }

    if (selectedRuntimeIndex >= (int)_LuaGlobals::_ServerResources.size())
        selectedRuntimeIndex = 0;
    selectedRuntime = _LuaGlobals::_ServerResources.empty()
        ? nullptr
        : _LuaGlobals::_ServerResources[selectedRuntimeIndex].runtime;
}

void Collector::CollectScriptRuntimes() noexcept
{
    if (!g_currentLuaRuntime)
        return;

    __try {
        _LuaClasses::LuaScriptRuntime* luascriptruntime = g_currentLuaRuntime->GetLuaScriptRuntime();
        if (luascriptruntime && IsReadableMemory(luascriptruntime, 0x100)) {
            LockRuntimes();
            auto& runtimes = _LuaGlobals::_AvailableRuntimes;
            if (std::find(runtimes.begin(), runtimes.end(), luascriptruntime) == runtimes.end()) {
                printf("[Found LuaScriptRuntime via Hook] %llX, LuaState: %llX\n", luascriptruntime, luascriptruntime->GetLuaState());
                runtimes.push_back(luascriptruntime);
                g_runtimeFound = true;
            }
            UnlockRuntimes();
        }
    }
    __except (EXCEPTION_EXECUTE_HANDLER) {}
}
