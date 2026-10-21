// Leaked By Cry Service & Siraj
#pragma once
#include <vector>
#include <string>
#include <Windows.h>
#include <winternl.h>
#ifndef XR
#define XR(str) str
#endif
#include "Psapi.h"
#include <time.h>
#include <fstream>
#include <random>

typedef BOOL(WINAPI* pfVirtualProtect)(
	IN  LPVOID lpAddress,
	IN  SIZE_T dwSize,
	IN  DWORD  flNewProtect,
	OUT PDWORD lpflOldProtect
	);

typedef NTSTATUS(NTAPI* pfnNtCreateThreadEx)(
	OUT PHANDLE ThreadHandle,
	IN ACCESS_MASK DesiredAccess,
	IN POBJECT_ATTRIBUTES ObjectAttributes,
	IN HANDLE ProcessHandle,
	IN PVOID StartRoutine,
	IN PVOID Argument,
	IN ULONG CreateFlags,
	IN SIZE_T ZeroBits,
	IN SIZE_T StackSize,
	IN SIZE_T MaximumStackSize,
	IN PVOID AttributeList
	);

#define RVA(addr, size) ((PBYTE)(addr + *(DWORD*)(addr + ((size) - 4)) + size))

namespace mem {

	struct CLIENT_ID
	{
		HANDLE UniqueProcess;
		HANDLE UniqueThread;
	};

	struct TEB
	{
		NT_TIB NtTib;
		PVOID EnvironmentPointer;
		CLIENT_ID ClientId;
		PVOID ActiveRpcHandle;
		PVOID ThreadLocalStoragePointer;
		struct PEB* ProcessEnvironmentBlock;

	};

	struct PEB_LDR_DATA
	{
		ULONG Length;
		BOOLEAN Initialized;
		HANDLE SsHandle;
		LIST_ENTRY InLoadOrderModuleList;
		LIST_ENTRY InMemoryOrderModuleList;
		LIST_ENTRY InInitializationOrderModuleList;
		PVOID EntryInProgress;
		BOOLEAN ShutdownInProgress;
		HANDLE ShutdownThreadId;
	};

	struct PEB
	{
		BOOLEAN InheritedAddressSpace;
		BOOLEAN ReadImageFileExecOptions;
		BOOLEAN BeingDebugged;
		union
		{
			BOOLEAN BitField;
			struct
			{
				BOOLEAN ImageUsesLargePages : 1;
				BOOLEAN IsProtectedProcess : 1;
				BOOLEAN IsImageDynamicallyRelocated : 1;
				BOOLEAN SkipPatchingUser32Forwarders : 1;
				BOOLEAN IsPackagedProcess : 1;
				BOOLEAN IsAppContainer : 1;
				BOOLEAN IsProtectedProcessLight : 1;
				BOOLEAN SpareBits : 1;
			};
		};
		HANDLE Mutant;
		PVOID ImageBaseAddress;
		PEB_LDR_DATA* Ldr;
	};

	struct UNICODE_STRING
	{
		USHORT Length;
		USHORT MaximumLength;
		PWCH Buffer;
	};

	struct LDR_DATA_TABLE_ENTRY
	{
		LIST_ENTRY InLoadOrderLinks;
		LIST_ENTRY InMemoryOrderLinks;
		union
		{
			LIST_ENTRY InInitializationOrderLinks;
			LIST_ENTRY InProgressLinks;
		};
		PVOID DllBase;
		PVOID EntryPoint;
		ULONG SizeOfImage;
		UNICODE_STRING FullDllName;
		UNICODE_STRING BaseDllName;
	};

	inline uintptr_t get_main_image_base() {
		PEB* ProcessEnvironmentBlock = ((PEB*)((TEB*)((TEB*)NtCurrentTeb())->ProcessEnvironmentBlock));
		return (uintptr_t)ProcessEnvironmentBlock->ImageBaseAddress;
	}

