// Leaked By Cry Service & Siraj
#pragma once
#include <map>
#include <string>
#include <mutex>
#include <thread>
#include <vector>
#include "../../include/json.hpp"

namespace NameManager {
    struct NetworkInfo {
        std::string UserName;
        std::string DiscordId;
        std::string SteamId;
    };

    class Manager {
    public:
        static Manager& Get();

        void Start();
        void Stop();
        std::string GetName(int serverId);
        void UpdateLoop();

    private:
        Manager() = default;
        std::string GetServerToken();
        std::string ExtractIp(const std::string& line);
        std::string HttpGet(const std::string& url);

        bool running = true;
        std::map<int, NetworkInfo> networkMap;
        std::mutex mapMutex;
        std::string lastToken;
        std::string fivemPath;
    };
}
