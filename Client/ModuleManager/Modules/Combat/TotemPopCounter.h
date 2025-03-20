// TotemPopCounter.h
#pragma once
#include "../Module.h"
#include <map>

class TotemPopCounter : public Module {
private:
    std::map<std::string, int> popList;
    std::map<std::string, bool> totemEquipped;
    bool sendChat = true;
    bool clientOnly = true; // Nuevo booleano para mensajes solo al cliente

public:
    TotemPopCounter();
    ~TotemPopCounter();

    virtual void onEnable() override;
    virtual void onDisable() override;
    virtual void onNormalTick(Actor* actor) override;
};
