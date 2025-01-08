#pragma once
#include "../Module.h"
class Scaffold : public Module {
public:
    Scaffold();
    void tryBuild7x7();
    void onNormalTick(Actor* actor) override;
    void onRender(MinecraftUIRenderContext* ctx) override;
    void onEnable() override;

private:
    int getObsidian();
    void tryBuildBlock(Vec3<int> tryBuildPos);

    bool center = false;
    bool disableComplete = false;
    bool render = true;
    bool sevenBySeven = false;

    UIColor color = { 255, 255, 255, 255 };    // Varsayýlan renk: beyaz.
    UIColor lineColor = { 0, 0, 0, 255 };      // Varsayýlan kenar rengi: siyah.

    int switchMode = 0;  // 0: None, 1: Full, 2: Silent.
};
