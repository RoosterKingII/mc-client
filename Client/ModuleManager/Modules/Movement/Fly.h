#pragma once
#include "../Module.h"

class Fly : public Module {
private:
	int IsWpressed = 0;
	int Modefly = 0;
	int lastMoveTime = 0;
	int DelayUpDown = 0;
	float speed = 0.325f;
	float Glide = -0.2f;
	float glideMod = -0.00f;
	float glideModEffective = 0;
	Vec3<float> oldPos;
	bool smborder;
	bool NoPacket = false;

public:
	float upanddown = 0.6f;
	Fly();
	float HorizontalSpeed = 1.f;
	~Fly();

	virtual void onNormalTick(Actor* actor) override;
	virtual void onEnable() override;
	virtual void onDisable() override;
};

/*#pragma once
#include "../Module.h"

class Fly : public Module {
private:
	float speed = 1.9f;
	float upanddown = 1.8f;
	bool sneak = false;
	float glideMod = -0.02f;
	float glideModEffective = 0;
	Vec3<float> oldPos;
	bool smborder;
	bool NoPacket = false;

public:
	Fly();
	~Fly();

	virtual void onNormalTick(Actor* actor) override;
	virtual void onEnable() override;
	virtual void onDisable() override;
};
*/