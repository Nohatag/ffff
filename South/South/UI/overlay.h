#pragma once
#include <Windows.h>
#include <d3d11.h>
#include <dwmapi.h>
#include <wincodec.h>
#pragma comment(lib, "windowscodecs.lib")
#include <string>
#include <vector>
#include <ctime>
#include <cstdlib>
#include <cstdio>
#include <chrono>
#include <thread>

#include "../../ImGui/imgui.h"
#include "../../ImGui/imgui_internal.h"
#include "../../ImGui/InterfaceScale.hpp"
#include "../../ImGui/imgui_impl_dx11.h"
#include "../../ImGui/imgui_impl_win32.h"
#include "../auth/settings.h"
#include "menu.h"
#include "../../ImGui/Fonts/Fonts.hpp"
#include "../../ImGui/Fonts/FontAwesome.hpp"
#include "../../ImGui/Fonts/FontAwesome6.hpp"
#include "../../ImGui/Fonts/ImageBytes.hpp"
#include "../../ImGui/Fonts/LoginHeroBytes.hpp"
#include "../../ImGui/Fonts/LoaderBrandBytes.hpp"
#include "../../ImGui/Fonts/MainMenuBrandBytes.hpp"
#include "../../ImGui/Fonts/ProggyClean.hpp"
#include "../Utils/xorstr.hpp"

