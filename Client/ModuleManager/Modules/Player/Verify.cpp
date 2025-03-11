#include "Verify.h"
#include "../../../Client.h"
#include <wininet.h>
#include <filesystem>
#include <fstream>
#include <ctime>
#pragma comment(lib, "wininet.lib")

Verify::Verify() : Module("Verify", "Verifies allowed players from OneDrive", Category::CLIENT) {
    this->setEnabled(true);
}

void Verify::LogError(const std::string& message) {
    try {
        std::string logPath = Utils::getRoamingStatePath() + "\\verify_log.txt";
        std::ofstream logFile(logPath, std::ios::app);
        time_t now = std::time(nullptr);
        logFile << "[" << now << "] " << message << std::endl;
    }
    catch (...) {
        // Silently ignore logging errors
    }
}

bool Verify::isValidPlayerListContent(const std::vector<char>& buffer) {
    std::string content(buffer.begin(), buffer.end());
    if (content.find("<!DOCTYPE") != std::string::npos ||
        content.find("<html") != std::string::npos) {
        return false;
    }
    return content.find_first_not_of(" \t\n\r") != std::string::npos;
}

bool Verify::downloadPlayerList() {
    try {
        LogError("Attempting to download player list...");

        std::string basePath = Utils::getRoamingStatePath();
        std::string configPath = basePath + "\\Config";
        std::string filePath = configPath + "\\" + fileName;

        std::filesystem::create_directories(configPath);

        HINTERNET hInternet = InternetOpen(USER_AGENT,
            INTERNET_OPEN_TYPE_DIRECT,
            NULL,
            NULL,
            0);
        if (!hInternet) {
            LogError("Failed to initialize WinINet");
            return false;
        }

        LPCSTR headers = "Accept: text/plain\r\n"
            "Connection: keep-alive\r\n";

        HINTERNET hConnect = InternetOpenUrlA(hInternet,
            oneDriveUrl.c_str(),
            headers,
            strlen(headers),
            INTERNET_FLAG_RELOAD |
            INTERNET_FLAG_NO_CACHE_WRITE |
            INTERNET_FLAG_NO_UI |
            INTERNET_FLAG_SECURE |
            INTERNET_FLAG_IGNORE_CERT_CN_INVALID,
            0);

        if (!hConnect) {
            LogError("Failed to connect to OneDrive URL");
            InternetCloseHandle(hInternet);
            return false;
        }

        std::vector<char> buffer;
        char tempBuffer[1024];
        DWORD bytesRead;

        while (InternetReadFile(hConnect, tempBuffer, sizeof(tempBuffer), &bytesRead) && bytesRead > 0) {
            buffer.insert(buffer.end(), tempBuffer, tempBuffer + bytesRead);
        }

        InternetCloseHandle(hConnect);
        InternetCloseHandle(hInternet);

        if (!isValidPlayerListContent(buffer)) {
            LogError("Invalid content received");
            return false;
        }

        std::ofstream outFile(filePath, std::ios::binary);
        if (!outFile.is_open()) {
            LogError("Failed to open local file for writing");
            return false;
        }

        outFile.write(buffer.data(), buffer.size());
        outFile.close();

        LogError("Successfully downloaded player list");
        return true;
    }
    catch (const std::exception& e) {
        LogError("Exception in downloadPlayerList: " + std::string(e.what()));
        return false;
    }
}

bool Verify::loadAllowedPlayers() {
    try {
        LogError("Loading allowed players...");
        std::string filePath = Utils::getRoamingStatePath() + "\\Config\\" + fileName;

        std::ifstream inFile(filePath);
        if (!inFile.is_open()) {
            LogError("Failed to open player list file");
            return false;
        }

        allowedPlayers.clear();
        std::string line;
        while (std::getline(inFile, line)) {
            if (!line.empty()) {
                allowedPlayers.insert(Utils::sanitize(line));
            }
        }

        inFile.close();
        LogError("Successfully loaded allowed players");
        return !allowedPlayers.empty();
    }
    catch (const std::exception& e) {
        LogError("Exception in loadAllowedPlayers: " + std::string(e.what()));
        return false;
    }
}

bool Verify::isPlayerAllowed(const std::string& playerName) {
    return allowedPlayers.find(playerName) != allowedPlayers.end();
}

void Verify::onSendPacket(Packet* packet, bool& shouldCancel) {
    if (!hasVerified && packet->getId() == PacketID::Login) {
        isConnected = true;
        connectionTime = std::chrono::steady_clock::now();
        LogError("Connection detected, starting verification delay");
    }
}

void Verify::onNormalTick(Actor* actor) {
    if (!hasVerified && isConnected) {
        auto currentTime = std::chrono::steady_clock::now();
        auto elapsedSeconds = std::chrono::duration_cast<std::chrono::seconds>(
            currentTime - connectionTime).count();

        if (elapsedSeconds >= VERIFICATION_DELAY) {
            performVerification();
        }
    }
}

void Verify::deleteFile(const std::string& filePath) {
    try {
        if (std::filesystem::exists(filePath)) {
            std::filesystem::remove(filePath);
        }
    }
    catch (...) {
        // Silently ignore deletion errors
    }
}

void Verify::onEnable() {
    if (!hasVerified && !isConnected) {
        std::string logPath = Utils::getRoamingStatePath() + "\\verify_log.txt";
        deleteFile(logPath);
        LogError("Module enabled, waiting for connection...");
    }
}

void Verify::performVerification() {
    try {
        LogError("Starting verification process...");

        LocalPlayer* localPlayer = mc.getLocalPlayer();
        if (!localPlayer) {
            LogError("LocalPlayer not available");
            return;
        }

        oneDriveUrl = "https://onedrive.live.com/download.aspx?authkey=%21ANI8FiEEca09Flc&resid=A2B2E8098926E94B%21128";
        fileName = "players.txt";

        if (!downloadPlayerList()) {
            LogError("Failed to download player list");
            mc.DisplayClientMessage("%s[Verify]%s %sFailed to download player list!", RED, WHITE, RED);
            std::this_thread::sleep_for(std::chrono::milliseconds(500));
            std::terminate();
            return;
        }

        if (!loadAllowedPlayers()) {
            LogError("Failed to load player list");
            mc.DisplayClientMessage("%s[Verify]%s %sFailed to load player list!", RED, WHITE, RED);
            std::this_thread::sleep_for(std::chrono::milliseconds(500));
            std::terminate();
            return;
        }

        std::string playerName = Utils::sanitize(*localPlayer->getNameTag());
        LogError("Checking player: " + playerName);

        if (!isPlayerAllowed(playerName)) {
            LogError("Unauthorized player: " + playerName);
            mc.DisplayClientMessage("%s[Verify]%s %sUnauthorized player!", RED, WHITE, RED);
            std::this_thread::sleep_for(std::chrono::milliseconds(500));
            std::terminate();
        }

        LogError("Verification successful for: " + playerName);
        mc.DisplayClientMessage("%s[Verify]%s %sVerification successful!", RED, WHITE, GREEN);
        hasVerified = true;

        std::string playersFilePath = Utils::getRoamingStatePath() + "\\Config\\" + fileName;
        deleteFile(playersFilePath);
    }
    catch (const std::exception& e) {
        LogError("Exception in performVerification: " + std::string(e.what()));
        mc.DisplayClientMessage("%s[Verify]%s %sError: %s", RED, WHITE, RED, e.what());
        std::this_thread::sleep_for(std::chrono::milliseconds(500));
        std::terminate();
    }
}

void Verify::onDisable() {
    if (!hasVerified) {
        this->setEnabled(true);
    }
}
