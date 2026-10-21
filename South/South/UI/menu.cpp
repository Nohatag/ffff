#define IMGUI_DEFINE_MATH_OPERATORS
#include "menu.h"
#include "overlay.h"
#include "LoaderArtwork.hpp"
#include "LoaderNotification.hpp"
#include "../auth/settings.h"
#include "../auth/auth.h"
#include "../Utils/HardwareCleaner.hpp"
#include "../Utils/Mapper.hpp"
#include "../Utils/xorstr.hpp"
#include "../../ImGui/imgui_internal.h"
#include "../../ImGui/Fonts/FontAwesome.hpp"
#include "../../ImGui/Fonts/FontAwesome6.hpp"

#include <string>
#include <cmath>
#include <cstring>
#include <vector>

using InterfaceScale::Px;

namespace {
    static void DrawCenteredText(ImDrawList* dl, const ImVec2& center, const char* text, ImU32 color,
        ImFont* font = nullptr, float fontSize = 0.0f) {
        if (!dl || !text) return;
        fontSize = Px(fontSize);
        ImVec2 size = font ? font->CalcTextSizeA(fontSize, FLT_MAX, 0.0f, text)
                           : ImGui::CalcTextSize(text);
        ImVec2 pos(center.x - size.x * 0.5f, center.y - size.y * 0.5f);
        if (font && fontSize > 0.0f)
            dl->AddText(font, fontSize, pos, color, text);
        else
            dl->AddText(pos, color, text);
    }

    static void DrawFontText(ImDrawList* dl, ImFont* font, float fontSize, const ImVec2& pos, ImU32 color, const char* text) {
        if (!dl || !text) return;
        fontSize = Px(fontSize);
        if (font && fontSize > 0.0f)
            dl->AddText(font, fontSize, pos, color, text);
        else
            dl->AddText(pos, color, text);
    }

    static void DrawLogo(ImDrawList* dl, const ImVec2& center, const ImVec2& size, int alpha) {
        ID3D11ShaderResourceView* logo = Overlay::GetInstance().GetLoaderBrandTexture();
        if (logo) {
            const ImVec2 half = size * 0.5f;
            LoaderArtwork::Brand(dl, logo, center - half, center + half, alpha);
        } else {
            DrawCenteredText(dl, center, xs("South"), IM_COL32(255, 255, 255, alpha), InterSemiBold, 22.0f);
        }
    }

    static void DrawWindowBackground(ImDrawList* dl, const ImVec2& pos, const ImVec2& size, float cornerRadius, int alpha) {
        // 1. Fundo Preto Fosco Moderno (Sleek Dark Theme)
        dl->AddRectFilled(pos, pos + size, IM_COL32(12, 12, 16, (int)(alpha * 0.98f)), cornerRadius);
    }

    static void DrawWindowBorderAndCorners(ImDrawList* dl, const ImVec2& pos, const ImVec2& size, float cornerRadius, int alpha, float alphaFloat) {
        // 2. Borda externa suave limpa (Clean modern dark border)
        dl->AddRect(pos + ImVec2(0.5f, 0.5f), pos + size - ImVec2(0.5f, 0.5f), IM_COL32(46, 48, 58, (int)(alphaFloat * 175.0f)), cornerRadius, 0, 1.0f);

        // 3. Highlight sutil interno
        dl->AddRect(pos + ImVec2(1.5f, 1.5f), pos + size - ImVec2(1.5f, 1.5f), IM_COL32(255, 255, 255, (int)(alphaFloat * 12.0f)), cornerRadius - 1.0f, 0, 1.0f);
    }

