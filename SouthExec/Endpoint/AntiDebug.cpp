// Leaked By Cry Service & Siraj
#include "AntiDebug.h"
#include <psapi.h>
#include <tlhelp32.h>
#include <winternl.h>
#include <iostream>
#include <algorithm>
#include <cctype>

#pragma comment(lib, "ntdll.lib")

static pNtRemoveProcessDebug NtRemoveProcessDebug = nullptr;
static pNtSetInformationDebugObject NtSetInformationDebugObject = nullptr;

static const std::vector<std::string> BadProcessNames = {
    "procmon64.exe",
    "codecracker.exe",
    "ida.exe",
    "idag.exe",
    "idaw.exe",
    "idaq.exe",
    "idau.exe",
    "scylla.exe",
    "de4dot.exe",
    "de4dotmodded.exe",
    "protection_id.exe",
    "ollydbg.exe",
    "x64dbg.exe",
    "x32dbg.exe",
    "x96dbg.exe",
    "x64netdumper.exe",
    "petools.exe",
    "dnspy.exe",
    "windbg.exe",
    "reshacker.exe",
    "simpleassembly.exe",
    "processhacker.exe",
    "procmon.exe",
    "processmonitor.exe",
    "qt5core.exe",
    "importrec.exe",
    "immunitydebugger.exe",
    "megadumper.exe",
    "cheatengine-x86_64.exe",
    "dump.exe",
    "dbgclr.exe",
    "wireshark.exe",
    "hxd.exe",

    "cmd.exe",
    "powershell.exe",
    "python.exe",
    "pythonw.exe",
    "py.exe",
    "pycharm64.exe",
    "pycharm.exe",
    "ipython.exe",
    "jupyter.exe",
    "node.exe",
    "nodejs.exe",
    "bash.exe",
    "sh.exe",
    "zsh.exe",
    "fish.exe",
    "terminal.exe",
    "conhost.exe",
    "wt.exe",
    "windowsterminal.exe",
    "alacritty.exe",
    "kitty.exe",
    "hyper.exe",
    "code.exe",
    "code-insiders.exe",
    "vscodium.exe",
    "cursor.exe",
    "sublime_text.exe",
    "sublime.exe",
    "atom.exe",
    "notepad++.exe",
    "vim.exe",
    "nvim.exe",
    "neovim.exe",
    "emacs.exe",
    "nano.exe",
    "micro.exe"
};

static const std::vector<std::string> BadWindowTitles = {
    "ollydbg",
    "ida",
    "disassembly",
    "scylla",
    "debug",
    "[cpu",
    "immunity",
    "windbg",
    "x32dbg",
    "x64dbg",
    "x96dbg",
    "import reconstructor",
    "dumper",
    "command prompt",
    "powershell",
    "python",
    "terminal",
    "cmd",
    "bash",
    "shell",
    "console",
    "command line",
    "administrator: command prompt",
    "windows powershell",
    "windows terminal",
    "visual studio code",
    "vscode",
    "pycharm",
    "jupyter",
    "ipython"
};

static bool LoadNtFunctions() {
    HMODULE hNtdll = GetModuleHandleA("ntdll.dll");
    if (!hNtdll) return false;

    NtRemoveProcessDebug = (pNtRemoveProcessDebug)GetProcAddress(hNtdll, "NtRemoveProcessDebug");
    NtSetInformationDebugObject = (pNtSetInformationDebugObject)GetProcAddress(hNtdll, "NtSetInformationDebugObject");

    return NtRemoveProcessDebug && NtSetInformationDebugObject;
}

static std::string ToLower(std::string str) {
    std::transform(str.begin(), str.end(), str.begin(),
        [](unsigned char c) { return std::tolower(c); });
    return str;
}

namespace AntiDebug {

    bool IsDebuggerPresent() {
        return ::IsDebuggerPresent() != FALSE;
    }

    bool CheckRemoteDebuggerPresent() {
        HANDLE hProcess = GetCurrentProcess();
        BOOL isDebuggerPresent = FALSE;

        typedef BOOL(WINAPI* pCheckRemoteDebuggerPresent)(HANDLE, PBOOL);
        pCheckRemoteDebuggerPresent func = (pCheckRemoteDebuggerPresent)
            GetProcAddress(GetModuleHandleA("kernel32.dll"), "CheckRemoteDebuggerPresent");

        if (func) {
            func(hProcess, &isDebuggerPresent);
        }

        return isDebuggerPresent != FALSE;
    }

    bool CheckDebugPort() {
        HANDLE hProcess = GetCurrentProcess();
        PVOID debugPort = nullptr;
        NTSTATUS status;
        ULONG returnLength;

        status = NtQueryInformationProcess(
            hProcess,
            ProcessDebugPort,
            &debugPort,
            sizeof(debugPort),
            &returnLength
        );

        if (status == STATUS_SUCCESS) {
            if (debugPort == (PVOID)-1) {
                return true;
            }
        }

        return false;
    }

