#pragma once

#include "../Module.h"


class TPAura : public Module {
public:
    TPAura();

    void onNormalTick(Actor* actor) override;

private:
    float range = 7.f;          // Hedefleme mesafesi
    float speed = 1.0f;         // Hareket hýzý
    int teleportPoints = 4;     // Rastgele teleport noktasý sayýsý
    bool targetMobs = true;     // Moblarý hedef al
    bool targetPlayers = true;  // Oyuncularý hedef al

    std::vector<Actor*> targetList;  // Hedef oyuncular ve moblar
};