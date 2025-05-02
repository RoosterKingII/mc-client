#pragma once
#include "../Module.h"

class AutoTotem : public Module {

public:
	AutoTotem();

	void onNormalTick(Actor* actor) override;
};