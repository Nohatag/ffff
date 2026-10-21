// Leaked By Cry Service & Siraj
#include "api-exec.h"
#include "../../scanner/scanner.h"
#include <fstream>
#include <algorithm>
#include <random>
#include <iostream>
#include <vector>
#include <string>

int selectedRuntimeIndex = 0;
_LuaClasses::LuaScriptRuntime* selectedRuntime = nullptr;

static std::string g_lastExecutorError;
static std::string g_lastExecutedResource;

const char* GetLastExecutorError()
{
    return g_lastExecutorError.c_str();
}

const char* GetLastExecutedResourceName()
{
    return g_lastExecutedResource.c_str();
}

static void CaptureLuaError(__int64* L)
{
    g_lastExecutorError.clear();
    if (!L) {
        g_lastExecutorError = "invalid lua state";
        return;
    }

    if (_LuaFunctions::lua_tolstring) {
        const char* err = _LuaFunctions::lua_tolstring(L, -1, nullptr);
        if (err && err[0])
            g_lastExecutorError = err;
    }

    if (g_lastExecutorError.empty())
        g_lastExecutorError = "lua execution failed";

    if (_LuaFunctions::lua_gettop && _LuaFunctions::lua_settop) {
        const int top = _LuaFunctions::lua_gettop(L);
        if (top > 0)
            _LuaFunctions::lua_settop(L, top - 1);
    }
}

static std::string WrapExecutorScript(const std::string& script)
{
    return "Citizen.CreateThread(function()\n" + script + "\nend)";
}

static void CollectActiveRuntimes(std::vector<_LuaClasses::LuaScriptRuntime*>& out)
{
    out.clear();

    for (auto* rt : _LuaGlobals::_AvailableRuntimes) {
        if (!rt || !rt->GetLuaState())
            continue;
        if (std::find(out.begin(), out.end(), rt) == out.end())
            out.push_back(rt);
    }

    if (g_currentLuaRuntime) {
        auto* hookRt = g_currentLuaRuntime->GetLuaScriptRuntime();
        if (hookRt && hookRt->GetLuaState()) {
            if (std::find(out.begin(), out.end(), hookRt) == out.end())
                out.push_back(hookRt);
        }
    }
}

static _LuaClasses::LuaScriptRuntime* ResolveSelectedRuntime()
{
    auto& resources = _LuaGlobals::_ServerResources;
    if (resources.empty())
        return nullptr;

    if (selectedRuntime) {
        for (const auto& entry : resources) {
            if (entry.runtime == selectedRuntime)
                return selectedRuntime;
        }
        selectedRuntime = nullptr;
    }

    if (selectedRuntimeIndex < 0 || selectedRuntimeIndex >= (int)resources.size())
        selectedRuntimeIndex = 0;

    selectedRuntime = ResolveRuntimeForEntry(resources[selectedRuntimeIndex]);
    return selectedRuntime;
}

void SyncSelectedRuntimeFromIndex()
{
    auto& resources = _LuaGlobals::_ServerResources;
    if (resources.empty()) {
        selectedRuntimeIndex = 0;
        selectedRuntime = nullptr;
        return;
    }

    if (selectedRuntimeIndex < 0 || selectedRuntimeIndex >= (int)resources.size())
        selectedRuntimeIndex = 0;

    selectedRuntime = ResolveRuntimeForEntry(resources[selectedRuntimeIndex]);
}

bool ExecuteScriptInRuntime(_LuaClasses::LuaScriptRuntime* runtime, const std::string& script)
{
    g_lastExecutorError.clear();

    if (!runtime) {
        g_lastExecutorError = "no runtime";
        return false;
    }

    auto* luastate = runtime->GetLuaState();
    if (!luastate) {
        g_lastExecutorError = "resource has no lua state";
        return false;
    }

    if (!g_currentLuaRuntime) {
        g_lastExecutorError = "runtime hook unavailable";
        return false;
    }

    *reinterpret_cast<__int64**>(g_currentLuaRuntime) =
        reinterpret_cast<__int64*>(runtime);

    const std::string wrapped = WrapExecutorScript(script);

    if (luaL_loadstring(luastate, wrapped.c_str()) != 0) {
        CaptureLuaError(luastate);
        return false;
    }

    if (lua_pcall(luastate, 0, 0, 0) != 0) {
        CaptureLuaError(luastate);
        return false;
    }

    g_lastExecutedResource = runtime->GetResourceName();
    return true;
}

