#include "ESP.h"

ESP::ESP() : Module("ESP", "ESP Enities.", Category::RENDER) {
 
    addEnumSetting("Mode", "Changes esp modes", { "3D", "2D" }, &renderMode);
    addColorPicker("Color", "NULL", &color);
    addColorPicker("LineColor", "NULL", &lineColor);
    addBoolCheck("Mobs", "ESP Mobs", &mobs);
    addBoolCheck("Items", "ESP Items", &itemESP); 
}

void ESP::onRender(MinecraftUIRenderContext* ctx) {
    LocalPlayer* localPlayer = mc.getLocalPlayer();
    if (localPlayer == nullptr) return;
    if (!mc.canUseMoveKeys()) return;
    Level* level = localPlayer->getLevel();
    if (level == nullptr) return;

    espList.clear();

    for (Actor* ent : level->getRuntimeActorList()) {
        bool isValid = TargetUtils::isTargetValid(ent, mobs);
        bool isItem = itemESP && ent->getEntityTypeId() == 64;

        if (isValid || isItem) {
            espList.push_back(ent);
        }
    }

    for (Actor* ent : espList) {
        AABB mobAABB = *ent->getAABB();

        switch (renderMode) {
        case 0: 
            RenderUtils::drawBox(mobAABB, color, lineColor, .3f, true, false);
            break;

        case 1: 
            RenderUtils::draw2DBox(mobAABB, color, lineColor, .3f, true, false);
            break;
        }
    }
}
	
