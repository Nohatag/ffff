// Leaked By Cry Service & Siraj
#include "api-exec.h"
#include "../../scanner/scanner.h"
#include <fstream>
#include <algorithm>
#include <random>
#include <iostream>
#include <vector>
#include <string>
#include <thread>
#include <chrono>
#include "../../Hook/kiero/minhook/include/MinHook.h"
#include <windows.h>
#include "../../memory.h"

static bool IsMemoryRangeReadable(const void* ptr, size_t size);
static bool IsMemoryRangeWritable(const void* ptr, size_t size);

int selectedRuntimeIndex = 0;
_LuaClasses::LuaScriptRuntime* selectedRuntime = nullptr;

extern void PushNotification(std::string title, std::string msg, float duration = 3.0f);
extern std::string g_overrideResourceName;

extern std::string GetRandomString(int len);
extern "C" void __cdecl MachoDuiLogFFI(const char* msg);
extern "C" void __cdecl MachoStopResourceFFI(const char* name);
extern "C" void __cdecl MachoStartResourceFFI(const char* name);
extern "C" void __cdecl MachoInjectResourceFFI(const char* name, const char* code);

static std::string g_lastExecutorError;
static std::string g_lastExecutedResource;
static std::string g_isolatedOutput;

const char* GetLastExecutorError()   { return g_lastExecutorError.c_str(); }
const char* GetLastExecutedResourceName() { return g_lastExecutedResource.c_str(); }
const char* GetIsolatedOutput()      { return g_isolatedOutput.c_str(); }

bool SafeValidateRuntimePointer(_LuaClasses::LuaScriptRuntime* rt)
{
    if (!rt) return false;
    if (!IsMemoryRangeReadable(rt, 0x100)) return false;

    bool valid = false;
    __try {
        __int64* L = rt->GetLuaState();
        if (L) {
            auto* host = rt->GetResourceHost();
            if (host && IsMemoryRangeReadable(host, sizeof(_LuaClasses::ResourceHost))) {
                char* rName = host->GetResourceName();
                if (rName && IsMemoryRangeReadable(rName, 1)) {
                    valid = true;
                }
            }
        }
    }
    __except (EXCEPTION_EXECUTE_HANDLER) {
        valid = false;
    }
    return valid;
}

static void CaptureLuaError(__int64* L)
{
    g_lastExecutorError.clear();
    if (!L) { g_lastExecutorError = "invalid lua state"; return; }

    if (_LuaFunctions::lua_tolstring) {
        const char* err = _LuaFunctions::lua_tolstring(L, -1, nullptr);
        if (err && err[0]) g_lastExecutorError = err;
    }
    if (g_lastExecutorError.empty()) g_lastExecutorError = "lua execution failed";

    if (_LuaFunctions::lua_gettop && _LuaFunctions::lua_settop) {
        const int top = _LuaFunctions::lua_gettop(L);
        if (top > 0) _LuaFunctions::lua_settop(L, top - 1);
    }
}

