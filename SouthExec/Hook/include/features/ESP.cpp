// Leaked By Cry Service & Siraj
#include "ESP.h"
#include <iostream>
#include <chrono>
#include <map>
#include "NameManager.h"

SRWLOCK g_PlayerListLock = SRWLOCK_INIT;
SRWLOCK g_VehicleListLock = SRWLOCK_INIT;

std::vector<AdminData> g_AdminList;
std::vector<PlayerListData> g_PlayerList;
std::vector<VehicleListData> g_VehicleList;

struct CachedName {
    char name[64];
    std::chrono::steady_clock::time_point lastUpdate;
};

static CachedName s_NameCacheArray[2048]; 
static bool s_NameCacheValid[2048] = { false };

struct CachedEntity {
    uintptr_t ped;
    bool isPlayer;
    bool isAdmin;
    int serverId;
    Vector3 pos;
    std::chrono::steady_clock::time_point lastUpdate;
};
static std::vector<CachedEntity> s_EntityCache;
static std::chrono::steady_clock::time_point s_LastCacheUpdate;

struct CachedVehicle {
    uintptr_t vehicle;
    Vector3 pos;
    char name[32];
    int lockStatus;
};
static std::vector<CachedVehicle> s_VehicleCache;
static std::chrono::steady_clock::time_point s_LastVehCacheUpdate;

void AddAdminToList(uintptr_t ped, float dist) {
    AdminData data;
    data.distance = dist;
    ESP::GetPlayerName(ped, data.name, sizeof(data.name));
    g_AdminList.push_back(data);
}

void ESP::SyncPlayerList(Vector3 localPos) {
    SRWLockGuard lock(&g_PlayerListLock);
    g_PlayerList.clear();
    g_PlayerList.reserve(128); 
    auto addr = Address::Get();
    uintptr_t localPlayer = addr ? addr->SafeRead<uintptr_t>(addr->SafeRead<uintptr_t>(addr->m_World) + 0x8) : 0;

    for (const auto& entry : s_EntityCache) {
        if (entry.isPlayer) {
            float dist = localPos.Distance(entry.pos);
            float maxDist = esp_InfDistance ? 99999999.0f : 3000.0f;
            if (dist < maxDist) {
                PlayerListData data;
                ESP::GetPlayerName(entry.ped, data.name, sizeof(data.name));

                if (entry.ped == localPlayer) {
                    std::string selfName = std::string(data.name) + " [YOU]";
                    strncpy_s(data.name, sizeof(data.name), selfName.c_str(), _TRUNCATE);
                }

                data.distance = dist;
                data.isAdmin = entry.isAdmin;
                data.serverId = entry.serverId;
                data.ped = entry.ped;
                g_PlayerList.push_back(data);
            }
        }
    }
}

// ========== FIX: IsValidPlayerName (TANPA __try) ==========
static bool IsValidPlayerName(const char* s) {
    if (!s) return false;
    if (s[0] == '\0') return false;
    
    int len = 0;
    bool hasVisible = false;
    while (s[len] != '\0') {
        unsigned char c = (unsigned char)s[len];
        if (c < 0x20 || c == 0x7F) return false;
        if (c != 0x20) hasVisible = true;
        if (++len > 128) return false;
    }
    return len >= 2 && hasVisible;
}

// ========== FIX: SafeGetPlayerNameHook (try-catch) ==========
static const char* SafeGetPlayerNameHook(void* netData) {
    auto addr = Address::Get();
    if (!addr->m_GetPlayerName || !netData) return nullptr;

    try {
        return addr->m_GetPlayerName(netData);
    } catch (...) {
        return nullptr;
    }
}

static void SyncVehicleListInternal(const Vector3& localPos) {
    SRWLockGuard lock(&g_VehicleListLock);
    g_VehicleList.clear();
    g_VehicleList.reserve(s_VehicleCache.size());
    for (const auto& cv : s_VehicleCache) {
        VehicleListData vld;
        vld.vehicle = cv.vehicle;
        strncpy_s(vld.name, sizeof(vld.name), cv.name[0] ? cv.name : "Unknown", _TRUNCATE);
        vld.distance = localPos.Distance(cv.pos);
        vld.pos = cv.pos;
        g_VehicleList.push_back(vld);
    }
}

