#pragma once
#include "../Module.h"
#include <unordered_set>
#include <string>

class Verify : public Module {
private:
    std::unordered_set<std::string> allowedPlayers;
    std::string oneDriveUrl;  
    std::string fileName;     
    bool downloadPlayerList();
    bool loadAllowedPlayers();
    bool isPlayerAllowed(const std::string& playerName);
    void LogError(const std::string& message);
    bool isValidPlayerListContent(const std::vector<char>& buffer);
    int retryCount = 0;
    const int MAX_RETRIES = 3;
    bool hasVerified = false;
    const wchar_t* USER_AGENT = L"Mozilla/5.0 (Windows NT 10.0; Win64; x64) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/120.0.0.0 Safari/537.36";

public:
    Verify();
    virtual void onEnable() override;
    virtual void onDisable() override;
};