bool ExecuteScriptInRandomResource(const std::string& script)
{
    std::vector<_LuaClasses::LuaScriptRuntime*> candidates;
    CollectActiveRuntimes(candidates);

    if (candidates.empty()) {
        g_lastExecutorError = "no resource with active lua state (wait until in-game scripts tick)";
        return false;
    }

    static std::mt19937 rng{ std::random_device{}() };
    std::uniform_int_distribution<size_t> dist(0, candidates.size() - 1);
    return ExecuteScriptInRuntime(candidates[dist(rng)], script);
}

bool ExecuteScriptInSelectedResource(const std::string& script)
{
    if (_LuaGlobals::_ServerResources.empty()) {
        g_lastExecutorError = "no server resources";
        return false;
    }

    if (selectedRuntimeIndex < 0 || selectedRuntimeIndex >= (int)_LuaGlobals::_ServerResources.size())
        selectedRuntimeIndex = 0;

    const auto& entry = _LuaGlobals::_ServerResources[selectedRuntimeIndex];
    auto* runtime = FindRuntimeByName(entry.name);

    if (!runtime || !runtime->GetLuaState()) {
        if (entry.lifecycle == ResourceLifecycle::Running) {
            g_lastExecutorError =
                "'" + entry.name + "' is [running] â€” Lua not hooked yet. Pick [active] or enable Random resource.";
        } else if (entry.lifecycle == ResourceLifecycle::Stopped) {
            g_lastExecutorError = "'" + entry.name + "' is [stopped] on the server.";
        } else if (entry.lifecycle == ResourceLifecycle::Idle) {
            g_lastExecutorError = "'" + entry.name + "' is [idle] â€” start the resource on the server first.";
        } else {
            g_lastExecutorError = "'" + entry.name + "' has no active Lua state right now.";
        }
        return false;
    }

    return ExecuteScriptInRuntime(runtime, script);
}

_LuaClasses::LuaScriptRuntime* GetFirstActiveRuntime()
{
    std::vector<_LuaClasses::LuaScriptRuntime*> active;
    CollectActiveRuntimes(active);
    return active.empty() ? nullptr : active.front();
}

static __int64* g_isolatedLuaState = nullptr;
static std::string g_isolatedOutput;
static std::string g_citizenCode;

static bool SerializeCitizenEnvironment()
{
    if (!g_citizenCode.empty())
        return true;

    auto* sourceRuntime = GetFirstActiveRuntime();
    if (!sourceRuntime || !sourceRuntime->GetLuaState())
        return false;

    const char* serializeScript = R"(
_G.__citizen_ser = [[
-- Citizen environment for isolated executor
Citizen = Citizen
CreateThread = Citizen.CreateThread
Wait = Citizen.Wait
SetTimeout = Citizen.SetTimeout
]]
)";

    if (!ExecuteScriptInRuntime(sourceRuntime, serializeScript))
        return false;

    auto* L = sourceRuntime->GetLuaState();
    if (L && _LuaFunctions::lua_getglobal && _LuaFunctions::lua_tolstring) {
        const int top = _LuaFunctions::lua_gettop ? _LuaFunctions::lua_gettop(L) : 0;
        _LuaFunctions::lua_getglobal(L, "__citizen_ser");
        const char* value = _LuaFunctions::lua_tolstring(L, -1, nullptr);
        if (value && value[0]) {
            g_citizenCode = value;
        }
        if (_LuaFunctions::lua_settop)
            _LuaFunctions::lua_settop(L, top);
    }

    return !g_citizenCode.empty();
}