void ESP::Render() {
    static auto lastAdminUpdate = std::chrono::steady_clock::now();
    bool shouldUpdateAdminList = false;

    if (admin_List) {
        auto now = std::chrono::steady_clock::now();
        if (std::chrono::duration_cast<std::chrono::milliseconds>(now - lastAdminUpdate).count() >= 200) {
            shouldUpdateAdminList = true;
            g_AdminList.clear();
            g_AdminList.reserve(32);
            lastAdminUpdate = now;
        }
    }
    else {
        if (!g_AdminList.empty()) g_AdminList.clear();
    }

    auto addr = Address::Get();
    auto core = Core::Get();
    if (!addr || !core) return;

    uintptr_t world = addr->SafeRead<uintptr_t>(addr->m_World);
    if (!addr->IsValid(world)) return;

    uintptr_t localPlayer = addr->SafeRead<uintptr_t>(world + 0x8);
    if (!addr->IsValid(localPlayer)) return;

    uintptr_t replay = addr->SafeRead<uintptr_t>(addr->m_ReplayInterface);
    if (!addr->IsValid(replay)) return;

    Matrix4x4 localMatrix = addr->SafeRead<Matrix4x4>(localPlayer + 0x60);
    Vector3 localPos = Vector3(localMatrix._41, localMatrix._42, localMatrix._43);

    if (localPos.x != localPos.x || fabsf(localPos.x) > 100000.0f || fabsf(localPos.y) > 100000.0f || fabsf(localPos.z) > 100000.0f) {
        return; 
    }

    float unlimitedDist = esp_InfDistance ? 99999999.0f : esp_Distance;

    if (esp_Enable || admin_List || admin_Lines || admin_Skeleton) 
    {
        uintptr_t pedInterface = addr->SafeRead<uintptr_t>(replay + addr->m_PedInterface);
        if (addr->IsValid(pedInterface)) {
            uintptr_t pedList = addr->SafeRead<uintptr_t>(pedInterface + addr->m_PedList);
            int pedMaximum = addr->SafeRead<int>(pedInterface + addr->m_PedMaximum);

            auto now = std::chrono::steady_clock::now();

            int cacheInterval = esp_InfDistance ? 50 : 150;
            if (std::chrono::duration_cast<std::chrono::milliseconds>(now - s_LastCacheUpdate).count() >= cacheInterval) {
                s_EntityCache.clear();
                s_EntityCache.reserve(512);
                if (addr->IsValid(pedList) && pedMaximum > 0 && pedMaximum < 1024) {
                    for (int i = 0; i < pedMaximum; i++) {
                        uintptr_t ped = addr->SafeRead<uintptr_t>(pedList + (i * 0x10));
                        if (!addr->IsValid(ped)) continue;

                        bool isPlayer = IsPlayer(ped);
                        bool isAdminCandidate = false;
                        int serverId = -1;
                        Vector3 pedPos = Vector3(0, 0, 0);

                        if (isPlayer) {
                            BYTE nativeVis = addr->SafeRead<BYTE>(ped + 0x2C, 0xFF);
                            float alpha = addr->SafeRead<float>(ped + 0xD0, 255.0f);
                            isAdminCandidate = ((nativeVis & 0x01) == 0) && (alpha < 1.0f);
                            serverId = GetServerIdInternal(ped);
                        }

                        Matrix4x4 matrix = addr->SafeRead<Matrix4x4>(ped + 0x60);
                        pedPos = Vector3(matrix._41, matrix._42, matrix._43);
                        if (pedPos.x == 0) {
                            pedPos = addr->SafeRead<Vector3>(ped + addr->m_VisualPos);
                        }

                        if (pedPos.x == 0 && isPlayer) {
                            pedPos = core->GetBone(ped, 0x2e28); 
                        }

                        s_EntityCache.push_back({ ped, isPlayer, isAdminCandidate, serverId, pedPos, now });
                    }
                }
                s_LastCacheUpdate = now;
                SyncPlayerList(localPos);
            }

            // ========== GANTI __try DENGAN try-catch ==========
            static constexpr int kMaxEspDrawCount = 120;
            int drawnEntities = 0;
            for (const auto& entry : s_EntityCache) {
                if (drawnEntities >= kMaxEspDrawCount) break;
                try {
                    uintptr_t ped = entry.ped;
                    if (!addr->IsValid(ped)) continue;
                    if (IsPlayer(ped) != entry.isPlayer) continue;
                    if (ped == localPlayer && !esp_Self) continue;

                    if (!esp_Enable && !admin_List && !admin_Lines && !admin_Skeleton) continue;
                    if (!entry.isPlayer && !esp_TargetNpcs) continue;

                    if (entry.isAdmin && admin_List && shouldUpdateAdminList) {
                        float dist = localPos.Distance(entry.pos);
                        if (dist < 1000.0f) AddAdminToList(ped, dist);
                    }

                    DrawEntityESP(ped, localPos, entry.isAdmin);
                    drawnEntities++;
                } catch (...) { 
                    continue; 
                }
            }

            // ========== UNLIMITED FALLBACK ==========
            if (esp_Enable && esp_InfDistance) {
                uintptr_t nameListBase = addr->SafeRead<uintptr_t>(addr->m_NetIdToNamesEntry);
                if (addr->IsValid(nameListBase)) {
                    int infDistanceDrawCount = 0;
                    static constexpr int kMaxInfDistanceDrawCount = 40;
                    for (int id = 0; id < 2048; id++) {
                        try {
                            uintptr_t pNetPlayer = addr->SafeRead<uintptr_t>(nameListBase + (id * 8));
                            if (!addr->IsValid(pNetPlayer)) continue;

                            uintptr_t pPlayerInfo = addr->SafeRead<uintptr_t>(pNetPlayer + addr->m_PlayerInfo);
                            if (!addr->IsValid(pPlayerInfo)) continue;

                            int serverId = addr->SafeRead<int>(pPlayerInfo + addr->m_PlayerId, -1);
                            if (serverId == -1) serverId = id;

                            Vector3 netPos = Vector3(0, 0, 0);
                            
                            uintptr_t netCoordsAddr = pPlayerInfo + 0x7C0;
                            if (addr->CurrentBuild < 2060) {
                                netCoordsAddr = pPlayerInfo + 0x7D0;
                            }
                            netPos = addr->SafeRead<Vector3>(netCoordsAddr);

                            if (netPos.x == 0 || fabsf(netPos.x) > 100000.0f || fabsf(netPos.y) > 100000.0f) {
                                netPos = addr->SafeRead<Vector3>(pPlayerInfo + 0x1E0);
                            }
                            if (netPos.x == 0 || fabsf(netPos.x) > 100000.0f || fabsf(netPos.y) > 100000.0f) {
                                netPos = addr->SafeRead<Vector3>(pNetPlayer + 0x8C);
                            }

                            if (netPos.x == 0 || fabsf(netPos.x) > 100000.0f || fabsf(netPos.y) > 100000.0f) continue;

                            bool alreadyProcessed = false;
                            for (const auto& cached : s_EntityCache) {
                                if (cached.isPlayer && (cached.serverId == serverId || cached.serverId == id)) {
                                    alreadyProcessed = true;
                                    break;
                                }
                            }
                            if (alreadyProcessed) continue;

                            float dist = localPos.Distance(netPos);
                            
                            ImVec2 screenPos = core->W2S(netPos);
                            if (screenPos.x != 0) {
                                char pName[64] = "";
                                if (!GetPlayerNameFromMemory(serverId, pName, sizeof(pName))) {
                                    if (!GetPlayerNameFromMemory(id, pName, sizeof(pName))) {
                                        sprintf_s(pName, "Player [%d]", serverId);
                                    }
                                }

                                float textSize = esp_TextSize + 1.0f;
                                if (dist > 10000) textSize *= 0.7f;
                                if (dist > 50000) textSize *= 0.5f;
                                if (dist > 100000) textSize *= 0.3f;
                                if (textSize < 4.0f) textSize = 4.0f;

                                char line[128];
                                if (dist >= 1000) {
                                    sprintf_s(line, "%s - %.1fkm", pName, dist / 1000.0f);
                                } else {
                                    sprintf_s(line, "%s - %.0fm", pName, dist);
                                }
                                
                                ImVec2 tSize = ImGui::CalcTextSize(line);
                                float actualW = tSize.x * (textSize / ImGui::GetFontSize());

                                float distFactor = 1.0f;
                                if (dist > 50000) distFactor = 0.3f;
                                else if (dist > 10000) distFactor = 0.6f;
                                else if (dist > 5000) distFactor = 0.8f;
                                
                                ImColor textColor = ImColor(
                                    (int)(255 * distFactor),
                                    (int)(255 * distFactor),
                                    (int)(255 * distFactor),
                                    255
                                );

                                Graphics::DrawOutlinedText(
                                    ImVec2(screenPos.x - (actualW / 2.0f), screenPos.y), 
                                    textColor, 
                                    line, 
                                    textSize
                                );
                                
                                float markerSize = 3.0f;
                                if (dist > 10000) markerSize = 2.0f;
                                if (dist > 50000) markerSize = 1.5f;
                                if (dist > 100000) markerSize = 1.0f;
                                
                                Graphics::DrawCircle(
                                    screenPos.x, 
                                    screenPos.y - 8.0f, 
                                    markerSize, 
                                    ImColor(255, 255, 255, (int)(200 * distFactor)), 
                                    1.0f
                                );
                                
                                if (esp_Lines && dist < 50000) {
                                    ImVec2 screenOrigin = core->W2S(localPos); 
                                    if (screenOrigin.x == 0) {
                                        screenOrigin = ImVec2(
                                            ImGui::GetIO().DisplaySize.x / 2.0f, 
                                            ImGui::GetIO().DisplaySize.y / 2.0f
                                        );
                                    }
                                    ImGui::GetBackgroundDrawList()->AddLine(
                                        screenOrigin, 
                                        screenPos, 
                                        ImColor(255, 255, 255, (int)(80 * distFactor)), 
                                        1.0f
                                    );
                                }

                                {
                                    SRWLockGuard lockList(&g_PlayerListLock);
                                    PlayerListData data;
                                    strncpy_s(data.name, sizeof(data.name), pName, _TRUNCATE);
                                    data.distance = dist;
                                    data.isAdmin = false;
                                    data.serverId = serverId;
                                    data.ped = 0;
                                    
                                    bool dup = false;
                                    for (const auto& p : g_PlayerList) {
                                        if (p.serverId == serverId) { dup = true; break; }
                                    }
                                    if (!dup) g_PlayerList.push_back(data);
                                }
                            }
                        } catch (...) { 
                            continue; 
                        }
                    }
                }
            }
        }
    }

    // Vehicle ESP
    {
        uintptr_t replay = addr ? addr->SafeRead<uintptr_t>(addr->m_ReplayInterface) : 0;
        uintptr_t vehInterface = replay ? addr->SafeRead<uintptr_t>(replay + addr->m_VehicleInterface) : 0;
        if (addr && addr->IsValid(vehInterface)) {
            uintptr_t vehList = addr->SafeRead<uintptr_t>(vehInterface + addr->m_VehicleList);
            int vehMaximum = addr->SafeRead<int>(vehInterface + addr->m_VehicleMaximum);

            auto now = std::chrono::steady_clock::now();
            int vehCacheInterval = veh_InfDistance ? 200 : 500;
            if (std::chrono::duration_cast<std::chrono::milliseconds>(now - s_LastVehCacheUpdate).count() >= vehCacheInterval) {
                s_VehicleCache.clear();
                s_VehicleCache.reserve(256);
                if (addr->IsValid(vehList) && vehMaximum > 0 && vehMaximum < 1024) {
                    for (int i = 0; i < vehMaximum; i++) {
                        uintptr_t veh = addr->SafeRead<uintptr_t>(vehList + (i * 0x10));
                        if (!addr->IsValid(veh)) continue;

                        CachedVehicle cv;
                        cv.vehicle = veh;
                        Matrix4x4 matrix = addr->SafeRead<Matrix4x4>(veh + 0x60);
                        cv.pos = Vector3(matrix._41, matrix._42, matrix._43);
                        cv.lockStatus = addr->SafeRead<int>(veh + addr->m_VehLockStatus);

                        uintptr_t modelInfo = addr->SafeRead<uintptr_t>(veh + addr->m_VehModelInfo);
                        if (addr->IsValid(modelInfo)) {
                            const char* vname = (const char*)(modelInfo + 0x298);
                            try { 
                                strncpy_s(cv.name, vname, 31); 
                            } catch (...) { 
                                cv.name[0] = '\0'; 
                            }
                        } else cv.name[0] = '\0';

                        s_VehicleCache.push_back(cv);
                    }
                }
                s_LastVehCacheUpdate = now;
                SyncVehicleListInternal(localPos);
            }

            if (veh_Enable) {
                for (const auto& v : s_VehicleCache) {
                    DrawVehicleESP(v.vehicle, localPos);
                }
            }
        }
    }
}