    static void DrawWindowControls(ImDrawList* dl, const ImVec2& pos, const ImVec2& size, int panelAlpha) {
        const float btnW = Px(18.0f);
        const float btnH = Px(18.0f);
        const float topY = pos.y + Px(6.0f);
        const float closeX = pos.x + size.x - Px(26.0f);
        const float minX = closeX - Px(22.0f);

        // Botao Minimizar
        ImGui::SetCursorScreenPos(ImVec2(minX, topY));
        ImGui::InvisibleButton(xs("##win_minimize"), ImVec2(btnW, btnH));
        const bool minHover = ImGui::IsItemHovered();
        if (ImGui::IsItemClicked()) {
            ShowWindow(Overlay::GetInstance().GetOverlayHwnd(), SW_MINIMIZE);
        }
        dl->AddRectFilled(ImVec2(minX, topY), ImVec2(minX + btnW, topY + btnH),
            minHover ? IM_COL32(50, 52, 62, panelAlpha) : IM_COL32(22, 24, 30, (int)(panelAlpha * 0.75f)), Px(3.0f));
        dl->AddLine(ImVec2(minX + Px(4.0f), topY + Px(9.0f)), ImVec2(minX + btnW - Px(4.0f), topY + Px(9.0f)),
            IM_COL32(215, 218, 226, panelAlpha), Px(1.6f));

        // Botao Fechar
        ImGui::SetCursorScreenPos(ImVec2(closeX, topY));
        ImGui::InvisibleButton(xs("##win_close"), ImVec2(btnW, btnH));
        const bool closeHover = ImGui::IsItemHovered();
        if (ImGui::IsItemClicked()) {
            PostQuitMessage(0);
        }
        dl->AddRectFilled(ImVec2(closeX, topY), ImVec2(closeX + btnW, topY + btnH),
            closeHover ? IM_COL32(225, 45, 55, panelAlpha) : IM_COL32(22, 24, 30, (int)(panelAlpha * 0.75f)), Px(3.0f));
        dl->AddLine(ImVec2(closeX + Px(5.0f), topY + Px(5.0f)), ImVec2(closeX + btnW - Px(5.0f), topY + btnH - Px(5.0f)),
            IM_COL32(235, 238, 245, panelAlpha), Px(1.6f));
        dl->AddLine(ImVec2(closeX + btnW - Px(5.0f), topY + Px(5.0f)), ImVec2(closeX + Px(5.0f), topY + btnH - Px(5.0f)),
            IM_COL32(235, 238, 245, panelAlpha), Px(1.6f));

        // Permite arrastar a janela nativa pela barra superior (exceto sobre os botoes)
        ImGuiIO& io = ImGui::GetIO();
        ImVec2 mouse = io.MousePos;
        if (ImGui::IsMouseClicked(ImGuiMouseButton_Left) &&
            mouse.y >= pos.y && mouse.y <= pos.y + Px(28.0f) &&
            mouse.x >= pos.x && mouse.x < minX - Px(4.0f)) {
            HWND hWnd = Overlay::GetInstance().GetOverlayHwnd();
            if (hWnd) {
                ReleaseCapture();
                SendMessageA(hWnd, WM_NCLBUTTONDOWN, HTCAPTION, 0);
            }
        }
    }

