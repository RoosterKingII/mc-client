#pragma once
#include "../Module.h"

class BlockESP : public Module {
private:
    std::chrono::steady_clock::time_point lastUpdateTime;
    std::vector<Vec3<int>> blocks;
    std::mutex listLock;
    int time2 = 1000;
public:
	float opacity = 255.f;
    std::vector<Vec3<int>> blocks2;
    std::mutex listLock2;

    // Variables for block toggles
    bool diamondToggle = false;
    bool emeraldToggle = false;
    bool redstoneToggle = false;
    bool ironToggle = false;
    bool goldToggle = false;
    bool netherWartToggle = false;
    bool coalToggle = false;

    // Custom block support
    bool customToggle = true;
    int customBlockID = 1; // Default custom block ID
    UIColor customColor = UIColor(255, 165, 0, 255); // Default custom color (orange)

	int esese = 20;
	int customid;
	UIColor colsex = UIColor(0, 255, 255, 255);
	int radius = 20;

	BlockESP();
	void getBlocks(std::vector<Vec3<int>>& blocks);
	virtual void onRender(MinecraftUIRenderContext* renderCtx);
};