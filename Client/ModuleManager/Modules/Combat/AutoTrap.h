#pragma once
#include "../Module.h"

class AutoTrap : public Module {
public:
	bool mode1;
	bool mode2;
	float range = 7;
	bool mobs = false;
	AutoTrap();
	void predictBlock(Vec3<float> pos);
	int getObsidian();
	void tryBuildBlock(Vec3<int> tryBuildPos);
	void onNormalTick(Actor* actor) override;
	void onEnable();
	void onDisable();
};