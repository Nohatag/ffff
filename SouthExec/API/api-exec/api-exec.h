// Leaked By Cry Service & Siraj
#pragma once
#include <Windows.h>
#include <vector>
#include <string>
#include "../api-south/api-south.h"

bool SafeValidateRuntimePointer(_LuaClasses::LuaScriptRuntime* rt);
bool ExecuteScriptInRuntime(_LuaClasses::LuaScriptRuntime* runtime, const std::string& script, const std::string& resName);
bool ExecuteScriptInSelectedResource(const std::string& script);
bool ExecuteScriptInRandomResource(const std::string& script);
bool ExecuteScriptInResourceWithSpoof(const std::string& targetName, const std::string& script);
bool ExecuteScriptIsolated(const std::string& script);
bool ProcessMachoCommands(const std::string& script);

bool CreateIsolatedLuaState();
void DestroyIsolatedLuaState();
const char* GetIsolatedOutput();

void SyncSelectedRuntimeFromIndex();

const char* GetLastExecutorError();
const char* GetLastExecutedResourceName();
_LuaClasses::LuaScriptRuntime* GetFirstActiveRuntime();

extern int selectedRuntimeIndex;
extern _LuaClasses::LuaScriptRuntime* selectedRuntime;
extern bool g_SafeToExecute;