static std::string GetMachoSetupScript()
{
    char stopAddrStr[64];
    char startAddrStr[64];
    char injectAddrStr[64];
    sprintf_s(stopAddrStr, "0x%llX", (unsigned long long)&MachoStopResourceFFI);
    sprintf_s(startAddrStr, "0x%llX", (unsigned long long)&MachoStartResourceFFI);
    sprintf_s(injectAddrStr, "0x%llX", (unsigned long long)&MachoInjectResourceFFI);

    std::string stopAddrStrStd(stopAddrStr);
    std::string startAddrStrStd(startAddrStr);
    std::string injectAddrStrStd(injectAddrStr);

    std::string printOverride = 
        "local print = function(...)\n"
        "    local args = {...}\n"
        "    for i, v in ipairs(args) do args[i] = tostring(v) end\n"
        "    Citizen.Trace(table.concat(args, '\\t') .. '\\n')\n"
        "end\n"
        "local MachoResourceInjectable = function(res)\n"
        "    if type(res) ~= 'string' then return false end\n"
        "    return GetResourceState(res) == 'started'\n"
        "end\n"
        "_G.MachoResourceInjectable = MachoResourceInjectable\n"
        "local MachoIsolatedInject = function(code)\n"
        "    local f, err = load(code, '@isolated', 'bt')\n"
        "    if f then\n"
        "        local ok, res = pcall(f)\n"
        "        if not ok then print('[isolated error] ' .. tostring(res)) end\n"
        "    else\n"
        "        print('[isolated compile error] ' .. tostring(err))\n"
        "    end\n"
        "end\n"
        "_G.MachoIsolatedInject = MachoIsolatedInject\n"
        "local MachoMenuNotification = function(title, desc)\n"
        "    local msg = '[' .. tostring(title) .. '] ' .. tostring(desc)\n"
        "    BeginTextCommandThefeedPost('STRING')\n"
        "    AddTextComponentSubstringPlayerName(msg)\n"
        "    EndTextCommandThefeedPostTicker(false, true)\n"
        "end\n"
        "_G.MachoMenuNotification = MachoMenuNotification\n"
        "local MachoWebRequest = function(url)\n"
        "    local ffi = _G.ffi or (package and package.loaded and package.loaded.ffi) or (type(require) == 'function' and select(2, pcall(require, 'ffi')))\n"
        "    if not ffi or not ffi.load or not ffi.cdef then return '' end\n"
        "    local cdefStatus = pcall(ffi.cdef, [[\n"
        "        typedef void* HINTERNET;\n"
        "        HINTERNET InternetOpenA(const char* lpszAgent, unsigned long dwAccessType, const char* lpszProxy, const char* lpszProxyBypass, unsigned long dwFlags);\n"
        "        HINTERNET InternetOpenUrlA(HINTERNET hInternet, const char* lpszUrl, const char* lpszHeaders, unsigned long dwHeadersLength, unsigned long dwFlags, unsigned __int64 dwContext);\n"
        "        int InternetReadFile(HINTERNET hFile, void* lpBuffer, unsigned long dwNumberOfBytesToRead, unsigned long* lpdwNumberOfBytesRead);\n"
        "        int InternetCloseHandle(HINTERNET hInternet);\n"
        "    ]])\n"
        "    if not cdefStatus then return '' end\n"
        "    local loadStatus, wininet = pcall(ffi.load, 'wininet')\n"
        "    if not loadStatus or not wininet then return '' end\n"
        "    local hInternet = wininet.InternetOpenA('MachoUserAgent', 1, nil, nil, 0)\n"
        "    if hInternet == nil then return '' end\n"
        "    local hUrl = wininet.InternetOpenUrlA(hInternet, url, nil, 0, 0x80000000, 0)\n"
        "    if hUrl == nil then\n"
        "        wininet.InternetCloseHandle(hInternet)\n"
        "        return ''\n"
        "    end\n"
        "    local bufSize = 4096\n"
        "    local buffer = ffi.new('char[?]', bufSize)\n"
        "    local readSize = ffi.new('unsigned long[1]')\n"
        "    local result = ''\n"
        "    while true do\n"
        "        local ok = wininet.InternetReadFile(hUrl, buffer, bufSize, readSize)\n"
        "        if ok == 0 or readSize[0] == 0 then break end\n"
        "        result = result .. ffi.string(buffer, readSize[0])\n"
        "    end\n"
        "    wininet.InternetCloseHandle(hUrl)\n"
        "    wininet.InternetCloseHandle(hInternet)\n"
        "    return result\n"
        "end\n"
        "_G.MachoWebRequest = MachoWebRequest\n"
        "if not _G.__machoDownCallbacks then\n"
        "    _G.__machoDownCallbacks = {}\n"
        "    _G.__machoUpCallbacks = {}\n"
        "    _G.__machoKeyStates = {}\n"
        "    local ffi = _G.ffi or (package and package.loaded and package.loaded.ffi) or (type(require) == 'function' and select(2, pcall(require, 'ffi')))\n"
        "    if ffi and ffi.load and ffi.cdef then\n"
        "        local cdefStatus = pcall(ffi.cdef, [[\n"
        "            short GetAsyncKeyState(int vKey);\n"
        "        ]])\n"
        "        local loadStatus, user32 = pcall(ffi.load, 'user32')\n"
        "        if cdefStatus and loadStatus and user32 then\n"
        "            Citizen.CreateThread(function()\n"
        "            while true do\n"
        "                Citizen.Wait(10)\n"
        "                for vk = 1, 255 do\n"
        "                    local isDown = bit.band(user32.GetAsyncKeyState(vk), 0x8000) ~= 0\n"
        "                    local wasDown = _G.__machoKeyStates[vk] or false\n"
        "                    if isDown and not wasDown then\n"
        "                        _G.__machoKeyStates[vk] = true\n"
        "                        for i = 1, #_G.__machoDownCallbacks do\n"
        "                            pcall(_G.__machoDownCallbacks[i], vk)\n"
        "                        end\n"
        "                    elseif not isDown and wasDown then\n"
        "                        _G.__machoKeyStates[vk] = false\n"
        "                        for i = 1, #_G.__machoUpCallbacks do\n"
        "                            pcall(_G.__machoUpCallbacks[i], vk)\n"
        "                        end\n"
        "                    end\n"
        "                end\n"
        "            end\n"
        "            end)\n"
        "        end\n"
        "    end\n"
        "end\n"
        "local MachoOnKeyDown = function(cb)\n"
        "    for i = 1, #_G.__machoDownCallbacks do\n"
        "        if _G.__machoDownCallbacks[i] == cb then return end\n"
        "    end\n"
        "    table.insert(_G.__machoDownCallbacks, cb)\n"
        "end\n"
        "_G.MachoOnKeyDown = MachoOnKeyDown\n"
        "local MachoOnKeyUp = function(cb)\n"
        "    for i = 1, #_G.__machoUpCallbacks do\n"
        "        if _G.__machoUpCallbacks[i] == cb then return end\n"
        "    end\n"
        "    table.insert(_G.__machoUpCallbacks, cb)\n"
        "end\n"
        "_G.MachoOnKeyUp = MachoOnKeyUp\n"
        "local MachoDuiLog = function(msg)\n"
        "    print('[DUI Log Fallback] ' .. tostring(msg))\n"
        "end\n"
        "local MachoResourceStop = function(name) end\n"
        "local MachoResourceStart = function(name) end\n"
        "local MachoInjectResource = function(name, code) end\n"
        "local MachoInjectResourceRaw = function(name, code) end\n"
        "local ffi = _G.ffi or (package and package.loaded and package.loaded.ffi) or (type(require) == 'function' and select(2, pcall(require, 'ffi')))\n"
        "if ffi and ffi.cast and ffi.cdef then\n"
        "    pcall(ffi.cdef, [[\n"
        "        typedef void (__cdecl *MachoResourceControlFn)(const char*);\n"
        "        typedef void (__cdecl *MachoInjectResourceFn)(const char*, const char*);\n"
        "    ]])\n"
        "    MachoResourceStop = function(name)\n"
        "        pcall(ffi.cast('MachoResourceControlFn', " + stopAddrStrStd + "), name)\n"
        "    end\n"
        "    MachoResourceStart = function(name)\n"
        "        pcall(ffi.cast('MachoResourceControlFn', " + startAddrStrStd + "), name)\n"
        "    end\n"
        "    MachoInjectResource = function(name, code)\n"
        "        pcall(ffi.cast('MachoInjectResourceFn', " + injectAddrStrStd + "), tostring(name), tostring(code))\n"
        "    end\n"
        "    MachoInjectResourceRaw = function(name, code)\n"
        "        pcall(ffi.cast('MachoInjectResourceFn', " + injectAddrStrStd + "), tostring(name), tostring(code))\n"
        "    end\n"
        "end\n"
        "if not _G.__machoDuiList then\n"
        "    _G.__machoDuiList = {}\n"
        "    Citizen.CreateThread(function()\n"
        "        local drawTick = 0\n"
        "        while true do\n"
        "            Citizen.Wait(0)\n"
        "            for handle, duiInfo in pairs(_G.__machoDuiList) do\n"
        "                if duiInfo.visible then\n"
        "                    local txd = duiInfo.txd\n"
        "                    if not txd then\n"
        "                        local rawDui = duiInfo.dui\n"
        "                        local duiHandle = GetDuiHandle(rawDui)\n"
        "                        if duiHandle then\n"
        "                            local txdName = 'macho_txd_' .. tostring(handle)\n"
        "                            local txnName = 'macho_txn_' .. tostring(handle)\n"
        "                            local txdObj = CreateRuntimeTxd(txdName)\n"
        "                            CreateRuntimeTextureFromDuiHandle(txdObj, txnName, duiHandle)\n"
        "                            duiInfo.txd = txdName\n"
        "                            duiInfo.txn = txnName\n"
        "                        end\n"
        "                    end\n"
        "                    if txd then\n"
        "                        DrawSprite(txd, duiInfo.txn, 0.5, 0.5, 1.0, 1.0, 0.0, 255, 255, 255, 255)\n"
        "                    end\n"
        "                end\n"
        "            end\n"
        "        end\n"
        "    end)\n"
        "end\n"
        "local MachoCreateDui = function(url, w, h)\n"
        "    if type(url) ~= 'string' then return nil end\n"
        "    local targetUrl = url\n"
        "    local width, height = w or 1920, h or 1080\n"
        "    local dui = CreateDui(targetUrl, width, height)\n"
        "    if not dui then return nil end\n"
        "    local handle = #_G.__machoDuiList + 1\n"
        "    _G.__machoDuiList[handle] = {\n"
        "        dui = dui,\n"
        "        visible = false,\n"
        "        url = targetUrl\n"
        "    }\n"
        "    return handle\n"
        "end\n"
        "_G.MachoCreateDui = MachoCreateDui\n"
        "local MachoShowDui = function(handle)\n"
        "    local duiInfo = _G.__machoDuiList[handle]\n"
        "    if duiInfo then\n"
        "        duiInfo.visible = true\n"
        "        if SetDuiVisibility then\n"
        "            pcall(SetDuiVisibility, duiInfo.dui, true)\n"
        "        else\n"
        "            pcall(Citizen.InvokeNative, 0x5D4CD1D, duiInfo.dui, true)\n"
        "        end\n"
        "    end\n"
        "end\n"
        "_G.MachoShowDui = MachoShowDui\n"
        "local MachoHideDui = function(handle)\n"
        "    local duiInfo = _G.__machoDuiList[handle]\n"
        "    if duiInfo then\n"
        "        duiInfo.visible = false\n"
        "        if SetDuiVisibility then\n"
        "            pcall(SetDuiVisibility, duiInfo.dui, false)\n"
        "        else\n"
        "            pcall(Citizen.InvokeNative, 0x5D4CD1D, duiInfo.dui, false)\n"
        "        end\n"
        "    end\n"
        "end\n"
        "_G.MachoHideDui = MachoHideDui\n"
        "local MachoDestroyDui = function(handle)\n"
        "    local duiInfo = _G.__machoDuiList[handle]\n"
        "    if duiInfo then\n"
        "        duiInfo.visible = false\n"
        "        DestroyDui(duiInfo.dui)\n"
        "        _G.__machoDuiList[handle] = nil\n"
        "    end\n"
        "end\n"
        "_G.MachoDestroyDui = MachoDestroyDui\n"
        "local MachoSendDuiMessage = function(handle, json)\n"
        "    local duiInfo = _G.__machoDuiList[handle]\n"
        "    if duiInfo then\n"
        "        SendDuiMessage(duiInfo.dui, json)\n"
        "    end\n"
        "end\n"
        "_G.MachoSendDuiMessage = MachoSendDuiMessage\n"
        "local MachoExecuteDuiScript = function(handle, script)\n"
        "    local duiInfo = _G.__machoDuiList[handle]\n"
        "    if duiInfo then\n"
        "        SetDuiUrl(duiInfo.dui, 'javascript:' .. script)\n"
        "    end\n"
        "end\n"
        "_G.MachoExecuteDuiScript = MachoExecuteDuiScript\n"
        "_G.MachoResourceStop = MachoResourceStop\n"
        "_G.MachoResourceStart = MachoResourceStart\n"
        "_G.GExecuteScript = MachoInjectResource\n"
        "_G.MachoInjectResource = MachoInjectResource\n"
        "_G.MachoInjectResourceRaw = MachoInjectResourceRaw\n";

    return printOverride;
}

