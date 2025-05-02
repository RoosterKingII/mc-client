#pragma once
#include "../Module.h"

class MidClick : public Module {
private:
	bool hasClicked = false;
public:
	MidClick() : Module("MidClick", "Friends players", Category::PLAYER) {};
	void onNormalTick(Actor* actor) override;
};