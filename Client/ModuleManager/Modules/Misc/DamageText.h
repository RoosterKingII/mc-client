#pragma once
#include "../Module.h"

class DamageText : public Module {
private:
	float width = 1.f;
	float height = 1.f;
	bool mobs = false;
	std::vector<Actor*> DamageList;
public:
	DamageText();

	virtual void onNormalTick(Actor* actor) override;
};