	inline uintptr_t get_module(LPCSTR _lpModuleName)
	{
		auto GetFileNameFromPath = [](wchar_t* Path) -> wchar_t*
			{
				wchar_t* LastSlash = NULL;
				for (DWORD i = 0; Path[i] != 0; i++)
				{
					if (Path[i] == '\\')
						LastSlash = &Path[i + 1];
				}
				return LastSlash;
			};

		auto RemoveFileExtension = [](wchar_t* FullFileName, wchar_t* OutputBuffer, DWORD OutputBufferSize)	-> wchar_t*
			{
				wchar_t* LastDot = NULL;
				for (DWORD i = 0; FullFileName[i] != NULL; i++)
					if (FullFileName[i] == '.')
						LastDot = &FullFileName[i];

				for (DWORD j = 0; j < OutputBufferSize; j++)
				{
					OutputBuffer[j] = FullFileName[j];
					if (&FullFileName[j] == LastDot)
					{
						OutputBuffer[j] = NULL;
						break;
					}
				}
				OutputBuffer[OutputBufferSize - 1] = NULL;
				return OutputBuffer;
			};

		DWORD ModuleNameLength = (DWORD)strlen(_lpModuleName) + 1;
		DWORD NewBufferSize = sizeof(wchar_t) * ModuleNameLength;

		wchar_t* lpModuleName = (wchar_t*)_alloca(NewBufferSize);

		for (DWORD i = 0; i < ModuleNameLength; i++)
			lpModuleName[i] = _lpModuleName[i];

		PEB* ProcessEnvironmentBlock = ((PEB*)((TEB*)((TEB*)NtCurrentTeb())->ProcessEnvironmentBlock));

		if (lpModuleName == nullptr || lpModuleName == L"" || _lpModuleName == nullptr || _lpModuleName == (""))
		{
			RtlSecureZeroMemory(lpModuleName, NewBufferSize);
			return (uintptr_t)(ProcessEnvironmentBlock->ImageBaseAddress);
		}

		PEB_LDR_DATA* Ldr = ProcessEnvironmentBlock->Ldr;

		LIST_ENTRY* ModuleLists[3] = { 0,0,0 };
		ModuleLists[0] = &Ldr->InLoadOrderModuleList;
		ModuleLists[1] = &Ldr->InMemoryOrderModuleList;
		ModuleLists[2] = &Ldr->InInitializationOrderModuleList;

		for (int j = 0; j < 3; j++)
		{
			for (LIST_ENTRY* pListEntry = ModuleLists[j]->Flink; pListEntry != ModuleLists[j]; pListEntry = pListEntry->Flink)
			{
				LDR_DATA_TABLE_ENTRY* pEntry = (LDR_DATA_TABLE_ENTRY*)((BYTE*)pListEntry - sizeof(LIST_ENTRY) * j);

				if (_wcsicmp(pEntry->BaseDllName.Buffer, lpModuleName) == 0)
				{
					RtlSecureZeroMemory(lpModuleName, NewBufferSize);
					return (uintptr_t)pEntry->DllBase;
				}

				wchar_t* FileName = GetFileNameFromPath(pEntry->FullDllName.Buffer);
				if (!FileName)
					continue;

				if (_wcsicmp(FileName, lpModuleName) == 0)
				{
					RtlSecureZeroMemory(lpModuleName, NewBufferSize);
					return (uintptr_t)pEntry->DllBase;
				}

				wchar_t FileNameWithoutExtension[256];
				RemoveFileExtension(FileName, FileNameWithoutExtension, 256);

				if (_wcsicmp(FileNameWithoutExtension, lpModuleName) == 0)
				{
					RtlSecureZeroMemory(lpModuleName, NewBufferSize);
					return (uintptr_t)pEntry->DllBase;
				}
			}
		}

		return 0;
	}

	inline HMODULE get_module_ex(LPCSTR lpModuleName)
	{
		return reinterpret_cast<HMODULE>(get_module(lpModuleName));
	}