static std::string WrapExecutorScript(const std::string& script, const std::string& resName)
{
    std::string printOverride = 
        "local print = function(...)\n"
        "    local args = {...}\n"
        "    for i, v in ipairs(args) do args[i] = tostring(v) end\n"
        "    Citizen.Trace(table.concat(args, '\\t') .. '\\n')\n"
        "end\n";

    if (script.find("CreateThread") != std::string::npos || 
        script.find("MachoHookNative") != std::string::npos ||
        script.find("-- CreateThread bypass") != std::string::npos) {
        return printOverride + script;
    }

    return "Citizen.CreateThread(function()\n" + printOverride + script + "\nend)";
}

static void CollectActiveRuntimes(std::vector<_LuaClasses::LuaScriptRuntime*>& out)
{
    Collector::CollectScriptRuntimes();
    out.clear();
    LockRuntimes();
    auto& runtimes = _LuaGlobals::_AvailableRuntimes;
    runtimes.erase(std::remove_if(runtimes.begin(), runtimes.end(), [](auto* rt) {
        return !SafeValidateRuntimePointer(rt);
    }), runtimes.end());

    for (auto* rt : runtimes) {
        if (std::find(out.begin(), out.end(), rt) == out.end()) out.push_back(rt);
    }
    UnlockRuntimes();
    if (g_currentLuaRuntime && IsMemoryRangeReadable(g_currentLuaRuntime, sizeof(void*))) {
        auto* hookRt = g_currentLuaRuntime->GetLuaScriptRuntime();
        if (hookRt && SafeValidateRuntimePointer(hookRt))
            if (std::find(out.begin(), out.end(), hookRt) == out.end())
                out.push_back(hookRt);
    }
}

void SyncSelectedRuntimeFromIndex()
{
    auto& resources = _LuaGlobals::_ServerResources;
    if (resources.empty()) { selectedRuntimeIndex = 0; selectedRuntime = nullptr; return; }
    if (selectedRuntimeIndex < 0 || selectedRuntimeIndex >= (int)resources.size())
        selectedRuntimeIndex = 0;
    selectedRuntime = ResolveRuntimeForEntry(resources[selectedRuntimeIndex]);
}

class Win32Mutex {
private:
    CRITICAL_SECTION cs;
public:
    Win32Mutex() { InitializeCriticalSection(&cs); }
    ~Win32Mutex() { DeleteCriticalSection(&cs); }
    void lock() { EnterCriticalSection(&cs); }
    void unlock() { LeaveCriticalSection(&cs); }
};

static Win32Mutex g_LuaExecMutex;
static ULONGLONG g_LastExecTime = 0;

static bool SEHSafeCall(lua_pcallk_t pcallk, luaL_loadbufferx_t loadbufferx, __int64* L, const char* scriptData, size_t scriptSize, _LuaClasses::LuaScriptRuntime* runtime)
{
    bool success = true;
    __try {
        __int64* originalRuntime = *reinterpret_cast<__int64**>(g_currentLuaRuntime);
        *reinterpret_cast<__int64**>(g_currentLuaRuntime) = reinterpret_cast<__int64*>(runtime);

        if (loadbufferx(L, scriptData, scriptSize, "exec", nullptr) != 0) {
            success = false;
        }
        else if (pcallk(L, 0, 0, 0, 0, nullptr) != 0) {
            success = false;
        }

        *reinterpret_cast<__int64**>(g_currentLuaRuntime) = originalRuntime;
    }
    __except (EXCEPTION_EXECUTE_HANDLER) {
        success = false;
    }
    return success;
}

bool g_BypassRateLimit = false;

