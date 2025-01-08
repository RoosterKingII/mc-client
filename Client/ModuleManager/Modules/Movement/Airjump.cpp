#include "Airjump.h"
#include <Windows.h>
#include "../../../../SDK/Classes/Actor.h"
#include "../../../Client.h"
#include "../../../../SDK/Components/Flags/OnGroundFlag.h"

Airjump::Airjump() : Module("AirJump", "Jump while in air", Category::MOVEMENT) {
    addBoolCheck("Legacy Mode", "Use legacy jumping method", &legacyMode);
}

void Airjump::onNormalTick(Actor* actor) {
    auto localPlayer = mc.getLocalPlayer();
    if (localPlayer == nullptr) return;

    if (legacyMode) {
        localPlayer->setonground(localPlayer->entityContext, true);
        return;
    }

    if (mc.isKeyDown(VK_SPACE) && hasJumped == 0) {
        tick++;
        if (tick >= tickTimer) {
            localPlayer->setonground(localPlayer->entityContext, true);
            // Como no existe jumpFromGround, podríamos simular el salto de otra manera
            // Por ejemplo, modificando la velocidad vertical del jugador
            tick = 0;
        }
        localPlayer->setonground(localPlayer->entityContext, true);
        hasJumped = 0.0001f;
    }
    else if (!mc.isKeyDown(VK_SPACE)) {
        tick++;
        if (tick >= tickTimer) {
            localPlayer->setonground(localPlayer->entityContext, true);
            tick = 0;
        }
        hasJumped = 0;
    }
}