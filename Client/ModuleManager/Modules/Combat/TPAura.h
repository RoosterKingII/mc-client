#pragma once

#include "../Module.h"


class TPAura : public Module {
public:
    TPAura();

    void onNormalTick(Actor* actor) override;

private:
    float range = 7.f;          // Hedefleme mesafesi
    float speed = 1.0f;         // Hareket h�z�
    int teleportPoints = 4;     // Rastgele teleport noktas� say�s�
    bool targetMobs = true;     // Moblar� hedef al
    bool targetPlayers = true;  // Oyuncular� hedef al

    std::vector<Actor*> targetList;  // Hedef oyuncular ve moblar
};