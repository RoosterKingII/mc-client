#pragma once
#include "../Module.h"

class Glide : public Module {
private:
	float Value = 0;
public:

	Glide();
	virtual void onNormalTick(Actor* actor) override;
};