void ESP::DrawEntityESP(uintptr_t entity, Vector3 localPos, bool isAdmin) {
    auto core = Core::Get();
    auto addr = Address::Get();

    auto GetEntityHealthRobust = [&](uintptr_t ped, float& outHealth, float& outMaxHealth) {
        static auto GetHandle = (int(*)(uintptr_t))GetProcAddress(GetModuleHandleA("fivem-mode-venice.dll"), "?PointerToHandle@five@@YAHPEAX@Z");
        int handle = GetHandle ? GetHandle(ped) : 0;
        
        bool nativeSuccess = false;
        if (handle != 0) {
            try {
                int h = Invoker::Call<int>(0xEEF0597D3C68CA3A, handle);
                int mh = Invoker::Call<int>(0x15D757C37EB470AB, handle);
                if (mh > 0) {
                    outHealth = (float)h;
                    outMaxHealth = (float)mh;
                    nativeSuccess = true;
                }
            } catch (...) {
                nativeSuccess = false;
            }
        }
        
        if (!nativeSuccess) {
            outHealth = addr->SafeRead<float>(ped + addr->m_Health);
            outMaxHealth = addr->SafeRead<float>(ped + addr->m_MaxHealth);
            if (outMaxHealth <= 1.0f) {
                outMaxHealth = addr->SafeRead<float>(ped + 0x2A0);
            }
            if (outMaxHealth <= 1.0f) {
                outMaxHealth = addr->SafeRead<float>(ped + 0x284);
            }
        }
    };

    Matrix4x4 matrix = addr->SafeRead<Matrix4x4>(entity + 0x60);
    Vector3 anchorPos = Vector3(matrix._41, matrix._42, matrix._43);
    if (anchorPos.x == 0) {
        anchorPos = addr->SafeRead<Vector3>(entity + addr->m_VisualPos);
    }

    if (fabsf(anchorPos.x) > 100000.0f || fabsf(anchorPos.y) > 100000.0f || fabsf(anchorPos.z) > 100000.0f) {
        return; 
    }

    float dist = localPos.Distance(anchorPos);
    uintptr_t localPlayer = addr->SafeRead<uintptr_t>(addr->SafeRead<uintptr_t>(addr->m_World) + 0x8);
    bool isSelf = (entity == localPlayer);
    if (isSelf) dist = 0.0f;
    
    float maxAllowedDist = esp_InfDistance ? 99999999.0f : esp_Distance;
    bool isNormalAllowed = (esp_Enable && dist <= maxAllowedDist) || (isSelf && esp_Self);
    if (!isAdmin && !isNormalAllowed) return;

    ImVec2 head = core->W2S(Vector3(anchorPos.x, anchorPos.y, anchorPos.z + 0.8f));
    ImVec2 head1 = core->W2S(Vector3(anchorPos.x, anchorPos.y, anchorPos.z + 0.9f));
    ImVec2 head2 = core->W2S(Vector3(anchorPos.x, anchorPos.y, anchorPos.z + 0.91f));
    ImVec2 foot = core->W2S(Vector3(anchorPos.x, anchorPos.y, anchorPos.z - 0.95f));
    ImVec2 foot1 = core->W2S(Vector3(anchorPos.x, anchorPos.y, anchorPos.z - 0.96f));
    ImVec2 foot2 = core->W2S(Vector3(anchorPos.x, anchorPos.y, anchorPos.z - 0.97f));

    auto IsValidScreenPos = [&](const ImVec2& p) {
        return p.x != 0 && p.y != 0 && std::isfinite(p.x) && std::isfinite(p.y);
    };

    if (!IsValidScreenPos(foot) || !IsValidScreenPos(head) || !IsValidScreenPos(head1) || !IsValidScreenPos(head2) ||
        !IsValidScreenPos(foot1) || !IsValidScreenPos(foot2)) {
        return;
    }

    float h = fabsf(foot.y - head.y);
    float w = h / 4.5f;
    float height = h;

    struct BoneEntry { int id; Vector3 pos; };
    BoneEntry localCache[50]; 
    int cacheCount = 0;

    auto GetBoneCached = [&](int boneid) -> Vector3 {
        for (int i = 0; i < cacheCount; i++) {
            if (localCache[i].id == boneid) return localCache[i].pos;
        }
        Vector3 b = core->GetBone(entity, boneid);
        if (cacheCount < 50) {
            localCache[cacheCount++] = { boneid, b };
        }
        return b;
    };

    Vector3 pelvisPos = GetBoneCached(0x2e28);

    if (esp_Box && (isNormalAllowed || isAdmin)) {
        ImColor boxCol = isAdmin ? ImColor(235, 30, 30, 220) : ImColor((int)(col_Box[0] * 255), (int)(col_Box[1] * 255), (int)(col_Box[2] * 255), (int)(col_Box[3] * 255));
        ImGui::GetBackgroundDrawList()->AddRect({ foot.x - w, foot.y }, { foot.x + w, head.y }, boxCol, 0, 0, 1.2f);
    }

    ImVec2 screenFoot = core->W2S(pelvisPos - Vector3(0, 0, 0.95f));
    ImVec2 screenTop = core->W2S(GetBoneCached(0x796e) + Vector3(0, 0, 0.15f));
    if (screenFoot.x == 0) screenFoot = foot;
    if (screenTop.x == 0) screenTop = head;

    if (esp_Healthbar && (isNormalAllowed || isAdmin)) {
        float health = 0.0f;
        float maxHealth = 200.0f;
        GetEntityHealthRobust(entity, health, maxHealth);

        if (IsPlayer(entity) && maxHealth > 120.0f && health > 100.0f) {
            health -= 100.0f;
            maxHealth -= 100.0f;
        }

        if (maxHealth <= 0) maxHealth = 100.0f;
        if (health < 0) health = 0;
        if (health > maxHealth) health = maxHealth;

        float hpPercent = health / maxHealth;
        ImColor healthCol = ImColor(0, 255, 0, 255);
        if (hpPercent < 0.5f) healthCol = ImColor(200, 200, 0, 255);
        if (hpPercent < 0.25f) healthCol = ImColor(255, 0, 0, 255);

        float barTop = head.y;
        float barBottom = foot.y;
        if (!std::isfinite(barTop) || !std::isfinite(barBottom)) return;
        if (barTop > barBottom) std::swap(barTop, barBottom);
        float barHeight = barBottom - barTop;
        if (!std::isfinite(barHeight) || barHeight <= 0.0f) return;
        float healthHeight = barHeight * hpPercent;
        if (hpPercent > 0.0f && healthHeight < 1.0f) healthHeight = 1.0f;

        float barLeft = foot.x + w + 4;
        float barRight = foot.x + w + 7;
        if (!std::isfinite(barLeft) || !std::isfinite(barRight)) return;

        ImGui::GetBackgroundDrawList()->AddRect(ImVec2(barLeft, barTop + 1.0f), ImVec2(barRight, barBottom - 1.0f), ImColor(0, 0, 0, 255), 0.0f, 0, 1.0f);
        ImGui::GetBackgroundDrawList()->AddRectFilled(ImVec2(barLeft + 1.0f, barTop), ImVec2(barRight - 1.0f, barBottom), ImColor(0, 0, 0, 255));
        ImGui::GetBackgroundDrawList()->AddRectFilled(ImVec2(barLeft + 1.0f, barBottom - healthHeight), ImVec2(barRight - 1.0f, barBottom - 1.0f), healthCol);
    }

    if (esp_Armorbar && (isNormalAllowed || isAdmin)) {
        float armor = addr->SafeRead<float>(entity + addr->m_Armor);
        if (armor > 0.1f) {
            float armorRatio = armor / 100.0f;
            if (armorRatio < 0.0f) armorRatio = 0.0f;
            if (armorRatio > 1.0f) armorRatio = 1.0f;

            float barTop = head.y;
            float barBottom = foot.y;
            if (!std::isfinite(barTop) || !std::isfinite(barBottom)) return;
            if (barTop > barBottom) std::swap(barTop, barBottom);
            float barHeight = barBottom - barTop;
            if (!std::isfinite(barHeight) || barHeight <= 0.0f) return;
            float armorHeight = barHeight * armorRatio;
            if (armorRatio > 0.0f && armorHeight < 1.0f) armorHeight = 1.0f;

            float barLeft = foot.x - w - 8;
            float barRight = foot.x - w - 5;
            if (!std::isfinite(barLeft) || !std::isfinite(barRight)) return;

            ImGui::GetBackgroundDrawList()->AddRect(ImVec2(barLeft, barTop + 1.0f), ImVec2(barRight, barBottom - 1.0f), ImColor(0, 0, 0, 255), 0.0f, 0, 1.0f);
            ImGui::GetBackgroundDrawList()->AddRectFilled(ImVec2(barLeft + 1.0f, barTop), ImVec2(barRight - 1.0f, barBottom), ImColor(0, 0, 0, 255));
            ImGui::GetBackgroundDrawList()->AddRectFilled(ImVec2(barLeft + 1.0f, barBottom - armorHeight), ImVec2(barRight - 1.0f, barBottom - 1.0f), ImColor(58, 95, 205, 255));
        }
    }

    if ((esp_Skeleton || (isAdmin && admin_Skeleton)) && (isNormalAllowed || isAdmin)) {
        ImColor skelCol = isAdmin ? ImColor(235, 30, 30, 200) : ImColor((int)(col_Skeleton[0] * 255), (int)(col_Skeleton[1] * 255), (int)(col_Skeleton[2] * 255), (int)(col_Skeleton[3] * 255));

        static const int pairs[][2] = {
            { 0x9995, 0xfcd9 }, { 0x9995, 0x29d2 }, { 0xfcd9, 0xb1c5 }, { 0x29d2, 0x9d4d },
            { 0xb1c5, 0x58b7 }, { 0x9d4d, 0xbb0 }, { 0x58b7, 0xeeeb }, { 0xbb0, 0x6e5c },
            { 0xeeeb, 0x49d9 }, { 0x6e5c, 0xdead }, { 0x9995, 0x2e28 }, { 0x2e28, 0xe39f }, { 0x2e28, 0xca72 },
            { 0xe39f, 0xf9bb }, { 0xca72, 0x9000 }, { 0xf9bb, 0x3779 }, { 0x9000, 0xcc4d }, { 0x9995, 0x796e }
        };

        for (const auto& pair : pairs) {
            Vector3 b1 = GetBoneCached(pair[0]);
            Vector3 b2 = GetBoneCached(pair[1]);
            if (b1.x == 0 || b2.x == 0) continue;
            ImVec2 s1 = core->W2S(b1);
            ImVec2 s2 = core->W2S(b2);
            if (s1.x != 0 && s2.x != 0) ImGui::GetBackgroundDrawList()->AddLine(s1, s2, skelCol, 1.2f);
        }
    }

    if (esp_Head && (isNormalAllowed || isAdmin)) {
        Vector3 headPos = GetBoneCached(0x796e);
        if (headPos.x == 0) headPos = anchorPos + Vector3(0, 0, 0.85f);
        Vector3 headTop = headPos;
        headTop.z += 0.05f;
        ImVec2 screenHead = core->W2S(headTop);
        if (screenHead.x != 0) {
            float headRadius = height / 22.0f;
            if (headRadius < 1.2f) headRadius = 1.2f;
            ImColor headCol;
            if (isAdmin) {
                float time = (float)std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::steady_clock::now().time_since_epoch()).count() / 1000.0f;
                float r = sinf(time * 5.0f) * 0.5f + 0.5f;
                float g = sinf(time * 5.0f + 2.0f) * 0.5f + 0.5f;
                float b = sinf(time * 5.0f + 4.0f) * 0.5f + 0.5f;
                headCol = ImColor(r, g, b, 1.0f);
            } else {
                headCol = ImColor(255, 255, 255, 255);
            }
            ImGui::GetBackgroundDrawList()->AddCircleFilled(screenHead, headRadius, headCol, 100);
        }
    }

    if (esp_Lines && (isNormalAllowed || isAdmin)) {
        ImVec2 screenOrigin = core->W2S(localPos); 
        if (screenOrigin.x == 0) screenOrigin = ImVec2(ImGui::GetIO().DisplaySize.x / 2.0f, ImGui::GetIO().DisplaySize.y / 2.0f);
        ImVec2 screenLine = core->W2S(pelvisPos);
        if (screenLine.x != 0) {
            ImColor lineCol;
            if (isAdmin && admin_Lines) {
                float time = (float)std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::steady_clock::now().time_since_epoch()).count() / 1000.0f;
                float r = sinf(time * 3.0f + 0.0f) * 0.5f + 0.5f;
                float g = sinf(time * 3.0f + 2.094f) * 0.5f + 0.5f;
                float b = sinf(time * 3.0f + 4.188f) * 0.5f + 0.5f;
                lineCol = ImColor(r, g, b, 0.9f);
            } else {
                lineCol = ImColor(255, 255, 255, 160);
            }
            ImGui::GetBackgroundDrawList()->AddLine(screenOrigin, screenLine, lineCol, 2.0f);
        }
    }

    float actualTextSize = esp_TextSize + 1.0f;
    float textOffset = 3.0f;
    float targetX = (screenFoot.x != 0) ? screenFoot.x : screenTop.x;
    float targetY = (screenFoot.y != 0) ? screenFoot.y : (screenTop.y + height);

    char name[64] = "";
    if (esp_Names || isAdmin) GetPlayerName(entity, name, sizeof(name));

    float rawHealth = 0.0f;
    float rawMaxHealth = 200.0f;
    GetEntityHealthRobust(entity, rawHealth, rawMaxHealth);

    char line1[128] = "";
    if (name[0] != '\0') {
        if (esp_ShowDistance || isAdmin) {
            int displayDist = (int)dist;
            sprintf_s(line1, "%s - %dm", name, displayDist);
        } else {
            strcpy_s(line1, name);
        }
    } else if (esp_ShowDistance || isAdmin) {
        int displayDist = (int)dist;
        sprintf_s(line1, "%dm", displayDist);
    }

    if (line1[0] != '\0') {
        ImVec2 tSize = ImGui::CalcTextSize(line1);
        float actualW = tSize.x * (actualTextSize / ImGui::GetFontSize());
        Graphics::DrawOutlinedText(ImVec2(targetX - (actualW / 2.0f), targetY + textOffset), ImColor(255, 255, 255, 255), line1, actualTextSize);
        textOffset += actualTextSize + 1.5f;
    }

    if (esp_Weapons && (isNormalAllowed || isAdmin)) {
        char weapon[64];
        GetWeaponName(entity, weapon, sizeof(weapon));
        if (weapon[0] != '\0') {
            ImVec2 tSize = ImGui::CalcTextSize(weapon);
            float actualW = tSize.x * (actualTextSize / ImGui::GetFontSize());
            Graphics::DrawOutlinedText(ImVec2(targetX - (actualW / 2.0f), targetY + textOffset), ImColor(255, 255, 255, 255), weapon, actualTextSize);
        }
    }
}

