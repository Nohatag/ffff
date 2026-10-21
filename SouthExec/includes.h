// Leaked By Cry Service & Siraj
#pragma once
#define _SILENCE_CXX17_ITERATOR_BASE_CLASS_DEPRECATION_WARNING
#pragma warning(disable: 4244)
#pragma warning(disable: 4018)
#pragma warning(disable: 4996)
#pragma warning(disable: 4267)
#include "Hooksth.h"
#include <Windows.h>
#include <d3d11.h>
#include <dxgi.h>
#include "Hook/imgui/imgui.h"
#include "Hook/imgui/imgui_internal.h"
#include "Hook/imgui/imgui_impl_win32.h"
#include "Hook/imgui/imgui_impl_dx11.h"

typedef HRESULT(__stdcall* Present) (IDXGISwapChain* pSwapChain, UINT SyncInterval, UINT Flags);
typedef LRESULT(CALLBACK* WNDPROC)(HWND, UINT, WPARAM, LPARAM);
typedef uintptr_t PTR;
