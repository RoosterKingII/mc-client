#pragma once
#include "../Module.h"

class DeathCoordinates : public Module {
private:
public:
	int lastDeathTime; 
	bool messageDisplayed;

	DeathCoordinates();
	virtual void onNormalTick(Actor* actor) override;
};