	inline uintptr_t get_pattern(LPCSTR lpModuleName, LPCSTR lpPattern)
	{
		uintptr_t moduleAdress = get_module(lpModuleName);

		if (!moduleAdress)
			return 0;

		static auto patternToByte = [](const char* pattern)
			{
				auto       bytes = std::vector<int>{};
				const auto start = const_cast<char*>(pattern);
				const auto end = const_cast<char*>(pattern) + strlen(pattern);

				for (auto current = start; current < end; ++current)
				{
					if (*current == '?')
					{
						++current;
						if (*current == '?')
							++current;
						bytes.push_back(-1);
					}
					else { bytes.push_back(strtoul(current, &current, 16)); }
				}
				return bytes;
			};

		const auto dosHeader = (PIMAGE_DOS_HEADER)moduleAdress;
		const auto ntHeaders = (PIMAGE_NT_HEADERS)((std::uint8_t*)moduleAdress + dosHeader->e_lfanew);

		const auto sizeOfImage = ntHeaders->OptionalHeader.SizeOfImage;
		auto       patternBytes = patternToByte(lpPattern);
		const auto scanBytes = reinterpret_cast<std::uint8_t*>(moduleAdress);

		const auto s = patternBytes.size();
		const auto d = patternBytes.data();

		for (auto i = 0ul; i < sizeOfImage - s; ++i)
		{
			bool found = true;
			for (auto j = 0ul; j < s; ++j)
			{
				if (scanBytes[i + j] != d[j] && d[j] != -1)
				{
					found = false;
					break;
				}
			}
			if (found) { return reinterpret_cast<uintptr_t>(&scanBytes[i]); }
		}
		return 0;
	}

	inline uintptr_t get_export(LPCSTR DllName, LPCSTR FunctionName)
	{
		PIMAGE_DOS_HEADER pIDH;
		PIMAGE_NT_HEADERS pINH;
		PIMAGE_EXPORT_DIRECTORY pIED;

		HMODULE hModule;
		PDWORD Address, Name;
		PWORD Ordinal;

		DWORD i;

		hModule = get_module_ex(DllName);

		if (!hModule)
			return 0;

		pIDH = (PIMAGE_DOS_HEADER)hModule;

		if (pIDH->e_magic != IMAGE_DOS_SIGNATURE)
			return 0;

		pINH = (PIMAGE_NT_HEADERS)((LPBYTE)hModule + pIDH->e_lfanew);

		if (pINH->Signature != IMAGE_NT_SIGNATURE)
			return 0;

		if (pINH->OptionalHeader.DataDirectory[IMAGE_DIRECTORY_ENTRY_EXPORT].VirtualAddress == 0)
			return 0;

		pIED = (PIMAGE_EXPORT_DIRECTORY)((LPBYTE)hModule + pINH->OptionalHeader.DataDirectory[IMAGE_DIRECTORY_ENTRY_EXPORT].VirtualAddress);

		Address = (PDWORD)((LPBYTE)hModule + pIED->AddressOfFunctions);
		Name = (PDWORD)((LPBYTE)hModule + pIED->AddressOfNames);

		Ordinal = (PWORD)((LPBYTE)hModule + pIED->AddressOfNameOrdinals);

		for (i = 0; i < pIED->AddressOfFunctions; i++)
		{
			if (!strcmp(FunctionName, (char*)hModule + Name[i]))
			{
				return (uintptr_t)(PVOID)((LPBYTE)hModule + Address[Ordinal[i]]);
			}
		}

		return 0;
	}

	static std::string read_registry(LPCSTR subkey, LPCSTR name, DWORD type)
	{
		HKEY key;
		CHAR value[1024];
		DWORD valueLength = 1024;

		return value;
	}

	inline void write_registry(LPCSTR subkey, LPCSTR name, DWORD type, LPCSTR data)
	{
		HKEY key;
	}

	template<typename T>
	inline void write(uintptr_t address, T& value)
	{
		if (reinterpret_cast<void*>(address) == nullptr || !address)
			return;

		*(T*)address = value;
	}

	template<typename T>
	inline T read(uintptr_t address)
	{
		if (reinterpret_cast<void*>(address) == nullptr || !address)
			return T();
		else
			return *(T*)address;
	}

