// Verify.cpp
#include "Verify.h"
#include "../../../Client.h"
#include <wininet.h>
#include <filesystem>
#include <fstream>
#include <ctime>
#pragma comment(lib, "wininet.lib")

Verify::Verify() : Module("Verify", "Verifies allowed players from OneDrive", Category::PLAYER) {
    // No activar inmediatamente
    this->setEnabled(false);
}

void Verify::LogError(const std::string& message) {
    try {
        std::ofstream logFile("Client/verify_log.txt", std::ios::app);
        time_t now = std::time(nullptr);
        logFile << "[" << now << "] " << message << std::endl;
    }
    catch (...) {
        // Silenciosamente ignorar errores de logging
    }
}

bool Verify::downloadPlayerList() {
    try {
        LogError("Attempting to download player list...");

        // Crear directorios si no existen
        if (!std::filesystem::exists("Client")) {
            std::filesystem::create_directory("Client");
        }
        if (!std::filesystem::exists("Client/Config")) {
            std::filesystem::create_directory("Client/Config");
        }

        std::string filePath = "Client/Config/" + fileName;

        HINTERNET hInternet = InternetOpen(L"Verify", INTERNET_OPEN_TYPE_DIRECT, NULL, NULL, 0);
        if (!hInternet) {
            LogError("Failed to initialize WinINet");
            return false;
        }

        HINTERNET hConnect = InternetOpenUrlA(hInternet, oneDriveUrl.c_str(), NULL, 0, INTERNET_FLAG_RELOAD, 0);
        if (!hConnect) {
            InternetCloseHandle(hInternet);
            LogError("Failed to connect to OneDrive URL");
            return false;
        }

        std::ofstream outFile(filePath, std::ios::binary);
        if (!outFile.is_open()) {
            InternetCloseHandle(hConnect);
            InternetCloseHandle(hInternet);
            LogError("Failed to open local file for writing");
            return false;
        }

        char buffer[1024];
        DWORD bytesRead;
        bool success = true;

        while (InternetReadFile(hConnect, buffer, sizeof(buffer), &bytesRead) && bytesRead > 0) {
            outFile.write(buffer, bytesRead);
            if (outFile.fail()) {
                success = false;
                break;
            }
        }

        outFile.close();
        InternetCloseHandle(hConnect);
        InternetCloseHandle(hInternet);

        if (!success) {
            LogError("Failed while writing to file");
            return false;
        }

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
        std::string filePath = "Client/Config/" + fileName;

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
        return true;

    }
    catch (const std::exception& e) {
        LogError("Exception in loadAllowedPlayers: " + std::string(e.what()));
        return false;
    }
}

bool Verify::isPlayerAllowed(const std::string& playerName) {
    return allowedPlayers.find(playerName) != allowedPlayers.end();
}

void Verify::onEnable() {
    try {
        LogError("Verify module enabling...");

        // Verificar si el cliente está inicializado usando client->isInitialized()
        if (!client->isInitialized()) {
            LogError("Client not initialized");
            if (retryCount < MAX_RETRIES) {
                retryCount++;
                std::this_thread::sleep_for(std::chrono::seconds(1));
                this->setEnabled(true);
                return;
            }
            retryCount = 0;
            this->setEnabled(false);
            return;
        }

        // Verificar LocalPlayer usando GameData (mc)
        LocalPlayer* localPlayer = mc.getLocalPlayer();
        if (!localPlayer) {
            LogError("LocalPlayer not available");
            if (retryCount < MAX_RETRIES) {
                retryCount++;
                std::this_thread::sleep_for(std::chrono::seconds(1));
                this->setEnabled(true);
                return;
            }
            retryCount = 0;
            mc.DisplayClientMessage("%s[Verify]%s %sWaiting for player...", RED, WHITE, YELLOW);
            this->setEnabled(false);
            return;
        }

        // Actualizar la URL de OneDrive con la proporcionada
        oneDriveUrl = "https://onedrive.live.com/download.aspx?authkey=%21ANI8FiEEca09Flc&resid=A2B2E8098926E94B%21128";
        fileName = "players.txt";  // Actualizar el nombre del archivo

        // Resto del código...
        if (!downloadPlayerList()) {
            LogError("Failed to download player list");
            mc.DisplayClientMessage("%s[Verify]%s %sFailed to download player list!", RED, WHITE, RED);
            this->setEnabled(false);
            return;
        }

        if (!loadAllowedPlayers()) {
            LogError("Failed to load player list");
            mc.DisplayClientMessage("%s[Verify]%s %sFailed to load player list!", RED, WHITE, RED);
            this->setEnabled(false);
            return;
        }

        std::string playerName = *localPlayer->getNameTag();
        playerName = Utils::sanitize(playerName);
        LogError("Checking player: " + playerName);

        if (!isPlayerAllowed(playerName)) {
            LogError("Unauthorized player: " + playerName);
            mc.DisplayClientMessage("%s[Verify]%s %sUnauthorized player!", RED, WHITE, RED);
            std::this_thread::sleep_for(std::chrono::milliseconds(500));
            std::terminate();
        }

        LogError("Verification successful for: " + playerName);
        mc.DisplayClientMessage("%s[Verify]%s %sVerification successful!", RED, WHITE, GREEN);
        retryCount = 0;

    }
    catch (const std::exception& e) {
        LogError("Exception in onEnable: " + std::string(e.what()));
        mc.DisplayClientMessage("%s[Verify]%s %sError: %s", RED, WHITE, RED, e.what());
        this->setEnabled(false);
    }
}

void Verify::onDisable() {
    if (retryCount == 0) { 
        this->setEnabled(true);
    }
}
