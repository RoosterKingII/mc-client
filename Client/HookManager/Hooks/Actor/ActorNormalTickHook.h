#pragma once
#include "../../../../SDK/Classes/Actor.h"

#include "../../../../Utils/MemoryUtils.h"
#include "../../../../SDK/GameData.h"
class VerifyHook;
class ActorNormalTickHook {
protected:
	using func_t = void(__fastcall*)(Actor*);
	static inline func_t func;
	static void callback(Actor* _this) {
		VerifyHook::onTick();
		//if(clientInstance = nullptr)
				if (_this == mc.getLocalPlayer())
					client->moduleMgr->onNormalTick(_this);
					func(_this);
	}
public:
	
	static void init(uintptr_t address) {
		MemoryUtils::CreateHook("Actor_NormalTickHook", address, (void*)&ActorNormalTickHook::callback, (void*)&func);
	}
};