	inline void start_thread(void* routine)
	{
		if (!routine)
			return;

		HANDLE hThread = INVALID_HANDLE_VALUE;
		CONTEXT context;

		typedef enum _THREADINFOCLASS {
			ThreadHideFromDebugger = 17
		} THREADINFOCLASS;

		auto NtCreateThreadEx = reinterpret_cast<NTSTATUS(__stdcall*)(PHANDLE, ACCESS_MASK, PVOID, HANDLE, PVOID, PVOID, ULONG, SIZE_T, SIZE_T, SIZE_T, PVOID)>(get_export(XR("ntdll.dll"), XR("NtCreateThreadEx")));
		auto NtSetInformationThread = reinterpret_cast<ULONG(__stdcall*)(HANDLE, THREADINFOCLASS, PVOID, ULONG)>(get_export(XR("ntdll.dll"), XR("NtSetInformationThread")));

		auto GetThreadContextF = reinterpret_cast<BOOL(__stdcall*)(HANDLE, LPCONTEXT)>(get_export(XR("kernel32.dll"), XR("GetThreadContext")));
		auto SetThreadContextF = reinterpret_cast<BOOL(__stdcall*)(HANDLE, const CONTEXT*)>(get_export(XR("kernel32.dll"), XR("SetThreadContext")));

		auto ResumeThreadF = reinterpret_cast<DWORD(__stdcall*)(HANDLE)>(get_export(XR("kernel32.dll"), XR("ResumeThread")));
		auto CloseHandleF = reinterpret_cast<BOOL(__stdcall*)(HANDLE)>(get_export(XR("kernel32.dll"), XR("CloseHandle")));

		NtCreateThreadEx(&hThread, THREAD_ALL_ACCESS, 0, ((HANDLE)(ULONG_PTR)-1), (LPTHREAD_START_ROUTINE)0 /* Start address */, 0, 0x1 /* Suspended */, 0, 0, 0, 0);

		context.ContextFlags = CONTEXT_ALL;
		GetThreadContextF(hThread, &context);

		context.Rcx = (DWORD64)routine;
		SetThreadContextF(hThread, &context);

		ResumeThreadF(hThread);
		NtSetInformationThread(hThread, ThreadHideFromDebugger, 0, 0);

		CloseHandleF(hThread);
	}

	inline HANDLE create_spoofed_thread(void* routine, void* parameter = nullptr)
	{
		if (!routine)
			return INVALID_HANDLE_VALUE;

		HANDLE hThread = INVALID_HANDLE_VALUE;
		CONTEXT context;

		typedef enum _THREADINFOCLASS {
			ThreadHideFromDebugger = 17
		} THREADINFOCLASS;

		auto NtCreateThreadEx = reinterpret_cast<NTSTATUS(__stdcall*)(PHANDLE, ACCESS_MASK, PVOID, HANDLE, PVOID, PVOID, ULONG, SIZE_T, SIZE_T, SIZE_T, PVOID)>(get_export(XR("ntdll.dll"), XR("NtCreateThreadEx")));
		auto NtSetInformationThread = reinterpret_cast<ULONG(__stdcall*)(HANDLE, THREADINFOCLASS, PVOID, ULONG)>(get_export(XR("ntdll.dll"), XR("NtSetInformationThread")));

		auto GetThreadContextF = reinterpret_cast<BOOL(__stdcall*)(HANDLE, LPCONTEXT)>(get_export(XR("kernel32.dll"), XR("GetThreadContext")));
		auto SetThreadContextF = reinterpret_cast<BOOL(__stdcall*)(HANDLE, const CONTEXT*)>(get_export(XR("kernel32.dll"), XR("SetThreadContext")));

		auto ResumeThreadF = reinterpret_cast<DWORD(__stdcall*)(HANDLE)>(get_export(XR("kernel32.dll"), XR("ResumeThread")));
		auto CloseHandleF = reinterpret_cast<BOOL(__stdcall*)(HANDLE)>(get_export(XR("kernel32.dll"), XR("CloseHandle")));

		void* spoofedStartAddress = reinterpret_cast<void*>(get_export(XR("kernel32.dll"), XR("BaseThreadInitThunk")));
		if (!spoofedStartAddress) {
			spoofedStartAddress = reinterpret_cast<void*>(get_export(XR("ntdll.dll"), XR("RtlUserThreadStart")));
		}

		NTSTATUS status = NtCreateThreadEx(&hThread, THREAD_ALL_ACCESS, nullptr, ((HANDLE)(ULONG_PTR)-1), spoofedStartAddress, nullptr, 0x1 /* Suspended */, 0, 0, 0, nullptr);
		if (!NT_SUCCESS(status) || hThread == INVALID_HANDLE_VALUE) {
			return INVALID_HANDLE_VALUE;
		}

		context.ContextFlags = CONTEXT_ALL;
		if (GetThreadContextF(hThread, &context)) {
			context.Rcx = reinterpret_cast<DWORD64>(routine);
			context.Rdx = reinterpret_cast<DWORD64>(parameter);
			SetThreadContextF(hThread, &context);
		}

		ResumeThreadF(hThread);
		NtSetInformationThread(hThread, ThreadHideFromDebugger, 0, 0);

		return hThread;
	}