extern LRESULT ImGui_ImplWin32_WndProcHandler(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

#pragma comment(lib, "d3d11.lib")
#pragma comment(lib, "dwmapi.lib")
#pragma comment(lib, "gdi32.lib")

// Globais de fontes exigidos pelo ImGui e Loader
inline ImFont* FontAwesomeRegular = nullptr;
inline ImFont* FontAwesomeSolid = nullptr;
inline ImFont* FontAwesomeSolid14 = nullptr;
inline ImFont* FontAwesomeBrands = nullptr;

inline ImFont* InterBlack = nullptr;
inline ImFont* InterBold = nullptr;
inline ImFont* InterBold12 = nullptr;
inline ImFont* InterExtraBold = nullptr;
inline ImFont* InterExtraLight = nullptr;
inline ImFont* InterLight = nullptr;
inline ImFont* InterMedium = nullptr;
inline ImFont* InterRegular = nullptr;
inline ImFont* InterSemiBold = nullptr;
inline ImFont* LoginInputFont = nullptr;
inline ImFont* MainMenuFont = nullptr;
inline ImFont* InterThin = nullptr;
inline ImFont* ProggyCleanEspFont = nullptr;

// Gerador de identificador aleatorio de 7 caracteres para a build
inline std::string GenerateRandomBuildString(int length = 7) {
    static const char charset[] = "0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz";
    std::string result;
    result.reserve(length);
    unsigned int seed = (unsigned int)GetTickCount() ^ (unsigned int)time(nullptr);
    for (int i = 0; i < length; ++i) {
        seed = seed * 1103515245 + 12345;
        result += charset[(seed >> 16) % (sizeof(charset) - 1)];
    }
    return result;
}

inline std::string g_BuildName = "";

class Overlay {
public:
    static Overlay& GetInstance() {
        static Overlay instance;
        return instance;
    }

    bool CreateLoaderWindow() {
        CoInitialize(NULL);
        if (g_BuildName.empty()) {
            g_BuildName = GenerateRandomBuildString(7);
        }

        std::string winClass = "South_Loader_" + g_BuildName;
        std::string winTitle = "South - " + g_BuildName;

        WNDCLASSEXA wc = { sizeof(WNDCLASSEXA), CS_CLASSDC, WndProc, 0L, 0L,
                           GetModuleHandle(NULL), NULL, NULL, NULL, NULL,
                           winClass.c_str(), NULL };
        RegisterClassExA(&wc);

        m_windowWidth = (int)InterfaceScale::Px(420.0f);
        m_windowHeight = (int)InterfaceScale::Px(260.0f);

        int screenW = GetSystemMetrics(SM_CXSCREEN);
        int screenH = GetSystemMetrics(SM_CYSCREEN);
        int posX = (screenW - m_windowWidth) / 2;
        int posY = (screenH - m_windowHeight) / 2;

        // Modo Normal: Janela desktop padrao independente, fora do jogo,
        // sem transparencia indesejada, sem auto-hide ao pressionar Win ou trocar foco.
        hwnd = CreateWindowExA(
            WS_EX_APPWINDOW,
            winClass.c_str(),
            winTitle.c_str(),
            WS_POPUP | WS_MINIMIZEBOX | WS_SYSMENU,
            posX, posY, m_windowWidth, m_windowHeight,
            NULL, NULL, wc.hInstance, NULL);

        if (!hwnd) {
            return false;
        }

        // Recorta fisicamente os 4 cantos da janela no formato arredondado
        // Eliminando de vez a caixa preta/cinza nos cantos externos
        const int radius = (int)InterfaceScale::Px(10.0f);
        HRGN hRgn = CreateRoundRectRgn(0, 0, m_windowWidth + 1, m_windowHeight + 1, radius * 2, radius * 2);
        if (hRgn) {
            SetWindowRgn(hwnd, hRgn, TRUE);
        }



        if (!InitD3D()) {
            return false;
        }

        ShowWindow(hwnd, SW_SHOW);
        UpdateWindow(hwnd);

        IMGUI_CHECKVERSION();
        ImGui::CreateContext();
        ImGuiIO& io = ImGui::GetIO();
        io.IniFilename = nullptr;
        io.LogFilename = nullptr;
        io.ConfigFlags &= ~ImGuiConfigFlags_NoMouseCursorChange;

        ImGui::StyleColorsDark();

        if (!ImGui_ImplWin32_Init(hwnd)) {
            return false;
        }
        if (!ImGui_ImplDX11_Init(device, deviceContext)) {
            return false;
        }

        io.Fonts->AddFontDefault();

        InterBlack      = io.Fonts->AddFontFromMemoryCompressedTTF(InterBlack_compressed_data,      InterBlack_compressed_size, InterfaceScale::Px(14));
        InterBold       = io.Fonts->AddFontFromMemoryCompressedTTF(InterBold_compressed_data,       InterBold_compressed_size, InterfaceScale::Px(16));
        InterBold12     = io.Fonts->AddFontFromMemoryCompressedTTF(InterBold_compressed_data,       InterBold_compressed_size, InterfaceScale::Px(12));
        InterExtraBold  = io.Fonts->AddFontFromMemoryCompressedTTF(InterExtraBold_compressed_data,  InterExtraBold_compressed_size, InterfaceScale::Px(14));
        InterExtraLight = io.Fonts->AddFontFromMemoryCompressedTTF(InterExtraLight_compressed_data, InterExtraLight_compressed_size, InterfaceScale::Px(14));
        InterLight      = io.Fonts->AddFontFromMemoryCompressedTTF(InterLight_compressed_data,      InterLight_compressed_size, InterfaceScale::Px(16));
        InterMedium     = io.Fonts->AddFontFromMemoryCompressedTTF(InterMedium_compressed_data,     InterMedium_compressed_size, InterfaceScale::Px(16));
        InterRegular    = io.Fonts->AddFontFromMemoryCompressedTTF(InterRegular_compressed_data,    InterRegular_compressed_size, InterfaceScale::Px(16));
        InterSemiBold   = io.Fonts->AddFontFromMemoryCompressedTTF(InterSemiBold_compressed_data,   InterSemiBold_compressed_size, InterfaceScale::Px(18));
        LoginInputFont  = io.Fonts->AddFontFromMemoryCompressedTTF(InterSemiBold_compressed_data,  InterSemiBold_compressed_size, InterfaceScale::Px(11.0f));
        MainMenuFont    = io.Fonts->AddFontFromMemoryCompressedTTF(InterRegular_compressed_data,   InterRegular_compressed_size, InterfaceScale::Px(11.0f));
        InterThin       = io.Fonts->AddFontFromMemoryCompressedTTF(InterThin_compressed_data,       InterThin_compressed_size, InterfaceScale::Px(14));

        ImFontConfig FontAwesomeConfig;
        static const ImWchar FontAwesomeRanges[] = { ICON_MIN_FA, ICON_MAX_FA, 0 };
        static const ImWchar FontAwesomeRangesBrands[] = { ICON_MIN_FAB, ICON_MAX_FAB, 0 };

        FontAwesomeRegular = io.Fonts->AddFontFromMemoryCompressedTTF(FontAwesome6Regular_compressed_data, FontAwesome6Regular_compressed_size, InterfaceScale::Px(17.f), &FontAwesomeConfig, FontAwesomeRanges);
        FontAwesomeSolid   = io.Fonts->AddFontFromMemoryCompressedTTF(FontAwesome6Solid_compressed_data,   FontAwesome6Solid_compressed_size, InterfaceScale::Px(19.f), &FontAwesomeConfig, FontAwesomeRanges);
        FontAwesomeSolid14 = io.Fonts->AddFontFromMemoryCompressedTTF(FontAwesome6Solid_compressed_data,   FontAwesome6Solid_compressed_size, InterfaceScale::Px(14.f), &FontAwesomeConfig, FontAwesomeRanges);
        FontAwesomeBrands  = io.Fonts->AddFontFromMemoryCompressedTTF(FontAwesome6Brands_compressed_data,  FontAwesome6Brands_compressed_size, InterfaceScale::Px(17.f), &FontAwesomeConfig, FontAwesomeRangesBrands);

        io.Fonts->Build();

        // Carrega as texturas embutidas usando o Windows Imaging Component nativo (zero dependência de DLLs externas)
        CreateTextureFromMemoryWIC(device, LoginHeroBytes, sizeof(LoginHeroBytes), &LoginHeroTexture);
        CreateTextureFromMemoryWIC(device, LoaderBrandBytes, sizeof(LoaderBrandBytes), &LoaderBrandTexture);

        return true;
    }

    static bool CreateTextureFromMemoryWIC(ID3D11Device* pDevice, const unsigned char* data, size_t size, ID3D11ShaderResourceView** out_srv) {
        if (!pDevice || !data || size == 0 || !out_srv) return false;
        *out_srv = nullptr;

        CoInitialize(NULL);

        IWICImagingFactory* factory = nullptr;
        HRESULT hr = CoCreateInstance(CLSID_WICImagingFactory, NULL, CLSCTX_INPROC_SERVER, IID_PPV_ARGS(&factory));
        if (FAILED(hr)) return false;

        IWICStream* stream = nullptr;
        hr = factory->CreateStream(&stream);
        if (FAILED(hr)) { factory->Release(); return false; }

        hr = stream->InitializeFromMemory(const_cast<BYTE*>(data), static_cast<DWORD>(size));
        if (FAILED(hr)) { stream->Release(); factory->Release(); return false; }

        IWICBitmapDecoder* decoder = nullptr;
        hr = factory->CreateDecoderFromStream(stream, NULL, WICDecodeMetadataCacheOnLoad, &decoder);
        if (FAILED(hr)) { stream->Release(); factory->Release(); return false; }

        IWICBitmapFrameDecode* frame = nullptr;
        hr = decoder->GetFrame(0, &frame);
        if (FAILED(hr)) { decoder->Release(); stream->Release(); factory->Release(); return false; }

        IWICFormatConverter* converter = nullptr;
        hr = factory->CreateFormatConverter(&converter);
        if (FAILED(hr)) { frame->Release(); decoder->Release(); stream->Release(); factory->Release(); return false; }

        hr = converter->Initialize(frame, GUID_WICPixelFormat32bppRGBA, WICBitmapDitherTypeNone, NULL, 0.0, WICBitmapPaletteTypeCustom);
        if (FAILED(hr)) { converter->Release(); frame->Release(); decoder->Release(); stream->Release(); factory->Release(); return false; }

        UINT width = 0, height = 0;
        converter->GetSize(&width, &height);

        std::vector<BYTE> pixels(width * height * 4);
        hr = converter->CopyPixels(NULL, width * 4, static_cast<UINT>(pixels.size()), pixels.data());

        converter->Release();
        frame->Release();
        decoder->Release();
        stream->Release();
        factory->Release();

        if (FAILED(hr)) return false;

        D3D11_TEXTURE2D_DESC desc = {};
        desc.Width = width;
        desc.Height = height;
        desc.MipLevels = 1;
        desc.ArraySize = 1;
        desc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
        desc.SampleDesc.Count = 1;
        desc.Usage = D3D11_USAGE_DEFAULT;
        desc.BindFlags = D3D11_BIND_SHADER_RESOURCE;

        D3D11_SUBRESOURCE_DATA initData = {};
        initData.pSysMem = pixels.data();
        initData.SysMemPitch = width * 4;

        ID3D11Texture2D* texture = nullptr;
        hr = pDevice->CreateTexture2D(&desc, &initData, &texture);
        if (FAILED(hr)) return false;

        D3D11_SHADER_RESOURCE_VIEW_DESC srvDesc = {};
        srvDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
        srvDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
        srvDesc.Texture2D.MipLevels = 1;

        hr = pDevice->CreateShaderResourceView(texture, &srvDesc, out_srv);
        texture->Release();

        return SUCCEEDED(hr);
    }

    void StartRenderLoop() {
        MSG msg;
        ZeroMemory(&msg, sizeof(msg));

        using clock = std::chrono::high_resolution_clock;
        const auto targetFrameTime = std::chrono::microseconds(1000000 / 60);

        while (msg.message != WM_QUIT) {
            while (PeekMessage(&msg, NULL, 0U, 0U, PM_REMOVE)) {
                TranslateMessage(&msg);
                DispatchMessage(&msg);
                if (msg.message == WM_QUIT)
                    return;
            }

            if (IsIconic(hwnd)) {
                Sleep(20);
                continue;
            }

            auto frameStart = clock::now();

            RenderFrame();

            auto frameEnd = clock::now();
            auto elapsed = std::chrono::duration_cast<std::chrono::microseconds>(frameEnd - frameStart);
            if (elapsed < targetFrameTime) {
                auto sleepMicros = targetFrameTime - elapsed;
                DWORD sleepMs = (DWORD)(sleepMicros.count() / 1000);
                if (sleepMs > 0) {
                    Sleep(sleepMs);
                } else {
                    std::this_thread::yield();
                }
            }
        }
    }

    HWND GetOverlayHwnd() const { return hwnd; }
    ID3D11ShaderResourceView* GetLoaderBrandTexture() { return LoaderBrandTexture; }
    ID3D11ShaderResourceView* GetLoginHeroTexture() { return LoginHeroTexture; }
    ID3D11Device* GetDevice() { return device; }
    ID3D11DeviceContext* GetDeviceContext() { return deviceContext; }

private:
    HWND hwnd = NULL;
    int m_windowWidth = 420;
    int m_windowHeight = 260;
    ID3D11Device* device = nullptr;
    ID3D11DeviceContext* deviceContext = nullptr;
    IDXGISwapChain* swapChain = nullptr;
    ID3D11RenderTargetView* renderTargetView = nullptr;

    ID3D11ShaderResourceView* LoginHeroTexture = nullptr;
    ID3D11ShaderResourceView* LoaderBrandTexture = nullptr;

    bool InitD3D() {
        DXGI_SWAP_CHAIN_DESC sd = {};
        sd.BufferCount = 2;
        sd.BufferDesc.Width = m_windowWidth;
        sd.BufferDesc.Height = m_windowHeight;
        sd.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
        sd.BufferDesc.RefreshRate.Numerator = 60;
        sd.BufferDesc.RefreshRate.Denominator = 1;
        sd.Flags = DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH;
        sd.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
        sd.OutputWindow = hwnd;
        sd.SampleDesc.Count = 1;
        sd.SampleDesc.Quality = 0;
        sd.Windowed = TRUE;
        sd.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;

        const D3D_FEATURE_LEVEL fl[2] = { D3D_FEATURE_LEVEL_11_0, D3D_FEATURE_LEVEL_10_0 };
        HRESULT hr = D3D11CreateDeviceAndSwapChain(
            nullptr, D3D_DRIVER_TYPE_HARDWARE, nullptr, 0,
            fl, 2, D3D11_SDK_VERSION, &sd,
            &swapChain, &device, nullptr, &deviceContext);
        if (FAILED(hr)) return false;

        CreateRTV();
        return true;
    }

    void CreateRTV() {
        if (!swapChain || !device) return;
        ID3D11Texture2D* back = nullptr;
        swapChain->GetBuffer(0, IID_PPV_ARGS(&back));
        if (back) {
            device->CreateRenderTargetView(back, nullptr, &renderTargetView);
            back->Release();
        }
    }

    void ReleaseRTV() {
        if (renderTargetView) {
            renderTargetView->Release();
            renderTargetView = nullptr;
        }
    }

    void RenderFrame() {
        if (!renderTargetView) return;

        D3D11_VIEWPORT vp;
        vp.TopLeftX = 0; vp.TopLeftY = 0;
        vp.Width = (float)m_windowWidth;
        vp.Height = (float)m_windowHeight;
        vp.MinDepth = 0.0f; vp.MaxDepth = 1.0f;
        deviceContext->RSSetViewports(1, &vp);

        const float clear_color[4] = { 0.0f, 0.0f, 0.0f, 0.0f };
        deviceContext->OMSetRenderTargets(1, &renderTargetView, nullptr);
        deviceContext->ClearRenderTargetView(renderTargetView, clear_color);

        ImGui_ImplDX11_NewFrame();
        ImGui_ImplWin32_NewFrame();
        ImGui::NewFrame();

        ImGuiIO& io = ImGui::GetIO();
        io.DisplaySize = ImVec2((float)m_windowWidth, (float)m_windowHeight);

        Menu::Render();

        ImGui::Render();
        ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());
        swapChain->Present(0, 0);
    }

    static LRESULT CALLBACK WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam) {
        if (ImGui_ImplWin32_WndProcHandler(hWnd, msg, wParam, lParam)) return true;
        switch (msg) {
        case WM_SYSCOMMAND:
            if ((wParam & 0xfff0) == SC_KEYMENU) return 0;
            break;
        case WM_DESTROY:
            PostQuitMessage(0);
            return 0;
        }
        return DefWindowProcA(hWnd, msg, wParam, lParam);
    }
};