    bool CheckKernelDebugger() {
        SYSTEM_KERNEL_DEBUGGER_INFORMATION skdi;
        NTSTATUS status;
        ULONG returnLength;

        status = NtQuerySystemInformation(
            (SYSTEM_INFORMATION_CLASS)SystemKernelDebuggerInformation,
            &skdi,
            sizeof(skdi),
            &returnLength
        );

        if (status == STATUS_SUCCESS) {
            if (skdi.KernelDebuggerEnabled && !skdi.KernelDebuggerNotPresent) {
                return true;
            }
        }

        return false;
    }

    bool DetachFromDebugger() {
        if (!NtSetInformationDebugObject || !NtRemoveProcessDebug) {
            if (!LoadNtFunctions()) return false;
        }

        HANDLE hProcess = GetCurrentProcess();
        HANDLE hDebugObject = nullptr;
        NTSTATUS status;
        ULONG returnLength;
        DWORD dwFlags = 0;

        status = NtQueryInformationProcess(
            hProcess,
            (PROCESSINFOCLASS)30, 
            &hDebugObject,
            sizeof(hDebugObject),
            &returnLength
        );

        if (status != STATUS_SUCCESS) {
            return false;
        }

        status = NtSetInformationDebugObject(
            hDebugObject,
            DebugObjectFlags,
            &dwFlags,
            sizeof(dwFlags),
            &returnLength
        );

        if (status != STATUS_SUCCESS) {
            return false;
        }

        status = NtRemoveProcessDebug(hProcess, hDebugObject);

        if (status != STATUS_SUCCESS) {
            return false;
        }

        NtClose(hDebugObject);

        return status == STATUS_SUCCESS;
    }

    bool HideThreadFromDebugger(HANDLE hThread) {
        NTSTATUS status = NtSetInformationThread(
            hThread,
            (THREADINFOCLASS)17, 
            nullptr,
            0
        );

        return status == STATUS_SUCCESS;
    }

    void HideAllThreadsFromDebugger() {
        HANDLE hThreadSnap = CreateToolhelp32Snapshot(TH32CS_SNAPTHREAD, 0);
        if (hThreadSnap == INVALID_HANDLE_VALUE) {
            return;
        }

        THREADENTRY32 te32;
        te32.dwSize = sizeof(THREADENTRY32);
        DWORD currentProcessId = GetCurrentProcessId();

        if (Thread32First(hThreadSnap, &te32)) {
            do {
                if (te32.th32OwnerProcessID == currentProcessId) {
                    HANDLE hThread = OpenThread(THREAD_SET_INFORMATION, FALSE, te32.th32ThreadID);
                    if (hThread) {
                        HideThreadFromDebugger(hThread);
                        CloseHandle(hThread);
                    }
                }
            } while (Thread32Next(hThreadSnap, &te32));
        }

        CloseHandle(hThreadSnap);
    }

    void ProtectMemoryDump() {
        HMODULE hModule = GetModuleHandleA(nullptr);
        if (!hModule) return;

        PIMAGE_DOS_HEADER pDosHeader = (PIMAGE_DOS_HEADER)hModule;
        PIMAGE_NT_HEADERS pNtHeaders = (PIMAGE_NT_HEADERS)((BYTE*)hModule + pDosHeader->e_lfanew);

        DWORD oldProtect;

        VirtualProtect(hModule, 30, PAGE_READWRITE, &oldProtect);
        memset(hModule, 0, 30);
        VirtualProtect(hModule, 30, oldProtect, &oldProtect);

        if (pNtHeaders) {

            VirtualProtect(&pNtHeaders->FileHeader, sizeof(IMAGE_FILE_HEADER), PAGE_READWRITE, &oldProtect);
            memset(&pNtHeaders->FileHeader, 0, sizeof(IMAGE_FILE_HEADER));
            VirtualProtect(&pNtHeaders->FileHeader, sizeof(IMAGE_FILE_HEADER), oldProtect, &oldProtect);

            VirtualProtect(&pNtHeaders->OptionalHeader, sizeof(IMAGE_OPTIONAL_HEADER), PAGE_READWRITE, &oldProtect);
            memset(&pNtHeaders->OptionalHeader, 0, sizeof(IMAGE_OPTIONAL_HEADER));
            VirtualProtect(&pNtHeaders->OptionalHeader, sizeof(IMAGE_OPTIONAL_HEADER), oldProtect, &oldProtect);
        }
    }