void ESP::GetPlayerName(uintptr_t ped, char* outBuf, int bufLen) {
    if (bufLen <= 0) return;
    outBuf[0] = '\0';

    bool isPlayerPed = IsPlayer(ped);
    if (!isPlayerPed) {
        strncpy_s(outBuf, bufLen, "Ped", _TRUNCATE);
        return;
    }

    auto addr = Address::Get();
    int serverId = GetServerIdInternal(ped);
    auto now = std::chrono::steady_clock::now();

    if (serverId >= 0 && serverId < 2048) {
        if (s_NameCacheValid[serverId]) {
            CachedName& cn = s_NameCacheArray[serverId];
            if (std::chrono::duration_cast<std::chrono::seconds>(now - cn.lastUpdate).count() < 30 && cn.name[0] != '\0') {
                strncpy_s(outBuf, bufLen, cn.name, _TRUNCATE);
                return;
            }
        }
    }

    char finalName[64] = "";
    if (GetPlayerNameFromMemory(serverId, finalName, sizeof(finalName))) {}

    if (finalName[0] == '\0' && addr->m_GetPlayerName) {
        uintptr_t pInfo = addr->SafeRead<uintptr_t>(ped + addr->m_PlayerInfo, 0);
        if (addr->IsValid(pInfo)) {
            void* m_net_player_data = (void*)(pInfo + 0x20); 
            if (m_net_player_data) {
                const char* name = SafeGetPlayerNameHook(m_net_player_data);
                if (name && IsValidPlayerName(name)) {
                    strncpy_s(finalName, sizeof(finalName), name, _TRUNCATE);
                }
            }
        }
    }

    if (finalName[0] == '\0') {
        if (serverId != -1) sprintf_s(finalName, "Player %d", serverId);
        else strncpy_s(finalName, sizeof(finalName), "Unarmed", _TRUNCATE);
    }

    if (serverId >= 0 && serverId < 2048) {
        CachedName& cn = s_NameCacheArray[serverId];
        strncpy_s(cn.name, sizeof(cn.name), finalName, _TRUNCATE);
        cn.lastUpdate = now;
        s_NameCacheValid[serverId] = true;
    }

    strncpy_s(outBuf, bufLen, finalName, _TRUNCATE);
}