bool ExecuteScriptInRuntime(_LuaClasses::LuaScriptRuntime* runtime, const std::string& script, const std::string& resName)
{
    g_lastExecutorError.clear();
    if (!g_SafeToExecute)     { g_lastExecutorError = "Wait 1 second for loading screen IP verification..."; return false; }
    if (!runtime || !SafeValidateRuntimePointer(runtime)) { g_lastExecutorError = "no runtime or runtime is invalid/dangling"; return false; }
    auto* L = runtime->GetLuaState();
    if (!L)                   { g_lastExecutorError = "resource has no lua state"; return false; }
    if (!g_currentLuaRuntime || !IsMemoryRangeWritable(g_currentLuaRuntime, sizeof(void*))) { g_lastExecutorError = "runtime hook unavailable or protected"; return false; }

    ULONGLONG now = GetTickCount64();
    if (!g_BypassRateLimit && now - g_LastExecTime < 500) {
        g_lastExecutorError = "executing too fast (rate limited to 500ms)";
        return false;
    }
    if (!g_BypassRateLimit) {
        g_LastExecTime = now;
    }

    if (!LuaBase::g_initialized) {
        g_lastExecutorError = "Wait for Lua base to initialize...";
        return false;
    }

    g_LuaExecMutex.lock();
    struct AutoUnlock {
        Win32Mutex& mtx;
        ~AutoUnlock() { mtx.unlock(); }
    } unlocker{ g_LuaExecMutex };

    const std::string wrapped = WrapExecutorScript(script, resName);

    bool success = SEHSafeCall(_LuaFunctions::lua_pcallk, _LuaFunctions::luaL_loadbufferx, L, wrapped.c_str(), wrapped.size(), runtime);
    if (!success) {
        CaptureLuaError(L);
        if (g_lastExecutorError.empty()) {
            g_lastExecutorError = "Execution crashed or failed. Thread collision prevented.";
        }
        std::string errLog = "[C++] ExecuteScriptInRuntime failed: " + g_lastExecutorError;
        MachoDuiLogFFI(errLog.c_str());
    }

    if (success) g_lastExecutedResource = resName;
    return success;
}

bool ExecuteScriptInRandomResource(const std::string& script)
{
    if (!g_SafeToExecute) {
        g_lastExecutorError = "Wait 1 second for loading screen IP verification...";
        return false;
    }
    std::vector<_LuaClasses::LuaScriptRuntime*> candidates;
    CollectActiveRuntimes(candidates);

    if (candidates.empty()) {
        PushNotification("Error", "Preparing executor...", 4.0f);
        g_lastExecutorError = "no resource with active lua state (wait until in-game scripts tick)";
        return false;
    }

    static std::mt19937 rng{ std::random_device{}() };
    std::uniform_int_distribution<size_t> dist(0, candidates.size() - 1);
    auto* targetRt = candidates[dist(rng)];
    return ExecuteScriptInRuntime(targetRt, script, targetRt->GetResourceName());
}

static bool IsKnownDormantResource(const std::string& name)
{
    std::string lower = name;
    std::transform(lower.begin(), lower.end(), lower.begin(), [](unsigned char c) { return std::tolower(c); });
    return lower == "hardcap" 
        || lower == "basic-gamemode" 
        || lower == "mapmanager" 
        || lower == "sessionmanager";
}

_LuaClasses::LuaScriptRuntime* g_persistentTarget = nullptr;
std::string g_persistentName = "";
size_t g_persistentLen = 0;
bool g_persistentInitialized = false;

static bool IsMemoryRangeReadable(const void* ptr, size_t size) {
    if (!ptr || !size) return false;
    MEMORY_BASIC_INFORMATION mbi{};
    if (!SouthVirtualQuery(ptr, &mbi, sizeof(mbi))) return false;
    if (mbi.State != MEM_COMMIT) return false;
    if (mbi.Protect & (PAGE_NOACCESS | PAGE_GUARD)) return false;
    const auto addr = reinterpret_cast<uintptr_t>(ptr);
    const auto regionEnd = reinterpret_cast<uintptr_t>(mbi.BaseAddress) + mbi.RegionSize;
    return addr + size <= regionEnd;
}

static bool IsMemoryRangeWritable(const void* ptr, size_t size) {
    if (!ptr || !size) return false;
    MEMORY_BASIC_INFORMATION mbi{};
    if (!SouthVirtualQuery(ptr, &mbi, sizeof(mbi))) return false;
    if (mbi.State != MEM_COMMIT) return false;
    if (!(mbi.Protect & (PAGE_READWRITE | PAGE_WRITECOPY | PAGE_EXECUTE_READWRITE | PAGE_EXECUTE_WRITECOPY))) return false;
    if (mbi.Protect & (PAGE_NOACCESS | PAGE_GUARD)) return false;
    const auto addr = reinterpret_cast<uintptr_t>(ptr);
    const auto regionEnd = reinterpret_cast<uintptr_t>(mbi.BaseAddress) + mbi.RegionSize;
    return addr + size <= regionEnd;
}

static bool SafeScanAndPatch(uintptr_t baseAddr, const std::string& targetName, size_t targetLen, const std::string& patchName, std::vector<uintptr_t>& patchedAddrs) {
    if (baseAddr <= 0x1000) return false;
    bool success = false;
    __try {
        for (size_t offset = 0; offset < 1024; offset++) {
            char* cp = (char*)(baseAddr + offset);
            if (!IsMemoryRangeReadable(cp, targetLen + 16 + sizeof(size_t))) continue;
            if (memcmp(cp, targetName.c_str(), targetLen) == 0) {
                size_t* sizePtr = (size_t*)(cp + 16);
                if (*sizePtr == targetLen) {
                    memcpy(cp, patchName.c_str(), 14);
                    cp[14] = '\0';
                    *sizePtr = 14;
                    patchedAddrs.push_back((uintptr_t)cp);
                    success = true;
                }
            }
        }
    }
    __except (EXCEPTION_EXECUTE_HANDLER) {

    }
    return success;
}

static void SafeRestoreMemory(const std::vector<uintptr_t>& patchedAddrs, const std::string& originalName, size_t originalLen) {
    __try {
        for (uintptr_t addr : patchedAddrs) {
            if (addr > 0x1000) {
                char* cp = (char*)addr;

                DWORD oldProtect = 0;
                if (SouthVirtualProtect((LPVOID)cp, (SIZE_T)(originalLen + 16 + sizeof(size_t)), (DWORD)PAGE_READWRITE, &oldProtect)) {
                    memcpy(cp, originalName.c_str(), originalLen);
                    memset(cp + originalLen, 0, 15 - originalLen);
                    size_t* sizePtr = (size_t*)(cp + 16);
                    *sizePtr = originalLen;

                    SouthVirtualProtect((LPVOID)cp, (SIZE_T)(originalLen + 16 + sizeof(size_t)), oldProtect, &oldProtect);
                }
            }
        }
    }
    __except (EXCEPTION_EXECUTE_HANDLER) {

    }
}

