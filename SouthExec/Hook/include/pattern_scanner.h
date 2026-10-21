// Leaked By Cry Service & Siraj
#pragma once
#include <Windows.h>
#include <vector>
#include <string>

class PatternScanner
{
public:
    static uintptr_t FindPattern(const char* moduleName, const char* pattern);
    static uintptr_t FindPattern(uintptr_t moduleHandle, const char* pattern);

private:
    static std::vector<int> PatternToBytes(const char* pattern);
};
