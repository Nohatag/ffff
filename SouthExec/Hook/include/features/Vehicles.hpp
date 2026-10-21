#pragma once
#include "../content/classes.hpp"
#include "../settings/settings.hpp"
#include "../fivem/native.hpp"

namespace hook {

    hk_Vehicle* GetBestVehicle()
    {
        hk_Vehicle* ClosestVehicle = nullptr;
        float closestdist = FLT_MAX;
        float dist;

        hk_World* World = (hk_World*)*(uint64_t*)(FiveM::World);
        if (!World)
            return nullptr;

        hk_Ped* LocalPlayer = World->LocalPlayer();
        if (!LocalPlayer)
            return nullptr;

        hk_ReplayInterface* ReplayInterface2 = (hk_ReplayInterface*)*(uint64_t*)(FiveM::ReplayInterface);
        if (!ReplayInterface2)
            return nullptr;

        hk_VehicleInterface* VehicleInterface2 = ReplayInterface2->VehicleInterface();
        if (!VehicleInterface2)
            return nullptr;

        for (int index = 0; index < VehicleInterface2->VehicleMaximum(); index++)
        {
            hk_Vehicle* Vehicle = VehicleInterface2->VehicleList()->Vehicle(index);

            if (!Vehicle) continue;

            Vector3 DistanceCalculation = (LocalPlayer->m_visual_pos() - Vehicle->m_visual_pos());
            double Distance = sqrtf(DistanceCalculation.x * DistanceCalculation.x + DistanceCalculation.y * DistanceCalculation.y + DistanceCalculation.z * DistanceCalculation.z);

            if (Distance <= 150)
            {
                ImVec2 PedPos = FiveM::WorldToScreen(Vehicle->m_visual_pos());
                ImVec2 middle = ImVec2(FiveM::WindowSize.x / 2, FiveM::WindowSize.y / 2);
                dist = FiveM::pythag(middle, PedPos);

                if (dist < closestdist)
                {
                    closestdist = dist;
                    ClosestVehicle = Vehicle;
                }
            }
        }
        return ClosestVehicle;
    }

    inline Vector3 GetCoordsInfrontOfCam(Vector3 CamPos, Vector3 CamRot, float distance) {
        return CamPos + CamRot * distance;
    }

