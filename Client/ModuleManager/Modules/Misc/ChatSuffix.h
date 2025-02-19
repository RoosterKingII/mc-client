#pragma once
#include "../Module.h"
#include <random>
#include <vector>

class ChatSuffix : public Module {
private:
    bool green = true;
    std::vector<std::string> suffixes;
    std::mt19937 rng;

public:
    ChatSuffix();
    virtual void onSendPacket(Packet* packet, bool& shouldCancel) override;
    std::string getRandomSuffix();
};
