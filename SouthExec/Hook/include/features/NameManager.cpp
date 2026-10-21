// Leaked By Cry Service & Siraj
#include <windows.h>
#include <winhttp.h>
#include "NameManager.h"
#include <fstream>
#include <regex>
#include <iostream>
#include <shlobj.h>

#pragma comment(lib, "winhttp.lib")

namespace NameManager {

    Manager& Manager::Get() {
        static Manager instance;
        return instance;
    }

    void Manager::Start() {
        running = true;
        std::thread([this]() { this->UpdateLoop(); }).detach();
    }

    void Manager::Stop() {
        running = false;
    }

    std::string Manager::GetName(int serverId) {
        std::lock_guard<std::mutex> lock(mapMutex);
        auto it = networkMap.find(serverId);
        if (it != networkMap.end()) {
            return it->second.UserName;
        }
        return "";
    }

    std::string Manager::ExtractIp(const std::string& line) {

        size_t lastUrlPos = line.find("last_server_url");
        if (lastUrlPos == std::string::npos) {

            size_t simple = line.find("last_server");
            if (simple == std::string::npos) return "";
            lastUrlPos = simple;
        }

        size_t start = line.find("last_server", lastUrlPos);
        if (start == std::string::npos) return "";

        size_t ipStart = line.find("\"", start + 11);
        if (ipStart == std::string::npos) ipStart = line.find("=", start + 11);
        if (ipStart == std::string::npos) return "";
        ipStart += 1;

        size_t ipEnd = line.find("\"", ipStart);
        if (ipEnd == std::string::npos) ipEnd = line.find(":", ipStart); 

        size_t totalEnd = line.find("\"", ipStart);
        if (totalEnd == std::string::npos) return "";

        return line.substr(ipStart, totalEnd - ipStart);
    }

    std::string Manager::GetServerToken() {
        if (fivemPath.empty()) {
            char path[MAX_PATH];
            DWORD size = sizeof(path);
            if (RegGetValueA(HKEY_CURRENT_USER, "Software\\CitizenFX\\FiveM", "Last Run Location", RRF_RT_REG_SZ, NULL, path, &size) == ERROR_SUCCESS) {
                fivemPath = path;
            }
        }

        if (fivemPath.empty()) return "";

        std::string crashometryPath = fivemPath + "\\data\\cache\\crashometry";
        std::ifstream file(crashometryPath, std::ios::binary);
        if (!file.is_open()) return "";

        std::string line;
        std::string serverIp;
        while (std::getline(file, line)) {
            if (line.find("last_server") != std::string::npos) {
                serverIp = ExtractIp(line);
                break;
            }
        }
        file.close();

        if (serverIp.empty()) return "";

        size_t pos = serverIp.find("fivem://connect/");
        if (pos != std::string::npos) {
            serverIp = serverIp.substr(pos + 16);
        }
        pos = serverIp.find("http://");
        if (pos != std::string::npos) {
            serverIp = serverIp.substr(pos + 7);
        }
        pos = serverIp.find("https://");
        if (pos != std::string::npos) {
            serverIp = serverIp.substr(pos + 8);
        }
        while (!serverIp.empty() && (serverIp.back() == '/' || serverIp.back() == '\\')) {
            serverIp.pop_back();
        }

        std::vector<std::string> candidateIps;
        candidateIps.push_back(serverIp);
        if (serverIp.find(':') == std::string::npos) {
            candidateIps.push_back(serverIp + ":30120");
            candidateIps.push_back(serverIp + ":33881");
        } else {
            size_t colonPos = serverIp.find_last_of(':');
            std::string ipOnly = serverIp.substr(0, colonPos);
            candidateIps.push_back(ipOnly + ":33881");
            candidateIps.push_back(ipOnly + ":30120");
        }

        std::string token = "";
        HINTERNET hSession = WinHttpOpen(L"CitizenFX/1", WINHTTP_ACCESS_TYPE_DEFAULT_PROXY, WINHTTP_NO_PROXY_NAME, WINHTTP_NO_PROXY_BYPASS, 0);
        if (!hSession) return "";

        for (const auto& ipCandidate : candidateIps) {
            std::string url = "http://" + ipCandidate + "/";
            URL_COMPONENTS urlComp = { sizeof(urlComp) };
            urlComp.dwHostNameLength = (DWORD)-1;
            urlComp.dwUrlPathLength = (DWORD)-1;

            wchar_t wUrl[1024];
            MultiByteToWideChar(CP_UTF8, 0, url.c_str(), -1, wUrl, 1024);

            if (WinHttpCrackUrl(wUrl, 0, 0, &urlComp)) {
                wchar_t host[256] = {0};
                DWORD toCopy = (urlComp.dwHostNameLength < 255) ? urlComp.dwHostNameLength : 255;
                wcsncpy_s(host, urlComp.lpszHostName, toCopy);

                HINTERNET hConnect = WinHttpConnect(hSession, host, urlComp.nPort, 0);
                if (hConnect) {
                    HINTERNET hRequest = WinHttpOpenRequest(hConnect, L"GET", urlComp.lpszUrlPath, NULL, WINHTTP_NO_REFERER, WINHTTP_DEFAULT_ACCEPT_TYPES, 0);
                    if (hRequest) {
                        if (WinHttpSendRequest(hRequest, WINHTTP_NO_ADDITIONAL_HEADERS, 0, WINHTTP_NO_REQUEST_DATA, 0, 0, 0)) {
                            if (WinHttpReceiveResponse(hRequest, NULL)) {
                                wchar_t redirectedUrl[1024] = {0};
                                DWORD redirectedUrlLen = sizeof(redirectedUrl);
                                if (WinHttpQueryOption(hRequest, WINHTTP_OPTION_URL, redirectedUrl, &redirectedUrlLen)) {
                                    std::wstring wRedirected(redirectedUrl);
                                    size_t lastSlash = wRedirected.find_last_of(L'/');
                                    if (lastSlash != std::wstring::npos && lastSlash < wRedirected.length() - 1) {
                                        std::wstring wToken = wRedirected.substr(lastSlash + 1);
                                        size_t qMark = wToken.find(L'?');
                                        if (qMark != std::wstring::npos) wToken = wToken.substr(0, qMark);
                                        token = std::string(wToken.begin(), wToken.end());
                                    }
                                }
                            }
                        }
                        WinHttpCloseHandle(hRequest);
                    }
                    WinHttpCloseHandle(hConnect);
                }
            }
            if (!token.empty()) break;
        }
        WinHttpCloseHandle(hSession);

        return token;
    }