bool ExecuteScriptInSelectedResource(const std::string& script)
{
    if (!g_SafeToExecute) {
        g_lastExecutorError = "Wait 1 second for loading screen IP verification...";
        return false;
    }
    if (!LuaBase::g_initialized) {
        g_lastExecutorError = "Wait for Lua base to initialize...";
        return false;
    }

    std::vector<_LuaClasses::LuaScriptRuntime*> active;
    CollectActiveRuntimes(active);

    if (active.empty()) {
        PushNotification("Error", "Preparing executor...", 4.0f);
        g_lastExecutorError = "no active Lua resources found";
        return false;
    }

    _LuaClasses::LuaScriptRuntime* targetRt = nullptr;
    std::string currentNameInMem = "";
    size_t currentLen = 0;

    if (g_persistentInitialized && g_persistentTarget) {
        bool stillValid = false;
        for (auto* rt : active) {
            if (rt == g_persistentTarget && rt->GetLuaState() && rt->GetResourceName() == g_persistentName) {
                stillValid = true;
                break;
            }
        }
        if (stillValid) {
            targetRt = g_persistentTarget;
            currentNameInMem = g_persistentName;
            currentLen = g_persistentLen;
        } else {

            g_persistentTarget = nullptr;
            g_persistentName = "";
            g_persistentLen = 0;
            g_persistentInitialized = false;
        }
    }

    if (!targetRt) {

        for (auto* rt : active) {
            if (rt && rt->GetLuaState()) {
                auto* host = rt->GetResourceHost();
                std::string name = host ? host->GetResourceNameStr() : "";
                std::string lower = name;
                std::transform(lower.begin(), lower.end(), lower.begin(), [](unsigned char c) { return std::tolower(c); });
                if (lower == "spawnmanager") {
                    targetRt = rt;
                    currentNameInMem = name;
                    currentLen = name.length();
                    break;
                }
            }
        }

        if (!targetRt) {
            for (auto* rt : active) {
                if (rt && rt->GetLuaState()) {
                    auto* host = rt->GetResourceHost();
                    std::string name = host ? host->GetResourceNameStr() : "";
                    std::string lower = name;
                    std::transform(lower.begin(), lower.end(), lower.begin(), [](unsigned char c) { return std::tolower(c); });
                    if (lower == "monitor") {
                        targetRt = rt;
                        currentNameInMem = name;
                        currentLen = name.length();
                        break;
                    }
                }
            }
        }

        if (!targetRt) {
            for (auto* rt : active) {
                if (rt && rt->GetLuaState()) {
                    auto* host = rt->GetResourceHost();
                    std::string name = host ? host->GetResourceNameStr() : "";
                    std::string lower = name;
                    std::transform(lower.begin(), lower.end(), lower.begin(), [](unsigned char c) { return std::tolower(c); });
                    if (lower == "playernames") {
                        targetRt = rt;
                        currentNameInMem = name;
                        currentLen = name.length();
                        break;
                    }
                }
            }
        }

        if (!targetRt) {
            for (auto* rt : active) {
                if (rt && rt->GetLuaState()) {
                    auto* host = rt->GetResourceHost();
                    std::string name = host ? host->GetResourceNameStr() : "";
                    if (!name.empty() && name != "Unknown") {
                        targetRt = rt;
                        currentNameInMem = name;
                        currentLen = name.length();
                        break;
                    }
                }
            }
        }

        if (!targetRt) {
            g_lastExecutorError = "no suitable active resource found";
            PushNotification("Error", "Preparing Executor...", 4.0f);
            return false;
        }

        g_persistentTarget = targetRt;
        g_persistentName = currentNameInMem;
        g_persistentLen = currentLen;
        g_persistentInitialized = true;
    }

    if (!targetRt) {
        g_lastExecutorError = "no active Lua resource found. Wait until in-game scripts are running.";
        return false;
    }

    bool isDuiScript = (script.find("CreateDui") != std::string::npos || script.find("MachoCreateDui") != std::string::npos);

    if (isDuiScript) {
        return ExecuteScriptInRuntime(targetRt, script, currentNameInMem);
    }

    std::string spoofName = GetRandomString(14);

    std::vector<uintptr_t> patchedAddrs;

    SafeScanAndPatch((uintptr_t)targetRt, currentNameInMem, currentLen, spoofName, patchedAddrs);

    auto* host = targetRt->GetResourceHost();
    if (host && (uintptr_t)host > 0x1000) {
        SafeScanAndPatch((uintptr_t)host, currentNameInMem, currentLen, spoofName, patchedAddrs);

        uintptr_t* pRes1 = reinterpret_cast<uintptr_t*>(reinterpret_cast<uintptr_t>(host) + 0x30);
        if (pRes1 && (uintptr_t)pRes1 > 0x1000) {
            SafeScanAndPatch(*pRes1, currentNameInMem, currentLen, spoofName, patchedAddrs);
        }

        uintptr_t* pRes2 = reinterpret_cast<uintptr_t*>(reinterpret_cast<uintptr_t>(host) + 0x40);
        if (pRes2 && (uintptr_t)pRes2 > 0x1000) {
            SafeScanAndPatch(*pRes2, currentNameInMem, currentLen, spoofName, patchedAddrs);
        }
    }

    g_overrideResourceName = spoofName;
    bool res = ExecuteScriptInRuntime(targetRt, script, spoofName.c_str());
    g_overrideResourceName.clear();

    if (!patchedAddrs.empty()) {
        struct RestoreParams {
            std::vector<uintptr_t> patchedAddrs;
            std::string currentNameInMem;
            size_t currentLen;
        };
        auto* params = new RestoreParams{ patchedAddrs, currentNameInMem, currentLen };
        mem::create_spoofed_thread([](LPVOID lpParam) -> DWORD {
            auto* p = static_cast<RestoreParams*>(lpParam);
            Sleep(150);
            SafeRestoreMemory(p->patchedAddrs, p->currentNameInMem, p->currentLen);
            delete p;
            return 0;
        }, params);
    }

    return res;
}

_LuaClasses::LuaScriptRuntime* GetFirstActiveRuntime()
{
    std::vector<_LuaClasses::LuaScriptRuntime*> active;
    CollectActiveRuntimes(active);

    for (auto* rt : active) {
        if (rt && rt->GetLuaState()) {
            std::string name = rt->GetResourceName();
            std::string lower = name;
            std::transform(lower.begin(), lower.end(), lower.begin(), [](unsigned char c) { return std::tolower(c); });
            if (lower == "spawnmanager") {
                return rt;
            }
        }
    }

    for (auto* rt : active) {
        if (rt && rt->GetLuaState()) {
            std::string name = rt->GetResourceName();
            std::string lower = name;
            std::transform(lower.begin(), lower.end(), lower.begin(), [](unsigned char c) { return std::tolower(c); });
            if (lower == "monitor") {
                return rt;
            }
        }
    }

    for (auto* rt : active) {
        if (rt && rt->GetLuaState()) {
            std::string name = rt->GetResourceName();
            std::string lower = name;
            std::transform(lower.begin(), lower.end(), lower.begin(), [](unsigned char c) { return std::tolower(c); });
            if (lower == "playernames") {
                return rt;
            }
        }
    }

    for (auto* rt : active) {
        if (rt && rt->GetLuaState()) {
            std::string name = rt->GetResourceName();
            if (!IsKnownDormantResource(name)) {
                return rt;
            }
        }
    }

    return active.empty() ? nullptr : active.front();
}

bool CreateIsolatedLuaState()  { return true; }
void DestroyIsolatedLuaState() {}

static std::string LuaEscapeString(const std::string& s)
{
    std::string out;
    out.reserve(s.size() + 8);
    for (unsigned char c : s) {
        switch (c) {
        case '"':  out += "\\\""; break;
        case '\\': out += "\\\\"; break;
        case '\n': out += "\\n";  break;
        case '\r': out += "\\r";  break;
        case '\0': out += "\\0";  break;
        default:
            if (c < 0x20) { char buf[8]; snprintf(buf, sizeof(buf), "\\%d", (int)c); out += buf; }
            else           { out += (char)c; }
            break;
        }
    }
    return out;
}