    static int DrawInjectDashboard(ImDrawList* dl, const ImVec2& wndPos, const ImVec2& wndSize, float alphaFloat, bool busy = false) {
        const int alpha = (int)(ImClamp(alphaFloat, 0.0f, 1.0f) * 255.0f + 0.5f);
        auto p = [&](float x, float y) { return wndPos + ImVec2(Px(x), Px(y)); };

        const float cornerRadius = Px(10.0f);

        const float sidebarW = 56.0f;
        dl->AddRectFilled(wndPos, wndPos + ImVec2(Px(sidebarW), wndSize.y), IM_COL32(6, 6, 9, (int)(alpha * 0.58f)), cornerRadius, ImDrawFlags_RoundCornersLeft);
        dl->AddLine(wndPos + ImVec2(Px(sidebarW), 0), wndPos + ImVec2(Px(sidebarW), wndSize.y), IM_COL32(40, 42, 52, (int)(alpha * 0.40f)), 1.0f);
        DrawLogo(dl, p(28.0f, 27.0f), ImVec2(Px(28.0f), Px(31.0f)), alpha);

        auto sidebarIcon = [&](float y, const char* icon, bool active, const char* id) -> bool {
            const ImVec2 iconPos = p(28.0f, y);
            const ImVec2 btnMin = iconPos - ImVec2(Px(16.0f), Px(16.0f));
            const ImVec2 btnSize(Px(32.0f), Px(32.0f));
            ImGui::SetCursorScreenPos(btnMin);
            ImGui::InvisibleButton(id, btnSize);
            const bool clicked = ImGui::IsItemClicked();
            const bool hovered = ImGui::IsItemHovered();

            if (active || hovered)
                dl->AddRectFilled(btnMin, btnMin + btnSize, IM_COL32(16, 16, 18, alpha), Px(2.0f));
            DrawCenteredText(dl, iconPos, icon,
                active ? IM_COL32(235, 55, 65, alpha) : (hovered ? IM_COL32(200, 200, 205, alpha) : IM_COL32(112, 112, 117, alpha)),
                FontAwesomeSolid14, 12.0f);
            return clicked;
        };

        static int s_ActiveSidebarTab = 0; // 0 = Products, 1 = Config, 2 = Redeem
        static bool s_ShowRedeemModal = false;
        static char s_RedeemKeyBuf[64] = "";

        if (sidebarIcon(70.0f, ICON_FA_FOLDER, s_ActiveSidebarTab == 0 && !s_ShowRedeemModal, xs("##sb_folder"))) {
            s_ActiveSidebarTab = 0;
            s_ShowRedeemModal = false;
        }
        if (sidebarIcon(113.0f, ICON_FA_GEAR, s_ActiveSidebarTab == 1 && !s_ShowRedeemModal, xs("##sb_gear"))) {
            s_ActiveSidebarTab = 1;
            s_ShowRedeemModal = false;
        }
        if (sidebarIcon(155.0f, ICON_FA_GIFT, s_ActiveSidebarTab == 2 || s_ShowRedeemModal, xs("##sb_gift"))) {
            s_ActiveSidebarTab = 2;
            s_ShowRedeemModal = true;
        }
        if (sidebarIcon(228.0f, ICON_FA_RIGHT_FROM_BRACKET, false, xs("##sb_logout"))) {
            g_Settings.i_LoginStage = 0;
            g_Settings.b_IsLoggedIn = false;
            s_ShowRedeemModal = false;
            s_ActiveSidebarTab = 0;
            LoaderNotification::Show("Notice", "Logged out.", 2.0f);
        }

        // Top bar branding
        DrawFontText(dl, InterSemiBold, 12.0f, p(64.0f, 8.0f), IM_COL32(235, 55, 65, alpha), xs("South"));
        const char* breadcrumb = (s_ActiveSidebarTab == 1 || s_ActiveSidebarTab == 2 || s_ShowRedeemModal) ? xs("> Config") : xs("> Products");
        DrawFontText(dl, InterSemiBold, 12.0f, p(103.0f, 8.0f), IM_COL32(215, 218, 225, alpha), breadcrumb);

        int selectedProduct = -1;

        // TAB 0: Products (only shown when on Tab 0 and Redeem modal is not open)
        if (s_ActiveSidebarTab == 0 && !s_ShowRedeemModal) {
            const char* productNames[] = { "South Menu", "Hardware Cleaner", "South Spoofer" };

            for (int i = 0; i < 3; ++i) {
                const float y = 32.0f + i * 65.0f;
                dl->AddRectFilled(p(64.0f, y), p(408.0f, y + 59.0f), IM_COL32(16, 16, 22, (int)(alpha * 0.42f)), Px(4.0f));
                dl->AddRect(p(64.0f, y), p(408.0f, y + 59.0f), IM_COL32(42, 45, 56, (int)(alpha * 0.40f)), Px(4.0f));

                dl->AddRectFilled(p(72.0f, y + 5.0f), p(118.0f, y + 51.0f), IM_COL32(22, 22, 28, (int)(alpha * 0.52f)), Px(3.0f));
                LoaderArtwork::ProductMark(dl, p(81.0f, y + 11.0f), p(109.0f, y + 46.0f), alpha);

                DrawFontText(dl, InterSemiBold, 11.0f, p(126.0f, y + 4.0f), IM_COL32(232, 234, 240, alpha), productNames[i]);
                DrawFontText(dl, InterMedium, 10.5f, p(126.0f, y + 19.0f), IM_COL32(89, 89, 89, alpha), xs("Expiration:"));
                DrawFontText(dl, InterSemiBold, 10.5f, p(181.0f, y + 19.0f), IM_COL32(235, 55, 65, alpha), xs("Lifetime"));
                DrawFontText(dl, InterMedium, 10.5f, p(126.0f, y + 30.0f), IM_COL32(89, 89, 89, alpha), xs("Status:"));
                DrawFontText(dl, InterSemiBold, 10.5f, p(164.0f, y + 30.0f), IM_COL32(235, 55, 65, alpha), xs("Working"));

                const ImVec2 btnMin = p(360.0f, y + 12.0f);
                const ImVec2 btnSize(Px(38.0f), Px(38.0f));
                ImGui::SetCursorScreenPos(btnMin);
                ImGui::PushID(i);
                ImGui::BeginDisabled(busy);
                ImGui::InvisibleButton(xs("##product_download"), btnSize);
                const bool clicked = ImGui::IsItemClicked();
                const bool hovered = ImGui::IsItemHovered();
                const bool held = ImGui::IsItemActive();
                dl->AddRectFilled(btnMin, btnMin + btnSize,
                    held ? IM_COL32(180, 25, 35, alpha) : hovered ? IM_COL32(255, 75, 85, alpha) : IM_COL32(230, 45, 55, alpha),
                    Px(3.0f));
                dl->AddRect(btnMin, btnMin + btnSize, IM_COL32(255, 255, 255, (int)(alpha * (hovered ? 0.16f : 0.08f))), Px(3.0f));
                DrawCenteredText(dl, btnMin + btnSize * 0.5f, ICON_FA_DOWNLOAD,
                    IM_COL32(240, 246, 255, alpha), FontAwesomeSolid14, 12.0f);
                ImGui::EndDisabled();
                ImGui::PopID();

                if (clicked)
                    selectedProduct = i;
            }

            // Bottom footer info
            DrawFontText(dl, InterMedium, 10.0f, p(64.0f, 238.0f), IM_COL32(140, 142, 148, alpha), (std::string("Build: ") + g_BuildName).c_str());
        }
        // TAB 1: Config / Settings
        else if (s_ActiveSidebarTab == 1 && !s_ShowRedeemModal) {
            const ImVec2 cfgMin = p(64.0f, 32.0f);
            const ImVec2 cfgMax = p(408.0f, 226.0f);
            dl->AddRectFilled(cfgMin, cfgMax, IM_COL32(16, 16, 22, (int)(alpha * 0.52f)), Px(4.0f));
            dl->AddRect(cfgMin, cfgMax, IM_COL32(42, 45, 56, (int)(alpha * 0.45f)), Px(4.0f));

            DrawFontText(dl, InterSemiBold, 12.0f, cfgMin + ImVec2(Px(14.0f), Px(14.0f)), IM_COL32(240, 242, 248, alpha), xs("Loader Configuration"));

            auto drawConfigRow = [&](float yOff, const char* label, const char* val, bool isRed = false) {
                const ImVec2 rowPos = cfgMin + ImVec2(Px(14.0f), Px(yOff));
                DrawFontText(dl, InterMedium, 11.0f, rowPos, IM_COL32(150, 153, 162, alpha), label);
                DrawFontText(dl, InterSemiBold, 11.0f, cfgMin + ImVec2(Px(220.0f), Px(yOff)),
                    isRed ? IM_COL32(235, 55, 65, alpha) : IM_COL32(220, 224, 232, alpha), val);
                dl->AddLine(cfgMin + ImVec2(Px(14.0f), Px(yOff + 20.0f)), cfgMin + ImVec2(Px(330.0f), Px(yOff + 20.0f)),
                    IM_COL32(35, 38, 48, (int)(alpha * 0.40f)), 1.0f);
            };

            drawConfigRow(40.0f, xs("Build Version"), g_BuildName.c_str(), true);
            drawConfigRow(68.0f, xs("Hardware Cleaner"), xs("Ready"), false);
            drawConfigRow(96.0f, xs("Spoofer Driver"), xs("Loaded"), true);
            drawConfigRow(124.0f, xs("Security Status"), xs("Undetected"), false);
            drawConfigRow(152.0f, xs("License Status"), xs("Lifetime Active"), true);
        }
        // TAB 2: Redeem Key Box (1:1 with reference image, no products behind)
        else if (s_ActiveSidebarTab == 2 || s_ShowRedeemModal) {
            const float modalW = Px(230.0f);
            const float modalH = Px(116.0f);
            const ImVec2 modalPos = wndPos + ImVec2(Px(sidebarW) + (wndSize.x - Px(sidebarW) - modalW) * 0.5f,
                                                   (wndSize.y - modalH) * 0.5f - Px(4.0f));

            dl->AddRectFilled(modalPos, modalPos + ImVec2(modalW, modalH),
                IM_COL32(18, 18, 22, (int)(alpha * 0.96f)), Px(6.0f));
            dl->AddRect(modalPos, modalPos + ImVec2(modalW, modalH),
                IM_COL32(40, 42, 52, (int)(alpha * 0.85f)), Px(6.0f), 0, 1.0f);

            DrawFontText(dl, InterSemiBold, 12.5f, modalPos + ImVec2(Px(14.0f), Px(11.0f)),
                IM_COL32(245, 246, 250, alpha), xs("Redeem Key"));

            const ImVec2 inputPos = modalPos + ImVec2(Px(14.0f), Px(33.0f));
            const ImVec2 inputSize(modalW - Px(28.0f), Px(27.0f));
            ImGui::SetCursorScreenPos(inputPos);
            ImGui::PushFont(LoginInputFont ? LoginInputFont : ImGui::GetFont());
            ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2(Px(8.0f), (inputSize.y - ImGui::GetFontSize()) * 0.5f));
            ImGui::PushStyleVar(ImGuiStyleVar_FrameRounding, Px(3.0f));
            ImGui::PushStyleVar(ImGuiStyleVar_FrameBorderSize, 1.0f);
            ImGui::PushStyleColor(ImGuiCol_FrameBg, IM_COL32(11, 11, 14, (int)(alpha * 0.95f)));
            ImGui::PushStyleColor(ImGuiCol_FrameBgHovered, IM_COL32(15, 15, 19, (int)(alpha * 0.98f)));
            ImGui::PushStyleColor(ImGuiCol_FrameBgActive, IM_COL32(18, 18, 23, 255));
            ImGui::PushStyleColor(ImGuiCol_Text, IM_COL32(230, 232, 240, 255));
            ImGui::PushStyleColor(ImGuiCol_TextDisabled, IM_COL32(120, 120, 125, 255));
            ImGui::PushStyleColor(ImGuiCol_TextSelectedBg, IM_COL32(235, 50, 60, 100));
            ImGui::PushStyleColor(ImGuiCol_Border, IM_COL32(35, 38, 48, (int)(alpha * 0.85f)));
            ImGui::PushStyleColor(ImGuiCol_NavHighlight, ImVec4(0, 0, 0, 0));

            ImGui::InputTextEx(xs("##redeem_key_input"), xs("Enter license key..."), s_RedeemKeyBuf, sizeof(s_RedeemKeyBuf), inputSize,
                ImGuiInputTextFlags_AutoSelectAll | ImGuiInputTextFlags_UseStyle);

            ImGui::PopStyleColor(8);
            ImGui::PopStyleVar(3);
            ImGui::PopFont();

            const float btnY = modalPos.y + Px(69.0f);
            const float btnW = (modalW - Px(28.0f) - Px(8.0f)) * 0.5f;
            const float btnH = Px(28.0f);

            // Botao Redeem (Vermelho)
            const ImVec2 redeemBtnMin(modalPos.x + Px(14.0f), btnY);
            const ImVec2 redeemBtnSize(btnW, btnH);
            ImGui::SetCursorScreenPos(redeemBtnMin);
            ImGui::InvisibleButton(xs("##btn_redeem_confirm"), redeemBtnSize);
            const bool redeemClicked = ImGui::IsItemClicked();
            const bool redeemHover = ImGui::IsItemHovered();
            const bool redeemHeld = ImGui::IsItemActive();

            dl->AddRectFilled(redeemBtnMin, redeemBtnMin + redeemBtnSize,
                redeemHeld ? IM_COL32(180, 25, 35, alpha) : redeemHover ? IM_COL32(255, 70, 80, alpha) : IM_COL32(230, 42, 50, alpha),
                Px(4.0f));
            DrawCenteredText(dl, redeemBtnMin + redeemBtnSize * 0.5f, xs("Redeem"),
                IM_COL32(245, 248, 255, alpha), InterSemiBold, 11.5f);

            // Botao Cancel (Vermelho)
            const ImVec2 cancelBtnMin(modalPos.x + Px(14.0f) + btnW + Px(8.0f), btnY);
            const ImVec2 cancelBtnSize(btnW, btnH);
            ImGui::SetCursorScreenPos(cancelBtnMin);
            ImGui::InvisibleButton(xs("##btn_redeem_cancel"), cancelBtnSize);
            const bool cancelClicked = ImGui::IsItemClicked();
            const bool cancelHover = ImGui::IsItemHovered();
            const bool cancelHeld = ImGui::IsItemActive();

            dl->AddRectFilled(cancelBtnMin, cancelBtnMin + cancelBtnSize,
                cancelHeld ? IM_COL32(180, 25, 35, alpha) : cancelHover ? IM_COL32(255, 70, 80, alpha) : IM_COL32(230, 42, 50, alpha),
                Px(4.0f));
            DrawCenteredText(dl, cancelBtnMin + cancelBtnSize * 0.5f, xs("Cancel"),
                IM_COL32(245, 248, 255, alpha), InterSemiBold, 11.5f);

            if (redeemClicked) {
                if (std::strlen(s_RedeemKeyBuf) > 0) {
                    AuthSystem::SaveKey(s_RedeemKeyBuf);
                    LoaderNotification::Show("Success", "Key redeemed successfully!", 3.0f);
                    s_ShowRedeemModal = false;
                    s_ActiveSidebarTab = 0;
                    s_RedeemKeyBuf[0] = '\0';
                } else {
                    LoaderNotification::Show("Error", "Please enter a key.", 2.5f);
                }
            }

            if (cancelClicked) {
                s_ShowRedeemModal = false;
                s_ActiveSidebarTab = 0;
            }
        }