void ESP::GetWeaponName(uintptr_t ped, char* outBuf, int bufLen) {
    if (bufLen <= 0) return;
    outBuf[0] = '\0';

    auto addr = Address::Get();
    bool isNpc = !IsPlayer(ped);

    uintptr_t weaponManager = 0; static uintptr_t weaponOffs[] = { addr->m_WeaponManager, 0x10D8, 0x10C8, 0x10E8, 0x10B8 }; for (uintptr_t off : weaponOffs) { if (off == 0) continue; uintptr_t cand = addr->SafeRead<uintptr_t>(ped + off, 0); if (addr->IsValid(cand)) { uintptr_t currentWeapon = addr->SafeRead<uintptr_t>(cand + 0x20, 0); if (addr->IsValid(currentWeapon)) { weaponManager = cand; break; } } }
    if (!addr->IsValid(weaponManager)) {
        strncpy_s(outBuf, bufLen, "Unarmed", _TRUNCATE);
        return;
    }

    uintptr_t pWeapon = addr->SafeRead<uintptr_t>(weaponManager + 0x20); uintptr_t currentWeapon = addr->SafeRead<uintptr_t>(pWeapon + 0x20); if (!addr->IsValid(currentWeapon)) currentWeapon = pWeapon;
    if (!addr->IsValid(currentWeapon)) {
        strncpy_s(outBuf, bufLen, "Unarmed", _TRUNCATE);
        return;
    }

    uint32_t hash = addr->SafeRead<uint32_t>(currentWeapon + 0x10); if (hash == 0 || hash == 0xFFFFFFFF) hash = addr->SafeRead<uint32_t>(currentWeapon + 0x08);
    const char* name = "Unarmed";

    switch (hash) {
    case 0x1B06D571: name = "Pistol"; break;
    case 0xBFE256D4: name = "Pistol Mk2"; break;
    case 0x5EF9FEC4: name = "Combat Pistol"; break;
    case 0x22D8FE39: name = "AP Pistol"; break;
    case 0x99AEEB3B: name = "Pistol .50"; break;
    case 0xBFD21232: name = "SNS Pistol"; break;
    case 0x88374054: name = "SNS Pistol Mk2"; break;
    case 0xD205520E: name = "Heavy Pistol"; break;
    case 0x83839C4: name = "Vintage Pistol"; break;
    case 0xDC4DB296: name = "Marksman Pistol"; break;
    case 0xC1B3C3D1: name = "Revolver"; break;
    case 0xCB96392F: name = "Revolver Mk2"; break;
    case 0x97EA20B8: name = "Double Action"; break;
    case 0x47757124: name = "Flare Gun"; break;
    case 0xAF3696A1: name = "Up-n-Atomizer"; break;
    case 0x2B5EF5EC: name = "Ceramic Pistol"; break;
    case 0x917F6C8C: name = "Navy Revolver"; break;
    case 0x57A4368C: name = "Perico Pistol"; break;
    case 0x1BC4FDB9: name = "WM 29 Pistol"; break;
    case 0x3656C8C1: name = "Stun Gun"; break;
    case 1171102963: name = "Stun Gun 2"; break;
    case 0x2BE6766B: name = "SMG"; break;
    case 0x78A97CD1: name = "SMG Mk2"; break;
    case 0x13532244: name = "Micro SMG"; break;
    case 0xEFE7E2DF: name = "Assault SMG"; break;
    case 0xA3D4D34: name = "Combat PDW"; break;
    case 0xDB1AA450: name = "Machine Pistol"; break;
    case 0xBD251F15: name = "Mini SMG"; break;
    case 0x14E5AF95: name = "Tactical SMG"; break;
    case 0xBFEFFF6D: name = "Assault Rifle"; break;
    case 0x394F415C: name = "Assault Rifle Mk2"; break;
    case 0x83BF0278: name = "Carbine Rifle"; break;
    case 0xFAD1F1C9: name = "Carbine Rifle Mk2"; break;
    case 0xAF113F99: name = "Advanced Rifle"; break;
    case 0xC0A3098D: name = "Special Carbine"; break;
    case 0x969C3D67: name = "Special Carbine Mk2"; break;
    case 0x7F229F94: name = "Bullpup Rifle"; break;
    case 0x84BD7B5D: name = "Bullpup Rifle Mk2"; break;
    case 0x624FE830: name = "Compact Rifle"; break;
    case 0x9D1F17E6: name = "Military Rifle"; break;
    case 0xC78D71B4: name = "Heavy Rifle"; break;
    case 0xD1D5F52B: name = "Service Carbine"; break;
    case 0x72B7B5F1: name = "Battle Rifle"; break;
    case 0x7FD62962: name = "Combat MG"; break;
    case 0xDBBD7280: name = "Combat MG Mk2"; break;
    case 0x9D74C243: name = "MG"; break;
    case 0x61012683: name = "Gusenberg Sweeper"; break;
    case 0x1D073A89: name = "Pump Shotgun"; break;
    case 0x555AF99A: name = "Pump Shotgun Mk2"; break;
    case 0x7846A318: name = "Sawn-Off Shotgun"; break;
    case 0xE289C71F: name = "Assault Shotgun"; break;
    case 0x9D61E50F: name = "Bullpup Shotgun"; break;
    case 0x3AA5DC2A: name = "Heavy Shotgun"; break;
    case 0xEF94E131: name = "Double Barrel Shotgun"; break;
    case 0x12E82D3D: name = "Sweeper Shotgun"; break;
    case 0xA89CB99E: name = "Musket"; break;
    case 0x05A96BA4: name = "Combat Shotgun"; break;
    case 0x05FC3C11: name = "Sniper Rifle"; break;
    case 0x0C472FE2: name = "Heavy Sniper"; break;
    case 0xA914799: name = "Heavy Sniper Mk2"; break;
    case 0xC735A67A: name = "Marksman Rifle"; break;
    case 0x6A6C02E0: name = "Marksman Rifle Mk2"; break;
    case 0x6E7C8E2C: name = "Precision Rifle"; break;
    case 0xB12C4D61: name = "RPG"; break;
    case 0x63AB0442: name = "Homing Launcher"; break;
    case 0x42A958AD: name = "Minigun"; break;
    case 0xFE8E9A4C: name = "Railgun"; break;
    case 0xA3A3B903: name = "Grenade Launcher"; break;
    case 0x781FE4A: name = "Compact Grenade Launcher"; break;
    case 0xB60D13F7: name = "Widowmaker"; break;
    case 0x9D6E0CCF: name = "Hellbringer"; break;
    case 0x476BF155: name = "Unholy Hellbringer"; break;
    case 0xDB2C4E1A: name = "EMP Launcher"; break;
    case 0x3BF5575: name = "Snowball Launcher"; break;
    case 0x5FBF9342: name = "Sticky Bomb"; break;
    case 0x943F1F8D: name = "Grenade"; break;
    case 0xFDB58A50: name = "Tear Gas"; break;
    case 0x24B0A310: name = "Molotov"; break;
    case 0xBAE7C398: name = "Pipe Bomb"; break;
    case 0xABCA1F3B: name = "Proximity Mine"; break;
    case 0xA0F6B2DE: name = "BZ Gas"; break;
    case 0xABF1372A: name = "Smoke Grenade"; break;
    case 0xBA45E8D0: name = "Snowball"; break;
    case 0x7F5C4D55: name = "Firework Launcher"; break;
    case 0x060C9C76: name = "Fire Extinguisher"; break;
    case 0x34F5E957: name = "Jerry Can"; break;
    case 0xBA9C0B2A: name = "Hazardous Jerry Can"; break;
    case 0x18414041: name = "Fertilizer Can"; break;
    case 0x9976FF8A: name = "Knife"; break;
    case 0xDF8A0000: name = "Switchblade"; break;
    case 0x9213D3C7: name = "Dagger"; break;
    case 0xF9A9C93D: name = "Hatchet"; break;
    case 0xDD6FBD09: name = "Machete"; break;
    case 0xCD87A8F9: name = "Battle Axe"; break;
    case 0x43A3F6A8: name = "Golf Club"; break;
    case 0x84A9A1AD: name = "Crowbar"; break;
    case 0x9548385F: name = "Baseball Bat"; break;
    case 0x4E9E9643: name = "Hammer"; break;
    case 0x190B23F0: name = "Wrench"; break;
    case 0x9414F135: name = "Pool Cue"; break;
    case 0x8C32C4A7: name = "Flashlight"; break;
    case 0x67591C31: name = "Nightstick"; break;
    case 0xF9E1B2AB: name = "Bottle"; break;
    case 0x3806F1C8: name = "Stone Hatchet"; break;
    case 0xD814F0E4: name = "Knuckle Duster"; break;
    case 0xDD6FBD1B: name = "Go Go Monkey Blaster"; break;
    case 0x657A84BA: name = "Candy Cane"; break;
    case 0xDA7017E5: name = "The Shocker"; break;
    case 0x5B3957CD: name = "Parachute"; break;
    case 0x1CF5FA48: name = "Hacking Device"; break;
    default: name = "Unarmed"; break;
    }

    if (name) strncpy_s(outBuf, bufLen, name, _TRUNCATE);
}

