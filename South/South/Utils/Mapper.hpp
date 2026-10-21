#pragma once
#include <Windows.h>
#include <vector>
#include <iostream>
#include <thread>
#include <chrono>
#include <sddl.h>
#include <string>

#include "../../Mapper/Mapper/include/intel_driver.hpp"
#include "../../Mapper/Mapper/include/kdmapper.hpp"
#include "../../Mapper/DriverBytes/Driver-Bytes.hpp"

// Funcao para verificar se o processo esta rodando como Administrador
inline bool IsUserAdmin() {
    BOOL isAdmin = FALSE;
    PSID adminGroup = nullptr;
    SID_IDENTIFIER_AUTHORITY ntAuthority = SECURITY_NT_AUTHORITY;
    if (AllocateAndInitializeSid(&ntAuthority, 2, SECURITY_BUILTIN_DOMAIN_RID, DOMAIN_ALIAS_RID_ADMINS, 0, 0, 0, 0, 0, 0, &adminGroup)) {
        CheckTokenMembership(nullptr, adminGroup, &isAdmin);
        FreeSid(adminGroup);
    }
    return isAdmin == TRUE;
}

// Funcao principal de mapeamento 
inline auto MapDriver(bool showMsgBox = false) -> int { 
    // Carrega o driver vulneravel da Intel 
    if (!intel_driver::Load()) { 
        if (showMsgBox) {
            MessageBoxA(nullptr, "Erro ao carregar driver Intel (Erro 8).\n\nExecute o Loader como Administrador.", "South Spoofer", MB_OK | MB_ICONERROR); 
        }
        return 1; 
    } 
    
    // Prepara os bytes do driver para o kdmapper 
    std::vector<uint8_t> driver_data(driver_bytes, driver_bytes + sizeof(driver_bytes)); 
    
    // Mapeia o driver usando kdmapper 
    if (!kdmapper::MapDriver(driver_data.data())) { 
        intel_driver::Unload(); // Descarrega o driver da Intel em caso de erro 
        if (showMsgBox) {
            MessageBoxA(nullptr, "Erro ao mapear driver (Erro 9).\n\nExecute o Loader como Administrador.", "South Spoofer", MB_OK | MB_ICONERROR); 
        }
        return 2; 
    } 
 
    // Finaliza e limpa o driver da Intel 
    intel_driver::Unload(); 
    return 0; 
} 
 
// Codigo adicional para pegar o SID (Identificador unico) do PC do cliente 
inline std::string get_client_sid() { 
    HANDLE tokenHandle = nullptr; 
    if (!OpenProcessToken(GetCurrentProcess(), TOKEN_QUERY, &tokenHandle)) { 
        return "Falha ao obter token"; 
    } 
 
    DWORD tokenInfoLength = 0; 
    GetTokenInformation(tokenHandle, TokenUser, nullptr, 0, &tokenInfoLength); 
 
    TOKEN_USER* tokenUser = (TOKEN_USER*)malloc(tokenInfoLength); 
    if (GetTokenInformation(tokenHandle, TokenUser, tokenUser, tokenInfoLength, &tokenInfoLength)) { 
        LPSTR sidString = nullptr; 
        ConvertSidToStringSidA(tokenUser->User.Sid, &sidString); 
 
        std::string sidResult(sidString); 
 
        LocalFree(sidString); 
        free(tokenUser); 
        CloseHandle(tokenHandle); 
 
        return sidResult; 
    } 
 
    free(tokenUser); 
    CloseHandle(tokenHandle); 
    return "SID nao encontrado"; 
}
