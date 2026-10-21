// Leaked By Cry Service & Siraj
#include "includes.h"
#include "memory.h"
#include "API/api-south/api-south.h"
#include "API/api-exec/api-exec.h"

// â”€â”€ Linker Export Forwarding for d3d10.dll proxy â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€
#pragma comment(linker, "/export:D3D10CreateDevice=C:\\Windows\\System32\\d3d10.D3D10CreateDevice")
#pragma comment(linker, "/export:D3D10CreateDeviceAndSwapChain=C:\\Windows\\System32\\d3d10.D3D10CreateDeviceAndSwapChain")
#pragma comment(linker, "/export:D3D10CreateBlob=C:\\Windows\\System32\\d3d10.D3D10CreateBlob")
#pragma comment(linker, "/export:D3D10CreateStateBlock=C:\\Windows\\System32\\d3d10.D3D10CreateStateBlock")
#pragma comment(linker, "/export:D3D10CompileEffectFromMemory=C:\\Windows\\System32\\d3d10.D3D10CompileEffectFromMemory")
#pragma comment(linker, "/export:D3D10CompileShader=C:\\Windows\\System32\\d3d10.D3D10CompileShader")
#pragma comment(linker, "/export:D3D10DisassembleEffect=C:\\Windows\\System32\\d3d10.D3D10DisassembleEffect")
#pragma comment(linker, "/export:D3D10DisassembleShader=C:\\Windows\\System32\\d3d10.D3D10DisassembleShader")
#pragma comment(linker, "/export:D3D10GetGeometryShaderProfile=C:\\Windows\\System32\\d3d10.D3D10GetGeometryShaderProfile")
#pragma comment(linker, "/export:D3D10GetInputSignatureBlob=C:\\Windows\\System32\\d3d10.D3D10GetInputSignatureBlob")
#pragma comment(linker, "/export:D3D10GetOutputSignatureBlob=C:\\Windows\\System32\\d3d10.D3D10GetOutputSignatureBlob")
#pragma comment(linker, "/export:D3D10GetPixelShaderProfile=C:\\Windows\\System32\\d3d10.D3D10GetPixelShaderProfile")
#pragma comment(linker, "/export:D3D10GetShaderDebugInfo=C:\\Windows\\System32\\d3d10.D3D10GetShaderDebugInfo")
#pragma comment(linker, "/export:D3D10GetVertexShaderProfile=C:\\Windows\\System32\\d3d10.D3D10GetVertexShaderProfile")
#pragma comment(linker, "/export:D3D10PreprocessShader=C:\\Windows\\System32\\d3d10.D3D10PreprocessShader")
#pragma comment(linker, "/export:D3D10ReflectShader=C:\\Windows\\System32\\d3d10.D3D10ReflectShader")
#pragma comment(linker, "/export:D3D10StateBlockMaskAlignTo32=C:\\Windows\\System32\\d3d10.D3D10StateBlockMaskAlignTo32")
#pragma comment(linker, "/export:D3D10StateBlockMaskClearAll=C:\\Windows\\System32\\d3d10.D3D10StateBlockMaskClearAll")
#pragma comment(linker, "/export:D3D10StateBlockMaskClearDeviceState=C:\\Windows\\System32\\d3d10.D3D10StateBlockMaskClearDeviceState")
#pragma comment(linker, "/export:D3D10StateBlockMaskDisableAll=C:\\Windows\\System32\\d3d10.D3D10StateBlockMaskDisableAll")
#pragma comment(linker, "/export:D3D10StateBlockMaskEnableAll=C:\\Windows\\System32\\d3d10.D3D10StateBlockMaskEnableAll")
#pragma comment(linker, "/export:D3D10StateBlockMaskGetSetting=C:\\Windows\\System32\\d3d10.D3D10StateBlockMaskGetSetting")
#pragma comment(linker, "/export:D3D10StateBlockMaskIntersect=C:\\Windows\\System32\\d3d10.D3D10StateBlockMaskIntersect")
#pragma comment(linker, "/export:D3D10StateBlockMaskMaskToSource=C:\\Windows\\System32\\d3d10.D3D10StateBlockMaskMaskToSource")
#pragma comment(linker, "/export:D3D10StateBlockMaskSetSetting=C:\\Windows\\System32\\d3d10.D3D10StateBlockMaskSetSetting")
#pragma comment(linker, "/export:D3D10StateBlockMaskUnion=C:\\Windows\\System32\\d3d10.D3D10StateBlockMaskUnion")

#include "Endpoint/TextEditor.h"
#include "UIX -EM/click_sound.h"
#include "UIX -EM/font_awesome_bytes.h"
#define MINIAUDIO_IMPLEMENTATION
#include "Hook/include/miniaudio.h"
// #include "AntiDebug.h" // DISABLED: Anti-debugging only in loader.exe
#include <map>
#include <string>
#include <vector>
#include <mutex>
#include <cmath>
#include <unordered_set>
#include <vector>
#include <string>
#include <wininet.h>
#pragma comment(lib, "wininet.lib")
#include <fstream>
#include <urlmon.h>
#pragma comment(lib, "urlmon.lib")
#include <commdlg.h>
#pragma comment(lib, "comdlg32.lib")

#include "Hook/kiero/minhook/include/MinHook.h"
#include <Psapi.h>

// Forward declaration of AddLog to prevent compile errors
void AddLog(std::string msg, unsigned int color);
const char* GetKeyName(int vk);
void DrawSouthKeybindSetupPopup();
void DrawVehicleSpawnerInputPopup();

static HMODULE g_hMod = nullptr;

static bool showMenu = true;

static ImVec2 g_AdminListPos = ImVec2(59.0f, 55.0f);
static bool g_AdminListDragging = false;
static ImVec2 g_AdminListDragOffset = ImVec2(0.0f, 0.0f);

static ImVec2 g_KeybindOverlayPos = ImVec2(-1.0f, -1.0f);
static bool g_KeybindOverlayDragging = false;
static ImVec2 g_KeybindOverlayDragOffset = ImVec2(0.0f, 0.0f);

// Global variables for Server dynamic player count
static int g_ServerPlayerCount = 0;
static std::string g_CurrentServerIP = "Loading...";
static std::string g_ServerJoinCode = "Loading...";
static std::string g_ServerHostname = "Loading...";
static bool g_optRandom = false;
static bool g_optIsolated = false;
bool g_SafeToExecute = false;


// FontAwesome Icons Definitions
#define ICON_FA_USER "\xef\x80\x87"
#define ICON_FA_USERS "\xef\x83\x80"
#define ICON_FA_GUN "\xee\x86\x9b" // Original pistol icon (U+E19B)
#define ICON_FA_CAR "\xef\x86\xb9"
#define ICON_FA_BOMB "\xef\x87\xa2"
#define ICON_FA_GEAR "\xef\x80\x93"
#define ICON_FA_SHIELD_HALVED "\xef\x8f\xad"
#define ICON_FA_CROSSHAIRS "\xef\x81\x9b"
#define ICON_FA_WRENCH "\xef\x82\xad"
#define ICON_FA_BOLT "\xef\x83\xa7"
#define ICON_FA_BULLSEYE "\xef\x85\x80"
#define ICON_FA_FILE "\xef\x85\x9b" // Trigger file icon (U+F15B)
#define ICON_FA_KEYBOARD "\xef\x84\x9c"

static std::string GetDllFolderPath() {
    char path[MAX_PATH];
    if (g_hMod) {
        GetModuleFileNameA(g_hMod, path, MAX_PATH);
        std::string sPath = path;
        size_t lastSlash = sPath.find_last_of("\\/");
        if (lastSlash != std::string::npos) {
            return sPath.substr(0, lastSlash + 1);
        }
    }
    return "";
}

static std::string GetServerInfoFilePath() {
    char tempPath[MAX_PATH];
    DWORD pathLen = GetTempPathA(MAX_PATH, tempPath);
    if (pathLen > 0 && pathLen < MAX_PATH) {
        return std::string(tempPath) + "current_server.txt";
    }
    return "C:\\South\\current_server.txt";
}


static bool FileExists(const std::string& name) {
    DWORD attrib = GetFileAttributesA(name.c_str());
    return (attrib != INVALID_FILE_ATTRIBUTES && !(attrib & FILE_ATTRIBUTE_DIRECTORY));
}

struct SouthVec3 { float x, y, z; };
#include <chrono>
#include <map>
#include <fstream>
#include <iostream>
#include <memory>
#include <atomic>
#include <thread>
#include <regex>
#include <ctime>
#include <Windows.h>
#include <deque>
#include <iomanip>
#include <sstream>
#include <functional>
#include <algorithm>
#include <wincodec.h>
#pragma comment(lib, "windowscodecs.lib")
#include <mmsystem.h>
#pragma comment(lib, "winmm.lib")

// CFX Headers
#include "Hook/include/xorstr/xorstr.hpp"
#include "Hook/cfx/resource.h"
#include "Hook/cfx/resource_manager.h"

#ifdef FindResource
#undef FindResource
#endif

#include <filesystem>
#include "Lua.h"
#include "Endpoint/SouthEngine.h"
#include "UIX -EM/v_icons.h"
#include "UIX -EM/south_logo_new.h"
#include "UIX -EM/banner_new.h"

#include "Hook/include/Address.h"
#include "Hook/include/Core.h"
#include "Hook/include/features/ESP.h"
#include "Hook/include/features/Aimbot.h"
#include "Hook/include/features/Weapon.hpp"
#include "Hook/include/features/NameManager.h"

using namespace fx;
extern LRESULT ImGui_ImplWin32_WndProcHandler(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

struct LogEntry { std::string text; ImU32 color; };
static std::deque<LogEntry> g_Logs;
static std::deque<LogEntry> g_ExecutorLogs;

// Clear logs on startup to hide logger content
static bool g_LogsCleared = false;
static std::string g_CurrentRandomName = "South Executor";
static ImU32 g_CurrentRandomColor = IM_COL32(185, 185, 185, 255);
static std::map<std::string, fx::ResourceImpl*> g_ResourceMap;
static fx::ResourceImpl* g_PersistentHome = nullptr;
static std::vector<fwRefContainer<fx::Resource>> cachedResources;

static ma_engine g_AudioEngine;
static ma_sound  g_ClickSound;
static ma_decoder g_ClickDecoder;
static bool      g_AudioInitialized = false;
static bool      g_DisableSound     = false;
static bool      g_DisableKeybindOverlay = false;
static std::map<std::string, int> g_ItemKeybinds; // item label -> VK code

void PlayClickSound() {
    if (g_DisableSound) return;
    if (!g_AudioInitialized) {
        if (ma_engine_init(NULL, &g_AudioEngine) == MA_SUCCESS) {
            if (ma_decoder_init_memory(click_sound_data, sizeof(click_sound_data), NULL, &g_ClickDecoder) == MA_SUCCESS) {
                if (ma_sound_init_from_data_source(&g_AudioEngine, &g_ClickDecoder, 0, NULL, &g_ClickSound) == MA_SUCCESS) {
                    ma_sound_set_volume(&g_ClickSound, 0.5f);
                    g_AudioInitialized = true;
                }
            }
        }
    }

    if (g_AudioInitialized) {
        ma_sound_seek_to_pcm_frame(&g_ClickSound, 0);
        ma_sound_start(&g_ClickSound);
    }
}

struct Notification {
    std::string title;
    std::string message;
    float time;
    float duration;
};
static std::vector<Notification> g_Notifications;
static CRITICAL_SECTION g_NotifCs;
static bool g_NotifCsInitialized = false;

void PushNotification(std::string title, std::string msg, float duration = 3.0f) {
    if (!g_NotifCsInitialized) {
        InitializeCriticalSection(&g_NotifCs);
        g_NotifCsInitialized = true;
    }
    EnterCriticalSection(&g_NotifCs);
    if (title == "Success" && (msg.rfind("Error:", 0) == 0 || msg.find("Error:") != std::string::npos)) {
        title = "Error";
    }
    g_Notifications.push_back({ title, msg, (float)ImGui::GetTime(), duration });
    LeaveCriticalSection(&g_NotifCs);
}

void PushNotification(std::string msg, float duration = 3.0f) {
    if (msg.rfind("Error:", 0) == 0 || msg.find("Error:") != std::string::npos) {
        PushNotification("Error", msg, duration);
    } else {
        PushNotification("Success", msg, duration);
    }
}

// --- South Dumper Infrastructure (Fixed-size buffer to prevent crashes) ---
struct DumpLog { char msg[256]; ImU32 col; };
static DumpLog g_DumpLogs[100];
static int g_DumpLogCount = 0;
static int g_DumpLogHead = 0;
static bool g_IsDumping = false;

void AddDumpLog(const char* msg, ImU32 col = IM_COL32(255, 255, 255, 255)) {
    if (!msg || msg[0] == '\0') return;
    
    int idx = g_DumpLogHead % 100;
    strncpy_s(g_DumpLogs[idx].msg, msg, 255);
    g_DumpLogs[idx].col = col;
    
    g_DumpLogHead++;
    if (g_DumpLogCount < 100) g_DumpLogCount++;
}

void InternalPerformDump(); // Forward declaration
std::string GetRandomName(); // Forward declaration
std::string CleanEndpoint(const std::string& endpoint); // Forward declaration

// Forward declarations for Keybinds Overlay state tracking
#ifndef SOUTH_STRUCTS_DEFINED
#define SOUTH_STRUCTS_DEFINED
struct SouthItem { 
    std::string label; std::string type; float val = 0; bool checked = false; std::string subVal = ""; 
};
struct SouthCategory { std::string label; std::vector<SouthItem> items; };
struct SouthSubMenu { std::string label; std::vector<SouthCategory> categories; };
#endif
extern std::vector<SouthSubMenu> g_MenuData;
extern bool g_FreecamActive;
extern bool g_NoclipEnabled;

void DrawKeybindsOverlay() {
    if (g_DisableKeybindOverlay) return;
    
    // Check if we have any active keybinds
    bool hasActiveKeybinds = false;
    for (const auto& [label, vk] : g_ItemKeybinds) {
        if (vk != 0) {
            hasActiveKeybinds = true;
            break;
        }
    }
    if (!hasActiveKeybinds) return;

    ImGuiIO& io = ImGui::GetIO();
    ImDrawList* draw = ImGui::GetForegroundDrawList();
    ImVec2 screenSize = io.DisplaySize;

    float width = 230.0f; 
    
    // Count active keybinds to calculate exact height
    int activeKeybindCount = 0;
    for (const auto& [label, vk] : g_ItemKeybinds) {
        if (vk != 0) activeKeybindCount++;
    }
    float totalHeight = 25.0f + (activeKeybindCount * 22.0f) + 4.0f;
    
    // Initialize default position if not set
    if (g_KeybindOverlayPos.x < 0.0f) {
        g_KeybindOverlayPos.x = screenSize.x - width - 20.0f;
        g_KeybindOverlayPos.y = 40.0f;
    }

    // Dragging logic
    if (showMenu) {
        ImVec2 mousePos = io.MousePos;
        bool isMouseDown = io.MouseDown[0];
        
        // Header bounding box
        float headerMinX = g_KeybindOverlayPos.x;
        float headerMaxX = g_KeybindOverlayPos.x + width;
        float headerMinY = g_KeybindOverlayPos.y;
        float headerMaxY = g_KeybindOverlayPos.y + 25.0f;
        
        if (isMouseDown) {
            if (!g_KeybindOverlayDragging) {
                if (mousePos.x >= headerMinX && mousePos.x <= headerMaxX &&
                    mousePos.y >= headerMinY && mousePos.y <= headerMaxY) {
                    g_KeybindOverlayDragging = true;
                    g_KeybindOverlayDragOffset.x = mousePos.x - g_KeybindOverlayPos.x;
                    g_KeybindOverlayDragOffset.y = mousePos.y - g_KeybindOverlayPos.y;
                }
            } else {
                g_KeybindOverlayPos.x = mousePos.x - g_KeybindOverlayDragOffset.x;
                g_KeybindOverlayPos.y = mousePos.y - g_KeybindOverlayDragOffset.y;
            }
        } else {
            g_KeybindOverlayDragging = false;
        }
    } else {
        g_KeybindOverlayDragging = false;
    }

    // Always keep inside screen (handles resolution changes)
    if (g_KeybindOverlayPos.x < 0.0f) g_KeybindOverlayPos.x = 0.0f;
    if (g_KeybindOverlayPos.x + width > screenSize.x) g_KeybindOverlayPos.x = screenSize.x - width;
    if (g_KeybindOverlayPos.y < 0.0f) g_KeybindOverlayPos.y = 0.0f;
    if (g_KeybindOverlayPos.y + totalHeight > screenSize.y) g_KeybindOverlayPos.y = screenSize.y - totalHeight;

    float startX = g_KeybindOverlayPos.x;
    float startY = g_KeybindOverlayPos.y;

    // Header title
    ImGui::PushFont(io.Fonts->Fonts[0]);
    draw->AddRectFilled(ImVec2(startX, startY), ImVec2(startX + width, startY + 25.0f), IM_COL32(18, 18, 22, 220), 4.0f, ImDrawCornerFlags_Top);
    draw->AddRectFilledMultiColor(ImVec2(startX, startY + 23.0f), ImVec2(startX + width, startY + 25.0f), IM_COL32(180, 20, 25, 255), IM_COL32(180, 20, 25, 0), IM_COL32(180, 20, 25, 0), IM_COL32(180, 20, 25, 255));
    
    // Icon & Title Text (Use keyboard icon)
    draw->AddText(ImGui::GetIO().Fonts->Fonts[2], 13.0f, ImVec2(startX + 8.0f, startY + 6.0f), IM_COL32(180, 20, 25, 255), "\xef\x84\x9c");
    draw->AddText(ImVec2(startX + 26.0f, startY + 4.0f), IM_COL32(240, 240, 245, 255), "Keybinds");
    
    float currentY = startY + 25.0f;

    // Active keybind list
    for (const auto& [label, vk] : g_ItemKeybinds) {
        if (vk == 0) continue;

        std::string keyName = GetKeyName(vk);

        // Dynamic State Lookup
        bool isFeatureOn = false;
        bool isToggle = false;
        
        // Specially tracked states (these are toggles)
        if (label == "Freecam") {
            isFeatureOn = g_FreecamActive;
            isToggle = true;
        } else if (label == "Noclip") {
            isFeatureOn = g_NoclipEnabled;
            isToggle = true;
        } else {
            // General lookup through the menu structure
            for (const auto& sub : g_MenuData) {
                bool found = false;
                for (const auto& cat : sub.categories) {
                    for (const auto& item : cat.items) {
                        if (item.label == label) {
                            isFeatureOn = item.checked;
                            if (item.type == "check" || item.type == "scroll-check" || item.type == "slider-check" || item.type == "check-slider") {
                                isToggle = true;
                            }
                            found = true;
                            break;
                        }
                    }
                    if (found) break;
                }
                if (found) break;
            }
        }

        draw->AddRectFilled(ImVec2(startX, currentY), ImVec2(startX + width, currentY + 22.0f), IM_COL32(14, 14, 16, 200));
        
        // Draw feature label (slightly muted white)
        draw->AddText(ImVec2(startX + 10.0f, currentY + 3.0f), IM_COL32(180, 180, 185, 255), label.c_str());
        
        // Draw key name (South signature red)
        ImVec2 keySize = ImGui::CalcTextSize(keyName.c_str());
        
        if (isToggle) {
            // Position key name in the middle-right area to allow space for status
            float keyX = startX + width - keySize.x - 52.0f; 
            draw->AddText(ImVec2(keyX, currentY + 3.0f), IM_COL32(180, 20, 25, 255), keyName.c_str());

            // Draw Status Indicator (Solid White [ON] or [OFF])
            std::string statusText = isFeatureOn ? "ON" : "OFF";
            ImU32 statusColor = IM_COL32(255, 255, 255, 255);
            ImVec2 statusSize = ImGui::CalcTextSize(statusText.c_str());
            draw->AddText(ImVec2(startX + width - statusSize.x - 10.0f, currentY + 3.0f), statusColor, statusText.c_str());
        } else {
            // Position key name at the far right since there's no ON/OFF status (it's a button)
            float keyX = startX + width - keySize.x - 10.0f;
            draw->AddText(ImVec2(keyX, currentY + 3.0f), IM_COL32(180, 20, 25, 255), keyName.c_str());
        }

        currentY += 22.0f;
    }

    // Draw bottom border rounding
    draw->AddRectFilled(ImVec2(startX, currentY), ImVec2(startX + width, currentY + 4.0f), IM_COL32(14, 14, 16, 200), 4.0f, ImDrawCornerFlags_Bot);
    draw->AddRect(ImVec2(startX, startY), ImVec2(startX + width, currentY + 4.0f), IM_COL32(255, 255, 255, 20), 4.0f);
    ImGui::PopFont();
}

void DrawNotifications() {
    ImGuiIO& io = ImGui::GetIO();
    float time = (float)ImGui::GetTime();
    ImDrawList* draw = ImGui::GetForegroundDrawList();
    ImVec2 screenSize = io.DisplaySize;

    float yOffset = 40.0f; // Start at 40px from the top
    float xTarget = screenSize.x - 20.0f; // 20px padding from the right

    std::vector<Notification> tempNotifs;
    {
        if (!g_NotifCsInitialized) {
            InitializeCriticalSection(&g_NotifCs);
            g_NotifCsInitialized = true;
        }
        EnterCriticalSection(&g_NotifCs);
        for (int i = 0; i < (int)g_Notifications.size(); i++) {
            auto& notif = g_Notifications[i];
            float elapsed = time - notif.time;

            if (elapsed > notif.duration) {
                g_Notifications.erase(g_Notifications.begin() + i);
                i--;
                continue;
            }
        }
        tempNotifs = g_Notifications;
        LeaveCriticalSection(&g_NotifCs);
    }

    for (int i = (int)tempNotifs.size() - 1; i >= 0; i--) {
        auto& notif = tempNotifs[i];
        float elapsed = time - notif.time;

        float alpha = 1.0f;
        float fadeIn = 0.4f;
        float fadeOut = 0.4f;

        if (elapsed < fadeIn) alpha = elapsed / fadeIn;
        else if (elapsed > notif.duration - fadeOut) alpha = (notif.duration - elapsed) / fadeOut;

        float slideAnim = 1.0f;
        if (elapsed < fadeIn) slideAnim = elapsed / fadeIn;
        else if (elapsed > notif.duration - fadeOut) slideAnim = (notif.duration - elapsed) / fadeOut;
        
        // Cubic easing for smoother slide
        slideAnim = 1.0f - powf(1.0f - slideAnim, 3.0f);

        ImVec2 titleSize = ImGui::CalcTextSize(notif.title.c_str());
        ImVec2 msgSize = ImGui::CalcTextSize(notif.message.c_str());
        float maxTextWidth = (std::max)(titleSize.x, msgSize.x);
        
        float cardWidth = maxTextWidth + 82.0f; 
        float cardHeight = 68.0f;

        float currentX = xTarget - (cardWidth * slideAnim);
        ImVec2 pMin = ImVec2(currentX, yOffset);
        ImVec2 pMax = ImVec2(currentX + cardWidth, yOffset + cardHeight);

        // Shadow (Sleek dark shadow)
        draw->AddRectFilled(ImVec2(pMin.x + 3, pMin.y + 3), ImVec2(pMax.x + 3, pMax.y + 3), IM_COL32(0, 0, 0, (int)(90 * alpha)), 14.0f);
        
        // Background (Sleek pure black card, smooth rounded corners, no border)
        draw->AddRectFilled(pMin, pMax, IM_COL32(8, 8, 8, (int)(250 * alpha)), 14.0f);

        ImVec2 cCenter = ImVec2(pMin.x + 30.0f, pMin.y + 34.0f);
        bool isError = (notif.title == "Error" || notif.message.rfind("Error:", 0) == 0 || notif.message.find("Error:") != std::string::npos);

        if (isError) {
            // Vibrant Red Circle with bold 'X' mark
            draw->AddCircleFilled(cCenter, 13.0f, IM_COL32(230, 40, 40, (int)(255 * alpha)));
            // Left to right line of 'X'
            draw->AddLine(ImVec2(cCenter.x - 4.5f, cCenter.y - 4.5f), ImVec2(cCenter.x + 4.5f, cCenter.y + 4.5f), IM_COL32(255, 255, 255, (int)(255 * alpha)), 2.5f);
            // Right to left line of 'X'
            draw->AddLine(ImVec2(cCenter.x + 4.5f, cCenter.y - 4.5f), ImVec2(cCenter.x - 4.5f, cCenter.y + 4.5f), IM_COL32(255, 255, 255, (int)(255 * alpha)), 2.5f);
        } else {
            // Vibrant Green Circle Icon with Checkmark (Slightly larger & bolder)
            draw->AddCircleFilled(cCenter, 13.0f, IM_COL32(0, 220, 80, (int)(255 * alpha)));
            draw->AddLine(ImVec2(cCenter.x - 5.5f, cCenter.y + 0.5f), ImVec2(cCenter.x - 1.0f, cCenter.y + 5.0f), IM_COL32(255, 255, 255, (int)(255 * alpha)), 2.5f);
            draw->AddLine(ImVec2(cCenter.x - 1.0f, cCenter.y + 5.0f), ImVec2(cCenter.x + 6.0f, cCenter.y - 3.5f), IM_COL32(255, 255, 255, (int)(255 * alpha)), 2.5f);
        }

        // Title text "Success" or "Error" (Bold White/Bright Grey)
        draw->AddText(ImGui::GetFont(), ImGui::GetFontSize() * 1.12f, ImVec2(pMin.x + 58.0f, pMin.y + 15.0f), IM_COL32(255, 255, 255, (int)(255 * alpha)), isError ? "Error" : notif.title.c_str());

        // Message description (Clean white/slightly muted grey)
        draw->AddText(ImGui::GetFont(), ImGui::GetFontSize() * 0.95f, ImVec2(pMin.x + 58.0f, pMin.y + 37.0f), IM_COL32(210, 210, 210, (int)(255 * alpha)), notif.message.c_str());

        yOffset += cardHeight + 10.0f; // Stack downwards
    }
}

void AddLog(std::string msg, ImU32 color = IM_COL32(230, 230, 235, 255)) {
    AddDumpLog(msg.c_str(), color);
    g_Logs.push_back({ msg, color });
    if (g_Logs.size() > 300) {
        g_Logs.pop_front();
    }
}

void AddExecutorLog(std::string msg, ImU32 color = IM_COL32(230, 230, 235, 255)) {
    g_ExecutorLogs.push_back({ msg, color });
    if (g_ExecutorLogs.size() > 100) {
        g_ExecutorLogs.pop_front();
    }
}

std::string GetRandomString(int len) {
    static const char alph[] = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789";
    std::string s = "";
    for (int i = 0; i < len; ++i) s += alph[rand() % (sizeof(alph) - 1)];
    return s;
}

struct MachoInjectCall {
    std::string resourceName;
    std::string code;
};

fx::ResourceImpl* MachoFindResource(const std::string& name, std::vector<fwRefContainer<fx::Resource>>& resources) {
    if (g_ResourceMap.count(name)) return g_ResourceMap[name];
    for (auto& r : resources) {
        auto resPtr = r.GetRef();
        if (resPtr && (name == "any" || resPtr->GetName() == name)) return (fx::ResourceImpl*)resPtr;
    }
    return nullptr;
}

void MachoResourceControl(const std::string& name, bool start, std::vector<fwRefContainer<fx::Resource>>& resources) {
    auto res = MachoFindResource(name, resources);
    if (res) {
        if (start) res->Start(); else res->Stop();
        AddLog("MachoResource" + std::string(start ? "Start" : "Stop") + " -> [" + name + "]", IM_COL32(180, 180, 255, 255));
    }
}

void MachoPushNotification(const std::string& title, const std::string& desc) {
    PushNotification(title, desc);
}

void AddExecutorLog(std::string msg, ImU32 color);

extern "C" __declspec(dllexport) void __cdecl MachoDuiLogFFI(const char* msg) {
    if (msg) {
        AddExecutorLog(msg, IM_COL32(230, 230, 235, 255));
    }
}

bool MachoInjectResource(const std::string& resourceName, const std::string& code, std::vector<fwRefContainer<fx::Resource>>& resources);

extern "C" __declspec(dllexport) void __cdecl MachoStopResourceFFI(const char* name) {
    if (name) {
        AddExecutorLog("MachoStopResourceFFI request (blocked native call for safety): " + std::string(name), IM_COL32(255, 100, 100, 255));
    }
}

extern "C" __declspec(dllexport) void __cdecl MachoStartResourceFFI(const char* name) {
    if (name) {
        AddExecutorLog("MachoStartResourceFFI request (blocked native call for safety): " + std::string(name), IM_COL32(255, 100, 100, 255));
    }
}

extern "C" __declspec(dllexport) void __cdecl MachoInjectResourceFFI(const char* name, const char* code) {
    if (name && code) {
        MachoInjectResource(name, code, cachedResources);
    }
}


std::string GetOriginalResourceName(fx::ResourceImpl* resPtr) {
    for (auto const& [name, ptr] : g_ResourceMap) {
        if (ptr == resPtr) return name;
    }
    return resPtr->GetName();
}

std::string GetMachoBootstrap() {
    return "";
}

extern bool g_NoclipEnabled;
extern float g_NoclipSpeed;
extern int g_NoclipMode;

extern bool SafeValidateRuntimePointer(_LuaClasses::LuaScriptRuntime* rt);
extern void RefreshServerResources(bool allowLua) noexcept;

bool MachoInjectResource(const std::string& resourceName, const std::string& code, std::vector<fwRefContainer<fx::Resource>>& resources) {
    RefreshServerResources(false);
    std::string finalCode = code;
    
    std::string targetResourceName = resourceName;
    if (targetResourceName == "any") {
        std::vector<std::string> activeNames;
        for (const auto& entry : _LuaGlobals::_ServerResources) {
            if (entry.runtime && SafeValidateRuntimePointer(entry.runtime)) {
                activeNames.push_back(entry.name);
            }
        }
        if (!activeNames.empty()) {
            static std::mt19937 rng{ std::random_device{}() };
            std::uniform_int_distribution<size_t> dist(0, activeNames.size() - 1);
            targetResourceName = activeNames[dist(rng)];
        }
    }

    _LuaClasses::LuaScriptRuntime* targetRt = nullptr;
    bool exactFound = false;
    
    // Fallback search: first try case-insensitive matching
    for (const auto& entry : _LuaGlobals::_ServerResources) {
        std::string entryLower = entry.name;
        std::string targetLower = targetResourceName;
        std::transform(entryLower.begin(), entryLower.end(), entryLower.begin(), [](unsigned char c) { return std::tolower(c); });
        std::transform(targetLower.begin(), targetLower.end(), targetLower.begin(), [](unsigned char c) { return std::tolower(c); });
        if (entryLower == targetLower && entry.runtime && SafeValidateRuntimePointer(entry.runtime)) {
            targetRt = entry.runtime;
            exactFound = true;
            break;
        }
    }

    if (!exactFound && resourceName != "any") {
        // Fallback to the first active runtime if target resource is not found, ensuring script injection still works
        targetRt = GetFirstActiveRuntime();
        if (targetRt) {
            exactFound = true;
        }
    }

    if (!targetRt) {
        if (resourceName == "any") {
            targetRt = GetFirstActiveRuntime();
        } else {
            return false;
        }
    }

    if (targetRt) {
        return ExecuteScriptInRuntime(targetRt, finalCode, targetResourceName);
    }

    return false;
}

std::string extractOneArg(const std::string& script, const std::string& cmd, int argIdx = 1) {
    size_t p = script.find(cmd);
    if (p == std::string::npos) return "";
    int currentArg = 0;
    for (size_t i = script.find("(", p); i < script.size() && script[i] != ')'; i++) {
        if (script[i] == '\'' || script[i] == '"') {
            char q = script[i]; size_t s = i + 1;
            i = script.find(q, s);
            if (i == std::string::npos) break;
            currentArg++;
            if (currentArg == argIdx) return script.substr(s, i - s);
        }
    }
    return "";
}

std::vector<MachoInjectCall> ParseMachoInjectCalls(const std::string& script, const std::string& cmdName = "MachoInjectResource") {
    std::vector<MachoInjectCall> calls;
    size_t pos = 0;
    while ((pos = script.find(cmdName, pos)) != std::string::npos) {
        std::string res = extractOneArg(script.substr(pos), cmdName, 1);
        size_t cS = script.find("[[", pos);
        size_t cE = script.find("]]", cS);
        if (cS != std::string::npos && cE != std::string::npos) {
            calls.push_back({ res, script.substr(cS + 2, cE - cS - 2) });
        }
        pos += cmdName.length() + 5;
    }
    return calls;
}

struct MachoHookCall {
    std::string hash;
    std::string callbackCode;
};

std::vector<MachoHookCall> ParseMachoHookCalls(const std::string& script) {
    std::vector<MachoHookCall> calls;
    std::string cmdName = "MachoHookNative";
    size_t pos = 0;
    while ((pos = script.find(cmdName, pos)) != std::string::npos) {
        size_t openParen = script.find("(", pos);
        if (openParen == std::string::npos) { pos += cmdName.length(); continue; }
        
        size_t comma = script.find(",", openParen);
        if (comma == std::string::npos) { pos += cmdName.length(); continue; }
        
        std::string hash = script.substr(openParen + 1, comma - openParen - 1);
        hash.erase(0, hash.find_first_not_of(" \t\r\n"));
        hash.erase(hash.find_last_not_of(" \t\r\n") + 1);
        
        size_t funcStart = script.find("function", comma);
        if (funcStart == std::string::npos) { pos += cmdName.length(); continue; }
        
        int scope = 0;
        size_t scan = funcStart;
        size_t matchEnd = std::string::npos;
        
        while (scan < script.length()) {
            if (script.compare(scan, 8, "function") == 0) {
                scope++; scan += 8;
            } else if (script.compare(scan, 3, "end") == 0) {
                scope--;
                if (scope == 0) {
                    matchEnd = scan + 3; 
                    break;
                }
                scan += 3;
            } else {
                scan++;
            }
        }
        
        if (matchEnd != std::string::npos) {
            std::string cbCode = script.substr(funcStart, matchEnd - funcStart);
            calls.push_back({ hash, cbCode });
            pos = matchEnd;
        } else {
            pos += cmdName.length();
        }
    }
    return calls;
}

#include "natives_map.h"
#include <algorithm>

static std::string GetNativeNameFromHash(const std::string& hashStr) {
    std::string clean = hashStr;
    clean.erase(std::remove(clean.begin(), clean.end(), ' '), clean.end());
    clean.erase(std::remove(clean.begin(), clean.end(), '\t'), clean.end());
    clean.erase(std::remove(clean.begin(), clean.end(), '\r'), clean.end());
    clean.erase(std::remove(clean.begin(), clean.end(), '\n'), clean.end());
    
    uint64_t hashVal = 0;
    try {
        if (clean.rfind("0x", 0) == 0 || clean.rfind("0X", 0) == 0) {
            hashVal = std::stoull(clean, nullptr, 16);
        } else {
            hashVal = std::stoull(clean, nullptr, 10);
        }
    } catch (...) {
        return "";
    }

    const auto& map = GetNativesMap();
    auto it = map.find(hashVal);
    if (it != map.end()) {
        std::string rawName = it->second;
        std::string camelCase = "";
        bool nextUpper = true;
        for (char c : rawName) {
            if (c == '_') {
                nextUpper = true;
            } else {
                if (nextUpper) {
                    camelCase += std::toupper(c);
                    nextUpper = false;
                } else {
                    camelCase += std::tolower(c);
                }
            }
        }
        return camelCase;
    }

    return "";
}

bool MachoHookNative(const std::string& hash, const std::string& callbackCode, std::vector<fwRefContainer<fx::Resource>>& resources) {
    std::string nativeName = GetNativeNameFromHash(hash);
    std::string nameOverrideCode = "";
    if (!nativeName.empty()) {
        nameOverrideCode = R"lua(
        local nativeName = [=[)lua" + nativeName + R"lua(]=]
        local function overrideGlobal(name)
            local origFunc = _G[name] or (_ENV and _ENV[name])
            if origFunc and type(origFunc) == "function" then
                local hookWrapper = function(...)
                    if _G.__machoHookActive then return origFunc(...) end
                    _G.__machoHookActive = true
                    
                    local hooks = _G.__machoHooks[callKey] or _G.__machoHooks[targetNum]
                    if not hooks and targetNum then
                        local ok, decStr = pcall(string.format, "%0.0f", targetNum)
                        if ok then hooks = _G.__machoHooks[decStr] end
                    end
                    
                    if hooks then
                        for j = 1, #hooks do
                            local res = { hooks[j](...) }
                            if res[1] == false then
                                _G.__machoHookActive = false
                                return _unpack(res, 2)
                            end
                        end
                    end
                    local results = { pcall(origFunc, ...) }
                    _G.__machoHookActive = false
                    if results[1] then
                        return _unpack(results, 2)
                    else
                        error(results[2])
                    end
                end
                _G[name] = hookWrapper
                if _ENV then _ENV[name] = hookWrapper end
            end
        end
        overrideGlobal(nativeName)
        overrideGlobal(nativeName:upper())
        )lua";
    }

    std::string payload = R"lua(
        -- CreateThread bypass to run synchronously
        local _unpack = table.unpack or unpack
        if not _G.__machoHooks then
            _G.__machoHooks = {}
            _G.__machoHookActive = false
            
            local _origInvoke = Citizen.InvokeNative
            Citizen.InvokeNative = function(h, ...)
                if _G.__machoHookActive then return _origInvoke(h, ...) end
                
                local hooks = _G.__machoHooks[h] or _G.__machoHooks[tostring(h)]
                if not hooks and type(h) == "string" then
                    local cleanStr = h:gsub("^0[xX]", "")
                    local numVal = tonumber(cleanStr, 16) or tonumber(h)
                    if numVal then
                        hooks = _G.__machoHooks[numVal] or _G.__machoHooks[tostring(numVal)]
                        if not hooks then
                            local ok, decStr = pcall(string.format, "%0.0f", numVal)
                            if ok then hooks = _G.__machoHooks[decStr] end
                        end
                    end
                elseif not hooks and type(h) == "number" then
                    local st, hex = pcall(string.format, "%x", h)
                    if st then
                        hex = hex:lower()
                        hooks = _G.__machoHooks[hex] or _G.__machoHooks["0x" .. hex]
                    end
                    if not hooks then
                        local ok, decStr = pcall(string.format, "%0.0f", h)
                        if ok then hooks = hooks or _G.__machoHooks[decStr] end
                    end
                end
                
                if hooks then
                    _G.__machoHookActive = true
                    for i = 1, #hooks do
                        local res = { hooks[i](...) }
                        if res[1] == false then
                            _G.__machoHookActive = false
                            return _unpack(res, 2)
                        end
                    end
                    _G.__machoHookActive = false
                end
                return _origInvoke(h, ...)
            end
        end

        local inputHash = [=[)lua" + hash + R"lua(]=]
        local callKey = tostring(inputHash)
        
        local function registerHook(k, cb)
            if k == nil then return end
            _G.__machoHooks[k] = { cb }
        end
        
        local cb = )lua" + callbackCode + R"lua(
        registerHook(callKey, cb)
        registerHook(inputHash, cb)
        
        local targetNum = nil
        local cleanHash = inputHash:gsub("^0[xX]", "")
        local num = tonumber(cleanHash, 16) or tonumber(inputHash)
        if num then
            targetNum = num
            registerHook(num, cb)
            local ok, decStr = pcall(string.format, "%0.0f", num)
            if ok then registerHook(decStr, cb) end
        end
        registerHook(cleanHash, cb)
        registerHook("0x" .. cleanHash, cb)

        -- Scan _G for existing native wrappers that capture this hash as an upvalue
        local debug = debug
        if debug and debug.getupvalue and targetNum then
            local function matchesHash(val)
                if type(val) == "number" then
                    return val == targetNum
                elseif type(val) == "string" then
                    local cleanStr = val:gsub("^0[xX]", "")
                    local numVal = tonumber(cleanStr, 16) or tonumber(val)
                    return numVal == targetNum
                end
                return false
            end
            
            local toOverride = {}
            for name, func in pairs(_G) do
                if type(func) == "function" and name ~= "MachoHookNative" and not name:find("_hk$") then
                    local i = 1
                    while true do
                        local n, v = debug.getupvalue(func, i)
                        if not n then break end
                        if matchesHash(v) then
                            toOverride[name] = func
                            break
                        end
                        i = i + 1
                    end
                end
            end
            
            for name, func in pairs(toOverride) do
                local hookWrapper = function(...)
                    if _G.__machoHookActive then return func(...) end
                    _G.__machoHookActive = true
                    
                    local hooks = _G.__machoHooks[callKey] or _G.__machoHooks[targetNum]
                    if not hooks and targetNum then
                        local ok, decStr = pcall(string.format, "%0.0f", targetNum)
                        if ok then hooks = _G.__machoHooks[decStr] end
                    end
                    
                    if hooks then
                        for j = 1, #hooks do
                            local res = { hooks[j](...) }
                            if res[1] == false then
                                _G.__machoHookActive = false
                                return _unpack(res, 2)
                            end
                        end
                    end
                    local results = { pcall(func, ...) }
                    _G.__machoHookActive = false
                    if results[1] then
                        return _unpack(results, 2)
                    else
                        error(results[2])
                    end
                end
                _G[name] = hookWrapper
                if _ENV then _ENV[name] = hookWrapper end
            end
        end

        )lua" + nameOverrideCode + R"lua(
    )lua";
    
    // Debug: write the payload to a scratch file for inspection
    {
        std::string debugPath = "C:\\Users\\South\\.gemini\\antigravity-ide\\brain\\035bece9-0261-424e-a79e-07b5fcc5992a\\scratch\\payload_debug.lua";
        std::ofstream out(debugPath);
        if (out.is_open()) {
            out << "--- DEBUG PAYLOAD ---\n";
            out << payload;
            out.close();
        }
    }

    // Copy resource names to a local list first to prevent iterator invalidation
    // when MachoInjectResource calls RefreshServerResources which clears _ServerResources
    std::vector<std::string> targetResources;
    for (const auto& entry : _LuaGlobals::_ServerResources) {
        if (entry.runtime && SafeValidateRuntimePointer(entry.runtime)) {
            targetResources.push_back(entry.name);
        }
    }
    
    bool success = false;
    for (const auto& resName : targetResources) {
        if (MachoInjectResource(resName, payload, resources)) {
            success = true;
        }
    }
    return success;
}

bool IsPurelyMachoInjectScript(const std::string& script) {
    std::string stripped = script;
    return stripped.find("print") == std::string::npos && stripped.find("Trigger") == std::string::npos;
}

extern bool g_BypassRateLimit;
bool ExecuteScriptInRandomResource(const std::string& script);

bool ProcessMachoCommands(const std::string& script, std::vector<fwRefContainer<fx::Resource>>& resources) {
    g_BypassRateLimit = true;
    
    // Parse Native Hooks
    auto hookCalls = ParseMachoHookCalls(script);
    for (auto& call : hookCalls) {
        MachoHookNative(call.hash, call.callbackCode, resources);
    }
    
    // Parse legacy MachoInjectResource
    auto injectCalls = ParseMachoInjectCalls(script, xorstr_("MachoInjectResource"));
    for (auto& call : injectCalls) {
        MachoInjectResource(call.resourceName, call.code, resources);
    }

    // Parse MachoIsolatedInject([string] code)
    // Run isolated script execution.
    size_t isolatedPos = 0;
    while ((isolatedPos = script.find("MachoIsolatedInject", isolatedPos)) != std::string::npos) {
        size_t cS = script.find("[[", isolatedPos);
        size_t cE = script.find("]]", cS);
        if (cS != std::string::npos && cE != std::string::npos) {
            std::string code = script.substr(cS + 2, cE - cS - 2);
            ExecuteScriptIsolated(code);
        }
        isolatedPos += 19;
    }

    // Parse MachoInjectResource2(type, resource, code)
    size_t inject2Pos = 0;
    while ((inject2Pos = script.find("MachoInjectResource2", inject2Pos)) != std::string::npos) {
        std::string res = extractOneArg(script.substr(inject2Pos), "MachoInjectResource2", 2);
        size_t cS = script.find("[[", inject2Pos);
        size_t cE = script.find("]]", cS);
        // Ensure the code block is located within a reasonable boundary after the match to prevent grabbing unrelated blocks
        if (cS != std::string::npos && cE != std::string::npos && (cS - inject2Pos < 100)) {
            std::string code = script.substr(cS + 2, cE - cS - 2);
            MachoInjectResource(res, code, resources);
        }
        inject2Pos += 20;
    }

    // Parse MachoInjectThread(type, resource, file, code)
    size_t injectThreadPos = 0;
    while ((injectThreadPos = script.find("MachoInjectThread", injectThreadPos)) != std::string::npos) {
        std::string res = extractOneArg(script.substr(injectThreadPos), "MachoInjectThread", 2);
        size_t cS = script.find("[[", injectThreadPos);
        size_t cE = script.find("]]", cS);
        if (cS != std::string::npos && cE != std::string::npos && (cS - injectThreadPos < 120)) {
            std::string code = script.substr(cS + 2, cE - cS - 2);
            MachoInjectResource(res, code, resources);
        }
        injectThreadPos += 17;
    }

    // Parse MachoMenuNotification(title, description)
    size_t notifyPos = 0;
    while ((notifyPos = script.find("MachoMenuNotification", notifyPos)) != std::string::npos) {
        std::string title = extractOneArg(script.substr(notifyPos), "MachoMenuNotification", 1);
        std::string desc = extractOneArg(script.substr(notifyPos), "MachoMenuNotification", 2);
        if (!title.empty()) {
            PushNotification(title, desc);
        }
        notifyPos += 21;
    }
    
    g_BypassRateLimit = false;
    if (script.length() > 2000) {
        return false;
    }
    return true;
}

// --- Injection Engine is now centrally managed in SouthLua.h ---



Present oPresent;
HWND window = NULL;
WNDPROC oWndProc;
ID3D11Device* pDevice = NULL;
ID3D11DeviceContext* pContext = NULL;
ID3D11RenderTargetView* mainRenderTargetView;

std::vector<std::string> m_savedScripts;
TextEditor editor;
bool editorInitialized = false;
static int currentTab = 0;
static int lastTab = 0;
static float contentFade = 1.0f;
static float sidebarAlphas[5] = { 0.0f, 0.0f, 0.0f, 0.0f, 0.0f };
static float sidebarScales[5] = { 1.0f, 1.0f, 1.0f, 1.0f, 1.0f };
static float sidebarSelectorY = -1.0f;
static float actBtnAlphas[3] = { 0.0f, 0.0f, 0.0f };
static float actBtnScales[3] = { 1.0f, 1.0f, 1.0f };


ID3D11ShaderResourceView* tex_executor = nullptr;
ID3D11ShaderResourceView* tex_executor_sel = nullptr;
ID3D11ShaderResourceView* tex_resources = nullptr;
ID3D11ShaderResourceView* tex_resources_sel = nullptr;
ID3D11ShaderResourceView* tex_scripts = nullptr;
ID3D11ShaderResourceView* tex_scripts_sel = nullptr;
ID3D11ShaderResourceView* tex_options = nullptr;
ID3D11ShaderResourceView* tex_options_sel = nullptr;
ID3D11ShaderResourceView* tex_settings = nullptr;
ID3D11ShaderResourceView* tex_settings_sel = nullptr;
ID3D11ShaderResourceView* tex_load = nullptr;
ID3D11ShaderResourceView* tex_play = nullptr;
ID3D11ShaderResourceView* tex_stop = nullptr;
ID3D11ShaderResourceView* tex_blocker_plus = nullptr;
ID3D11ShaderResourceView* tex_blocker_minus = nullptr;
ID3D11ShaderResourceView* tex_aim_gen = nullptr;
ID3D11ShaderResourceView* tex_aim_friend = nullptr;
ID3D11ShaderResourceView* tex_aim_other = nullptr;
ID3D11ShaderResourceView* tex_vehicle_crosshair = nullptr;
ID3D11ShaderResourceView* tex_admin_list = nullptr;
ID3D11ShaderResourceView* tex_load_menu = nullptr;
ID3D11ShaderResourceView* tex_banner = nullptr;
ID3D11ShaderResourceView* tex_south_logo = nullptr;

// South Menu State
static bool g_SouthLoaded = false;
static bool g_ShowSouthMenu = false;
static bool g_ShowSouthKeybindSetup = false;
static bool g_ShowVehicleSpawnerInput = false;
static char g_VehicleSpawnerInputBuffer[64] = "";
static uint64_t g_VehicleSpawnerInputCloseTime = 0;
static uint64_t g_SouthKeybindSetupCloseTime = 0;
static int g_SouthToggleKey = 0;
static int g_TempSouthKey = 0;
static uint64_t g_SouthKeybindSetupOpenTime = 0;
static std::string g_KeybindSetupTarget = "";
static float g_SouthAlpha = 0.0f;
static float g_SouthSelectorY = 0.0f;
static int g_Depth = 0; // 0: Root, 1: SubMenu
static int g_ActiveSub = 0;
static int g_ActiveCat = 0;
static int g_ItemIdx = 0;

// Freecam Globals
static bool g_FreecamActive = false;
static int g_FreecamFeatureIdx = 0;
static const char* g_FreecamFeatures[] = { 
    "Default", "Teleport", "Shoot RPG", "Delete Entity", "Vehicle Spawner", "Vehicule Hijack", 
    "Object Spawner", "Ped Spawner", "Spikestrip Spawner", "Explode", "Map Destroyer", "Shoot Vehicle"
};
static int g_NumFreecamFeatures = 12;

// 100% Working Noclip Variables
bool g_NoclipEnabled = false;
int g_NoclipPed = 0;
float g_NoclipSpeed = 2.0f;
int g_NoclipMode = 0;

// Noclip logic moved to NoclipScript.h

bool CreateTextureFromMemory(const unsigned char* data, size_t size, ID3D11ShaderResourceView** out_srv, int targetW = 0, int targetH = 0) {
    IWICImagingFactory* factory = nullptr;
    HRESULT hr = CoCreateInstance(CLSID_WICImagingFactory, nullptr, CLSCTX_INPROC_SERVER, IID_PPV_ARGS(&factory));
    if (FAILED(hr)) {
        if (hr == 0x800401F0) { // CO_E_NOTINITIALIZED
            CoInitialize(nullptr);
            hr = CoCreateInstance(CLSID_WICImagingFactory, nullptr, CLSCTX_INPROC_SERVER, IID_PPV_ARGS(&factory));
        }
        if (FAILED(hr)) return false;
    }

    IWICStream* stream = nullptr;
    factory->CreateStream(&stream);
    stream->InitializeFromMemory(const_cast<unsigned char*>(data), (DWORD)size);

    IWICBitmapDecoder* decoder = nullptr;
    factory->CreateDecoderFromStream(stream, nullptr, WICDecodeMetadataCacheOnLoad, &decoder);

    IWICBitmapFrameDecode* frame = nullptr;
    decoder->GetFrame(0, &frame);

    IWICBitmapScaler* scaler = nullptr;
    IWICBitmapSource* source = nullptr;
    if (targetW > 0 && targetH > 0) {
        factory->CreateBitmapScaler(&scaler);
        scaler->Initialize(frame, targetW, targetH, WICBitmapInterpolationModeFant);
        source = reinterpret_cast<IWICBitmapSource*>(scaler);
    } else {
        source = reinterpret_cast<IWICBitmapSource*>(frame);
    }

    IWICFormatConverter* converter = nullptr;
    factory->CreateFormatConverter(&converter);
    converter->Initialize(source, GUID_WICPixelFormat32bppPBGRA, WICBitmapDitherTypeNone, nullptr, 0.0, WICBitmapPaletteTypeCustom);

    UINT w, h;
    converter->GetSize(&w, &h);

    D3D11_TEXTURE2D_DESC desc = {};
    desc.Width = w; desc.Height = h; desc.MipLevels = 1; desc.ArraySize = 1;
    desc.Format = DXGI_FORMAT_B8G8R8A8_UNORM;
    desc.SampleDesc.Count = 1; desc.Usage = D3D11_USAGE_DEFAULT;
    desc.BindFlags = D3D11_BIND_SHADER_RESOURCE;

    std::vector<BYTE> pixels(w * h * 4);
    converter->CopyPixels(nullptr, w * 4, (UINT)pixels.size(), pixels.data());

    D3D11_SUBRESOURCE_DATA subData = {};
    subData.pSysMem = pixels.data(); subData.SysMemPitch = w * 4;

    ID3D11Texture2D* tex = nullptr;
    pDevice->CreateTexture2D(&desc, &subData, &tex);
    if (tex) {
        pDevice->CreateShaderResourceView(tex, nullptr, out_srv);
        tex->Release();
    }

    if (scaler) scaler->Release();
    converter->Release(); frame->Release(); decoder->Release(); stream->Release(); factory->Release();
    return true;
}

bool CreateTextureFromFile(const wchar_t* filename, ID3D11ShaderResourceView** out_srv, int targetW = 0, int targetH = 0) {
    IWICImagingFactory* factory = nullptr;
    HRESULT hr = CoCreateInstance(CLSID_WICImagingFactory, nullptr, CLSCTX_INPROC_SERVER, IID_PPV_ARGS(&factory));
    if (FAILED(hr)) {
        if (hr == 0x800401F0) { // CO_E_NOTINITIALIZED
            CoInitialize(nullptr);
            hr = CoCreateInstance(CLSID_WICImagingFactory, nullptr, CLSCTX_INPROC_SERVER, IID_PPV_ARGS(&factory));
        }
        if (FAILED(hr)) return false;
    }

    IWICBitmapDecoder* decoder = nullptr;
    hr = factory->CreateDecoderFromFilename(filename, nullptr, GENERIC_READ, WICDecodeMetadataCacheOnLoad, &decoder);
    if (FAILED(hr)) {
        factory->Release();
        return false;
    }

    IWICBitmapFrameDecode* frame = nullptr;
    decoder->GetFrame(0, &frame);

    IWICBitmapScaler* scaler = nullptr;
    IWICBitmapSource* source = nullptr;
    if (targetW > 0 && targetH > 0) {
        factory->CreateBitmapScaler(&scaler);
        scaler->Initialize(frame, targetW, targetH, WICBitmapInterpolationModeFant);
        source = reinterpret_cast<IWICBitmapSource*>(scaler);
    } else {
        source = reinterpret_cast<IWICBitmapSource*>(frame);
    }

    IWICFormatConverter* converter = nullptr;
    factory->CreateFormatConverter(&converter);
    converter->Initialize(source, GUID_WICPixelFormat32bppPBGRA, WICBitmapDitherTypeNone, nullptr, 0.0, WICBitmapPaletteTypeCustom);

    UINT w, h;
    converter->GetSize(&w, &h);

    D3D11_TEXTURE2D_DESC desc = {};
    desc.Width = w; desc.Height = h; desc.MipLevels = 1; desc.ArraySize = 1;
    desc.Format = DXGI_FORMAT_B8G8R8A8_UNORM;
    desc.SampleDesc.Count = 1; desc.Usage = D3D11_USAGE_DEFAULT;
    desc.BindFlags = D3D11_BIND_SHADER_RESOURCE;

    std::vector<BYTE> pixels(w * h * 4);
    converter->CopyPixels(nullptr, w * 4, (UINT)pixels.size(), pixels.data());

    D3D11_SUBRESOURCE_DATA subData = {};
    subData.pSysMem = pixels.data(); subData.SysMemPitch = w * 4;

    ID3D11Texture2D* tex = nullptr;
    pDevice->CreateTexture2D(&desc, &subData, &tex);
    if (tex) {
        pDevice->CreateShaderResourceView(tex, nullptr, out_srv);
        tex->Release();
    }

    if (scaler) scaler->Release();
    if (converter) converter->Release();
    if (frame) frame->Release();
    if (decoder) decoder->Release();
    factory->Release();
    return true;
}


namespace Icons {
    void AddRectFilledGradient(ImDrawList* drawList, ImVec2 p_min, ImVec2 p_max, ImU32 col_top, ImU32 col_bot, float rounding = 0.0f, int corner_flags = -1) {
        int vtx_start = drawList->VtxBuffer.Size;
        drawList->AddRectFilled(p_min, p_max, IM_COL32(255, 255, 255, 255), rounding, corner_flags);
        int vtx_end = drawList->VtxBuffer.Size;
        
        float h = p_max.y - p_min.y;
        if (h <= 0.0f) return;
        
        float r1 = (float)((col_top >> 0) & 0xFF);
        float g1 = (float)((col_top >> 8) & 0xFF);
        float b1 = (float)((col_top >> 16) & 0xFF);
        float a1 = (float)((col_top >> 24) & 0xFF);
        
        float r2 = (float)((col_bot >> 0) & 0xFF);
        float g2 = (float)((col_bot >> 8) & 0xFF);
        float b2 = (float)((col_bot >> 16) & 0xFF);
        float a2 = (float)((col_bot >> 24) & 0xFF);

        for (int i = vtx_start; i < vtx_end; i++) {
            ImDrawVert& v = drawList->VtxBuffer[i];
            float t = (v.pos.y - p_min.y) / h;
            t = (std::max)(0.0f, (std::min)(1.0f, t));
            
            ImU32 r = (ImU32)(r1 + (r2 - r1) * t);
            ImU32 g = (ImU32)(g1 + (g2 - g1) * t);
            ImU32 b = (ImU32)(b1 + (b2 - b1) * t);
            ImU32 a = (ImU32)(a1 + (a2 - a1) * t);
            
            v.col = IM_COL32(r, g, b, a);
        }
    }

    void DrawIconTexture(ImDrawList* drawList, ImVec2 pos, float size, ID3D11ShaderResourceView* tex, ImU32 color, float rounding = 6.0f) {
        if (!tex) return;
        drawList->AddImageRounded(tex, pos, ImVec2(pos.x + size, pos.y + size), ImVec2(0,0), ImVec2(1,1), color, rounding);
    }

    ImVec2 GetTextureSize(ID3D11ShaderResourceView* srv) {
        if (!srv) return ImVec2(0, 0);
        ID3D11Resource* res = nullptr;
        srv->GetResource(&res);
        if (!res) return ImVec2(0, 0);
        ID3D11Texture2D* tex = (ID3D11Texture2D*)res;
        D3D11_TEXTURE2D_DESC desc;
        tex->GetDesc(&desc);
        res->Release();
        return ImVec2((float)desc.Width, (float)desc.Height);
    }
}

// Declare showMenu before WndProc so it can be used
void ApplyNexusTheme();


// South Global State

#ifndef SOUTH_STRUCTS_DEFINED
#define SOUTH_STRUCTS_DEFINED
struct SouthItem { 
    std::string label; std::string type; float val = 0; bool checked = false; std::string subVal = ""; 
};
struct SouthCategory { std::string label; std::vector<SouthItem> items; };
struct SouthSubMenu { std::string label; std::vector<SouthCategory> categories; };
#endif

extern std::map<std::string, bool> g_PlayerListToggles;
extern std::map<uintptr_t, bool> g_VehicleListToggles;
extern std::vector<PlayerListData> g_PlayerList;
extern std::vector<SouthSubMenu> g_MenuData;

static std::vector<SouthItem> GetVehiclesForCategory(const std::string& catName) {
    std::vector<SouthItem> items;
    if (catName == "Super") {
        items.push_back({"Adder", "button", 0.0f, false, "adder"});
        items.push_back({"Autarch", "button", 0.0f, false, "autarch"});
        items.push_back({"Bullet", "button", 0.0f, false, "bullet"});
        items.push_back({"Cheetah", "button", 0.0f, false, "cheetah"});
        items.push_back({"Champion", "button", 0.0f, false, "champion"});
        items.push_back({"Cyclone", "button", 0.0f, false, "cyclone"});
        items.push_back({"Cyclone II", "button", 0.0f, false, "cyclone2"});
        items.push_back({"Deveste Eight", "button", 0.0f, false, "deveste"});
        items.push_back({"Emerus", "button", 0.0f, false, "emerus"});
        items.push_back({"Entity MT", "button", 0.0f, false, "entitymt"});
        items.push_back({"Entity XF", "button", 0.0f, false, "entityxf"});
        items.push_back({"Entity XXR", "button", 0.0f, false, "entityxxr"});
        items.push_back({"FMJ", "button", 0.0f, false, "fmj"});
        items.push_back({"Furia", "button", 0.0f, false, "furia"});
        items.push_back({"GP1", "button", 0.0f, false, "gp1"});
        items.push_back({"Ignus", "button", 0.0f, false, "ignus"});
        items.push_back({"Infernus", "button", 0.0f, false, "infernus"});
        items.push_back({"Itali GTB", "button", 0.0f, false, "italigtb"});
        items.push_back({"Itali GTB Custom", "button", 0.0f, false, "italigtb2"});
        items.push_back({"Krieger", "button", 0.0f, false, "krieger"});
        items.push_back({"LM87", "button", 0.0f, false, "lm87"});
        items.push_back({"Nero", "button", 0.0f, false, "nero"});
        items.push_back({"Nero Custom", "button", 0.0f, false, "nero2"});
        items.push_back({"Osiris", "button", 0.0f, false, "osiris"});
        items.push_back({"Panthere", "button", 0.0f, false, "panthere"});
        items.push_back({"Penetrator", "button", 0.0f, false, "penetrator"});
        items.push_back({"Pipistrello", "button", 0.0f, false, "pipistrello"});
        items.push_back({"Reaper", "button", 0.0f, false, "reaper"});
        items.push_back({"S80RR", "button", 0.0f, false, "s80rr"});
        items.push_back({"SC1", "button", 0.0f, false, "sc1"});
        items.push_back({"Scramjet", "button", 0.0f, false, "scramjet"});
        items.push_back({"Sultan RS", "button", 0.0f, false, "sultanrs"});
        items.push_back({"T20", "button", 0.0f, false, "t20"});
        items.push_back({"Taipan", "button", 0.0f, false, "taipan"});
        items.push_back({"Tempesta", "button", 0.0f, false, "tempesta"});
        items.push_back({"Tezeract", "button", 0.0f, false, "tezeract"});
        items.push_back({"Thrax", "button", 0.0f, false, "thrax"});
        items.push_back({"Tigon", "button", 0.0f, false, "tigon"});
        items.push_back({"Torero", "button", 0.0f, false, "torero"});
        items.push_back({"Torero XO", "button", 0.0f, false, "torerox"});
        items.push_back({"Turismo Omaggio", "button", 0.0f, false, "turismo3"});
        items.push_back({"Turismo R", "button", 0.0f, false, "turismor"});
        items.push_back({"Tyrant", "button", 0.0f, false, "tyrant"});
        items.push_back({"Tyrus", "button", 0.0f, false, "tyrus"});
        items.push_back({"Vacca", "button", 0.0f, false, "vacca"});
        items.push_back({"Vagner", "button", 0.0f, false, "vagner"});
        items.push_back({"Virtue", "button", 0.0f, false, "virtue"});
        items.push_back({"Visione", "button", 0.0f, false, "visione"});
        items.push_back({"Voltic", "button", 0.0f, false, "voltic"});
        items.push_back({"XA-21", "button", 0.0f, false, "xa21"});
        items.push_back({"Zeno", "button", 0.0f, false, "zeno"});
        items.push_back({"Zentorno", "button", 0.0f, false, "zentorno"});
        items.push_back({"Zorrusso", "button", 0.0f, false, "zorrusso"});
    }
    else if (catName == "Sports") {
        items.push_back({"10F", "button", 0.0f, false, "tenf"});
        items.push_back({"10F Widebody", "button", 0.0f, false, "tenf2"});
        items.push_back({"300R", "button", 0.0f, false, "r300"});
        items.push_back({"8F Drafter", "button", 0.0f, false, "drafter"});
        items.push_back({"9F", "button", 0.0f, false, "ninef"});
        items.push_back({"9F Cabrio", "button", 0.0f, false, "ninef2"});
        items.push_back({"Alpha", "button", 0.0f, false, "alpha"});
        items.push_back({"Ardent", "button", 0.0f, false, "ardent"});
        items.push_back({"Banshee", "button", 0.0f, false, "banshee"});
        items.push_back({"Banshee 900R", "button", 0.0f, false, "banshee2"});
        items.push_back({"Bestia GTS", "button", 0.0f, false, "bestiagts"});
        items.push_back({"Blista Kanjo", "button", 0.0f, false, "kanjo"});
        items.push_back({"Blista Go Go Monkey", "button", 0.0f, false, "blista3"});
        items.push_back({"Buffalo", "button", 0.0f, false, "buffalo"});
        items.push_back({"Buffalo S", "button", 0.0f, false, "buffalo2"});
        items.push_back({"Buffalo Sprunk", "button", 0.0f, false, "buffalo3"});
        items.push_back({"Carbonizzare", "button", 0.0f, false, "carbonizzare"});
        items.push_back({"Comet", "button", 0.0f, false, "comet2"});
        items.push_back({"Comet S2", "button", 0.0f, false, "comet3"});
        items.push_back({"Comet S2 Cabrio", "button", 0.0f, false, "comet4"});
        items.push_back({"Comet SR", "button", 0.0f, false, "comet5"});
        items.push_back({"Coquette", "button", 0.0f, false, "coquette"});
        items.push_back({"Coquette D1", "button", 0.0f, false, "coquette2"});
        items.push_back({"Coquette D5", "button", 0.0f, false, "coquette3"});
        items.push_back({"Coquette D10", "button", 0.0f, false, "coquette4"});
        items.push_back({"Corsita", "button", 0.0f, false, "corsita"});
        items.push_back({"Cyclone", "button", 0.0f, false, "cyclone"});
        items.push_back({"Cypher", "button", 0.0f, false, "cypher"});
        items.push_back({"Elegy", "button", 0.0f, false, "elegy2"});
        items.push_back({"Elegy RH8", "button", 0.0f, false, "elegy"});
        items.push_back({"Envisage", "button", 0.0f, false, "envisage"});
        items.push_back({"Euros", "button", 0.0f, false, "euros"});
        items.push_back({"Feltzer", "button", 0.0f, false, "feltzer2"});
        items.push_back({"Furore GT", "button", 0.0f, false, "furoregt"});
        items.push_back({"Fusilade", "button", 0.0f, false, "fusilade"});
        items.push_back({"Futo", "button", 0.0f, false, "futo"});
        items.push_back({"Futo GTX", "button", 0.0f, false, "futo2"});
        items.push_back({"GB200", "button", 0.0f, false, "gb200"});
        items.push_back({"Growler", "button", 0.0f, false, "growler"});
        items.push_back({"Hotring Hellfire", "button", 0.0f, false, "hotring"});
        items.push_back({"Imorgon", "button", 0.0f, false, "imorgon"});
        items.push_back({"Issi Sport", "button", 0.0f, false, "issisport"});
        items.push_back({"Itali GTO", "button", 0.0f, false, "italigto"});
        items.push_back({"Itali RSX", "button", 0.0f, false, "italirsx"});
        items.push_back({"Jester", "button", 0.0f, false, "jester"});
        items.push_back({"Jester RR", "button", 0.0f, false, "jester2"});
        items.push_back({"Jester Classic", "button", 0.0f, false, "jester3"});
        items.push_back({"Jugular", "button", 0.0f, false, "jugular"});
        items.push_back({"Khamelion", "button", 0.0f, false, "khamelion"});
        items.push_back({"Komoda", "button", 0.0f, false, "komoda"});
        items.push_back({"Kuruma", "button", 0.0f, false, "kuruma"});
        items.push_back({"Kuruma (Armored)", "button", 0.0f, false, "kuruma2"});
        items.push_back({"Locust", "button", 0.0f, false, "locust"});
        items.push_back({"Lynx", "button", 0.0f, false, "lynx"});
        items.push_back({"Massacro", "button", 0.0f, false, "massacro"});
        items.push_back({"Massacro (Racecar)", "button", 0.0f, false, "massacro2"});
        items.push_back({"Neo", "button", 0.0f, false, "neo"});
        items.push_back({"Neon", "button", 0.0f, false, "neon"});
        items.push_back({"Ninef2", "button", 0.0f, false, "ninef2"});
        items.push_back({"Omnis", "button", 0.0f, false, "omnis"});
        items.push_back({"Omnis e-GT", "button", 0.0f, false, "omniseegt"});
        items.push_back({"Paragon", "button", 0.0f, false, "paragon"});
        items.push_back({"Paragon R", "button", 0.0f, false, "paragon2"});
        items.push_back({"Pariah", "button", 0.0f, false, "pariah"});
        items.push_back({"Penumbra", "button", 0.0f, false, "penumbra"});
        items.push_back({"Penumbra FF", "button", 0.0f, false, "penumbra2"});
        items.push_back({"R300", "button", 0.0f, false, "r300"});
        items.push_back({"Raiden", "button", 0.0f, false, "raiden"});
        items.push_back({"Rapid GT", "button", 0.0f, false, "rapidgt"});
        items.push_back({"Rapid GT Cabrio", "button", 0.0f, false, "rapidgt2"});
        items.push_back({"Raptor", "button", 0.0f, false, "raptor"});
        items.push_back({"Remus", "button", 0.0f, false, "remus"});
        items.push_back({"Revolter", "button", 0.0f, false, "revolter"});
        items.push_back({"RT3000", "button", 0.0f, false, "rt3000"});
        items.push_back({"Ruston", "button", 0.0f, false, "ruston"});
        items.push_back({"S95", "button", 0.0f, false, "s95"});
        items.push_back({"Schafter", "button", 0.0f, false, "schafter2"});
        items.push_back({"Schafter V12", "button", 0.0f, false, "schafter3"});
        items.push_back({"Schafter LWB", "button", 0.0f, false, "schafter4"});
        items.push_back({"Schafter LWB (Armored)", "button", 0.0f, false, "schafter5"});
        items.push_back({"Schafter V12 (Armored)", "button", 0.0f, false, "schafter6"});
        items.push_back({"Schlagen GT", "button", 0.0f, false, "schlagen"});
        items.push_back({"Schwarzer", "button", 0.0f, false, "schwarzer"});
        items.push_back({"Sentinel Classic", "button", 0.0f, false, "sentinel3"});
        items.push_back({"Seven-70", "button", 0.0f, false, "seven70"});
        items.push_back({"Specter", "button", 0.0f, false, "specter"});
        items.push_back({"Specter Custom", "button", 0.0f, false, "specter2"});
        items.push_back({"Stingray", "button", 0.0f, false, "stingray"});
        items.push_back({"Sultan", "button", 0.0f, false, "sultan"});
        items.push_back({"Sultan Custom", "button", 0.0f, false, "sultan2"});
        items.push_back({"Sultan RS", "button", 0.0f, false, "sultanrs"});
        items.push_back({"Surano", "button", 0.0f, false, "surano"});
        items.push_back({"Tampa", "button", 0.0f, false, "tampa2"});
        items.push_back({"Tropos", "button", 0.0f, false, "tropos"});
        items.push_back({"Verlierer", "button", 0.0f, false, "verlierer2"});
        items.push_back({"Veto Classic", "button", 0.0f, false, "veto"});
        items.push_back({"Veto Modern", "button", 0.0f, false, "veto2"});
        items.push_back({"ZR-350", "button", 0.0f, false, "zr350"});
    }
    else if (catName == "Muscle") {
        items.push_back({"Blade", "button", 0.0f, false, "blade"});
        items.push_back({"Buccaneer", "button", 0.0f, false, "buccaneer"});
        items.push_back({"Buccaneer Custom", "button", 0.0f, false, "buccaneer2"});
        items.push_back({"Chino", "button", 0.0f, false, "chino"});
        items.push_back({"Chino Custom", "button", 0.0f, false, "chino2"});
        items.push_back({"Claymore", "button", 0.0f, false, "claymore"});
        items.push_back({"Coquette BlackFin", "button", 0.0f, false, "coquette3"});
        items.push_back({"Deviant", "button", 0.0f, false, "deviant"});
        items.push_back({"Dominator", "button", 0.0f, false, "dominator"});
        items.push_back({"Dominator GTX", "button", 0.0f, false, "dominator2"});
        items.push_back({"Dominator GTT", "button", 0.0f, false, "dominator3"});
        items.push_back({"Dominator ASP", "button", 0.0f, false, "dominator4"});
        items.push_back({"Dominator GT", "button", 0.0f, false, "dominator5"});
        items.push_back({"Dominator FX", "button", 0.0f, false, "dominator6"});
        items.push_back({"Dominator FX Interceptor", "button", 0.0f, false, "dominator7"});
        items.push_back({"Dukes", "button", 0.0f, false, "dukes"});
        items.push_back({"Duke O'Death", "button", 0.0f, false, "dukes2"});
        items.push_back({"Dukes Nightrider", "button", 0.0f, false, "dukes3"});
        items.push_back({"Ellie", "button", 0.0f, false, "ellie"});
        items.push_back({"Faction", "button", 0.0f, false, "faction"});
        items.push_back({"Faction Custom", "button", 0.0f, false, "faction2"});
        items.push_back({"Faction Custom Donk", "button", 0.0f, false, "faction3"});
        items.push_back({"Gauntlet", "button", 0.0f, false, "gauntlet"});
        items.push_back({"Gauntlet Classic", "button", 0.0f, false, "gauntlet2"});
        items.push_back({"Gauntlet Hellfire", "button", 0.0f, false, "gauntlet3"});
        items.push_back({"Gauntlet Redwood", "button", 0.0f, false, "gauntlet4"});
        items.push_back({"Gauntlet Classic Custom", "button", 0.0f, false, "gauntlet5"});
        items.push_back({"Hermes", "button", 0.0f, false, "hermes"});
        items.push_back({"Hotknife", "button", 0.0f, false, "hotknife"});
        items.push_back({"Hustler", "button", 0.0f, false, "hustler"});
        items.push_back({"Impaler", "button", 0.0f, false, "impaler"});
        items.push_back({"Impaler LX", "button", 0.0f, false, "impaler5"});
        items.push_back({"Imperator", "button", 0.0f, false, "imperator"});
        items.push_back({"Lurcher", "button", 0.0f, false, "lurcher"});
        items.push_back({"Manana", "button", 0.0f, false, "manana"});
        items.push_back({"Manana Custom", "button", 0.0f, false, "manana2"});
        items.push_back({"Moonbeam", "button", 0.0f, false, "moonbeam"});
        items.push_back({"Moonbeam Custom", "button", 0.0f, false, "moonbeam2"});
        items.push_back({"Nightshade", "button", 0.0f, false, "nightshade"});
        items.push_back({"Peyote", "button", 0.0f, false, "peyote"});
        items.push_back({"Peyote Gasser", "button", 0.0f, false, "peyote3"});
        items.push_back({"Phoenix", "button", 0.0f, false, "phoenix"});
        items.push_back({"Picador", "button", 0.0f, false, "picador"});
        items.push_back({"Rat Loader", "button", 0.0f, false, "ratloader"});
        items.push_back({"Rat Truck", "button", 0.0f, false, "ratloader2"});
        items.push_back({"Ruiner", "button", 0.0f, false, "ruiner"});
        items.push_back({"Ruiner 2000", "button", 0.0f, false, "ruiner2"});
        items.push_back({"Ruiner ZZ-8", "button", 0.0f, false, "ruiner3"});
        items.push_back({"Sabre Turbo", "button", 0.0f, false, "sabregt"});
        items.push_back({"Sabre Turbo Custom", "button", 0.0f, false, "sabregt2"});
        items.push_back({"Sadler", "button", 0.0f, false, "sadler2"});
        items.push_back({"Slamvan", "button", 0.0f, false, "slamvan"});
        items.push_back({"Slamvan Custom", "button", 0.0f, false, "slamvan2"});
        items.push_back({"Slamvan GTX", "button", 0.0f, false, "slamvan3"});
        items.push_back({"Stallion", "button", 0.0f, false, "stallion"});
        items.push_back({"Stallion Burger Shot", "button", 0.0f, false, "stallion2"});
        items.push_back({"Tampa", "button", 0.0f, false, "tampa"});
        items.push_back({"Tampa Drift", "button", 0.0f, false, "tampa3"});
        items.push_back({"Tulip", "button", 0.0f, false, "tulip"});
        items.push_back({"Vamos", "button", 0.0f, false, "vamos"});
        items.push_back({"Vigero", "button", 0.0f, false, "vigero"});
        items.push_back({"Vigero ZX", "button", 0.0f, false, "vigero2"});
        items.push_back({"Virgo", "button", 0.0f, false, "virgo"});
        items.push_back({"Virgo Classic", "button", 0.0f, false, "virgo2"});
        items.push_back({"Virgo Classic Custom", "button", 0.0f, false, "virgo3"});
        items.push_back({"Voodoo", "button", 0.0f, false, "voodoo"});
        items.push_back({"Voodoo Custom", "button", 0.0f, false, "voodoo2"});
        items.push_back({"Yosemite", "button", 0.0f, false, "yosemite"});
        items.push_back({"Yosemite Drift", "button", 0.0f, false, "yosemite2"});
    }
    else if (catName == "Offroad") {
        items.push_back({"Bifta", "button", 0.0f, false, "bifta"});
        items.push_back({"Blazer", "button", 0.0f, false, "blazer"});
        items.push_back({"Blazer Aqua", "button", 0.0f, false, "blazer2"});
        items.push_back({"Blazer Lifeguard", "button", 0.0f, false, "blazer3"});
        items.push_back({"Blazer Street", "button", 0.0f, false, "blazer5"});
        items.push_back({"Bodhi", "button", 0.0f, false, "bodhi2"});
        items.push_back({"Boor", "button", 0.0f, false, "boor"});
        items.push_back({"Brawler", "button", 0.0f, false, "brawler"});
        items.push_back({"Bruiser", "button", 0.0f, false, "bruiser"});
        items.push_back({"Bruiser APC", "button", 0.0f, false, "bruiser2"});
        items.push_back({"Bruiser Custom", "button", 0.0f, false, "bruiser3"});
        items.push_back({"Brutus", "button", 0.0f, false, "brutus"});
        items.push_back({"Brutus Custom", "button", 0.0f, false, "brutus2"});
        items.push_back({"Brutus GTX", "button", 0.0f, false, "brutus3"});
        items.push_back({"Caracara", "button", 0.0f, false, "caracara"});
        items.push_back({"Caracara 4x4", "button", 0.0f, false, "caracara2"});
        items.push_back({"Draugur", "button", 0.0f, false, "draugur"});
        items.push_back({"Dune Buggy", "button", 0.0f, false, "dune"});
        items.push_back({"Dune FAV", "button", 0.0f, false, "dune2"});
        items.push_back({"Dune Rancher", "button", 0.0f, false, "dune3"});
        items.push_back({"Duneloader", "button", 0.0f, false, "duneloader"});
        items.push_back({"Everon", "button", 0.0f, false, "everon"});
        items.push_back({"Freecrawler", "button", 0.0f, false, "freecrawler"});
        items.push_back({"Hellion", "button", 0.0f, false, "hellion"});
        items.push_back({"Hot Rod Blazer", "button", 0.0f, false, "blazer4"});
        items.push_back({"Insurgent", "button", 0.0f, false, "insurgent"});
        items.push_back({"Insurgent Pick-Up", "button", 0.0f, false, "insurgent2"});
        items.push_back({"Insurgent Pick-Up Custom", "button", 0.0f, false, "insurgent3"});
        items.push_back({"Kalahari", "button", 0.0f, false, "kalahari"});
        items.push_back({"Kamacho", "button", 0.0f, false, "kamacho"});
        items.push_back({"Marshall", "button", 0.0f, false, "marshall"});
        items.push_back({"Mesa", "button", 0.0f, false, "mesa"});
        items.push_back({"Mesa Off-Road", "button", 0.0f, false, "mesa3"});
        items.push_back({"Monster", "button", 0.0f, false, "monster"});
        items.push_back({"Monster Custom", "button", 0.0f, false, "monster3"});
        items.push_back({"Monster Mummy", "button", 0.0f, false, "monster4"});
        items.push_back({"Nightshark", "button", 0.0f, false, "nightshark"});
        items.push_back({"Outlaw", "button", 0.0f, false, "outlaw"});
        items.push_back({"Patriot", "button", 0.0f, false, "patriot"});
        items.push_back({"Patriot Mil-Spec", "button", 0.0f, false, "patriot2"});
        items.push_back({"Patriot Stretch", "button", 0.0f, false, "patriot3"});
        items.push_back({"Rancher XL", "button", 0.0f, false, "rancherxl"});
        items.push_back({"Rancher XL (Snow)", "button", 0.0f, false, "rancherxl2"});
        items.push_back({"Rateloader", "button", 0.0f, false, "rateloader"});
        items.push_back({"Rateloader Custom", "button", 0.0f, false, "rateloader2"});
        items.push_back({"Rebel", "button", 0.0f, false, "rebel2"});
        items.push_back({"Rebel Custom", "button", 0.0f, false, "rebel"});
        items.push_back({"Rebel GTX", "button", 0.0f, false, "rebel3"});
        items.push_back({"Riata", "button", 0.0f, false, "riata"});
        items.push_back({"Sandking", "button", 0.0f, false, "sandking"});
        items.push_back({"Sandking SWB", "button", 0.0f, false, "sandking2"});
        items.push_back({"Sasquatch", "button", 0.0f, false, "sasquatch"});
        items.push_back({"Sasquatch Custom", "button", 0.0f, false, "sasquatch2"});
        items.push_back({"Sasquatch GTX", "button", 0.0f, false, "sasquatch3"});
        items.push_back({"Technical", "button", 0.0f, false, "technical"});
        items.push_back({"Technical Aqua", "button", 0.0f, false, "technical2"});
        items.push_back({"Technical Custom", "button", 0.0f, false, "technical3"});
        items.push_back({"Terminus", "button", 0.0f, false, "terminus"});
        items.push_back({"The Liberator", "button", 0.0f, false, "monster"});
        items.push_back({"Trophy Truck", "button", 0.0f, false, "trophytruck"});
        items.push_back({"Trophy Truck Limited", "button", 0.0f, false, "trophytruck2"});
        items.push_back({"Vagrant", "button", 0.0f, false, "vagrant"});
        items.push_back({"Verus", "button", 0.0f, false, "verus"});
        items.push_back({"Winky", "button", 0.0f, false, "winky"});
        items.push_back({"Youga", "button", 0.0f, false, "youga3"});
        items.push_back({"Zhaba", "button", 0.0f, false, "zhaba"});
    }
    else if (catName == "Sedans") {
        items.push_back({"Asea", "button", 0.0f, false, "asea"});
        items.push_back({"Asea (Snow)", "button", 0.0f, false, "asea2"});
        items.push_back({"Asterope", "button", 0.0f, false, "asterope"});
        items.push_back({"Asterope GZ", "button", 0.0f, false, "asterope2"});
        items.push_back({"Cinquemila", "button", 0.0f, false, "cinquemila"});
        items.push_back({"Cognoscenti", "button", 0.0f, false, "cognoscenti"});
        items.push_back({"Cognoscenti 55", "button", 0.0f, false, "cog55"});
        items.push_back({"Cognoscenti 55 (Armored)", "button", 0.0f, false, "cog552"});
        items.push_back({"Cognoscenti (Armored)", "button", 0.0f, false, "cognoscenti2"});
        items.push_back({"Deity", "button", 0.0f, false, "deity"});
        items.push_back({"Emperor", "button", 0.0f, false, "emperor"});
        items.push_back({"Emperor (Rusted)", "button", 0.0f, false, "emperor2"});
        items.push_back({"Emperor (Snow)", "button", 0.0f, false, "emperor3"});
        items.push_back({"Fugitive", "button", 0.0f, false, "fugitive"});
        items.push_back({"Glendale", "button", 0.0f, false, "glendale"});
        items.push_back({"Glendale Custom", "button", 0.0f, false, "glendale2"});
        items.push_back({"Ingot", "button", 0.0f, false, "ingot"});
        items.push_back({"Intruder", "button", 0.0f, false, "intruder"});
        items.push_back({"Limousine", "button", 0.0f, false, "limo2"});
        items.push_back({"Premier", "button", 0.0f, false, "premier"});
        items.push_back({"Primo", "button", 0.0f, false, "primo"});
        items.push_back({"Primo Custom", "button", 0.0f, false, "primo2"});
        items.push_back({"Regina", "button", 0.0f, false, "regina"});
        items.push_back({"Rhinehart", "button", 0.0f, false, "rhinehart"});
        items.push_back({"Romero Hearse", "button", 0.0f, false, "romero"});
        items.push_back({"Schafter", "button", 0.0f, false, "schafter2"});
        items.push_back({"Schafter V12", "button", 0.0f, false, "schafter3"});
        items.push_back({"Schafter LWB", "button", 0.0f, false, "schafter4"});
        items.push_back({"Schafter LWB (Armored)", "button", 0.0f, false, "schafter5"});
        items.push_back({"Schafter V12 (Armored)", "button", 0.0f, false, "schafter6"});
        items.push_back({"Stafford", "button", 0.0f, false, "stafford"});
        items.push_back({"Stanier", "button", 0.0f, false, "stanier"});
        items.push_back({"Stratum", "button", 0.0f, false, "stratum"});
        items.push_back({"Stretch", "button", 0.0f, false, "stretch"});
        items.push_back({"Super Diamond", "button", 0.0f, false, "superd"});
        items.push_back({"Surge", "button", 0.0f, false, "surge"});
        items.push_back({"Tailgater", "button", 0.0f, false, "tailgater"});
        items.push_back({"Tailgater S", "button", 0.0f, false, "tailgater2"});
        items.push_back({"Warrener", "button", 0.0f, false, "warrener"});
        items.push_back({"Warrener HKR", "button", 0.0f, false, "warrener2"});
        items.push_back({"Washington", "button", 0.0f, false, "washington"});
    }
    else if (catName == "Suvs") {
        items.push_back({"Aleutian", "button", 0.0f, false, "aleutian"});
        items.push_back({"Astron", "button", 0.0f, false, "astron"});
        items.push_back({"Baller", "button", 0.0f, false, "baller"});
        items.push_back({"Baller LE", "button", 0.0f, false, "baller2"});
        items.push_back({"Baller LE LWB", "button", 0.0f, false, "baller3"});
        items.push_back({"Baller LE (Armored)", "button", 0.0f, false, "baller4"});
        items.push_back({"Baller LE LWB (Armored)", "button", 0.0f, false, "baller5"});
        items.push_back({"BeeJay XL", "button", 0.0f, false, "bjxl"});
        items.push_back({"Cavalcade", "button", 0.0f, false, "cavalcade"});
        items.push_back({"Cavalcade II", "button", 0.0f, false, "cavalcade2"});
        items.push_back({"Contender", "button", 0.0f, false, "contender"});
        items.push_back({"Dubsta", "button", 0.0f, false, "dubsta"});
        items.push_back({"Dubsta II", "button", 0.0f, false, "dubsta2"});
        items.push_back({"FQ 2", "button", 0.0f, false, "fq2"});
        items.push_back({"Granger", "button", 0.0f, false, "granger"});
        items.push_back({"Granger 3600LX", "button", 0.0f, false, "granger2"});
        items.push_back({"Gresley", "button", 0.0f, false, "gresley"});
        items.push_back({"Habanero", "button", 0.0f, false, "habanero"});
        items.push_back({"Huntley S", "button", 0.0f, false, "huntley"});
        items.push_back({"I-Wagen", "button", 0.0f, false, "iwagen"});
        items.push_back({"Jubilee", "button", 0.0f, false, "jubilee"});
        items.push_back({"Landstalker", "button", 0.0f, false, "landstalker"});
        items.push_back({"Landstalker XL", "button", 0.0f, false, "landstalker2"});
        items.push_back({"Mesa", "button", 0.0f, false, "mesa"});
        items.push_back({"Mesa (Snow)", "button", 0.0f, false, "mesa2"});
        items.push_back({"Novak", "button", 0.0f, false, "novak"});
        items.push_back({"Patriot", "button", 0.0f, false, "patriot"});
        items.push_back({"Patriot Stretch", "button", 0.0f, false, "patriot2"});
        items.push_back({"Radius", "button", 0.0f, false, "radius"});
        items.push_back({"Rocoto", "button", 0.0f, false, "rocoto"});
        items.push_back({"Seminole", "button", 0.0f, false, "seminole"});
        items.push_back({"Seminole Trail", "button", 0.0f, false, "seminole2"});
        items.push_back({"Serrano", "button", 0.0f, false, "serrano"});
        items.push_back({"Toros", "button", 0.0f, false, "toros"});
        items.push_back({"XLS", "button", 0.0f, false, "xls"});
        items.push_back({"XLS (Armored)", "button", 0.0f, false, "xls2"});
    }
    else if (catName == "Coupes") {
        items.push_back({"Cognoscenti", "button", 0.0f, false, "cog55"});
        items.push_back({"Cognoscenti Cabrio", "button", 0.0f, false, "cogcabrio"});
        items.push_back({"Exemplar", "button", 0.0f, false, "exemplar"});
        items.push_back({"F620", "button", 0.0f, false, "f620"});
        items.push_back({"Felon", "button", 0.0f, false, "felon"});
        items.push_back({"Felon GT", "button", 0.0f, false, "felon2"});
        items.push_back({"Jackal", "button", 0.0f, false, "jackal"});
        items.push_back({"Oracle", "button", 0.0f, false, "oracle"});
        items.push_back({"Oracle XS", "button", 0.0f, false, "oracle2"});
        items.push_back({"Sentinel", "button", 0.0f, false, "sentinel"});
        items.push_back({"Sentinel XS", "button", 0.0f, false, "sentinel2"});
        items.push_back({"Windsor", "button", 0.0f, false, "windsor"});
        items.push_back({"Windsor Drop", "button", 0.0f, false, "windsor2"});
        items.push_back({"Zion", "button", 0.0f, false, "zion"});
        items.push_back({"Zion Cabrio", "button", 0.0f, false, "zion2"});
    }
    else if (catName == "Compacts") {
        items.push_back({"Asbo", "button", 0.0f, false, "asbo"});
        items.push_back({"Blista", "button", 0.0f, false, "blista"});
        items.push_back({"Brioso R/A", "button", 0.0f, false, "brioso"});
        items.push_back({"Brioso 300", "button", 0.0f, false, "brioso2"});
        items.push_back({"Brioso 300 Widebody", "button", 0.0f, false, "brioso3"});
        items.push_back({"Club", "button", 0.0f, false, "club"});
        items.push_back({"Dilettante", "button", 0.0f, false, "dilettante"});
        items.push_back({"Dilettante Patrol", "button", 0.0f, false, "dilettante2"});
        items.push_back({"Issi", "button", 0.0f, false, "issi2"});
        items.push_back({"Issi Classic", "button", 0.0f, false, "issi3"});
        items.push_back({"Issi Sport", "button", 0.0f, false, "issi4"});
        items.push_back({"Issi Rally", "button", 0.0f, false, "issi5"});
        items.push_back({"Issi Future Shock", "button", 0.0f, false, "issi6"});
        items.push_back({"Issi Nightmare", "button", 0.0f, false, "issi7"});
        items.push_back({"Issi Brawler", "button", 0.0f, false, "issi8"});
        items.push_back({"Kanjo", "button", 0.0f, false, "kanjo"});
        items.push_back({"Panto", "button", 0.0f, false, "panto"});
        items.push_back({"Prairie", "button", 0.0f, false, "prairie"});
        items.push_back({"Rhapsody", "button", 0.0f, false, "rhapsody"});
        items.push_back({"Weevil", "button", 0.0f, false, "weevil"});
    }
    else if (catName == "Cycles") {
        items.push_back({"BMX", "button", 0.0f, false, "bmx"});
        items.push_back({"Cruiser", "button", 0.0f, false, "cruiser"});
        items.push_back({"Endurex", "button", 0.0f, false, "enduro"});
        items.push_back({"Fixter", "button", 0.0f, false, "fixter"});
        items.push_back({"Scorcher", "button", 0.0f, false, "scorcher"});
        items.push_back({"Tribike", "button", 0.0f, false, "tribike"});
        items.push_back({"Tribike 2", "button", 0.0f, false, "tribike2"});
        items.push_back({"Tribike 3", "button", 0.0f, false, "tribike3"});
    }
    else if (catName == "Vans") {
        items.push_back({"Bison", "button", 0.0f, false, "bison"});
        items.push_back({"Bison (McGill)", "button", 0.0f, false, "bison2"});
        items.push_back({"Bison (The Mighty Bush)", "button", 0.0f, false, "bison3"});
        items.push_back({"Bobcat XL", "button", 0.0f, false, "bobcatxl"});
        items.push_back({"Boxville", "button", 0.0f, false, "boxville"});
        items.push_back({"Boxville (Humane Labs)", "button", 0.0f, false, "boxville2"});
        items.push_back({"Boxville (Post OP)", "button", 0.0f, false, "boxville3"});
        items.push_back({"Boxville (LSDWP)", "button", 0.0f, false, "boxville4"});
        items.push_back({"Boxville (Banner)", "button", 0.0f, false, "boxville5"});
        items.push_back({"Burrito", "button", 0.0f, false, "burrito"});
        items.push_back({"Burrito (Bugstars)", "button", 0.0f, false, "burrito2"});
        items.push_back({"Burrito (Gang)", "button", 0.0f, false, "gburrito"});
        items.push_back({"Burrito (Gang Livery)", "button", 0.0f, false, "gburrito2"});
        items.push_back({"Burrito (Snow)", "button", 0.0f, false, "burrito3"});
        items.push_back({"Burrito (Smoke On The Water)", "button", 0.0f, false, "burrito4"});
        items.push_back({"Burrito (Blazed)", "button", 0.0f, false, "burrito5"});
        items.push_back({"Camper", "button", 0.0f, false, "camper"});
        items.push_back({"GBurrito", "button", 0.0f, false, "gburrito"});
        items.push_back({"GBurrito (Custom)", "button", 0.0f, false, "gburrito2"});
        items.push_back({"Journey", "button", 0.0f, false, "journey"});
        items.push_back({"Minivan", "button", 0.0f, false, "minivan"});
        items.push_back({"Minivan Custom", "button", 0.0f, false, "minivan2"});
        items.push_back({"Paradise", "button", 0.0f, false, "paradise"});
        items.push_back({"Pony", "button", 0.0f, false, "pony"});
        items.push_back({"Pony (Smoke On The Water)", "button", 0.0f, false, "pony2"});
        items.push_back({"Rumpo", "button", 0.0f, false, "rumpo"});
        items.push_back({"Rumpo (Deludamol)", "button", 0.0f, false, "rumpo2"});
        items.push_back({"Rumpo (Weazel News)", "button", 0.0f, false, "rumpo3"});
        items.push_back({"Speedo", "button", 0.0f, false, "speedo"});
        items.push_back({"Speedo (Clown)", "button", 0.0f, false, "speedo2"});
        items.push_back({"Speedo (Festival)", "button", 0.0f, false, "speedo4"});
        items.push_back({"Surfer", "button", 0.0f, false, "surfer"});
        items.push_back({"Surfer (Rusty)", "button", 0.0f, false, "surfer2"});
        items.push_back({"Taco Van", "button", 0.0f, false, "taco"});
        items.push_back({"Youga", "button", 0.0f, false, "youga"});
        items.push_back({"Youga Classic", "button", 0.0f, false, "youga2"});
        items.push_back({"Youga Classic 4x4", "button", 0.0f, false, "youga3"});
        items.push_back({"Youga Custom", "button", 0.0f, false, "youga4"});
    }
    else if (catName == "Trucks") {
        items.push_back({"Benson", "button", 0.0f, false, "benson"});
        items.push_back({"Biff", "button", 0.0f, false, "biff"});
        items.push_back({"Cerberus", "button", 0.0f, false, "cerberus"});
        items.push_back({"Cerberus Custom", "button", 0.0f, false, "cerberus2"});
        items.push_back({"Cerberus GTX", "button", 0.0f, false, "cerberus3"});
        items.push_back({"Hauler", "button", 0.0f, false, "hauler"});
        items.push_back({"Hauler Custom", "button", 0.0f, false, "hauler2"});
        items.push_back({"Mule", "button", 0.0f, false, "mule"});
        items.push_back({"Mule Custom", "button", 0.0f, false, "mule2"});
        items.push_back({"Mule (Snow)", "button", 0.0f, false, "mule3"});
        items.push_back({"Mule Biff", "button", 0.0f, false, "mule4"});
        items.push_back({"Packer", "button", 0.0f, false, "packer"});
        items.push_back({"Phantom", "button", 0.0f, false, "phantom"});
        items.push_back({"Phantom Custom", "button", 0.0f, false, "phantom2"});
        items.push_back({"Phantom Wedge", "button", 0.0f, false, "phantom3"});
        items.push_back({"Pounder", "button", 0.0f, false, "pounder"});
        items.push_back({"Pounder Custom", "button", 0.0f, false, "pounder2"});
        items.push_back({"Stockade", "button", 0.0f, false, "stockade"});
        items.push_back({"Stockade (Snow)", "button", 0.0f, false, "stockade3"});
        items.push_back({"Terrorbyte", "button", 0.0f, false, "terbyte"});
        items.push_back({"Trashmaster", "button", 0.0f, false, "trash"});
        items.push_back({"Trashmaster (Rusty)", "button", 0.0f, false, "trash2"});
        items.push_back({"Wastelander", "button", 0.0f, false, "wastelander"});
    }
    else if (catName == "Emergency") {
        items.push_back({"Ambulance", "button", 0.0f, false, "ambulance"});
        items.push_back({"FBI", "button", 0.0f, false, "fbi"});
        items.push_back({"FBI Rancher", "button", 0.0f, false, "fbi2"});
        items.push_back({"Fire Truck", "button", 0.0f, false, "firetruk"});
        items.push_back({"Lifeguard", "button", 0.0f, false, "lguard"});
        items.push_back({"Park Ranger", "button", 0.0f, false, "pranger"});
        items.push_back({"Police Bike", "button", 0.0f, false, "policeb"});
        items.push_back({"Police Cruiser", "button", 0.0f, false, "police"});
        items.push_back({"Police Buffalo", "button", 0.0f, false, "police2"});
        items.push_back({"Police Interceptor", "button", 0.0f, false, "police3"});
        items.push_back({"Police Rancher", "button", 0.0f, false, "police4"});
        items.push_back({"Police Transporter", "button", 0.0f, false, "policet"});
        items.push_back({"Police Riot", "button", 0.0f, false, "riot"});
        items.push_back({"Police Predator", "button", 0.0f, false, "riot2"});
        items.push_back({"Prison Bus", "button", 0.0f, false, "pbus"});
        items.push_back({"Prison Bus (Riot)", "button", 0.0f, false, "pbus2"});
        items.push_back({"Sheriff Cruiser", "button", 0.0f, false, "sheriff"});
        items.push_back({"Sheriff SUV", "button", 0.0f, false, "sheriff2"});
        items.push_back({"Unmarked Cruiser", "button", 0.0f, false, "police5"});
    }
    else if (catName == "Military") {
        items.push_back({"Barracks", "button", 0.0f, false, "barracks"});
        items.push_back({"Barracks (Semi)", "button", 0.0f, false, "barracks2"});
        items.push_back({"Barracks (Cargo)", "button", 0.0f, false, "barracks3"});
        items.push_back({"Crusader", "button", 0.0f, false, "crusader"});
        items.push_back({"Halftrack", "button", 0.0f, false, "halftrack"});
        items.push_back({"Khanjali", "button", 0.0f, false, "khanjali"});
        items.push_back({"Rhino Tank", "button", 0.0f, false, "rhino"});
        items.push_back({"Scarab", "button", 0.0f, false, "scarab"});
        items.push_back({"Scarab Custom", "button", 0.0f, false, "scarab2"});
        items.push_back({"Scarab GTX", "button", 0.0f, false, "scarab3"});
        items.push_back({"Thruster", "button", 0.0f, false, "thruster"});
        items.push_back({"Trailers (Small)", "button", 0.0f, false, "trailersmall2"});
    }
    else if (catName == "Motorcycles") {
        items.push_back({"Akuma", "button", 0.0f, false, "akuma"});
        items.push_back({"Avarus", "button", 0.0f, false, "avarus"});
        items.push_back({"Bagger", "button", 0.0f, false, "bagger"});
        items.push_back({"Bati 801", "button", 0.0f, false, "bati"});
        items.push_back({"Bati 801RR", "button", 0.0f, false, "bati2"});
        items.push_back({"BF400", "button", 0.0f, false, "bf400"});
        items.push_back({"Blazer (Street)", "button", 0.0f, false, "blazer6"});
        items.push_back({"Carbon RS", "button", 0.0f, false, "carbonrs"});
        items.push_back({"Chimera", "button", 0.0f, false, "chimera"});
        items.push_back({"Cliffhanger", "button", 0.0f, false, "cliffhanger"});
        items.push_back({"Daemon", "button", 0.0f, false, "daemon"});
        items.push_back({"Daemon Custom", "button", 0.0f, false, "daemon2"});
        items.push_back({"Defiler", "button", 0.0f, false, "defiler"});
        items.push_back({"Deathbike", "button", 0.0f, false, "deathbike"});
        items.push_back({"Deathbike Apocalypse", "button", 0.0f, false, "deathbike2"});
        items.push_back({"Deathbike Future Shock", "button", 0.0f, false, "deathbike3"});
        items.push_back({"Diabolus", "button", 0.0f, false, "diablous"});
        items.push_back({"Diabolus Custom", "button", 0.0f, false, "diablous2"});
        items.push_back({"Double T", "button", 0.0f, false, "double"});
        items.push_back({"Enduro", "button", 0.0f, false, "enduro"});
        items.push_back({"Esskey", "button", 0.0f, false, "esskey"});
        items.push_back({"Faggio", "button", 0.0f, false, "faggio2"});
        items.push_back({"Faggio Mod", "button", 0.0f, false, "faggio3"});
        items.push_back({"Faggio Sport", "button", 0.0f, false, "faggio"});
        items.push_back({"Felon GT", "button", 0.0f, false, "felon2"});
        items.push_back({"Fcr 1000", "button", 0.0f, false, "fcr"});
        items.push_back({"Fcr 1000 Custom", "button", 0.0f, false, "fcr2"});
        items.push_back({"Gargoyle", "button", 0.0f, false, "gargoyle"});
        items.push_back({"Hakuchou", "button", 0.0f, false, "hakuchou"});
        items.push_back({"Hakuchou Drag", "button", 0.0f, false, "hakuchou2"});
        items.push_back({"Hexer", "button", 0.0f, false, "hexer"});
        items.push_back({"Innovation", "button", 0.0f, false, "innovation"});
        items.push_back({"Lectro", "button", 0.0f, false, "lectro"});
        items.push_back({"Manchez", "button", 0.0f, false, "manchez"});
        items.push_back({"Manchez Scout", "button", 0.0f, false, "manchez2"});
        items.push_back({"Manchez Scout (Cafe)", "button", 0.0f, false, "manchez3"});
        items.push_back({"Nemesis", "button", 0.0f, false, "nemesis"});
        items.push_back({"Nightblade", "button", 0.0f, false, "nightblade"});
        items.push_back({"Oppressor", "button", 0.0f, false, "oppressor"});
        items.push_back({"Oppressor Mk II", "button", 0.0f, false, "oppressor2"});
        items.push_back({"PCJ 600", "button", 0.0f, false, "pcj"});
        items.push_back({"Rat Bike", "button", 0.0f, false, "ratbike"});
        items.push_back({"Ruffian", "button", 0.0f, false, "ruffian"});
        items.push_back({"Sanchez", "button", 0.0f, false, "sanchez"});
        items.push_back({"Sanchez (Livery)", "button", 0.0f, false, "sanchez2"});
        items.push_back({"Sanctus", "button", 0.0f, false, "sanctus"});
        items.push_back({"Shotaro", "button", 0.0f, false, "shotaro"});
        items.push_back({"Sovereign", "button", 0.0f, false, "sovereign"});
        items.push_back({"Stryder", "button", 0.0f, false, "stryder"});
        items.push_back({"Thrust", "button", 0.0f, false, "thrust"});
        items.push_back({"Vader", "button", 0.0f, false, "vader"});
        items.push_back({"Vindicator", "button", 0.0f, false, "vindicator"});
        items.push_back({"Vortex", "button", 0.0f, false, "vortex"});
        items.push_back({"Wolfsbane", "button", 0.0f, false, "wolfsbane"});
        items.push_back({"Zombie Bobber", "button", 0.0f, false, "zombiea"});
        items.push_back({"Zombie Chopper", "button", 0.0f, false, "zombieb"});
    }
    else if (catName == "Planes") {
        items.push_back({"Alphajet", "button", 0.0f, false, "alphajet"});
        items.push_back({"Avenger", "button", 0.0f, false, "avenger"});
        items.push_back({"Avenger (Missile)", "button", 0.0f, false, "avenger2"});
        items.push_back({"Besra", "button", 0.0f, false, "besra"});
        items.push_back({"Blimp", "button", 0.0f, false, "blimp"});
        items.push_back({"Blimp (Atomic)", "button", 0.0f, false, "blimp2"});
        items.push_back({"Blimp (Xero)", "button", 0.0f, false, "blimp3"});
        items.push_back({"Bombushka", "button", 0.0f, false, "bombushka"});
        items.push_back({"Cargobob", "button", 0.0f, false, "cargobob"});
        items.push_back({"Cargobob (Jetsam)", "button", 0.0f, false, "cargobob2"});
        items.push_back({"Cargobob (TPE)", "button", 0.0f, false, "cargobob3"});
        items.push_back({"Cargobob (Dropzone)", "button", 0.0f, false, "cargobob4"});
        items.push_back({"Cuban 800", "button", 0.0f, false, "cuban800"});
        items.push_back({"Dodo", "button", 0.0f, false, "dodo"});
        items.push_back({"Duster", "button", 0.0f, false, "duster"});
        items.push_back({"Howard NX-25", "button", 0.0f, false, "howard"});
        items.push_back({"Hydra", "button", 0.0f, false, "hydra"});
        items.push_back({"Jet", "button", 0.0f, false, "jet"});
        items.push_back({"Lazer", "button", 0.0f, false, "lazer"});
        items.push_back({"Luxor", "button", 0.0f, false, "luxor"});
        items.push_back({"Luxor Deluxe", "button", 0.0f, false, "luxor2"});
        items.push_back({"Mammatus", "button", 0.0f, false, "mammatus"});
        items.push_back({"Microlight", "button", 0.0f, false, "microlight"});
        items.push_back({"Miljet", "button", 0.0f, false, "miljet"});
        items.push_back({"Mogul", "button", 0.0f, false, "mogul"});
        items.push_back({"Molotok", "button", 0.0f, false, "molotok"});
        items.push_back({"Nimbus", "button", 0.0f, false, "nimbus"});
        items.push_back({"Nokota", "button", 0.0f, false, "nokota"});
        items.push_back({"Pyro", "button", 0.0f, false, "pyro"});
        items.push_back({"Rogue", "button", 0.0f, false, "rogue"});
        items.push_back({"Seabreeze", "button", 0.0f, false, "seabreeze"});
        items.push_back({"Shamal", "button", 0.0f, false, "shamal"});
        items.push_back({"Starling", "button", 0.0f, false, "starling"});
        items.push_back({"Stunt", "button", 0.0f, false, "stunt"});
        items.push_back({"Titan", "button", 0.0f, false, "titan"});
        items.push_back({"Tula", "button", 0.0f, false, "tula"});
        items.push_back({"Velum", "button", 0.0f, false, "velum"});
        items.push_back({"Velum (5-Seater)", "button", 0.0f, false, "velum2"});
        items.push_back({"Vestra", "button", 0.0f, false, "vestra"});
        items.push_back({"Volatol", "button", 0.0f, false, "volatol"});
    }
    else if (catName == "Openwheel") {
        items.push_back({"Formula", "button", 0.0f, false, "formula"});
        items.push_back({"Formula 2", "button", 0.0f, false, "formula2"});
        items.push_back({"Open Wheel 1", "button", 0.0f, false, "openwheel1"});
        items.push_back({"Open Wheel 2", "button", 0.0f, false, "openwheel2"});
    }
    else if (catName == "Helicopters") {
        items.push_back({"Akula", "button", 0.0f, false, "akula"});
        items.push_back({"Annihilator", "button", 0.0f, false, "annihilator"});
        items.push_back({"Annihilator Stealth", "button", 0.0f, false, "annihilator2"});
        items.push_back({"Buzzard", "button", 0.0f, false, "buzzard"});
        items.push_back({"Buzzard Attack", "button", 0.0f, false, "buzzard2"});
        items.push_back({"Cargobob", "button", 0.0f, false, "cargobob"});
        items.push_back({"Cargobob (Jetsam)", "button", 0.0f, false, "cargobob2"});
        items.push_back({"Cargobob (TPE)", "button", 0.0f, false, "cargobob3"});
        items.push_back({"Cargobob (Dropzone)", "button", 0.0f, false, "cargobob4"});
        items.push_back({"Frogger", "button", 0.0f, false, "frogger"});
        items.push_back({"Frogger (TPE)", "button", 0.0f, false, "frogger2"});
        items.push_back({"Havok", "button", 0.0f, false, "havok"});
        items.push_back({"Hunter", "button", 0.0f, false, "hunter"});
        items.push_back({"Maverick", "button", 0.0f, false, "maverick"});
        items.push_back({"Maverick (Police)", "button", 0.0f, false, "maverick2"});
        items.push_back({"Savage", "button", 0.0f, false, "savage"});
        items.push_back({"Sea Sparrow", "button", 0.0f, false, "seasparrow"});
        items.push_back({"Sea Sparrow (DLC)", "button", 0.0f, false, "seasparrow2"});
        items.push_back({"Sea Sparrow (Weaponized)", "button", 0.0f, false, "seasparrow3"});
        items.push_back({"Skylift", "button", 0.0f, false, "skylift"});
        items.push_back({"SuperVolcano", "button", 0.0f, false, "supervolcano"});
        items.push_back({"Swift", "button", 0.0f, false, "swift"});
        items.push_back({"Swift Deluxe", "button", 0.0f, false, "swift2"});
        items.push_back({"Valkyrie", "button", 0.0f, false, "valkyrie"});
        items.push_back({"Valkyrie MOD.0", "button", 0.0f, false, "valkyrie2"});
        items.push_back({"Volatus", "button", 0.0f, false, "volatus"});
    }
    else if (catName == "Boats") {
        items.push_back({"Dinghy", "button", 0.0f, false, "dinghy"});
        items.push_back({"Dinghy (2-Seat)", "button", 0.0f, false, "dinghy2"});
        items.push_back({"Dinghy (Yacht)", "button", 0.0f, false, "dinghy3"});
        items.push_back({"Dinghy (Weaponized)", "button", 0.0f, false, "dinghy4"});
        items.push_back({"Jetmax", "button", 0.0f, false, "jetmax"});
        items.push_back({"Kraken", "button", 0.0f, false, "kraken"});
        items.push_back({"Longfin", "button", 0.0f, false, "longfin"});
        items.push_back({"Marquis", "button", 0.0f, false, "marquis"});
        items.push_back({"Patrol Boat", "button", 0.0f, false, "patrolboat"});
        items.push_back({"Seashark", "button", 0.0f, false, "seashark"});
        items.push_back({"Seashark (Lifeguard)", "button", 0.0f, false, "seashark2"});
        items.push_back({"Seashark (Yacht)", "button", 0.0f, false, "seashark3"});
        items.push_back({"Speeder", "button", 0.0f, false, "speeder"});
        items.push_back({"Speeder (Yacht)", "button", 0.0f, false, "speeder2"});
        items.push_back({"Squalo", "button", 0.0f, false, "squalo"});
        items.push_back({"Submersible", "button", 0.0f, false, "submersible"});
        items.push_back({"Submersible (Kraken)", "button", 0.0f, false, "submersible2"});
        items.push_back({"Suntrap", "button", 0.0f, false, "suntrap"});
        items.push_back({"Toro", "button", 0.0f, false, "toro"});
        items.push_back({"Toro (Yacht)", "button", 0.0f, false, "toro2"});
        items.push_back({"Tropic", "button", 0.0f, false, "tropic"});
        items.push_back({"Tropic (Yacht)", "button", 0.0f, false, "tropic2"});
        items.push_back({"Tug", "button", 0.0f, false, "tug"});
    }
    else if (catName == "Industrial") {
        items.push_back({"Bulldozer", "button", 0.0f, false, "bulldozer"});
        items.push_back({"Cutter", "button", 0.0f, false, "cutter"});
        items.push_back({"Dump", "button", 0.0f, false, "dump"});
        items.push_back({"Flatbed", "button", 0.0f, false, "flatbed"});
        items.push_back({"Flatbed (Soda)", "button", 0.0f, false, "flatbed2"});
        items.push_back({"Flatbed (Trailer)", "button", 0.0f, false, "flatbed3"});
        items.push_back({"Guardian", "button", 0.0f, false, "guardian"});
        items.push_back({"Handler", "button", 0.0f, false, "handler"});
        items.push_back({"Mixer", "button", 0.0f, false, "mixer"});
        items.push_back({"Mixer (2-Axle)", "button", 0.0f, false, "mixer2"});
        items.push_back({"Rubble", "button", 0.0f, false, "rubble"});
        items.push_back({"Tiptruck", "button", 0.0f, false, "tiptruck"});
        items.push_back({"Tiptruck (2-Axle)", "button", 0.0f, false, "tiptruck2"});
    }
    else if (catName == "Utility") {
        items.push_back({"Airtug", "button", 0.0f, false, "airtug"});
        items.push_back({"Caddy", "button", 0.0f, false, "caddy"});
        items.push_back({"Caddy (Golf)", "button", 0.0f, false, "caddy2"});
        items.push_back({"Caddy (Bunker)", "button", 0.0f, false, "caddy3"});
        items.push_back({"Docktug", "button", 0.0f, false, "docktug"});
        items.push_back({"Forklift", "button", 0.0f, false, "forklift"});
        items.push_back({"Mower", "button", 0.0f, false, "mower"});
        items.push_back({"Ripley", "button", 0.0f, false, "ripley"});
        items.push_back({"Sadler", "button", 0.0f, false, "sadler"});
        items.push_back({"Sadler (Snow)", "button", 0.0f, false, "sadler2"});
        items.push_back({"Scrap", "button", 0.0f, false, "scrap"});
        items.push_back({"Tow Truck", "button", 0.0f, false, "towtruck"});
        items.push_back({"Tow Truck (Small)", "button", 0.0f, false, "towtruck2"});
        items.push_back({"Tractor", "button", 0.0f, false, "tractor"});
        items.push_back({"Tractor (Rusty)", "button", 0.0f, false, "tractor2"});
        items.push_back({"Tractor (Festival)", "button", 0.0f, false, "tractor3"});
        items.push_back({"UtilliTruck", "button", 0.0f, false, "utillitruck"});
        items.push_back({"UtilliTruck (Dock)", "button", 0.0f, false, "utillitruck2"});
        items.push_back({"UtilliTruck (Flatbed)", "button", 0.0f, false, "utillitruck3"});
    }
    else if (catName == "Service") {
        items.push_back({"Airport Bus", "button", 0.0f, false, "airbus"});
        items.push_back({"Bus", "button", 0.0f, false, "bus"});
        items.push_back({"Dashound", "button", 0.0f, false, "coach"});
        items.push_back({"Rental Shuttle Bus", "button", 0.0f, false, "rentalbus"});
        items.push_back({"Taxi", "button", 0.0f, false, "taxi"});
        items.push_back({"Taxi (Lost)", "button", 0.0f, false, "taxi2"});
        items.push_back({"Tourbus", "button", 0.0f, false, "tourbus"});
        items.push_back({"Trashmaster", "button", 0.0f, false, "trash"});
        items.push_back({"Trashmaster (Rusty)", "button", 0.0f, false, "trash2"});
    }
    else if (catName == "Commercial") {
        items.push_back({"Benson", "button", 0.0f, false, "benson"});
        items.push_back({"Biff", "button", 0.0f, false, "biff"});
        items.push_back({"Cerberus", "button", 0.0f, false, "cerberus"});
        items.push_back({"Cerberus Custom", "button", 0.0f, false, "cerberus2"});
        items.push_back({"Cerberus GTX", "button", 0.0f, false, "cerberus3"});
        items.push_back({"Hauler", "button", 0.0f, false, "hauler"});
        items.push_back({"Hauler Custom", "button", 0.0f, false, "hauler2"});
        items.push_back({"Mule", "button", 0.0f, false, "mule"});
        items.push_back({"Mule Custom", "button", 0.0f, false, "mule2"});
        items.push_back({"Mule (Snow)", "button", 0.0f, false, "mule3"});
        items.push_back({"Mule Biff", "button", 0.0f, false, "mule4"});
        items.push_back({"Packer", "button", 0.0f, false, "packer"});
        items.push_back({"Phantom", "button", 0.0f, false, "phantom"});
        items.push_back({"Phantom Custom", "button", 0.0f, false, "phantom2"});
        items.push_back({"Phantom Wedge", "button", 0.0f, false, "phantom3"});
        items.push_back({"Pounder", "button", 0.0f, false, "pounder"});
        items.push_back({"Pounder Custom", "button", 0.0f, false, "pounder2"});
        items.push_back({"Stockade", "button", 0.0f, false, "stockade"});
        items.push_back({"Stockade (Snow)", "button", 0.0f, false, "stockade3"});
        items.push_back({"Terrorbyte", "button", 0.0f, false, "terbyte"});
    }
    else if (catName == "Sportsclassics") {
        items.push_back({"Casco", "button", 0.0f, false, "casco"});
        items.push_back({"Cheburek", "button", 0.0f, false, "cheburek"});
        items.push_back({"Cheetah Classic", "button", 0.0f, false, "cheetah2"});
        items.push_back({"Coquette Classic", "button", 0.0f, false, "coquette2"});
        items.push_back({"Deluxo", "button", 0.0f, false, "deluxo"});
        items.push_back({"Dynasty", "button", 0.0f, false, "dynasty"});
        items.push_back({"Fagaloa", "button", 0.0f, false, "fagaloa"});
        items.push_back({"GT500", "button", 0.0f, false, "gt500"});
        items.push_back({"Infernus Classic", "button", 0.0f, false, "infernus2"});
        items.push_back({"JB 700", "button", 0.0f, false, "jb700"});
        items.push_back({"JB 700W", "button", 0.0f, false, "jb7002"});
        items.push_back({"Mamba", "button", 0.0f, false, "mamba"});
        items.push_back({"Manana", "button", 0.0f, false, "manana"});
        items.push_back({"Manana Custom", "button", 0.0f, false, "manana2"});
        items.push_back({"Michelli", "button", 0.0f, false, "michelli"});
        items.push_back({"Monroe", "button", 0.0f, false, "monroe"});
        items.push_back({"Nebula", "button", 0.0f, false, "nebula"});
        items.push_back({"Peyote", "button", 0.0f, false, "peyote"});
        items.push_back({"Peyote (Gasser)", "button", 0.0f, false, "peyote2"});
        items.push_back({"Peyote (Baja)", "button", 0.0f, false, "peyote3"});
        items.push_back({"Pigalle", "button", 0.0f, false, "pigalle"});
        items.push_back({"Rapid GT Classic", "button", 0.0f, false, "rapidgt3"});
        items.push_back({"Retinue", "button", 0.0f, false, "retinue"});
        items.push_back({"Retinue MK2", "button", 0.0f, false, "retinue2"});
        items.push_back({"Savestra", "button", 0.0f, false, "savestra"});
        items.push_back({"Stinger", "button", 0.0f, false, "stinger"});
        items.push_back({"Stinger GT", "button", 0.0f, false, "stingergt"});
        items.push_back({"Stirling GT", "button", 0.0f, false, "feltzer3"});
        items.push_back({"Swinger", "button", 0.0f, false, "swinger"});
        items.push_back({"Torero", "button", 0.0f, false, "torero"});
        items.push_back({"Tornado", "button", 0.0f, false, "tornado"});
        items.push_back({"Tornado (Convertible)", "button", 0.0f, false, "tornado2"});
        items.push_back({"Tornado (Rusty)", "button", 0.0f, false, "tornado3"});
        items.push_back({"Tornado (Mariachi)", "button", 0.0f, false, "tornado4"});
        items.push_back({"Tornado (Rat Rod)", "button", 0.0f, false, "tornado5"});
        items.push_back({"Tornado (Gasser)", "button", 0.0f, false, "tornado6"});
        items.push_back({"Turismo Classic", "button", 0.0f, false, "turismo2"});
        items.push_back({"Viseris", "button", 0.0f, false, "viseris"});
        items.push_back({"Z190", "button", 0.0f, false, "z190"});
        items.push_back({"Zion Classic", "button", 0.0f, false, "zion3"});
        items.push_back({"Z-Type", "button", 0.0f, false, "ztype"});
    }
    return items;
}

static void UpdateVehicleList() {
    static uint64_t lastUpdate = 0;
    uint64_t now = GetTickCount64();
    if (now - lastUpdate < 150) return;
    lastUpdate = now;

    SRWLockGuard lock(&g_VehicleListLock);

    if (g_MenuData.size() <= 2) return;

    // --- Dynamic Cleanup: Detect if any selected vehicle is gone ---
    for (auto it = g_VehicleListToggles.begin(); it != g_VehicleListToggles.end(); ) {
        if (it->second) {
            bool found = false;
            for (const auto& v : g_VehicleList) {
                if (v.vehicle == it->first) {
                    found = true;
                    break;
                }
            }
            if (!found) {
                it = g_VehicleListToggles.erase(it);
                continue;
            }
        }
        ++it;
    }

    // Populate List Tab (Category 0)
    auto& listItems = g_MenuData[2].categories[0].items;
    if (listItems.size() > 4) {
        listItems.erase(listItems.begin() + 4, listItems.end());
    } else if (listItems.size() < 4) {
        listItems = {
            {"Select All Vehicles", "button"},
            {"Un-Select All Vehicles", "button"},
            {"Clear Vehicle Selection", "button"},
            {"Vehicle List", "separator"}
        };
    }

    std::vector<VehicleListData> sortedList = g_VehicleList;
    std::sort(sortedList.begin(), sortedList.end(), [](const VehicleListData& a, const VehicleListData& b) {
        int cmp = std::strcmp(a.name, b.name);
        if (cmp != 0) return cmp < 0;
        return a.vehicle < b.vehicle;
    });

    for (const auto& v : sortedList) {
        std::string label = v.name;
        label += " - [" + std::to_string((int)v.distance) + "m]";
        
        SouthItem item;
        item.label = label;
        item.type = "check";
        item.checked = g_VehicleListToggles[v.vehicle];
        item.subVal = std::to_string(v.vehicle);
        listItems.push_back(item);
    }

    // Populate Options Tab Spawner items (Category 1) dynamically
    auto& optionsItems = g_MenuData[2].categories[1].items;
    if (optionsItems.size() > 11) {
        optionsItems.erase(optionsItems.begin() + 11, optionsItems.end());
    } else if (optionsItems.size() < 11) {
        optionsItems = {
            {"Teleport to Vehicle", "button"},
            {"Fix Vehicle", "button"},
            {"Explode Vehicle", "button"},
            {"Explode Vehicle Tires", "button"},
            {"Launch Vehicle", "button"},
            {"Copy Vehicle", "button"},
            {"Destroy Vehicle", "button"},
            {"Vehicle Spawner", "separator"},
            {"Type Model", "button"},
            {"Spawn Category", "scroll", 0.0f, false, "Super"},
            {"Spawn into Vehicle", "check"}
        };
    }

    std::string curCat = "Super";
    if (optionsItems.size() >= 11) {
        curCat = optionsItems[9].subVal;
    }

    std::vector<SouthItem> dynamicVehs = GetVehiclesForCategory(curCat);
    for (const auto& item : dynamicVehs) {
        optionsItems.push_back(item);
    }
}


static void UpdateServerPlayerList() {
    static uint64_t lastUpdate = 0;
    uint64_t now = GetTickCount64();
    if (now - lastUpdate < 150) return; // Fast responsive updates
    lastUpdate = now;

    SRWLockGuard lock(&g_PlayerListLock);

    // Check if MenuData is valid and has Online Options submenu (index 1)
    if (g_MenuData.size() <= 1) return;

    // --- Dynamic Cleanup: Detect if any selected player is gone from g_PlayerList ---
    bool selectedPlayerGone = false;
    for (auto it = g_PlayerListToggles.begin(); it != g_PlayerListToggles.end(); ) {
        if (it->second) {
            bool found = false;
            for (const auto& p : g_PlayerList) {
                if (std::strcmp(p.name, it->first.c_str()) == 0) {
                    found = true;
                    break;
                }
            }
            if (!found) {
                selectedPlayerGone = true;
                it = g_PlayerListToggles.erase(it);
                continue;
            }
        }
        ++it;
    }

    // If a selected player disappeared, auto-disable all Trolls & Risky toggles
    if (selectedPlayerGone) {
        if (g_MenuData[1].categories.size() > 2) {
            // Reset Trolls category (index 1)
            for (auto& item : g_MenuData[1].categories[1].items) {
                if (item.type == "check") {
                    if (item.checked) {
                        item.checked = false;
                        if (item.label == "Spectate Player") {
                            LuaCore::spectateActive = false;
                            LuaCore::spectateTargetSid = -1;
                        }
                    }
                }
            }
            // Reset Risky category (index 2)
            for (auto& item : g_MenuData[1].categories[2].items) {
                if (item.type == "check") {
                    item.checked = false;
                }
            }
        }
    }

    // Keep the first 4 items of Online Options -> List
    auto& listItems = g_MenuData[1].categories[0].items;
    if (listItems.size() > 4) {
        listItems.erase(listItems.begin() + 4, listItems.end());
    } else if (listItems.size() < 4) {
        listItems = {
            {"Select Everyone", "button"},
            {"Un-Select Everyone", "button"},
            {"Clear Selection", "button"},
            {"Player List", "separator"}
        };
    }

    // Populate using our highly reliable g_PlayerList from the ESP engine!
    std::vector<PlayerListData> sortedList = g_PlayerList;
    std::sort(sortedList.begin(), sortedList.end(), [](const PlayerListData& a, const PlayerListData& b) {
        return std::strcmp(a.name, b.name) < 0;
    });

    for (const auto& player : sortedList) {
        std::string label = player.name;
        label += " - [" + std::to_string((int)player.distance) + "m]";
        
        SouthItem item;
        item.label = label;
        item.type = "check";
        item.checked = g_PlayerListToggles[player.name];
        item.subVal = player.name; // Keep original player name for mapping
        listItems.push_back(item);
    }
}

std::vector<SouthSubMenu> g_MenuData = {
    {"Self Options", {
        {"Player", {
            {"Revive", "button"}, 
            {"Health", "slider", 100}, 
            {"Armour", "slider", 100}, 
            {"Refill Health", "button"},
            {"Refill Armour", "button"},
            {"Godmode", "check", 0, false, "Safe"},
            {"Invisibility", "check"},
            {"Movement", "separator"},
            {"Noclip", "scroll-check", 0, false, "Default"},
            {"Freecam", "check"},
            {"Fast Run", "check"},
            {"Fast Punch", "check"},
            {"Super Jump", "check"},
            {"Invincibility", "check"}
        }},
        {"Miscellaneous", {
            {"Self Crash", "button"},
            {"Clear Screen Effects", "button"},
            {"Suicide", "button"},
            {"Force Ragdoll", "button"},
            {"Remove Crutch", "button"},
            {"Clear Tasks", "scroll", 0, false, "Primary"},
            {"Toggles", "separator"},
            {"No Ragdoll", "check"},
            {"Anti Freeze", "check"},
            {"Anti Headshot", "check"},
            {"Anti Spectate", "check"},
            {"Force Third Person", "check"},
            {"Force Driveby", "check"},
            {"Infinity Stamina", "check"},
            {"Tiny Ped", "check"},
            {"Super Punch", "check"},
            {"Fake Options", "separator"},
            {"Fake Bandage", "button"},
            {"Fake Armour", "button"},
            {"Fake Death", "button"},
            {"txAdmin Feature", "separator"},
            {"Give txAdmin", "scroll", 0, false, "Only UI"},
            {"txAdmin Noclip", "check"},
            {"Disable All txAdmin", "check"},
            {"Disable txAdmin Teleport", "check"},
            {"Disable txAdmin Freeze", "check"},
            {"Auto Hunger", "check"},
            {"Auto Thirst", "check"},
            {"Semi Godmode", "check"},
            {"Pickup/Throw Vehicles", "check"}
        }},
        {"Wardrobe", {
            {"Outfit", "button"},
            {"Ped Options", "separator"},
            {"Freemode", "scroll", 0, false, "Freemode Male"},
            {"Peds", "scroll", 0, false, "Michael"},
            {"Animal Peds", "scroll", 0, false, "Boar"},
            {"Solo Session", "check"}
        }}
    }},
    {"Online Options", {
        {"List", {
            {"Select Everyone", "button"}, 
            {"Un-Select Everyone", "button"},
            {"Clear Selection", "button"},
            {"Player List", "separator"},
            {"player name", "check"}
        }},
        {"Trolls", {
            {"Crash Player", "scroll", 0, false, "StateBag"},
            {"Teleport To Player", "button"},
            {"Spectate Player", "check"},
            {"Copy Appearance", "button"},
            {"Kill Player", "button"},
            {"Launch Player", "button"},
            {"Launch To Sky (Vehicle)", "button"},
            {"Fall To Void (Vehicle)", "button"},
            {"Explode Player", "button"},
            {"Bring Player", "button"},
            {"Clone Player", "button"},
            {"Attack Clone Player", "button"},
            {"Heal Player", "button"},
            {"Give Armour", "button"},
            {"Give Weapons", "button"},
            {"Ragdoll Player", "button"},
            {"Kick From Vehicle", "button"},
            {"Kick From Vehicle (Loop)", "check"},
            {"Ram With Vehicle", "button"},
            {"Cage Player", "button"},
            {"Flash Bang", "button"},
            {"Tazer Player", "button"},
            {"Rape Player", "button"},
            {"Block Jumping", "button"},
            {"Send to Void", "button"},
            {"Freeze Player", "check"},
            {"Attach Truck", "button"},
            {"Attach Object", "button"},
            {"Glitch Player", "button"}
        }},
        {"Risky", {
            {"Ban Player", "button"},
            {"Kill Player", "button"},
            {"Burn Player", "button"},
            {"Clone Player", "button"},
            {"Attack Clone Player", "button"},
            {"Steal Inventory", "button"},
            {"Punch Player", "button"},
            {"Punch Player (Loop)", "check"},
        }}
    }},
    {"Vehicle Options", {
        {"List", {
            {"vehicle list", "check"}
        }},
        {"Options", {
            {"Teleport to Vehicle", "button"},
            {"Fix Vehicle", "button"},
            {"Explode Vehicle", "button"},
            {"Explode Vehicle Tires", "button"},
            {"Launch Vehicle", "button"},
            {"Copy Vehicle", "button"},
            {"Destroy Vehicle", "button"},
            {"Vehicle Spawner", "separator"},
            {"Type Model", "button"},
            {"Spawn Category", "scroll", 0, false, "Super"},
            {"Spawn into Vehicle", "check"},
        }}
    }},
    {"Weapon Options", {
        {"Combat", {
            {"Infinite Ammo", "check"}, 
            {"Rapid Fire", "check"},
            {"No Recoil", "check"}
        }},
        {"Options", {
            {"Give All Weapons", "button"}
        }}
    }},
    {"Exploits", {
        {"South", {
            {"Crash All Players", "button"},
            {"Crash Nearby Players", "scroll", 0, false, "StateBag"}, 
            {"Ban All Players", "button"},
            {"Kill All Players", "button"},
            {"Ragdoll All Players", "button"},
            {"Explode All Players", "button"},
            {"Explode All Vehicles", "button"},
            {"Launch All Players", "button"},
            {"Launch All Vehicles", "button"},
            {"Steal All Vehicles", "button"},
            {"Kick All Players From Vehicle", "button"},
            {"Ram All Players With Vehicle", "button"},
            {"Heal All Players", "button"},
            {"Give Armour to All Players", "button"},
            {"Give Weapons to All Players", "button"},
            {"Freeze All Players", "button"},
            {"Clone All Players", "button"},
            {"Talk to All Players", "check"},
            {"Flash Bang All Players", "button"},
            {"Drop Planes On All Players", "button"},
            {"Fuck Map", "button"},
            {"White Screen All", "check"},
            {"Fire Screen All", "check"},
            {"Sound Spam All", "check"},
            {"Tazer All Players", "button"},
            {"Rape All Players", "button"},
            {"Block Jumping All Players", "button"},
            {"Send to Void All Players", "button"},
            {"Attach Truck All Players", "button"}
        }}
    }},
    {"Triggers", {
        {"New Triggers", {
            {"Spawn Money", "button"}, 
            {"Spawn All Item", "button"},
            {"Money Wash (100k)", "button"},
            {"Printer Money (10k)", "button"},
            {"Drug Reward (Money)", "button"},
            {"Gain Item (Cash)", "button"},
            {"Lashes Reward", "button"},
            {"Nails Reward", "button"},
            {"Caps Reward", "button"},
            {"Wigs Reward", "button"},
            {"Icebox Reward", "button"},
            {"Sneaker Reward", "button"},
            {"Handbag Reward", "button"},
            {"Spoody Sim Swap", "button"},
            {"Spoody Clone Card", "button"},
            {"Drug Receive", "button"},
            {"Set Chat Tag", "button"}
        }}
    }},
    {"Settings", {
        {"Menu", {
            {"Menu Key", "scroll", 0, false, "DELETE"},
            {"Disable Keybind Overlay", "check"},
            {"Disable Sound", "check"},
            {"Unload Menu", "button"}
        }}
    }}
};

// Dynamically append missing Lua example labels from missing_labels.txt
// File is generated by the workspace helper script and should live next to the executable or in the DLL folder.
void InitializeDynamicMenu() {
    std::ifstream in("missing_labels.txt");
    if (!in) {
        std::string dllPath = GetDllFolderPath();
        if (!dllPath.empty()) {
            std::ifstream in2(dllPath + "missing_labels.txt");
            if (in2) in.swap(in2);
        }
    }
    if (in) {
        std::string line;
        // first line may be a count, skip
        if (std::getline(in, line)) {}
        std::vector<SouthItem> dynamicItems;
        while (std::getline(in, line)) {
            if (line.empty()) continue;
            SouthItem it;
            it.label = line;
            it.type = "button";
            dynamicItems.push_back(it);
        }
        if (!dynamicItems.empty()) {
            SouthSubMenu sub;
            sub.label = "Lua Examples";
            SouthCategory cat;
            cat.label = "Examples";
            cat.items = dynamicItems;
            sub.categories.push_back(cat);
            g_MenuData.push_back(sub);
        }
    }
}

void DrawFreecamOverlay() {
    if (!g_FreecamActive) return;

    ImGuiIO& io = ImGui::GetIO();
    ImVec2 screen = io.DisplaySize;
    float centerX = floorf(screen.x * 0.5f);
    float centerY = floorf(screen.y * 0.5f);
    ImDrawList* draw = ImGui::GetBackgroundDrawList();

    // 1. Crosshair (Text-based '+' with Outline)
    const char* crossStr = "+";
    ImFont* crossFont = io.Fonts->Fonts[1]; // Using the larger font for clarity
    float crossSize = 24.0f;
    ImVec2 tSize = crossFont->CalcTextSizeA(crossSize, FLT_MAX, 0.0f, crossStr);
    ImVec2 tPos = ImVec2(centerX - tSize.x * 0.5f, centerY - tSize.y * 0.5f);
    
    // Outline (4-way Black)
    draw->AddText(crossFont, crossSize, ImVec2(tPos.x - 1, tPos.y), IM_COL32(0, 0, 0, 255), crossStr);
    draw->AddText(crossFont, crossSize, ImVec2(tPos.x + 1, tPos.y), IM_COL32(0, 0, 0, 255), crossStr);
    draw->AddText(crossFont, crossSize, ImVec2(tPos.x, tPos.y - 1), IM_COL32(0, 0, 0, 255), crossStr);
    draw->AddText(crossFont, crossSize, ImVec2(tPos.x, tPos.y + 1), IM_COL32(0, 0, 0, 255), crossStr);
    
    // Main Crosshair (Red)
    draw->AddText(crossFont, crossSize, tPos, IM_COL32(255, 0, 0, 255), crossStr);

    // 2. Feature List (Centered)
    float startY = floorf(screen.y * 0.62f);
    float lineHeight = 28.0f; 

    for (int i = 0; i < g_NumFreecamFeatures; i++) {
        bool active = (i == g_FreecamFeatureIdx);
        ImU32 textCol = active ? IM_COL32(255, 255, 255, 255) : IM_COL32(180, 180, 185, 140);
        
        // Use 20px font for active, 18px for others
        ImFont* font = io.Fonts->Fonts[active ? 1 : 0];
        
        std::string label = g_FreecamFeatures[i];
        ImVec2 tSize = font->CalcTextSizeA(font->FontSize, FLT_MAX, 0.0f, label.c_str());
        
        // HORIZONTAL AND VERTICAL CENTERING
        float currentSlotY = startY + (i * lineHeight);
        ImVec2 tPos = ImVec2(floorf(centerX - tSize.x * 0.5f), floorf(currentSlotY - tSize.y * 0.5f));
        
        // Clean drop shadow
        draw->AddText(font, font->FontSize, ImVec2(tPos.x + 1, tPos.y + 1), IM_COL32(0, 0, 0, active ? 220 : 120), label.c_str());
        
        // Main Text
        draw->AddText(font, font->FontSize, tPos, textCol, label.c_str());
    }
}

void DrawSouthMenu() {

    ImGuiIO& io = ImGui::GetIO();
    
    // Tab Change Detection
    static int lastSub = -1;
    static int lastCat = -1;
    if (g_ShowSouthMenu) {
        if (g_ActiveSub == 1) {
            UpdateServerPlayerList();
        }
        if (g_ActiveSub == 2) {
            UpdateVehicleList();
        }
        if (lastSub != g_ActiveSub || lastCat != g_ActiveCat) {
            if (lastSub != -1) PlayClickSound(); // Don't play on first open
            lastSub = g_ActiveSub;
            lastCat = g_ActiveCat;
        }
    } else {
        lastSub = -1; lastCat = -1;
    }

    g_SouthAlpha = ImLerp(g_SouthAlpha, g_ShowSouthMenu ? 1.0f : 0.0f, io.DeltaTime * 12.0f);
    ImDrawList* drawList = ImGui::GetBackgroundDrawList();
    
    float menuW = 365.0f; 
    float menuX = 50.0f; 
    float menuY = 150.0f;
    
    ImU32 bgCol = IM_COL32(5, 5, 5, (int)(180 * g_SouthAlpha));
    ImU32 accentCol = IM_COL32(180, 20, 25, (int)(255 * g_SouthAlpha));
    ImU32 textCol = IM_COL32(255, 255, 255, (int)(255 * g_SouthAlpha));
    ImU32 textDimCol = IM_COL32(230, 230, 230, (int)(255 * g_SouthAlpha));
    
    // 1. Banner (South.gg Premium Lua Menu Banner)
    float bannerH = 100.0f; 
    ImVec2 uv0 = ImVec2(0.0f, 0.0f);
    ImVec2 uv1 = ImVec2(1.0f, 1.0f);
    if (tex_banner) {
        ImVec2 texSize = Icons::GetTextureSize(tex_banner);
        if (texSize.x > 0 && texSize.y > 0) {
            // Full uncropped South.gg banner display matching native aspect ratio
            bannerH = menuW * (texSize.y / texSize.x);
        }
    }
    ImVec2 bMin = ImVec2(menuX, menuY);
    ImVec2 bMax = ImVec2(menuX + menuW, menuY + bannerH);
    drawList->AddRectFilled(bMin, bMax, IM_COL32(0, 0, 0, (int)(255 * g_SouthAlpha)), 4.0f, ImDrawCornerFlags_Top);
    
    if (tex_banner) {
        drawList->AddImageRounded(tex_banner, bMin, bMax, uv0, uv1, IM_COL32(255, 255, 255, (int)(255 * g_SouthAlpha)), 4.0f, ImDrawCornerFlags_Top);
    }

    // 3. Header Tab (Now matches image for Home/SubMenu)
    float catH = 34.0f;
    float currentY = bMax.y;
    
    // Draw "Main Menu" tab or Category Tabs
    if (g_Depth == 0) {
        // Home Header Bar Background
        ImVec2 cMin = ImVec2(menuX, currentY);
        ImVec2 cMax = ImVec2(menuX + menuW, cMin.y + catH);
        drawList->AddRectFilled(cMin, cMax, IM_COL32(0, 0, 0, (int)(255 * g_SouthAlpha)));

        // Home Header Tab (Matches Image)
        ImVec2 shMin = ImVec2(menuX + 5, currentY + 3);
        std::string headerLabel = "Main Menu";
        float tabW = ImGui::CalcTextSize(headerLabel.c_str()).x + 30.0f;
        ImVec2 shMax = ImVec2(shMin.x + tabW, currentY + catH);
        
        drawList->AddRectFilledMultiColor(shMin, shMax,
            IM_COL32(230, 20, 30, (int)(255 * g_SouthAlpha)),
            IM_COL32(230, 20, 30, (int)(255 * g_SouthAlpha)),
            IM_COL32(0, 0, 0, (int)(255 * g_SouthAlpha)),
            IM_COL32(0, 0, 0, (int)(255 * g_SouthAlpha)));

        // Mask corners to make it rounded
        ImU32 tabMaskCol = IM_COL32(0, 0, 0, (int)(255 * g_SouthAlpha));
        drawList->PathLineTo(shMin);
        drawList->PathLineTo(ImVec2(shMin.x + 6.0f, shMin.y));
        drawList->PathArcTo(ImVec2(shMin.x + 6.0f, shMin.y + 6.0f), 6.0f, 1.5f * 3.14159265f, 1.0f * 3.14159265f);
        drawList->PathFillConvex(tabMaskCol);

        drawList->PathLineTo(ImVec2(shMax.x, shMin.y));
        drawList->PathLineTo(ImVec2(shMax.x - 6.0f, shMin.y));
        drawList->PathArcTo(ImVec2(shMax.x - 6.0f, shMin.y + 6.0f), 6.0f, 1.5f * 3.14159265f, 2.0f * 3.14159265f);
        drawList->PathFillConvex(tabMaskCol);

        // Brighter red border and smooth outer red glow
        drawList->AddRect(shMin, shMax, IM_COL32(255, 10, 20, (int)(230 * g_SouthAlpha)), 6.0f, ImDrawCornerFlags_Top);
        for (int g = 1; g <= 8; ++g) {
            float offset = (float)g * 0.75f;
            ImVec2 gMin = ImVec2(shMin.x - offset, shMin.y - offset);
            ImVec2 gMax = ImVec2(shMax.x + offset, shMax.y + offset);
            float falloff = 1.0f - ((float)g / 8.0f);
            ImU32 glowCol = IM_COL32(255, 0, 0, (int)(45 * falloff * g_SouthAlpha));
            drawList->AddRect(gMin, gMax, glowCol, 6.0f + offset, ImDrawCornerFlags_Top, 1.0f);
        }
        drawList->AddText(ImVec2(shMin.x + 15, shMin.y + (catH - 8 - 16)*0.5f), textCol, headerLabel.c_str());
        currentY += catH;
    } else {
        // SubMenu Category Tabs (Red Theme)
        ImVec2 cMin = ImVec2(menuX, currentY);
        ImVec2 cMax = ImVec2(menuX + menuW, cMin.y + catH);
        drawList->AddRectFilled(cMin, cMax, IM_COL32(0, 0, 0, (int)(255 * g_SouthAlpha)));
        
        auto& cats = g_MenuData[g_ActiveSub].categories;
        if (cats.size() <= 4) {
            float cw = (menuW - 10) / (float)cats.size();
            for (int i = 0; i < (int)cats.size(); i++) {
                bool act = (g_ActiveCat == i);
                ImVec2 tcMin = ImVec2(cMin.x + 5 + i * cw, cMin.y + 3);
                ImVec2 tcMax = ImVec2(tcMin.x + cw - 2, cMax.y);
                
                if (act) {
                    drawList->AddRectFilledMultiColor(tcMin, tcMax,
                        IM_COL32(230, 20, 30, (int)(255 * g_SouthAlpha)),
                        IM_COL32(230, 20, 30, (int)(255 * g_SouthAlpha)),
                        IM_COL32(0, 0, 0, (int)(255 * g_SouthAlpha)),
                        IM_COL32(0, 0, 0, (int)(255 * g_SouthAlpha)));

                    // Mask corners to make it rounded
                    ImU32 tabMaskCol = IM_COL32(0, 0, 0, (int)(255 * g_SouthAlpha));
                    drawList->PathLineTo(tcMin);
                    drawList->PathLineTo(ImVec2(tcMin.x + 6.0f, tcMin.y));
                    drawList->PathArcTo(ImVec2(tcMin.x + 6.0f, tcMin.y + 6.0f), 6.0f, 1.5f * 3.14159265f, 1.0f * 3.14159265f);
                    drawList->PathFillConvex(tabMaskCol);

                    drawList->PathLineTo(ImVec2(tcMax.x, tcMin.y));
                    drawList->PathLineTo(ImVec2(tcMax.x - 6.0f, tcMin.y));
                    drawList->PathArcTo(ImVec2(tcMax.x - 6.0f, tcMin.y + 6.0f), 6.0f, 1.5f * 3.14159265f, 2.0f * 3.14159265f);
                    drawList->PathFillConvex(tabMaskCol);

                    // Brighter red border and smooth outer red glow
                    drawList->AddRect(tcMin, tcMax, IM_COL32(255, 10, 20, (int)(230 * g_SouthAlpha)), 6.0f, ImDrawCornerFlags_Top);
                    for (int g = 1; g <= 8; ++g) {
                        float offset = (float)g * 0.75f;
                        ImVec2 gMin = ImVec2(tcMin.x - offset, tcMin.y - offset);
                        ImVec2 gMax = ImVec2(tcMax.x + offset, tcMax.y + offset);
                        float falloff = 1.0f - ((float)g / 8.0f);
                        ImU32 glowCol = IM_COL32(255, 0, 0, (int)(45 * falloff * g_SouthAlpha));
                        drawList->AddRect(gMin, gMax, glowCol, 6.0f + offset, ImDrawCornerFlags_Top, 1.0f);
                    }
                }
                
                ImVec2 ts = ImGui::CalcTextSize(cats[i].label.c_str());
                drawList->AddText(ImVec2(tcMin.x + (cw - ts.x)*0.5f, tcMin.y + (catH - 8 - ts.y)*0.5f), act ? textCol : textDimCol, cats[i].label.c_str());
            }
        } else {
            ImVec2 tcMin = ImVec2(cMin.x + 5, cMin.y + 3);
            ImVec2 tcMax = ImVec2(cMin.x + menuW - 5, cMax.y);
            
            drawList->AddRectFilledMultiColor(tcMin, tcMax,
                IM_COL32(230, 20, 30, (int)(255 * g_SouthAlpha)),
                IM_COL32(230, 20, 30, (int)(255 * g_SouthAlpha)),
                IM_COL32(0, 0, 0, (int)(255 * g_SouthAlpha)),
                IM_COL32(0, 0, 0, (int)(255 * g_SouthAlpha)));

            ImU32 tabMaskCol = IM_COL32(0, 0, 0, (int)(255 * g_SouthAlpha));
            drawList->PathLineTo(tcMin);
            drawList->PathLineTo(ImVec2(tcMin.x + 6.0f, tcMin.y));
            drawList->PathArcTo(ImVec2(tcMin.x + 6.0f, tcMin.y + 6.0f), 6.0f, 1.5f * 3.14159265f, 1.0f * 3.14159265f);
            drawList->PathFillConvex(tabMaskCol);

            drawList->PathLineTo(ImVec2(tcMax.x, tcMin.y));
            drawList->PathLineTo(ImVec2(tcMax.x - 6.0f, tcMin.y));
            drawList->PathArcTo(ImVec2(tcMax.x - 6.0f, tcMin.y + 6.0f), 6.0f, 1.5f * 3.14159265f, 2.0f * 3.14159265f);
            drawList->PathFillConvex(tabMaskCol);

            // Brighter red border and smooth outer red glow
            drawList->AddRect(tcMin, tcMax, IM_COL32(255, 10, 20, (int)(230 * g_SouthAlpha)), 6.0f, ImDrawCornerFlags_Top);
            for (int g = 1; g <= 8; ++g) {
                float offset = (float)g * 0.75f;
                ImVec2 gMin = ImVec2(tcMin.x - offset, tcMin.y - offset);
                ImVec2 gMax = ImVec2(tcMax.x + offset, tcMax.y + offset);
                float falloff = 1.0f - ((float)g / 8.0f);
                ImU32 glowCol = IM_COL32(255, 0, 0, (int)(45 * falloff * g_SouthAlpha));
                drawList->AddRect(gMin, gMax, glowCol, 6.0f + offset, ImDrawCornerFlags_Top, 1.0f);
            }

            std::string labelText = "<  " + cats[g_ActiveCat].label + "  >";
            ImVec2 ts = ImGui::CalcTextSize(labelText.c_str());
            drawList->AddText(ImVec2(tcMin.x + ((menuW - 10) - ts.x)*0.5f, tcMin.y + (catH - 8 - ts.y)*0.5f), textCol, labelText.c_str());
        }
        currentY += catH;
    }

    // 4. Logic & Data
    std::vector<SouthItem> items;
    if (g_Depth == 0) {
        for (auto& s : g_MenuData) {
            std::string rootLabel = s.label;
            if (s.label == "Self Options") rootLabel = ICON_FA_USER "  " + rootLabel;
            else if (s.label == "Online Options") rootLabel = ICON_FA_USERS "  " + rootLabel;
            else if (s.label == "Weapon Options") rootLabel = ICON_FA_GUN "  " + rootLabel;
            else if (s.label == "Vehicle Options") rootLabel = ICON_FA_CAR "  " + rootLabel;
            else if (s.label == "Triggers") rootLabel = ICON_FA_FILE "  " + rootLabel;
            else if (s.label == "Exploits") rootLabel = ICON_FA_BOMB "  " + rootLabel;
            else if (s.label == "Settings") rootLabel = ICON_FA_GEAR "  " + rootLabel;
            
            items.push_back({rootLabel, "subMenu"});
        }
    } else {
        items = g_MenuData[g_ActiveSub].categories[g_ActiveCat].items;
    }
    int numItems = (int)items.size();

    // Clamp g_ItemIdx to prevent out-of-bounds crashes when list size shrinks
    if (g_ItemIdx >= numItems) {
        g_ItemIdx = numItems > 0 ? numItems - 1 : 0;
    }
    if (g_ItemIdx < 0) {
        g_ItemIdx = 0;
    }

    // Smooth Item Index Animation
    static float g_SmoothIdx = 0.0f;
    g_SmoothIdx = ImLerp(g_SmoothIdx, (float)g_ItemIdx, io.DeltaTime * 18.0f);

    static uint64_t lastInput = 0;
    if (g_ShowSouthMenu && !g_ShowSouthKeybindSetup && !g_ShowVehicleSpawnerInput && (GetTickCount64() - g_SouthKeybindSetupCloseTime > 500) && (GetTickCount64() - g_VehicleSpawnerInputCloseTime > 500) && GetForegroundWindow() == window && GetTickCount64() - lastInput > 140) {
        if (GetAsyncKeyState(VK_UP) & 0x8000) { 
            int startIdx = g_ItemIdx;
            do { 
                g_ItemIdx = (g_ItemIdx - 1 + numItems) % (numItems ? numItems : 1); 
                if (g_ItemIdx == startIdx) break; // Avoid infinite loop
            } while (numItems > 0 && g_ItemIdx >= 0 && g_ItemIdx < numItems && items[g_ItemIdx].type == "separator");
            PlayClickSound();
            lastInput = GetTickCount64(); 
        }
        if (GetAsyncKeyState(VK_DOWN) & 0x8000) { 
            int startIdx = g_ItemIdx;
            do { 
                g_ItemIdx = (g_ItemIdx + 1) % (numItems ? numItems : 1); 
                if (g_ItemIdx == startIdx) break; // Avoid infinite loop
            } while (numItems > 0 && g_ItemIdx >= 0 && g_ItemIdx < numItems && items[g_ItemIdx].type == "separator");
            PlayClickSound();
            lastInput = GetTickCount64(); 
        }
        if (g_Depth == 1 && numItems > 0 && g_ItemIdx >= 0 && g_ItemIdx < (int)g_MenuData[g_ActiveSub].categories[g_ActiveCat].items.size()) {
            SouthItem& it = g_MenuData[g_ActiveSub].categories[g_ActiveCat].items[g_ItemIdx];
            if (it.type == "slider" || it.type == "slider-check") {
                float oldVal = it.val;
                float step = 1.0f;
                float maxLimit = (it.label == "Noclip") ? 10.0f : (it.label == "FOV" ? 120.0f : 100.0f);

                if (GetAsyncKeyState(VK_LEFT) & 0x8000) { it.val = (std::max)(0.0f, it.val - step); PlayClickSound(); lastInput = GetTickCount64(); } 
                if (GetAsyncKeyState(VK_RIGHT) & 0x8000) { it.val = (std::min)(maxLimit, it.val + step); PlayClickSound(); lastInput = GetTickCount64(); }
                
                // Health & Armour Work (Lua-style logic)
                if (oldVal != it.val) {
                    if (it.label == "Health") {
                        LuaCore::SetHealthLua(it.val);
                    }
                    else if (it.label == "Armour") {
                        LuaCore::SetArmourLua(it.val);
                    }
                }
            }
            else if (it.type == "scroll" || it.type == "scroll-check") {
                if (GetAsyncKeyState(VK_LEFT) & 0x8000 || GetAsyncKeyState(VK_RIGHT) & 0x8000) {
                    if (it.label == "Menu Key") {
                        g_ShowSouthKeybindSetup = true;
                        g_TempSouthKey = 0;
                        g_SouthKeybindSetupOpenTime = GetTickCount64();
                        g_ShowSouthMenu = false;
                        g_SouthAlpha = 0.0f;
                    }
                    else if (it.label == "Clear Tasks") {
                        it.subVal = (it.subVal == "Primary") ? "Secondary" : "Primary";
                    }
                    else if (it.label == "Noclip") {
                        static uint64_t lastModeInp = 0;
                        if (GetTickCount64() - lastModeInp > 250) {
                            const char* modes[] = { "Default", "Native Invisible", "Local Invisible" };
                            int numModes = 3;
                            int oldMode = g_NoclipMode;
                            if (GetAsyncKeyState(VK_LEFT) & 0x8000) g_NoclipMode = (g_NoclipMode - 1 + numModes) % numModes;
                            else if (GetAsyncKeyState(VK_RIGHT) & 0x8000) g_NoclipMode = (g_NoclipMode + 1) % numModes;
                            it.subVal = modes[g_NoclipMode];
                            if (it.checked) LuaCore::SetInvisibleNoclipMode(g_NoclipMode);
                            lastModeInp = GetTickCount64();
                        }
                    }
                    else if (it.label == "Outfit") {
                        const char* skins[] = { "Civilian", "Police", "SWAT", "Medic" };
                        int numSkins = 4;
                        static int curSkin = 0;
                        if (GetAsyncKeyState(VK_LEFT) & 0x8000) curSkin = (curSkin - 1 + numSkins) % numSkins;
                        else if (GetAsyncKeyState(VK_RIGHT) & 0x8000) curSkin = (curSkin + 1) % numSkins;
                        it.subVal = skins[curSkin];
                    }
                    else if (it.label == "Spawn Category") {
                        static uint64_t lastScrollInp = 0;
                        if (GetTickCount64() - lastScrollInp > 200) {
                            const char* categories[] = {
                                "Super", "Sports", "Muscle", "Offroad", "Sedans", "Suvs", "Coupes", "Compacts",
                                "Cycles", "Vans", "Trucks", "Emergency", "Military", "Motorcycles", "Planes",
                                "Openwheel", "Helicopters", "Boats", "Industrial", "Utility", "Service",
                                "Commercial", "Sportsclassics"
                            };
                            int numCats = 23;
                            int curCatIdx = 0;
                            for (int idx = 0; idx < numCats; idx++) {
                                if (it.subVal == categories[idx]) {
                                    curCatIdx = idx;
                                    break;
                                }
                            }
                            if (GetAsyncKeyState(VK_LEFT) & 0x8000) curCatIdx = (curCatIdx - 1 + numCats) % numCats;
                            else if (GetAsyncKeyState(VK_RIGHT) & 0x8000) curCatIdx = (curCatIdx + 1) % numCats;
                            it.subVal = categories[curCatIdx];
                            lastScrollInp = GetTickCount64();
                        }
                    }
                    else if (it.label == "Spawn Category") {
                        static uint64_t lastScrollInp = 0;
                        if (GetTickCount64() - lastScrollInp > 200) {
                            const char* categories[] = {
                                "Super", "Sports", "Muscle", "Offroad", "Sedans", "Suvs", "Coupes", "Compacts",
                                "Cycles", "Vans", "Trucks", "Emergency", "Military", "Motorcycles", "Planes",
                                "Openwheel", "Helicopters", "Boats", "Industrial", "Utility", "Service",
                                "Commercial", "Sportsclassics"
                            };
                            int numCats = 23;
                            int curCatIdx = 0;
                            for (int idx = 0; idx < numCats; idx++) {
                                if (it.subVal == categories[idx]) {
                                    curCatIdx = idx;
                                    break;
                                }
                            }
                            if (GetAsyncKeyState(VK_LEFT) & 0x8000) curCatIdx = (curCatIdx - 1 + numCats) % numCats;
                            else if (GetAsyncKeyState(VK_RIGHT) & 0x8000) curCatIdx = (curCatIdx + 1) % numCats;
                            it.subVal = categories[curCatIdx];
                            lastScrollInp = GetTickCount64();
                        }
                    }
                    else if (it.label == "Crash Player" || it.label == "Crash Nearby Players") {
                        static uint64_t lastScrollInp = 0;
                        if (GetTickCount64() - lastScrollInp > 200) {
                            const char* modes[] = { "StateBag", "OxLib", "Ped" };
                            int numModes = 3;
                            int idx = 0;
                            for (int i = 0; i < numModes; i++) {
                                if (it.subVal == modes[i]) {
                                    idx = i;
                                    break;
                                }
                            }
                            if (GetAsyncKeyState(VK_LEFT) & 0x8000) idx = (idx - 1 + numModes) % numModes;
                            else if (GetAsyncKeyState(VK_RIGHT) & 0x8000) idx = (idx + 1) % numModes;
                            it.subVal = modes[idx];
                            lastScrollInp = GetTickCount64();
                        }
                    }
                    else if (it.label == "Give txAdmin") {
                        static uint64_t lastScrollInp = 0;
                        if (GetTickCount64() - lastScrollInp > 200) {
                            const char* options[] = { "Only UI", "Risk" };
                            int numOpts = 2;
                            int curIdx = (it.subVal == "Risk") ? 1 : 0;
                            if (GetAsyncKeyState(VK_LEFT) & 0x8000) curIdx = (curIdx - 1 + numOpts) % numOpts;
                            else if (GetAsyncKeyState(VK_RIGHT) & 0x8000) curIdx = (curIdx + 1) % numOpts;
                            it.subVal = options[curIdx];
                            lastScrollInp = GetTickCount64();
                        }
                    }
                    else if (it.label == "Freemode") {
                        static uint64_t lastScrollInp = 0;
                        if (GetTickCount64() - lastScrollInp > 200) {
                            const char* skins[] = { "Freemode Male", "Freemode Female" };
                            int numSkins = 2;
                            static int curSkin = 0;
                            if (GetAsyncKeyState(VK_LEFT) & 0x8000) curSkin = (curSkin - 1 + numSkins) % numSkins;
                            else if (GetAsyncKeyState(VK_RIGHT) & 0x8000) curSkin = (curSkin + 1) % numSkins;
                            it.subVal = skins[curSkin];
                            lastScrollInp = GetTickCount64();
                        }
                    }
                    else if (it.label == "Peds") {
                        static uint64_t lastScrollInp = 0;
                        if (GetTickCount64() - lastScrollInp > 200) {
                            const char* skins[] = {
                                "Michael", "Franklin", "Trevor", "Lamar", "Jimmy", "Amanda", "Tracey", "Ron", "Wade", "Dave Norton", 
                                "Steve Haines", "Devin Weston", "Floyd", "Chef", "Lester", "Chop", "Brad", 
                                "Police Officer Male", "Police Officer Female", "SWAT", "Sheriff Male", "Sheriff Female",
                                "Highway Cop", "FIB Male", "FIB Female", "Paramedic", "Firefighter", "Doctor",
                                "Construction Worker", "Pilot Male", "Pilot Female", "Business Male", "Business Female",
                                "Street Dealer", "Gang Male 1", "Gang Male 2", "Gang Female 1", "Ballas 1", "Ballas 2", "Ballas Female",
                                "Families 1", "Families 2", "Vagos 1", "Vagos 2", "Lost MC 1", "Lost MC 2", "Lost MC Female",
                                "Army Soldier", "Marine 1", "Marine 2", "Prisoner Male", "Prison Guard", "Cop Undercover",
                                "Security Guard", "Janitor", "Hobo Male", "Hobo Female", "Prostitute 1", "Prostitute 2",
                                "Beach Male", "Beach Female", "Tourist Male", "Tourist Female", "Skater", "Hipster Male", "Hipster Female",
                                "Bouncer", "Shopkeeper", "Chef", "Bartender", "Waiter", "Mechanic", "Taxi Driver", "Gardener", "Farmer",
                                "Dock Worker", "Trash Worker", "Postal Worker", "Bus Driver", "Pilot", "Air Hostess",
                                "Cop Traffic", "Cop Detective", "Agent", "Reporter", "News Cameraman",
                                "Hunter", "Hiker Male", "Hiker Female", "Golfer Male", "Golfer Female", "Tennis Player Male", "Tennis Player Female"
                            };
                            int numSkins = sizeof(skins) / sizeof(skins[0]);
                            static int curSkin = 0;
                            if (GetAsyncKeyState(VK_LEFT) & 0x8000) curSkin = (curSkin - 1 + numSkins) % numSkins;
                            else if (GetAsyncKeyState(VK_RIGHT) & 0x8000) curSkin = (curSkin + 1) % numSkins;
                            it.subVal = skins[curSkin];
                            lastScrollInp = GetTickCount64();
                        }
                    }
                    else if (it.label == "Animal Peds") {
                        static uint64_t lastScrollInp = 0;
                        if (GetTickCount64() - lastScrollInp > 200) {
                            const char* skins[] = {
                                "Boar", "Cat", "Chicken", "Chimp", "Cow", "Coyote", "Crow", 
                                "Deer", "Dolphin", "Fish", "Hen", "Humpback", "Husky", 
                                "Killer Whale", "Mountain Lion", "Pig", "Pigeon", "Poodle", 
                                "Pug", "Rabbit", "Rat", "Retriever", "Rhesus Monkey",
                                "Rottweiler", "Seagull", "Shepherd", "Stingray", "Tiger Shark", 
                                "Hammerhead Shark", "Cow", "Cat2", "Chickenhawk", "Cormorant",
                                "Coyote2", "Chimp2", "Boar2", "Deer2", "Fish2", "Husky2",
                                "Pug2", "Poodle2", "Retriever2", "Shepherd2", "Rat2", "Rabbit2",
                                "Dolphin2", "Killer Whale2", "Mountain Lion2", "Pig2", "Seagull2",
                                "Stingray2", "Tiger Shark2", "Hammerhead Shark2"
                            };
                            int numSkins = sizeof(skins) / sizeof(skins[0]);
                            static int curSkin = 0;
                            if (GetAsyncKeyState(VK_LEFT) & 0x8000) curSkin = (curSkin - 1 + numSkins) % numSkins;
                            else if (GetAsyncKeyState(VK_RIGHT) & 0x8000) curSkin = (curSkin + 1) % numSkins;
                            it.subVal = skins[curSkin];
                            lastScrollInp = GetTickCount64();
                        }
                    }
                    PlayClickSound(); lastInput = GetTickCount64();
                }
            }
        }

        if (g_Depth == 1) {
            int numCats = (int)g_MenuData[g_ActiveSub].categories.size();
            if (GetAsyncKeyState('Q') & 0x8000) { g_ActiveCat = (g_ActiveCat - 1 + numCats) % numCats; g_ItemIdx = 0; PlayClickSound(); lastInput = GetTickCount64(); }
            if (GetAsyncKeyState('E') & 0x8000) { g_ActiveCat = (g_ActiveCat + 1) % numCats; g_ItemIdx = 0; PlayClickSound(); lastInput = GetTickCount64(); }
        }

        if (GetAsyncKeyState(VK_RETURN) & 0x8000) {
            PlayClickSound();
            lastInput = GetTickCount64();
            if (g_Depth == 0) { g_ActiveSub = g_ItemIdx; g_Depth = 1; g_ActiveCat = 0; g_ItemIdx = 0; }
            else { 
                static uint64_t lastActionTime = 0;
                uint64_t currentTime = GetTickCount64();
                if (currentTime - lastActionTime >= 500) {
                    lastActionTime = currentTime;
                    if (g_ItemIdx >= 0 && g_ItemIdx < (int)g_MenuData[g_ActiveSub].categories[g_ActiveCat].items.size()) {
                        SouthItem& it = g_MenuData[g_ActiveSub].categories[g_ActiveCat].items[g_ItemIdx]; 
                        if (it.type == "check" || it.type == "slider-check" || it.type == "scroll-check") {
                    if (it.label == "Noclip") {
                        if (LuaCore::ToggleNoclip(!it.checked, g_NoclipSpeed, g_NoclipMode)) {
                            it.checked = !it.checked;
                            g_NoclipEnabled = it.checked;
                            if (g_NoclipEnabled) {
                                // Turn off Freecam if active to prevent crashes
                                if (g_FreecamActive) {
                                    g_FreecamActive = false;
                                    LuaCore::ToggleFreecam(false, 0, 0);
                                    for (auto& sub : g_MenuData) {
                                        for (auto& cat : sub.categories) {
                                            for (auto& item_sub : cat.items) {
                                                if (item_sub.label == "Freecam") item_sub.checked = false;
                                            }
                                        }
                                    }
                                }
                            } else {
                                g_NoclipPed = 0;
                            }
                            PushNotification(it.checked ? "Noclip Success!" : "Noclip Disabled");
                        } else {
                            char buf[64];
                            sprintf_s(buf, "Noclip Toggle Cooldown (%dms)", (g_NoclipMode == 1 || g_NoclipMode == 2) ? 2000 : 1000);
                            PushNotification(buf);
                        }
                    }
                    else if (it.label == "Freecam") {
                        it.checked = !it.checked;
                        g_FreecamActive = it.checked;
                        if (g_FreecamActive) {
                            // Turn off Noclip if active to prevent crashes
                            if (g_NoclipEnabled) {
                                LuaCore::ToggleNoclip(false, g_NoclipSpeed, g_NoclipMode);
                                g_NoclipEnabled = false;
                                g_NoclipPed = 0;
                                for (auto& sub : g_MenuData) {
                                    for (auto& cat : sub.categories) {
                                        for (auto& item_sub : cat.items) {
                                            if (item_sub.label == "Noclip") item_sub.checked = false;
                                        }
                                    }
                                }
                            }
                        }
                        LuaCore::ToggleFreecam(it.checked, it.val, g_FreecamFeatureIdx + 1);
                    }
                    else if (it.label == "Tiny Ped") { it.checked = !it.checked; LuaCore::ToggleTinyPed(it.checked); }
                    else if (it.label == "Super Punch") { it.checked = !it.checked; LuaCore::ToggleSuperPunch(it.checked); }
                    else if (it.label == "Infinite Ammo") { it.checked = !it.checked; LuaCore::ToggleInfiniteAmmo(it.checked); }
                    else if (it.label == "Rapid Fire") { it.checked = !it.checked; LuaCore::ToggleRapidFire(it.checked); }
                    else if (it.label == "No Recoil") { it.checked = !it.checked; LuaCore::ToggleNoRecoil(it.checked); }
                    else if (it.label == "No Spread") { it.checked = !it.checked; LuaCore::ToggleNoSpread(it.checked); }
                    else {
                        it.checked = !it.checked;
                        if (it.label == "Godmode") LuaCore::ToggleGodmode(it.checked);
                        else if (it.label == "Auto Hunger") LuaCore::ToggleAutoHunger(it.checked);
                        else if (it.label == "Auto Thirst") LuaCore::ToggleAutoThirst(it.checked);
                        else if (it.label == "Semi Godmode") LuaCore::ToggleSemiGod(it.checked);
                        else if (it.label == "Pickup/Throw Vehicles") LuaCore::TogglePickupVehicles(it.checked);
                        else if (it.label == "White Screen All (RISK)") LuaCore::ToggleWhiteScreenAll(it.checked);
                        else if (it.label == "Fire Screen All (RISK)") LuaCore::ToggleFireScreenAll(it.checked);
                        else if (it.label == "Sound Spam All (RISK)") LuaCore::ToggleSoundSpamAll(it.checked);
                        else if (it.label == "Invisibility") LuaCore::ToggleInvisibility(it.checked);
                        else if (it.label == "Fast Run") LuaCore::ToggleFastRun(it.checked);
                        else if (it.label == "Fast Punch") LuaCore::ToggleFastPunch(it.checked);
                        else if (it.label == "Super Jump") LuaCore::ToggleSuperJump(it.checked);
                        else if (it.label == "Invincibility") LuaCore::ToggleInvincibility(it.checked);
                        else if (it.label == "txAdmin Noclip") LuaCore::ToggleTxAdminNoclip(it.checked);
                        else if (it.label == "No Ragdoll") LuaCore::ToggleNoRagdoll(it.checked);
                        else if (it.label == "Anti Freeze") LuaCore::ToggleAntiFreeze(it.checked);
                        else if (it.label == "Anti Headshot") LuaCore::ToggleAntiHeadshot(it.checked);
                        else if (it.label == "Anti Spectate") LuaCore::ToggleAntiSpectate(it.checked);
                        else if (it.label == "Force Third Person") LuaCore::ToggleForceThirdPerson(it.checked);
                        else if (it.label == "Force Driveby") LuaCore::ToggleForceDriveby(it.checked);
                        else if (it.label == "Infinity Stamina") LuaCore::ToggleInfiniteStamina(it.checked);
                        else if (it.label == "Disable All txAdmin" || it.label == "Disable txAdmin Teleport" || it.label == "Disable txAdmin Freeze") {
                            LuaCore::ToggleDisableTxAdmin(it.checked);
                        }
                        
                        // Sync back to g_PlayerListToggles if it's a dynamic player item!
                        if (g_ActiveSub == 1 && g_ActiveCat == 0 && !it.subVal.empty()) {
                            SRWLockGuard lock(&g_PlayerListLock);
                            if (it.checked) {
                                for (auto& pair : g_PlayerListToggles) {
                                    if (pair.first != it.subVal) {
                                        pair.second = false;
                                    }
                                }
                                for (auto& menu_item : g_MenuData[1].categories[0].items) {
                                    if (menu_item.subVal != it.subVal && !menu_item.subVal.empty()) {
                                        menu_item.checked = false;
                                    }
                                }
                            }
                            g_PlayerListToggles[it.subVal] = it.checked;
                        }

                        // Sync back to g_VehicleListToggles if it's a dynamic vehicle item!
                        if (g_ActiveSub == 2 && g_ActiveCat == 0 && !it.subVal.empty()) {
                            uintptr_t vehAddr = std::stoull(it.subVal);
                            SRWLockGuard lock(&g_VehicleListLock);
                            if (it.checked) {
                                for (auto& pair : g_VehicleListToggles) {
                                    if (pair.first != vehAddr) {
                                        pair.second = false;
                                    }
                                }
                                for (auto& menu_item : g_MenuData[2].categories[0].items) {
                                    if (menu_item.subVal != it.subVal && !menu_item.subVal.empty()) {
                                        menu_item.checked = false;
                                    }
                                }
                            }
                            g_VehicleListToggles[vehAddr] = it.checked;
                        }

                        // Trigger Troll/Risky checkboxes for selected players!
                        if (g_ActiveSub == 1 && (g_ActiveCat == 1 || g_ActiveCat == 2)) {
                            bool executed = false;
                            SRWLockGuard lock(&g_PlayerListLock);
                            for (const auto& player : g_PlayerList) {
                                if (g_PlayerListToggles[player.name]) {
                                    LuaCore::ExecutePlayerAction(player.serverId, it.label, it.checked);
                                    executed = true;
                                }
                            }
                            if (!executed) {
                                it.checked = !it.checked; // Revert state
                                PushNotification("Error: No players selected!");
                                return;
                            }
                        }

                        if (!(g_ActiveSub == 1 && g_ActiveCat == 0) && !(g_ActiveSub == 2 && g_ActiveCat == 0)) {
                            PushNotification(it.label + (it.checked ? " Success!" : " Disabled"));
                        }
                    }

                    if (it.label == "Disable Sound") {
                        g_DisableSound = it.checked;
                    }
                    if (it.label == "Disable Keybind Overlay") {
                        g_DisableKeybindOverlay = it.checked;
                    }
                }
                else if (it.type == "scroll") {
                    if (it.label == "Freemode" || it.label == "Peds" || it.label == "Animal Peds") {
                        LuaCore::ChangePedModel(it.subVal);
                        PushNotification("Ped Success!");
                    }
                    else if (it.label == "Menu Key") {
                        g_ShowSouthKeybindSetup = true;
                        g_TempSouthKey = 0;
                        g_SouthKeybindSetupOpenTime = GetTickCount64();
                        g_ShowSouthMenu = false;
                        g_SouthAlpha = 0.0f;
                    }
                    else if (it.label == "Give txAdmin") {
                        if (LuaCore::ExecuteTxAdmin(it.subVal)) {
                            PushNotification("txAdmin Success (" + it.subVal + ")!");
                        } else {
                            PushNotification("Error: FiveM Manager not ready!");
                        }
                    }
                    else if (it.label == "Crash Nearby Players") {
                        static uint64_t lastCrashNearbyPress = 0;
                        uint64_t now = GetTickCount64();
                        if (now - lastCrashNearbyPress >= 2000) {
                            lastCrashNearbyPress = now;
                            std::string action = "Crash Nearby Players (" + it.subVal + ")";
                            LuaCore::ExecuteExploitAction(action);
                            PushNotification("Crash Nearby Players Success (" + it.subVal + ")!");
                        }
                    }
                    else if (it.label == "Crash Player") {
                        static uint64_t lastCrashPress = 0;
                        uint64_t now = GetTickCount64();
                        if (now - lastCrashPress >= 2000) {
                            lastCrashPress = now;
                            SRWLockGuard lock(&g_PlayerListLock);
                            for (const auto& player : g_PlayerList) {
                                if (g_PlayerListToggles[player.name]) {
                                    std::string action = "Crash Player (" + it.subVal + ")";
                                    LuaCore::ExecutePlayerAction(player.serverId, action, false);
                                }
                            }
                            PushNotification("Crash Player Success (" + it.subVal + ")!");
                        }
                    }
                }
                else if (it.type == "button") {
                    if (it.label == "Revive") {
                        LuaCore::ExecuteUniversalRevive();
                        PushNotification("Revive Success!");
                    }
                    else if (it.label == "Money Wash (100k)") {
                        LuaCore::ExecuteTriggerAction("Money Wash");
                        PushNotification("Money Wash Success!");
                    }
                    else if (it.label == "Printer Money (10k)") {
                        LuaCore::ExecuteTriggerAction("Printer Money");
                        PushNotification("Printer Money Success!");
                    }
                    else if (it.label == "Drug Reward (Money)") {
                        LuaCore::ExecuteTriggerAction("Drug Reward");
                        PushNotification("Drug Reward Success!");
                    }
                    else if (it.label == "Gain Item (Cash)") {
                        LuaCore::ExecuteTriggerAction("Gain Item");
                        PushNotification("Gain Item Success!");
                    }
                    else if (it.label == "Spawn Money") {
                        LuaCore::ExecuteTriggerAction("Spawn Money");
                        PushNotification("Spawn Money Success!");
                    }
                    else if (it.label == "Spawn All Item") {
                        LuaCore::ExecuteTriggerAction("Spawn All Item");
                        PushNotification("Spawn Items Success!");
                    }
                    else if (it.label == "Lashes Reward") {
                        LuaCore::ExecuteTriggerAction("Lashes");
                        PushNotification("Lashes Reward Success!");
                    }
                    else if (it.label == "Nails Reward") {
                        LuaCore::ExecuteTriggerAction("Nails");
                        PushNotification("Nails Reward Success!");
                    }
                    else if (it.label == "Caps Reward") {
                        LuaCore::ExecuteTriggerAction("Caps");
                        PushNotification("Caps Reward Success!");
                    }
                    else if (it.label == "Wigs Reward") {
                        LuaCore::ExecuteTriggerAction("Wigs");
                        PushNotification("Wigs Reward Success!");
                    }
                    else if (it.label == "Icebox Reward") {
                        LuaCore::ExecuteTriggerAction("Icebox");
                        PushNotification("Icebox Reward Success!");
                    }
                    else if (it.label == "Sneaker Reward") {
                        LuaCore::ExecuteTriggerAction("Sneaker");
                        PushNotification("Sneaker Reward Success!");
                    }
                    else if (it.label == "Handbag Reward") {
                        LuaCore::ExecuteTriggerAction("Handbag");
                        PushNotification("Handbag Reward Success!");
                    }
                    else if (it.label == "Spoody Sim Swap") {
                        LuaCore::ExecuteTriggerAction("Spoody Sim");
                        PushNotification("Spoody Sim Swap Success!");
                    }
                    else if (it.label == "Spoody Clone Card") {
                        LuaCore::ExecuteTriggerAction("Spoody Clone");
                        PushNotification("Spoody Clone Card Success!");
                    }
                    else if (it.label == "Drug Receive") {
                        LuaCore::ExecuteTriggerAction("Drug Receive");
                        PushNotification("Drug Receive Success!");
                    }
                    else if (it.label == "Set Chat Tag") {
                        LuaCore::ExecuteTriggerAction("Set Chat Tag");
                        PushNotification("Chat Tag Set Success!");
                    }
                    else if (it.label == "Refill Health") {
                        LuaCore::SetHealthLua(100.0f); // Max Health (100+100=200)
                        g_MenuData[0].categories[0].items[1].val = 100.0f; // Sync slider
                        PushNotification("Health Success!");
                    }
                    else if (it.label == "Refill Armour") {
                        LuaCore::SetArmourLua(100.0f); // Max Armour
                        g_MenuData[0].categories[0].items[2].val = 100.0f; // Sync slider
                        PushNotification("Armour Success!");
                    }
                    else if (it.label == "Outfit") {
                        LuaCore::ApplyRandomOutfit();
                        PushNotification("Outfit Success!");
                    }
                    else if (it.label == "Self Crash") {
                        LuaCore::ExecuteSelfCrash();
                        PushNotification("Self Crash Success!");
                    }
                    else if (it.label == "Clear Screen Effects") {
                        LuaCore::ClearScreenEffects();
                        PushNotification("Screen Effects Success!");
                    }
                    else if (it.label == "Suicide") {
                        LuaCore::ExecuteSuicide();
                    }
                    else if (it.label == "Force Ragdoll") {
                        LuaCore::ExecuteForceRagdoll();
                    }
                    else if (it.label == "Remove Crutch") {
                        LuaCore::StopWasabiCrutch();
                        PushNotification("Crutch Success!");
                    }
                    else if (it.label == "Fake Bandage") {
                        LuaCore::ApplyFakeBandage();
                    }
                    else if (it.label == "Fake Armour") {
                        LuaCore::ApplyFakeArmour();
                    }
                    else if (it.label == "Fake Death") {
                        LuaCore::ApplyFakeDeath();
                    }
                    else if (it.label == "Clear Tasks") {
                        LuaCore::ExecuteClearTasks(it.subVal == "Primary");
                        PushNotification("Tasks Success!");
                    }
                    else if (it.label == "Teleport to Vehicle" || it.label == "Fix Vehicle" ||
                             it.label == "Explode Vehicle" || it.label == "Explode Vehicle Tires" ||
                             it.label == "Launch Vehicle" || it.label == "Copy Vehicle" ||
                             it.label == "Destroy Vehicle") {
                        float vx = 0, vy = 0, vz = 0;
                        {
                            SRWLockGuard lock(&g_VehicleListLock);
                            for (const auto& v : g_VehicleList) {
                                if (g_VehicleListToggles[v.vehicle]) {
                                    vx = v.pos.x;
                                    vy = v.pos.y;
                                    vz = v.pos.z;
                                    break;
                                }
                            }
                        }
                        LuaCore::ExecuteVehicleAction(it.label, vx, vy, vz);
                        PushNotification(it.label + " Success!");
                    }
                    else if (it.label == "Give Weapons" || it.label == "Give All Weapons") {
                        LuaCore::GiveAllWeapons();
                        PushNotification("Give All Weapons Success!");
                    }
                    else if (it.label == "Type Model") {
                        g_ShowVehicleSpawnerInput = true;
                        g_VehicleSpawnerInputBuffer[0] = '\0';
                        g_ShowSouthMenu = false;
                        g_SouthAlpha = 0.0f;
                    }
                    else if (g_ActiveSub == 2 && g_ActiveCat == 1 && g_ItemIdx >= 11 && !it.subVal.empty()) {
                        bool warp = g_MenuData[2].categories[1].items[10].checked;
                        LuaCore::SpawnVehicle(it.subVal, warp);
                        PushNotification("Spawn " + it.label + " Success!");
                    }
                    else if (it.label == "Kill All Players") {
                        LuaCore::ExecuteExploitAction("Kill All Players");
                        PushNotification("Kill All Players Success!");
                    }
                    else if (it.label == "Ragdoll All Players") {
                        LuaCore::ExecuteExploitAction("Ragdoll All Players");
                        PushNotification("Ragdoll All Players Success!");
                    }
                    else if (it.label == "Explode All Players") {
                        LuaCore::ExecuteExploitAction("Explode All Players");
                        PushNotification("Explode All Players Success!");
                    }
                    else if (it.label == "Explode All Vehicles") {
                        LuaCore::ExecuteExploitAction("Explode All Vehicles");
                        PushNotification("Explode All Vehicles Success!");
                    }
                    else if (it.label == "Launch All Players") {
                        static uint64_t lastLaunchAllPress = 0;
                        uint64_t now = GetTickCount64();
                        if (now - lastLaunchAllPress >= 500) {
                            lastLaunchAllPress = now;
                            LuaCore::ExecuteExploitAction("Launch All Players");
                            PushNotification("Launch All Players Success!");
                        }
                    }
                    else if (it.label == "Crash All Players") {
                        static uint64_t lastCrashAllPress = 0;
                        uint64_t now = GetTickCount64();
                        if (now - lastCrashAllPress >= 2000) {
                            lastCrashAllPress = now;
                            LuaCore::ExecuteExploitAction("Crash All Players");
                            PushNotification("Crash All Players Success!");
                        }
                    }
                    else if (it.label == "Launch All Vehicles") {
                        LuaCore::ExecuteExploitAction("Launch All Vehicles");
                        PushNotification("Launch All Vehicles Success!");
                    }
                    else if (it.label == "Steal All Vehicles") {
                        LuaCore::ExecuteExploitAction("Steal All Vehicles");
                        PushNotification("Steal All Vehicles Success!");
                    }
                    else if (it.label == "Kick All Players From Vehicle") {
                        LuaCore::ExecuteExploitAction("Kick All Players From Vehicle");
                        PushNotification("Kick All Players From Vehicle Success!");
                    }
                    else if (it.label == "Ram All Players With Vehicle") {
                        LuaCore::ExecuteExploitAction("Ram All Players With Vehicle");
                        PushNotification("Ram All Players With Vehicle Success!");
                    }
                    else if (it.label == "Heal All Players") {
                        LuaCore::ExecuteExploitAction("Heal All Players");
                        PushNotification("Heal All Players Success!");
                    }
                    else if (it.label == "Give Armour to All Players") {
                        LuaCore::ExecuteExploitAction("Give Armour to All Players");
                        PushNotification("Give Armour to All Players Success!");
                    }
                    else if (it.label == "Give Weapons to All Players") {
                        LuaCore::ExecuteExploitAction("Give Weapons to All Players");
                        PushNotification("Give Weapons to All Players Success!");
                    }
                    else if (it.label == "Drop Planes On All Players") {
                        LuaCore::ExecuteExploitAction("Drop Planes On All Players");
                        PushNotification("Drop Planes On All Players Success!");
                    }
                    else if (it.label == "Freeze All Players") {
                        LuaCore::ExecuteExploitAction("Freeze All Players");
                        PushNotification("Freeze All Players Success!");
                    }
                    else if (it.label == "Clone All Players") {
                        LuaCore::ExecuteExploitAction("Clone All Players");
                        PushNotification("Clone All Players Success!");
                    }
                    else if (it.label == "Talk to All Players") {
                        LuaCore::ExecuteExploitAction("Talk to All Players");
                        PushNotification("Talk to All Players Success!");
                    }
                    else if (it.label == "Flash Bang All Players") {
                        LuaCore::ExecuteExploitAction("Flash Bang All Players");
                        PushNotification("Flash Bang All Players Success!");
                    }
                    else if (it.label == "Tazer All Players") {
                        LuaCore::ExecuteExploitAction("Tazer All Players");
                        PushNotification("Tazer All Players Success!");
                    }
                    else if (it.label == "Rape All Players") {
                        LuaCore::ExecuteExploitAction("Rape All Players");
                        PushNotification("Rape All Players Success!");
                    }
                    else if (it.label == "Block Jumping All Players") {
                        LuaCore::ExecuteExploitAction("Block Jumping All Players");
                        PushNotification("Block Jumping All Players Success!");
                    }
                    else if (it.label == "Send to Void All Players") {
                        LuaCore::ExecuteExploitAction("Send to Void All Players");
                        PushNotification("Send to Void All Players Success!");
                    }
                    else if (it.label == "Attach Truck All Players") {
                        LuaCore::ExecuteExploitAction("Attach Truck All Players");
                        PushNotification("Attach Truck All Players Success!");
                    }
                    else if (it.label == "Select Everyone") {
                        SRWLockGuard lock(&g_PlayerListLock);
                        for (const auto& p : g_PlayerList) {
                            g_PlayerListToggles[p.name] = true;
                        }
                        UpdateServerPlayerList();
                        PushNotification("Select All Success!");
                    }
                    else if (it.label == "Un-Select Everyone" || it.label == "Clear Selection") {
                        SRWLockGuard lock(&g_PlayerListLock);
                        g_PlayerListToggles.clear();
                        UpdateServerPlayerList();
                    }
                    else if (it.label == "Select All Vehicles") {
                        SRWLockGuard lock(&g_VehicleListLock);
                        for (const auto& v : g_VehicleList) {
                            g_VehicleListToggles[v.vehicle] = true;
                        }
                        UpdateVehicleList();
                        PushNotification("Select All Vehicles Success!");
                    }
                    else if (it.label == "Un-Select All Vehicles" || it.label == "Clear Vehicle Selection") {
                        SRWLockGuard lock(&g_VehicleListLock);
                        g_VehicleListToggles.clear();
                        UpdateVehicleList();
                        
                        // Reset all Trolls and Risky toggles
                        if (g_MenuData.size() > 1 && g_MenuData[1].categories.size() > 2) {
                            // Reset Trolls category (index 1)
                            for (auto& item : g_MenuData[1].categories[1].items) {
                                if (item.type == "check") {
                                    item.checked = false;
                                    // Disable active Lua states
                                    if (item.label == "Spectate Player") {
                                        LuaCore::spectateActive = false;
                                        LuaCore::spectateTargetSid = -1;
                                    }
                                }
                            }
                            // Reset Risky category (index 2)
                            for (auto& item : g_MenuData[1].categories[2].items) {
                                if (item.type == "check") {
                                    item.checked = false;
                                }
                            }
                        }
                        
                        PushNotification("Clear Selection Success!");
                    }
                    // Trigger Troll/Risky buttons for selected players!
                    else if (g_ActiveSub == 1 && (g_ActiveCat == 1 || g_ActiveCat == 2)) {
                        bool allowed = true;
                        if (it.label == "Launch Player") {
                            static uint64_t lastLaunchPress = 0;
                            uint64_t now = GetTickCount64();
                            if (now - lastLaunchPress < 500) {
                                allowed = false;
                            } else {
                                lastLaunchPress = now;
                            }
                        }

                        if (allowed) {
                            bool executed = false;
                            SRWLockGuard lock(&g_PlayerListLock);
                            for (const auto& player : g_PlayerList) {
                                if (g_PlayerListToggles[player.name]) {
                                    // "Teleport To Player" in main.cpp label, mapped to "Teleport to Player" in Lua.h
                                    std::string action = it.label;
                                    if (action == "Teleport To Player") action = "Teleport to Player";
                                    
                                    LuaCore::ExecutePlayerAction(player.serverId, action, false);
                                    executed = true;
                                }
                            }
                            if (!executed) {
                                PushNotification("Error: No players selected!");
                            }
                        }
                    }
                }
            }
            }
            lastInput = GetTickCount64();
        }
    }

        if (GetAsyncKeyState(VK_F9) & 0x8000) {
            // Check if current item supports keybinding
            bool isKeybindItem = false;
            if (g_Depth == 1) {
                auto& curItems = g_MenuData[g_ActiveSub].categories[g_ActiveCat].items;
                if (g_ItemIdx >= 0 && g_ItemIdx < (int)curItems.size()) {
                    static const char* kbItems[] = {
                        "Noclip", "Freecam", "Revive", "Health", "Armour",
                        "Refill Health", "Refill Armour", "Godmode", "Invisibility",
                        "Fast Run", "Super Jump", "Invincibility",
                        "Self Crash", "Clear Screen Effects", "Suicide",
                        "Force Ragdoll", "Remove Crutch", "Clear Tasks",
                        "Infinity Stamina", "Tiny Ped", "Super Punch"
                    };
                    for (const auto& kb : kbItems) {
                        if (curItems[g_ItemIdx].label == kb) { isKeybindItem = true; break; }
                    }
                }
            }

            if (isKeybindItem) {
                // Open keybind setup popup for this item
                auto& curItems = g_MenuData[g_ActiveSub].categories[g_ActiveCat].items;
                g_KeybindSetupTarget = curItems[g_ItemIdx].label;
                g_ShowSouthKeybindSetup = true;
                g_TempSouthKey = 0;
                g_SouthKeybindSetupOpenTime = GetTickCount64();
                g_ShowSouthMenu = false;
                g_SouthAlpha = 0.0f;
                PlayClickSound();
            }
            lastInput = GetTickCount64();
        }

        if (GetAsyncKeyState(VK_BACK) & 0x8000) {
            PlayClickSound();
            if (g_Depth == 1) {
                g_Depth = 0;
                g_ItemIdx = g_ActiveSub;
            } else {
                g_ShowSouthMenu = false;
            }
            lastInput = GetTickCount64();
        }
    }



    // 5. List
    float itemH = 34.0f;
    int maxV = 7;
    int visible = (numItems > maxV) ? maxV : numItems;
    
    static int displayStart = 0;
    if (numItems > maxV) {
        if (g_ItemIdx < displayStart) displayStart = g_ItemIdx;
        if (g_ItemIdx >= displayStart + maxV) displayStart = g_ItemIdx - maxV + 1;
    } else {
        displayStart = 0;
    }

    ImVec2 lMin = ImVec2(menuX, currentY);
    ImVec2 lMax = ImVec2(menuX + menuW, lMin.y + visible * itemH + 8);
    drawList->AddRectFilled(lMin, lMax, bgCol);
    
    // Smooth Selection Bar (Relative to displayStart)
    if (numItems > 0) {
        float relativeIdx = g_SmoothIdx - (float)displayStart;
        // Clamp relativeIdx to visible range for safety during animation
        relativeIdx = (std::max)(0.0f, (std::min)((float)maxV - 1.0f, relativeIdx));
        
        float selY = lMin.y + 4 + (relativeIdx * itemH);
        ImVec2 iMin = ImVec2(lMin.x + 8, selY);
        ImVec2 iMax = ImVec2(lMin.x + menuW - 8, selY + itemH - 4);
        drawList->AddRectFilledMultiColor(iMin, iMax, IM_COL32(110, 15, 20, (int)(200 * g_SouthAlpha)), IM_COL32(110, 15, 20, 0), IM_COL32(110, 15, 20, 0), IM_COL32(110, 15, 20, (int)(200 * g_SouthAlpha)));
        drawList->AddRectFilled(ImVec2(iMin.x, iMin.y), ImVec2(iMin.x + 3.0f, iMax.y), IM_COL32(180, 20, 25, (int)(255 * g_SouthAlpha)), 1.5f);
        drawList->AddRect(iMin, iMax, IM_COL32(180, 20, 25, (int)(100 * g_SouthAlpha)), 6.0f);
    }

    for (int i = 0; i < visible; i++) {
        int idx = displayStart + i;
        if (idx < 0 || idx >= (int)items.size()) continue;
        SouthItem& it = items[idx];
        ImVec2 iMin = ImVec2(lMin.x + 8, lMin.y + 4 + i * itemH);
        ImVec2 iMax = ImVec2(lMin.x + menuW - 8, iMin.y + itemH - 4);
        bool act = (idx == g_ItemIdx);
        
        if (it.type == "separator") {
            float lineW = 60.0f;
            ImVec2 ts = ImGui::CalcTextSize(it.label.c_str());
            float midX = lMin.x + menuW * 0.5f;
            float midY = iMin.y + (itemH - 8) * 0.5f;
            drawList->AddLine(ImVec2(midX - ts.x * 0.5f - 10 - lineW, midY), ImVec2(midX - ts.x * 0.5f - 10, midY), IM_COL32(255, 255, 255, 200), 2.0f);
            drawList->AddLine(ImVec2(midX + ts.x * 0.5f + 10, midY), ImVec2(midX + ts.x * 0.5f + 10 + lineW, midY), IM_COL32(255, 255, 255, 200), 2.0f);
            drawList->AddText(ImVec2(midX - ts.x * 0.5f, midY - ts.y * 0.5f), textCol, it.label.c_str());
            continue;
        }

        ImVec2 tp = ImVec2(iMin.x + 10, iMin.y + (itemH - 8 - 16)*0.5f);
        if (it.label.size() >= 3 && ((unsigned char)it.label[0] == 0xef || (unsigned char)it.label[0] == 0xee)) {
            std::string iconStr = it.label.substr(0, 3);
            std::string textStr = it.label.substr(3);
            
            // Strip leading spaces from the text so it aligns perfectly
            size_t firstNotSpace = textStr.find_first_not_of(" ");
            std::string cleanText = (firstNotSpace != std::string::npos) ? textStr.substr(firstNotSpace) : textStr;

            ImGui::PushFont(ImGui::GetIO().Fonts->Fonts[2]); // Smaller Segoe UI + FontAwesome (15.0f)
            float iconW = ImGui::CalcTextSize(iconStr.c_str()).x;
            // Center the icon inside a 20px wide grid column
            float centeredIconX = tp.x + (20.0f - iconW) * 0.5f;
            drawList->AddText(ImVec2(centeredIconX, tp.y + 1.5f), IM_COL32(255, 255, 255, (int)(255 * g_SouthAlpha)), iconStr.c_str());
            ImGui::PopFont();
            
            // Draw all submenu texts exactly aligned at 28px offset
            drawList->AddText(ImVec2(tp.x + 28.0f, tp.y), act ? textCol : textDimCol, cleanText.c_str());
        } else {
            drawList->AddText(tp, act ? textCol : textDimCol, it.label.c_str());
        }

        // Right-side controls
        float rightX = iMax.x - 10;
        if (it.type == "check" || it.type == "slider-check") {
            float tw = 34.0f, th = 18.0f;
            ImVec2 tPos = ImVec2(rightX - tw, iMin.y + (itemH - 8 - th) * 0.5f);
            
            if (it.type == "slider-check") {
                float sw = 60.0f;
                ImVec2 sPos = ImVec2(tPos.x - sw - 10, iMin.y + (itemH - 8 - 4.0f) * 0.5f);
                drawList->AddRectFilled(sPos, ImVec2(sPos.x + sw, sPos.y + 4.0f), IM_COL32(40, 40, 40, (int)(255 * g_SouthAlpha)), 2.0f);
                float mVal = (it.label == "Noclip") ? 10.0f : 100.0f;
                float progress = (std::max)(0.0f, (std::min)(1.0f, it.val / mVal));
                drawList->AddRectFilled(sPos, ImVec2(sPos.x + sw * progress, sPos.y + 4.0f), accentCol, 2.0f);
                drawList->AddCircleFilled(ImVec2(sPos.x + sw * progress, sPos.y + 2.0f), 4.0f, textCol);
                
                char valBuf[16]; sprintf_s(valBuf, "%.0f", it.val);
                ImVec2 vs = ImGui::CalcTextSize(valBuf);
                drawList->AddText(ImVec2(sPos.x - vs.x - 8, tp.y), textDimCol, valBuf);
            }

            // Toggle Animation
            static std::map<std::string, float> toggleAnims;
            bool isPlayerListItem = (g_ActiveSub == 1 && g_ActiveCat == 0);
            bool isVehicleListItem = (g_ActiveSub == 2 && g_ActiveCat == 0);
            std::string animKey = ((isPlayerListItem || isVehicleListItem) && !it.subVal.empty()) ? it.subVal : it.label;
            float& anim = toggleAnims[animKey];
            anim = ImLerp(anim, it.checked ? 1.0f : 0.0f, io.DeltaTime * 12.0f);

            // Draw vehicle icon if player is inside a vehicle
            if (isPlayerListItem && !it.subVal.empty()) {
                bool playerInVeh = false;
                std::string targetPlayerName = it.subVal;
                PlayerListData targetPlayerInfo;
                bool foundPlayer = false;
                {
                    SRWLockGuard lock(&g_PlayerListLock);
                    for (const auto& p : g_PlayerList) {
                        if (p.name == targetPlayerName) {
                            targetPlayerInfo = p;
                            foundPlayer = true;
                            break;
                        }
                    }
                }

                if (foundPlayer && targetPlayerInfo.ped) {
                    uintptr_t vehiclePtr = Address::Get()->SafeRead<uintptr_t>(targetPlayerInfo.ped + 0x14D0);
                    if (!Address::Get()->IsValid(vehiclePtr)) {
                        vehiclePtr = Address::Get()->SafeRead<uintptr_t>(targetPlayerInfo.ped + 0x14C0);
                    }
                    if (Address::Get()->IsValid(vehiclePtr)) {
                        playerInVeh = true;
                    } else {
                        // Delta speed calculations
                        static std::map<std::string, Vector3> lastPositions;
                        static std::map<std::string, uint64_t> lastTimes;
                        static std::map<std::string, float> smoothedSpeeds;

                        Matrix4x4 matrix = Address::Get()->SafeRead<Matrix4x4>(targetPlayerInfo.ped + 0x60);
                        Vector3 currentPos = Vector3(matrix._41, matrix._42, matrix._43);
                        if (currentPos.x == 0) {
                            currentPos = Address::Get()->SafeRead<Vector3>(targetPlayerInfo.ped + Address::Get()->m_VisualPos);
                        }

                        float speedKmh = 0.0f;
                        uint64_t nowMs = GetTickCount64();
                        if (lastTimes.count(targetPlayerName) != 0) {
                            speedKmh = smoothedSpeeds[targetPlayerName];
                        }

                        if (currentPos.x != 0) {
                            float maxCheckDist = (speedKmh > 30.0f) ? 15.0f : 1.5f;
                            SRWLockGuard lockVeh(&g_VehicleListLock);
                            for (const auto& v : g_VehicleList) {
                                if (currentPos.Distance(v.pos) <= maxCheckDist) {
                                    playerInVeh = true;
                                    break;
                                }
                            }
                        }
                    }
                }

                if (playerInVeh) {
                    ImGui::PushFont(ImGui::GetIO().Fonts->Fonts[2]);
                    drawList->AddText(ImVec2(tPos.x - 22.0f, tp.y + 1.5f), IM_COL32(230, 20, 30, (int)(255 * g_SouthAlpha)), ICON_FA_CAR);
                    ImGui::PopFont();
                }
            }

            ImU32 tCol = IM_COL32(30 + (int)(150 * anim), 30 - (int)(10 * anim), 30 - (int)(5 * anim), (int)(255 * g_SouthAlpha));
            drawList->AddRectFilled(tPos, ImVec2(tPos.x + tw, tPos.y + th), tCol, th * 0.5f);
            drawList->AddCircleFilled(ImVec2(tPos.x + th*0.5f + anim * (tw - th), tPos.y + th*0.5f), th*0.5f - 2, IM_COL32(255, 255, 255, (int)(255 * g_SouthAlpha)));

            if (it.subVal != "" && it.subVal != "Safe" && it.type != "slider-check" && it.type != "scroll-check" && !isPlayerListItem && !isVehicleListItem) {
                std::string drawVal = it.subVal;
                if (it.type == "scroll") {
                    drawVal = "< " + it.subVal + " >";
                }
                ImVec2 svs = ImGui::CalcTextSize(drawVal.c_str());
                drawList->AddText(ImVec2(tPos.x - svs.x - 10, tp.y), act ? textCol : textDimCol, drawVal.c_str());
            }
        } else if (it.type == "scroll" || it.type == "scroll-check") {
            float tw = 34.0f, th = 18.0f;
            ImVec2 tPos = ImVec2(rightX - tw, iMin.y + (itemH - 8 - th) * 0.5f);
            
            if (it.type == "scroll-check") {
                // Render Toggle Switch
                static std::map<std::string, float> toggleAnims;
                float& anim = toggleAnims[it.label];
                anim = ImLerp(anim, it.checked ? 1.0f : 0.0f, io.DeltaTime * 12.0f);
                ImU32 tCol = IM_COL32(30 + (int)(150 * anim), 30 - (int)(10 * anim), 30 - (int)(5 * anim), (int)(255 * g_SouthAlpha));
                drawList->AddRectFilled(tPos, ImVec2(tPos.x + tw, tPos.y + th), tCol, th * 0.5f);
                drawList->AddCircleFilled(ImVec2(tPos.x + th*0.5f + anim * (tw - th), tPos.y + th*0.5f), th*0.5f - 2, IM_COL32(255, 255, 255, (int)(255 * g_SouthAlpha)));
            } else {
                // If pure scroll, move tPos back so text is at the edge
                tPos.x += tw;
            }

            // Render Arrow Selection
            if (it.subVal != "") {
                std::string drawVal = "< " + it.subVal + " >";
                ImVec2 svs = ImGui::CalcTextSize(drawVal.c_str());
                drawList->AddText(ImVec2(tPos.x - svs.x - 10, tp.y), act ? textCol : textDimCol, drawVal.c_str());
            }
        } else if (it.type == "slider") {
            float sw = 60.0f;
            float barH = 5.0f;
            float thumbR = 6.0f;
            ImVec2 sPos = ImVec2(rightX - sw, iMin.y + (itemH - 8) * 0.5f + 4.0f);
            float mVal = (it.label == "Noclip") ? 10.0f : 100.0f;
            float progress = ImClamp(it.val / mVal, 0.0f, 1.0f);
            // Track (dark)
            drawList->AddRectFilled(ImVec2(sPos.x, sPos.y - barH*0.5f), ImVec2(sPos.x + sw, sPos.y + barH*0.5f), IM_COL32(25, 25, 30, (int)(255*g_SouthAlpha)), barH*0.5f);
            // Fill (red)
            if (progress > 0.001f)
                drawList->AddRectFilled(ImVec2(sPos.x, sPos.y - barH*0.5f), ImVec2(sPos.x + sw*progress, sPos.y + barH*0.5f), IM_COL32(210, 0, 20, (int)(255*g_SouthAlpha)), barH*0.5f);
            // Thumb
            drawList->AddCircleFilled(ImVec2(sPos.x + sw*progress, sPos.y), thumbR, IM_COL32(255, 255, 255, (int)(255*g_SouthAlpha)));
            drawList->AddCircle(ImVec2(sPos.x + sw*progress, sPos.y), thumbR + 0.8f, IM_COL32(20, 20, 25, (int)(200*g_SouthAlpha)), 0, 1.5f);
            // Value text
            char valBuf[16]; sprintf_s(valBuf, "%.0f", it.val);
            ImVec2 vs = ImGui::CalcTextSize(valBuf);
            drawList->AddText(ImVec2(sPos.x - vs.x - 8, tp.y), textDimCol, valBuf);
        } else if (it.type == "subMenu") {
            drawList->AddText(ImVec2(rightX - 10, tp.y), textDimCol, ">");
        }

        // Keyboard icon for items that support keybinding
        {
            static const char* kbItems[] = {
                "Noclip", "Freecam", "Revive", "Health", "Armour",
                "Refill Health", "Refill Armour", "Godmode", "Invisibility",
                "Fast Run", "Super Jump", "Invincibility",
                "Self Crash", "Clear Screen Effects", "Suicide",
                "Force Ragdoll", "Remove Crutch", "Clear Tasks",
                "Infinity Stamina", "Tiny Ped", "Super Punch"
            };
            bool showKb = false;
            for (const auto& kb : kbItems) {
                if (it.label == kb) { showKb = true; break; }
            }

            if (showKb) {
                const char* kbIcon = "\xef\x84\x9c"; // FontAwesome keyboard U+F11C
                ImGui::PushFont(ImGui::GetIO().Fonts->Fonts[2]); // 15.0f FA font
                ImVec2 kbSize = ImGui::CalcTextSize(kbIcon);
                float kbY = iMin.y + (itemH - 8 - kbSize.y) * 0.5f;
                float kbX = 0;

                if (it.type == "check" || it.type == "slider-check") {
                    // Left of the toggle switch (toggle is at rightX - 34)
                    float toggleLeft = rightX - 34.0f;
                    if (it.type == "slider-check") {
                        // slider-check has slider + toggle, put icon left of slider value text
                        float sw = 60.0f;
                        kbX = toggleLeft - sw - 10 - 30 - kbSize.x - 14;
                    } else {
                        kbX = toggleLeft - kbSize.x - 16;
                    }
                } else if (it.type == "scroll-check") {
                    // Right of the < > arrows: toggle is at rightX - 34, arrows are to its left
                    // Put icon to the LEFT of the arrow text
                    float toggleLeft = rightX - 34.0f;
                    if (it.subVal != "") {
                        std::string drawVal = "< " + it.subVal + " >";
                        ImVec2 svs = ImGui::CalcTextSize(drawVal.c_str());
                        kbX = toggleLeft - svs.x - 10 - kbSize.x - 14;
                    } else {
                        kbX = toggleLeft - kbSize.x - 16;
                    }
                } else if (it.type == "slider") {
                    // Left of the slider value text
                    float sw = 60.0f;
                    float sliderLeft = rightX - sw;
                    char valBuf2[16]; sprintf_s(valBuf2, "%.0f", it.val);
                    ImVec2 vs2 = ImGui::CalcTextSize(valBuf2);
                    kbX = sliderLeft - vs2.x - 8 - kbSize.x - 14;
                } else {
                    // button: place at the same position as a toggle would be
                    kbX = rightX - kbSize.x - 8;
                }

                drawList->AddText(ImGui::GetIO().Fonts->Fonts[2], 15.0f, ImVec2(kbX, kbY), IM_COL32(255, 255, 255, (int)(80 * g_SouthAlpha)), kbIcon);
                ImGui::PopFont();
            }
        }
    }

    // 6. Footer
    float footH = 36.0f;
    ImVec2 fMin = ImVec2(menuX, lMax.y);
    ImVec2 fMax = ImVec2(menuX + menuW, fMin.y + footH);
    drawList->AddRectFilled(fMin, fMax, IM_COL32(0, 0, 0, (int)(255 * g_SouthAlpha)), 4.0f, ImDrawCornerFlags_Bot);
    
    if (tex_south_logo) {
        float logoSize = 28.0f; // Pre-scaled 28x28 Sharp Clean Logo (increased from 22.0f)
        drawList->AddImage(tex_south_logo, ImVec2(fMin.x + 10, fMin.y + (footH - logoSize) * 0.5f + 2.0f), ImVec2(fMin.x + 10 + logoSize, fMin.y + (footH + logoSize) * 0.5f + 2.0f), ImVec2(0, 0), ImVec2(1, 1), IM_COL32(255, 255, 255, (int)(255 * g_SouthAlpha)));
    } else {
        drawList->AddText(ImVec2(fMin.x + 15, fMin.y + (footH - 18)*0.5f), textCol, "Z"); 
    }
    drawList->AddText(ImVec2(fMin.x + 46.0f, fMin.y + (footH - 16)*0.5f), textCol, "South Menu");
    
    char idxBuf[32]; sprintf_s(idxBuf, "(%d/%d)", numItems ? g_ItemIdx + 1 : 0, numItems);
    drawList->AddText(ImVec2(fMax.x - ImGui::CalcTextSize(idxBuf).x - 15, fMin.y + (footH - 16)*0.5f), textCol, idxBuf);

    if (!g_ShowSouthMenu && g_SouthAlpha < 0.01f) return;

    // 7. Fodo Style Scrollbar (Full height, thin, red)
    float scW = 6.0f;
    ImVec2 sMin = ImVec2(menuX + menuW + 8, bMax.y);
    ImVec2 sMax = ImVec2(sMin.x + scW, fMax.y);
    drawList->AddRectFilled(sMin, sMax, IM_COL32(0, 0, 0, (int)(255 * g_SouthAlpha)), 3.0f);
    
    float totalH = sMax.y - sMin.y;
    float thumbH = (std::max)(20.0f, (std::min)(totalH, ((float)maxV / (float)(numItems ? numItems : 1)) * totalH));
    
    // Smooth Scrollbar Progress
    static float smoothProgress = 0.0f;
    float targetProgress = numItems > 1 ? (float)g_ItemIdx / (float)(numItems - 1) : 0.0f;
    smoothProgress = ImLerp(smoothProgress, targetProgress, io.DeltaTime * 14.0f);

    float hPos = sMin.y + smoothProgress * (totalH - thumbH);
    drawList->AddRectFilled(ImVec2(sMin.x + 1, hPos), ImVec2(sMax.x - 1, hPos + thumbH), IM_COL32(180, 20, 25, (int)(255 * g_SouthAlpha)), 2.0f);

    // 8. Player Info Overlay (on the right side)
    std::string targetPlayerName = "";
    if (g_ActiveSub == 1 && g_ActiveCat == 0 && g_ItemIdx >= 4) {
        auto& listItems = g_MenuData[1].categories[0].items;
        if (g_ItemIdx < (int)listItems.size()) {
            targetPlayerName = listItems[g_ItemIdx].subVal; // original player name
        }
    }
    if (targetPlayerName.empty()) {
        SRWLockGuard lock(&g_PlayerListLock);
        for (const auto& player : g_PlayerList) {
            if (g_PlayerListToggles[player.name]) {
                targetPlayerName = player.name;
                break;
            }
        }
    }

    if (!targetPlayerName.empty()) {
        PlayerListData targetPlayerInfo;
        bool foundPlayer = false;
        {
            SRWLockGuard lock(&g_PlayerListLock);
            for (const auto& p : g_PlayerList) {
                if (p.name == targetPlayerName) {
                    targetPlayerInfo = p;
                    foundPlayer = true;
                    break;
                }
            }
        }

        if (foundPlayer) {
            // Read Stats
            float speedKmh = 0.0f;
            float hpPercent = 0.0f;
            float armor = 0.0f;
            char weaponName[64] = "Unknown";
            std::string vehName = "None";

            if (targetPlayerInfo.ped) {
                // Speed calculated via delta position over time (works for noclip, driving, walking)
                static std::map<std::string, Vector3> lastPositions;
                static std::map<std::string, uint64_t> lastTimes;
                static std::map<std::string, float> smoothedSpeeds;

                Matrix4x4 matrix = Address::Get()->SafeRead<Matrix4x4>(targetPlayerInfo.ped + 0x60);
                Vector3 currentPos = Vector3(matrix._41, matrix._42, matrix._43);
                if (currentPos.x == 0) {
                    currentPos = Address::Get()->SafeRead<Vector3>(targetPlayerInfo.ped + Address::Get()->m_VisualPos);
                }

                uint64_t nowMs = GetTickCount64();
                if (lastTimes.count(targetPlayerName) == 0) {
                    lastPositions[targetPlayerName] = currentPos;
                    lastTimes[targetPlayerName] = nowMs;
                    smoothedSpeeds[targetPlayerName] = 0.0f;
                } else {
                    uint64_t dtMs = nowMs - lastTimes[targetPlayerName];
                    if (dtMs >= 100) {
                        float dist = currentPos.Distance(lastPositions[targetPlayerName]);
                        float speedMs = dist / ((float)dtMs / 1000.0f);
                        float calculatedSpeed = speedMs * 3.6f;
                        if (dist < 150.0f) {
                            smoothedSpeeds[targetPlayerName] = calculatedSpeed;
                        }
                        lastPositions[targetPlayerName] = currentPos;
                        lastTimes[targetPlayerName] = nowMs;
                    }
                }
                speedKmh = smoothedSpeeds[targetPlayerName];

                // Health
                float health = Address::Get()->SafeRead<float>(targetPlayerInfo.ped + Address::Get()->m_Health);
                float maxHealth = Address::Get()->SafeRead<float>(targetPlayerInfo.ped + Address::Get()->m_MaxHealth);
                if (maxHealth <= 1.0f) maxHealth = Address::Get()->SafeRead<float>(targetPlayerInfo.ped + 0x2A0);
                if (maxHealth <= 1.0f) maxHealth = Address::Get()->SafeRead<float>(targetPlayerInfo.ped + 0x284);
                if (maxHealth <= 0) maxHealth = 100.0f;
                
                if (maxHealth > 120.0f && health > 100.0f) {
                    health -= 100.0f;
                    maxHealth -= 100.0f;
                }
                hpPercent = (health / maxHealth) * 100.0f;
                if (hpPercent < 0.0f) hpPercent = 0.0f;
                if (hpPercent > 100.0f) hpPercent = 100.0f;

                // Armor
                armor = Address::Get()->SafeRead<float>(targetPlayerInfo.ped + Address::Get()->m_Armor);
                if (armor < 0.0f) armor = 0.0f;
                if (armor > 100.0f) armor = 100.0f;

                // Weapon
                ESP::GetWeaponName(targetPlayerInfo.ped, weaponName, sizeof(weaponName));

                // Vehicle Proximity Check
                uintptr_t vehiclePtr = Address::Get()->SafeRead<uintptr_t>(targetPlayerInfo.ped + 0x14D0);
                if (!Address::Get()->IsValid(vehiclePtr)) {
                    vehiclePtr = Address::Get()->SafeRead<uintptr_t>(targetPlayerInfo.ped + 0x14C0);
                }
                
                if (Address::Get()->IsValid(vehiclePtr)) {
                    uintptr_t modelInfo = Address::Get()->SafeRead<uintptr_t>(vehiclePtr + Address::Get()->m_VehModelInfo);
                    if (Address::Get()->IsValid(modelInfo)) {
                        const char* vname = (const char*)(modelInfo + 0x298);
                        if (vname && vname[0] != '\0') {
                            vehName = vname;
                        } else {
                            vehName = "Unknown Vehicle";
                        }
                    }
                }

                // Proximity fallback if native pointer is zero or invalid
                if (vehName == "None" || vehName == "Unknown Vehicle") {
                    Matrix4x4 targetMatrix = Address::Get()->SafeRead<Matrix4x4>(targetPlayerInfo.ped + 0x60);
                    Vector3 targetPos = Vector3(targetMatrix._41, targetMatrix._42, targetMatrix._43);
                    if (targetPos.x == 0) {
                        targetPos = Address::Get()->SafeRead<Vector3>(targetPlayerInfo.ped + Address::Get()->m_VisualPos);
                    }

                    if (targetPos.x != 0) {
                        float maxCheckDist = (speedKmh > 30.0f) ? 15.0f : 1.5f;
                        float closestDist = 999999.0f;
                        std::string closestName = "";
                        
                        // Access the synced vehicle cache from ESP thread safely
                        SRWLockGuard lockVeh(&g_VehicleListLock);
                        for (const auto& v : g_VehicleList) {
                            float distToVeh = targetPos.Distance(v.pos);
                            if (distToVeh <= maxCheckDist && distToVeh < closestDist) {
                                closestDist = distToVeh;
                                closestName = v.name;
                            }
                        }
                        
                        if (!closestName.empty()) {
                            vehName = closestName;
                        }
                    }
                }
            }

            // Draw Overlay Card
            float cardW = 260.0f;
            float cardH = 220.0f;
            float cardX = menuX + menuW + 20.0f;
            float cardY = menuY;

            ImVec2 cMin = ImVec2(cardX, cardY);
            ImVec2 cMax = ImVec2(cardX + cardW, cardY + cardH);

            // Dark card background
            drawList->AddRectFilled(cMin, cMax, IM_COL32(8, 8, 8, (int)(220 * g_SouthAlpha)), 6.0f);
            drawList->AddRect(cMin, cMax, IM_COL32(180, 20, 25, (int)(100 * g_SouthAlpha)), 6.0f, 15, 1.2f);

            float curY = cardY + 15.0f;
            float paddingX = 15.0f;

            auto DrawRow = [&](const std::string& label, const std::string& val, ImU32 valCol = IM_COL32(255, 255, 255, 255), bool hasBar = false, ImU32 barCol = 0) {
                // Label (left)
                drawList->AddText(ImVec2(cardX + paddingX, curY), IM_COL32(180, 180, 185, (int)(255 * g_SouthAlpha)), label.c_str());
                
                // Value (right)
                float valX = cardX + cardW - paddingX - ImGui::CalcTextSize(val.c_str()).x;
                if (hasBar) valX -= 10.0f; // Offset to fit bar on far right
                drawList->AddText(ImVec2(valX, curY), valCol, val.c_str());

                if (hasBar) {
                    float barW = 4.0f;
                    float barH = 14.0f;
                    float bx = cardX + cardW - paddingX - barW;
                    float by = curY + 2.0f;
                    drawList->AddRectFilled(ImVec2(bx, by), ImVec2(bx + barW, by + barH), barCol, 1.0f);
                }
                curY += 24.0f;
            };

            DrawRow("Name", targetPlayerInfo.name);
            DrawRow("ID", std::to_string(targetPlayerInfo.serverId));
            
            char distStr[32];
            sprintf_s(distStr, "%.1f m", targetPlayerInfo.distance);
            DrawRow("Distance", distStr);

            char speedStr[32];
            sprintf_s(speedStr, "%.0f km/h", speedKmh);
            DrawRow("Speed", speedStr);

            char hpStr[32];
            sprintf_s(hpStr, "%.0f%%", hpPercent);
            DrawRow("Health", hpStr, IM_COL32(255, 255, 255, (int)(255 * g_SouthAlpha)), true, IM_COL32(230, 40, 40, (int)(255 * g_SouthAlpha)));

            char armorStr[32];
            sprintf_s(armorStr, "%.0f%%", armor);
            DrawRow("Armor", armorStr, IM_COL32(255, 255, 255, (int)(255 * g_SouthAlpha)), true, IM_COL32(58, 95, 205, (int)(255 * g_SouthAlpha)));

            DrawRow("Weapon", weaponName);
            DrawRow("Vehicle", vehName);
        }
    }
}


void ApplyNexusTheme()
{
    ImGuiStyle& style = ImGui::GetStyle();
    
    style.WindowRounding = 3.0f;
    style.ChildRounding = 3.0f;
    style.FrameRounding = 3.0f;
    style.ScrollbarRounding = 3.0f;
    style.WindowBorderSize = 0.0f;
    style.ChildBorderSize = 0.0f;
    style.FrameBorderSize = 0.0f;
    style.WindowPadding = ImVec2(0, 0); 
    style.ItemSpacing = ImVec2(0, 0);

    ImVec4* colors = style.Colors;
    colors[ImGuiCol_WindowBg] = ImVec4(0.0549f, 0.0549f, 0.0705f, 1.00f);
    colors[ImGuiCol_ChildBg] = ImVec4(0.0549f, 0.0549f, 0.0705f, 1.00f);
    colors[ImGuiCol_PopupBg] = ImVec4(0.06f, 0.06f, 0.06f, 1.00f);
    colors[ImGuiCol_Border] = ImVec4(0.10f, 0.10f, 0.10f, 1.00f);
    colors[ImGuiCol_FrameBg] = ImVec4(0.08f, 0.08f, 0.08f, 1.00f);
    colors[ImGuiCol_FrameBgHovered] = ImVec4(0.12f, 0.12f, 0.12f, 1.00f);
    colors[ImGuiCol_FrameBgActive] = ImVec4(0.15f, 0.15f, 0.15f, 1.00f);
    colors[ImGuiCol_ScrollbarBg] = ImVec4(0.00f, 0.00f, 0.00f, 0.00f);
    colors[ImGuiCol_ScrollbarGrab] = ImVec4(0.00f, 0.00f, 0.00f, 0.00f);
    colors[ImGuiCol_ScrollbarGrabHovered] = ImVec4(0.00f, 0.00f, 0.00f, 0.00f);
    colors[ImGuiCol_ScrollbarGrabActive] = ImVec4(0.00f, 0.00f, 0.00f, 0.00f);
    colors[ImGuiCol_Header] = ImVec4(0.00f, 0.00f, 0.00f, 0.00f);
    colors[ImGuiCol_HeaderHovered] = ImVec4(0.00f, 0.00f, 0.00f, 0.00f);
    colors[ImGuiCol_HeaderActive] = ImVec4(0.00f, 0.00f, 0.00f, 0.00f);
    colors[ImGuiCol_NavHighlight] = ImVec4(0.00f, 0.00f, 0.00f, 0.00f);
    
    style.ScrollbarSize = 2.0f;
    
    if (!editorInitialized) {
        auto lang = TextEditor::LanguageDefinition::Lua();
        editor.SetLanguageDefinition(lang);
        auto pal = TextEditor::GetDarkPalette();
        pal[(int)TextEditor::PaletteIndex::Background] = IM_COL32(20, 19, 22, 255);
        pal[(int)TextEditor::PaletteIndex::LineNumber] = IM_COL32(80, 80, 80, 255);
        
        // Setup dark line highlight
        pal[(int)TextEditor::PaletteIndex::CurrentLineFill] = IM_COL32(30, 29, 32, 255);
        pal[(int)TextEditor::PaletteIndex::CurrentLineFillInactive] = IM_COL32(30, 29, 32, 255);
        pal[(int)TextEditor::PaletteIndex::CurrentLineEdge] = IM_COL32(60, 60, 65, 255);
        
        editor.SetPalette(pal);
        editor.SetText("");
        editorInitialized = true;
    }
}

// Global State for Tab 2
static bool loggerEnabled = false;
static int currentSubTab = 0;
static int lastSubTab = 0;
static float resourceFade = 1.0f;
static float subTabSelectorX = -1.0f;
static float subTabSelectorW = 0.0f;
static char resourceSearch[128] = "";
static char eventName[128] = "";
static std::vector<std::string> g_BlockedEvents;
static std::string eventSelected = "";
static std::string rSelected = "";
static std::unordered_set<std::string> g_ManualStopped;
static std::chrono::steady_clock::time_point lastResourceRefresh;
static std::map<std::string, float> resAlphas;
static float dumpAlpha = 0.0f;
static float logToggleAlpha = 0.0f;
static float logClearAlpha = 0.0f;

static int optionsSubTab = 0;
static float optionsSubTabFade = 1.0f;
static int optionsLastSubTab = 0;
static float optSubSelectorX = -1.0f;
static float optSubSelectorW = 0.0f;

// --- Global State for Tab 4 (Scripts) ---
static int scriptsSubTab = 0;
static float scriptsSubTabFade = 1.0f;
static int scriptsLastSubTab = 0;
static float scriptSubSelectorX = -1.0f;
static float scriptSubSelectorW = 0.0f;

// --- Global State for Tab 5 (Settings) ---
static int settingsSubTab = 0;
static float settingsSubTabFade = 1.0f;
static int settingsLastSubTab = 0;
static float settingsSubSelectorX = -1.0f;
static float settingsSubSelectorW = 0.0f;

static bool set_BlockInput = false;
static bool set_MenuBypass = true;
static int set_MenuKey = 0;
static bool g_BlockInputFocusActive = false;

static void UpdateBlockInputFocusState(bool menuOpen) {
    if (!window) return;

    if (set_BlockInput && menuOpen) {
        if (!g_BlockInputFocusActive) {
            SetForegroundWindow(window);
            SetFocus(window);
            g_BlockInputFocusActive = true;
        }
    } else {
        if (g_BlockInputFocusActive) {
            g_BlockInputFocusActive = false;
        }
    }
}

// Aimbot General
bool aim_Enable = false;
int aim_Key = 0;
bool aim_ShowFov = false;
float col_aim_Fov[4] = { 1.0f, 1.0f, 1.0f, 1.0f };
float aim_Fov = 50000.0f;
float aim_Smoothing = 0.000f;
int aim_BoneIdx = 0;
bool aim_VisibleCheck = false;
bool aim_ExcludeDeads = false;
bool aim_TargetNpcs = false;
bool aim_TargetAdmins = false;

// Silent Aim
bool silent_Enable = false;
int silent_Key = 0;
bool silent_MagicBullet = false;
bool silent_ShowFov = false;
float col_silent_Fov[4] = { 0.2f, 0.5f, 1.0f, 1.0f };
float silent_Fov = 50000.0f;

// Weapon Modifiers
static bool wep_NoRecoil = false;
static bool wep_NoSpread = false;
static bool wep_InfRange = false;
bool wep_ShowCrosshair = false;


// ESP Options
bool esp_Enable = false;
bool esp_Box = false;
bool esp_Names = false;
bool esp_Weapons = false;
bool esp_Head = false;
bool esp_Skeleton = false;
bool esp_Healthbar = false;
bool esp_Armorbar = false;
bool esp_Lines = false;

bool admin_List = false;
bool admin_Lines = false;
bool admin_Skeleton = false;

float esp_Distance = 1000.0f;
bool esp_InfDistance = false;
bool esp_ShowDistance = false;
bool esp_TargetNpcs = false;
bool esp_Self = false;
bool esp_VisibleCheck = false;
float esp_TextSize = 15.0f;

float col_Skeleton[4] = { 1.0f, 1.0f, 1.0f, 1.0f };
float col_InvSkeleton[4] = { 1.0f, 0.0f, 0.0f, 1.0f };
float col_FriendSkeleton[4] = { 0.2f, 0.5f, 1.0f, 1.0f };
float col_Box[4] = { 0.0f, 0.7f, 1.0f, 1.0f };
float col_BoxFill[4] = { 1.0f, 1.0f, 1.0f, 0.25f };

bool veh_Enable = false;
bool veh_Name = false;
bool veh_Marker = false;
bool veh_LockState = false;
bool veh_Lines = false;
float veh_Distance = 1000.0f;
bool veh_InfDistance = false;
bool veh_ShowDistance = false;
float veh_TextSize = 15.0f;


static std::map<std::string, float> optAnims;
static std::map<std::string, bool> comboOpen;
static std::map<std::string, float> comboAnims;
static bool anyComboOpen = false;
static ImVec2 comboDropMin, comboDropMax;
static std::map<std::string, bool> isBinding;
static std::map<std::string, float> bindAnims;
static std::map<std::string, float> scrollTarget;
static std::map<std::string, float> scrollCurrent;
static float aimTabAlpha = 1.0f;
static float adminListAlpha = 0.0f;
static float adminListHeight = 0.0f;
static float adminListContentAlpha = 0.0f;
std::map<std::string, bool> g_PlayerListToggles;
std::map<uintptr_t, bool> g_VehicleListToggles;

#pragma pack(push, 1)
struct MenuConfigStruct {
    char signature[4] = {'Z', 'E', 'Y', 'C'};
    int version = 2;

    // Settings
    bool blockInput = false;
    bool menuBypass = true;
    int menuKey = VK_INSERT;

    // Aimbot General
    bool aim_Enable = false;
    int aim_Key = 0;
    bool aim_ShowFov = false;
    float aim_Fov = 50.0f;
    float aim_Smoothing = 0.0f;
    int aim_BoneIdx = 0;
    bool aim_VisibleCheck = false;
    bool aim_ExcludeDeads = false;
    bool aim_TargetNpcs = false;
    bool aim_TargetAdmins = false;

    // Silent Aim
    bool silent_Enable = false;
    int silent_Key = 0;
    bool silent_MagicBullet = false;
    bool silent_ShowFov = false;
    float silent_Fov = 50.0f;

    // Weapon Modifiers
    bool wep_NoRecoil = false;
    bool wep_NoSpread = false;
    bool wep_InfRange = false;
    bool wep_ShowCrosshair = false;

    // ESP Options
    bool esp_Enable = false;
    bool esp_Box = false;
    bool esp_Names = false;
    bool esp_Weapons = false;
    bool esp_Head = false;
    bool esp_Skeleton = false;
    bool esp_Healthbar = false;
    bool esp_Armorbar = false;
    bool esp_Lines = false;

    bool admin_List = false;
    bool admin_Lines = false;
    bool admin_Skeleton = false;

    float esp_Distance = 1000.0f;
    bool esp_InfDistance = false;
    bool esp_ShowDistance = false;
    bool esp_TargetNpcs = false;
    bool esp_Self = false;
    bool esp_VisibleCheck = false;
    float esp_TextSize = 15.0f;

    // Vehicles ESP
    bool veh_Enable = false;
    bool veh_Name = false;
    bool veh_Marker = false;
    bool veh_LockState = false;
    bool veh_Lines = false;
    float veh_Distance = 1000.0f;
    bool veh_InfDistance = false;
    bool veh_ShowDistance = false;
    float veh_TextSize = 15.0f;

    // Colors (R, G, B, A)
    float col_aim_Fov[4] = { 1.0f, 1.0f, 1.0f, 1.0f };
    float col_silent_Fov[4] = { 0.2f, 0.5f, 1.0f, 1.0f };
    float col_Skeleton[4] = { 1.0f, 1.0f, 1.0f, 1.0f };
    float col_InvSkeleton[4] = { 1.0f, 0.0f, 0.0f, 1.0f };
    float col_FriendSkeleton[4] = { 0.2f, 0.5f, 1.0f, 1.0f };
    float col_Box[4] = { 0.0f, 0.7f, 1.0f, 1.0f };
    float col_BoxFill[4] = { 1.0f, 1.0f, 1.0f, 0.25f };
};
#pragma pack(pop)

void SaveConfig() {
    FILE* f = nullptr;
    if (fopen_s(&f, "C:\\Users\\South\\Downloads\\config.dat", "wb") == 0 && f) {
        MenuConfigStruct cfg;
        cfg.blockInput = set_BlockInput;
        cfg.menuBypass = set_MenuBypass;
        cfg.menuKey = set_MenuKey;

        cfg.aim_Enable = aim_Enable;
        cfg.aim_Key = aim_Key;
        cfg.aim_ShowFov = aim_ShowFov;
        cfg.aim_Fov = aim_Fov;
        cfg.aim_Smoothing = aim_Smoothing;
        cfg.aim_BoneIdx = aim_BoneIdx;
        cfg.aim_VisibleCheck = aim_VisibleCheck;
        cfg.aim_ExcludeDeads = aim_ExcludeDeads;
        cfg.aim_TargetNpcs = aim_TargetNpcs;
        cfg.aim_TargetAdmins = aim_TargetAdmins;

        cfg.silent_Enable = silent_Enable;
        cfg.silent_Key = silent_Key;
        cfg.silent_MagicBullet = silent_MagicBullet;
        cfg.silent_ShowFov = silent_ShowFov;
        cfg.silent_Fov = silent_Fov;

        cfg.wep_NoRecoil = wep_NoRecoil;
        cfg.wep_NoSpread = wep_NoSpread;
        cfg.wep_InfRange = wep_InfRange;
        cfg.wep_ShowCrosshair = wep_ShowCrosshair;

        cfg.esp_Enable = esp_Enable;
        cfg.esp_Box = esp_Box;
        cfg.esp_Names = esp_Names;
        cfg.esp_Weapons = esp_Weapons;
        cfg.esp_Head = esp_Head;
        cfg.esp_Skeleton = esp_Skeleton;
        cfg.esp_Healthbar = esp_Healthbar;
        cfg.esp_Armorbar = esp_Armorbar;
        cfg.esp_Lines = esp_Lines;

        cfg.admin_List = admin_List;
        cfg.admin_Lines = admin_Lines;
        cfg.admin_Skeleton = admin_Skeleton;

        cfg.esp_Distance = esp_Distance;
        cfg.esp_InfDistance = esp_InfDistance;
        cfg.esp_ShowDistance = esp_ShowDistance;
        cfg.esp_TargetNpcs = esp_TargetNpcs;
        cfg.esp_Self = esp_Self;
        cfg.esp_VisibleCheck = esp_VisibleCheck;
        cfg.esp_TextSize = esp_TextSize;

        cfg.veh_Enable = veh_Enable;
        cfg.veh_Name = veh_Name;
        cfg.veh_Marker = veh_Marker;
        cfg.veh_LockState = veh_LockState;
        cfg.veh_Lines = veh_Lines;
        cfg.veh_Distance = veh_Distance;
        cfg.veh_InfDistance = veh_InfDistance;
        cfg.veh_ShowDistance = veh_ShowDistance;
        cfg.veh_TextSize = veh_TextSize;

        memcpy(cfg.col_aim_Fov, col_aim_Fov, sizeof(col_aim_Fov));
        memcpy(cfg.col_silent_Fov, col_silent_Fov, sizeof(col_silent_Fov));
        memcpy(cfg.col_Skeleton, col_Skeleton, sizeof(col_Skeleton));
        memcpy(cfg.col_InvSkeleton, col_InvSkeleton, sizeof(col_InvSkeleton));
        memcpy(cfg.col_FriendSkeleton, col_FriendSkeleton, sizeof(col_FriendSkeleton));
        memcpy(cfg.col_Box, col_Box, sizeof(col_Box));
        memcpy(cfg.col_BoxFill, col_BoxFill, sizeof(col_BoxFill));

        fwrite(&cfg, sizeof(MenuConfigStruct), 1, f);
        fclose(f);
    }
}

void LoadConfig() {
    FILE* f = nullptr;
    if (fopen_s(&f, "C:\\Users\\South\\Downloads\\config.dat", "rb") == 0 && f) {
        MenuConfigStruct cfg;
        if (fread(&cfg, sizeof(MenuConfigStruct), 1, f) == 1) {
            // Validasi Signature
            if (cfg.signature[0] == 'Z' && cfg.signature[1] == 'E' && cfg.signature[2] == 'Y' && cfg.signature[3] == 'C') {
                set_BlockInput = cfg.blockInput;
                set_MenuBypass = cfg.menuBypass;
                set_MenuKey = cfg.menuKey;

                aim_Enable = cfg.aim_Enable;
                aim_Key = cfg.aim_Key;
                aim_ShowFov = cfg.aim_ShowFov;
                aim_Fov = cfg.aim_Fov;
                aim_Smoothing = cfg.aim_Smoothing;
                aim_BoneIdx = cfg.aim_BoneIdx;
                aim_VisibleCheck = cfg.aim_VisibleCheck;
                aim_ExcludeDeads = cfg.aim_ExcludeDeads;
                aim_TargetNpcs = cfg.aim_TargetNpcs;
                aim_TargetAdmins = cfg.aim_TargetAdmins;

                silent_Enable = cfg.silent_Enable;
                silent_Key = cfg.silent_Key;
                silent_MagicBullet = cfg.silent_MagicBullet;
                silent_ShowFov = cfg.silent_ShowFov;
                silent_Fov = cfg.silent_Fov;

                wep_NoRecoil = cfg.wep_NoRecoil;
                wep_NoSpread = cfg.wep_NoSpread;
                wep_InfRange = cfg.wep_InfRange;
                wep_ShowCrosshair = cfg.wep_ShowCrosshair;

                esp_Enable = cfg.esp_Enable;
                esp_Box = cfg.esp_Box;
                esp_Names = cfg.esp_Names;
                esp_Weapons = cfg.esp_Weapons;
                esp_Head = cfg.esp_Head;
                esp_Skeleton = cfg.esp_Skeleton;
                esp_Healthbar = cfg.esp_Healthbar;
                esp_Armorbar = cfg.esp_Armorbar;
                esp_Lines = cfg.esp_Lines;

                admin_List = cfg.admin_List;
                admin_Lines = cfg.admin_Lines;
                admin_Skeleton = cfg.admin_Skeleton;

                esp_Distance = cfg.esp_Distance;
                esp_InfDistance = cfg.esp_InfDistance;
                esp_ShowDistance = cfg.esp_ShowDistance;
                esp_TargetNpcs = cfg.esp_TargetNpcs;
                esp_Self = cfg.esp_Self;
                esp_VisibleCheck = cfg.esp_VisibleCheck;
                esp_TextSize = cfg.esp_TextSize;

                veh_Enable = cfg.veh_Enable;
                veh_Name = cfg.veh_Name;
                veh_Marker = cfg.veh_Marker;
                veh_LockState = cfg.veh_LockState;
                veh_Lines = cfg.veh_Lines;
                veh_Distance = cfg.veh_Distance;
                veh_InfDistance = cfg.veh_InfDistance;
                veh_ShowDistance = cfg.veh_ShowDistance;
                veh_TextSize = cfg.veh_TextSize;

                memcpy(col_aim_Fov, cfg.col_aim_Fov, sizeof(col_aim_Fov));
                memcpy(col_silent_Fov, cfg.col_silent_Fov, sizeof(col_silent_Fov));
                memcpy(col_Skeleton, cfg.col_Skeleton, sizeof(col_Skeleton));
                memcpy(col_InvSkeleton, cfg.col_InvSkeleton, sizeof(col_InvSkeleton));
                memcpy(col_FriendSkeleton, cfg.col_FriendSkeleton, sizeof(col_FriendSkeleton));
                memcpy(col_Box, cfg.col_Box, sizeof(col_Box));
                memcpy(col_BoxFill, cfg.col_BoxFill, sizeof(col_BoxFill));
            }
        }
        fclose(f);
    }
}

const char* GetKeyName(int vk) {
    if (vk == 0) return "N/A";
    if (vk == VK_LBUTTON) return "L.Mouse";
    if (vk == VK_RBUTTON) return "R.Mouse";
    if (vk == VK_MBUTTON) return "M.Mouse";
    if (vk == VK_XBUTTON1) return "X1.Mouse";
    if (vk == VK_XBUTTON2) return "X2.Mouse";
    
    static char name[32];
    UINT scanCode = MapVirtualKey(vk, MAPVK_VK_TO_VSC);
    if (GetKeyNameTextA(scanCode << 16, name, sizeof(name))) {
        return name;
    }

    static char fallback[16];
    sprintf_s(fallback, "%c", (char)vk);
    return fallback;
}

void DrawSouthKeybindSetupPopup() {
    ImGuiIO& io = ImGui::GetIO();
    ImDrawList* drawList = ImGui::GetBackgroundDrawList();
    ImVec2 screenSize = io.DisplaySize;

    // 1. Dim background
    drawList->AddRectFilled(ImVec2(0.0f, 0.0f), screenSize, IM_COL32(0, 0, 0, 110));

    // 2. Define popup size and center position
    float popupW = 320.0f;
    float popupH = 105.0f;
    float popupX = floorf((screenSize.x - popupW) * 0.5f);
    float popupY = floorf((screenSize.y - popupH) * 0.5f);
    
    ImVec2 pMin = ImVec2(popupX, popupY);
    ImVec2 pMax = ImVec2(popupX + popupW, popupY + popupH);

    // 3. Draw premium card background and outline
    drawList->AddRectFilled(pMin, pMax, IM_COL32(8, 8, 8, 255), 6.0f);
    drawList->AddRect(pMin, pMax, IM_COL32(255, 255, 255, 15), 6.0f, 0, 1.0f);

    // 4. Draw Header
    float headerH = 34.0f;

    // Logo box for keyboard icon
    float iconBoxSize = 20.0f;
    ImVec2 iconBoxMin = ImVec2(popupX + 10.0f, popupY + floorf((headerH - iconBoxSize) * 0.5f));
    ImVec2 iconBoxMax = ImVec2(iconBoxMin.x + iconBoxSize, iconBoxMin.y + iconBoxSize);
    drawList->AddRect(iconBoxMin, iconBoxMax, IM_COL32(255, 255, 255, 80), 2.0f);

    // Font Awesome Keyboard Icon
    ImGui::PushFont(io.Fonts->Fonts[2]); // 15px font
    ImVec2 iconS = ImGui::CalcTextSize(ICON_FA_KEYBOARD);
    ImVec2 iconPos = ImVec2(iconBoxMin.x + floorf((iconBoxSize - iconS.x) * 0.5f), iconBoxMin.y + floorf((iconBoxSize - iconS.y) * 0.5f));
    drawList->AddText(iconPos, IM_COL32(255, 255, 255, 255), ICON_FA_KEYBOARD);
    ImGui::PopFont();

    // Title: dynamic based on mode
    std::string popupTitle = g_KeybindSetupTarget.empty() ? "Set Open Key" : ("Set Keybind: " + g_KeybindSetupTarget);
    ImGui::PushFont(io.Fonts->Fonts[0]); // 18px font
    ImVec2 titlePos = ImVec2(iconBoxMax.x + 8.0f, popupY + floorf((headerH - ImGui::GetFontSize()) * 0.5f));
    drawList->AddText(titlePos, IM_COL32(230, 230, 230, 255), popupTitle.c_str());
    ImGui::PopFont();

    // Red theme line separator under header
    drawList->AddLine(ImVec2(popupX, popupY + headerH), ImVec2(popupX + popupW, popupY + headerH), IM_COL32(180, 20, 25, 255), 1.5f);

    // 5. Draw Input Box inside Body
    float boxW = popupW - 20.0f;
    float boxH = 34.0f;
    ImVec2 boxMin = ImVec2(popupX + 10.0f, popupY + headerH + 10.0f);
    ImVec2 boxMax = ImVec2(boxMin.x + boxW, boxMin.y + boxH);

    drawList->AddRectFilled(boxMin, boxMax, IM_COL32(20, 20, 20, 255), 4.0f);
    drawList->AddRect(boxMin, boxMax, IM_COL32(255, 255, 255, 10), 4.0f);

    std::string keyText = "Press any key...";
    ImU32 textCol = IM_COL32(150, 150, 150, 255);

    if (g_TempSouthKey != 0) {
        keyText = GetKeyName(g_TempSouthKey);
        textCol = IM_COL32(255, 255, 255, 255);
    }

    ImGui::PushFont(io.Fonts->Fonts[0]); // 18px font
    ImVec2 textS = ImGui::CalcTextSize(keyText.c_str());
    ImVec2 textPos = ImVec2(boxMin.x + 10.0f, boxMin.y + floorf((boxH - textS.y) * 0.5f));
    drawList->AddText(textPos, textCol, keyText.c_str());

    // Blinking cursor | if waiting for key input
    if (g_TempSouthKey == 0) {
        static float cursorTimer = 0.0f;
        cursorTimer += io.DeltaTime;
        if (fmodf(cursorTimer, 1.0f) < 0.5f) {
            drawList->AddLine(ImVec2(textPos.x + textS.x + 1.0f, textPos.y), ImVec2(textPos.x + textS.x + 1.0f, textPos.y + textS.y), IM_COL32(150, 150, 150, 255), 1.5f);
        }
    }
    ImGui::PopFont();

    // 6. Draw White "Enter to apply" centered below the box
    if (g_TempSouthKey != 0) {
        ImGui::PushFont(io.Fonts->Fonts[2]); // 15px font
        ImVec2 helperS = ImGui::CalcTextSize("Enter to apply");
        float helperY = boxMax.y + floorf((popupH - (headerH + 10.0f + boxH) - helperS.y) * 0.5f);
        ImVec2 helperPos = ImVec2(popupX + floorf((popupW - helperS.x) * 0.5f), helperY);
        drawList->AddText(helperPos, IM_COL32(255, 255, 255, 255), "Enter to apply");
        ImGui::PopFont();
    }

    // 7. Dynamic Keyboard Input listener
    if (GetForegroundWindow() == window) {
        for (int i = 1; i < 256; i++) {
            if (i == VK_RETURN) {
                // Only allow apply if a temporary key is recorded, and 300ms has elapsed since setup opened (prevents instant activation from Enter click)
                if ((GetAsyncKeyState(i) & 0x8000) && g_TempSouthKey != 0 && (GetTickCount64() - g_SouthKeybindSetupOpenTime > 300)) {
                    // Check if key is already in use
                    bool keyAlreadyInUse = false;
                    std::string inUseBy = "";
                    if (g_KeybindSetupTarget.empty()) {
                        for (const auto& [lbl, vk] : g_ItemKeybinds) {
                            if (vk == g_TempSouthKey) {
                                keyAlreadyInUse = true;
                                inUseBy = lbl;
                                break;
                            }
                        }
                    } else {
                        if (g_TempSouthKey == g_SouthToggleKey) {
                            keyAlreadyInUse = true;
                            inUseBy = "Menu Toggle";
                        } else {
                            for (const auto& [lbl, vk] : g_ItemKeybinds) {
                                if (lbl != g_KeybindSetupTarget && vk == g_TempSouthKey) {
                                    keyAlreadyInUse = true;
                                    inUseBy = lbl;
                                    break;
                                }
                            }
                        }
                    }

                    if (keyAlreadyInUse) {
                        PlayClickSound();
                        PushNotification("Keybind already used by: " + inUseBy + "!");
                        g_TempSouthKey = 0;
                    } else {
                        if (g_KeybindSetupTarget.empty()) {
                            // Menu toggle key mode
                            g_SouthToggleKey = g_TempSouthKey;
                            g_ShowSouthKeybindSetup = false;
                            g_SouthKeybindSetupCloseTime = GetTickCount64();
                            g_ShowSouthMenu = true;
                            
                            if (g_MenuData.size() > 6 && g_MenuData[6].categories.size() > 0 && g_MenuData[6].categories[0].items.size() > 0) {
                                g_MenuData[6].categories[0].items[0].subVal = GetKeyName(g_SouthToggleKey);
                            }
                            
                            PlayClickSound();
                            PushNotification("South Menu keybind set successfully!");
                        } else {
                            // Item keybind mode
                            g_ItemKeybinds[g_KeybindSetupTarget] = g_TempSouthKey;
                            g_ShowSouthKeybindSetup = false;
                            g_SouthKeybindSetupCloseTime = GetTickCount64();
                            g_ShowSouthMenu = true;
                            
                            PlayClickSound();
                            std::string msg = g_KeybindSetupTarget + " keybind set to " + GetKeyName(g_TempSouthKey) + "!";
                            PushNotification(msg);
                            g_KeybindSetupTarget = "";
                        }
                    }
                }
                continue;
            }
            // Skip mouse clicks
            if (i == VK_LBUTTON || i == VK_RBUTTON || i == VK_MBUTTON || i == VK_XBUTTON1 || i == VK_XBUTTON2) continue;
            // Prevent taking Executor keys
            if (i == VK_INSERT || i == set_MenuKey) continue;

            if (GetAsyncKeyState(i) & 0x8000) {
                g_TempSouthKey = i;
            }
        }
    }
}

void DrawVehicleSpawnerInputPopup() {
    ImGuiIO& io = ImGui::GetIO();
    ImDrawList* drawList = ImGui::GetBackgroundDrawList();
    ImVec2 screenSize = io.DisplaySize;

    // 1. Dim background
    drawList->AddRectFilled(ImVec2(0.0f, 0.0f), screenSize, IM_COL32(0, 0, 0, 110));

    // 2. Define popup size and center position
    float popupW = 320.0f;
    float popupH = 115.0f;
    float popupX = floorf((screenSize.x - popupW) * 0.5f);
    float popupY = floorf((screenSize.y - popupH) * 0.5f);
    
    ImVec2 pMin = ImVec2(popupX, popupY);
    ImVec2 pMax = ImVec2(popupX + popupW, popupY + popupH);

    // 3. Draw premium card background and outline
    drawList->AddRectFilled(pMin, pMax, IM_COL32(8, 8, 8, 255), 6.0f);
    drawList->AddRect(pMin, pMax, IM_COL32(255, 255, 255, 15), 6.0f, 0, 1.0f);

    // 4. Draw Header
    float headerH = 34.0f;
    drawList->AddLine(ImVec2(popupX, popupY + headerH), ImVec2(popupX + popupW, popupY + headerH), IM_COL32(180, 20, 25, 255), 1.5f);

    // Title
    ImGui::PushFont(io.Fonts->Fonts[0]); // 18px font
    ImVec2 titlePos = ImVec2(popupX + 15.0f, popupY + floorf((headerH - ImGui::GetFontSize()) * 0.5f));
    drawList->AddText(titlePos, IM_COL32(230, 230, 230, 255), "Spawn Vehicle by Model");
    ImGui::PopFont();

    // 5. Draw ImGui InputText inside Body
    ImGui::SetNextWindowPos(ImVec2(popupX + 10.0f, popupY + headerH + 10.0f));
    ImGui::SetNextWindowSize(ImVec2(popupW - 20.0f, 34.0f));
    ImGui::Begin("##veh_spawner_input_win", nullptr, ImGuiWindowFlags_NoDecoration | ImGuiWindowFlags_NoBackground | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoScrollWithMouse);
    
    ImGui::PushStyleColor(ImGuiCol_FrameBg, IM_COL32(20, 20, 20, 255));
    ImGui::PushStyleColor(ImGuiCol_Border, IM_COL32(255, 255, 255, 10));
    ImGui::PushStyleColor(ImGuiCol_Text, IM_COL32(255, 255, 255, 255));
    ImGui::PushItemWidth(popupW - 20.0f);
    
    // Auto focus on start
    static bool firstShow = true;
    if (firstShow) {
        ImGui::SetKeyboardFocusHere();
        firstShow = false;
    }

    if (ImGui::InputTextWithHint("##veh_model_input", "Enter model name...", g_VehicleSpawnerInputBuffer, sizeof(g_VehicleSpawnerInputBuffer), ImGuiInputTextFlags_EnterReturnsTrue)) {
        if (g_VehicleSpawnerInputBuffer[0] != '\0') {
            bool warp = g_MenuData[2].categories[1].items[10].checked;
            LuaCore::SpawnVehicle(g_VehicleSpawnerInputBuffer, warp);
            PushNotification("Spawn " + std::string(g_VehicleSpawnerInputBuffer) + " Success!");
            g_VehicleSpawnerInputBuffer[0] = '\0';
        }
        g_ShowVehicleSpawnerInput = false;
        g_VehicleSpawnerInputCloseTime = GetTickCount64();
        g_ShowSouthMenu = true;
        firstShow = true;
    }
    
    ImGui::PopItemWidth();
    ImGui::PopStyleColor(3);
    ImGui::End();

    // 6. Escape / cancel logic
    if (ImGui::IsKeyPressed(ImGuiKey_Escape)) {
        g_ShowVehicleSpawnerInput = false;
        g_VehicleSpawnerInputCloseTime = GetTickCount64();
        g_ShowSouthMenu = true;
        firstShow = true;
    }

    // Helper text
    ImGui::PushFont(io.Fonts->Fonts[2]); // 15px font
    ImVec2 helperS = ImGui::CalcTextSize("Press ENTER to spawn, ESC to cancel");
    ImVec2 helperPos = ImVec2(popupX + floorf((popupW - helperS.x) * 0.5f), popupY + headerH + 50.0f);
    drawList->AddText(helperPos, IM_COL32(150, 150, 150, 255), "Press ENTER to spawn, ESC to cancel");
    ImGui::PopFont();
}

void InitImGui()
{
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO();
    io.ConfigFlags = ImGuiConfigFlags_NoMouseCursorChange;
    // Load Windows default Segoe UI Font for accurate look
    ImFontConfig config;
    config.MergeMode = true;
    config.PixelSnapH = true;
    static const ImWchar ranges[] = { 0xe000, 0xf8ff, 0 }; // FontAwesome Solid ranges (expanded to load 0xe000+ glyphs like gun)
    
    // Load Font Awesome directly from memory (embedded in the DLL)
    io.Fonts->AddFontFromFileTTF("C:\\Windows\\Fonts\\segoeui.ttf", 18.0f);
    io.Fonts->AddFontFromMemoryTTF((void*)font_awesome_bytes, sizeof(font_awesome_bytes), 18.0f, &config, ranges);
    
    io.Fonts->AddFontFromFileTTF("C:\\Windows\\Fonts\\segoeui.ttf", 20.0f);
    io.Fonts->AddFontFromMemoryTTF((void*)font_awesome_bytes, sizeof(font_awesome_bytes), 20.0f, &config, ranges);
    
    io.Fonts->AddFontFromFileTTF("C:\\Windows\\Fonts\\segoeui.ttf", 15.0f);
    io.Fonts->AddFontFromMemoryTTF((void*)font_awesome_bytes, sizeof(font_awesome_bytes), 15.0f, &config, ranges);

    ImFontConfig boldConfig;
    boldConfig.PixelSnapH = true;
    io.Fonts->AddFontFromFileTTF("C:\\Windows\\Fonts\\seguisb.ttf", 18.0f, &boldConfig);

    io.Fonts->AddFontFromFileTTF("C:\\Windows\\Fonts\\segoeui.ttf", 18.0f);

    ImGui_ImplWin32_Init(window);
    ImGui_ImplDX11_Init(pDevice, pContext);

    ApplyNexusTheme();
}

LRESULT __stdcall WndProc(const HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam) {
    if (g_FreecamActive && uMsg == WM_MOUSEWHEEL) {
        static int wheelAccumulator = 0;
        static uint64_t lastScrollTime = 0;
        uint64_t currentTime = GetTickCount64();

        if (currentTime - lastScrollTime > 100) { // Reduced to 100ms for faster response
            wheelAccumulator += GET_WHEEL_DELTA_WPARAM(wParam);

            if (wheelAccumulator >= 100) {
                g_FreecamFeatureIdx--;
                if (g_FreecamFeatureIdx < 0) g_FreecamFeatureIdx = g_NumFreecamFeatures - 1;
                wheelAccumulator = 0;
                lastScrollTime = currentTime;
            } else if (wheelAccumulator <= -100) {
                g_FreecamFeatureIdx++;
                if (g_FreecamFeatureIdx >= g_NumFreecamFeatures) g_FreecamFeatureIdx = 0;
                wheelAccumulator = 0;
                lastScrollTime = currentTime;
            }
        }
        return true; 
    }

    bool handled = ImGui_ImplWin32_WndProcHandler(hWnd, uMsg, wParam, lParam);
    
    // Jika Menu Utama sedang terbuka dan fitur Block Input diaktifkan
    if (showMenu && set_BlockInput) {
        // Blokir semua input keyboard, mouse click, dan control keys (seperti CTRL, ALT, SHIFT, ENTER, dll.)
        // agar tidak diteruskan ke window game FiveM (tidak bisa ngetik, tidak bisa memicu shortcut game, dsb.)
        // Namun, biarkan ImGui tetap memproses input tersebut agar menu dapat berfungsi secara penuh.
        switch (uMsg) {
            case WM_KEYDOWN:
            case WM_KEYUP:
            case WM_SYSKEYDOWN:
            case WM_SYSKEYUP:
            case WM_CHAR:
            case WM_INPUT:
            case WM_LBUTTONDOWN:
            case WM_LBUTTONUP:
            case WM_RBUTTONDOWN:
            case WM_RBUTTONUP:
            case WM_MBUTTONDOWN:
            case WM_MBUTTONUP:
            case WM_MOUSEMOVE:
            case WM_MOUSEWHEEL:
            case WM_XBUTTONDOWN:
            case WM_XBUTTONUP:
                return true; // Drop message agar tidak sampai ke game
        }
    }

    if (showMenu && handled) return true;

    return CallWindowProc(oWndProc, hWnd, uMsg, wParam, lParam);
}

static SHORT lastInsertState = 0;
static SHORT lastDeleteState = 0;
static SHORT lastRShiftState = 0;
bool init = false;

HRESULT __stdcall hkPresent(IDXGISwapChain* pSwapChain, UINT SyncInterval, UINT Flags)
{
    if (!init)
    {
        if (SUCCEEDED(pSwapChain->GetDevice(__uuidof(ID3D11Device), (void**)&pDevice)))
        {
            pDevice->GetImmediateContext(&pContext);
            DXGI_SWAP_CHAIN_DESC sd;
            pSwapChain->GetDesc(&sd);
            window = sd.OutputWindow;
            ID3D11Texture2D* pBackBuffer;
            pSwapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), (LPVOID*)&pBackBuffer);
            pDevice->CreateRenderTargetView(pBackBuffer, NULL, &mainRenderTargetView);
            pBackBuffer->Release();
            oWndProc = (WNDPROC)SetWindowLongPtr(window, GWLP_WNDPROC, (LONG_PTR)WndProc);
            
            CoInitialize(NULL);

            InitImGui();

            // Load textures synchronously on the main thread. Multithreaded creation using the same device/context crashes GTA V (WIC decoder and D3D11 device are not thread-safe by default).
            if (!tex_banner) {
                bool loadedFromFile = false;
                // 1. Check if custom South.gg.jfif exists in current working directory
                if (std::filesystem::exists("South.gg.jfif")) {
                    loadedFromFile = CreateTextureFromFile(L"South.gg.jfif", &tex_banner);
                }
                // 2. Check if South.gg.jfif exists alongside the injected .dll module
                if (!loadedFromFile) {
                    wchar_t modulePath[MAX_PATH];
                    HMODULE hMod = NULL;
                    GetModuleHandleExW(GET_MODULE_HANDLE_EX_FLAG_FROM_ADDRESS | GET_MODULE_HANDLE_EX_FLAG_UNCHANGED_REFCOUNT, (LPCWSTR)&hkPresent, &hMod);
                    if (hMod && GetModuleFileNameW(hMod, modulePath, MAX_PATH)) {
                        std::filesystem::path p(modulePath);
                        std::filesystem::path jfifPath = p.parent_path() / L"South.gg.jfif";
                        if (std::filesystem::exists(jfifPath)) {
                            loadedFromFile = CreateTextureFromFile(jfifPath.c_str(), &tex_banner);
                        }
                    }
                }
                // 3. Fallback to embedded South.gg.jfif binary data inside the .dll
                if (!loadedFromFile || !tex_banner) {
                    CreateTextureFromMemory(banner_data_new, sizeof(banner_data_new), &tex_banner);
                }
            }
            if (!tex_south_logo) CreateTextureFromMemory(SouthLogo_128, sizeof(SouthLogo_128), &tex_south_logo, 100, 100);
            CreateTextureFromMemory(tab_executor_png, sizeof(tab_executor_png), &tex_executor);
            CreateTextureFromMemory(tab_executor_sel_png, sizeof(tab_executor_sel_png), &tex_executor_sel);
            CreateTextureFromMemory(tab_resources_png, sizeof(tab_resources_png), &tex_resources);
            CreateTextureFromMemory(tab_resources_sel_png, sizeof(tab_resources_sel_png), &tex_resources_sel);
            CreateTextureFromMemory(tab_scripts_png, sizeof(tab_scripts_png), &tex_scripts);
            CreateTextureFromMemory(tab_scripts_sel_png, sizeof(tab_scripts_sel_png), &tex_scripts_sel);
            CreateTextureFromMemory(tab_options_png, sizeof(tab_options_png), &tex_options);
            CreateTextureFromMemory(tab_options_sel_png, sizeof(tab_options_sel_png), &tex_options_sel);
            CreateTextureFromMemory(tab_settings_png, sizeof(tab_settings_png), &tex_settings);
            CreateTextureFromMemory(tab_settings_sel_png, sizeof(tab_settings_sel_png), &tex_settings_sel);
            CreateTextureFromMemory(icon_load_png, sizeof(icon_load_png), &tex_load);
            CreateTextureFromMemory(icon_play_png, sizeof(icon_play_png), &tex_play);
            CreateTextureFromMemory(icon_stop_png, sizeof(icon_stop_png), &tex_stop);
            CreateTextureFromMemory(icon_blocker_plus_png, sizeof(icon_blocker_plus_png), &tex_blocker_plus);
            CreateTextureFromMemory(icon_blocker_minus_png, sizeof(icon_blocker_minus_png), &tex_blocker_minus);
            CreateTextureFromMemory(logo_gen_silent_png, sizeof(logo_gen_silent_png), &tex_aim_gen);
            CreateTextureFromMemory(logo_friendlist_png, sizeof(logo_friendlist_png), &tex_aim_friend);
            CreateTextureFromMemory(logo_other_png, sizeof(logo_other_png), &tex_aim_other);
            CreateTextureFromMemory(logo_vehicle_crosshair_binary, sizeof(logo_vehicle_crosshair_binary), &tex_vehicle_crosshair);
            CreateTextureFromMemory(admin_list_png, sizeof(admin_list_png), &tex_admin_list);
            CreateTextureFromMemory(logo_load_menu_binary, sizeof(logo_load_menu_binary), &tex_load_menu);

            init = true;
            set_MenuBypass = true;
            Address::Get()->Load();
            LoadConfig();
            NameManager::Manager::Get().Start();



            // Force clear logs immediately on startup
            g_Logs.clear();
        }
        else
            return oPresent(pSwapChain, SyncInterval, Flags);
    }

    // Update server IP dynamically on the main thread (every 100 frames to save CPU)
    static int ipCheckCounter = 0;

    // Force clear logs every frame permanently to ensure logger stays completely empty
    g_Logs.clear();

    if (init && ++ipCheckCounter >= 100) {
        ipCheckCounter = 0;
        
        auto addr = Address::Get();
        uintptr_t world = addr ? addr->SafeRead<uintptr_t>(addr->m_World) : 0;
        uintptr_t localPlayer = world ? addr->SafeRead<uintptr_t>(world + 0x8) : 0;
        
        if (localPlayer != 0) {
            const char* endpoint = Invoker::Call<const char*>(0xEA11BFBA); // GET_CURRENT_SERVER_ENDPOINT
            if (endpoint && endpoint[0] != '\0') {
                g_CurrentServerIP = endpoint;
            } else {
                g_CurrentServerIP = "Loading...";
            }
        } else {
            g_CurrentServerIP = "Loading...";
        }
    }

    SHORT currentInsertState = GetAsyncKeyState(VK_INSERT);
    SHORT currentRShiftState = GetAsyncKeyState(VK_RSHIFT);
    SHORT currentDeleteState = GetAsyncKeyState(VK_DELETE);

    // Main Menu Toggle (Insert or RShift)
    if (GetForegroundWindow() == window && (((currentInsertState & 0x8000) && !(lastInsertState & 0x8000)) ||
        ((currentRShiftState & 0x8000) && !(lastRShiftState & 0x8000))))
    {
        showMenu = !showMenu;
    }

    // South Menu Toggle (Custom Key) - Only after loaded and set
    static uint64_t lastSouthToggle = 0;
    if (g_SouthLoaded && LuaBase::g_initialized && !g_ShowSouthKeybindSetup && !g_ShowVehicleSpawnerInput && !showMenu && g_SouthToggleKey != 0 && GetForegroundWindow() == window) {
        if ((GetAsyncKeyState(g_SouthToggleKey) & 0x8000) && GetTickCount64() - lastSouthToggle > 250) {
            g_ShowSouthMenu = !g_ShowSouthMenu;
            if (g_ShowSouthMenu) {
                g_Depth = 0;
                g_ActiveSub = 0;
                g_ActiveCat = 0;
                g_ItemIdx = 0;
            }
            lastSouthToggle = GetTickCount64();
        }
    }

    // Item Keybinds - Check and trigger features
    static std::map<std::string, uint64_t> lastKeybindPress;
    if (g_SouthLoaded && LuaBase::g_initialized && !g_ShowSouthKeybindSetup && !g_ShowVehicleSpawnerInput && !showMenu && GetForegroundWindow() == window) {
        for (const auto& [label, vk] : g_ItemKeybinds) {
            int cooldown = (label == "Noclip") ? ((g_NoclipMode == 1 || g_NoclipMode == 2) ? 2000 : 1000) : 250;
            if ((GetAsyncKeyState(vk) & 0x8000) && GetTickCount64() - lastKeybindPress[label] > cooldown) {
                lastKeybindPress[label] = GetTickCount64();
                
                // Find and toggle the corresponding menu item
                for (auto& sub : g_MenuData) {
                    for (auto& cat : sub.categories) {
                        for (auto& item : cat.items) {
                            if (item.label == label) {
                                if (item.type == "check" || item.type == "scroll-check" || item.type == "slider-check") {
                                    item.checked = !item.checked;
                                    
                                    // Handle specific features
                                     if (label == "Freecam") {
                                         g_FreecamActive = item.checked;
                                          if (g_FreecamActive) {
                                              // Turn off Noclip if active to prevent crashes
                                              if (g_NoclipEnabled) {
                                                  LuaCore::ToggleNoclip(false, g_NoclipSpeed, g_NoclipMode);
                                                  g_NoclipEnabled = false;
                                                  g_NoclipPed = 0;
                                                  for (auto& sub : g_MenuData) {
                                                      for (auto& cat : sub.categories) {
                                                          for (auto& menu_item : cat.items) {
                                                              if (menu_item.label == "Noclip") menu_item.checked = false;
                                                          }
                                                      }
                                                  }
                                              }
                                          }
                                          LuaCore::ToggleFreecam(item.checked, item.val, g_FreecamFeatureIdx + 1);
                                      }
                                      else if (label == "Noclip") {
                                          // Toggle Noclip safely using return validation to prevent spamming
                                          // Restore item.checked if ToggleNoclip failed (e.g. during cooldown)
                                          bool oldState = !item.checked;
                                          if (LuaCore::ToggleNoclip(item.checked, g_NoclipSpeed, g_NoclipMode)) {
                                              g_NoclipEnabled = item.checked;
                                              if (g_NoclipEnabled) {
                                                  // Turn off Freecam if active to prevent crashes
                                                  if (g_FreecamActive) {
                                                      g_FreecamActive = false;
                                                      LuaCore::ToggleFreecam(false, 0, 0);
                                                      for (auto& sub : g_MenuData) {
                                                          for (auto& cat : sub.categories) {
                                                              for (auto& menu_item : cat.items) {
                                                                  if (menu_item.label == "Freecam") menu_item.checked = false;
                                                              }
                                                          }
                                                      }
                                                  }
                                              } else {
                                                  g_NoclipPed = 0;
                                              }
                                         } else {
                                             item.checked = oldState;
                                         }
                                      }
                                    else if (label == "Godmode") {
                                        LuaCore::ToggleGodmode(item.checked);
                                    }
                                    else if (label == "Invisibility") {
                                        LuaCore::ToggleInvisibility(item.checked);
                                    }
                                    else if (label == "Fast Run") {
                                        LuaCore::ToggleFastRun(item.checked);
                                    }
                                    else if (label == "Fast Punch") {
                                        LuaCore::ToggleFastPunch(item.checked);
                                    }
                                    else if (label == "Super Jump") {
                                        LuaCore::ToggleSuperJump(item.checked);
                                    }
                                    else if (label == "Invincibility") {
                                        LuaCore::ToggleInvincibility(item.checked);
                                    }
                                    else if (label == "Infinity Stamina") {
                                        LuaCore::ToggleInfiniteStamina(item.checked);
                                    }
                                    else if (label == "Tiny Ped") {
                                        LuaCore::ToggleTinyPed(item.checked);
                                    }
                                    else if (label == "Super Punch") {
                                        LuaCore::ToggleSuperPunch(item.checked);
                                    }
                                    else if (label == "No Ragdoll") {
                                        LuaCore::ToggleNoRagdoll(item.checked);
                                    }
                                    else if (label == "Anti Freeze") {
                                        LuaCore::ToggleAntiFreeze(item.checked);
                                    }
                                    else if (label == "Anti Headshot") {
                                        LuaCore::ToggleAntiHeadshot(item.checked);
                                    }
                                    else if (label == "Anti Spectate") {
                                        LuaCore::ToggleAntiSpectate(item.checked);
                                    }
                                    else if (label == "Force Third Person") {
                                        LuaCore::ToggleForceThirdPerson(item.checked);
                                    }
                                    else if (label == "Force Driveby") {
                                        LuaCore::ToggleForceDriveby(item.checked);
                                    }
                                    else if (label == "txAdmin Noclip") {
                                        LuaCore::ToggleTxAdminNoclip(item.checked);
                                    }
                                    else if (label == "Disable All txAdmin" || label == "Disable txAdmin Teleport" || label == "Disable txAdmin Freeze") {
                                        LuaCore::ToggleDisableTxAdmin(item.checked);
                                    }
                                }
                                break;
                            }
                        }
                    }
                }
            }
        }
    }

    lastInsertState = currentInsertState;
    lastRShiftState = currentRShiftState;
    lastDeleteState = currentDeleteState;

    ImGuiIO& io = ImGui::GetIO();
    ImGui_ImplDX11_NewFrame();
    ImGui_ImplWin32_NewFrame();
    ImGui::NewFrame();

    // C++ Draw Re-enabled per user request
    DrawFreecamOverlay();




    DrawKeybindsOverlay();
    DrawNotifications();

    // Reverted: Removed synchronization between C++ and Lua

    // Pengecekan apakah Menu Utama atau Setup Keybind sedang aktif terbuka
    bool isAnyMenuOpen = showMenu || g_ShowSouthKeybindSetup || g_ShowVehicleSpawnerInput;
    UpdateBlockInputFocusState(isAnyMenuOpen);

    if (set_BlockInput && isAnyMenuOpen) {
        // =================================================================================
        // SECURE & UNDETECTABLE CAMERA / PLAYER BLOCKING
        // =================================================================================
        // Kami menggunakan pemblokiran kontrol game lokal per-frame (DISABLE_CONTROL_ACTION).
        // 
        // Mengapa ini 100% aman dari deteksi server-side anticheat?
        // 1. Fungsi 'SET_PLAYER_CONTROL' TIDAK digunakan karena mengubah status persistent engine
        //    yang sering dibaca oleh script anticheat server (IsPlayerControlOn).
        // 2. DISABLE_CONTROL_ACTION bekerja di level klien lokal dengan memfilter input mouse/keyboard,
        //    sehingga karakter di server terlihat diam secara natural tanpa anomali desinkronisasi.
        // 3. Kamera terkunci total tanpa getaran atau pergeseran 1 piksel pun.
        
        // Blokir Control Group 1 (Look/Camera - Mouse & Gamepad) secara agresif
        for (int i = 0; i < 6; i++) {
            Invoker::Call<void>(0xFE99B66D079CF6BC, 0, i);
        }

        // Blokir control gerakan (Move), serangan (Attacks/Aim), dan kontrol mouse kendaraan
        int extraControls[] = {
            24, 25, 30, 31, 32, 33, 34, 35, 37, 44, 45, 69, 70, 92, 114,
            140, 141, 142, 257, 263, 264, 268, 269, 270, 271, 272, 273,
            329, 330, 331, 332, 333, 345, 346, 347
        };
        for (int c : extraControls) {
            Invoker::Call<void>(0xFE99B66D079CF6BC, 0, c);
        }
    }



    io.MouseDrawCursor = showMenu;
    lastInsertState = currentInsertState;

    if (showMenu)
    {
        // Auto-warmup: execute dummy script in ALL resources to activate them
        static bool warmupDone = false;
        static int warmupFrame = 0;
        static int warmupResourceIndex = 0;
        static uint64_t warmupStart = GetTickCount64();
        
        if (!warmupDone && LuaBase::g_initialized) {
            warmupFrame++;
            
            // Start warmup after 20 frames
            if (warmupFrame > 20 && warmupFrame % 5 == 0) {
                auto& resources = _LuaGlobals::_ServerResources;
                
                if (warmupResourceIndex < (int)resources.size()) {
                    const auto& entry = resources[warmupResourceIndex];
                    auto* runtime = FindRuntimeByName(entry.name);
                    
                    // Try to execute in this resource to activate it
                    if (runtime) {
                        ExecuteScriptInRuntime(runtime, "-- warmup", entry.name);
                    }
                    
                    warmupResourceIndex++;
                } else {
                    warmupDone = true;
                    AddExecutorLog("Warmup complete. Executor active!");
                }
            }
        }
        else if (!warmupDone && GetTickCount64() - warmupStart > 15000) {
            // Prevent endless warmup attempts if Lua base never became ready
            warmupDone = true;
        }

        static ImVec2 southMenuPos = ImVec2(100.0f, 100.0f);
        ImVec2 screenSize = ImGui::GetIO().DisplaySize;
        ImVec2 targetWinSize = ImVec2(680, 520);
        bool needsClamp = false;
        if (southMenuPos.x < 0.0f) { southMenuPos.x = 0.0f; needsClamp = true; }
        if (southMenuPos.y < 0.0f) { southMenuPos.y = 0.0f; needsClamp = true; }
        if (southMenuPos.x + targetWinSize.x > screenSize.x) { southMenuPos.x = screenSize.x - targetWinSize.x; needsClamp = true; }
        if (southMenuPos.y + targetWinSize.y > screenSize.y) { southMenuPos.y = screenSize.y - targetWinSize.y; needsClamp = true; }
        if (needsClamp) {
            ImGui::SetNextWindowPos(southMenuPos);
        }

        ImGui::SetNextWindowSize(targetWinSize); 
        ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0, 0));
        ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 6.0f);
        ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.0f);
        ImGui::PushStyleColor(ImGuiCol_WindowBg, ImVec4(0.0549f, 0.0549f, 0.0705f, 1.0f));

        ImGui::Begin("South Menu", nullptr, ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoScrollWithMouse);
         
        ImVec2 windowPos = ImGui::GetWindowPos();
        ImVec2 windowSize = ImGui::GetWindowSize();
        ImDrawList* drawList = ImGui::GetWindowDrawList();

        ImGui::PopStyleVar(3);
        
        float topPad = 15.0f;
        float footerHeight = 25.0f; 
        float sidebarWidth = 75.0f;
        float gap = 18.0f;
        float btnInterW = 45.0f; 
        float btnVisualS = 80.0f; 
        
        

        float mainAreaH = windowSize.y - footerHeight;
        float btnY = windowSize.y - footerHeight - 45.0f;
        float lineY = btnY - 7.0f; 
        float editorHeight = (lineY - 0.0f) - topPad; 


        drawList->AddRectFilled(windowPos, ImVec2(windowPos.x + windowSize.x, windowPos.y + windowSize.y), IM_COL32(16, 16, 18, 255), 6.0f);


        float contentW = windowSize.x - sidebarWidth - gap - 10;
        float colWidthL = contentW * 0.50f;
        float colWidthR = contentW * 0.49f;

        auto NexusSection = [&](const char* title, ID3D11ShaderResourceView* icon, float height, std::function<void()> content, bool customIcon = false, float iconSize = 34.0f, ImVec2 iconOffset = ImVec2(0, 0), bool showScrollbar = true, bool allowScroll = true, float rounding = 6.0f, float widthOffset = 0.0f, int cornerFlags = 15) {
            ImVec2 p = ImGui::GetCursorScreenPos();
            float width = (colWidthL - 7) + widthOffset;

            drawList->AddRectFilled(p, ImVec2(p.x + width, p.y + height), IM_COL32(20, 19, 22, 255), rounding, cornerFlags);
            drawList->AddRect(p, ImVec2(p.x + width, p.y + height), IM_COL32(255, 255, 255, 20), rounding, cornerFlags);
            drawList->AddRectFilledMultiColor(p, ImVec2(p.x + width, p.y + 28), IM_COL32(75, 10, 12, 255), IM_COL32(75, 10, 12, 3), IM_COL32(75, 10, 12, 3), IM_COL32(75, 10, 12, 255));

            // Mask Top-Left
            drawList->PathLineTo(p);
            drawList->PathLineTo(ImVec2(p.x + rounding, p.y));
            drawList->PathArcTo(ImVec2(p.x + rounding, p.y + rounding), rounding, 1.5f * 3.14159f, 1.0f * 3.14159f);
            drawList->PathFillConvex(IM_COL32(16, 16, 18, 255));
            // Mask Top-Right
            drawList->PathLineTo(ImVec2(p.x + width, p.y));
            drawList->PathLineTo(ImVec2(p.x + width - rounding, p.y));
            drawList->PathArcTo(ImVec2(p.x + width - rounding, p.y + rounding), rounding, 1.5f * 3.14159f, 2.0f * 3.14159f);
            drawList->PathFillConvex(IM_COL32(16, 16, 18, 255));

            if (icon && !customIcon) {
                float yOff = (iconSize > 34.0f) ? -8.0f : -3.0f;
                float baseIconOffset = (strcmp(title, "Weapon Modifiers") == 0) ? 1.0f : 4.0f;
                ImVec2 iconPos = ImVec2(p.x + baseIconOffset + iconOffset.x, p.y + yOff + iconOffset.y);
                
                Icons::DrawIconTexture(drawList, iconPos, iconSize, icon, IM_COL32(180, 20, 25, 255));
            }
            
            if (customIcon && std::string(title).find("Block Input") != std::string::npos) {
                float ir = (iconSize > 34.0f) ? 12.0f : 9.0f;
                float lOff = (iconSize > 34.0f) ? 8.5f : 6.4f;
                ImVec2 iPos = (iconSize > 34.0f) ? ImVec2(p.x + 21, p.y + 13.5f) : ImVec2(p.x + 19, p.y + 13.5f);
                
                drawList->AddCircle(iPos, ir, IM_COL32(180, 20, 25, 255), 32, (iconSize > 34.0f) ? 2.5f : 2.0f);
                drawList->AddLine(ImVec2(iPos.x - lOff, iPos.y + lOff - 1.0f), ImVec2(iPos.x + lOff, iPos.y - lOff - 1.0f), IM_COL32(180, 20, 25, 255), (iconSize > 34.0f) ? 2.5f : 2.0f);
            }

            ImGui::PushFont(ImGui::GetIO().Fonts->Fonts[1]);
            std::string tStr = title;
            size_t qPos = tStr.find(" (?)");
            float baseTextOffset = (strcmp(title, "Weapon Modifiers") == 0) ? 36.0f : 39.0f;
            if (qPos != std::string::npos) {
                std::string mainTitle = tStr.substr(0, qPos);
                drawList->AddText(ImVec2(p.x + baseTextOffset, p.y + 3), IM_COL32(240, 240, 245, 255), mainTitle.c_str());
                float mainW = ImGui::CalcTextSize(mainTitle.c_str()).x;
                drawList->AddText(ImVec2(p.x + baseTextOffset + mainW + 3, p.y + 3), IM_COL32(180, 20, 25, 255), "(?)");
            } else {
                drawList->AddText(ImVec2(p.x + baseTextOffset, p.y + 3), IM_COL32(240, 240, 245, 255), title);
            }
            ImGui::PopFont();

            ImGui::SetCursorPosY(ImGui::GetCursorPosY() + 36);
            ImGui::PushStyleColor(ImGuiCol_ChildBg, IM_COL32(0, 0, 0, 0));
            ImGui::PushStyleVar(ImGuiStyleVar_ScrollbarSize, 0);
            ImGui::BeginChild(std::string("##child_").append(title).c_str(), ImVec2(width, height - 36), false, ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoBackground);

            float& targetY = scrollTarget[title];
            float& currentY = scrollCurrent[title];
            if (allowScroll) {
                if (ImGui::IsWindowHovered(ImGuiHoveredFlags_ChildWindows)) targetY -= io.MouseWheel * 45.0f;
                targetY = ImClamp(targetY, 0.0f, ImGui::GetScrollMaxY());
                currentY = ImLerp(currentY, targetY, io.DeltaTime * 10.0f);
            } else {
                targetY = 0.0f;
                currentY = 0.0f;
            }
            ImGui::SetScrollY(currentY);

            ImGui::Indent(12);
            content();
            ImGui::Dummy(ImVec2(0, 5));
            float maxScrollY = ImGui::GetScrollMaxY();
            ImGui::Unindent(12);
            ImGui::EndChild();
            ImGui::PopStyleVar();
            ImGui::PopStyleColor();


            if (showScrollbar && maxScrollY > 0) {
                float barH = (height - 40) * ((height - 40) / (height - 40 + maxScrollY));
                float barY = (height - 40 - barH) * (currentY / maxScrollY);

                ImVec2 bMin = ImVec2(p.x + width - 3, p.y + 38 + barY);
                ImVec2 bMax = ImVec2(p.x + width - 1, p.y + 38 + barY + barH);
                
                // Glow layers for 'light' effect
                drawList->AddRectFilled(ImVec2(bMin.x - 2.5f, bMin.y - 1.0f), ImVec2(bMax.x + 2.5f, bMax.y + 1.0f), IM_COL32(130, 15, 18, 35), 2.0f);
                drawList->AddRectFilled(ImVec2(bMin.x - 1.5f, bMin.y - 0.5f), ImVec2(bMax.x + 1.5f, bMax.y + 0.5f), IM_COL32(130, 15, 18, 75), 1.5f);
                
                // Bright Core
                drawList->AddRectFilled(bMin, bMax, IM_COL32(235, 30, 35, 255), 1.0f);
            }

            ImGui::SetCursorScreenPos(ImVec2(p.x, p.y + height + 10)); 
        };

        auto NexusSectionR = [&](const char* title, ID3D11ShaderResourceView* icon, float height, std::function<void()> content, float iconSize = 34.0f, ImVec2 iconOffset = ImVec2(0, 0), bool showScrollbar = true, bool allowScroll = true, float rounding = 6.0f, float widthOffset = 0.0f, int cornerFlags = 15) {
            ImVec2 p = ImGui::GetCursorScreenPos();
            float width = (colWidthR - 4) + widthOffset;

            drawList->AddRectFilled(p, ImVec2(p.x + width, p.y + height), IM_COL32(20, 19, 22, 255), rounding, cornerFlags);
            drawList->AddRect(p, ImVec2(p.x + width, p.y + height), IM_COL32(255, 255, 255, 20), rounding, cornerFlags);
            drawList->AddRectFilledMultiColor(p, ImVec2(p.x + width, p.y + 28), IM_COL32(75, 10, 12, 255), IM_COL32(75, 10, 12, 3), IM_COL32(75, 10, 12, 3), IM_COL32(75, 10, 12, 255));

            // Mask Top-Left
            drawList->PathLineTo(p);
            drawList->PathLineTo(ImVec2(p.x + rounding, p.y));
            drawList->PathArcTo(ImVec2(p.x + rounding, p.y + rounding), rounding, 1.5f * 3.14159f, 1.0f * 3.14159f);
            drawList->PathFillConvex(IM_COL32(16, 16, 18, 255));
            // Mask Top-Right
            drawList->PathLineTo(ImVec2(p.x + width, p.y));
            drawList->PathLineTo(ImVec2(p.x + width - rounding, p.y));
            drawList->PathArcTo(ImVec2(p.x + width - rounding, p.y + rounding), rounding, 1.5f * 3.14159f, 2.0f * 3.14159f);
            drawList->PathFillConvex(IM_COL32(16, 16, 18, 255));

            if (icon && std::string(title).find("Custom Keybind") == std::string::npos) {
                float yOff = (iconSize > 34.0f) ? -8.0f : -3.0f;
                float baseIconOffset = (strcmp(title, "Player List") == 0) ? 1.0f : 4.0f;
                ImVec2 iconPos = ImVec2(p.x + baseIconOffset + iconOffset.x, p.y + yOff + iconOffset.y);
 
                Icons::DrawIconTexture(drawList, iconPos, iconSize, icon, IM_COL32(180, 20, 25, 255));
            }

            if (std::string(title).find("Custom Keybind") != std::string::npos) {
                drawList->AddText(ImGui::GetIO().Fonts->Fonts[2], 17.0f, ImVec2(p.x + 13.0f + iconOffset.x, p.y + 7.0f + iconOffset.y), IM_COL32(180, 20, 25, 255), ICON_FA_KEYBOARD);
            }
            ImGui::PushFont(ImGui::GetIO().Fonts->Fonts[1]);
            
            std::string tStr = title;
            size_t qPos = tStr.find(" (?)");
            float baseTextOffset = (strcmp(title, "Player List") == 0) ? 36.0f : 39.0f;
            if (qPos != std::string::npos) {
                std::string mainTitle = tStr.substr(0, qPos);
                drawList->AddText(ImVec2(p.x + baseTextOffset, p.y + 3), IM_COL32(240, 240, 245, 255), mainTitle.c_str());
                float mainW = ImGui::CalcTextSize(mainTitle.c_str()).x;
                drawList->AddText(ImVec2(p.x + baseTextOffset + mainW + 3, p.y + 3), IM_COL32(180, 20, 25, 255), "(?)");
            } else {
                drawList->AddText(ImVec2(p.x + baseTextOffset, p.y + 3), IM_COL32(240, 240, 245, 255), title);
            }
            ImGui::PopFont();

            ImGui::SetCursorPosY(ImGui::GetCursorPosY() + 36);
            ImGui::PushStyleColor(ImGuiCol_ChildBg, IM_COL32(0, 0, 0, 0));
            ImGui::PushStyleVar(ImGuiStyleVar_ScrollbarSize, 0); 
            ImGui::BeginChild(std::string("##childR_").append(title).c_str(), ImVec2(width, height - 36), false, ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoBackground);


            float& targetY_R = scrollTarget[title];
            float& currentY_R = scrollCurrent[title];
            if (allowScroll) {
                if (ImGui::IsWindowHovered(ImGuiHoveredFlags_ChildWindows)) targetY_R -= io.MouseWheel * 45.0f;
                targetY_R = ImClamp(targetY_R, 0.0f, ImGui::GetScrollMaxY());
                currentY_R = ImLerp(currentY_R, targetY_R, io.DeltaTime * 10.0f);
            } else {
                targetY_R = 0.0f;
                currentY_R = 0.0f;
            }
            ImGui::SetScrollY(currentY_R);

            ImGui::Indent(12);
            content();
            ImGui::Dummy(ImVec2(0, 5));
            float maxScrollY_R = ImGui::GetScrollMaxY();
            ImGui::Unindent(12);
            ImGui::EndChild();
            ImGui::PopStyleVar();
            ImGui::PopStyleColor();


            if (showScrollbar && maxScrollY_R > 0) {
                float barH = (height - 40) * ((height - 40) / (height - 40 + maxScrollY_R));
                float barY = (height - 40 - barH) * (currentY_R / maxScrollY_R);

                ImVec2 bMin = ImVec2(p.x + width - 3, p.y + 38 + barY);
                ImVec2 bMax = ImVec2(p.x + width - 1, p.y + 38 + barY + barH);

                // Glow layers for 'light' effect
                drawList->AddRectFilled(ImVec2(bMin.x - 2.5f, bMin.y - 1.0f), ImVec2(bMax.x + 2.5f, bMax.y + 1.0f), IM_COL32(130, 15, 18, 35), 2.0f);
                drawList->AddRectFilled(ImVec2(bMin.x - 1.5f, bMin.y - 0.5f), ImVec2(bMax.x + 1.5f, bMax.y + 0.5f), IM_COL32(130, 15, 18, 75), 1.5f);

                // Bright Core
                drawList->AddRectFilled(bMin, bMax, IM_COL32(235, 30, 35, 255), 1.0f);
            }

            ImGui::SetCursorScreenPos(ImVec2(p.x, p.y + height + 10)); 
        };

        auto AimKeybind = [&](const char* label, int* key, float stride = 28.0f, float currentColWidth = 0.0f) {
            float activeColWidth = (currentColWidth > 0.0f) ? currentColWidth : colWidthL;
            ImVec2 rowPos = ImGui::GetCursorPos();
            ImGui::SetCursorPosY(rowPos.y + 2.0f);

            bool& binding = isBinding[label];
            float& anim = bindAnims[label];
            anim = ImLerp(anim, binding ? 1.0f : 0.0f, io.DeltaTime * 12.0f);

            ImGui::TextColored(ImVec4(0.45f, 0.45f, 0.48f, 1.0f), label);

            const char* keyName = binding ? "None" : GetKeyName(*key);
            float valW = ImGui::CalcTextSize(keyName).x;

            ImGui::SameLine(activeColWidth - 23 - valW);

            ImVec4 baseCol = ImVec4(0.45f, 0.45f, 0.48f, 1.0f);
            ImVec4 bindCol = ImGui::ColorConvertU32ToFloat4(IM_COL32(130, 15, 18, 255));
            ImVec4 tCol = ImVec4(
                baseCol.x + (bindCol.x - baseCol.x) * anim,
                baseCol.y + (bindCol.y - baseCol.y) * anim,
                baseCol.z + (bindCol.z - baseCol.z) * anim,
                1.0f
            );

            ImGui::TextColored(tCol, keyName);

            if (binding && GetForegroundWindow() == window) {
                for (int i = 1; i < 256; i++) {
                    if (i == VK_INSERT || i == set_MenuKey) continue;
                    if (GetAsyncKeyState(i) & 0x8000) {
                        *key = (i == VK_ESCAPE) ? 0 : i;
                        binding = false;
                        break;
                    }
                }
            }

            ImGui::SetCursorPos(rowPos);
            if (!(anyComboOpen && ImGui::IsMouseHoveringRect(comboDropMin, comboDropMax))) {
                if (ImGui::InvisibleButton(label, ImVec2(activeColWidth - 25, 20))) {
                    binding = true;
                }
            }
            
            ImGui::NewLine();
            ImGui::SetCursorPos(ImVec2(rowPos.x, rowPos.y + stride));
        };

        auto AimLabel = [&](const char* label, const char* value, float stride = 28.0f) {
            ImVec2 rowPos = ImGui::GetCursorPos();
            ImGui::SetCursorPosY(rowPos.y + 2.0f);
            ImGui::TextColored(ImVec4(0.45f, 0.45f, 0.48f, 1.0f), label);

            float valW = ImGui::CalcTextSize(value).x;
            ImGui::SameLine(colWidthL - 23 - valW);
            ImGui::TextDisabled(value);
            ImGui::NewLine();
            ImGui::SetCursorPos(ImVec2(rowPos.x, rowPos.y + stride));
        };

        auto AimToggle = [&](const char* label, bool* v, float stride = 28.0f, float currentColWidth = 0.0f, float rightOffset = 46.0f, ImVec4 customOffCol = ImVec4(0.45f, 0.45f, 0.48f, 1.0f), const char* displayOverride = nullptr) -> bool {
            float activeColWidth = (currentColWidth > 0.0f) ? currentColWidth : colWidthL;
            ImVec2 rowPos = ImGui::GetCursorPos();
            ImDrawList* d = ImGui::GetWindowDrawList();
            ImGui::PushID(label);
            float& anim = optAnims[label];
            anim = ImLerp(anim, *v ? 1.0f : 0.0f, io.DeltaTime * 14.0f);

            ImGui::SetCursorPosY(rowPos.y + 2.0f);
            ImVec4 offCol = customOffCol;
            ImVec4 onCol = ImVec4(0.95f, 0.95f, 0.98f, 1.0f);
            ImVec4 tCol = ImVec4(
                offCol.x + (onCol.x - offCol.x) * anim,
                offCol.y + (onCol.y - offCol.y) * anim,
                offCol.z + (onCol.z - offCol.z) * anim,
                1.0f
            );

            const char* textToDraw = (displayOverride != nullptr) ? displayOverride : label;
            std::string lbl(textToDraw);
            size_t qPos = lbl.find(" (?)");
            if (qPos != std::string::npos) {
                ImGui::TextColored(tCol, "%s", lbl.substr(0, qPos).c_str());
                ImGui::SameLine(0, 0);
                ImGui::TextColored(ImVec4(0.55f, 0.04f, 0.08f, 1.0f), " (?)");
            } else {
                ImGui::TextColored(tCol, "%s", textToDraw);
            }
            ImGui::SameLine(activeColWidth - rightOffset);

            ImVec2 p = ImGui::GetCursorScreenPos();
            bool changed = false;
            if (anyComboOpen && ImGui::IsMouseHoveringRect(comboDropMin, comboDropMax)) {
            } else {
                if (ImGui::InvisibleButton("##t", ImVec2(30, 16))) {
                    *v = !*v;
                    changed = true;
                }
            }

            ImU32 bgCol = IM_COL32((int)(14 + 166 * anim), (int)(14 + 21 * anim), (int)(18 + 22 * anim), 255);
            d->AddRectFilled(ImVec2(p.x, p.y + 1), ImVec2(p.x + 30, p.y + 15), bgCol, 10.0f);

            ImU32 knobCol = IM_COL32((int)(70 + 185 * anim), (int)(70 + 185 * anim), (int)(75 + 180 * anim), 255);
            d->AddCircleFilled(ImVec2(p.x + 8.0f + 14.0f * anim, p.y + 8), 4.5f, knobCol);
            ImGui::PopID();

            ImGui::NewLine();
            ImGui::SetCursorPos(ImVec2(rowPos.x, rowPos.y + stride));
            return changed;
        };

        auto AimSlider = [&](const char* label, float* v, float min, float max, const char* fmt = "%.3f", float sliderWidth = 160.0f, float stride = 28.0f, float currentColWidth = 0.0f) {
            float activeColWidth = (currentColWidth > 0.0f) ? currentColWidth : colWidthL;
            ImVec2 rowPos = ImGui::GetCursorPos();
            ImDrawList* d = ImGui::GetWindowDrawList();
            float width = activeColWidth - 16;

            ImGui::SetCursorPosY(rowPos.y + 2.0f);
            ImGui::TextColored(ImVec4(0.92f, 0.92f, 0.94f, 1.0f), label);

            char valBuf[32];
            sprintf_s(valBuf, sizeof(valBuf), fmt, *v);


            if (std::string(fmt) == "%.0f" && *v >= 1000.0f) {
                std::string s = valBuf;
                int n = (int)s.length() - 3;
                while (n > 0) {
                    s.insert(n, ".");
                    n -= 3;
                }
                strcpy_s(valBuf, s.c_str());
            }

            float sliderStart = width - sliderWidth;
            float valW = ImGui::CalcTextSize(valBuf).x;

            ImGui::SameLine(sliderStart - valW - 10);
            ImGui::TextColored(ImVec4(0.94f, 0.94f, 0.96f, 1.0f), valBuf);

            float t = (*v - min) / (max - min);

            ImGui::SameLine(sliderStart);
            ImVec2 sPos = ImGui::GetCursorScreenPos();
            if (!(anyComboOpen && ImGui::IsMouseHoveringRect(comboDropMin, comboDropMax))) {
                if (ImGui::InvisibleButton(label, ImVec2(sliderWidth, 18))) {
                    *v = min + (io.MousePos.x - sPos.x) / sliderWidth * (max - min);
                }
                if (ImGui::IsItemActive()) {
                    *v = min + (io.MousePos.x - sPos.x) / sliderWidth * (max - min);
                }
            }
            *v = ImClamp(*v, min, max);

            ImVec2 barP = ImVec2(sPos.x, sPos.y + 9.0f);
            float barH = 6.0f;
            // Track (dark background)
            d->AddRectFilled(ImVec2(barP.x, barP.y - barH * 0.5f), ImVec2(barP.x + sliderWidth, barP.y + barH * 0.5f), IM_COL32(27, 27, 31, 255), barH * 0.5f);
            // Fill (red) - using clip rect to match sbaggy exactly
            if (t > 0.001f) {
                ImGui::PushClipRect(ImVec2(barP.x - 1.0f, barP.y - barH), ImVec2(barP.x + sliderWidth * t, barP.y + barH), true);
                d->AddRectFilled(ImVec2(barP.x, barP.y - barH * 0.5f), ImVec2(barP.x + sliderWidth, barP.y + barH * 0.5f), IM_COL32(230, 8, 24, 255), barH * 0.5f);
                ImGui::PopClipRect();
            }
            // Thumb (white circle with small dark center dot)
            ImVec2 thumbPos = ImVec2(barP.x + sliderWidth * t, barP.y);
            d->AddCircleFilled(thumbPos, 6.0f, IM_COL32(255, 255, 255, 255), 36);
            d->AddCircleFilled(thumbPos, 3.0f, IM_COL32(25, 25, 25, 76), 36);

            ImGui::NewLine();
            ImGui::SetCursorPos(ImVec2(rowPos.x, rowPos.y + stride));
        };

        auto AimCombo = [&](const char* label, int* current, const char** items, int items_count, float stride = 28.0f) {
            ImVec2 rowPos = ImGui::GetCursorPos();
            ImVec2 screenBase = ImGui::GetCursorScreenPos();
            ImDrawList* d = ImGui::GetWindowDrawList();
            float width = colWidthL - 38;

            ImGui::SetCursorPosY(rowPos.y + 2.0f);
            ImGui::TextColored(ImVec4(0.45f, 0.45f, 0.48f, 1.0f), label);

            const char* valStr = items[*current];
            float valW = ImGui::CalcTextSize(valStr).x;

            bool& open = comboOpen[label];
            float& anim = comboAnims[label];
            anim = ImLerp(anim, open ? 1.0f : 0.0f, io.DeltaTime * 14.0f);

            float comboW = 65.0f;
            ImVec2 bgMin = ImVec2(screenBase.x + width - comboW, screenBase.y - 4.0f);
            ImVec2 bgMax = ImVec2(screenBase.x + width + 9, screenBase.y + 16.0f);
            
            d->AddRectFilled(bgMin, bgMax, IM_COL32(14, 14, 18, 255), 4.0f);
            d->AddRect(bgMin, bgMax, IM_COL32(255, 255, 255, 12), 4.0f);

            // Center the text inside the closed combo box (excluding arrow space on the right)
            float textX = width - comboW + (comboW - valW) * 0.5f + 2.0f;
            ImVec2 tSize = ImGui::CalcTextSize(valStr);
            ImVec2 tPos = ImVec2(screenBase.x + textX, screenBase.y + 6.0f - tSize.y * 0.5f);
            d->AddText(tPos, IM_COL32(200, 200, 205, 255), valStr);

            float aY = screenBase.y + 5.0f;
            float arrowOffset = 2.0f * (1.0f - 2.0f * anim);
            float extraHeight = items_count * 30.0f;

            ImVec2 p1 = ImVec2(screenBase.x + width - 6, aY + 4.0f - arrowOffset);
            ImVec2 p2 = ImVec2(screenBase.x + width - 2, aY + 4.0f + arrowOffset);
            ImVec2 p3 = ImVec2(screenBase.x + width + 2, aY + 4.0f - arrowOffset);

            d->AddLine(p1, p2, IM_COL32(100, 100, 105, 255), 1.2f);
            d->AddLine(p2, p3, IM_COL32(100, 100, 105, 255), 1.2f);

            ImGui::SetCursorPos(rowPos);
            if (!(anyComboOpen && ImGui::IsMouseHoveringRect(comboDropMin, comboDropMax))) {
                if (ImGui::InvisibleButton(label, ImVec2(width + 20, 22))) open = !open;
            }

            if (anim > 0.4f) {
                 anyComboOpen = true;
                 comboDropMin = ImVec2(bgMin.x, bgMax.y);
                 comboDropMax = ImVec2(bgMax.x, comboDropMin.y + extraHeight);
            }

            if (anim > 0.005f) {
                ImDrawList* overlay = ImGui::GetForegroundDrawList();

                ImVec2 dropMin = ImVec2(bgMin.x, bgMax.y);
                ImVec2 dropMax = ImVec2(bgMax.x, dropMin.y + (extraHeight * anim));

                overlay->AddRectFilled(dropMin, dropMax, IM_COL32(14, 14, 18, (int)(255 * anim)), 4.0f, ImDrawCornerFlags_BotLeft | ImDrawCornerFlags_BotRight);
                overlay->AddRect(dropMin, dropMax, IM_COL32(255, 255, 255, (int)(12 * anim)), 4.0f, ImDrawCornerFlags_BotLeft | ImDrawCornerFlags_BotRight);

                ImGui::SetCursorPos(ImVec2(rowPos.x + width - comboW, rowPos.y + 24.0f));
                ImGui::BeginGroup();
                for (int i = 0; i < items_count; i++) {
                    ImGui::PushID(i);
                    ImVec2 iPos = ImGui::GetCursorScreenPos();
                    float itemW = bgMax.x - bgMin.x;

                    bool hovered = ImGui::IsMouseHoveringRect(iPos, ImVec2(iPos.x + itemW, iPos.y + 25.0f));
                    if (hovered && ImGui::IsMouseClicked(0)) {
                        *current = i;
                        open = false;
                    }

                    if (hovered && anim > 0.5f) {
                        overlay->AddRectFilled(iPos, ImVec2(iPos.x + itemW, iPos.y + 25.0f), IM_COL32(255, 255, 255, (int)(15 * anim)), 4.0f);
                    }

                    ImU32 tCol = (*current == i) ? IM_COL32(230, 8, 24, (int)(255 * anim)) : (hovered ? IM_COL32(240, 240, 245, (int)(255 * anim)) : IM_COL32(140, 140, 145, (int)(255 * anim)));

                    ImVec2 tSize = ImGui::CalcTextSize(items[i]);
                    ImVec2 tPos = ImVec2(iPos.x + (itemW - tSize.x) * 0.5f, iPos.y + (25.0f - tSize.y) * 0.5f);
                    overlay->AddText(tPos, tCol, items[i]);

                    ImGui::PopID();
                    ImGui::Dummy(ImVec2(itemW, 25.0f));
                }
                ImGui::EndGroup();
            }

            ImGui::NewLine();
            ImGui::SetCursorPos(ImVec2(rowPos.x, rowPos.y + stride));
        };

        auto AimColor = [&](const char* label, float* col, float stride = 28.0f, float currentColWidth = 0.0f) {
            float activeColWidth = (currentColWidth > 0.0f) ? currentColWidth : colWidthL;
            ImVec2 rowPos = ImGui::GetCursorPos();
            ImDrawList* d = ImGui::GetWindowDrawList();
            ImVec2 screenBase = ImGui::GetCursorScreenPos();
            float width = (currentColWidth > 0.0f) ? (activeColWidth - 30) : (activeColWidth - 38);

            ImGui::PushID(label);

            ImGui::SetCursorPosY(rowPos.y + 2.0f);
            ImGui::TextColored(ImVec4(0.45f, 0.45f, 0.48f, 1.0f), label);

            ImVec2 tileSize = ImVec2(20, 20);
            ImVec2 p = ImVec2(screenBase.x + width - tileSize.x + 2.0f, screenBase.y + 1.0f);


            ImU32 c = ImGui::ColorConvertFloat4ToU32(*(ImVec4*)col);
            d->AddRectFilled(p, ImVec2(p.x + tileSize.x, p.y + tileSize.y), c, 2.0f);
            d->AddRect(p, ImVec2(p.x + tileSize.x, p.y + tileSize.y), IM_COL32(45, 45, 50, 255), 2.0f, 0, 1.0f);


            ImGui::SetCursorPos(rowPos);
            if (ImGui::InvisibleButton("##color_hit", ImVec2(activeColWidth, stride - 2.0f))) ImGui::OpenPopup("##cp");

            if (ImGui::BeginPopup("##cp")) {
                ImGui::ColorPicker4("##picker", col, ImGuiColorEditFlags_AlphaBar | ImGuiColorEditFlags_NoSidePreview);
                ImGui::EndPopup();
            }

            ImGui::PopID();
            ImGui::SetCursorPos(ImVec2(rowPos.x, rowPos.y + stride));
        };



        ImVec2 sbMin = windowPos;
        ImVec2 sbMax = ImVec2(windowPos.x + sidebarWidth, windowPos.y + mainAreaH);
        drawList->AddRectFilled(sbMin, sbMax, IM_COL32(14, 14, 16, 255), 6.0f);
        drawList->AddRectFilled(ImVec2(sbMin.x, sbMax.y - 10.0f), sbMax, IM_COL32(14, 14, 16, 255));
        drawList->AddRectFilled(ImVec2(sbMax.x - 10.0f, sbMin.y), sbMax, IM_COL32(14, 14, 16, 255));


        ImVec2 edMin = ImVec2(windowPos.x + sidebarWidth + gap, windowPos.y);
        ImVec2 edMax = ImVec2(windowPos.x + windowSize.x, windowPos.y + mainAreaH);
        drawList->AddRectFilled(edMin, edMax, IM_COL32(16, 16, 18, 255), 6.0f);
        drawList->AddRectFilled(ImVec2(edMin.x, edMax.y - 10.0f), edMax, IM_COL32(16, 16, 18, 255));
        drawList->AddRectFilled(ImVec2(edMin.x, edMin.y), ImVec2(edMin.x + 10.0f, edMax.y), IM_COL32(16, 16, 18, 255));


        ImVec2 ftMin = ImVec2(windowPos.x, windowPos.y + mainAreaH);
        ImVec2 ftMax = ImVec2(windowPos.x + windowSize.x, windowPos.y + windowSize.y);
        drawList->AddRectFilled(ftMin, ftMax, IM_COL32(23, 23, 25, 255), 6.0f);
        drawList->AddRectFilled(ftMin, ImVec2(ftMax.x, ftMin.y + 10.0f), IM_COL32(23, 23, 25, 255));


        float lineX = windowPos.x + sidebarWidth + 1.0f; 
        drawList->AddLine(ImVec2(lineX, windowPos.y), ImVec2(lineX, windowPos.y + mainAreaH), IM_COL32(255, 255, 255, 15), 1.0f); 


        float tabBtnSize = 54.0f; 
        float tabGap = 10.0f;
        float totalTabBlockHeight = (5 * tabBtnSize) + (4 * tabGap);
        float startY = (mainAreaH - totalTabBlockHeight) * 0.5f;


        ImGui::SetCursorPos(ImVec2(0, 0));
        ImGui::PushStyleColor(ImGuiCol_ChildBg, IM_COL32(14, 14, 16, 255));
        ImGui::BeginChild("##sidebar", ImVec2(sidebarWidth, mainAreaH), false, ImGuiWindowFlags_NoScrollbar);
        {

            float targetY = startY + currentTab * (tabBtnSize + tabGap);

            sidebarSelectorY = ImLerp(sidebarSelectorY, targetY, ImGui::GetIO().DeltaTime * 12.0f);

            ImVec2 selectorMin = ImVec2(windowPos.x + (sidebarWidth - tabBtnSize) * 0.5f, windowPos.y + sidebarSelectorY);
            ImVec2 selectorMax = ImVec2(selectorMin.x + tabBtnSize, selectorMin.y + tabBtnSize);
            drawList->AddRectFilled(selectorMin, selectorMax, IM_COL32(255, 255, 255, 15), 6.0f); 

            auto SidebarTab = [&](int tabIndex, ID3D11ShaderResourceView* texNormal, ID3D11ShaderResourceView* texSelected, float baseIconSize = 46.0f) {
                ImGui::PushID(tabIndex);
                bool isActive = (currentTab == tabIndex);
                ImGui::SetCursorPosX((sidebarWidth - tabBtnSize) * 0.5f);
                ImVec2 pos = ImGui::GetCursorScreenPos();
                
                if (ImGui::InvisibleButton("##tab", ImVec2(tabBtnSize, tabBtnSize))) currentTab = tabIndex;
                bool hovered = ImGui::IsItemHovered();


                float targetAlpha = (isActive || hovered) ? 1.0f : 0.0f;
                sidebarAlphas[tabIndex] = ImLerp(sidebarAlphas[tabIndex], targetAlpha, ImGui::GetIO().DeltaTime * 12.0f);
                
                float targetScale = 1.0f; 
                sidebarScales[tabIndex] = ImLerp(sidebarScales[tabIndex], targetScale, ImGui::GetIO().DeltaTime * 10.0f);


                if (hovered && !isActive) {
                    drawList->AddRectFilled(pos, ImVec2(pos.x + tabBtnSize, pos.y + tabBtnSize), IM_COL32(255, 255, 255, (int)(15 * sidebarAlphas[tabIndex])), 6.0f);
                }
                
                float animatedSize = baseIconSize * sidebarScales[tabIndex];
                float animatedSizeH = (tabIndex == 3) ? (animatedSize + 4.0f) : animatedSize; // Only stretch options tab (tabIndex 3) vertically by 4px
                ImVec2 iPos = ImVec2(pos.x + (tabBtnSize - animatedSize) * 0.5f, pos.y + (tabBtnSize - animatedSizeH) * 0.5f);
                
                ID3D11ShaderResourceView* currentTex = (isActive) ? texSelected : texNormal;
                ImU32 iconColor = IM_COL32(255, 255, 255, (int)(180 + 75 * sidebarAlphas[tabIndex]));

                if (isActive) {
                    ImVec2 center = ImVec2(pos.x + tabBtnSize * 0.5f, pos.y + tabBtnSize * 0.5f);
                    // Custom radial glow effect using concentric circles (slightly smaller, faster outer fade)
                    for (int i = 1; i <= 17; i++) {
                        float r = 8.0f + (float)i * 1.25f; // Max radius ~29px (down from 34px)
                        float t = (float)i / 17.0f;
                        float alpha = 6.0f * (1.0f - t * t * t); // Cubic falloff makes outer edges dimmer and softer
                        drawList->AddCircleFilled(center, r, IM_COL32(230, 8, 24, (int)alpha), 36);
                    }
                }

                if (currentTex) {
                    drawList->AddImageRounded(currentTex, iPos, ImVec2(iPos.x + animatedSize, iPos.y + animatedSizeH), ImVec2(0, 0), ImVec2(1, 1), iconColor, 6.0f);
                }
                
                ImGui::PopID();
                ImGui::Dummy(ImVec2(0, tabGap));
            };

            ImGui::SetCursorPosY(startY);
            SidebarTab(0, tex_executor, tex_executor_sel, 42.0f); 
            SidebarTab(1, tex_resources, tex_resources_sel);
            SidebarTab(2, tex_scripts, tex_scripts_sel);
            SidebarTab(3, tex_options, tex_options_sel, 52.0f);
            SidebarTab(4, tex_settings, tex_settings_sel);
        }
        ImGui::EndChild();
        ImGui::PopStyleColor();

        if (currentTab != lastTab) {
            contentFade = 0.0f;
            lastTab = currentTab;
        }
        contentFade = ImLerp(contentFade, 1.0f, ImGui::GetIO().DeltaTime * 8.0f);


        float subtabShift = 6.0f;
        ImGui::SetCursorPos(ImVec2(sidebarWidth + gap, topPad - subtabShift)); 
        ImGui::PushStyleVar(ImGuiStyleVar_Alpha, contentFade);
        float dynamicEditorH = (currentTab >= 2) ? (mainAreaH - (topPad - subtabShift) - 2.0f) : (editorHeight + subtabShift);
        ImGui::BeginChild("##editor_comp", ImVec2(windowSize.x - sidebarWidth - gap - 10, dynamicEditorH), false, ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoScrollWithMouse | ImGuiWindowFlags_NoBackground);
        {
            // Collect script runtimes safely via background thread/hooks instead of every frame on the DX11 Present thread
            // Collector::CollectScriptRuntimes();
            if (g_runtimeFound) {
                RefreshServerResources();
                g_runtimeFound = false;
            }

            auto now = std::chrono::steady_clock::now();
            {
                // Adaptive refresh: fast (2000ms) for the first 30 seconds after injection
                // or after joining a new server (detected by resource count changes)
                static auto g_injectTime = std::chrono::steady_clock::now();
                static bool g_fastPhase = true;
                static size_t g_lastResourceCount = 0;

                size_t currentResourceCount = 0;
                try {
                    auto rm = fx::ResourceManager::getCurrentIl();
                    if (rm) {
                        currentResourceCount = rm->getAllResources().size();
                    }
                } catch (...) {}

                // If resource count changed (e.g. connected to server or loaded in lobby), reset fast phase
                if (currentResourceCount != g_lastResourceCount) {
                    g_fastPhase = true;
                    g_injectTime = now;
                    g_lastResourceCount = currentResourceCount;
                }

                auto elapsed_since_inject = std::chrono::duration_cast<std::chrono::seconds>(now - g_injectTime).count();
                if (elapsed_since_inject > 30) g_fastPhase = false;

                int refreshInterval = g_fastPhase ? 5000 : 10000;
                
                if (std::chrono::duration_cast<std::chrono::milliseconds>(now - lastResourceRefresh).count() >= refreshInterval) {
                    try {
                        auto rm = fx::ResourceManager::getCurrentIl();
                        if (rm) cachedResources = rm->getAllResources();
                        RefreshServerResources();
                    } catch (...) {}
                    lastResourceRefresh = now;
                }
            }

            if (currentTab == 0) {
                ImGui::SetCursorPosY(ImGui::GetCursorPosY() + 5.0f);
                float editorHeight = btnY - ImGui::GetCursorPos().y - 20.0f;
                if (editorHeight < 100.0f) editorHeight = 100.0f;

                editor.Render("##editor_lua", ImVec2(0, editorHeight), false);
                
                {
                    float lineW = windowSize.x - sidebarWidth - gap - 10.0f;
                    ImVec2 basePos = ImVec2(windowPos.x + sidebarWidth + gap, windowPos.y + topPad + editorHeight + 5.0f);
                    
                    float contentW = editor.mLastScrollMaxX + editor.mLastWindowWidth;
                    float ratio = (contentW > editor.mLastWindowWidth) ? (editor.mLastWindowWidth / contentW) : 1.0f;
                    float barW = ImMax(lineW * ratio, 35.0f); 
                    
                    float scrollOffset = (editor.mLastScrollMaxX > 0) ? (editor.mLastScrollX / editor.mLastScrollMaxX) * (lineW - barW) : 0.0f;
                    
                    ImVec2 bMin = ImVec2(basePos.x + scrollOffset, basePos.y - 1.5f);
                    ImVec2 bMax = ImVec2(bMin.x + barW, basePos.y + 1.5f);
                    
                    auto fgDraw = ImGui::GetForegroundDrawList();
                    
                    fgDraw->AddRectFilled(ImVec2(basePos.x, basePos.y), ImVec2(basePos.x + lineW, basePos.y + 1.0f), IM_COL32(255, 0, 0, 20)); 
                    
                    fgDraw->AddRectFilled(ImVec2(bMin.x - 3.0f, bMin.y - 1.0f), ImVec2(bMax.x + 3.0f, bMax.y + 1.0f), IM_COL32(255, 0, 0, 25), 1.0f); 
                    fgDraw->AddRectFilled(ImVec2(bMin.x - 1.0f, bMin.y - 0.5f), ImVec2(bMax.x + 1.0f, bMax.y + 0.5f), IM_COL32(255, 0, 0, 60), 1.0f); 
                    fgDraw->AddRectFilled(bMin, bMax, IM_COL32(255, 0, 0, 255), 1.0f); 
                    fgDraw->AddRectFilled(bMin, ImVec2(bMax.x, bMin.y + 1.0f), IM_COL32(255, 120, 120, 40), 1.0f); 
                }


            }
            else if (currentTab == 1) {
                ImGui::PushStyleColor(ImGuiCol_ChildBg, IM_COL32(16, 16, 18, 255));
                ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2(22, 0));
                ImGui::PushStyleVar(ImGuiStyleVar_ButtonTextAlign, ImVec2(0.5f, 0.32f));
                
                static float subAlphas[5] = { 0.0f, 0.0f, 0.0f, 0.0f, 0.0f };

                auto SubTab = [&](const char* label, int index) {
                    ImGui::PushFont(io.Fonts->Fonts[3]);
                    ImVec2 p = ImGui::GetCursorScreenPos();
                    bool active = (currentSubTab == index);
                    float textW = ImGui::CalcTextSize(label).x;
                    float tW = textW + 56.0f;
                    float tH = 40.0f;

                    subAlphas[index] = ImLerp(subAlphas[index], active ? 1.0f : 0.0f, io.DeltaTime * 12.0f);

                    if (subAlphas[index] > 0.001f) {
                        ImVec2 bMin = ImVec2(p.x, p.y);
                        ImVec2 bMax = ImVec2(p.x + tW, p.y + tH);
                        drawList->AddRectFilled(bMin, bMax, IM_COL32(23, 23, 28, (int)(255 * subAlphas[index])), 5.0f);
                    }

                    ImGui::PushStyleColor(ImGuiCol_Button, IM_COL32(0, 0, 0, 0));
                    ImGui::PushStyleColor(ImGuiCol_ButtonHovered, IM_COL32(0, 0, 0, 0));
                    ImGui::PushStyleColor(ImGuiCol_ButtonActive, IM_COL32(0, 0, 0, 0));

                    if (ImGui::InvisibleButton(label, ImVec2(tW, tH))) currentSubTab = index;

                    ImVec2 tSize = ImGui::CalcTextSize(label);
                    ImVec2 tPos = ImVec2(p.x + (tW - tSize.x) * 0.5f, p.y + (tH - tSize.y) * 0.5f);

                    ImU32 tCol = active ? IM_COL32(255, 255, 255, 255) : IM_COL32(110, 110, 115, 255);
                    drawList->AddText(tPos, tCol, label);

                    ImGui::PopStyleColor(3);

                    if (subAlphas[index] > 0.001f) {
                        float lineX1 = p.x + (tW - textW) * 0.5f;
                        float lineX2 = lineX1 + textW;
                        float lineY1 = p.y + tH - 3.0f;
                        float lineY2 = p.y + tH;

                        // Brighter red line
                        drawList->AddRectFilled(ImVec2(lineX1, lineY1), ImVec2(lineX2, lineY2), IM_COL32(255, 0, 0, (int)(255 * subAlphas[index])), 360.f, ImDrawCornerFlags_Top);
                        
                        // Soft red light glow effect
                        for (int g = 1; g <= 6; ++g) {
                            float offset = (float)g * 0.8f;
                            float falloff = 1.0f - ((float)g / 6.0f);
                            ImU32 glowCol = IM_COL32(255, 0, 0, (int)(50 * falloff * subAlphas[index]));
                            drawList->AddRectFilled(ImVec2(lineX1 - offset, lineY1 - offset), ImVec2(lineX2 + offset, lineY2 + offset), glowCol, 360.f, ImDrawCornerFlags_Top);
                        }
                    }

                    ImGui::PopFont();
                    ImGui::SameLine();
                };


                float horizLineY = ImGui::GetWindowPos().y + 48.0f;
                float startX = windowPos.x + sidebarWidth + 2.0f;
                drawList->AddLine(ImVec2(startX, horizLineY), ImVec2(ImGui::GetWindowPos().x + windowSize.x - 10, horizLineY), IM_COL32(255, 255, 255, 15), 1.0f);

                ImGui::SetCursorPos(ImVec2(4, 0)); 
                SubTab("Stopper", 0);
                SubTab("Dumper", 3);
                SubTab("Logger", 1);
                SubTab("Blocker", 2);

                ImGui::PopStyleVar(2);


                if (currentSubTab != lastSubTab) {
                    resourceFade = 0.0f;
                    lastSubTab = currentSubTab;
                }
                resourceFade = ImLerp(resourceFade, 1.0f, ImGui::GetIO().DeltaTime * 10.0f);

                ImGui::PushStyleVar(ImGuiStyleVar_Alpha, contentFade * resourceFade);
                if (currentSubTab == 0) {
                    anyComboOpen = false;
                    ImGui::SetCursorPos(ImVec2(0, 56)); 
                    ImGui::PushStyleColor(ImGuiCol_ChildBg, IM_COL32(20, 19, 22, 255)); 
                    ImGui::PushStyleVar(ImGuiStyleVar_ChildRounding, 6.0f);
                    
                    float blockW = windowSize.x - sidebarWidth - gap - 10; 
                    if (ImGui::BeginChild("##res_block", ImVec2(blockW, editorHeight - 50), false, ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoScrollWithMouse)) {
                        
                        ImDrawList* d = ImGui::GetWindowDrawList();
                        ImVec2 p = ImGui::GetWindowPos(); 
                        float w = ImGui::GetWindowWidth();
                        
                        // Nexus Style Header (Identik Tab 3)
                        float rounding = 6.0f;
                        float h = ImGui::GetWindowHeight();
                        d->AddRectFilled(p, ImVec2(p.x + w, p.y + 30), IM_COL32(20, 19, 22, 255), rounding, ImDrawCornerFlags_Top);
                        d->AddRectFilledMultiColor(p, ImVec2(p.x + w, p.y + 28), IM_COL32(75, 10, 12, 255), IM_COL32(75, 10, 12, 3), IM_COL32(75, 10, 12, 3), IM_COL32(75, 10, 12, 255));

                        // Masking Pojokan (Biar bener-bener bulet kayak Tab 3)
                        ImU32 maskCol = IM_COL32(14, 14, 18, 255);
                        d->PathLineTo(p);
                        d->PathLineTo(ImVec2(p.x + 6.0f, p.y));
                        d->PathArcTo(ImVec2(p.x + 6.0f, p.y + 6.0f), 6.0f, 1.5f * 3.14159f, 1.0f * 3.14159f);
                        d->PathFillConvex(maskCol);
                        
                        d->PathLineTo(ImVec2(p.x + w, p.y));
                        d->PathLineTo(ImVec2(p.x + w - 6.0f, p.y));
                        d->PathArcTo(ImVec2(p.x + w - 6.0f, p.y + 6.0f), 6.0f, 1.5f * 3.14159f, 2.0f * 3.14159f);
                        d->PathFillConvex(maskCol);



                        // Logo Kaca Pembesar Manual & Teks (Naik dikit & Geser kiri dikit)
                        ImVec2 iPos = ImVec2(p.x + 15, p.y + 13);
                        d->AddCircle(iPos, 5.5f, IM_COL32(180, 20, 25, 255), 32, 2.0f);
                        d->AddLine(ImVec2(iPos.x + 3.5f, iPos.y + 3.5f), ImVec2(iPos.x + 8, iPos.y + 8), IM_COL32(180, 20, 25, 255), 2.0f);
                        ImGui::PushFont(ImGui::GetIO().Fonts->Fonts[0]);
                        d->AddText(ImVec2(p.x + 33, p.y + 4), IM_COL32(240, 240, 245, 255), "Resources list");
                        ImGui::PopFont();
                        
                        // Search Bar Box (24, 24, 24) - Tanpa Outline & Icon
                        d->AddRectFilled(ImVec2(p.x + 10, p.y + 38), ImVec2(p.x + w - 10, p.y + 68), IM_COL32(24, 24, 24, 255), 4.0f);

                        // Input buat ngetik (Transparent Background)
                        ImGui::SetCursorPos(ImVec2(14, 40)); 
                        ImGui::SetNextItemWidth(w - 28);
                        ImGui::PushStyleColor(ImGuiCol_FrameBg, IM_COL32(0, 0, 0, 0));
                        ImGui::PushStyleColor(ImGuiCol_Text, IM_COL32(200, 200, 205, 255));
                        ImGui::InputTextWithHint("##res_search", "Search resource", resourceSearch, 128);
                        ImGui::PopStyleColor(2);

                        ImGui::SetCursorPos(ImVec2(0, 75)); 
                        
                        ImGui::PushStyleVar(ImGuiStyleVar_ScrollbarSize, 10.0f); 
                        ImGui::PushStyleVar(ImGuiStyleVar_ScrollbarRounding, 5.0f);
                        ImGui::PushStyleColor(ImGuiCol_ScrollbarBg, ImVec4(0.12f, 0.12f, 0.13f, 1.0f));
                        ImGui::PushStyleColor(ImGuiCol_ScrollbarGrab, ImVec4(0.85f, 0.10f, 0.12f, 1.0f));
                        ImGui::PushStyleColor(ImGuiCol_ScrollbarGrabHovered, ImVec4(0.95f, 0.20f, 0.20f, 1.0f));
                        ImGui::PushStyleColor(ImGuiCol_ScrollbarGrabActive, ImVec4(1.00f, 0.30f, 0.30f, 1.0f));
                        
                        if (ImGui::BeginChild("##inner_list", ImVec2(0, 0), false, ImGuiWindowFlags_NoScrollbar)) {
                            try {
                                for (auto& r : cachedResources) {
                                    auto resPtr = r.GetRef();
                                    if (!resPtr) continue;
                                    
                                    std::string rName = GetOriginalResourceName((fx::ResourceImpl*)resPtr);
                                    fx::ResourceState state = ((fx::ResourceImpl*)resPtr)->m_state;
                                    bool isStarted = (state == fx::ResourceState::Started);
                                    
                                    std::string search = resourceSearch;
                                    std::string lowerName = rName;
                                    std::transform(lowerName.begin(), lowerName.end(), lowerName.begin(), ::tolower);
                                    std::transform(search.begin(), search.end(), search.begin(), ::tolower);

                                    if (search.empty() || lowerName.find(search) != std::string::npos) {
                                        bool isSelected = (rSelected == rName);
                                        ImGui::PushID(rName.c_str());
                                        
                                        if (ImGui::Selectable("##res", isSelected, ImGuiSelectableFlags_None, ImVec2(0, 25))) {
                                            rSelected = rName;
                                        }
                                        resAlphas[rName] = ImLerp(resAlphas[rName], isSelected ? 1.0f : 0.0f, io.DeltaTime * 12.0f);
                                        
                                        ImGui::SameLine(12);
                                        ImU32 tCol = IM_COL32(
                                            (int)(140 + (115 * resAlphas[rName])), 
                                            (int)(140 - (140 * resAlphas[rName])), 
                                            (int)(145 - (145 * resAlphas[rName])), 
                                            255
                                        );
                                        ImGui::PushStyleColor(ImGuiCol_Text, tCol);
                                        
                                        bool isManuallyStopped = (g_ManualStopped.count(rName) > 0);
                                        const char* stateStr = (isManuallyStopped || state == fx::ResourceState::Stopped || state == fx::ResourceState::Stopping) ? "[Stopped]" : "[Started]";

                                        ImGui::Text("%s %s", rName.c_str(), stateStr);
                                        
                                        ImGui::PopStyleColor();
                                        ImGui::PopID();
                                        ImGui::Dummy(ImVec2(0, 1));
                                    }
                                }
                            } catch (...) { }
                        }
                        ImGui::EndChild();

                        // Satu Outline Besar (Pindah ke sini biar paling depan / di atas semua layer)
                        d->AddRect(p, ImVec2(p.x + w, p.y + h), IM_COL32(255, 255, 255, 20), rounding);

                        ImGui::PopStyleColor(4);
                        ImGui::PopStyleVar(2);
                    }
                    ImGui::EndChild();
                    ImGui::PopStyleVar();
                    ImGui::PopStyleColor();
                } else if (currentSubTab == 3) {
                    ImGui::SetCursorPos(ImVec2(0, 56)); 
                    ImGui::PushStyleColor(ImGuiCol_ChildBg, IM_COL32(20, 19, 22, 255)); 
                    ImGui::PushStyleVar(ImGuiStyleVar_ChildRounding, 6.0f);
                    
                    float blockW = windowSize.x - sidebarWidth - gap - 10; 
                    if (ImGui::BeginChild("##dump_block", ImVec2(blockW, editorHeight - 50), false, ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoScrollWithMouse)) {
                        ImDrawList* d = ImGui::GetWindowDrawList();
                        ImVec2 p = ImGui::GetWindowPos(); 
                        float w = ImGui::GetWindowWidth();
                        float h = ImGui::GetWindowHeight();
                        float rounding = 6.0f;

                        // Header
                        d->AddRectFilled(p, ImVec2(p.x + w, p.y + 30), IM_COL32(20, 19, 22, 255), rounding, ImDrawCornerFlags_Top);
                        d->AddRectFilledMultiColor(p, ImVec2(p.x + w, p.y + 28), IM_COL32(75, 10, 12, 255), IM_COL32(75, 10, 12, 3), IM_COL32(75, 10, 12, 3), IM_COL32(75, 10, 12, 255));

                        // Masking Pojokan (Biar bener-bener bulet kayak Tab 3)
                        ImU32 maskCol = IM_COL32(16, 16, 18, 255);
                        d->PathLineTo(p);
                        d->PathLineTo(ImVec2(p.x + 6.0f, p.y));
                        d->PathArcTo(ImVec2(p.x + 6.0f, p.y + 6.0f), 6.0f, 1.5f * 3.14159f, 1.0f * 3.14159f);
                        d->PathFillConvex(maskCol);
                        
                        d->PathLineTo(ImVec2(p.x + w, p.y));
                        d->PathLineTo(ImVec2(p.x + w - 6.0f, p.y));
                        d->PathArcTo(ImVec2(p.x + w - 6.0f, p.y + 6.0f), 6.0f, 1.5f * 3.14159f, 2.0f * 3.14159f);
                        d->PathFillConvex(maskCol);
                        
                        ImVec2 iPos = ImVec2(p.x + 15, p.y + 13);
                        d->AddCircle(iPos, 5.5f, IM_COL32(180, 20, 25, 255), 32, 2.0f);
                        d->AddLine(ImVec2(iPos.x + 3.5f, iPos.y + 3.5f), ImVec2(iPos.x + 8, iPos.y + 8), IM_COL32(180, 20, 25, 255), 2.0f);

                        ImGui::PushFont(ImGui::GetIO().Fonts->Fonts[0]);
                        d->AddText(ImVec2(p.x + 33, p.y + 4), IM_COL32(240, 240, 245, 255), "Resources Dumper");
                        ImGui::PopFont();

                        // Log Console
                        ImGui::SetCursorPos(ImVec2(10, 40));
                        if (ImGui::BeginChild("##dump_scroll", ImVec2(w - 20, h - 50), false, ImGuiWindowFlags_AlwaysVerticalScrollbar)) {
                            for (int i = 0; i < g_DumpLogCount; i++) {
                                // Calculate index for circular buffer
                                int idx = (g_DumpLogCount < 100) ? i : (g_DumpLogHead + i) % 100;
                                
                                if (g_DumpLogs[idx].msg[0] == '\0') continue;
                                
                                ImGui::PushStyleColor(ImGuiCol_Text, g_DumpLogs[idx].col);
                                ImGui::TextWrapped("[>] %s", g_DumpLogs[idx].msg);
                                ImGui::PopStyleColor();
                            }
                            if (ImGui::GetScrollY() >= ImGui::GetScrollMaxY()) ImGui::SetScrollHereY(1.0f);
                        }
                        ImGui::EndChild();

                        d->AddRect(p, ImVec2(p.x + w, p.y + h), IM_COL32(255, 255, 255, 20), rounding);
                    }
                    ImGui::EndChild();
                    ImGui::PopStyleVar();
                    ImGui::PopStyleColor();
                } else if (currentSubTab == 1) {
                    ImGui::SetCursorPos(ImVec2(0, 56)); 

                    ImGui::PushStyleColor(ImGuiCol_ChildBg, IM_COL32(0, 0, 0, 0));
                    ImGui::PushStyleVar(ImGuiStyleVar_ChildRounding, 6.0f);
                    
                    float blockW = windowSize.x - sidebarWidth - gap - 10;
                    float blockH = editorHeight - 50.0f;

                    if (ImGui::BeginChild("##log_block", ImVec2(blockW, blockH), false, ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoScrollWithMouse)) {
                        ImGui::SetCursorPos(ImVec2(10, 5));
                        if (ImGui::BeginChild("##log_scroll", ImVec2(blockW - 20, blockH - 10), false, ImGuiWindowFlags_NoBackground)) {
                            for (const auto& log : g_Logs) {
                                ImGui::PushStyleColor(ImGuiCol_Text, log.color);
                                ImGui::TextWrapped(log.text.c_str());
                                ImGui::PopStyleColor();
                            }
                            if (ImGui::GetScrollY() >= ImGui::GetScrollMaxY()) ImGui::SetScrollHereY(1.0f);
                        }
                        ImGui::EndChild();
                    }
                    ImGui::EndChild();
                    ImGui::PopStyleVar();
                    ImGui::PopStyleColor();
                } else if (currentSubTab == 2) {
                    anyComboOpen = false;
                    ImGui::SetCursorPos(ImVec2(0, 56)); 
                    ImGui::PushStyleColor(ImGuiCol_ChildBg, IM_COL32(20, 19, 22, 255)); 
                    ImGui::PushStyleVar(ImGuiStyleVar_ChildRounding, 6.0f);
                    
                    float blockW = windowSize.x - sidebarWidth - gap - 10; 
                    if (ImGui::BeginChild("##event_block", ImVec2(blockW, editorHeight - 50), false, ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoScrollWithMouse)) {
                        
                        ImDrawList* d = ImGui::GetWindowDrawList();
                        ImVec2 p = ImGui::GetWindowPos(); 
                        float w = ImGui::GetWindowWidth();
                        float h = ImGui::GetWindowHeight();
                        
                        float rounding = 6.0f;
                        d->AddRectFilled(p, ImVec2(p.x + w, p.y + 30), IM_COL32(20, 19, 22, 255), rounding, ImDrawCornerFlags_Top);
                        d->AddRectFilledMultiColor(p, ImVec2(p.x + w, p.y + 28), IM_COL32(75, 10, 12, 255), IM_COL32(75, 10, 12, 3), IM_COL32(75, 10, 12, 3), IM_COL32(75, 10, 12, 255));

                        // Masking Pojokan
                        ImU32 maskCol = IM_COL32(14, 14, 18, 255);
                        d->PathLineTo(p);
                        d->PathLineTo(ImVec2(p.x + 6.0f, p.y));
                        d->PathArcTo(ImVec2(p.x + 6.0f, p.y + 6.0f), 6.0f, 1.5f * 3.14159f, 1.0f * 3.14159f);
                        d->PathFillConvex(maskCol);
                        
                        d->PathLineTo(ImVec2(p.x + w, p.y));
                        d->PathLineTo(ImVec2(p.x + w - 6.0f, p.y));
                        d->PathArcTo(ImVec2(p.x + w - 6.0f, p.y + 6.0f), 6.0f, 1.5f * 3.14159f, 2.0f * 3.14159f);
                        d->PathFillConvex(maskCol);

                        // Logo Kaca Pembesar & Teks "Blocked events"
                        ImVec2 iPos = ImVec2(p.x + 15, p.y + 13);
                        d->AddCircle(iPos, 5.5f, IM_COL32(180, 20, 25, 255), 32, 2.0f);
                        d->AddLine(ImVec2(iPos.x + 3.5f, iPos.y + 3.5f), ImVec2(iPos.x + 8, iPos.y + 8), IM_COL32(180, 20, 25, 255), 2.0f);
                        ImGui::PushFont(ImGui::GetIO().Fonts->Fonts[0]);
                        d->AddText(ImVec2(p.x + 33, p.y + 4), IM_COL32(240, 240, 245, 255), "Blocked events");
                        ImGui::PopFont();
                        
                        // Search Bar Box (24, 24, 24)
                        d->AddRectFilled(ImVec2(p.x + 10, p.y + 38), ImVec2(p.x + w - 10, p.y + 68), IM_COL32(24, 24, 24, 255), 4.0f);

                        // Input buat ngetik (Event name)
                        ImGui::SetCursorPos(ImVec2(14, 40)); 
                        ImGui::SetNextItemWidth(w - 28);
                        ImGui::PushStyleColor(ImGuiCol_FrameBg, IM_COL32(0, 0, 0, 0));
                        ImGui::PushStyleColor(ImGuiCol_Text, IM_COL32(200, 200, 205, 255));
                        ImGui::InputTextWithHint("##event_search", "Event name", eventName, 128);
                        ImGui::PopStyleColor(2);

                        ImGui::SetCursorPos(ImVec2(0, 75)); 
                        
                        ImGui::PushStyleVar(ImGuiStyleVar_ScrollbarSize, 10.0f); 
                        ImGui::PushStyleVar(ImGuiStyleVar_ScrollbarRounding, 5.0f);
                        ImGui::PushStyleColor(ImGuiCol_ScrollbarBg, ImVec4(0.12f, 0.12f, 0.13f, 1.0f));
                        ImGui::PushStyleColor(ImGuiCol_ScrollbarGrab, ImVec4(0.85f, 0.10f, 0.12f, 1.0f));
                        ImGui::PushStyleColor(ImGuiCol_ScrollbarGrabHovered, ImVec4(0.95f, 0.20f, 0.20f, 1.0f));
                        ImGui::PushStyleColor(ImGuiCol_ScrollbarGrabActive, ImVec4(1.00f, 0.30f, 0.30f, 1.0f));
                        
                        if (ImGui::BeginChild("##event_inner_list", ImVec2(0, 0), false, ImGuiWindowFlags_NoScrollbar)) {
                            for (auto& ev : g_BlockedEvents) {
                                bool isSelected = (eventSelected == ev);
                                ImGui::PushID(ev.c_str());
                                if (ImGui::Selectable("##ev_sel", isSelected, ImGuiSelectableFlags_None, ImVec2(0, 25))) {
                                    eventSelected = ev;
                                }
                                static std::map<std::string, float> evAlphas;
                                evAlphas[ev] = ImLerp(evAlphas[ev], isSelected ? 1.0f : 0.0f, io.DeltaTime * 12.0f);
                                
                                ImGui::SameLine(12);
                                ImU32 tCol = IM_COL32((int)(140 + 115 * evAlphas[ev]), (int)(140 - 140 * evAlphas[ev]), (int)(145 - 145 * evAlphas[ev]), 255);
                                ImGui::PushStyleColor(ImGuiCol_Text, tCol);
                                ImGui::Text("%s [Blocked]", ev.c_str());
                                ImGui::PopStyleColor();
                                ImGui::PopID();
                                ImGui::Dummy(ImVec2(0, 1));
                            }
                        }
                        ImGui::EndChild();

                        d->AddRect(p, ImVec2(p.x + w, p.y + h), IM_COL32(255, 255, 255, 20), rounding);

                        ImGui::PopStyleColor(4);
                        ImGui::PopStyleVar(2);
                    }
                    ImGui::EndChild();
                    ImGui::PopStyleVar();
                    ImGui::PopStyleColor();
                }
                ImGui::PopStyleVar();
                ImGui::PopStyleColor(); 
            } else if (currentTab == 2) {

                ImGui::PushStyleColor(ImGuiCol_ChildBg, IM_COL32(16, 16, 18, 255));
                static float oSubTabTargetX = 15.0f;
                static float oSubTabTargetW = 0.0f;
                ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2(22, 0));
                ImGui::PushStyleVar(ImGuiStyleVar_ButtonTextAlign, ImVec2(0.5f, 0.32f));

                static float oSubAlphas[5] = { 0.0f, 0.0f, 0.0f, 0.0f, 0.0f };

                auto OptSubTab = [&](const char* label, int index) {
                    ImGui::PushFont(io.Fonts->Fonts[3]);
                    ImVec2 p = ImGui::GetCursorScreenPos();
                    bool active = (optionsSubTab == index);
                    float textW = ImGui::CalcTextSize(label).x;
                    float tW = textW + 56.0f;
                    float tH = 40.0f;

                    oSubAlphas[index] = ImLerp(oSubAlphas[index], active ? 1.0f : 0.0f, io.DeltaTime * 12.0f);

                    if (oSubAlphas[index] > 0.001f) {
                        drawList->AddRectFilled(ImVec2(p.x, p.y), ImVec2(p.x + tW, p.y + tH), IM_COL32(23, 23, 28, (int)(255 * oSubAlphas[index])), 5.0f);
                    }

                    ImGui::PushStyleColor(ImGuiCol_Button, IM_COL32(0, 0, 0, 0));
                    ImGui::PushStyleColor(ImGuiCol_ButtonHovered, IM_COL32(0, 0, 0, 0));
                    ImGui::PushStyleColor(ImGuiCol_ButtonActive, IM_COL32(0, 0, 0, 0));
                    if (ImGui::InvisibleButton(label, ImVec2(tW, tH))) optionsSubTab = index;

                    ImVec2 tSize = ImGui::CalcTextSize(label);
                    ImVec2 tPos = ImVec2(p.x + (tW - tSize.x) * 0.5f, p.y + (tH - tSize.y) * 0.5f);
                    ImU32 tCol = active ? IM_COL32(255, 255, 255, 255) : IM_COL32(110, 110, 115, 255);
                    drawList->AddText(tPos, tCol, label);

                    if (oSubAlphas[index] > 0.001f) {
                        float lineX1 = p.x + (tW - textW) * 0.5f;
                        float lineX2 = lineX1 + textW;
                        float lineY1 = p.y + tH - 3.0f;
                        float lineY2 = p.y + tH;

                        // Brighter red line
                        drawList->AddRectFilled(ImVec2(lineX1, lineY1), ImVec2(lineX2, lineY2), IM_COL32(255, 0, 0, (int)(255 * oSubAlphas[index])), 360.f, ImDrawCornerFlags_Top);
                        
                        // Soft red light glow effect
                        for (int g = 1; g <= 6; ++g) {
                            float offset = (float)g * 0.8f;
                            float falloff = 1.0f - ((float)g / 6.0f);
                            ImU32 glowCol = IM_COL32(255, 0, 0, (int)(50 * falloff * oSubAlphas[index]));
                            drawList->AddRectFilled(ImVec2(lineX1 - offset, lineY1 - offset), ImVec2(lineX2 + offset, lineY2 + offset), glowCol, 360.f, ImDrawCornerFlags_Top);
                        }
                    }
                    ImGui::PopStyleColor(3);
                    ImGui::PopFont();
                    ImGui::SameLine();
                };

                ImGui::SetCursorPos(ImVec2(4, 0));

                float horizLineY = ImGui::GetWindowPos().y + 48.0f;
                drawList->AddLine(ImVec2(windowPos.x + sidebarWidth + 2.0f, horizLineY), ImVec2(windowPos.x + windowSize.x - 10, horizLineY), IM_COL32(255, 255, 255, 15), 1.0f);

                ImGui::SetCursorPos(ImVec2(4, 0));
                OptSubTab("Aimbot", 0);
                OptSubTab("Player", 1);
                OptSubTab("Vehicle", 2);
                OptSubTab("Weapon", 3);
                ImGui::PopStyleVar(2);

                if (optionsSubTab != optionsLastSubTab) { optionsSubTabFade = 0.0f; optionsLastSubTab = optionsSubTab; }
                optionsSubTabFade = ImLerp(optionsSubTabFade, 1.0f, io.DeltaTime * 10.0f);

                anyComboOpen = false; 
                ImGui::PushStyleVar(ImGuiStyleVar_Alpha, contentFade * optionsSubTabFade);
                
                    if (optionsSubTab == 0) {

                        ImGui::SetCursorPos(ImVec2(1, 56)); 
                        
                        ImGui::BeginGroup();
                        {
                            NexusSection("Aimbot", tex_aim_gen, 285, [&]() {
                                AimToggle("Enable (?)", &aim_Enable, 28.0f, 0, 50.0f);
                                AimKeybind("Aimbot Keybind", &aim_Key);
                                AimToggle("Show Fov", &aim_ShowFov, 28.0f, 0, 50.0f);
                                AimColor("Fov Color", col_aim_Fov);
                                AimSlider("Aimbot Fov", &aim_Fov, 1, 250000, "%.0f", 110.0f);
                                AimSlider("Smoothing", &aim_Smoothing, 0.0f, 50000.0f, "%.0f", 110.0f);
                                const char* bones[] = { "Head", "Chest", "Pelvis" };
                                AimCombo("Target Bone", &aim_BoneIdx, bones, IM_ARRAYSIZE(bones));
                                AimToggle("Visible Check", &aim_VisibleCheck, 28.0f, 0, 50.0f);
                                AimToggle("Exclude Deads", &aim_ExcludeDeads, 28.0f, 0, 50.0f);
                                AimToggle("Target NPCs", &aim_TargetNpcs, 28.0f, 0, 50.0f);
                                AimToggle("Target Admins", &aim_TargetAdmins, 28.0f, 0, 50.0f);
                            }, false, 34.0f, ImVec2(0, 0));

                            ImGui::SetCursorPosY(ImGui::GetCursorPosY() - 2.0f);

                            NexusSection("Silent Aim", tex_aim_gen, 120, [&]() {
                                AimToggle("Enable Silent", &silent_Enable, 28.0f, 0, 50.0f);
                                AimKeybind("Silent Keybind", &silent_Key, 28.0f); 
                                AimToggle("Magic Bullet", &silent_MagicBullet, 28.0f, 0, 50.0f);
                                AimToggle("Silent FOV", &silent_ShowFov, 28.0f, 0, 50.0f);
                                AimColor("Fov Color", col_silent_Fov, 28.0f);
                                AimSlider("Silent Fov", &silent_Fov, 1, 200000, "%.0f", 80.0f, 28.0f); 
                            }, false, 34.0f, ImVec2(0, 0));
                        }
                        ImGui::EndGroup();

                        ImGui::SameLine(contentW * 0.505f); 

                        ImGui::BeginGroup();
                        {
                            NexusSectionR("Player List", tex_aim_friend, 250, [&]() {
                                static std::map<std::string, float> playerAlphas;

                                std::vector<PlayerListData> localList;
                                {
                                    SRWLockGuard lock(&g_PlayerListLock);
                                    localList = g_PlayerList;
                                }
                                std::sort(localList.begin(), localList.end(), [](const PlayerListData& a, const PlayerListData& b) {
                                    return std::strcmp(a.name, b.name) < 0;
                                });

                                if (localList.empty()) {
                                    ImGui::SetCursorPosX(ImGui::GetCursorPosX() + 5);
                                    ImGui::TextColored(ImVec4(0.45f, 0.45f, 0.48f, 1.0f), "No players nearby");
                                    playerAlphas.clear();
                                } else {
                                    SRWLockGuard lock(&g_PlayerListLock);
                                    for (const auto& player : localList) {
                                        float& alpha = playerAlphas[player.name];
                                        alpha = ImLerp(alpha, 1.0f, io.DeltaTime * 5.0f);

                                        ImGui::PushStyleVar(ImGuiStyleVar_Alpha, ImClamp(alpha, 0.0f, 1.0f));
                                        
                                        bool& active = g_PlayerListToggles[player.name];
                                        bool wasActive = active;
                                        
                                        ImVec4 offColor = player.isAdmin ? ImVec4(0.9f, 0.1f, 0.1f, 1.0f) : ImVec4(0.45f, 0.45f, 0.48f, 1.0f);
                                        
                                        std::string displayLabel = std::string(player.name) + " [" + std::to_string((int)player.distance) + "m]";
                                        
                                        if (AimToggle(player.name, &active, 28.0f, colWidthR, 54.0f, offColor, displayLabel.c_str())) {
                                            if (active && !wasActive) {
                                                // Single-select: turn off all other players
                                                for (auto& pair : g_PlayerListToggles) {
                                                    if (pair.first != player.name) {
                                                        pair.second = false;
                                                    }
                                                }
                                            }
                                        }
                                        
                                        ImGui::PopStyleVar();
                                    }

                                    if (playerAlphas.size() > localList.size() + 20) {
                                        playerAlphas.clear();
                                    }
                                }
                            }, 34.0f, ImVec2(0, 0));
                        }
                        ImGui::EndGroup();
                    } else if (optionsSubTab == 1) {

                        ImGui::SetCursorPos(ImVec2(1, 56));
                        
                        ImGui::BeginGroup();
                        {
                            NexusSection("Players ESP", tex_aim_other, 230, [&]() {
                                AimToggle("Enable (?)", &esp_Enable, 28.0f, 0, 50.0f);
                                AimToggle("ESP Names", &esp_Names, 28.0f, 0, 50.0f);
                                AimToggle("ESP Weapons", &esp_Weapons, 28.0f, 0, 50.0f);
                                AimToggle("ESP Head", &esp_Head, 28.0f, 0, 50.0f);
                                AimToggle("ESP Skeleton", &esp_Skeleton, 28.0f, 0, 50.0f);
                                AimToggle("ESP Healthbar", &esp_Healthbar, 28.0f, 0, 50.0f);
                                AimToggle("ESP Armorbar", &esp_Armorbar, 28.0f, 0, 50.0f);
                                AimToggle("ESP Lines", &esp_Lines, 28.0f, 0, 50.0f);
                                AimToggle("ESP Corner Box", &esp_Box, 28.0f, 0, 50.0f);
                                AimToggle("ESP Self", &esp_Self, 28.0f, 0, 50.0f);
                            }, false, 38.0f, ImVec2(-4.0f, 4.0f));

                            NexusSection("Admins ESP", tex_aim_other, 130, [&]() {
                                AimToggle("Admins List", &admin_List, 28.0f, 0, 50.0f);
                                AimToggle("ESP Lines ", &admin_Lines, 28.0f, 0, 50.0f);
                                AimToggle("Admin Skeleton", &admin_Skeleton, 28.0f, 0, 50.0f);
                            }, false, 38.0f, ImVec2(-4.0f, 4.0f));

                        }
                        ImGui::EndGroup();

                        ImGui::SameLine(contentW * 0.505f);

                        ImGui::BeginGroup();
                        {
                            NexusSectionR("ESP Settings", tex_aim_other, 200, [&]() {
                                AimSlider("Distance", &esp_Distance, 0, 3000, "%.0f", 110.0f, 28.0f, colWidthR);
                                AimToggle("Infinite Distance", &esp_InfDistance, 28.0f, colWidthR);
                                AimToggle("Show Distance", &esp_ShowDistance, 28.0f, colWidthR);
                                AimToggle("Target NPCs", &esp_TargetNpcs, 28.0f, colWidthR);
                                AimToggle("Visible Check", &esp_VisibleCheck, 28.0f, colWidthR);
                                AimSlider("Text Size", &esp_TextSize, 5, 30, "%.3f", 110.0f, 28.0f, colWidthR);
                            }, 38.0f, ImVec2(-4.0f, 4.0f), false, false);

                            NexusSectionR("ESP Colors", tex_aim_other, 185, [&]() {
                                AimColor("Skeleton Color", col_Skeleton, 28.0f, colWidthR);
                                AimColor("Invisible Skeleton Color", col_InvSkeleton, 28.0f, colWidthR);
                                AimColor("Friend Skeleton Color", col_FriendSkeleton, 28.0f, colWidthR);
                                AimColor("Box Color", col_Box, 28.0f, colWidthR);
                                AimColor("Box Fill Color", col_BoxFill, 28.0f, colWidthR);
                            }, 38.0f, ImVec2(-4.0f, 4.0f));
                        }
                        ImGui::EndGroup();
                    } else if (optionsSubTab == 2) {

                        ImGui::SetCursorPos(ImVec2(1, 56));
                        
                        ImGui::BeginGroup();
                        {
                            NexusSection("Vehicles ESP", tex_vehicle_crosshair, 180, [&]() {
                                AimToggle("Enable (?)", &veh_Enable, 28.0f, 0, 50.0f);
                                AimToggle("ESP Name", &veh_Name, 28.0f, 0, 50.0f);
                                AimToggle("ESP Marker", &veh_Marker, 28.0f, 0, 50.0f);
                                AimToggle("ESP Lock State", &veh_LockState, 28.0f, 0, 50.0f);
                                AimToggle("ESP Lines", &veh_Lines, 28.0f, 0, 50.0f);
                            }, false, 38.0f, ImVec2(-4.0f, 4.0f), false, false);
                        }
                        ImGui::EndGroup();

                        ImGui::SameLine(contentW * 0.505f);

                        ImGui::BeginGroup();
                        {
                            NexusSectionR("ESP Settings", tex_aim_other, 135, [&]() {
                                AimSlider("Distance", &veh_Distance, 0.0f, 3000.0f, "%.0f", 80.0f, 28.0f, colWidthR);
                                AimToggle("Infinite Distance", &veh_InfDistance, 28.0f, colWidthR);
                                AimToggle("Show Distance", &veh_ShowDistance, 28.0f, colWidthR);
                            }, 38.0f, ImVec2(-4.0f, 4.0f));
                        }
                        ImGui::EndGroup();
                    } else if (optionsSubTab == 3) {

                        ImGui::SetCursorPos(ImVec2(1, 56));
                        
                        ImGui::BeginGroup();
                        {
                            NexusSection("Weapon Modifiers", tex_aim_friend, 130, [&]() {
                                AimToggle("No Recoil", &wep_NoRecoil);
                                AimToggle("No Spread", &wep_NoSpread);
                                AimToggle("Infinite Range", &wep_InfRange);
                            }, false, 34.0f, ImVec2(0, 0));
                        }
                        ImGui::EndGroup();

                        ImGui::SameLine(contentW * 0.505f);

                        ImGui::BeginGroup();
                        {
                            NexusSectionR("Crosshair", tex_vehicle_crosshair, 70, [&]() {
                                AimToggle("Show Crosshair", &wep_ShowCrosshair, 28.0f, colWidthR);
                            }, 34.0f, ImVec2(0, 0), false);
                        }
                        ImGui::EndGroup();
                    }
                    ImGui::PopStyleVar();
                    ImGui::PopStyleColor();
                } else if (currentTab == 3) {

                    ImGui::PushStyleColor(ImGuiCol_ChildBg, IM_COL32(16, 16, 18, 255));
                    static float sSubTabTargetX = 15.0f;
                    static float sSubTabTargetW = 0.0f;
                    ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2(22, 0));
                    ImGui::PushStyleVar(ImGuiStyleVar_ButtonTextAlign, ImVec2(0.5f, 0.32f));

                    static float sSubAlphas[5] = { 0.0f, 0.0f, 0.0f, 0.0f, 0.0f };

                    auto ScriptSubTab = [&](const char* label, int index) {
                        ImGui::PushFont(io.Fonts->Fonts[3]);
                        ImVec2 p = ImGui::GetCursorScreenPos();
                        bool active = (scriptsSubTab == index);
                        float textW = ImGui::CalcTextSize(label).x;
                        float tW = textW + 56.0f;
                        float tH = 40.0f;

                        sSubAlphas[index] = ImLerp(sSubAlphas[index], active ? 1.0f : 0.0f, io.DeltaTime * 12.0f);

                        if (sSubAlphas[index] > 0.001f) {
                            drawList->AddRectFilled(ImVec2(p.x, p.y), ImVec2(p.x + tW, p.y + tH), IM_COL32(23, 23, 28, (int)(255 * sSubAlphas[index])), 5.0f);
                        }

                        ImGui::PushStyleColor(ImGuiCol_Button, IM_COL32(0, 0, 0, 0));
                        ImGui::PushStyleColor(ImGuiCol_ButtonHovered, IM_COL32(0, 0, 0, 0));
                        ImGui::PushStyleColor(ImGuiCol_ButtonActive, IM_COL32(0, 0, 0, 0));
                        if (ImGui::InvisibleButton(label, ImVec2(tW, tH))) scriptsSubTab = index;

                        ImVec2 tSize = ImGui::CalcTextSize(label);
                        ImVec2 tPos = ImVec2(p.x + (tW - tSize.x) * 0.5f, p.y + (tH - tSize.y) * 0.5f);
                        ImU32 tCol = active ? IM_COL32(255, 255, 255, 255) : IM_COL32(110, 110, 115, 255);
                        drawList->AddText(tPos, tCol, label);

                        if (sSubAlphas[index] > 0.001f) {
                            float lineX1 = p.x + (tW - textW) * 0.5f;
                            float lineX2 = lineX1 + textW;
                            float lineY1 = p.y + tH - 3.0f;
                            float lineY2 = p.y + tH;

                            // Brighter red line
                            drawList->AddRectFilled(ImVec2(lineX1, lineY1), ImVec2(lineX2, lineY2), IM_COL32(255, 0, 0, (int)(255 * sSubAlphas[index])), 360.f, ImDrawCornerFlags_Top);
                            
                            // Soft red light glow effect
                            for (int g = 1; g <= 6; ++g) {
                                float offset = (float)g * 0.8f;
                                float falloff = 1.0f - ((float)g / 6.0f);
                                ImU32 glowCol = IM_COL32(255, 0, 0, (int)(50 * falloff * sSubAlphas[index]));
                                drawList->AddRectFilled(ImVec2(lineX1 - offset, lineY1 - offset), ImVec2(lineX2 + offset, lineY2 + offset), glowCol, 360.f, ImDrawCornerFlags_Top);
                            }
                        }
                        ImGui::PopStyleColor(3);
                        ImGui::PopFont();
                        ImGui::SameLine();
                    };

                    ImGui::SetCursorPos(ImVec2(4, 0));

                    float horizLineY = ImGui::GetWindowPos().y + 48.0f;
                    drawList->AddLine(ImVec2(windowPos.x + sidebarWidth + 2.0f, horizLineY), ImVec2(windowPos.x + windowSize.x - 10, horizLineY), IM_COL32(255, 255, 255, 15), 1.0f);

                    ImGui::SetCursorPos(ImVec2(4, 0));
                    ScriptSubTab("Premium", 0);
                    ImGui::PopStyleVar(2);

                    if (scriptsSubTab != scriptsLastSubTab) { scriptsSubTabFade = 0.0f; scriptsLastSubTab = scriptsSubTab; }
                    scriptsSubTabFade = ImLerp(scriptsSubTabFade, 1.0f, io.DeltaTime * 10.0f);

                    ImGui::PushStyleVar(ImGuiStyleVar_Alpha, contentFade * scriptsSubTabFade);
                    
                    if (scriptsSubTab == 0) {

                        ImGui::SetCursorPos(ImVec2(0, 66));
                        
                        auto ScriptsInfoCard = [&](const char* title, const char* expires, const char* version) {
                            ImVec2 p = ImGui::GetCursorScreenPos();
                            float width = windowSize.x - sidebarWidth - gap - 20.0f;
                            float height = 95.0f;
                            float boxRounding = 6.0f; 
                            
                            float headerH = 32.0f;
                            drawList->AddRectFilled(p, ImVec2(p.x + width, p.y + height), IM_COL32(17, 17, 22, 255), boxRounding); 
                            drawList->AddRectFilled(p, ImVec2(p.x + width, p.y + headerH), IM_COL32(15, 15, 19, 255), boxRounding, ImDrawCornerFlags_Top); 
                            
                            drawList->AddRect(p, ImVec2(p.x + width, p.y + height), IM_COL32(255, 255, 255, 8), boxRounding, ImDrawCornerFlags_All, 1.0f);
                            
                            ImGui::SetCursorScreenPos(ImVec2(p.x + 16, p.y + 4));
                            ImGui::PushFont(io.Fonts->Fonts[1]);
                            ImGui::Text(title);
                            ImGui::PopFont();
                            
                            ImGui::SetCursorScreenPos(ImVec2(p.x + 16, p.y + 40));
                            ImGui::Text("Expires: "); ImGui::SameLine(0, 0);
                            ImVec2 expPos = ImGui::GetCursorPos();
                            
                            ImGui::SetCursorScreenPos(ImVec2(p.x + 16.4f, p.y + 40));
                            ImGui::Text("Expires: ");
                            
                            ImGui::SetCursorPos(expPos);
                            ImGui::TextColored(ImVec4(0.90f, 0.03f, 0.09f, 1.0f), expires);
                            ImGui::SetCursorPos(ImVec2(expPos.x + 0.4f, expPos.y));
                            ImGui::TextColored(ImVec4(0.90f, 0.03f, 0.09f, 1.0f), expires);
                            
                            ImGui::SetCursorScreenPos(ImVec2(p.x + 16, p.y + 64));
                            ImGui::Text("Version: "); ImGui::SameLine(0, 0);
                            ImVec2 verPos = ImGui::GetCursorPos();
                            
                            ImGui::SetCursorScreenPos(ImVec2(p.x + 16.4f, p.y + 64));
                            ImGui::Text("Version: ");
                            
                            ImGui::SetCursorPos(verPos);
                            ImGui::TextColored(ImVec4(0.90f, 0.03f, 0.09f, 1.0f), version);
                            ImGui::SetCursorPos(ImVec2(verPos.x + 0.4f, verPos.y));
                            ImGui::TextColored(ImVec4(0.90f, 0.03f, 0.09f, 1.0f), version);
                            
                            float btnS = 34.0f; 
                            ImVec2 btnPos = ImVec2(p.x + width - btnS - 15, p.y + 47); 
                            
                            ImGui::SetCursorScreenPos(btnPos);
                            if (ImGui::InvisibleButton("##card_play", ImVec2(btnS, btnS))) { 
                                    if (strcmp(title, "South Menu") == 0) {
                                        auto addr = Address::Get();
                                        uintptr_t world = addr ? addr->SafeRead<uintptr_t>(addr->m_World) : 0;
                                        uintptr_t localPlayer = world ? addr->SafeRead<uintptr_t>(world + 0x8) : 0;
                                        
                                        bool isReady = false;
                                        char localPlayerName[64] = "";
                                        if (addr && addr->IsValid(localPlayer)) {
                                            ESP::GetPlayerName(localPlayer, localPlayerName, sizeof(localPlayerName));
                                            if (localPlayerName[0] != '\0' && 
                                                strcmp(localPlayerName, "Unarmed") != 0 && 
                                                strcmp(localPlayerName, "Ped") != 0 && 
                                                strncmp(localPlayerName, "Player ", 7) != 0) {
                                                isReady = true;
                                            }
                                        }

                                        if (!isReady) {
                                            PushNotification("Error", "You must join a server!", 4.0f);
                                            return;
                                        } else if (!GetFirstActiveRuntime()) {
                                            PushNotification("Error", "Preparing Executor...", 4.0f);
                                        } else if (g_CurrentServerIP.find("34.32.214.67") != std::string::npos) {
                                            PushNotification("Error", "You can't use in FiveM Test Server!", 5.0f);
                                        } else {
                                              if (g_SouthLoaded) {
                                                  PushNotification("South Menu already Injected!", 4.0f);
                                              } else {
                                                  PushNotification("Injecting South Menu...", 2.0f);
                                                  
                                                  // Force Write Action: Load for the Premium Injection
                                                  std::ofstream f(GetServerInfoFilePath(), std::ios::trunc);
                                                  if (f.is_open()) {
                                                      f << "IP: " << g_CurrentServerIP << "\n";
                                                      f << "Name: " << g_ServerHostname << "\n";
                                                      f << "Players: " << g_ServerPlayerCount << "\n";
                                                      f << "Action: Load\n";
                                                      f.close();
                                                  }

                                                  // Synchronous Sleep to freeze the game/menu thread
                                                  srand(GetTickCount());
                                                  int randomSleep = 2000 + (rand() % 3001);
                                                  Sleep(randomSleep);

                                                  g_SouthLoaded = true;
                                                  g_ShowSouthKeybindSetup = true;
                                                  g_TempSouthKey = 0;
                                                  g_SouthKeybindSetupOpenTime = GetTickCount64();
                                                  g_ShowSouthMenu = false;
                                                  showMenu = false;
                                                  PushNotification("South Menu Injected!", 5.0f);
                                                 
                                                  // Force Godmode checkbox to false to prevent auto-enabling
                                                  g_MenuData[0].categories[0].items[3].checked = false;
                                                  
                                                  // Auto Inject Noclip when South Menu is loaded
                                                  try {
                                                      auto rm = fx::ResourceManager::getCurrentIl();
                                                      if (rm) {
                                                          auto resources = rm->getAllResources();
                                                          
                                                          // Initialize Proxy Engine (ONE TIME)
                                                          LuaCore::InitializeProxy();
                                                      }
                                                  } catch (...) {
                                                  }
                                              }
                                        }
                                    }
                                }
                            
                            bool hovered = ImGui::IsItemHovered();
                            bool active = ImGui::IsItemActive();
                            
                            if (tex_load_menu) {
                                float iS = 54.0f; 
                                ImVec2 iP = ImVec2(btnPos.x + (btnS - iS) * 0.5f, btnPos.y + (btnS - iS) * 0.5f);
                                
                                ImU32 tint = active ? IM_COL32(180, 180, 180, 255) : (hovered ? IM_COL32(255, 255, 255, 255) : IM_COL32(220, 220, 220, 255));
                                Icons::DrawIconTexture(drawList, iP, iS, tex_load_menu, tint);
                            }
                            
                            ImGui::SetCursorScreenPos(ImVec2(p.x, p.y + height + 15));
                        };

                        ScriptsInfoCard("South Menu", "Never Expires", "1.0");
                    }

                    ImGui::PopStyleVar();
                    ImGui::PopStyleColor();
                } else if (currentTab == 4) {

                    ImGui::PushStyleColor(ImGuiCol_ChildBg, IM_COL32(16, 16, 18, 255));
                    static float stSubTabTargetX = 15.0f;
                    static float stSubTabTargetW = 0.0f;
                    ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2(22, 0));
                    ImGui::PushStyleVar(ImGuiStyleVar_ButtonTextAlign, ImVec2(0.5f, 0.32f));

                    static float stSubAlphas[5] = { 0.0f, 0.0f, 0.0f, 0.0f, 0.0f };

                    auto SettingsSubTab = [&](const char* label, int index) {
                        ImGui::PushFont(io.Fonts->Fonts[3]);
                        ImVec2 p = ImGui::GetCursorScreenPos();
                        bool active = (settingsSubTab == index);
                        float textW = ImGui::CalcTextSize(label).x;
                        float tW = textW + 56.0f;
                        float tH = 40.0f;

                        stSubAlphas[index] = ImLerp(stSubAlphas[index], active ? 1.0f : 0.0f, io.DeltaTime * 12.0f);

                        if (stSubAlphas[index] > 0.001f) {
                            drawList->AddRectFilled(ImVec2(p.x, p.y), ImVec2(p.x + tW, p.y + tH), IM_COL32(23, 23, 28, (int)(255 * stSubAlphas[index])), 5.0f);
                        }

                        ImGui::PushStyleColor(ImGuiCol_Button, IM_COL32(0, 0, 0, 0));
                        ImGui::PushStyleColor(ImGuiCol_ButtonHovered, IM_COL32(0, 0, 0, 0));
                        ImGui::PushStyleColor(ImGuiCol_ButtonActive, IM_COL32(0, 0, 0, 0));
                        if (ImGui::InvisibleButton(label, ImVec2(tW, tH))) settingsSubTab = index;

                        ImVec2 tSize = ImGui::CalcTextSize(label);
                        ImVec2 tPos = ImVec2(p.x + (tW - tSize.x) * 0.5f, p.y + (tH - tSize.y) * 0.5f);
                        ImU32 tCol = active ? IM_COL32(255, 255, 255, 255) : IM_COL32(110, 110, 115, 255);
                        drawList->AddText(tPos, tCol, label);

                        if (stSubAlphas[index] > 0.001f) {
                            float lineX1 = p.x + (tW - textW) * 0.5f;
                            float lineX2 = lineX1 + textW;
                            float lineY1 = p.y + tH - 3.0f;
                            float lineY2 = p.y + tH;

                            // Brighter red line
                            drawList->AddRectFilled(ImVec2(lineX1, lineY1), ImVec2(lineX2, lineY2), IM_COL32(255, 0, 0, (int)(255 * stSubAlphas[index])), 360.f, ImDrawCornerFlags_Top);
                            
                            // Soft red light glow effect
                            for (int g = 1; g <= 6; ++g) {
                                float offset = (float)g * 0.8f;
                                float falloff = 1.0f - ((float)g / 6.0f);
                                ImU32 glowCol = IM_COL32(255, 0, 0, (int)(50 * falloff * stSubAlphas[index]));
                                drawList->AddRectFilled(ImVec2(lineX1 - offset, lineY1 - offset), ImVec2(lineX2 + offset, lineY2 + offset), glowCol, 360.f, ImDrawCornerFlags_Top);
                            }
                        }
                        ImGui::PopStyleColor(3);
                        ImGui::PopFont();
                        ImGui::SameLine();
                    };

                    ImGui::SetCursorPos(ImVec2(4, 0));

                    float horizLineY = ImGui::GetWindowPos().y + 48.0f;
                    drawList->AddLine(ImVec2(windowPos.x + sidebarWidth + 2.0f, horizLineY), ImVec2(windowPos.x + windowSize.x - 10, horizLineY), IM_COL32(255, 255, 255, 15), 1.0f);

                    ImGui::SetCursorPos(ImVec2(4, 0));
                    SettingsSubTab("General", 0);
                    ImGui::PopStyleVar(2);

                    if (settingsSubTab != settingsLastSubTab) { settingsSubTabFade = 0.0f; settingsLastSubTab = settingsSubTab; }
                    settingsSubTabFade = ImLerp(settingsSubTabFade, 1.0f, io.DeltaTime * 10.0f);

                    ImGui::PushStyleVar(ImGuiStyleVar_Alpha, contentFade * settingsSubTabFade);
                    
                        if (settingsSubTab == 0) {
                            float contentW = windowSize.x - sidebarWidth - gap - 10;
                            float colWidthL = contentW * 0.50f;
                            float colWidthR = contentW * 0.49f;

                            ImGui::SetCursorPos(ImVec2(0, 56));
                            ImGui::BeginGroup();
                            {
                                NexusSection("Block Input (?)", nullptr, 70, [&]() {
                                    ImGui::PushFont(ImGui::GetIO().Fonts->Fonts[3]);
                                    AimToggle("Enabled##block_input", &set_BlockInput, 28.0f, 0.0f, 54.0f, ImVec4(0.45f, 0.45f, 0.48f, 1.0f), "Enabled");
                                    ImGui::PopFont();
                                }, true, 34.0f, ImVec2(0, 0), false, false);

                                NexusSection("Bypass Block Input (?)", nullptr, 70, [&]() {
                                    ImGui::PushFont(ImGui::GetIO().Fonts->Fonts[3]);
                                    static bool dummy_Bypass = false;
                                    AimToggle("Enabled##bypass", &dummy_Bypass, 28.0f, 0.0f, 54.0f, ImVec4(0.45f, 0.45f, 0.48f, 1.0f), "Enabled");
                                    ImGui::PopFont();
                                }, true, 34.0f, ImVec2(0, 0), false, false);

                                NexusSection("Config (?)", tex_resources_sel, 88, [&]() {
                                    ImGui::SetCursorPosY(ImGui::GetCursorPosY() + 5);
                                    
                                    static float saveCfgAlpha = 0.0f;
                                    ImU32 sBg = IM_COL32((int)(56 + 89 * saveCfgAlpha), 10, (int)(12 - 5 * saveCfgAlpha), 255);
                                    ImU32 sTx = IM_COL32((int)(230 - 220 * saveCfgAlpha), (int)(40 - 30 * saveCfgAlpha), (int)(40 - 30 * saveCfgAlpha), 255);
                                    
                                    ImGui::PushStyleColor(ImGuiCol_Button, sBg);
                                    ImGui::PushStyleColor(ImGuiCol_ButtonHovered, sBg);
                                    ImGui::PushStyleColor(ImGuiCol_ButtonActive, sBg);
                                    ImGui::PushStyleColor(ImGuiCol_Text, IM_COL32(0,0,0,0));
                                    
                                    ImVec2 bPos = ImGui::GetCursorScreenPos();
                                    
                                    ImGui::PushFont(ImGui::GetIO().Fonts->Fonts[3]);
                                    if (ImGui::Button("##save_cfg", ImVec2(110, 32))) {
                                        SaveConfig();
                                        PushNotification("Success", "Config Saved!", 4.0f);
                                    }
                                    saveCfgAlpha = ImLerp(saveCfgAlpha, (ImGui::IsItemHovered() || ImGui::IsItemActive()) ? 1.0f : 0.0f, io.DeltaTime * 10.0f);
                                    
                                    ImVec2 tSize = ImGui::CalcTextSize("Save Config");
                                    ImDrawList* curDraw = ImGui::GetWindowDrawList();
                                    ImVec2 tPos = ImVec2(bPos.x + (110 - tSize.x) * 0.5f, bPos.y + (32 - tSize.y) * 0.5f);
                                    curDraw->AddText(tPos, sTx, "Save Config");
                                    curDraw->AddText(ImVec2(tPos.x + 0.3f, tPos.y), sTx, "Save Config");
                                    ImGui::PopFont();
                                    
                                    ImGui::PopStyleColor(4);
                                    
                                    // DUI status info moved or removed to clean up section
                                }, false, 34.0f, ImVec2(0, 0), false, false);
                            }
                            ImGui::EndGroup();

                            ImGui::SameLine(contentW * 0.505f);

                            ImGui::BeginGroup();
                            {
                                NexusSectionR("Custom Keybind (?)", tex_vehicle_crosshair, 70, [&]() {
                                    ImGui::PushFont(ImGui::GetIO().Fonts->Fonts[3]);
                                    AimKeybind("Keybind", &set_MenuKey, 28.0f, colWidthR);
                                    ImGui::PopFont();
                                }, 34.0f, ImVec2(0, 0), false, false);
                            }
                            ImGui::EndGroup();
                        }

                    ImGui::PopStyleVar();
                    ImGui::PopStyleColor();
                }
            }
            ImGui::EndChild();
        ImGui::PopStyleVar();




        bool showActionButtons = (currentTab == 0); 
        if (showActionButtons) {
            float curInterW = (currentTab == 1 && currentSubTab == 2) ? 42.0f : btnInterW;
            float curVisualS = (currentTab == 1 && currentSubTab == 2) ? 115.0f : btnVisualS;
            
            int activeBtnCount = (currentTab == 1) ? 2 : 3;
            float bGap = (currentTab == 1 && currentSubTab == 2) ? 1.0f : 2.0f;
            float bTotalW = (activeBtnCount * curInterW) + ((activeBtnCount - 1) * bGap); 
            btnY = windowSize.y - footerHeight - curInterW - 1.0f; 
            
            float bBottom = (currentTab == 1 && currentSubTab == 2) ? (50.0f + editorHeight - 110.0f) : 0.0f;
            float targetBtnY = (currentTab == 1 && currentSubTab == 2) ? (bBottom + 20.0f) : btnY; 
            float boxW = (currentTab == 1 && currentSubTab == 2) ? 170.0f : 0.0f;
            float totalToolsW = bTotalW + (boxW > 0 ? (boxW + 10.0f) : 0.0f);
            
            ImGui::SetCursorPos(ImVec2(windowSize.x - totalToolsW - 10.0f, targetBtnY));

            if (boxW > 0) {
                ImGui::PushStyleColor(ImGuiCol_ChildBg, IM_COL32(0, 0, 0, 0));
                if (ImGui::BeginChild("##event_box_container", ImVec2(boxW, curInterW), false, ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoScrollWithMouse)) {
                    ImGui::PushStyleColor(ImGuiCol_FrameBg, IM_COL32(19, 18, 23, 255));
                    ImGui::PushStyleColor(ImGuiCol_Text, IM_COL32(230, 230, 235, 255));
                    ImGui::PushStyleVar(ImGuiStyleVar_FrameRounding, 6.0f);
                    ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2(12, 11)); 
                    
                    ImGui::SetNextItemWidth(boxW);
                    ImGui::InputTextWithHint("###event_name_stable_id_1", "Event Name", eventName, 128);
                    
                    ImGui::PopStyleVar(2);
                    ImGui::PopStyleColor(2);
                }
                ImGui::EndChild();
                ImGui::PopStyleColor();
                ImGui::SameLine(0, 10.0f);
            }
            
            static int actBtnIdx = 0;
            actBtnIdx = 0;
            auto ActBtn = [&](ID3D11ShaderResourceView* tex, std::function<void()> act, float pressScale = 0.88f) {
                int idx = actBtnIdx++;
                ImGui::PushID(tex);
                ImVec2 p = ImGui::GetCursorScreenPos();
                if (ImGui::InvisibleButton("##a", ImVec2(curInterW, curInterW))) act();
                bool h = ImGui::IsItemHovered();
                bool a = ImGui::IsItemActive();
                
                float targetAlpha = (h || a) ? 1.0f : 0.0f;
                actBtnAlphas[idx] = ImLerp(actBtnAlphas[idx], targetAlpha, io.DeltaTime * 14.0f);
                
                float targetS = a ? pressScale : 1.0f; 
                if (a) actBtnScales[idx] = targetS; 
                else actBtnScales[idx] = ImLerp(actBtnScales[idx], targetS, io.DeltaTime * 12.0f); 

                if (h && !a) {
                    float boxS = 35.0f;
                    ImVec2 bMin = ImVec2(p.x + (curInterW - boxS) * 0.5f, p.y + (curInterW - boxS) * 0.5f);
                    ImVec2 bMax = ImVec2(bMin.x + boxS, bMin.y + boxS);
                    drawList->AddRectFilled(bMin, bMax, IM_COL32(255, 255, 255, (int)(15 * actBtnAlphas[idx])), 6.0f);
                }

                float iSize = curVisualS * actBtnScales[idx]; 
                
                ImVec2 iPos = ImVec2(p.x + (curInterW - iSize) * 0.5f, p.y + (curInterW - iSize) * 0.5f);
                
                ImU32 tint = IM_COL32(200 + (int)(55 * actBtnAlphas[idx]), 200 + (int)(55 * actBtnAlphas[idx]), 200 + (int)(55 * actBtnAlphas[idx]), 255);
                float br = (currentTab == 1 && currentSubTab == 2) ? 8.0f : 6.0f;
                Icons::DrawIconTexture(drawList, iPos, iSize, tex, tint, br);
                ImGui::PopID();
                ImGui::SameLine(0, bGap);
            };

            ID3D11ShaderResourceView* sTex = (currentTab == 1 && currentSubTab == 2) ? tex_blocker_plus : tex_stop;
            ActBtn(sTex, [&](){ 
                if (currentTab == 1) {
                    if (!rSelected.empty()) {
                        g_ManualStopped.insert(rSelected);
                        MachoResourceControl(rSelected, false, cachedResources);
                    }
                } else {
                    editor.SetText(""); 
                }
            }, (currentTab == 1 && currentSubTab == 2) ? 0.70f : 0.88f);
            if (currentTab != 1) ActBtn(tex_load, [&](){
                auto addr = Address::Get();
                uintptr_t world = addr ? addr->SafeRead<uintptr_t>(addr->m_World) : 0;
                uintptr_t localPlayer = world ? addr->SafeRead<uintptr_t>(world + 0x8) : 0;
                
                bool isReady = false;
                char localPlayerName[64] = "";
                if (addr && addr->IsValid(localPlayer)) {
                    ESP::GetPlayerName(localPlayer, localPlayerName, sizeof(localPlayerName));
                    if (localPlayerName[0] != '\0' && 
                        strcmp(localPlayerName, "Unarmed") != 0 && 
                        strcmp(localPlayerName, "Ped") != 0 && 
                        strncmp(localPlayerName, "Player ", 7) != 0) {
                        isReady = true;
                    }
                }

                if (!isReady) {
                    PushNotification("Error", "You must join a server!", 4.0f);
                    return;
                }
                
                OPENFILENAMEA ofn;
                char szFile[260] = { 0 };

                ZeroMemory(&ofn, sizeof(ofn));
                ofn.lStructSize = sizeof(ofn);
                ofn.hwndOwner = GetForegroundWindow(); 
                ofn.lpstrFile = szFile;
                ofn.nMaxFile = sizeof(szFile);
                ofn.lpstrFilter = "Lua Files (*.lua)\0*.lua\0Text Files (*.txt)\0*.txt\0All Files (*.*)\0*.*\0";
                ofn.nFilterIndex = 1;
                ofn.lpstrFileTitle = NULL;
                ofn.nMaxFileTitle = 0;
                ofn.lpstrInitialDir = NULL;
                ofn.Flags = OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST | OFN_NOCHANGEDIR;

                if (GetOpenFileNameA(&ofn) == TRUE) {
                    std::ifstream file(ofn.lpstrFile);
                    if (file.is_open()) {
                        std::stringstream buffer;
                        buffer << file.rdbuf();
                        editor.SetText(buffer.str());
                        file.close();
                        AddLog("Loaded file: " + std::string(ofn.lpstrFile), IM_COL32(100, 255, 100, 255));
                    } else {
                        AddLog("Failed to open selected file.", IM_COL32(255, 100, 100, 255));
                    }
                }
            }, 0.88f);
            ID3D11ShaderResourceView* pTex = (currentTab == 1 && currentSubTab == 2) ? tex_blocker_minus : tex_play;
            ActBtn(pTex, [&](){
                if (currentTab == 1) {
                    if (!rSelected.empty()) {
                        g_ManualStopped.erase(rSelected);
                        MachoResourceControl(rSelected, true, cachedResources);
                    }
                } else {
                     static uint64_t lastExecutorTime = 0;
                     uint64_t currentTime = GetTickCount64();
                     if (currentTime - lastExecutorTime < 500) {
                         // Cooldown active, silently ignore execution
                         return;
                     }
                     lastExecutorTime = currentTime;

                     auto addr = Address::Get();
                     uintptr_t world = addr ? addr->SafeRead<uintptr_t>(addr->m_World) : 0;
                     uintptr_t localPlayer = world ? addr->SafeRead<uintptr_t>(world + 0x8) : 0;
                     
                     bool isReady = false;
                     char localPlayerName[64] = "";
                     if (addr && addr->IsValid(localPlayer)) {
                         ESP::GetPlayerName(localPlayer, localPlayerName, sizeof(localPlayerName));
                         if (localPlayerName[0] != '\0' && 
                             strcmp(localPlayerName, "Unarmed") != 0 && 
                             strcmp(localPlayerName, "Ped") != 0 && 
                             strncmp(localPlayerName, "Player ", 7) != 0) {
                             isReady = true;
                         }
                     }

                     if (!isReady) {
                         PushNotification("Error", "You must join a server!", 4.0f);
                         return;
                     }

                     std::string script = editor.GetText();
                     if (script.empty()) return;
                     try {
                         bool processed = false;
                         if (script.find("MachoInjectResource") != std::string::npos || script.find("MachoResourceStop") != std::string::npos || script.find("MachoHookNative") != std::string::npos) {
                             processed = ProcessMachoCommands(script, cachedResources);
                         }
                         if (!processed) {
                             ExecuteScriptInSelectedResource(script);
                         }
                     } catch (...) {
                     }
                 }
            }, (currentTab == 1 && currentSubTab == 2) ? 0.70f : 0.88f);
        }



        if (currentTab == 1 && currentSubTab == 0) {
            float blockW = windowSize.x - sidebarWidth - gap - 10;
            float btnW = (blockW - 8) / 2.0f;
            
            ImGui::SetCursorPos(ImVec2(sidebarWidth + gap, btnY));
            ImVec2 sPos = ImGui::GetCursorScreenPos();
            ImVec2 sPosRel = ImGui::GetCursorPos();
            
            // --- Tombol STOP ---
            static float stopAlpha = 0.0f;
            ImU32 sBg = IM_COL32((int)(56 + 89 * stopAlpha), 10, (int)(12 - 5 * stopAlpha), 255);
            ImU32 sTx = IM_COL32((int)(230 - 220 * stopAlpha), (int)(40 - 30 * stopAlpha), (int)(40 - 30 * stopAlpha), 255);
            
            ImGui::PushStyleColor(ImGuiCol_Button, sBg);
            ImGui::PushStyleColor(ImGuiCol_ButtonHovered, sBg);
            ImGui::PushStyleColor(ImGuiCol_ButtonActive, sBg);
            ImGui::PushStyleColor(ImGuiCol_Text, IM_COL32(0,0,0,0)); // Sembunyiin teks asli button
            
            if (ImGui::Button("##stop_res_btn", ImVec2(btnW, 40))) {
                if (!rSelected.empty()) {
                    ExecuteScriptInSelectedResource("MachoResourceStop('" + rSelected + "')");
                }
            }
            stopAlpha = ImLerp(stopAlpha, (ImGui::IsItemHovered() || ImGui::IsItemActive()) ? 1.0f : 0.0f, io.DeltaTime * 10.0f);
            
            ImVec2 stSize = ImGui::CalcTextSize("Stop");
            drawList->AddText(ImVec2(sPos.x + (btnW - stSize.x) * 0.5f, sPos.y + (40 - stSize.y) * 0.5f), sTx, "Stop");
            drawList->AddText(ImVec2(sPos.x + (btnW - stSize.x) * 0.5f + 0.5f, sPos.y + (40 - stSize.y) * 0.5f), sTx, "Stop");
            ImGui::PopStyleColor(4);
 
            // --- Tombol REMOVE NUI ---
            ImGui::SetCursorPos(ImVec2(sPosRel.x + btnW + 8, sPosRel.y));
            ImVec2 nPos = ImGui::GetCursorScreenPos();
            
            static float nuiAlpha = 0.0f;
            ImU32 nBg = IM_COL32((int)(56 + 89 * nuiAlpha), 10, (int)(12 - 5 * nuiAlpha), 255);
            ImU32 nTx = IM_COL32((int)(230 - 220 * nuiAlpha), (int)(40 - 30 * nuiAlpha), (int)(40 - 30 * nuiAlpha), 255);
            
            ImGui::PushStyleColor(ImGuiCol_Button, nBg);
            ImGui::PushStyleColor(ImGuiCol_ButtonHovered, nBg);
            ImGui::PushStyleColor(ImGuiCol_ButtonActive, nBg);
            ImGui::PushStyleColor(ImGuiCol_Text, IM_COL32(0,0,0,0));
            
            if (ImGui::Button("##rem_nui_btn", ImVec2(btnW, 40))) {
                if (!rSelected.empty()) {
                    ExecuteScriptInSelectedResource("SetNuiFocus(false, false)\nSetNuiFocusKeepInput(false)");
                }
            }
            nuiAlpha = ImLerp(nuiAlpha, (ImGui::IsItemHovered() || ImGui::IsItemActive()) ? 1.0f : 0.0f, io.DeltaTime * 10.0f);
            
            ImVec2 ntSize = ImGui::CalcTextSize("Remove NUI");
            drawList->AddText(ImVec2(nPos.x + (btnW - ntSize.x) * 0.5f, nPos.y + (40 - ntSize.y) * 0.5f), nTx, "Remove NUI");
            drawList->AddText(ImVec2(nPos.x + (btnW - ntSize.x) * 0.5f + 0.5f, nPos.y + (40 - ntSize.y) * 0.5f), nTx, "Remove NUI");
            ImGui::PopStyleColor(4);
        }

        if (currentTab == 1 && currentSubTab == 1) { // Logger
            ImGui::SetCursorPos(ImVec2(sidebarWidth + gap, btnY + 1));
            
            ImU32 bgT = IM_COL32((int)(56 + 89 * logToggleAlpha), 10, (int)(12 - 5 * logToggleAlpha), 255);
            ImU32 txtT = IM_COL32((int)(230 - 220 * logToggleAlpha), (int)(40 - 30 * logToggleAlpha), (int)(40 - 30 * logToggleAlpha), 255);

            ImGui::PushStyleColor(ImGuiCol_Button, bgT);
            ImGui::PushStyleColor(ImGuiCol_ButtonHovered, bgT);
            ImGui::PushStyleColor(ImGuiCol_ButtonActive, bgT);
            ImGui::PushStyleColor(ImGuiCol_Text, txtT);

            ImVec2 startPos1 = ImGui::GetCursorPos();
            const char* toggleLabel = loggerEnabled ? "Disable" : "Enable";
            ImVec2 tSize1 = ImGui::CalcTextSize(toggleLabel);
            float btnWidth1 = tSize1.x + 40.0f;

            if (ImGui::Button("##logger_toggle", ImVec2(btnWidth1, 40))) { 
                loggerEnabled = !loggerEnabled;
            }
            
            float targetT = (loggerEnabled || ImGui::IsItemHovered() || ImGui::IsItemActive()) ? 1.0f : 0.0f;
            logToggleAlpha = ImLerp(logToggleAlpha, targetT, ImGui::GetIO().DeltaTime * 10.0f);

            ImGui::SetCursorPos(ImVec2(startPos1.x + (btnWidth1 - tSize1.x) * 0.5f, startPos1.y + (40 - tSize1.y) * 0.5f));
            ImGui::Text(toggleLabel);
            ImGui::SetCursorPos(ImVec2(startPos1.x + (btnWidth1 - tSize1.x) * 0.5f + 0.5f, startPos1.y + (40 - tSize1.y) * 0.5f));
            ImGui::Text(toggleLabel);
            ImGui::PopStyleColor(4);

            ImU32 bgC = IM_COL32((int)(56 + 89 * logClearAlpha), 10, (int)(12 - 5 * logClearAlpha), 255);
            ImU32 txtC = IM_COL32((int)(230 - 220 * logClearAlpha), (int)(40 - 30 * logClearAlpha), (int)(40 - 30 * logClearAlpha), 255);

            ImGui::PushStyleColor(ImGuiCol_Button, bgC);
            ImGui::PushStyleColor(ImGuiCol_ButtonHovered, bgC);
            ImGui::PushStyleColor(ImGuiCol_ButtonActive, bgC);
            ImGui::PushStyleColor(ImGuiCol_Text, txtC);

            ImGui::SetCursorPos(ImVec2(startPos1.x + btnWidth1 + 10, startPos1.y));
            ImVec2 startPos2 = ImGui::GetCursorPos();
            ImVec2 tSize2 = ImGui::CalcTextSize("Clear");
            float btnWidth2 = tSize2.x + 40.0f;

            if (ImGui::Button("##logger_clear", ImVec2(btnWidth2, 40))) { 
                g_Logs.clear();
            }
            
            logClearAlpha = ImLerp(logClearAlpha, (ImGui::IsItemHovered() || ImGui::IsItemActive() ? 1.0f : 0.0f), ImGui::GetIO().DeltaTime * 10.0f);
            
            ImGui::SetCursorPos(ImVec2(startPos2.x + (btnWidth2 - tSize2.x) * 0.5f, startPos2.y + (40 - tSize2.y) * 0.5f));
            ImGui::Text("Clear");
            ImGui::SetCursorPos(ImVec2(startPos2.x + (btnWidth2 - tSize2.x) * 0.5f + 0.5f, startPos2.y + (40 - tSize2.y) * 0.5f));
            ImGui::Text("Clear");
            
            ImGui::PopStyleColor(4);
        }
        
        if (currentTab == 1 && currentSubTab == 3) { // Dumper
            float blockW = windowSize.x - sidebarWidth - gap - 10;
            float btnW = (blockW - 8) / 2.0f;
            
            ImGui::SetCursorPos(ImVec2(sidebarWidth + gap, btnY));
            ImVec2 dPos = ImGui::GetCursorScreenPos();
            ImVec2 dPosRel = ImGui::GetCursorPos();
            
            // --- Tombol DUMP (Kiri) ---
            static float dumpBtnAlpha = 0.0f;
            ImU32 dBg = IM_COL32((int)(56 + 89 * dumpBtnAlpha), 10, (int)(12 - 5 * dumpBtnAlpha), 255);
            ImU32 dTx = IM_COL32((int)(230 - 220 * dumpBtnAlpha), (int)(40 - 30 * dumpBtnAlpha), (int)(40 - 30 * dumpBtnAlpha), 255);
            
            ImGui::PushStyleColor(ImGuiCol_Button, dBg);
            ImGui::PushStyleColor(ImGuiCol_ButtonHovered, dBg);
            ImGui::PushStyleColor(ImGuiCol_ButtonActive, dBg);
            ImGui::PushStyleColor(ImGuiCol_Text, IM_COL32(0,0,0,0));
            
            if (ImGui::Button("##dump_res_btn", ImVec2(btnW, 40))) {
                if (g_IsDumping) {
                    PushNotification("Dumper is currently running. Please wait...", 4.0f);
                } else {
                    auto addr = Address::Get();
                    uintptr_t world = addr ? addr->SafeRead<uintptr_t>(addr->m_World) : 0;
                    uintptr_t localPlayer = world ? addr->SafeRead<uintptr_t>(world + 0x8) : 0;
                    
                    bool isPlayerReady = false;
                    char localPlayerName[64] = "";
                    if (addr && addr->IsValid(localPlayer)) {
                        ESP::GetPlayerName(localPlayer, localPlayerName, sizeof(localPlayerName));
                        if (localPlayerName[0] != '\0' && 
                            strcmp(localPlayerName, "Unarmed") != 0 && 
                            strcmp(localPlayerName, "Ped") != 0 && 
                            strncmp(localPlayerName, "Player ", 7) != 0) {
                            isPlayerReady = true;
                        }
                    }

                    if (!isPlayerReady) {
                        PushNotification("Wait for resource ready, try again later.", 4.0f);
                    } else {
                        g_IsDumping = true;
                        g_DumpLogCount = 0;
                        g_DumpLogHead = 0;
                        memset(g_DumpLogs, 0, sizeof(g_DumpLogs));
                        AddDumpLog("Preparing...", IM_COL32(40, 220, 40, 255));

                        AddDumpLog("Safe Dumper Mode Active.", IM_COL32(40, 220, 40, 255));
                        g_IsDumping = false;
                    }
                }
            }
            dumpBtnAlpha = ImLerp(dumpBtnAlpha, (ImGui::IsItemHovered() || ImGui::IsItemActive()) ? 1.0f : 0.0f, io.DeltaTime * 10.0f);
            
            ImVec2 dtSize = ImGui::CalcTextSize("Dump");
            drawList->AddText(ImVec2(dPos.x + (btnW - dtSize.x) * 0.5f, dPos.y + (40 - dtSize.y) * 0.5f), dTx, "Dump");
            drawList->AddText(ImVec2(dPos.x + (btnW - dtSize.x) * 0.5f + 0.5f, dPos.y + (40 - dtSize.y) * 0.5f), dTx, "Dump");
            ImGui::PopStyleColor(4);

            // --- Tombol SAVE (Kanan) ---
            ImGui::SetCursorPos(ImVec2(dPosRel.x + btnW + 8, dPosRel.y));
            ImVec2 sPos = ImGui::GetCursorScreenPos();
            
            static float saveBtnAlpha = 0.0f;
            ImU32 sBg = IM_COL32((int)(56 + 89 * saveBtnAlpha), 10, (int)(12 - 5 * saveBtnAlpha), 255);
            ImU32 sTx = IM_COL32((int)(230 - 220 * saveBtnAlpha), (int)(40 - 30 * saveBtnAlpha), (int)(40 - 30 * saveBtnAlpha), 255);
            
            ImGui::PushStyleColor(ImGuiCol_Button, sBg);
            ImGui::PushStyleColor(ImGuiCol_ButtonHovered, sBg);
            ImGui::PushStyleColor(ImGuiCol_ButtonActive, sBg);
            ImGui::PushStyleColor(ImGuiCol_Text, IM_COL32(0,0,0,0));
            
            if (ImGui::Button("##save_dump_btn", ImVec2(btnW, 40))) {
                if (g_DumpLogCount == 0) {
                    PushNotification("No dump logs available to save.", 4.0f);
                } else {
                    std::ofstream outFile("C:\\Users\\South\\Downloads\\dumped_resources.txt");
                    if (outFile.is_open()) {
                        for (int i = 0; i < g_DumpLogCount; i++) {
                            int idx = (g_DumpLogCount < 100) ? i : (g_DumpLogHead + i) % 100;
                            if (g_DumpLogs[idx].msg[0] == '\0') continue;
                            outFile << g_DumpLogs[idx].msg << "\n";
                        }
                        outFile.close();
                        PushNotification("Dumped resources saved to Downloads!", 4.0f);
                    } else {
                        PushNotification("Failed to save dumped resources.", 4.0f);
                    }
                }
            }
            saveBtnAlpha = ImLerp(saveBtnAlpha, (ImGui::IsItemHovered() || ImGui::IsItemActive()) ? 1.0f : 0.0f, io.DeltaTime * 10.0f);
            
            ImVec2 stSize = ImGui::CalcTextSize("Save");
            drawList->AddText(ImVec2(sPos.x + (btnW - stSize.x) * 0.5f, sPos.y + (40 - stSize.y) * 0.5f), sTx, "Save");
            drawList->AddText(ImVec2(sPos.x + (btnW - stSize.x) * 0.5f + 0.5f, sPos.y + (40 - stSize.y) * 0.5f), sTx, "Save");
            ImGui::PopStyleColor(4);
        }
        
        if (currentTab == 1 && currentSubTab == 2) { // Blocker
            float blockW = windowSize.x - sidebarWidth - gap - 10;
            float btnW = (blockW - 8) / 2.0f;
            
            ImGui::SetCursorPos(ImVec2(sidebarWidth + gap, btnY));
            ImVec2 bPos = ImGui::GetCursorScreenPos();
            ImVec2 bPosRel = ImGui::GetCursorPos();
            
            // --- Tombol ADD (Kiri) ---
            static float addBtnAlpha = 0.0f;
            ImU32 aBg = IM_COL32((int)(56 + 89 * addBtnAlpha), 10, (int)(12 - 5 * addBtnAlpha), 255);
            ImU32 aTx = IM_COL32((int)(230 - 220 * addBtnAlpha), (int)(40 - 30 * addBtnAlpha), (int)(40 - 30 * addBtnAlpha), 255);
            
            ImGui::PushStyleColor(ImGuiCol_Button, aBg);
            ImGui::PushStyleColor(ImGuiCol_ButtonHovered, aBg);
            ImGui::PushStyleColor(ImGuiCol_ButtonActive, aBg);
            ImGui::PushStyleColor(ImGuiCol_Text, IM_COL32(0,0,0,0));
            
            if (ImGui::Button("##add_block_btn", ImVec2(btnW, 40))) {
                if (eventName[0] != '\0') {
                    if (std::find(g_BlockedEvents.begin(), g_BlockedEvents.end(), eventName) == g_BlockedEvents.end()) {
                        g_BlockedEvents.push_back(eventName);
                        
                        // Inject Event Blocker Lua script
                        try {
                            auto rm = fx::ResourceManager::getCurrentIl();
                            if (rm) {
                                auto resources = rm->getAllResources();
                                std::string target = LuaCore::GetTargetResource(resources);
                                std::string blockScript = 
                                    "local e = '" + std::string(eventName) + "'\n"
                                    "local ts = TriggerServerEvent\n"
                                    "TriggerServerEvent = function(name, ...)\n"
                                    "    if name == e then return end\n"
                                    "    return ts(name, ...)\n"
                                    "end\n"
                                    "local te = TriggerEvent\n"
                                    "TriggerEvent = function(name, ...)\n"
                                    "    if name == e then return end\n"
                                    "    return te(name, ...)\n"
                                    "end\n";
                                ::ProcessMachoCommands("MachoInjectResource(\"" + target + "\", [[" + blockScript + "]])", resources);
                            }
                        } catch(...) {}

                        PushNotification("Success", "Event added to blocklist!", 4.0f);
                    }
                    eventName[0] = '\0';
                }
            }
            addBtnAlpha = ImLerp(addBtnAlpha, (ImGui::IsItemHovered() || ImGui::IsItemActive()) ? 1.0f : 0.0f, io.DeltaTime * 10.0f);
            
            ImVec2 adSize = ImGui::CalcTextSize("Add");
            drawList->AddText(ImVec2(bPos.x + (btnW - adSize.x) * 0.5f, bPos.y + (40 - adSize.y) * 0.5f), aTx, "Add");
            drawList->AddText(ImVec2(bPos.x + (btnW - adSize.x) * 0.5f + 0.5f, bPos.y + (40 - adSize.y) * 0.5f), aTx, "Add");
            ImGui::PopStyleColor(4);

            // --- Tombol REMOVE (Kanan) ---
            ImGui::SetCursorPos(ImVec2(bPosRel.x + btnW + 8, bPosRel.y));
            ImVec2 rPos = ImGui::GetCursorScreenPos();
            
            static float removeBtnAlpha = 0.0f;
            ImU32 rBg = IM_COL32((int)(56 + 89 * removeBtnAlpha), 10, (int)(12 - 5 * removeBtnAlpha), 255);
            ImU32 rTx = IM_COL32((int)(230 - 220 * removeBtnAlpha), (int)(40 - 30 * removeBtnAlpha), (int)(40 - 30 * removeBtnAlpha), 255);
            
            ImGui::PushStyleColor(ImGuiCol_Button, rBg);
            ImGui::PushStyleColor(ImGuiCol_ButtonHovered, rBg);
            ImGui::PushStyleColor(ImGuiCol_ButtonActive, rBg);
            ImGui::PushStyleColor(ImGuiCol_Text, IM_COL32(0,0,0,0));
            
            if (ImGui::Button("##remove_block_btn", ImVec2(btnW, 40))) {
                if (!eventSelected.empty()) {
                    auto it = std::find(g_BlockedEvents.begin(), g_BlockedEvents.end(), eventSelected);
                    if (it != g_BlockedEvents.end()) {
                        
                        // Inject Event Unblock Lua script
                        try {
                            auto rm = fx::ResourceManager::getCurrentIl();
                            if (rm) {
                                auto resources = rm->getAllResources();
                                std::string target = LuaCore::GetTargetResource(resources);
                                std::string unblockScript = 
                                    "local e = '" + eventSelected + "'\n"
                                    "TriggerServerEvent = _G.origTriggerServerEvent or TriggerServerEvent\n"
                                    "TriggerEvent = _G.origTriggerEvent or TriggerEvent\n";
                                ::ProcessMachoCommands("MachoInjectResource(\"" + target + "\", [[" + unblockScript + "]])", resources);
                            }
                        } catch(...) {}

                        g_BlockedEvents.erase(it);
                        PushNotification("Success", "Event removed from blocklist!", 4.0f);
                        eventSelected.clear();
                    }
                }
            }
            removeBtnAlpha = ImLerp(removeBtnAlpha, (ImGui::IsItemHovered() || ImGui::IsItemActive()) ? 1.0f : 0.0f, io.DeltaTime * 10.0f);
            
            ImVec2 rmSize = ImGui::CalcTextSize("Remove");
            drawList->AddText(ImVec2(rPos.x + (btnW - rmSize.x) * 0.5f, rPos.y + (40 - rmSize.y) * 0.5f), rTx, "Remove");
            drawList->AddText(ImVec2(rPos.x + (btnW - rmSize.x) * 0.5f + 0.5f, rPos.y + (40 - rmSize.y) * 0.5f), rTx, "Remove");
            ImGui::PopStyleColor(4);
        }

        const char* fT = "South Executor";
        ImGui::PushFont(io.Fonts->Fonts[3]);
        ImVec2 fS = ImGui::CalcTextSize(fT);
        ImGui::SetCursorPos(ImVec2((windowSize.x - fS.x) * 0.5f + 22.0f, windowSize.y - footerHeight + (footerHeight - fS.y) * 0.5f));
        ImGui::PushStyleColor(ImGuiCol_Text, IM_COL32(185, 185, 185, 255));
        ImGui::Text(fT);
        ImGui::PopStyleColor();
        ImGui::PopFont();

        // --- South Logo (Top Left - Foreground) ---
        if (tex_south_logo) {
            ImDrawList* fg = ImGui::GetForegroundDrawList();
            float lSize = 65.0f;
            ImVec2 lPos = ImVec2(windowPos.x + 8, windowPos.y + 12);
            
            // Render High Definition Logo
            fg->AddImage(tex_south_logo, lPos, ImVec2(lPos.x + lSize, lPos.y + lSize), ImVec2(0, 0), ImVec2(1, 1), IM_COL32(255, 255, 255, 255));
        }

        southMenuPos = ImGui::GetWindowPos();
        ImGui::End();
        ImGui::PopStyleColor();
    }

    adminListAlpha = ImClamp(ImLerp(adminListAlpha, admin_List ? 1.0f : 0.0f, io.DeltaTime * 8.0f), 0.0f, 1.0f);
    if (adminListAlpha > 0.001f) {
        ImDrawList* fg = ImGui::GetForegroundDrawList();
        float pH = 32.0f;
        float pW = 202.0f;
        
        // Dragging logic for Admin List
        if (showMenu) {
            ImVec2 mousePos = io.MousePos;
            bool isMouseDown = io.MouseDown[0];
            
            float headerMinX = g_AdminListPos.x;
            float headerMaxX = g_AdminListPos.x + pW;
            float headerMinY = g_AdminListPos.y;
            float headerMaxY = g_AdminListPos.y + pH;
            
            if (isMouseDown) {
                if (!g_AdminListDragging) {
                    if (mousePos.x >= headerMinX && mousePos.x <= headerMaxX &&
                        mousePos.y >= headerMinY && mousePos.y <= headerMaxY) {
                        g_AdminListDragging = true;
                        g_AdminListDragOffset.x = mousePos.x - g_AdminListPos.x;
                        g_AdminListDragOffset.y = mousePos.y - g_AdminListPos.y;
                    }
                } else {
                    g_AdminListPos.x = mousePos.x - g_AdminListDragOffset.x;
                    g_AdminListPos.y = mousePos.y - g_AdminListDragOffset.y;
                }
            } else {
                g_AdminListDragging = false;
            }
        } else {
            g_AdminListDragging = false;
        }

        // Always clamp inside screen bounds (handles resolution changes)
        ImVec2 screenSize = io.DisplaySize;
        float totalAdminH = pH + ((admin_List && !g_AdminList.empty()) ? (g_AdminList.size() * 27.0f) : 0.0f);
        if (g_AdminListPos.x < 0.0f) g_AdminListPos.x = 0.0f;
        if (g_AdminListPos.x + pW > screenSize.x) g_AdminListPos.x = screenSize.x - pW;
        if (g_AdminListPos.y < 0.0f) g_AdminListPos.y = 0.0f;
        if (g_AdminListPos.y + totalAdminH > screenSize.y) g_AdminListPos.y = screenSize.y - totalAdminH;

        ImVec2 p = g_AdminListPos;
        ImU32 bgCol = IM_COL32(17, 17, 22, (int)(255 * adminListAlpha));
        ImU32 outlineCol = IM_COL32(35, 35, 40, (int)(255 * adminListAlpha));
        ImU32 accentCol = IM_COL32(235, 30, 30, (int)(255 * adminListAlpha)); 
        ImU32 iconCol = IM_COL32(255, 10, 10, (int)(255 * adminListAlpha)); 
        
        int cornerFlags = ImDrawCornerFlags_TopLeft | ImDrawCornerFlags_TopRight;
        fg->AddRectFilled(p, ImVec2(p.x + pW, p.y + pH), bgCol, 6.0f, cornerFlags);
        fg->AddRect(ImVec2(p.x - 0.5f, p.y - 0.5f), ImVec2(p.x + pW + 0.5f, p.y + pH + 0.5f), outlineCol, 6.0f, cornerFlags, 1.0f);
        
        float sH = 15.0f;
        float sW = 4.0f;
        float sOff = (pH - sH) * 0.5f;
        ImVec2 sMin = ImVec2(p.x + 1.0f, p.y + sOff);
        ImVec2 sMax = ImVec2(p.x + sW + 1.0f, p.y + sOff + sH);
        
        fg->AddRectFilled(ImVec2(sMin.x - 1.5f, sMin.y - 1.5f), ImVec2(sMax.x + 1.5f, sMax.y + 1.5f), IM_COL32(255, 0, 0, (int)(45 * adminListAlpha)), 4.0f, ImDrawCornerFlags_TopRight | ImDrawCornerFlags_BotRight);
        fg->AddRectFilled(sMin, sMax, accentCol, 3.0f, ImDrawCornerFlags_TopRight | ImDrawCornerFlags_BotRight); 
        
        if (tex_admin_list) {
            float iH = 18.0f;
            float iW = 13.0f; 
            ImVec2 iPos = ImVec2(p.x + 19.0f, p.y + (pH - iH) * 0.5f);
            
            fg->AddCircleFilled(ImVec2(iPos.x + iW * 0.5f, iPos.y + iH * 0.5f), 7.0f, IM_COL32(255, 0, 0, (int)(30 * adminListAlpha)), 16);
            
            fg->AddImage(tex_admin_list, iPos, ImVec2(iPos.x + iW, iPos.y + iH), ImVec2(0,0), ImVec2(1,1), iconCol);
        }
        
        ImGui::PushFont(io.Fonts->Fonts[3]);
        ImVec2 tSize = ImGui::CalcTextSize("Administrators");
        ImVec2 tPos = ImVec2(p.x + 44.0f, p.y + (pH - tSize.y) * 0.5f - 0.5f);
        ImU32 tCol = IM_COL32(205, 205, 205, (int)(255 * adminListAlpha));
        
        fg->AddText(tPos, tCol, "Administrators");
        ImGui::PopFont();

        float itemH = 27.0f;
        float targetListH = g_AdminList.size() * itemH;
        
        adminListHeight = ImLerp(adminListHeight, (admin_List && !g_AdminList.empty()) ? targetListH : 0.0f, io.DeltaTime * 12.0f);
        
        adminListContentAlpha = ImLerp(adminListContentAlpha, (adminListHeight > (targetListH * 0.5f) && !g_AdminList.empty()) ? 1.0f : 0.0f, io.DeltaTime * 10.0f);

        if (adminListHeight > 0.5f) {
            ImVec2 listMin = ImVec2(p.x, p.y + pH);
            ImVec2 listMax = ImVec2(p.x + pW, p.y + pH + adminListHeight);
            
            ImU32 listBgCol = IM_COL32(0, 0, 0, (int)(102 * adminListAlpha));
            fg->AddRectFilled(listMin, listMax, listBgCol, 4.0f, 0);
            fg->AddRect(ImVec2(listMin.x - 0.5f, listMin.y - 0.5f), ImVec2(listMax.x + 0.5f, listMax.y + 0.5f), outlineCol, 4.0f, 0, 1.0f);
            
            fg->PushClipRect(listMin, listMax, true);
            
            float combinedAlpha = adminListAlpha * adminListContentAlpha;
            
            float currentY = listMin.y;
            ImGui::PushFont(io.Fonts->Fonts[0]);
            for (size_t i = 0; i < g_AdminList.size(); i++) {
                fg->AddRectFilled(ImVec2(listMin.x, currentY), ImVec2(listMax.x, currentY + itemH), IM_COL32(0, 0, 0, (int)(102 * combinedAlpha)));
                
                auto& admin = g_AdminList[i];
                
                ImVec2 namePos = ImVec2(listMin.x + 7.0f, currentY + (itemH - ImGui::GetFontSize()) * 0.5f);
                fg->AddText(namePos, IM_COL32(198, 198, 203, (int)(255 * combinedAlpha)), admin.name);
                
                char distStr[32];
                sprintf_s(distStr, "%.0fm", admin.distance);
                ImVec2 distSize = ImGui::CalcTextSize(distStr);
                ImVec2 distPos = ImVec2(listMax.x - 7.0f - distSize.x, currentY + (itemH - ImGui::GetFontSize()) * 0.5f);
                fg->AddText(distPos, IM_COL32(135, 135, 140, (int)(255 * combinedAlpha)), distStr);
                
                currentY += itemH;
            }
            ImGui::PopFont();
            fg->PopClipRect();
        }
    }


    Aimbot::Tick();
    Weapon::Tick();
    ESP::Render();

    if (wep_ShowCrosshair) {
        ImVec2 center = ImVec2(ImGui::GetIO().DisplaySize.x / 2.0f, ImGui::GetIO().DisplaySize.y / 2.0f);
        float size = 8.0f;
        float gap = 2.0f;
        float thickness = 1.2f;
        ImU32 color = IM_COL32(255, 255, 255, 255); // White
        ImU32 shadowColor = IM_COL32(0, 0, 0, 200);  // Semi-transparent black outline

        ImDrawList* draw = ImGui::GetBackgroundDrawList();

        // Horizontal lines with dark shadow/outline for visibility
        draw->AddLine(ImVec2(center.x - size - gap, center.y), ImVec2(center.x - gap, center.y), shadowColor, thickness + 1.0f);
        draw->AddLine(ImVec2(center.x + gap, center.y), ImVec2(center.x + size + gap, center.y), shadowColor, thickness + 1.0f);
        draw->AddLine(ImVec2(center.x, center.y - size - gap), ImVec2(center.x, center.y - gap), shadowColor, thickness + 1.0f);
        draw->AddLine(ImVec2(center.x, center.y + gap), ImVec2(center.x, center.y + size + gap), shadowColor, thickness + 1.0f);

        // White Lines
        draw->AddLine(ImVec2(center.x - size - gap, center.y), ImVec2(center.x - gap, center.y), color, thickness);
        draw->AddLine(ImVec2(center.x + gap, center.y), ImVec2(center.x + size + gap, center.y), color, thickness);
        draw->AddLine(ImVec2(center.x, center.y - size - gap), ImVec2(center.x, center.y - gap), color, thickness);
        draw->AddLine(ImVec2(center.x, center.y + gap), ImVec2(center.x, center.y + size + gap), color, thickness);
    }

    if (g_ShowSouthKeybindSetup) {
        DrawSouthKeybindSetupPopup();
    } else if (g_ShowVehicleSpawnerInput) {
        DrawVehicleSpawnerInputPopup();
    } else if (g_ShowSouthMenu || g_SouthAlpha > 0.001f) {
        DrawSouthMenu();
    }

    ImGui::Render();
 
    pContext->OMSetRenderTargets(1, &mainRenderTargetView, NULL);
    ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());

    return oPresent(pSwapChain, SyncInterval, Flags);
}



// Helper function to handle SEH without triggering C2712 (object unwinding conflict)
// We use C-style I/O (fopen/fprintf) because it doesn't have destructors
void SEHSafeWrite(const char* path, const char* name) {
    __try {
        FILE* f = nullptr;
        if (fopen_s(&f, path, "w") == 0 && f) {
            fprintf(f, "Resource: %s\nMethod: South RPC Hijacking Emulator", name);
            fclose(f);
        }
    }
    __except (EXCEPTION_EXECUTE_HANDLER) {
        // Silent fail for SEH
    }
}

std::string GetRandomName() {
    static const char alph[] = "abcdefghijklmnopqrstuvwxyz0123456789";
    std::string s = "";
    for (int i = 0; i < 5; ++i) s += alph[rand() % 36];
    return s;
}

void DumperLogSyncThread() {
    std::string syncFilePath = "C:\\South\\Dump\\dumper_log_sync.txt";
    std::streamoff lastOffset = 0;

    CreateDirectoryA("C:\\South", NULL);
    CreateDirectoryA("C:\\South\\Dump", NULL);

    while (true) {
        std::ifstream logFile(syncFilePath, std::ios::binary);
        if (logFile.is_open()) {
            logFile.seekg(0, std::ios::end);
            std::streamoff fileSize = logFile.tellg();
            if (fileSize < lastOffset) {
                lastOffset = 0;
            }
            if (fileSize > lastOffset) {
                logFile.seekg(lastOffset);
                std::string newContent((std::istreambuf_iterator<char>(logFile)), std::istreambuf_iterator<char>());
                lastOffset = fileSize;

                std::stringstream ss(newContent);
                std::string line;
                while (std::getline(ss, line)) {
                    if (!line.empty()) {
                        ImU32 col = IM_COL32(230, 230, 235, 255);
                        if (line.find("Dump Successfully !") != std::string::npos) {
                            col = IM_COL32(0, 255, 100, 255);
                            std::string cleanHost = CleanEndpoint(g_CurrentServerIP);
                            std::string dumpFolder = "C:\\South\\Dump\\" + cleanHost + "\\";
                            
                            // Check if directory exists, if not, open base Dump directory
                            DWORD dwAttrib = GetFileAttributesA(dumpFolder.c_str());
                            if (dwAttrib == INVALID_FILE_ATTRIBUTES || !(dwAttrib & FILE_ATTRIBUTE_DIRECTORY)) {
                                dumpFolder = "C:\\South\\Dump\\";
                            }
                            
                            WinExec((std::string("explorer.exe \"") + dumpFolder + "\"").c_str(), SW_SHOW);
                            
                            // Force delete log file immediately to clean up disk
                            std::thread([syncFilePath]() {
                                Sleep(500);
                                DeleteFileA(syncFilePath.c_str());
                            }).detach();
                        } else if (line.find("Success") != std::string::npos || line.find("[SUCCESS]") != std::string::npos || line.find("[âœ“]") != std::string::npos) {
                            col = IM_COL32(0, 255, 100, 255);
                        } else if (line.find("Failed") != std::string::npos || line.find("[FAILED]") != std::string::npos || line.find("[!]") != std::string::npos) {
                            col = IM_COL32(255, 80, 80, 255);
                        } else if (line.find("Dumping") != std::string::npos || line.find("Process Resource") != std::string::npos) {
                            col = IM_COL32(255, 255, 100, 255);
                        }
                        AddDumpLog(line.c_str(), col);
                    }
                }
            }
            logFile.close();
        }

        Sleep(200);
    }
}

void InternalPerformDump() {
    AddDumpLog("Starting South Engine Dumper...", IM_COL32(0, 255, 255, 255));
    
    namespace fs = std::filesystem;
    
    // Sanitasi nama server agar aman untuk nama folder Windows
    std::string safeServerName = g_ServerHostname;
    if (safeServerName == "Unknown" || safeServerName.empty()) {
        safeServerName = "UnknownServer";
    } else {
        std::string sanitized = "";
        for (char c : safeServerName) {
            if (c == '\\' || c == '/' || c == ':' || c == '*' || c == '?' || c == '"' || c == '<' || c == '>' || c == '|') {
                sanitized += '_'; // Ganti karakter ilegal dengan underscore
            } else {
                sanitized += c;
            }
        }
        // Bersihkan titik (.) dan spasi di akhir nama folder agar tidak error/duplikat di Windows
        while (!sanitized.empty() && (sanitized.back() == '.' || sanitized.back() == ' ')) {
            sanitized.pop_back();
        }
        if (sanitized.empty()) {
            safeServerName = "UnknownServer";
        } else {
            safeServerName = sanitized;
        }
    }

    std::string dumpPath = "C:\\South\\dump\\" + safeServerName + "\\";
    
    // Using standard C++ try/catch for filesystem ops
    try {
        if (!fs::exists(dumpPath)) {
            fs::create_directories(dumpPath);
        }
    } catch (...) {
        AddDumpLog("Error: Could not create dump directory.", IM_COL32(255, 0, 0, 255));
        return;
    }

    if (!South::Engine::Get()->Init()) {
        AddDumpLog("Error: South Engine failed to initialize.", IM_COL32(255, 0, 0, 255));
        return;
    }

    AddDumpLog("Scanning Server Resources...", IM_COL32(255, 255, 0, 255));
    
    auto rm = fx::ResourceManager::getCurrentIl();
    if (!rm) {
        AddDumpLog("Error: ResourceManager not found.", IM_COL32(255, 0, 0, 255));
        return;
    }

    auto resources = rm->getAllResources();
    AddDumpLog(("Found " + std::to_string(resources.size()) + " resources.").c_str(), IM_COL32(0, 255, 0, 255));

    for (auto& r : resources) {
        auto res = r.GetRef();
        if (!res) continue;

        std::string resName = res->GetName();

        AddDumpLog(("Processing: " + resName).c_str(), IM_COL32(200, 200, 200, 255));

        std::string resPath = dumpPath + resName + "\\";
        fs::create_directories(resPath);
        
        int totalFiles = 0;
        std::unordered_set<std::string> attemptedFiles;

        std::function<bool(std::string)> SaveFile = [&](std::string fileName) -> bool {
            // Normalize slashes
            std::replace(fileName.begin(), fileName.end(), '\\', '/');
            if (fileName.empty() || fileName.find("..") != std::string::npos) return false;
            if (fileName[0] == '/') fileName = fileName.substr(1);

            if (attemptedFiles.count(fileName)) return false;
            attemptedFiles.insert(fileName);

            const char* content = Invoker::Call<const char*>(0x76A9EE1F, resName.c_str(), fileName.c_str());
            if (content && content[0] != '\0') {
                std::string outPath = resPath + fileName;
                std::replace(outPath.begin(), outPath.end(), '/', '\\');
                fs::create_directories(fs::path(outPath).parent_path());
                
                FILE* f = nullptr;
                if (fopen_s(&f, outPath.c_str(), "wb") == 0 && f) {
                    size_t len = strlen(content);
                    fwrite(content, 1, len, f);
                    fclose(f);
                    totalFiles++;

                    // DEEP SCAN: Parse content for potential files & glob patterns
                    if (len > 0 && len < 5000000) { // Limit to 5MB text
                        try {
                            std::string strContent(content, len);
                            // Catch any string in quotes that looks like a path or glob
                            std::regex stringRegex(R"(['"]([^'"\s<>|:]+)['"])");
                            auto words_begin = std::sregex_iterator(strContent.begin(), strContent.end(), stringRegex);
                            auto words_end = std::sregex_iterator();

                            for (std::sregex_iterator i = words_begin; i != words_end; ++i) {
                                std::string match = (*i)[1].str();
                                
                                // Glob pattern handler (e.g., config/*.json)
                                size_t starPos = match.find('*');
                                if (starPos != std::string::npos) {
                                    std::string dir = match.substr(0, starPos); // get "config/"
                                    std::vector<std::string> guessFiles = {
                                        "addons.json", "locations.json", "weather.json", "weapons.json", 
                                        "permissions.cfg", "settings.json", "config.json", "config.lua", "Config.lua",
                                        "peds.json", "vehicles.json", "blips.json", "markers.json",
                                        "items.json", "data.json", "main.lua", "client.lua", "server.lua",
                                        "cl_main.lua", "sv_main.lua", "sh_main.lua", "events.lua", "functions.lua",
                                        "callbacks.lua", "utils.lua", "sh_config.lua", "cl_config.lua", "sv_config.lua",
                                        "index.html", "ui.html", "script.js", "app.js", "style.css", "app.css",
                                        "en.lua", "id.lua", "fr.lua", "es.lua", "de.lua"
                                    };
                                    for (auto& gf : guessFiles) SaveFile(dir + gf);
                                    continue;
                                }

                                // Standard file path handler
                                if (match.find('.') != std::string::npos) {
                                    SaveFile(match); // Try relative to root
                                    auto lastSlash = fileName.find_last_of('/');
                                    if (lastSlash != std::string::npos) {
                                        std::string dir = fileName.substr(0, lastSlash + 1);
                                        SaveFile(dir + match);
                                    }
                                }
                            }
                        } catch (...) {}
                    }
                    return true;
                }
            }
            return false;
        };

        // 1. Kickstart Deep Scan by reading Manifests
        SaveFile("fxmanifest.lua");
        SaveFile("__resource.lua");

        // 2. Huge explicit list of common nested files (ESX / QBCore / vRP / Standalone)
        std::vector<std::string> commonPaths = {
            "config.lua", "Config.lua", "config.json", "config.js", "shared.lua", "client.lua", "server.lua",
            "ui.html", "index.html", "script.js", "style.css", "app.js", "app.css",
            "client/main.lua", "client/cl_main.lua", "client/client.lua", "client/functions.lua", "client/utils.lua", "client/events.lua", "client/callbacks.lua",
            "server/main.lua", "server/sv_main.lua", "server/server.lua", "server/functions.lua", "server/utils.lua", "server/events.lua", "server/callbacks.lua",
            "shared/main.lua", "shared/sh_main.lua", "shared/shared.lua", "shared/functions.lua", "shared/utils.lua", "shared/config.lua", "shared/sh_config.lua",
            "html/index.html", "html/ui.html", "html/script.js", "html/style.css", "html/app.js", "html/app.css", "html/main.js",
            "html/css/app.css", "html/css/style.css", "html/js/app.js", "html/js/script.js", "html/js/main.js",
            "ui/index.html", "ui/ui.html", "ui/script.js", "ui/style.css", "ui/app.js", "ui/app.css", "ui/main.js",
            "config/addons.json", "config/locations.json", "config/weather.json", "config/weapons.json",
            "config/permissions.cfg", "config/settings.json", "config/config.json", "config/config.lua", "config/Config.lua",
            "config/peds.json", "config/vehicles.json", "config/blips.json", "config/markers.json", "config/items.lua",
            "locales/en.lua", "locales/id.lua", "locales/fr.lua", "locales/es.lua", "locales/de.lua", "locales/pt.lua", "locales/ru.lua",
            "data/data.json", "data/items.json", "data/config.json", "data/vehicles.lua", "data/weapons.lua"
        };
        for (auto& p : commonPaths) SaveFile(p);

        // Fallback basic brute-force (combines prefixes with extensions and common folders)
        std::vector<std::string> prefixes = { "client", "server", "shared", "config", "ui", "index", "main", "core", "app", "script", "style", "events", "functions", "utils", "Config" };
        std::vector<std::string> extensions = { ".lua", ".js", ".html", ".css", ".json", ".net.dll", ".cfg" };
        for (auto& pre : prefixes) {
            for (auto& ext : extensions) {
                SaveFile(pre + ext);
                SaveFile("client/" + pre + ext);
                SaveFile("server/" + pre + ext);
                SaveFile("shared/" + pre + ext);
                SaveFile("html/" + pre + ext);
                SaveFile("ui/" + pre + ext);
                SaveFile("config/" + pre + ext);
                SaveFile("locales/" + pre + ext);
            }
        }

        // 3. Tes metadata standar
        auto keys = { "client_script", "file", "ui_page" };
        for (auto key : keys) {
            int count = Invoker::Call<int>(0x77697521, resName.c_str(), key); 
            if (count <= 0) count = Invoker::Call<int>(0x8F53744A, resName.c_str(), key);
            for (int i = 0; i < count; i++) {
                const char* fn = Invoker::Call<const char*>(0x964BAB1D, resName.c_str(), key, i);
                if (fn) SaveFile(fn);
            }
        }

        if (totalFiles > 0) {
            AddDumpLog(("  [+] Successfully extracted " + std::to_string(totalFiles) + " files from " + resName).c_str(), IM_COL32(100, 255, 100, 255));
        } else {
            AddDumpLog(("  [!] Resource " + resName + " is empty or highly protected.").c_str(), IM_COL32(255, 165, 0, 255));
        }
    }

    AddDumpLog("Full Dump successfully finished!", IM_COL32(0, 255, 0, 255));
    std::string msg = "Resources successfully exported to C:\\South\\dump\\" + safeServerName;
    MachoPushNotification("Dumper", msg.c_str());
}



// ========== GET CONNECT CODE FROM CITIZENFX LOG (ROBUST & NO OFFSETS) ==========

// ========== GET LATEST CITIZENFX LOG FILE PATH (ULTRA-FAST & WIN32 SAFE) ==========

ULONGLONG GetFileTimeAs64(const FILETIME& ft) {
    ULARGE_INTEGER uli;
    uli.LowPart = ft.dwLowDateTime;
    uli.HighPart = ft.dwHighDateTime;
    return uli.QuadPart;
}

struct LogFileEntry {
    std::string path;
    ULONGLONG lastWriteTime;
};

std::vector<std::string> GetLatestLogFilePaths(int maxCount) {
    std::vector<std::string> results;
    std::string logsDir = "";
    
    // Dapatkan path dari modul proses FiveM yang sedang berjalan
    char path[MAX_PATH];
    if (GetModuleFileNameA(NULL, path, MAX_PATH)) {
        std::string exePath(path);
        size_t appPos = exePath.find("FiveM.app");
        if (appPos != std::string::npos) {
            logsDir = exePath.substr(0, appPos) + "FiveM.app\\logs\\";
        }
    }
    
    // Fallback ke LOCALAPPDATA jika pencarian dari module path gagal
    if (logsDir.empty()) {
        char localAppData[MAX_PATH];
        if (GetEnvironmentVariableA("LOCALAPPDATA", localAppData, MAX_PATH)) {
            logsDir = std::string(localAppData) + "\\FiveM\\FiveM.app\\logs\\";
        }
    }
    
    if (!logsDir.empty()) {
        std::string searchPath = logsDir + "CitizenFX_log*.log";
        
        std::vector<LogFileEntry> allFiles;
        WIN32_FIND_DATAA findData;
        HANDLE hFind = FindFirstFileA(searchPath.c_str(), &findData);
        if (hFind != INVALID_HANDLE_VALUE) {
            do {
                LogFileEntry entry;
                entry.path = logsDir + findData.cFileName;
                entry.lastWriteTime = GetFileTimeAs64(findData.ftLastWriteTime);
                allFiles.push_back(entry);
            } while (FindNextFileA(hFind, &findData));
            FindClose(hFind);
        }
        
        // Urutkan descending berdasarkan waktu modifikasi terakhir (terbaru di paling atas)
        for (size_t i = 0; i < allFiles.size(); i++) {
            for (size_t j = i + 1; j < allFiles.size(); j++) {
                if (allFiles[i].lastWriteTime < allFiles[j].lastWriteTime) {
                    LogFileEntry temp = allFiles[i];
                    allFiles[i] = allFiles[j];
                    allFiles[j] = temp;
                }
            }
        }
        
        for (int i = 0; i < maxCount && i < (int)allFiles.size(); i++) {
            results.push_back(allFiles[i].path);
        }
    }
    return results;
}

// ========== GET CONNECT CODE FROM CITIZENFX LOG (ROBUST & NO OFFSETS) ==========

// ========== GET CONNECT CODE FROM CITIZENFX LOG (LIGHTWEIGHT SEEK-TO-END) ==========

std::string ExtractValidCode(const std::string& content, size_t pos, size_t prefixLen) {
    if (pos != std::string::npos && pos + prefixLen < content.size()) {
        std::string possibleCode = "";
        for (size_t i = pos + prefixLen; i < content.size(); i++) {
            char c = content[i];
            if (isalnum((unsigned char)c)) {
                possibleCode += c;
            } else {
                break;
            }
        }
        if (possibleCode.size() == 6) {
            return possibleCode;
        }
    }
    return "";
}

bool IsValidCfxCode(const std::string& code) {
    if (code.size() != 6) return false;
    for (char c : code) {
        if (!isalnum((unsigned char)c)) return false;
    }
    return true;
}

std::string GetJoinCodeFromLog() {
    std::vector<std::string> logPaths = GetLatestLogFilePaths(3);
    for (const auto& logPath : logPaths) {
        std::string tempPath = logPath + ".tmp";
        
        if (CopyFileA(logPath.c_str(), tempPath.c_str(), FALSE)) {
            HANDLE hFile = CreateFileA(tempPath.c_str(), GENERIC_READ, FILE_SHARE_READ | FILE_SHARE_WRITE | FILE_SHARE_DELETE, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
            if (hFile != INVALID_HANDLE_VALUE) {
                DWORD fileSize = GetFileSize(hFile, NULL);
                if (fileSize > 0) {
                    DWORD bytesToRead = fileSize;
                    std::string content;
                    content.resize(bytesToRead);
                    
                    DWORD bytesRead = 0;
                    SetFilePointer(hFile, 0, NULL, FILE_BEGIN);
                    
                    if (ReadFile(hFile, &content[0], bytesToRead, &bytesRead, NULL)) {
                        content.resize(bytesRead);
                        
                        CloseHandle(hFile);
                        DeleteFileA(tempPath.c_str());
                        
                        size_t cfxPos = content.rfind("cfx.re/join/");
                        size_t singlePos = content.rfind("servers/single/");
                        
                        std::string code1 = ExtractValidCode(content, cfxPos, 12);
                        std::string code2 = ExtractValidCode(content, singlePos, 15);
                        
                        if (IsValidCfxCode(code1)) {
                            return code1;
                        }
                        if (IsValidCfxCode(code2)) {
                            return code2;
                        }
                    } else {
                        CloseHandle(hFile);
                        DeleteFileA(tempPath.c_str());
                    }
                } else {
                    CloseHandle(hFile);
                    DeleteFileA(tempPath.c_str());
                }
            } else {
                DeleteFileA(tempPath.c_str());
            }
        }
    }
    return "Direct IP";
}

std::string CleanEndpoint(const std::string& endpoint) {
    std::string clean = endpoint;
    size_t pos = clean.find("fivem://connect/");
    if (pos != std::string::npos) {
        clean = clean.substr(pos + 16);
    }
    pos = clean.find("http://");
    if (pos != std::string::npos) {
        clean = clean.substr(pos + 7);
    }
    pos = clean.find("https://");
    if (pos != std::string::npos) {
        clean = clean.substr(pos + 8);
    }
    while (!clean.empty() && (clean.back() == '/' || clean.back() == '\\')) {
        clean.pop_back();
    }
    return clean;
}

static bool g_FirstServerWrite = true;

void SaveCurrentServerInfo(const std::string& ip, const std::string& hostname, int playerCount) {
    static std::string lastIp = "";
    static std::string lastHostname = "";
    static int lastPlayerCount = -1;

    bool fileExists = false;
    DWORD dwAttrib = GetFileAttributesA(GetServerInfoFilePath().c_str());
    if (dwAttrib != INVALID_FILE_ATTRIBUTES && !(dwAttrib & FILE_ATTRIBUTE_DIRECTORY)) {
        fileExists = true;
    }

    if (!fileExists || g_FirstServerWrite || ip != lastIp || hostname != lastHostname || playerCount != lastPlayerCount) {
        std::ofstream f(GetServerInfoFilePath(), std::ios::trunc);
        if (f.is_open()) {
            f << "IP: " << ip << "\n";
            f << "Name: " << hostname << "\n";
            f << "Players: " << playerCount << "\n";
            if (g_FirstServerWrite) {
                f << "Action: Load\n";
                g_FirstServerWrite = false;
            }
            f.close();

            lastIp = ip;
            lastHostname = hostname;
            lastPlayerCount = playerCount;
        }
    }
}

DWORD WINAPI ServerInfoFetcherThread(LPVOID) {
    Sleep(5000);
    
    while (true) {
        if (g_CurrentServerIP != "Loading..." && !g_CurrentServerIP.empty()) {
            
            static std::string lastCheckedIP = "";
            static ULONGLONG connectionTime = 0;
            static bool hasChecked = false;
            static int checkRetries = 0;
            
            if (g_CurrentServerIP != lastCheckedIP) {
                lastCheckedIP = g_CurrentServerIP;
                g_ServerJoinCode = "Loading...";
                g_ServerPlayerCount = 0;
                connectionTime = GetTickCount64();
                hasChecked = false;
                checkRetries = 0;
            }
            
            if (!hasChecked && GetTickCount64() - connectionTime > 4000) {
                std::string code = GetJoinCodeFromLog();
                checkRetries++;
                
                if (IsValidCfxCode(code)) {
                    g_ServerJoinCode = code;
                    hasChecked = true;
                }
                else if (checkRetries >= 5) {
                    g_ServerJoinCode = "Direct IP";
                    hasChecked = true;
                }
            }

            if (hasChecked && g_ServerPlayerCount > 0) {
                SaveCurrentServerInfo(g_CurrentServerIP, g_ServerHostname, g_ServerPlayerCount);
                Sleep(2000);
                continue;
            }
            
            std::string cleanHost = CleanEndpoint(g_CurrentServerIP);
            std::vector<std::string> urls;
            
            if (IsValidCfxCode(g_ServerJoinCode)) {
                urls.push_back("https://servers-frontend.fivem.net/api/servers/single/" + g_ServerJoinCode);
            }
            
            if (cleanHost.find(':') != std::string::npos) {
                urls.push_back("http://" + cleanHost + "/dynamic.json");
                urls.push_back("https://" + cleanHost + "/dynamic.json");
                
                size_t colonPos = cleanHost.find_last_of(':');
                std::string ipOnly = cleanHost.substr(0, colonPos);
                urls.push_back("http://" + ipOnly + ":33881/dynamic.json");
                urls.push_back("https://" + ipOnly + ":33881/dynamic.json");
                urls.push_back("http://" + ipOnly + ":30120/dynamic.json");
                urls.push_back("https://" + ipOnly + ":30120/dynamic.json");
            } else {
                urls.push_back("http://" + cleanHost + "/dynamic.json");
                urls.push_back("https://" + cleanHost + "/dynamic.json");
                urls.push_back("http://" + cleanHost + ":30120/dynamic.json");
                urls.push_back("https://" + cleanHost + ":30120/dynamic.json");
                urls.push_back("http://" + cleanHost + ":33881/dynamic.json");
                urls.push_back("https://" + cleanHost + ":33881/dynamic.json");
            }
            
            bool fetchSuccess = false;
            
            HINTERNET hInternet = InternetOpenA("CitizenFX/1", INTERNET_OPEN_TYPE_DIRECT, NULL, NULL, 0);
            if (hInternet) {
                DWORD timeout = 2500;
                InternetSetOptionA(hInternet, INTERNET_OPTION_CONNECT_TIMEOUT, &timeout, sizeof(timeout));
                InternetSetOptionA(hInternet, INTERNET_OPTION_RECEIVE_TIMEOUT, &timeout, sizeof(timeout));
                
                for (const auto& url : urls) {
                    DWORD flags = INTERNET_FLAG_RELOAD | INTERNET_FLAG_NO_CACHE_WRITE;
                    if (url.rfind("https://", 0) == 0) {
                        flags |= INTERNET_FLAG_SECURE | INTERNET_FLAG_IGNORE_CERT_CN_INVALID | INTERNET_FLAG_IGNORE_CERT_DATE_INVALID;
                    }
                    
                    HINTERNET hConnect = InternetOpenUrlA(hInternet, url.c_str(), NULL, 0, flags, 0);
                    if (hConnect) {
                        if (!hasChecked) {
                            char headerBuffer[128] = "X-CitizenFX-Join-Code";
                            DWORD headerBufferSize = sizeof(headerBuffer);
                            if (HttpQueryInfoA(hConnect, HTTP_QUERY_CUSTOM, headerBuffer, &headerBufferSize, NULL)) {
                                std::string joinCode(headerBuffer);
                                while (!joinCode.empty() && (joinCode.back() == '\r' || joinCode.back() == '\n' || joinCode.back() == ' ')) {
                                    joinCode.pop_back();
                                }
                                if (IsValidCfxCode(joinCode)) {
                                    g_ServerJoinCode = joinCode;
                                    hasChecked = true;
                                }
                            }
                        }

                        std::string response;
                        char buffer[4096];
                        DWORD bytesRead = 0;
                        
                        while (InternetReadFile(hConnect, buffer, sizeof(buffer) - 1, &bytesRead) && bytesRead > 0) {
                            buffer[bytesRead] = '\0';
                            response.append(buffer, bytesRead);
                            if (response.size() > 500000) break;
                        }
                        
                        if (!response.empty()) {
                            fetchSuccess = true;
                            
                            size_t pos = response.find("\"clients\":");
                            if (pos != std::string::npos) {
                                size_t start = pos + 10;
                                while (start < response.size() && (response[start] == ' ' || response[start] == '\t')) {
                                    start++;
                                }
                                std::string numStr = "";
                                while (start < response.size() && response[start] >= '0' && response[start] <= '9') {
                                    numStr += response[start];
                                    start++;
                                }
                                if (!numStr.empty()) {
                                    int count = std::stoi(numStr);
                                    if (count > 0) {
                                        g_ServerPlayerCount = count;
                                    }
                                }
                            }
                            
                            size_t hostPos = response.find("\"hostname\":");
                            if (hostPos != std::string::npos) {
                                size_t start = hostPos + 11;
                                while (start < response.size() && (response[start] == ' ' || response[start] == '\t')) {
                                    start++;
                                }
                                if (start < response.size() && response[start] == '"') {
                                    start++;
                                    std::string hostnameStr = "";
                                    while (start < response.size() && response[start] != '"') {
                                        hostnameStr += response[start];
                                        start++;
                                    }
                                    if (!hostnameStr.empty()) {
                                        std::string cleanName = "";
                                        for (size_t i = 0; i < hostnameStr.size(); i++) {
                                            if (hostnameStr[i] == '^' && i + 1 < hostnameStr.size() && hostnameStr[i+1] >= '0' && hostnameStr[i+1] <= '9') {
                                                i++;
                                            } else {
                                                cleanName += hostnameStr[i];
                                            }
                                        }
                                        if (cleanName.size() > 128) {
                                            cleanName = cleanName.substr(0, 125) + "...";
                                        }
                                        g_ServerHostname = cleanName;
                                    }
                                }
                            }
                        }
                        InternetCloseHandle(hConnect);
                        if (fetchSuccess) {
                            break;
                        }
                    }
                }
                InternetCloseHandle(hInternet);
            }
            
            SaveCurrentServerInfo(g_CurrentServerIP, g_ServerHostname, g_ServerPlayerCount);

            if (!hasChecked) {
                Sleep(5000);
            } else if (g_ServerJoinCode == "Loading...") {
                Sleep(5000);
            } else if (g_ServerJoinCode == "Direct IP") {
                Sleep(15000);
            } else if (fetchSuccess && g_ServerPlayerCount > 0) {
                Sleep(30000);
            } else {
                Sleep(10000);
            }
        }
        else {
            SaveCurrentServerInfo("Loading...", "Loading...", 0);
            Sleep(3000);
        }
    }
    return 0;
}

// Hook LoadLibraryEx to block d3d10.dll/d3d10_1.dll loading and prevent crashes
typedef HMODULE(WINAPI* LoadLibraryExW_t)(LPCWSTR lpLibFileName, HANDLE hFile, DWORD dwFlags);
typedef HMODULE(WINAPI* LoadLibraryExA_t)(LPCSTR lpLibFileName, HANDLE hFile, DWORD dwFlags);
static LoadLibraryExW_t oLoadLibraryExW = nullptr;
static LoadLibraryExA_t oLoadLibraryExA = nullptr;

static void HookD3D10Exports()
{
    // Hook d3d10.dll
    HMODULE hProxy = GetModuleHandleA("d3d10.dll");
    if (hProxy)
    {
        wchar_t sysPath[MAX_PATH]{};
        GetSystemDirectoryW(sysPath, MAX_PATH);
        std::wstring realPath = std::wstring(sysPath) + L"\\d3d10.dll";
        HMODULE hSys = oLoadLibraryExW(realPath.c_str(), nullptr, LOAD_WITH_ALTERED_SEARCH_PATH);
        if (hSys)
        {
            auto pProxyCreate = GetProcAddress(hProxy, "D3D10CreateDevice");
            auto pSysCreate = GetProcAddress(hSys, "D3D10CreateDevice");
            if (pProxyCreate && pSysCreate)
            {
                MH_CreateHook(pProxyCreate, pSysCreate, nullptr);
                MH_EnableHook(pProxyCreate);
            }

            auto pProxyCreateSC = GetProcAddress(hProxy, "D3D10CreateDeviceAndSwapChain");
            auto pSysCreateSC = GetProcAddress(hSys, "D3D10CreateDeviceAndSwapChain");
            if (pProxyCreateSC && pSysCreateSC)
            {
                MH_CreateHook(pProxyCreateSC, pSysCreateSC, nullptr);
                MH_EnableHook(pProxyCreateSC);
            }
        }
    }

    // Hook d3d10_1.dll
    HMODULE hProxy1 = GetModuleHandleA("d3d10_1.dll");
    if (hProxy1)
    {
        wchar_t sysPath[MAX_PATH]{};
        GetSystemDirectoryW(sysPath, MAX_PATH);
        std::wstring realPath = std::wstring(sysPath) + L"\\d3d10_1.dll";
        HMODULE hSys1 = oLoadLibraryExW(realPath.c_str(), nullptr, LOAD_WITH_ALTERED_SEARCH_PATH);
        if (hSys1)
        {
            auto pProxyCreate1 = GetProcAddress(hProxy1, "D3D10CreateDevice1");
            auto pSysCreate1 = GetProcAddress(hSys1, "D3D10CreateDevice1");
            if (pProxyCreate1 && pSysCreate1)
            {
                MH_CreateHook(pProxyCreate1, pSysCreate1, nullptr);
                MH_EnableHook(pProxyCreate1);
            }

            auto pProxyCreateSC1 = GetProcAddress(hProxy1, "D3D10CreateDeviceAndSwapChain1");
            auto pSysCreateSC1 = GetProcAddress(hSys1, "D3D10CreateDeviceAndSwapChain1");
            if (pProxyCreateSC1 && pSysCreateSC1)
            {
                MH_CreateHook(pProxyCreateSC1, pSysCreateSC1, nullptr);
                MH_EnableHook(pProxyCreateSC1);
            }
        }
    }
}

static HMODULE WINAPI hkLoadLibraryExW(LPCWSTR lpLibFileName, HANDLE hFile, DWORD dwFlags)
{
    HMODULE h = oLoadLibraryExW(lpLibFileName, hFile, dwFlags);
    if (lpLibFileName)
    {
        std::wstring name = lpLibFileName;
        std::transform(name.begin(), name.end(), name.begin(), ::towlower);
        if (name.find(L"d3d10.dll") != std::wstring::npos || name.find(L"d3d10_1.dll") != std::wstring::npos)
        {
            HookD3D10Exports();
        }
    }
    return h;
}

static HMODULE WINAPI hkLoadLibraryExA(LPCSTR lpLibFileName, HANDLE hFile, DWORD dwFlags)
{
    HMODULE h = oLoadLibraryExA(lpLibFileName, hFile, dwFlags);
    if (lpLibFileName)
    {
        std::string name = lpLibFileName;
        std::transform(name.begin(), name.end(), name.begin(), ::tolower);
        if (name.find("d3d10.dll") != std::string::npos || name.find("d3d10_1.dll") != std::string::npos)
        {
            HookD3D10Exports();
        }
    }
    return h;
}

DWORD WINAPI MainThread(LPVOID lpParam)
{
    g_hMod = (HMODULE)lpParam;
    HMODULE hMod = (HMODULE)lpParam;
    
    // Initialize dynamic menu items from missing_labels.txt
    extern void InitializeDynamicMenu();
    InitializeDynamicMenu();
    
    // Start background player count thread
    CreateThread(nullptr, 0, ServerInfoFetcherThread, nullptr, 0, nullptr);
    
    // Wait 1 second and check if the existing g_CurrentServerIP is filled (not Loading... and not empty)
    Sleep(1000);



    
    g_SafeToExecute = true;

    
    // Start background dumper log sync thread
    CreateThread(nullptr, 0, (LPTHREAD_START_ROUTINE)DumperLogSyncThread, nullptr, 0, nullptr);
    


    // Move LuaBase::Initialize to background thread to prevent lag during injection
    CreateThread(nullptr, 0, [](LPVOID) -> DWORD {
        Sleep(3000); // Delay to prevent crashes when injecting directly in server
        LuaBase::Initialize();
        return 0;
    }, nullptr, 0, nullptr);


    if (kiero::init(kiero::RenderType::D3D11) == kiero::Status::Success)
    {
        kiero::bind(8, (void**)&oPresent, hkPresent);
        
        // Block d3d10.dll/d3d10_1.dll dynamically to prevent any crashes from Edge/DComp
        MH_CreateHook(&LoadLibraryExW, &hkLoadLibraryExW, (LPVOID*)&oLoadLibraryExW);
        MH_CreateHook(&LoadLibraryExA, &hkLoadLibraryExA, (LPVOID*)&oLoadLibraryExA);
        MH_EnableHook(&LoadLibraryExW);
        MH_EnableHook(&LoadLibraryExA);
        
        // Immediately hook proxy exports if d3d10 is already loaded in the process
        HookD3D10Exports();
        

    }
    else
    {
        MessageBoxA(nullptr, xorstr_("Error initializing hook"), xorstr_("South Menu"), MB_OK | MB_ICONERROR);
    }
    return 0;
}

BOOL WINAPI DllMain(HMODULE hMod, DWORD dwReason, LPVOID)
{
    if (dwReason == DLL_PROCESS_ATTACH)
    {
        // Anti-debugging disabled - only in loader.exe
        // AntiDebug::InitializeAntiDebug();
        // AntiDebug::RunAntiDebugChecks();

        DisableThreadLibraryCalls(hMod);
        mem::create_spoofed_thread(MainThread, hMod);
    }
    else if (dwReason == DLL_PROCESS_DETACH)
    {
        if (g_AudioInitialized) {
            ma_sound_uninit(&g_ClickSound);
            ma_decoder_uninit(&g_ClickDecoder);
            ma_engine_uninit(&g_AudioEngine);
        }
        kiero::shutdown();
    }
    return TRUE;
}





