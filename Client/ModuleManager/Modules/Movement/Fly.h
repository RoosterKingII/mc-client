#pragma once
#include "../Module.h"

class Fly2 : public Module {
private:
    int IsWpressed = 0;
    int Modefly = 0;
    int lastMoveTime = 0;
    int DelayUpDown = 0;
    float speed = 0.325f;
    float Glide = -0.2f;
    float glideMod = -0.00f;
    float glideModEffective = 0;
    float waterSpeedMultiplier = 0.7f;
    Vec3<float> oldPos;
    bool smborder=false;
    bool NoPacket = false;

    // Parámetros para normalización por distancia
    int distanceThreshold;
    float speedMultiplierMin = 0.1f;
    float normalizationStrength = 1.0f;

public:
    float upanddown = 0.6f;
    float HorizontalSpeed = 1.f;

    Fly2();
    ~Fly2();

    virtual void onNormalTick(Actor* actor) override;
    virtual void onEnable() override;
    virtual void onDisable() override;
};