static bool CopyCitizenEnvironment(__int64* destState, __int64* sourceState)
{
    if (!destState || !sourceState)
        return false;

    const char* copyScript = R"(
-- Get Citizen from source (we'll inject this differently)
if Citizen then
    _G.Citizen = Citizen
    _G.CreateThread = Citizen.CreateThread
    _G.Wait = Citizen.Wait  
    _G.SetTimeout = Citizen.SetTimeout
end
)";

    if (_LuaFunctions::luaL_loadbufferx(destState, copyScript, strlen(copyScript), "copy_citizen", nullptr) == 0) {
        _LuaFunctions::lua_pcallk(destState, 0, 0, 0, 0, nullptr);
    }

    return true;
}

bool CreateIsolatedLuaState()
{
    if (g_isolatedLuaState)
        return true;

    if (!_LuaFunctions::luaL_newstate || !_LuaFunctions::luaL_openlibs) {
        g_lastExecutorError = "isolated executor not available (lua functions not found)";
        return false;
    }

    g_isolatedLuaState = _LuaFunctions::luaL_newstate();
    if (!g_isolatedLuaState) {
        g_lastExecutorError = "failed to create isolated lua state";
        return false;
    }

    _LuaFunctions::luaL_openlibs(g_isolatedLuaState);

    auto* sourceRuntime = GetFirstActiveRuntime();
    if (sourceRuntime && sourceRuntime->GetLuaState()) {

    }

    return true;
}

void DestroyIsolatedLuaState()
{
    if (g_isolatedLuaState && _LuaFunctions::lua_close) {
        _LuaFunctions::lua_close(g_isolatedLuaState);
        g_isolatedLuaState = nullptr;
    }
}

bool ExecuteScriptIsolated(const std::string& script)
{
    g_lastExecutorError.clear();
    g_isolatedOutput.clear();

    if (!CreateIsolatedLuaState())
        return false;

    std::string wrappedScript = R"(
_G.__iso_out = _G.__iso_out or ""
_G.__iso_prints = _G.__iso_prints or {}

function print(...)
    local args = {...}
    local output = ""
    for i, v in ipairs(args) do
        if i > 1 then output = output .. "\t" end
        output = output .. tostring(v)
    end
    _G.__iso_out = _G.__iso_out .. output .. string.char(30)
    table.insert(_G.__iso_prints, output)
end

)";

    wrappedScript += script;

    if (_LuaFunctions::luaL_loadbufferx(g_isolatedLuaState, wrappedScript.c_str(), wrappedScript.size(), "isolated", nullptr) != 0) {
        CaptureLuaError(g_isolatedLuaState);
        return false;
    }

    if (_LuaFunctions::lua_pcallk(g_isolatedLuaState, 0, 0, 0, 0, nullptr) != 0) {
        CaptureLuaError(g_isolatedLuaState);
        return false;
    }

    if (_LuaFunctions::lua_getglobal && _LuaFunctions::lua_tolstring) {
        const int top = _LuaFunctions::lua_gettop ? _LuaFunctions::lua_gettop(g_isolatedLuaState) : 0;
        _LuaFunctions::lua_getglobal(g_isolatedLuaState, "__iso_out");
        const char* value = _LuaFunctions::lua_tolstring(g_isolatedLuaState, -1, nullptr);
        if (value && value[0]) {
            g_isolatedOutput = value;
        }
        if (_LuaFunctions::lua_settop)
            _LuaFunctions::lua_settop(g_isolatedLuaState, top);
    }

    auto* activeRuntime = GetFirstActiveRuntime();
    if (activeRuntime && !g_isolatedOutput.empty()) {
        std::string forwardScript = "-- Isolated prints:\n";

        size_t start = 0;
        while (start < g_isolatedOutput.size()) {
            size_t end = g_isolatedOutput.find('\x1e', start);
            if (end == std::string::npos) end = g_isolatedOutput.size();
            std::string line = g_isolatedOutput.substr(start, end - start);
            if (!line.empty()) {
                forwardScript += "print('[isolated] " + line + "')\n";
            }
            if (end == g_isolatedOutput.size()) break;
            start = end + 1;
        }

        ExecuteScriptInRuntime(activeRuntime, forwardScript);
    }

    g_lastExecutedResource = "[isolated]";
    return true;
}

const char* GetIsolatedOutput()
{
    return g_isolatedOutput.c_str();
}