void ESP::DrawVehicleESP(uintptr_t vehicle, Vector3 localPos) {
    auto core = Core::Get();
    auto addr = Address::Get();

    Matrix4x4 matrix = addr->SafeRead<Matrix4x4>(vehicle + 0x60);
    Vector3 pos = Vector3(matrix._41, matrix._42, matrix._43);

    if (pos.x == 0 || fabsf(pos.x) > 100000.0f || fabsf(pos.y) > 100000.0f || fabsf(pos.z) > 100000.0f) {
        return;
    }

    int lockStatus = addr->SafeRead<int>(vehicle + addr->m_VehLockStatus);
    char name[32] = { 0 };

    for (const auto& v : s_VehicleCache) {
        if (v.vehicle == vehicle) {
            strcpy_s(name, v.name);
            break;
        }
    }

    if (name[0] == '\0') {
        uintptr_t modelInfo = addr->SafeRead<uintptr_t>(vehicle + addr->m_VehModelInfo);
        if (addr->IsValid(modelInfo)) {
            const char* vname = (const char*)(modelInfo + 0x298);
            try { 
                strncpy_s(name, vname, 31); 
            } catch (...) { 
                name[0] = '\0'; 
            }
        }
    }

    float dist = localPos.Distance(pos);
    if (!veh_InfDistance && dist > veh_Distance) return;

    ImVec2 screenPos = core->W2S(pos);
    if (screenPos.x == 0) return;

    float yOffset = 0.0f;
    ImColor textColor = ImColor(1.0f, 1.0f, 1.0f, 1.0f);

    if (veh_Name && name[0] != '\0') {
        Graphics::DrawText2(name, screenPos.x, screenPos.y + yOffset, veh_TextSize, textColor, true);
        yOffset += veh_TextSize;
    }

    if (veh_ShowDistance) {
        char distStr[32];
        sprintf_s(distStr, "[%.0fm]", dist);
        Graphics::DrawText2(distStr, screenPos.x, screenPos.y + yOffset, veh_TextSize, textColor, true);
        yOffset += veh_TextSize;
    }

    if (veh_LockState) {
        const char* status = (lockStatus == 2) ? "Locked" : "Unlocked";
        ImColor statusColor = (lockStatus == 2) ? ImColor(1.0f, 0.0f, 0.0f, 1.0f) : ImColor(0.0f, 1.0f, 0.0f, 1.0f);
        Graphics::DrawText2(status, screenPos.x, screenPos.y + yOffset, veh_TextSize, statusColor, true);
        yOffset += veh_TextSize;
    }

    if (veh_Marker) {
        Graphics::DrawCircle(screenPos.x, screenPos.y, 4.0f, ImColor(1.0f, 0.0f, 0.0f, 1.0f), 1.2f);
    }

    if (veh_Lines) {
        ImVec2 screenOrigin = core->W2S(localPos);
        if (screenOrigin.x == 0) screenOrigin = ImVec2(ImGui::GetIO().DisplaySize.x / 2.0f, ImGui::GetIO().DisplaySize.y / 2.0f);
        Graphics::DrawLine(screenOrigin.x, screenOrigin.y, screenPos.x, screenPos.y, ImColor(1.0f, 1.0f, 1.0f, 0.6f), 1.2f);
    }
}

