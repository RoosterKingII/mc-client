#include "BlockOutline.h"

BlockOutline::BlockOutline() : Module("AirPlace", "Outlines the air block your looking at", Category::PLAYER) {
}


void BlockOutline::onRender(MinecraftUIRenderContext* renderCtx) {
	auto player = mc.getLocalPlayer();
	if (player == nullptr) return;

	 Vec3<float> sex = mc.getLocalPlayer()->getLevel()->getPointingBlockPos();
	if (mc.canUseMoveKeys()) {
		Vec3<int> outline = sex.toInt();
		outline = outline.floor();
		outline.y += 0.25f;
		
		 RenderUtils::drawBox(sex, UIColor(0,0,0,0), UIColor(red, green, blue, 255), width, false, true);
		 if (mc.isRightClickDown()) {
			 mc.getGameMode()->buildBlock(outline, 0, 0);
		 }
	}
}