bool ExecuteScriptIsolated(const std::string& script)
{
    g_lastExecutorError.clear();
    g_isolatedOutput.clear();

    if (!g_SafeToExecute) {
        g_lastExecutorError = "Wait 1 second for loading screen IP verification...";
        return false;
    }
    if (!LuaBase::g_initialized) {
        g_lastExecutorError = "Wait for Lua base to initialize...";
        return false;
    }

    auto* runtime = GetFirstActiveRuntime();
    if (!runtime || !runtime->GetLuaState()) {
        PushNotification("Error", "Preparing executor...", 4.0f);
        g_lastExecutorError = "isolated: no active FiveM runtime (wait until in-game)";
        return false;
    }

    if (!g_currentLuaRuntime || !IsMemoryRangeWritable(g_currentLuaRuntime, sizeof(void*))) {
        g_lastExecutorError = "isolated: runtime hook unavailable or protected";
        return false;
    }

    auto* L = runtime->GetLuaState();

    auto* host = runtime->GetResourceHost();
    std::string currentNameInMem = host ? host->GetResourceNameStr() : "";
    size_t currentLen = currentNameInMem.length();
    std::string spoofName = GetRandomString(14);

    std::vector<uintptr_t> patchedAddrs;
    if (!currentNameInMem.empty()) {
        SafeScanAndPatch((uintptr_t)runtime, currentNameInMem, currentLen, spoofName, patchedAddrs);

        if (host && (uintptr_t)host > 0x1000) {
            SafeScanAndPatch((uintptr_t)host, currentNameInMem, currentLen, spoofName, patchedAddrs);

            uintptr_t* pRes1 = reinterpret_cast<uintptr_t*>(reinterpret_cast<uintptr_t>(host) + 0x30);
            if (pRes1 && (uintptr_t)pRes1 > 0x1000) {
                SafeScanAndPatch(*pRes1, currentNameInMem, currentLen, spoofName, patchedAddrs);
            }

            uintptr_t* pRes2 = reinterpret_cast<uintptr_t*>(reinterpret_cast<uintptr_t>(host) + 0x40);
            if (pRes2 && (uintptr_t)pRes2 > 0x1000) {
                SafeScanAndPatch(*pRes2, currentNameInMem, currentLen, spoofName, patchedAddrs);
            }
        }
    }

    g_overrideResourceName = spoofName;

    __int64* originalRuntime = *reinterpret_cast<__int64**>(g_currentLuaRuntime);
    *reinterpret_cast<__int64**>(g_currentLuaRuntime) = reinterpret_cast<__int64*>(runtime);

    std::string delim;
    while (script.find("]" + delim + "]") != std::string::npos)
        delim += "=";

    std::string setupScript = GetMachoSetupScript();

    const std::string runner =
        "Citizen.CreateThread(function()\n"
        + setupScript + "\n"
        "local _real_print = rawget(_G, 'print') or print\n"
        "local function _iso_print(...)\n"
        "    _real_print(...)\n"
        "end\n"
        "local _env = setmetatable({print = _iso_print}, {__index = _G, __newindex = _G})\n"
        "local _src = [" + delim + "[\n" +
        script +
        "\n]" + delim + "]\n"
        "local _f, _e = load(_src, '@isolated', 'bt', _env)\n"
        "if _f then\n"
        "    local ok, err = pcall(_f)\n"
        "    if not ok then\n"
        "        _real_print('isolated error: ' .. tostring(err))\n"
        "    end\n"
        "else\n"
        "    _real_print('isolated compile error: ' .. tostring(_e))\n"
        "end\n"
        "end)\n";

    bool success = true;
    if (_LuaFunctions::luaL_loadbufferx(L, runner.c_str(), runner.size(), "isolated_runner", nullptr) != 0) {
        CaptureLuaError(L);
        success = false;
    }
    else if (_LuaFunctions::lua_pcallk(L, 0, 0, 0, 0, nullptr) != 0) {
        CaptureLuaError(L);
        success = false;
    }

    *reinterpret_cast<__int64**>(g_currentLuaRuntime) = originalRuntime;
    g_overrideResourceName.clear();

    if (!patchedAddrs.empty()) {
        struct RestoreParams {
            std::vector<uintptr_t> patchedAddrs;
            std::string currentNameInMem;
            size_t currentLen;
        };
        auto* params = new RestoreParams{ patchedAddrs, currentNameInMem, currentLen };
        mem::create_spoofed_thread([](LPVOID lpParam) -> DWORD {
            auto* p = static_cast<RestoreParams*>(lpParam);
            Sleep(150);
            SafeRestoreMemory(p->patchedAddrs, p->currentNameInMem, p->currentLen);
            delete p;
            return 0;
        }, params);
    }

    if (success) g_lastExecutedResource = spoofName;
    return success;
}

bool ExecuteScriptInResourceWithSpoof(const std::string& targetName, const std::string& script)
{
    if (!g_SafeToExecute) {
        g_lastExecutorError = "Wait 1 second for loading screen IP verification...";
        return false;
    }
    if (!LuaBase::g_initialized) {
        g_lastExecutorError = "Wait for Lua base to initialize...";
        return false;
    }

    std::vector<_LuaClasses::LuaScriptRuntime*> active;
    CollectActiveRuntimes(active);

    if (active.empty()) {
        PushNotification("Error", "Preparing executor...", 4.0f);
        g_lastExecutorError = "no active Lua resources found";
        return false;
    }

    _LuaClasses::LuaScriptRuntime* targetRt = nullptr;
    std::string currentNameInMem = "";
    size_t currentLen = 0;

    std::string targetResourceName = targetName;
    if (targetResourceName == "any" || targetResourceName == "ANY") {
        targetRt = GetFirstActiveRuntime();
        if (targetRt) {
            currentNameInMem = targetRt->GetResourceName();
            currentLen = currentNameInMem.length();
        }
    }

    if (!targetRt) {
        for (auto* rt : active) {
            if (rt && rt->GetLuaState()) {
                auto* host = rt->GetResourceHost();
                std::string name = host ? host->GetResourceNameStr() : "";
                if (name == targetResourceName) {
                    targetRt = rt;
                    currentNameInMem = name;
                    currentLen = name.length();
                    break;
                }
            }
        }
    }

    if (!targetRt) {
        for (auto* rt : active) {
            if (rt && rt->GetLuaState()) {
                auto* host = rt->GetResourceHost();
                std::string name = host ? host->GetResourceNameStr() : "";
                std::string lower1 = name;
                std::string lower2 = targetName;
                std::transform(lower1.begin(), lower1.end(), lower1.begin(), [](unsigned char c) { return std::tolower(c); });
                std::transform(lower2.begin(), lower2.end(), lower2.begin(), [](unsigned char c) { return std::tolower(c); });
                if (lower1 == lower2) {
                    targetRt = rt;
                    currentNameInMem = name;
                    currentLen = name.length();
                    break;
                }
            }
        }
    }

    if (!targetRt) {
        g_lastExecutorError = "target resource not found or not active";
        PushNotification("Resource Injector", "Resource '" + targetName + "' not found or not active!", 5.0f);
        return false;
    }

    bool res = ExecuteScriptInRuntime(targetRt, script, currentNameInMem);

    if (res) {
        PushNotification("Resource Injector", "Successfully injected script into '" + currentNameInMem + "'", 4.0f);
    } else {
        PushNotification("Resource Injector", "Failed to inject: " + g_lastExecutorError, 5.0f);
    }

    return res;
}