    void vehicle(void) {

        hk_World* World = (hk_World*)*(uint64_t*)(FiveM::World);
        if (!World)
            return;

        hk_Ped* LocalPlayer = World->LocalPlayer( );
        if (!LocalPlayer)
            return;

        hk_ReplayInterface* ReplayInterface = (hk_ReplayInterface*)*(uint64_t*)(FiveM::ReplayInterface);
        if (!ReplayInterface)
            return;

        hk_VehicleInterface* VehicleInterface = ReplayInterface->VehicleInterface( );
        if (!VehicleInterface)
            return;

        ImVec2 LocalPlayer_Location = FiveM::WorldToScreen(LocalPlayer->GetCoordinate( ));

        hk_Vehicle* ActualVehicle = LocalPlayer->VehicleManager( );
        if (!ActualVehicle)
            return;

        auto player = PLAYER::PLAYER_PED_ID();
        auto trying_to_open_veicle = PED::GET_VEHICLE_PED_IS_TRYING_TO_ENTER(player);
        auto current_vehicle = PED::GET_VEHICLE_PED_IS_IN(PLAYER::PLAYER_PED_ID(), true);

        if (settings::vehicle::vehicle_speed_bool) 
        {
        }

        if (settings::vehicle::vehicle_godmode) {

            ENTITY::SET_ENTITY_INVINCIBLE(current_vehicle, true);
            ENTITY::SET_ENTITY_PROOFS(current_vehicle, 1, 1, 1, 1, 1, 1, 1, 1);
            VEHICLE::SET_VEHICLE_TYRES_CAN_BURST(current_vehicle, 0);
            VEHICLE::SET_VEHICLE_WHEELS_CAN_BREAK(current_vehicle, 0);
            VEHICLE::SET_VEHICLE_CAN_BE_VISIBLY_DAMAGED(current_vehicle, 0);
            VEHICLE::SET_VEHICLE_TYRES_CAN_BURST(current_vehicle, false);
            VEHICLE::SET_VEHICLE_WHEELS_CAN_BREAK(current_vehicle, false);
            VEHICLE::SET_VEHICLE_HAS_STRONG_AXLES(current_vehicle, true);

        }
        bool fallOffRestore = false;
        if (settings::vehicle::dont_fall)
        {
                PED::SET_PED_CAN_BE_KNOCKED_OFF_VEHICLE(PLAYER::PLAYER_PED_ID(), true);
            fallOffRestore = true;
        }
        else if (fallOffRestore)
        {
            PED::SET_PED_CAN_BE_KNOCKED_OFF_VEHICLE(PLAYER::PLAYER_PED_ID(), false);
            fallOffRestore = false;
        }
        if (settings::vehicle::vehicle_repair) {

            VEHICLE::SET_VEHICLE_ENGINE_HEALTH(current_vehicle, 1000.0);
            VEHICLE::SET_VEHICLE_BODY_HEALTH(current_vehicle, 1000.0);
            VEHICLE::SET_VEHICLE_TYRE_FIXED(current_vehicle, 1);
            VEHICLE::SET_DISABLE_VEHICLE_ENGINE_FIRES(current_vehicle, true);
            VEHICLE::_SET_VEHICLE_WHEELS_DEAL_DAMAGE(current_vehicle, false);
            VEHICLE::SET_VEHICLE_FIXED(current_vehicle);

            settings::vehicle::vehicle_repair = false;
        }
        if (settings::vehicle::SinkCar)
        {
            ENTITY::SET_ENTITY_COLLISION(current_vehicle, false, false);
        }
        if (settings::vehicle::vehicle_unlock) {

            VEHICLE::SET_VEHICLE_DOORS_LOCKED(current_vehicle, 1);

            settings::vehicle::vehicle_unlock = false;
        }

        if (settings::vehicle::vehicle_lock) {

            VEHICLE::SET_VEHICLE_DOORS_LOCKED(current_vehicle, 3);

            settings::vehicle::vehicle_lock = false;
        }

        if (settings::vehicle::vehicle_esp) {
            for (int i = 0; i < VehicleInterface->VehicleMaximum( ); i++) {

                hk_Vehicle* Vehicle = VehicleInterface->VehicleList( )->Vehicle(i);
                if (!Vehicle)
                    return;

                ImVec2 Vehicle_location = FiveM::WorldToScreen(Vehicle->GetCoordinate( ));
                if (!IsOnScreen(Vehicle_location))
                    continue;

                Vector3 DistanceCalculation = (LocalPlayer->GetCoordinate( ) - Vehicle->GetCoordinate( ));

                int Distance = sqrtf(DistanceCalculation.x * DistanceCalculation.x + DistanceCalculation.y * DistanceCalculation.y + DistanceCalculation.z * DistanceCalculation.z);

                if (Distance < settings::visuals::vehicle_distance) {

                    if (LocalPlayer->GetHealth( ) <= 100)
                        continue;

                    const char* caca = Vehicle->VehicleModelInfo( )->GetCharName( );
                    if (settings::vehicle::vehicle_name) {
                        DrawOutlinedText(ImGui::GetFont( ), caca, ImVec2(Vehicle_location.x, Vehicle_location.y), 13.0f, settings::visuals::vehicle_esp_color, true);
                    }

                    if (settings::vehicle::vehicle_health) {

                        int bite = Vehicle->GetHealth( );
                        if (!settings::vehicle::vehicle_name) {
                            if (Vehicle->GetHealth( ) != 0) {

                                DrawOutlinedText(ImGui::GetFont( ), std::to_string(bite), ImVec2(Vehicle_location.x, Vehicle_location.y + 10), 13.0f, settings::visuals::vehicle_esp_health, true);
                            } else {
                                DrawOutlinedText(ImGui::GetFont( ), "[BROKEN]", ImVec2(Vehicle_location.x, Vehicle_location.y + 10), 13.0f, settings::visuals::vehiclebroken_esp_color, true);
                            }
                        }

                        else if (settings::vehicle::vehicle_distance) {

                            if (Vehicle->GetHealth( ) != 0) {

                                DrawOutlinedText(ImGui::GetFont( ), std::to_string(bite), ImVec2(Vehicle_location.x, Vehicle_location.y + 20), 13.0f, settings::visuals::vehicle_esp_health, true);
                            } else {
                                DrawOutlinedText(ImGui::GetFont( ), "[BROKEN]", ImVec2(Vehicle_location.x, Vehicle_location.y + 20), 13.0f, settings::visuals::vehiclebroken_esp_color, true);
                            }

                        } else {

                            if (Vehicle->GetHealth( ) != 0) {

                                DrawOutlinedText(ImGui::GetFont( ), std::to_string(bite), ImVec2(Vehicle_location.x, Vehicle_location.y + 10), 13.0f, settings::visuals::vehicle_esp_health, true);
                            } else {
                                DrawOutlinedText(ImGui::GetFont( ), "[BROKEN]", ImVec2(Vehicle_location.x, Vehicle_location.y + 10), 13.0f, settings::visuals::vehiclebroken_esp_color, true);
                            }
                        }
                    }

                    if (settings::vehicle::vehicle_distance) {

                        std::string str = std::to_string(Distance) + "m";
                        if (!settings::vehicle::vehicle_name) {

                            DrawOutlinedText(ImGui::GetFont( ), str, ImVec2(Vehicle_location.x, Vehicle_location.y + 0), 13.0f, settings::visuals::vehicle_esp_distance, true);

                        }

                        else if (settings::vehicle::vehicle_health) {

                            DrawOutlinedText(ImGui::GetFont( ), str, ImVec2(Vehicle_location.x, Vehicle_location.y + 10), 13.0f, settings::visuals::vehicle_esp_distance, true);

                        } else {

                            DrawOutlinedText(ImGui::GetFont( ), str, ImVec2(Vehicle_location.x, Vehicle_location.y + 10), 13.0f, settings::visuals::vehicle_esp_distance, true);
                        }
                    }

                    if (settings::vehicle::vehicle_marker) {

                        ImGui::GetBackgroundDrawList( )->AddCircle(ImVec2(Vehicle_location.x, Vehicle_location.y + 37), 2, ImColor(0, 0, 0, 255), 100, 3.0f);
                        ImGui::GetBackgroundDrawList( )->AddCircle(ImVec2(Vehicle_location.x, Vehicle_location.y + 37), 2, settings::visuals::vehicle_esp_marker, 100, 3.2f);
                    }
                    if (settings::vehicle::vehicle_snapline) {

                        ImGui::GetBackgroundDrawList( )->AddLine(ImVec2(FiveM::WindowSize.x / 2, FiveM::WindowSize.y), ImVec2(Vehicle_location.x, Vehicle_location.y), ImColor(255, 255, 255, 255), 0.5f);
                    }
                }
            }
        }
    }
}