    std::string Manager::HttpGet(const std::string& urlString) {
        std::string response;
        HINTERNET hSession = WinHttpOpen(L"CitizenFX/1", WINHTTP_ACCESS_TYPE_DEFAULT_PROXY, WINHTTP_NO_PROXY_NAME, WINHTTP_NO_PROXY_BYPASS, 0);
        if (!hSession) return "";

        wchar_t wUrl[1024];
        MultiByteToWideChar(CP_UTF8, 0, urlString.c_str(), -1, wUrl, 1024);

        URL_COMPONENTS urlComp = { sizeof(urlComp) };
        urlComp.dwHostNameLength = (DWORD)-1;
        urlComp.dwUrlPathLength = (DWORD)-1;
        urlComp.dwExtraInfoLength = (DWORD)-1;

        if (WinHttpCrackUrl(wUrl, 0, 0, &urlComp)) {
            wchar_t host[256] = {0};
            DWORD toCopy = (urlComp.dwHostNameLength < 255) ? urlComp.dwHostNameLength : 255;
            wcsncpy_s(host, urlComp.lpszHostName, toCopy);

            HINTERNET hConnect = WinHttpConnect(hSession, host, INTERNET_DEFAULT_HTTPS_PORT, 0);
            if (hConnect) {
                HINTERNET hRequest = WinHttpOpenRequest(hConnect, L"GET", urlComp.lpszUrlPath, NULL, WINHTTP_NO_REFERER, WINHTTP_DEFAULT_ACCEPT_TYPES, WINHTTP_FLAG_SECURE);
                if (hRequest) {
                    if (WinHttpSendRequest(hRequest, WINHTTP_NO_ADDITIONAL_HEADERS, 0, WINHTTP_NO_REQUEST_DATA, 0, 0, 0)) {
                        if (WinHttpReceiveResponse(hRequest, NULL)) {
                            DWORD dwSize = 0;
                            do {
                                if (!WinHttpQueryDataAvailable(hRequest, &dwSize)) break;
                                if (dwSize == 0) break;
                                char* pszOutBuffer = new char[dwSize + 1];
                                ZeroMemory(pszOutBuffer, dwSize + 1);
                                DWORD dwDownloaded = 0;
                                if (WinHttpReadData(hRequest, (LPVOID)pszOutBuffer, dwSize, &dwDownloaded)) {
                                    response.append(pszOutBuffer, dwDownloaded);
                                }
                                delete[] pszOutBuffer;
                            } while (dwSize > 0);
                        }
                    }
                    WinHttpCloseHandle(hRequest);
                }
                WinHttpCloseHandle(hConnect);
            }
        }
        WinHttpCloseHandle(hSession);
        return response;
    }

    void Manager::UpdateLoop() {
        while (running) {
            std::string token = GetServerToken();
            if (!token.empty()) {
                std::string apiUrl = "https://servers-frontend.fivem.net/api/servers/single/" + token;
                std::string response = HttpGet(apiUrl);
                if (!response.empty()) {
                    try {
                        auto j = nlohmann::json::parse(response);
                        if (j.contains("Data") && j["Data"].contains("players")) {
                            std::map<int, NetworkInfo> tempMap;
                            auto& playersData = j["Data"]["players"];

                            nlohmann::json playerList;
                            if (playersData.is_array()) {
                                playerList = playersData;
                            } else if (playersData.is_object() && playersData.contains("list")) {
                                playerList = playersData["list"];
                            }

                            if (playerList.is_array()) {
                                for (auto& player : playerList) {
                                    if (player.contains("id") && player.contains("name")) {
                                        int id = -1;
                                        if (player["id"].is_number()) id = player["id"].get<int>();
                                        else if (player["id"].is_string()) id = std::stoi(player["id"].get<std::string>());

                                        std::string name = player["name"].get<std::string>();
                                        if (id != -1) tempMap[id] = { name, "", "" };
                                    }
                                }
                            }

                            std::lock_guard<std::mutex> lock(mapMutex);
                            networkMap = std::move(tempMap);
                        }
                    } catch (...) {}
                }
            }

            for (int i = 0; i < 100 && running; i++) {
                std::this_thread::sleep_for(std::chrono::milliseconds(100));
            }
        }
    }
}