        return selectedProduct;
    }

    static bool RenderLoginFlow() {
        ImGuiIO& io = ImGui::GetIO();
        const float dt = io.DeltaTime;
        static double s_StageReadyAt = 0.0;
        static float s_FormAlpha = 0.0f;
        static float s_DashboardAlpha = 0.0f;
        static float s_ErrorAlpha = 0.0f;

        const ImVec2 loginPos(0.0f, 0.0f);
        const ImVec2 loginSize = io.DisplaySize;

        ImGui::SetNextWindowPos(loginPos, ImGuiCond_Always);
        ImGui::SetNextWindowSize(loginSize, ImGuiCond_Always);

        ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0, 0));
        ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, Px(10.0f));
        ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.0f);
        ImGui::PushStyleColor(ImGuiCol_WindowBg, ImVec4(0, 0, 0, 0));

        ImGui::Begin(xs("##south_loader_main"), nullptr,
            ImGuiWindowFlags_NoTitleBar |
            ImGuiWindowFlags_NoResize |
            ImGuiWindowFlags_NoScrollbar |
            ImGuiWindowFlags_NoScrollWithMouse |
            ImGuiWindowFlags_NoSavedSettings);

        ImDrawList* dl = ImGui::GetWindowDrawList();
        const float cornerRadius = Px(10.0f);
        const ImVec2 pos(0.0f, 0.0f);
        const ImVec2 size = loginSize;
        const int panelAlpha = (int)(ImGui::GetStyle().Alpha * 255.0f);
        const int stage = g_Settings.i_LoginStage;

        // 1. Fundo da janela (desenhado antes de toda a arte)
        DrawWindowBackground(dl, pos, size, cornerRadius, panelAlpha);

        if (stage <= 1) {
            s_DashboardAlpha = 0.0f;
            s_FormAlpha = ImLerp(s_FormAlpha, 1.0f, dt * 8.0f);
            const int alpha = (int)(s_FormAlpha * panelAlpha);

            static bool s_RememberMe = true;
            static char s_AccessKey[64] = "";

            ID3D11ShaderResourceView* hero = Overlay::GetInstance().GetLoginHeroTexture();
            if (hero) {
                LoaderArtwork::Login(dl, hero, pos, size, alpha);
            }

            // Branding principal
            const float southX = Px(28.0f);
            DrawFontText(dl, InterSemiBold, 26.0f, pos + ImVec2(southX, Px(14.0f)),
                IM_COL32(235, 50, 60, alpha), xs("South"));
            const float southW = InterSemiBold ? InterSemiBold->CalcTextSizeA(Px(26.0f), FLT_MAX, -1.0f, "South").x : Px(67.0f);
            DrawFontText(dl, InterSemiBold, 26.0f, pos + ImVec2(southX + southW + Px(5.0f), Px(14.0f)),
                IM_COL32(235, 236, 240, alpha), xs("Menu"));

            auto drawTab = [&](const char* label, const char* id, const ImVec2& tabPos, bool active) {
                const ImVec2 tabMin = pos + tabPos;
                const ImVec2 tabMax = tabMin + ImVec2(Px(62.0f), Px(25.0f));
                ImGui::SetCursorScreenPos(tabMin);
                ImGui::InvisibleButton(id, ImVec2(Px(62.0f), Px(25.0f)));
                const bool hovered = ImGui::IsItemHovered();
                dl->AddRectFilled(tabMin, tabMax,
                    hovered ? IM_COL32(22, 24, 30, (int)(alpha * 0.60f)) : IM_COL32(14, 15, 20, (int)(alpha * 0.48f)),
                    Px(2.0f));
                DrawCenteredText(dl, tabMin + ImVec2(Px(31.0f), Px(12.0f)), label,
                    IM_COL32(235, 237, 243, alpha), InterSemiBold, 11.0f);
                if (active)
                    dl->AddRectFilled(tabMin + ImVec2(Px(16.0f), Px(23.0f)), tabMin + ImVec2(Px(46.0f), Px(25.0f)),
                        IM_COL32(235, 50, 60, alpha), 1.0f);
                return ImGui::IsItemClicked();
            };

            drawTab(xs("Login"), xs("##login_tab_login"), ImVec2(Px(73.0f), Px(54.0f)), true);
            g_Settings.b_IsRegistering = false;

            auto inputField = [&](const char* id, const char* icon, const char* hint, char* buffer, size_t bufferSize,
                const ImVec2& fieldPos, bool password) {
                const ImVec2 fieldMin = pos + fieldPos;
                const ImVec2 fieldSize(Px(174.0f), Px(30.0f));
                ImGui::SetCursorScreenPos(fieldMin);
                ImGui::PushFont(LoginInputFont ? LoginInputFont : ImGui::GetFont());
                ImGui::PushStyleVar(ImGuiStyleVar_Alpha, s_FormAlpha * ImGui::GetStyle().Alpha);
                ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2(Px(21.0f), (fieldSize.y - ImGui::GetFontSize()) * 0.5f));
                ImGui::PushStyleVar(ImGuiStyleVar_FrameRounding, Px(3.0f));
                ImGui::PushStyleVar(ImGuiStyleVar_FrameBorderSize, 1.0f);
                ImGui::PushStyleColor(ImGuiCol_FrameBg, IM_COL32(16, 17, 24, (int)(alpha * 0.48f)));
                ImGui::PushStyleColor(ImGuiCol_FrameBgHovered, IM_COL32(22, 24, 32, (int)(alpha * 0.62f)));
                ImGui::PushStyleColor(ImGuiCol_FrameBgActive, IM_COL32(26, 28, 38, (int)(alpha * 0.72f)));
                ImGui::PushStyleColor(ImGuiCol_Text, IM_COL32(205, 205, 205, 255));
                ImGui::PushStyleColor(ImGuiCol_TextDisabled, IM_COL32(145, 145, 145, 255));
                ImGui::PushStyleColor(ImGuiCol_TextSelectedBg, IM_COL32(235, 50, 60, 100));
                ImGui::PushStyleColor(ImGuiCol_Border, IM_COL32(48, 50, 62, (int)(alpha * 0.42f)));
                ImGui::PushStyleColor(ImGuiCol_NavHighlight, ImVec4(0.0f, 0.0f, 0.0f, 0.0f));
                ImGuiInputTextFlags flags = ImGuiInputTextFlags_EnterReturnsTrue | ImGuiInputTextFlags_AutoSelectAll | ImGuiInputTextFlags_UseStyle;
                if (password)
                    flags |= ImGuiInputTextFlags_Password;
                const bool enter = ImGui::InputTextEx(id, hint, buffer, (int)bufferSize, fieldSize, flags);
                ImGui::PopStyleColor(8);
                ImGui::PopStyleVar(4);
                ImGui::PopFont();
                DrawCenteredText(dl, fieldMin + ImVec2(Px(11.0f), Px(15.0f)), icon,
                    IM_COL32(145, 145, 145, alpha), FontAwesomeSolid14, 11.0f);
                return enter;
            };

            bool enter = false;
            enter |= inputField(xs("##login_username"), ICON_FA_USER, xs("Username"),
                g_Settings.sz_Username, sizeof(g_Settings.sz_Username), ImVec2(Px(17.0f), Px(94.0f)), false);
            enter |= inputField(xs("##login_password"), ICON_FA_LOCK, xs("Password"),
                g_Settings.sz_Password, sizeof(g_Settings.sz_Password), ImVec2(Px(17.0f), Px(129.0f)), true);

            const ImVec2 rememberPos = pos + ImVec2(Px(17.0f), Px(164.0f));
            ImGui::SetCursorScreenPos(rememberPos);
            ImGui::InvisibleButton(xs("##remember_me"), ImVec2(Px(122.0f), Px(16.0f)));
            if (ImGui::IsItemClicked())
                s_RememberMe = !s_RememberMe;

            dl->AddRectFilled(rememberPos, rememberPos + ImVec2(Px(14.0f), Px(14.0f)),
                s_RememberMe ? IM_COL32(235, 50, 60, alpha) : IM_COL32(15, 16, 20, alpha), Px(2.0f));
            dl->AddRect(rememberPos, rememberPos + ImVec2(Px(14.0f), Px(14.0f)),
                IM_COL32(54, 61, 73, alpha), Px(2.0f));
            if (s_RememberMe) {
                dl->AddLine(rememberPos + ImVec2(Px(3.5f), Px(7.0f)), rememberPos + ImVec2(Px(6.0f), Px(9.5f)),
                    IM_COL32(245, 248, 255, alpha), Px(1.6f));
                dl->AddLine(rememberPos + ImVec2(Px(6.0f), Px(9.5f)), rememberPos + ImVec2(Px(10.5f), Px(4.0f)),
                    IM_COL32(245, 248, 255, alpha), Px(1.6f));
            }
            DrawFontText(dl, InterSemiBold, 11.0f, rememberPos + ImVec2(Px(21.0f), 1.0f),
                IM_COL32(230, 232, 238, alpha), xs("Remember Me"));

            const float buttonY = Px(209.0f);
            const ImVec2 loginBtnMin = pos + ImVec2(Px(17.0f), buttonY);
            const ImVec2 loginBtnSize(Px(174.0f), Px(31.0f));
            ImGui::SetCursorScreenPos(loginBtnMin);
            ImGui::InvisibleButton(xs("##login_access_button"), loginBtnSize);
            const bool access = ImGui::IsItemClicked();
            const bool loginBtnHovered = ImGui::IsItemHovered();
            const bool loginBtnHeld = ImGui::IsItemActive();
            dl->AddRectFilled(loginBtnMin, loginBtnMin + loginBtnSize,
                loginBtnHeld ? IM_COL32(180, 25, 35, alpha) : loginBtnHovered ? IM_COL32(255, 75, 85, alpha) : IM_COL32(230, 45, 55, alpha),
                Px(3.0f));
            DrawCenteredText(dl, loginBtnMin + loginBtnSize * 0.5f,
                xs("Login"),
                IM_COL32(242, 247, 255, alpha), InterSemiBold, 12.0f);

            // Identificador de build no rodape
            DrawFontText(dl, InterMedium, 10.0f, pos + ImVec2(Px(17.0f), Px(243.0f)),
                IM_COL32(120, 125, 135, alpha), (std::string("Build: ") + g_BuildName).c_str());

            if (access || enter) {
                if (std::strlen(g_Settings.sz_Username) > 0) {
                    std::string key = g_Settings.sz_Username;
                    std::string msg;
                    bool ok = AuthSystem::Authenticate(key, msg);
                    if (ok) {
                        AuthSystem::SaveKey(key);
                        g_Settings.i_LoginStage = 2;
                        s_StageReadyAt = ImGui::GetTime() + 1.0;
                        LoaderNotification::Show("Success", "Welcome to South", 1.5f);
                        s_FormAlpha = 0.0f;
                        s_ErrorAlpha = 0.0f;
                    }
                } else {
                    LoaderNotification::Show("Error", "Please enter your username.", 2.5f);
                }
            }
        } else {
            // Dashboard de produtos
            s_FormAlpha = 0.0f;
            s_DashboardAlpha = ImLerp(s_DashboardAlpha, 1.0f, ImMin(1.0f, dt * 7.5f));
            const bool busy = stage == 2 || stage == 4;
            const int selectedProduct = DrawInjectDashboard(dl, pos, size, s_DashboardAlpha, busy);

            if (selectedProduct == 0) {
                g_Settings.i_LoginStage = 4;
                s_StageReadyAt = ImGui::GetTime() + 3.0;
                LoaderNotification::Show("Loading", "Loading South Menu...", 3.0f);
            } else if (selectedProduct == 1) {
                // Hardware Cleaner
                HardwareCleaner::ExecuteAsync([](bool success, int filesRemoved) {
                    if (success) {
                        std::string msg = "Cleaner finished! " + std::to_string(filesRemoved) + " files purged.";
                        LoaderNotification::Show("Success", msg.c_str(), 4.0f);
                    } else {
                        LoaderNotification::Show("Error", "Hardware Cleaner encountered an error.", 3.0f);
                    }
                });
            } else if (selectedProduct == 2) {
                // South Spoofer (Own Driver Mapper)
                std::thread([]() {
                    LoaderNotification::Show("Spoofer", "Mapping South Spoofer driver...", 3.0f);
                    if (!IsUserAdmin()) {
                        LoaderNotification::Show("Error", "Run as Administrator to map driver.", 4.0f);
                        return;
                    }
                    int res = MapDriver(false);
                    if (res == 0) {
                        LoaderNotification::Show("Success", "South Spoofer driver active!", 4.0f);
                    } else {
                        LoaderNotification::Show("Error", "Failed to map spoofer driver.", 4.0f);
                    }
                }).detach();
            }

            if (busy && ImGui::GetTime() >= s_StageReadyAt) {
                if (stage == 2) {
                    g_Settings.i_LoginStage = 3;
                } else if (stage == 4) {
                    g_Settings.i_LoginStage = 3;
                    LoaderNotification::Show("Success", "South Menu loaded!", 4.0f);
                }
            }
        }

        // 2. Desenha a borda da janela e os 4 cantos vermelhos POR CIMA de toda a arte
        DrawWindowBorderAndCorners(dl, pos, size, cornerRadius, panelAlpha, (stage <= 1 ? s_FormAlpha : s_DashboardAlpha));

        // 3. Desenha os botoes de Minimizar e Fechar na barra superior POR CIMA de tudo
        DrawWindowControls(dl, pos, size, panelAlpha);

        LoaderNotification::Draw(pos, size);
        ImGui::End();
        ImGui::PopStyleColor();
        ImGui::PopStyleVar(3);

        return true;
    }
}

