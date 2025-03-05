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

    // Nuevas variables para vetas
    bool onlyVeins = false;
    int minVeinSize = 3;
    int veinRadius = 2;

    // Nueva función auxiliar
    int countNearbyOres(Vec3<int> pos, int blockId);

public:
    TeleportModule();
    virtual void onEnable() override;
    virtual void onNormalTick(Actor* actor) override;
    void onDisable();
};