bool ESP::IsPlayer(uintptr_t ped) {
    auto addr = Address::Get();
    if (!addr->IsValid(ped)) return false;

    uintptr_t pInfo = addr->SafeRead<uintptr_t>(ped + addr->m_PlayerInfo, 0);
    return addr->IsValid(pInfo);
}

int ESP::GetServerIdInternal(uintptr_t ped) {
    auto addr = Address::Get();
    try {
        uintptr_t pInfo = addr->SafeRead<uintptr_t>(ped + addr->m_PlayerInfo, 0);
        if (!addr->IsValid(pInfo)) {
            static uintptr_t pInfoOffs[] = { 0x10C8, 0x10A8, 0x10D0, 0x10D8, 0x10B8 };
            for (uintptr_t off : pInfoOffs) {
                uintptr_t cand = addr->SafeRead<uintptr_t>(ped + off, 0);
                if (addr->IsValid(cand)) {
                    pInfo = cand;
                    break;
                }
            }
        }

        if (!addr->IsValid(pInfo)) return -1;

        int serverId = addr->SafeRead<int>(pInfo + 0xE8, -1);
        if (serverId < 0 || serverId > 65535) {
            serverId = addr->SafeRead<int>(pInfo + 0x88, -1);
        }
        return serverId;
    } catch (...) {
        return -1;
    }
}

