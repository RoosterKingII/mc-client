#pragma once
#include "../Module.h"
#include <map>
#include <random>

class TotemPopCounter : public Module {
private:
    bool sendChat = false;
    bool clientOnly = true;
    bool randomString = false;
    int stringLength = 8;
    std::map<std::string, int> popList;
    std::map<std::string, bool> totemEquipped;
    std::mt19937 rng;

    std::string generateRandomString(int length);

public:
    TotemPopCounter();
    virtual ~TotemPopCounter();
    virtual void onNormalTick(Actor*) override;
    virtual void onEnable() override;
    virtual void onDisable() override;
};
