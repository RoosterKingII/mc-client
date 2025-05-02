#pragma once
#include "../Module.h"
#include "../../ModuleManager.h"

class LifeboatReach : public Module {
public:
    float ARange = 100.f;
    int timer = 20;
    int APS = 400;
    Vec2<float> joe;
    bool noSwing = false;
    bool tpActive = false;
    float MaxSrange = 100.f;
    float MinSrange = 0.f;
    float targetRange = 100.f;
    bool attackMob = false;
    std::vector<Actor*> targetList;
    Vec2<float> rotAngle;
    LifeboatReach(); 

    void onSendPacket(Packet* packet, bool& shouldCancel) override;

    void teleportToTarget(Player* player, Actor* target, float distance);

    void attackTarget(Player* player, Actor* target);

    void onDisable() override;

private:
    int lastMoveTime = 30;
    float HorizontalSpeed = 1.f;
    float maxTPDistance = 1.0f;
    int tpDelay = 1;
    int speedX = 14.0f;              // X eksenindeki hýz
    int slowSpeedX = 14.0f;          // Sol hareket için yavaþ hýz
    int maxMoveSpeed = 14.0f;      // Maksimum hareket hýzý
    int attackdelay = 600.0f;      // lllllll zero one best LOL
    int moveDistance = 14.0f;      // 
    int delayVariable = 0;           // Geri dönüþ süresi
    int Mode = 0;                     // Mod ayarý
};