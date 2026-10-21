#define Debug 0
#include <Windows.h>

#if Debug == 0
#pragma comment(linker, "/SUBSYSTEM:windows /ENTRY:mainCRTStartup")
#endif

#include <string>
#include "settings.h"
#include "../UI/overlay.h"
#include "../UI/menu.h"
#include "../UI/LoaderNotification.hpp"

Settings g_Settings;

int main() {
    // Configura o estado inicial do Loader
    g_Settings.b_IsLoggedIn = false;
    g_Settings.b_MenuVisible = true;
    g_Settings.i_LoginStage = 0;

    // Inicializa e cria a janela desktop nativa do Loader (fora do jogo, modo normal, sem overlay)
    if (!Overlay::GetInstance().CreateLoaderWindow()) {
        MessageBoxA(NULL, "Failed to initialize South Loader.", "Error", MB_OK | MB_ICONERROR);
        return 1;
    }

    LoaderNotification::Show("South", "Welcome to South", 3.5f);

    // Loop de eventos e renderizacao da interface
    Overlay::GetInstance().StartRenderLoop();

    return 0;
}
