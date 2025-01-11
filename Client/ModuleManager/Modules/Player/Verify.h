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
    void forceGameCrash();
    int retryCount = 0;
    const int MAX_RETRIES = 3;
    bool hasVerified = false;

public:
    Verify();
    virtual void onEnable() override;
    virtual void onDisable() override;
};
