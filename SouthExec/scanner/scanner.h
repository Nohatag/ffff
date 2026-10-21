// Leaked By Cry Service & Siraj
#pragma once
#include <windows.h>
#include <cstdint>
#include <string> 

uintptr_t scan(HMODULE hModule, const std::string patternString);

uint64_t ResolveRelativeAddress(uint64_t Address, int InstructionLength);