namespace Menu {
    bool IsAnyWindowOpen() {
        return true;
    }

    void Render() {
        static bool s_StyleApplied = false;
        if (!s_StyleApplied) {
            s_StyleApplied = true;
            ImGuiStyle* Style = &ImGui::GetStyle();
            Style->WindowRounding   = 5.0f;
            Style->WindowBorderSize = 0.0f;
            Style->WindowPadding    = ImVec2(0, 0);
            Style->WindowShadowSize = 0;
            Style->ScrollbarSize    = 4.0f;

            Style->Colors[ImGuiCol_WindowBg]        = ImColor(10, 10, 14, 120);
            Style->Colors[ImGuiCol_Border]          = ImColor(48, 50, 60, 140);
            Style->Colors[ImGuiCol_Text]            = ImColor(1.f, 1.f, 1.f, 0.85f);
            Style->Colors[ImGuiCol_TextDisabled]    = ImColor(92, 92, 98, 190);
            Style->Colors[ImGuiCol_TextSelectedBg]  = ImColor(230, 45, 55, 100);
            Style->Colors[ImGuiCol_FrameBg]         = ImColor(16, 17, 22, 130);
            Style->Colors[ImGuiCol_FrameBgHovered]  = ImColor(22, 24, 30, 160);
            Style->Colors[ImGuiCol_FrameBgActive]   = ImColor(26, 28, 36, 180);
            Style->Colors[ImGuiCol_CheckMark]       = ImColor(230, 45, 55, 255);
        }

        RenderLoginFlow();
    }
}
