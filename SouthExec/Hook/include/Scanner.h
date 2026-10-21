// Leaked By Cry Service & Siraj
#pragma once
#include <Windows.h>
#include <vector>
#include <string>

class Scanner
{
public:
    static Scanner* Get() {
        static Scanner* m_pSingleton = nullptr;
        if (m_pSingleton == nullptr)
            m_pSingleton = new Scanner;
        return m_pSingleton;
    }

    uint64_t resolverelativeaddress(uint64_t Address, int InstructionLength) {
        if (!Address) return 0;
        DWORD Offset = *(DWORD*)(Address + (InstructionLength - 4));
        return Address + InstructionLength + Offset;
    }

    uint64_t ReturnScan(uint64_t pModuleBaseAddress, const char* sSignature, size_t nSelectResultIndex)
    {
        static auto patternToByte = [](const char* pattern) {
            auto bytes = std::vector<int>{};
            const auto start = const_cast<char*>(pattern);
            const auto end = const_cast<char*>(pattern) + strlen(pattern);

            for (auto current = start; current < end; ++current) {
                if (*current == '?') {
                    ++current;
                    if (*current == '?')
                        ++current;
                    bytes.push_back(-1);
                }
                else
                    bytes.push_back(strtoul((const char*)current, &current, 16));
            }
            return bytes;
        };

        if (!pModuleBaseAddress) return 0;
        const auto dosHeader = (PIMAGE_DOS_HEADER)pModuleBaseAddress;
        if (IsBadReadPtr(dosHeader, sizeof(IMAGE_DOS_HEADER))) return 0;
        const auto ntHeaders = (PIMAGE_NT_HEADERS)((std::uint8_t*)pModuleBaseAddress + dosHeader->e_lfanew);
        if (IsBadReadPtr(ntHeaders, sizeof(IMAGE_NT_HEADERS))) return 0;

        auto patternBytes = patternToByte(sSignature);
        const auto size = patternBytes.size();
        const auto data = patternBytes.data();

        MEMORY_BASIC_INFORMATION mbi;
        uint8_t* currentAddr = (uint8_t*)(pModuleBaseAddress);
        uint8_t* endAddr = currentAddr + ntHeaders->OptionalHeader.SizeOfImage;

        size_t nFoundResults = 0;

        while (currentAddr < endAddr) {
            if (!VirtualQueryEx((HANDLE)-1, currentAddr, &mbi, sizeof(mbi))) {
                break;
            }

            if (mbi.Protect != PAGE_NOACCESS && !(mbi.Protect & PAGE_GUARD)) {
                const auto scanBytes = (uint8_t*)(mbi.BaseAddress);
                const auto regionSize = mbi.RegionSize;

                for (size_t i = 0; i < regionSize - size; ++i) {
                    bool found = true;

                    for (size_t j = 0; j < size; ++j) {
                        if (scanBytes[i + j] != data[j] && data[j] != -1) {
                            found = false;
                            break;
                        }
                    }

                    if (found) {
                        if (nFoundResults < nSelectResultIndex) {
                            nFoundResults++;
                        }
                        else {
                            return (uint64_t)(&currentAddr[i]);
                        }
                    }
                }
            }
            currentAddr = (uint8_t*)(mbi.BaseAddress) + mbi.RegionSize;
        }

        return NULL;
    }

    uint64_t Scan(const char* szModuleName, const char* sSignature, size_t nSelectResultIndex = 0, int InstructionLength = 0) {
        auto ret = ReturnScan((uint64_t)GetModuleHandleA(szModuleName), sSignature, nSelectResultIndex);
        if (InstructionLength != 0 && ret != 0) {
            ret = resolverelativeaddress(ret, InstructionLength);
        }
        return ret;
    }

    uintptr_t resolvePtr(const char* szModuleName, int64_t targetValue, BYTE compareByte) {
        uintptr_t baseaddy = (uintptr_t)GetModuleHandleA(szModuleName);
        if (!baseaddy) return 0;
        const auto dosHeader = (PIMAGE_DOS_HEADER)baseaddy;
        const auto ntHeaders = (PIMAGE_NT_HEADERS)((std::uint8_t*)baseaddy + dosHeader->e_lfanew);
        const auto sizeOfImage = ntHeaders->OptionalHeader.SizeOfImage;

        int64_t currentAddress = baseaddy + 0x13000;

        while (currentAddress < (int64_t)(baseaddy + sizeOfImage)) {
            auto cp = *(int64_t*)currentAddress;
            if (cp == targetValue) {
                if (*(BYTE*)cp == compareByte)
                    return (uintptr_t)currentAddress;
            }
            currentAddress++;
        }

        return NULL;
    }
};
