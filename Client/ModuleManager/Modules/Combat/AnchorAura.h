#pragma once
#include "../../ModuleManager.h"
#include "../Module.h"

class AnchorAura : public Module {
private:
	int Neorange = 3;
	bool silent = false;
	std::string names;
	bool shit = false;
	//ground level  ~~~ funni initializations
	Vec3<float> bottom1;
	//middle level  ~ last priority because it is the least effective
	Vec2<float> rotAnglePlace{};
public:
	AnchorAura();
	~AnchorAura();



	int range = 10;
	// Inherited via IModule
	//void charge2(C_GameMode* gm, Vec3<float>* pos);
	virtual void onNormalTick(Actor* actor) override;
	virtual void onRender(MinecraftUIRenderContext* renderCtx) override;
	virtual void onSendPacket(Packet* packet, bool& shouldCancel) override;
	virtual void onDisable() override;
};