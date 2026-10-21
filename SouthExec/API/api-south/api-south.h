// Leaked By Cry Service & Siraj
#pragma once
#include <windows.h>
#include <vector>
#include <string>

namespace _LuaClasses {
    class ResourceHost;
    class ResourceTemp;
    class LuaScriptRuntime;
    class LuaScriptRuntimeHolder;

    class ResourceTemp
    {
    public:
        char* GetResourceName() noexcept;
    private:
        char pad_1[0xD0];
        void* ResourceName;
    };

    class ResourceHost
    {
    public:
        char* GetResourceName() noexcept;
        std::string GetResourceNameStr();
    private:
        char pad_1[0x30];
        ResourceTemp* Resource;
        char pad_2[0x8];
        ResourceTemp* Resource2;
    };

    class LuaScriptRuntime
    {
    public:
        __int64* GetLuaState();
        ResourceHost* GetResourceHost();
        std::string GetResourceName();
    };

    class LuaScriptRuntimeHolder
    {
    public:
        LuaScriptRuntime* GetLuaScriptRuntime();
    };
}

typedef __int64(__fastcall* lua_pcallk_t)(__int64* L, int nargs, int nresults, int errfunc, __int64 ctx, __int64* k);
typedef __int64(__fastcall* luaL_loadfilex_t)(__int64* L, const char* filename, const char* mode);
typedef int(__fastcall* luaL_loadbufferx_t)(__int64* L, const char* buff, unsigned __int64 size, const char* name, const char* mode);
typedef __int64*(__fastcall* luaL_newstate_t)();
typedef void(__fastcall* lua_close_t)(__int64* L);
typedef void(__fastcall* luaL_openlibs_t)(__int64* L);
typedef void(__cdecl* lua_getglobal_t)(__int64* L, const char* name);
typedef const char* (__cdecl* lua_tolstring_t)(__int64* L, int idx, size_t* len);
typedef int(__cdecl* lua_gettop_t)(__int64* L);
typedef void(__cdecl* lua_settop_t)(__int64* L, int idx);

typedef void(__cdecl* lua_createtable_t)(__int64* L, int narr, int nrec);
typedef void(__cdecl* lua_setfield_t)(__int64* L, int idx, const char* k);
typedef int(__cdecl* lua_getfield_t)(__int64* L, int idx, const char* k);
typedef void(__cdecl* lua_pushvalue_t)(__int64* L, int idx);
typedef void(__cdecl* lua_pushstring_t)(__int64* L, const char* s);
typedef void(__cdecl* lua_pushcclosure_t)(__int64* L, void* fn, int n);
typedef int(__cdecl* lua_type_t)(__int64* L, int idx);
typedef void(__cdecl* lua_pushnil_t)(__int64* L);
typedef int(__cdecl* lua_next_t)(__int64* L, int idx);
typedef void(__cdecl* lua_rawgeti_t)(__int64* L, int idx, __int64 n);
typedef void(__cdecl* lua_rawseti_t)(__int64* L, int idx, __int64 n);

namespace _LuaFunctions {

    extern lua_pcallk_t        lua_pcallk;
    extern luaL_loadfilex_t    luaL_loadfilex;
    extern luaL_loadbufferx_t  luaL_loadbufferx;
    extern lua_getglobal_t     lua_getglobal;
    extern lua_tolstring_t     lua_tolstring;
    extern lua_gettop_t        lua_gettop;
    extern lua_settop_t        lua_settop;
    extern luaL_newstate_t     luaL_newstate;
    extern lua_close_t         lua_close;
    extern luaL_openlibs_t     luaL_openlibs;
}

enum class ResourceLifecycle {
    Active,   
    Running,  
    Idle,     
    Stopped   
};

struct ResourceEntry {
    std::string name;
    _LuaClasses::LuaScriptRuntime* runtime = nullptr;
    ResourceLifecycle lifecycle = ResourceLifecycle::Idle;
};

const char* GetResourceLifecycleLabel(ResourceLifecycle life) noexcept;
ResourceLifecycle DetectResourceLifecycle(_LuaClasses::LuaScriptRuntime* runtime, void* fxResource, bool registeredOnServer) noexcept;
_LuaClasses::LuaScriptRuntime* ResolveRuntimeForEntry(const ResourceEntry& entry) noexcept;
bool ReadLuaGlobalString(_LuaClasses::LuaScriptRuntime* runtime, const char* globalName, std::string& out) noexcept;

const char* GetLastExecutorError();
const char* GetLastExecutedResourceName();
bool ExecuteScriptInRuntime(_LuaClasses::LuaScriptRuntime* runtime, const std::string& script, const std::string& resName);
bool ExecuteScriptInSelectedResource(const std::string& script);
bool ExecuteScriptInRandomResource(const std::string& script);
void SyncSelectedRuntimeFromIndex();
_LuaClasses::LuaScriptRuntime* GetFirstActiveRuntime();

namespace _LuaGlobals {
    extern HMODULE CitizenResourcesMetadataLua;
    extern HMODULE CitizenScriptingLua;

    extern std::vector<_LuaClasses::LuaScriptRuntime*> _AvailableRuntimes;
    extern std::vector<ResourceEntry> _ServerResources;
}

void RefreshServerResources(bool allowLua = true) noexcept;
void LockRuntimes();
void UnlockRuntimes();

class Collector {
public:
    static void CollectScriptRuntimes() noexcept;
};

_LuaClasses::LuaScriptRuntime* FindRuntimeByName(const std::string& name) noexcept;

class LuaBase {
public:
    static void Initialize();
    static inline bool g_initialized = false;
};

extern bool g_runtimeFound;

extern _LuaClasses::LuaScriptRuntimeHolder* g_currentLuaRuntime;

struct PendingScript {
    _LuaClasses::LuaScriptRuntime* runtime;
    std::string script;
    std::string resourceName;
};

void QueueLuaScript(_LuaClasses::LuaScriptRuntime* runtime, const std::string& script, const std::string& resName);

#define lua_pcall(L, n, r, f)          _LuaFunctions::lua_pcallk(L, (n), (r), (f), 0, nullptr)

#define luaL_loadfile(L, f)            _LuaFunctions::luaL_loadfilex(L, f, nullptr)

#define luaL_loadbuffer(L, s, sz, n)   _LuaFunctions::luaL_loadbufferx(L, s, sz, n, nullptr)

inline int luaL_loadstring(__int64* L, const char* s) {
    return luaL_loadbuffer(L, s, strlen(s), s);
}

#define LUA_MULTRET -1

#define luaL_dostring(L, s) (luaL_loadstring(L, s) || lua_pcall(L, 0, LUA_MULTRET, 0))