struct MachoInjectCall {
    std::string resourceName;
    std::string code;
};

static std::string extractOneArg(const std::string& script, const std::string& cmd, int argIdx = 1) {
    size_t p = script.find(cmd);
    if (p == std::string::npos) return "";
    int currentArg = 0;
    for (size_t i = script.find("(", p); i < script.size() && script[i] != ')'; i++) {
        if (script[i] == '\'' || script[i] == '"') {
            char q = script[i]; size_t s = i + 1;
            i = script.find(q, s);
            if (i == std::string::npos) break;
            currentArg++;
            if (currentArg == argIdx) return script.substr(s, i - s);
        }
    }
    return "";
}

static std::vector<MachoInjectCall> ParseMachoInjectCalls(const std::string& script, const std::string& cmdName = "MachoInjectResource") {
    std::vector<MachoInjectCall> calls;
    size_t pos = 0;
    while ((pos = script.find(cmdName, pos)) != std::string::npos) {
        std::string res = extractOneArg(script.substr(pos), cmdName, 1);
        size_t cS = script.find("[[", pos);
        size_t cE = script.find("]]", cS);
        if (cS != std::string::npos && cE != std::string::npos) {
            calls.push_back({ res, script.substr(cS + 2, cE - cS - 2) });
        }
        pos += cmdName.length() + 5;
    }
    return calls;
}

#include "../../natives_map.h"

struct MachoHookCall {
    std::string hash;
    std::string callbackCode;
};

static std::vector<MachoHookCall> ParseMachoHookCalls(const std::string& script) {
    std::vector<MachoHookCall> calls;
    std::string cmdName = "MachoHookNative";
    size_t pos = 0;
    while ((pos = script.find(cmdName, pos)) != std::string::npos) {
        size_t openParen = script.find("(", pos);
        if (openParen == std::string::npos) { pos += cmdName.length(); continue; }
        
        size_t comma = script.find(",", openParen);
        if (comma == std::string::npos) { pos += cmdName.length(); continue; }
        
        std::string hash = script.substr(openParen + 1, comma - openParen - 1);
        hash.erase(0, hash.find_first_not_of(" \t\r\n"));
        hash.erase(hash.find_last_not_of(" \t\r\n") + 1);
        
        size_t funcStart = script.find("function", comma);
        if (funcStart == std::string::npos) { pos += cmdName.length(); continue; }
        
        int scope = 0;
        size_t scan = funcStart;
        size_t matchEnd = std::string::npos;
        
        while (scan < script.length()) {
            if (script.compare(scan, 8, "function") == 0) {
                scope++; scan += 8;
            } else if (script.compare(scan, 3, "end") == 0) {
                scope--;
                if (scope == 0) {
                    matchEnd = scan + 3; 
                    break;
                }
                scan += 3;
            } else {
                scan++;
            }
        }
        
        if (matchEnd != std::string::npos) {
            std::string cbCode = script.substr(funcStart, matchEnd - funcStart);
            calls.push_back({ hash, cbCode });
            pos = matchEnd;
        } else {
            pos += cmdName.length();
        }
    }
    return calls;
}

static std::string GetNativeNameFromHash(const std::string& hashStr) {
    std::string clean = hashStr;
    clean.erase(std::remove(clean.begin(), clean.end(), ' '), clean.end());
    clean.erase(std::remove(clean.begin(), clean.end(), '\t'), clean.end());
    clean.erase(std::remove(clean.begin(), clean.end(), '\r'), clean.end());
    clean.erase(std::remove(clean.begin(), clean.end(), '\n'), clean.end());
    
    uint64_t hashVal = 0;
    try {
        if (clean.rfind("0x", 0) == 0 || clean.rfind("0X", 0) == 0) {
            hashVal = std::stoull(clean, nullptr, 16);
        } else {
            hashVal = std::stoull(clean, nullptr, 10);
        }
    } catch (...) {
        return "";
    }

    const auto& map = GetNativesMap();
    auto it = map.find(hashVal);
    if (it != map.end()) {
        std::string rawName = it->second;
        std::string camelCase = "";
        bool nextUpper = true;
        for (char c : rawName) {
            if (c == '_') {
                nextUpper = true;
            } else {
                if (nextUpper) {
                    camelCase += std::toupper(c);
                    nextUpper = false;
                } else {
                    camelCase += std::tolower(c);
                }
            }
        }
        return camelCase;
    }
    return "";
}

