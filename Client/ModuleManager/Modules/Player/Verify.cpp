#include "Verify.h"
#include "../../../Client.h"
#include <wininet.h>
#include <filesystem>
#include <fstream>
#include <ctime>
#pragma comment(lib, "wininet.lib")

Verify::Verify() : Module("Verify", "Verifies allowed players from OneDrive", Category::PLAYER) {
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

        // Inicializar WinINet con User-Agent personalizado
        HINTERNET hInternet = InternetOpen(USER_AGENT, 
                                         INTERNET_OPEN_TYPE_DIRECT, 
                                         NULL, 
                                         NULL, 
                                         0);
        if (!hInternet) {
            LogError("Failed to initialize WinINet");
            return false;
        }

        // Configurar headers adicionales
        LPCSTR headers = "Accept: text/plain\r\n"
                        "Connection: keep-alive\r\n";

        // Abrir URL con flags adicionales
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
            DWORD error = GetLastError();
            LogError("Failed to connect to OneDrive URL. Error: " + std::to_string(error));
            InternetCloseHandle(hInternet);
            return false;
        }

        // Verificar código de respuesta HTTP
        DWORD statusCode = 0;
        DWORD length = sizeof(statusCode);
        HttpQueryInfo(hConnect, 
                     HTTP_QUERY_STATUS_CODE | HTTP_QUERY_FLAG_NUMBER, 
                     &statusCode, 
                     &length, 
                     NULL);

        if (statusCode != 200) {
            LogError("HTTP error: " + std::to_string(statusCode));
            InternetCloseHandle(hConnect);
            InternetCloseHandle(hInternet);
            return false;
        }

        // Descargar a buffer en memoria
        std::vector<char> buffer;
        char tempBuffer[1024];
        DWORD bytesRead;

        while (InternetReadFile(hConnect, tempBuffer, sizeof(tempBuffer), &bytesRead) && bytesRead > 0) {
            buffer.insert(buffer.end(), tempBuffer, tempBuffer + bytesRead);
        }

        InternetCloseHandle(hConnect);
        InternetCloseHandle(hInternet);

        // Verificar contenido válido
        if (!isValidPlayerListContent(buffer)) {
            LogError("Invalid content received");
            return false;
        }

        // Escribir archivo
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

void Verify::onEnable() {
    try {
        LogError("Verify module enabling...");

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

        oneDriveUrl = "https://onedrive.live.com/download.aspx?authkey=%21ANI8FiEEca09Flc&resid=A2B2E8098926E94B%21128";
        fileName = "players.txt";

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
