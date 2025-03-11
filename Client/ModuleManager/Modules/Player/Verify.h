#pragma once
#include "../Module.h"
#include <unordered_set>
#include <string>
#include <chrono>

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
    bool hasVerified = false;
    bool isConnected = false;
    std::chrono::steady_clock::time_point connectionTime;
    const int VERIFICATION_DELAY = 20; // 20 segundos de espera
    const wchar_t* USER_AGENT = L"Mozilla/5.0 (Windows NT 10.0; Win64; x64) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/120.0.0.0 Safari/537.36";
    void deleteFile(const std::string& filePath);
public:
    Verify();
    virtual void onEnable() override;
    virtual void onDisable() override;
    virtual void onNormalTick(Actor* actor) override;
    virtual void onSendPacket(Packet* packet, bool& shouldCancel) override;
    void performVerification();
};