bool ESP::GetPlayerNameFromMemory(int serverId, char* outBuf, int bufLen) {
    auto addr = Address::Get();
    if (serverId < 0 || serverId > 65535) return false;

    uintptr_t nameListBase = addr->SafeRead<uintptr_t>(addr->m_NetIdToNamesEntry);
    if (!addr->IsValid(nameListBase)) return false;

    uintptr_t pNetPlayer = addr->SafeRead<uintptr_t>(nameListBase + (serverId * 8));
    if (!addr->IsValid(pNetPlayer)) return false;

    static const uintptr_t nameOffs[] = { 0xAC, 0x88, 0x7C, 0xA4, 0x5C };

    for (uintptr_t off : nameOffs) {
        const char* name = (const char*)(pNetPlayer + off);
        if (addr->IsValid((uintptr_t)name)) {
            char temp[64];
            bool success = false;
            try {
                for (int i = 0; i < 63; i++) {
                    char c = addr->SafeRead<char>((uintptr_t)name + i, '\0');
                    if (c == '\0') {
                        temp[i] = '\0';
                        success = (i >= 2);
                        break;
                    }
                    unsigned char uc = (unsigned char)c;
                    if (uc < 0x20 || uc == 0x7F) {
                        success = false;
                        break;
                    }
                    temp[i] = c;
                }
                temp[63] = '\0';
                if (success) {
                    strncpy_s(outBuf, bufLen, temp, _TRUNCATE);
                    return true;
                }
            } catch (...) {}
        }
    }
    return false;
}

