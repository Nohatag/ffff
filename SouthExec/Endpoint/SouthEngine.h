// Leaked By Cry Service & Siraj
#pragma once
#include <windows.h>
#include <string>
#include <vector>
#include <functional>
#include <mutex>
#include <future>
#include "Hook/include/Scanner.h"
#include "Hook/include/json.hpp"

namespace South {

    struct ScriptContext {
        void* m_argumentBuffer;
        int m_numArguments;
        int m_numResults;

        template<typename T>
        void Push(const T& value) {
            uintptr_t* functionData = (uintptr_t*)m_argumentBuffer;
            functionData[m_numArguments++] = (uintptr_t)value;
        }

        template<typename T>
        T GetResult() {
            return *(T*)m_argumentBuffer;
        }
    };

    struct ScriptContextBuffer : ScriptContext {
        uintptr_t m_functionData[32];
        ScriptContextBuffer() {
            m_argumentBuffer = m_functionData;
            m_numArguments = 0;
            m_numResults = 0;
            memset(m_functionData, 0, sizeof(m_functionData));
        }
    };

    typedef void(*TNativeHandler)(ScriptContext&);
    typedef void(*CallHandlerType)(TNativeHandler, uint64_t, ScriptContext&);

    class Engine {
    public:
        static Engine* Get() {
            static Engine instance;
            return &instance;
        }

        bool Init() {
            if (m_initialized) return true;

            HMODULE hNet = GetModuleHandleA("gta-net-five.dll");
            if (!hNet) return false;

            auto scanner = Scanner::Get();

            m_originalCallHandler = (CallHandlerType)scanner->Scan("gta-net-five.dll", "48 89 5C 24 ? 48 89 6C 24 ? 48 89 74 24 ? 57 41 56 41 57 48 83 EC 40 4C 8B F2 48 8B E9");

            if (!m_originalCallHandler) {

                m_originalCallHandler = (CallHandlerType)((uintptr_t)hNet + 0x118B0);
            }

            uintptr_t rpcPtr = scanner->Scan("gta-net-five.dll", "48 8B 05 ? ? ? ? 48 8D 0D ? ? ? ? FF 15 ? ? ? ? 48 8B C8");
            if (rpcPtr) {
                int32_t rel = *(int32_t*)(rpcPtr + 3);
                m_rpcConfig = *(void**)(rpcPtr + rel + 7);
            }

            m_initialized = true;
            return true;
        }

        void PerformDump() {
            if (!Init()) return;

        }

    private:
        bool m_initialized = false;
        CallHandlerType m_originalCallHandler = nullptr;
        void* m_rpcConfig = nullptr;
    };
}
