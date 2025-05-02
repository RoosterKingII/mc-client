#pragma once
#include "../Module.h"

class NoClip : public Module {
private:
	float speed = 0.325f;
	float upanddown = 0.6f;
	float glideMod = -0.02f;
	float phase ;
	float glideModEffective = 0;
	Vec3<float> oldPos;
	bool smborder;
	bool enablePhase = true;
	bool NoPacket = false;

public:
	NoClip();
	~NoClip();

	virtual void onNormalTick(Actor* actor) override;
	virtual void onEnable() override;
	virtual void onDisable() override;
};