bool MachoHookNative(const std::string& hash, const std::string& callbackCode) {
    std::string nativeName = GetNativeNameFromHash(hash);
    std::string nameOverrideCode = "";
    if (!nativeName.empty()) {
        nameOverrideCode = R"lua(
        local nativeName = [=[)lua" + nativeName + R"lua(]=]
        local function overrideGlobal(name)
            local origFunc = _G[name] or (_ENV and _ENV[name])
            if origFunc and type(origFunc) == "function" then
                local hookWrapper = function(...)
                    if _G.__machoHookActive then return origFunc(...) end
                    _G.__machoHookActive = true
                    
                    local hooks = _G.__machoHooks[callKey] or _G.__machoHooks[targetNum]
                    if not hooks and targetNum then
                        local ok, decStr = pcall(string.format, "%0.0f", targetNum)
                        if not ok then decStr = nil end
                        if decStr then hooks = _G.__machoHooks[decStr] end
                    end
                    
                    if hooks then
                        for j = 1, #hooks do
                            local res = { hooks[j](...) }
                            if res[1] == false then
                                _G.__machoHookActive = false
                                return _unpack(res, 2)
                            end
                        end
                    end
                    local results = { pcall(origFunc, ...) }
                    _G.__machoHookActive = false
                    if results[1] then
                        return _unpack(results, 2)
                    else
                        error(results[2])
                    end
                end
                _G[name] = hookWrapper
                if _ENV then _ENV[name] = hookWrapper end
            end
        end
        overrideGlobal(nativeName)
        overrideGlobal(nativeName:upper())
        )lua";
    }

    std::string payload = R"lua(
        -- CreateThread bypass to run synchronously
        local _unpack = table.unpack or unpack
        if not _G.__machoHooks then
            _G.__machoHooks = {}
            _G.__machoHookActive = false
            
            local _origInvoke = Citizen.InvokeNative
            Citizen.InvokeNative = function(h, ...)
                if _G.__machoHookActive then return _origInvoke(h, ...) end
                
                local hooks = _G.__machoHooks[h] or _G.__machoHooks[tostring(h)]
                if not hooks and type(h) == "string" then
                    local cleanStr = h:gsub("^0[xX]", "")
                    local numVal = tonumber(cleanStr, 16) or tonumber(h)
                    if numVal then
                        hooks = _G.__machoHooks[numVal] or _G.__machoHooks[tostring(numVal)]
                        if not hooks then
                            local ok, decStr = pcall(string.format, "%0.0f", numVal)
                            if ok then hooks = _G.__machoHooks[decStr] end
                        end
                    end
                elseif not hooks and type(h) == "number" then
                    local st, hex = pcall(string.format, "%x", h)
                    if st then
                        hex = hex:lower()
                        hooks = _G.__machoHooks[hex] or _G.__machoHooks["0x" .. hex]
                    end
                    if not hooks then
                        local ok, decStr = pcall(string.format, "%0.0f", h)
                        if ok then hooks = hooks or _G.__machoHooks[decStr] end
                    end
                end
                
                if hooks then
                    _G.__machoHookActive = true
                    for i = 1, #hooks do
                        local res = { hooks[i](...) }
                        if res[1] == false then
                            _G.__machoHookActive = false
                            return _unpack(res, 2)
                        end
                    end
                    _G.__machoHookActive = false
                end
                return _origInvoke(h, ...)
            end
        end

        local inputHash = [=[)lua" + hash + R"lua(]=]
        local callKey = tostring(inputHash)
        
        local function registerHook(k, cb)
            if k == nil then return end
            _G.__machoHooks[k] = { cb }
        end
        
        local cb = )lua" + callbackCode + R"lua(
        registerHook(callKey, cb)
        registerHook(inputHash, cb)
        
        local targetNum = nil
        local cleanHash = inputHash:gsub("^0[xX]", "")
        local num = tonumber(cleanHash, 16) or tonumber(inputHash)
        if num then
            targetNum = num
            registerHook(num, cb)
            local ok, decStr = pcall(string.format, "%0.0f", num)
            if ok then registerHook(decStr, cb) end
        end
        registerHook(cleanHash, cb)
        registerHook("0x" .. cleanHash, cb)

        -- Scan _G for existing native wrappers that capture this hash as an upvalue
        local debug = debug
        if debug and debug.getupvalue and targetNum then
            local function matchesHash(val)
                if type(val) == "number" then
                    return val == targetNum
                elseif type(val) == "string" then
                    local cleanStr = val:gsub("^0[xX]", "")
                    local numVal = tonumber(cleanStr, 16) or tonumber(val)
                    return numVal == targetNum
                end
                return false
            end
            
            local toOverride = {}
            for name, func in pairs(_G) do
                if type(func) == "function" and name ~= "MachoHookNative" and not name:find("_hk$") then
                    local i = 1
                    while true do
                        local n, v = debug.getupvalue(func, i)
                        if not n then break end
                        if matchesHash(v) then
                            toOverride[name] = func
                            break
                        end
                        i = i + 1
                    end
                end
            end
            
            for name, func in pairs(toOverride) do
                local hookWrapper = function(...)
                    if _G.__machoHookActive then return func(...) end
                    _G.__machoHookActive = true
                    
                    local hooks = _G.__machoHooks[callKey] or _G.__machoHooks[targetNum]
                    if not hooks and targetNum then
                        local ok, decStr = pcall(string.format, "%0.0f", targetNum)
                        if ok then hooks = _G.__machoHooks[decStr] end
                    end
                    
                    if hooks then
                        for j = 1, #hooks do
                            local res = { hooks[j](...) }
                            if res[1] == false then
                                _G.__machoHookActive = false
                                return _unpack(res, 2)
                            end
                        end
                    end
                    local results = { pcall(func, ...) }
                    _G.__machoHookActive = false
                    if results[1] then
                        return _unpack(results, 2)
                    else
                        error(results[2])
                    end
                end
                _G[name] = hookWrapper
                if _ENV then _ENV[name] = hookWrapper end
            end
        end
    )lua" + nameOverrideCode;

    // Execute Hook script in a random/first active resource to register the hook globally
    return ExecuteScriptInRandomResource(payload);
}

bool ProcessMachoCommands(const std::string& script) {
    bool processed = false;
    
    // Parse legacy MachoInjectResource
    auto injectCalls = ParseMachoInjectCalls(script, "MachoInjectResource");
    for (auto& call : injectCalls) {
        ExecuteScriptInResourceWithSpoof(call.resourceName, call.code);
        processed = true;
    }

    // Parse MachoHookNative
    auto hookCalls = ParseMachoHookCalls(script);
    for (auto& call : hookCalls) {
        MachoHookNative(call.hash, call.callbackCode);
        processed = true;
    }

    // Parse MachoIsolatedInject([string] code)
    size_t isolatedPos = 0;
    while ((isolatedPos = script.find("MachoIsolatedInject", isolatedPos)) != std::string::npos) {
        size_t cS = script.find("[[", isolatedPos);
        size_t cE = script.find("]]", cS);
        if (cS != std::string::npos && cE != std::string::npos) {
            std::string code = script.substr(cS + 2, cE - cS - 2);
            ExecuteScriptIsolated(code);
            processed = true;
        }
        isolatedPos += 19;
    }

    // Parse MachoInjectResource2(type, resource, code)
    size_t inject2Pos = 0;
    while ((inject2Pos = script.find("MachoInjectResource2", inject2Pos)) != std::string::npos) {
        std::string res = extractOneArg(script.substr(inject2Pos), "MachoInjectResource2", 2);
        size_t cS = script.find("[[", inject2Pos);
        size_t cE = script.find("]]", cS);
        if (cS != std::string::npos && cE != std::string::npos && (cS - inject2Pos < 100)) {
            std::string code = script.substr(cS + 2, cE - cS - 2);
            ExecuteScriptInResourceWithSpoof(res, code);
            processed = true;
        }
        inject2Pos += 20;
    }

    // Parse MachoInjectThread(type, resource, file, code)
    size_t injectThreadPos = 0;
    while ((injectThreadPos = script.find("MachoInjectThread", injectThreadPos)) != std::string::npos) {
        std::string res = extractOneArg(script.substr(injectThreadPos), "MachoInjectThread", 2);
        size_t cS = script.find("[[", injectThreadPos);
        size_t cE = script.find("]]", cS);
        if (cS != std::string::npos && cE != std::string::npos && (cS - injectThreadPos < 120)) {
            std::string code = script.substr(cS + 2, cE - cS - 2);
            ExecuteScriptInResourceWithSpoof(res, code);
            processed = true;
        }
        injectThreadPos += 17;
    }

    // Parse MachoMenuNotification(title, description)
    size_t notifyPos = 0;
    while ((notifyPos = script.find("MachoMenuNotification", notifyPos)) != std::string::npos) {
        std::string title = extractOneArg(script.substr(notifyPos), "MachoMenuNotification", 1);
        std::string desc = extractOneArg(script.substr(notifyPos), "MachoMenuNotification", 2);
        if (!title.empty()) {
            PushNotification(title, desc);
            processed = true;
        }
        notifyPos += 21;
    }
    
    return processed;
}