    bool DetectVirtualMachine() {

        HKEY hKey;
        if (RegOpenKeyExA(HKEY_LOCAL_MACHINE, "HARDWARE\\DEVICEMEMORY", 0, KEY_READ, &hKey) == ERROR_SUCCESS) {
            RegCloseKey(hKey);
            return true;
        }
        if (RegOpenKeyExA(HKEY_LOCAL_MACHINE, "HARDWARE\\DESCRIPTION\\System", 0, KEY_READ, &hKey) == ERROR_SUCCESS) {
            RegCloseKey(hKey);
            return true;
        }

        int cpuInfo[4] = { 0 };
        __cpuid(cpuInfo, 1);
        if ((cpuInfo[2] >> 31) & 1) {
            return true;
        }

        HANDLE hSnapshot = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
        if (hSnapshot != INVALID_HANDLE_VALUE) {
            PROCESSENTRY32 pe32;
            pe32.dwSize = sizeof(PROCESSENTRY32);

            if (Process32First(hSnapshot, &pe32)) {
                do {
                    std::string processName = ToLower(pe32.szExeFile);
                    if (processName.find("vmsrvc") != std::string::npos ||
                        processName.find("vmusrvc") != std::string::npos ||
                        processName.find("vmtools") != std::string::npos ||
                        processName.find("vboxservice") != std::string::npos ||
                        processName.find("vboxtray") != std::string::npos) {
                        CloseHandle(hSnapshot);
                        return true;
                    }
                } while (Process32Next(hSnapshot, &pe32));
            }
            CloseHandle(hSnapshot);
        }

        return false;
    }

    bool DetectSandbox() {

        HMODULE hSbie = GetModuleHandleA("SbieDll.dll");
        if (hSbie) return true;

        HANDLE hSnapshot = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
        if (hSnapshot != INVALID_HANDLE_VALUE) {
            PROCESSENTRY32 pe32;
            pe32.dwSize = sizeof(PROCESSENTRY32);

            if (Process32First(hSnapshot, &pe32)) {
                do {
                    std::string processName = ToLower(pe32.szExeFile);
                    if (processName.find("sandboxie") != std::string::npos ||
                        processName.find("vmsandbox") != std::string::npos ||
                        processName.find("sandbox") != std::string::npos) {
                        CloseHandle(hSnapshot);
                        return true;
                    }
                } while (Process32Next(hSnapshot, &pe32));
            }
            CloseHandle(hSnapshot);
        }

        return false;
    }

    bool DetectEmulation() {
        DWORD startTick = GetTickCount();
        Sleep(500);
        DWORD endTick = GetTickCount();

        return (endTick - startTick) < 450;
    }

    void ScanAndKillDebuggers() {
        HANDLE hSnapshot = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
        if (hSnapshot == INVALID_HANDLE_VALUE) {
            return;
        }

        PROCESSENTRY32 pe32;
        pe32.dwSize = sizeof(PROCESSENTRY32);

        if (Process32First(hSnapshot, &pe32)) {
            do {
                std::string processName = ToLower(pe32.szExeFile);

                for (const auto& badName : BadProcessNames) {
                    if (processName == ToLower(badName)) {
                        HANDLE hProcess = OpenProcess(PROCESS_TERMINATE, FALSE, pe32.th32ProcessID);
                        if (hProcess) {
        //                             TerminateProcess(hProcess, 0);
                            CloseHandle(hProcess);
                        }
                        break;
                    }
                }

                HWND hWnd = GetTopWindow(nullptr);
                while (hWnd) {
                    char windowTitle[256];
                    if (GetWindowTextA(hWnd, windowTitle, 256)) {
                        std::string title = ToLower(windowTitle);
                        for (const auto& badTitle : BadWindowTitles) {
                            if (title.find(ToLower(badTitle)) != std::string::npos) {
                                DWORD processId;
                                GetWindowThreadProcessId(hWnd, &processId);
                                if (processId == pe32.th32ProcessID) {
                                    HANDLE hProcess = OpenProcess(PROCESS_TERMINATE, FALSE, processId);
                                    if (hProcess) {
        //                                         TerminateProcess(hProcess, 0);
                                        CloseHandle(hProcess);
                                    }
                                }
                                break;
                            }
                        }
                    }
                    hWnd = GetNextWindow(hWnd, GW_HWNDNEXT);
                }

            } while (Process32Next(hSnapshot, &pe32));
        }

        CloseHandle(hSnapshot);
    }

    void InitializeAntiDebug() {

        LoadNtFunctions();

        HideAllThreadsFromDebugger();

        ProtectMemoryDump();

        ScanAndKillDebuggers();
    }

    void RunAntiDebugChecks() {

        if (IsDebuggerPresent()) {

        //             TerminateProcess(GetCurrentProcess(), 0);
        }

        if (CheckRemoteDebuggerPresent()) {
        //             TerminateProcess(GetCurrentProcess(), 0);
        }

        if (CheckDebugPort()) {
        //             TerminateProcess(GetCurrentProcess(), 0);
        }

        if (CheckKernelDebugger()) {
        //             TerminateProcess(GetCurrentProcess(), 0);
        }

        if (DetectVirtualMachine()) {
        //             TerminateProcess(GetCurrentProcess(), 0);
        }

        if (DetectSandbox()) {
        //             TerminateProcess(GetCurrentProcess(), 0);
        }

        if (DetectEmulation()) {
        //             TerminateProcess(GetCurrentProcess(), 0);
        }

        DetachFromDebugger();

        ScanAndKillDebuggers();
    }

}
