#pragma once
#include "../Module.h"
#include "../../ModuleManager.h"
class BasePlace : public Module
{
private:
	bool aovcl = false;
	//	vec3_ti blockpos2 = { 0,0,0 };
	int slotab = 0;

public:
	bool silentab = false;
	float range = 5;
	bool notarget = false;
	bool mobs = false;

	bool tryAutoTrap2(Vec3<int> blkPlacement);
	bool canpla(Vec3<int> blockPos);

	BasePlace();

	virtual void onRender(MinecraftUIRenderContext* renderCtx) override;
	//	virtual void onPlayerTick(C_Player* plr) override;
	virtual void onNormalTick(Actor* actor) override;
	//	virtual void onEnable() override;
};