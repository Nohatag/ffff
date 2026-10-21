// Leaked By Cry Service & Siraj
#include "pattern_scanner.h"
#include <Psapi.h>

uintptr_t PatternScanner::FindPattern(const char* moduleName, const char* pattern)
{
    HMODULE hModule = GetModuleHandleA(moduleName);
    if (!hModule) return 0;

    return FindPattern((uintptr_t)hModule, pattern);
}

uintptr_t PatternScanner::FindPattern(uintptr_t moduleHandle, const char* pattern)
{
    MODULEINFO moduleInfo = { 0 };
    if (!GetModuleInformation(GetCurrentProcess(), (HMODULE)moduleHandle, &moduleInfo, sizeof(MODULEINFO)))
        return 0;

    auto patternBytes = PatternToBytes(pattern);
    uint8_t* scanStart = (uint8_t*)moduleHandle;
    uint8_t* scanEnd = scanStart + moduleInfo.SizeOfImage;

    size_t patternSize = patternBytes.size();
    int* patternData = patternBytes.data();

    for (uint8_t* i = scanStart; i < scanEnd - patternSize; ++i)
    {
        bool found = true;
        for (size_t j = 0; j < patternSize; ++j)
        {
            if (patternData[j] != -1 && i[j] != patternData[j])
            {
                found = false;
                break;
            }
        }
        if (found)
            return (uintptr_t)i;
    }

    return 0;
}

std::vector<int> PatternScanner::PatternToBytes(const char* pattern)
{
    std::vector<int> bytes;
    char* start = const_cast<char*>(pattern);
    char* end = const_cast<char*>(pattern) + strlen(pattern);

    for (char* current = start; current < end; ++current)
    {
        if (*current == '?')
        {
            ++current;
            if (*current == '?') ++current;
            bytes.push_back(-1);
        }
        else
        {
            bytes.push_back(strtoul(current, &current, 16));
        }
    }
    return bytes;
}
