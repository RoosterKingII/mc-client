#pragma once
#include "../Module.h"
#include "../../../../SDK/Classes/Actor.h"

class ESP : public Module {
private:
    std::vector<Actor*> espList;
    int renderMode = 0;
    UIColor color = UIColor(0, 255, 255, 50);
    UIColor lineColor = UIColor(0, 255, 255, 255);
    bool itemESP = false; 
public:
    bool mobs = false;
    ESP();
    virtual void onRender(MinecraftUIRenderContext* ctx) override;
};
