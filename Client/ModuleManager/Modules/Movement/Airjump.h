#pragma once
#include "../Module.h"

class Airjump : public Module {
private:
    bool legacyMode = false;
    float hasJumped = 0;
    int tick = 0;
    const int tickTimer = 2;

public:
    Airjump();
    virtual void onNormalTick(Actor* actor) override;
};