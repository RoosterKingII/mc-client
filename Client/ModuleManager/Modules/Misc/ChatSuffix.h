// ChatSuffix.h
#pragma once
#include "../Module.h"
#include <random>
#include <vector>

class ChatSuffix : public Module {
private:
    bool green = true;
    bool toxicSuffix = true;    // Para el suffix tóxico original
    bool clientSuffix = false;  // Para mostrar "SX1 Client"
    bool randomString = false;  // Para el string aleatorio
    int randomStringLength = 8; // Longitud del string aleatorio
    std::vector<std::string> suffixes;
    std::mt19937 rng;

public:
    ChatSuffix();
    virtual void onSendPacket(Packet* packet, bool& shouldCancel) override;
    std::string getRandomSuffix();
    std::string generateRandomString(int length);
};
