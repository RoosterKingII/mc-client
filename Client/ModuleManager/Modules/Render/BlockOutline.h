#pragma once
#include "../Module.h"

class BlockOutline : public Module {
private:
	float red, green, blue = 1.f;
	bool rainbow = false;
	float width = 0.5f;
public:
	int mode;

	virtual void onRender(MinecraftUIRenderContext* renderCtx);
	BlockOutline();
};