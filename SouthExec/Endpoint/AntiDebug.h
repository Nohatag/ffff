// Leaked By Cry Service & Siraj
#pragma once
#include <windows.h>
#include <tlhelp32.h>
#include <winternl.h>
#include <vector>
#include <string>

#define STATUS_SUCCESS 0x00000000
#define STATUS_INFO_LENGTH_MISMATCH 0xC0000004

typedef enum _SYSTEMINFORMATIONCLASS {
    SystemKernelDebuggerInformation = 35
} SYSTEMINFORMATIONCLASS;

typedef enum _DEBUGOBJECTINFOCLASS {
    DebugObjectFlags = 1
} DEBUGOBJECTINFOCLASS;

typedef struct _SYSTEM_KERNEL_DEBUGGER_INFORMATION {
    BOOLEAN KernelDebuggerEnabled;
    BOOLEAN KernelDebuggerNotPresent;
} SYSTEM_KERNEL_DEBUGGER_INFORMATION, *PSYSTEM_KERNEL_DEBUGGER_INFORMATION;

typedef NTSTATUS(NTAPI* pNtQueryInformationProcess)(
    HANDLE ProcessHandle,
    PROCESSINFOCLASS ProcessInformationClass,
    PVOID ProcessInformation,
    ULONG ProcessInformationLength,
    PULONG ReturnLength
    );

typedef NTSTATUS(NTAPI* pNtClose)(
    HANDLE Handle
    );

typedef NTSTATUS(NTAPI* pNtRemoveProcessDebug)(
    HANDLE ProcessHandle,
    HANDLE DebugObjectHandle
    );

typedef NTSTATUS(NTAPI* pNtSetInformationDebugObject)(
    HANDLE DebugObjectHandle,
    DEBUGOBJECTINFOCLASS DebugObjectInformationClass,
    PVOID DebugObjectInformation,
    ULONG DebugObjectInformationLength,
    PULONG ReturnLength
    );

typedef NTSTATUS(NTAPI* pNtQuerySystemInformation)(
    SYSTEMINFORMATIONCLASS SystemInformationClass,
    PVOID SystemInformation,
    ULONG SystemInformationLength,
    PULONG ReturnLength
    );

typedef NTSTATUS(NTAPI* pNtSetInformationThread)(
    HANDLE ThreadHandle,
    THREADINFOCLASS ThreadInformationClass,
    PVOID ThreadInformation,
    ULONG ThreadInformationLength
    );

namespace AntiDebug {
    bool IsDebuggerPresent();
    bool CheckRemoteDebuggerPresent();
    bool CheckDebugPort();
    bool CheckKernelDebugger();
    bool DetachFromDebugger();
    bool HideThreadFromDebugger(HANDLE hThread);
    void HideAllThreadsFromDebugger();
    void ProtectMemoryDump();
    bool DetectVirtualMachine();
    bool DetectSandbox();
    bool DetectEmulation();
    void ScanAndKillDebuggers();
    void InitializeAntiDebug();
    void RunAntiDebugChecks();
}