	inline void start_safe_thread(void* routine)
	{
		if (!routine) return;

		HANDLE ntdllThread = INVALID_HANDLE_VALUE;

		auto NtCreateThreadEx = reinterpret_cast<pfnNtCreateThreadEx>(get_export(XR("ntdll.dll"), XR("NtCreateThreadEx")));
		auto CloseHandleF = reinterpret_cast<BOOL(__stdcall*)(HANDLE)>(get_export(XR("kernel32.dll"), XR("CloseHandle")));

		NTSTATUS status = NtCreateThreadEx(&ntdllThread, THREAD_ALL_ACCESS, 0, ((HANDLE)(ULONG_PTR)-1), (LPTHREAD_START_ROUTINE)routine /* Start address */, 0, 0 /* Suspended */, 0, 0, 0, 0);
		CloseHandleF(ntdllThread);

	}

	inline std::string read_file_data(std::string filename)
	{
		std::string buffer = { };
		std::ifstream file(filename);

		buffer = std::string(std::istreambuf_iterator<char>(file), std::istreambuf_iterator<char>());
		file.close();

		return buffer;
	}

	inline std::string random_string(size_t length)
	{
		std::string result{};
		static std::string chars = XR("ACBDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz");

		static std::random_device rd;
		static std::mt19937 generator(rd());
		std::uniform_int_distribution<size_t> distribution(0, chars.length() - 1);

		for (size_t i = 0; i < length; i++)
			result += chars[distribution(generator)];

		return result;
	}

	struct process_entry {
		ULONG handle_count;
		wchar_t name[60];
		uint32_t id;
	};

	inline std::string narrow_from_wide(const wchar_t* ws) {
		if (!ws) {
			return {};
		}

		int size_needed = WideCharToMultiByte(CP_UTF8, 0, ws, -1, nullptr, 0, nullptr, nullptr);
		if (size_needed <= 0) {
			return {};
		}

		std::string out(static_cast<size_t>(size_needed - 1), '\0');
		WideCharToMultiByte(CP_UTF8, 0, ws, -1, out.data(), size_needed, nullptr, nullptr);
		return out;
	}

	class process_enumerator {
	public:
		process_enumerator() {
			using NtQuerySystemInformationFn = NTSTATUS WINAPI(IN SYSTEM_INFORMATION_CLASS, OUT PVOID, IN ULONG, OUT PULONG);
			const auto NtQuerySystemInformation = reinterpret_cast<NtQuerySystemInformationFn*>(mem::get_export(XR("ntdll.dll"), XR("NtQuerySystemInformation"))/*GetProcAddress(GetModuleHandleA("ntdll.dll"),"NtQuerySystemInformation")*/);
			ULONG return_length;
			while (NtQuerySystemInformation(SystemProcessInformation, buffer.get(), static_cast<ULONG>(buffer_len), &return_length) < 0) {
				buffer = std::unique_ptr<uint8_t[]>(new uint8_t[return_length]);
				buffer_len = return_length;
			}
		}
		bool next(process_entry& entry) {
			if (next_offset >= buffer_len) {
				return false;
			}
			const auto pi = reinterpret_cast<const SYSTEM_PROCESS_INFORMATION*>(buffer.get() + next_offset);
			next_offset += pi->NextEntryOffset != 0 ? pi->NextEntryOffset : buffer_len - next_offset;

			entry.handle_count = pi->HandleCount;
			entry.id = static_cast<uint32_t>((size_t)pi->UniqueProcessId);

			memset(&entry.name, 0, sizeof(entry.name));
			memcpy(&entry.name, pi->ImageName.Buffer, (((pi->ImageName.Length) < (sizeof(entry.name) - 1)) ? (pi->ImageName.Length) : (sizeof(entry.name) - 1)));

			return true;
		}
	private:
		std::unique_ptr<uint8_t[]> buffer;
		size_t buffer_len = 0;
		size_t next_offset = 0;
	};

