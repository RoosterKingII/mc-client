#pragma once
#include "../Module.h"

class TeleportModule : public Module {
private:
    bool works = false;
    bool ad = false;
    bool spawner = false;
    bool dOre = false;
    bool eOre = false;
    bool rOre = false;
    bool gOre = false;
    bool iOre = false;
    bool cOre = false;
    bool cuOre = false;
    bool entity = false;
    int range = 50;
    int range2 = 4;
    bool border = true;
    int breakKeybind = 0x0;
    int why = 2;
    int timerV = 480;
    float closestDistance = 0;
    int speed = 1;
    int cuid = 1;
    int i;
    bool al = true;
    Vec3<float> goal;
    bool TP = false;
    bool tpdone = false;
    bool ab = true;
    bool as = true;
    bool autoAutoMine = false; 
    int currentBlockId = 0;    
    bool onlyVeins = false;
    int minVeinSize = 3;
    int veinRadius = 1;

    void mineNearbyOres(Vec3<float> pos);
    bool checkForLava(Vec3<int> pos);
    int countNearbyOres(Vec3<int> pos, int blockId);
    Vec3<int> findNextOreInVein(Vec3<float> currentPos, int targetBlockId);
public:
    TeleportModule();
    virtual void onEnable() override;
    virtual void onNormalTick(Actor* actor) override;
    void onDisable();
};
