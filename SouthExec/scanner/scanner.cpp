// Leaked By Cry Service & Siraj
#include "scanner.h"
#include <vector>
#include <string>
#include <cstring>
#include <cctype>
#include <windows.h>  
#include <cstdint>      
#include <sstream>      
#include <iostream>      
#include <fstream>    
#include <algorithm>
#include <random>

bool parsePattern(const std::string& pattern, std::vector<uint8_t>& bytes, std::vector<uint8_t>& mask) {
    std::istringstream stream(pattern);
    std::string token;

    while (stream >> token) {
        if (token == "?") {
            bytes.push_back(0x00); 
            mask.push_back(0x00);  
        }
        else {
            try {
                uint8_t byte = static_cast<uint8_t>(std::stoul(token, nullptr, 16));
                bytes.push_back(byte);
                mask.push_back(0xFF);
            }
            catch (...) {
                return false;
            }
        }
    }

    return !bytes.empty();
}

size_t getModuleSize(HMODULE hModule) {
    if (!hModule) return 0;

    auto dosHeader = reinterpret_cast<PIMAGE_DOS_HEADER>(hModule);
    if (dosHeader->e_magic != IMAGE_DOS_SIGNATURE) return 0;

    auto ntHeaders = reinterpret_cast<PIMAGE_NT_HEADERS>(
        reinterpret_cast<uint8_t*>(hModule) + dosHeader->e_lfanew
        );
    if (ntHeaders->Signature != IMAGE_NT_SIGNATURE) return 0;

    return ntHeaders->OptionalHeader.SizeOfImage;
}

uintptr_t scan(HMODULE hModule, const std::string patternString) {
    std::vector<uint8_t> patternBytes;
    std::vector<uint8_t> patternMask;

    if (!parsePattern(patternString, patternBytes, patternMask)) {
        return 0;
    }

    size_t moduleSize = getModuleSize(hModule);
    if (moduleSize == 0) {
        return 0;
    }

    uint8_t* moduleStart = reinterpret_cast<uint8_t*>(hModule);
    uint8_t* moduleEnd = moduleStart + moduleSize;

    for (uint8_t* current = moduleStart; current < moduleEnd - patternBytes.size(); ++current) {
        bool match = true;

        for (size_t i = 0; i < patternBytes.size(); ++i) {
            if (patternMask[i] && current[i] != patternBytes[i]) {
                match = false;
                break;
            }
        }

        if (match) {
            return reinterpret_cast<uintptr_t>(current);
        }

    }

    return 0;
}

uint64_t ResolveRelativeAddress(uint64_t Address, int InstructionLength)
{
    DWORD Offset = *reinterpret_cast<DWORD*>(Address + (InstructionLength - 4));
    return Address + InstructionLength + Offset;
}