	inline ULONG64 get_process_id(const char* pProcess) {

		process_entry entry;

		for (process_enumerator processes{};
			processes.next(entry); ) {

			auto name = narrow_from_wide(entry.name);

			if (!strcmp(name.c_str(), pProcess))
				return entry.id;
		}

		return 0;
	}

	inline std::string get_current_process_name(std::string cmp, std::vector<std::string> skips) {
		process_entry entry;

		for (process_enumerator processes{};
			processes.next(entry); ) {

			auto s = narrow_from_wide(entry.name);

			if (s.find(cmp) != std::string::npos)
			{
				bool found = false;

				for (auto& skip : skips)
					if (s.find(skip) != std::string::npos)
					{
						found = true;
					}

				if (!found)
					return s;
				else
					continue;
			}
		}

		return ("null");
	}

	inline uintptr_t read_chain(uintptr_t address, std::vector<uintptr_t> pointers) {
		auto addr = address;
		for (int i = 0; i < pointers.size(); i++)
		{
			addr = *(uintptr_t*)(addr);
			addr += pointers[i];
		}
		return addr;
	}

	inline PIMAGE_NT_HEADERS get_header(PVOID module) {
		return (PIMAGE_NT_HEADERS)((PBYTE)module + PIMAGE_DOS_HEADER(module)->e_lfanew);
	}

	inline PBYTE sig(PVOID module, DWORD size, LPCSTR pattern, LPCSTR mask) {

		auto checkMask = [](PBYTE buffer, LPCSTR pattern, LPCSTR mask) -> BOOL
			{
				for (auto x = buffer; *mask; pattern++, mask++, x++) {
					auto addr = *(BYTE*)(pattern);
					if (addr != *x && *mask != '?')
						return FALSE;
				}

				return TRUE;
			};

		for (auto x = 0; x < size - strlen(mask); x++) {

			auto addr = (PBYTE)module + x;
			if (checkMask(addr, pattern, mask)) {
				return addr;
			}
		}

		return NULL;
	}

	inline PBYTE sig(PVOID base, LPCSTR pattern, LPCSTR mask) {

		auto header = get_header(base);

		if (!header) return nullptr;

		auto section = IMAGE_FIRST_SECTION(header);

		if (!section) return nullptr;

		for (auto x = 0; x < header->FileHeader.NumberOfSections; x++, section++) {

			if (!memcmp(section->Name, XR(".text"), 5) || !memcmp(section->Name, XR("PAGE"), 4)) {

				auto addr = sig((PBYTE)base + section->VirtualAddress, section->Misc.VirtualSize, pattern, mask);

				if (addr) {

					return addr;
				}
			}
		}

		return NULL;
	}

	inline PBYTE sig(LPCSTR module, LPCSTR pattern, LPCSTR mask) {
		return RVA(sig((PVOID)mem::get_module(module), pattern, mask), 7);
	}

	inline PVOID detour_pointer(LPCSTR module, LPCSTR pattern, LPCSTR mask, void* detour) {
		return _InterlockedExchangePointer((volatile PVOID*)sig(module, pattern, mask), (PVOID)detour);
	}

	inline void patch(BYTE* destination, BYTE* source, size_t size)
	{
		memcpy(destination, source, size);
	}

	using AquireOrRelease = NTSTATUS(NTAPI*)();

	typedef std::pair <PLIST_ENTRY, int> Pair;

