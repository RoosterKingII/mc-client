#pragma once

#include "../Module.h"
class Jetpack : public Module {
private:
	int delay = 0;
	bool isBypass = false;
	float speedMod = 1;
public:
	MovePlayerPacket* jetpack = nullptr;
	Jetpack();
	~Jetpack();
	virtual void onNormalTick(Actor* actor) override;
	virtual bool isFlashMode();
};