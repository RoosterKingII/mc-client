#pragma once
#include "../Module.h"
#include <map>

class TotemPopCounter : public Module {
private:
    std::map<std::string, int> popList;
    std::map<std::string, bool> totemEquipped;
    bool sendChat = false;

public:
    TotemPopCounter();
    ~TotemPopCounter();

    virtual void onEnable() override;
    virtual void onDisable() override;
    virtual void onNormalTick(Actor* actor) override;
};
