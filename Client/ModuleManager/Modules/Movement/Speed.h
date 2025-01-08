#pragma once
#include "../Module.h"

class Speed : public Module {
public:
	float speed = 2.f;
	bool bunnyHopEnabled = false;
	Speed();
	void onNormalTick(Actor* actor) override;
};