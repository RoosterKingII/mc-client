#pragma once
#include "../Module.h"

class ProxyFake : public Module {
private:
	float width = 1.f;
	int tickCounter = 0.f;
	int Ticks = 0.f;
	float height = 1.f;
	bool mobs = false;
	std::vector<Actor*> DamageList;
public:
	ProxyFake();

	virtual void onNormalTick(Actor* actor) override;
	void onEnable();
	void onDisable();
};