	typedef struct _LDR_MODULE {

		LIST_ENTRY              InLoadOrderModuleList;
		LIST_ENTRY              InMemoryOrderModuleList;
		LIST_ENTRY              InInitializationOrderModuleList;
		PVOID                   BaseAddress;
		PVOID                   EntryPoint;
		ULONG                   SizeOfImage;
		UNICODE_STRING          FullDllName;
		UNICODE_STRING          BaseDllName;
		ULONG                   Flags;
		SHORT                   LoadCount;
		SHORT                   TlsIndex;
		LIST_ENTRY              HashTableEntry;
		ULONG                   TimeDateStamp;

	} LDR_MODULE, * PLDR_MODULE;

	struct Info
	{
		PLIST_ENTRY pListHead = nullptr, pListMod = nullptr;
		PEB_LDR_DATA* pLdr = nullptr;
		UNICODE_STRING* pModName = nullptr;
		PLDR_MODULE pLdrMod = NULL;
		uintptr_t PebAddr = NULL;
		AquireOrRelease PebLock;
		AquireOrRelease ReleasePebLock;
		char tModName[MAX_PATH];

	} inline UnlinkData;

	inline uintptr_t GetPEB()
	{
		return reinterpret_cast<uintptr_t>((PPEB)__readgsqword(0x60));
	}

	enum : int
	{

		InLoadOrderModuleList = 36, InMemoryOrderModuleList = 28, InInitializationOrderModuleList = 20
	};

	inline void clear_list_entries(int EntryOffset, const char* ModName)
	{
		UnlinkData.PebLock();

		while (UnlinkData.pListMod->Flink != UnlinkData.pListHead)
		{
			UnlinkData.pListMod = UnlinkData.pListMod->Flink;
			UnlinkData.pModName = (UNICODE_STRING*)(((DWORD)(UnlinkData.pListMod)) + EntryOffset);

			WORD DllNameLength = (UnlinkData.pModName->Length) / 2;

			int n = DllNameLength + 1;

			while (n--)
			{
				if (n != DllNameLength)
				{
					UnlinkData.tModName[n] = (CHAR)(*((UnlinkData.pModName->Buffer) + (n)));
					continue;

				}

				UnlinkData.tModName[n] = (char)'\000';

			}

			if (strstr(UnlinkData.tModName, ModName))
			{
				if (EntryOffset == InLoadOrderModuleList)
				{
					UnlinkData.pLdrMod = (PLDR_MODULE)(((DWORD)(UnlinkData.pListMod)));

					UnlinkData.pLdrMod->HashTableEntry.Blink->Flink = UnlinkData.pLdrMod->HashTableEntry.Flink;
					UnlinkData.pLdrMod->HashTableEntry.Flink->Blink = UnlinkData.pLdrMod->HashTableEntry.Blink;
				}

				UnlinkData.pListMod->Blink->Flink = UnlinkData.pListMod->Flink;
				UnlinkData.pListMod->Flink->Blink = UnlinkData.pListMod->Blink;
			}

		}

		UnlinkData.ReleasePebLock();
	}

	inline void unlink_module_from_ped(const char* TargetModName)
	{

		UnlinkData.PebLock = (AquireOrRelease)(mem::get_export(XR("ntdll.dll"), XR("RtlAcquirePebLock")));
		UnlinkData.ReleasePebLock = (AquireOrRelease)(mem::get_export(XR("ntdll.dll"), XR("RtlReleasePebLock")));

		UnlinkData.PebAddr = GetPEB();

		UnlinkData.pLdr = (PEB_LDR_DATA*)(DWORD*)*(DWORD*)(UnlinkData.PebAddr + 12);

		std::vector <Pair> pairs;

		pairs.push_back(std::make_pair(&(UnlinkData.pLdr->InLoadOrderModuleList), InLoadOrderModuleList));
		pairs.push_back(std::make_pair(&(UnlinkData.pLdr->InMemoryOrderModuleList), InMemoryOrderModuleList));
		pairs.push_back(std::make_pair(&(UnlinkData.pLdr->InInitializationOrderModuleList), InInitializationOrderModuleList));

		for (std::vector<std::pair<PLIST_ENTRY, int>>::const_iterator iterator = pairs.begin(); iterator != pairs.end(); ++iterator)
		{
			UnlinkData.pListHead = UnlinkData.pListMod = iterator->first;
			clear_list_entries(iterator->second, TargetModName);
		}
	}

}
