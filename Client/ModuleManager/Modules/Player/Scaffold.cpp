#include "Scaffold.h"

Scaffold::Scaffold() : Module("Scaffold", "Place blocks beneath you.", Category::PLAYER) {
    addBoolCheck("Render", "Render placement area", &render);
    addColorPicker("Color", "Block fill color", &color);
    addColorPicker("LineColor", "Block outline color", &lineColor);
    addBoolCheck("7x7 Area", "Place blocks in a 7x7 area", &sevenBySeven);  
}

int Scaffold::getObsidian() {
    PlayerInventory* plrInv = mc.getLocalPlayer()->getPlayerInventory();
    Inventory* inv = plrInv->inventory;

    for (int i = 0; i < 9; i++) {
        ItemStack* itemStack = inv->getItemStack(i);
        if (itemStack->isValid() && itemStack->getItemPtr()->itemId == 49) {  // Obsidian ID = 49
            return i;
        }
    }
    return plrInv->selectedSlot;  
}

void Scaffold::tryBuildBlock(Vec3<int> tryBuildPos) {
    LocalPlayer* localPlayer = mc.getLocalPlayer();
    GameMode* gm = localPlayer->getGameMode();
    PlayerInventory* plrInv = localPlayer->getPlayerInventory();

    Block* block = localPlayer->dimension->blockSource->getBlock(tryBuildPos);
    if (block->blockLegacy->blockId == 0) {  
        int bestSlot = getObsidian();
        bool shouldSwitch = (bestSlot != plrInv->selectedSlot);

        if (shouldSwitch && (switchMode == 1 || switchMode == 2)) {
            plrInv->selectedSlot = bestSlot;
            if (switchMode == 2) {
                MobEquipmentPacket pk(localPlayer->getRuntimeID(), plrInv->inventory->getItemStack(bestSlot), bestSlot, bestSlot);
                mc.getClientInstance()->loopbackPacketSender->send(&pk);
            }
        }

        gm->buildBlock(tryBuildPos, 0, 0); 
    }
}

void Scaffold::tryBuild7x7() {
    LocalPlayer* localPlayer = mc.getLocalPlayer();
    if (!sevenBySeven) return; 

    Vec3<float> playerPos = *localPlayer->getPosition();
    playerPos.y -= 2.f;  
    Vec3<int> startPos = playerPos.floor().toInt();

    for (int x = -3; x <= 3; x++) {
        for (int z = -3; z <= 3; z++) {
            Vec3<int> targetPos = startPos.add(x, 0, z); 
            tryBuildBlock(targetPos);  
        }
    }
}

void Scaffold::onRender(MinecraftUIRenderContext* ctx) {
    if (!render) return;

    LocalPlayer* localPlayer = mc.getLocalPlayer();
    if (localPlayer == nullptr) return;

    Vec3<float> playerPos = *localPlayer->getPosition();
    playerPos.y -= 2.f;  

    if (sevenBySeven) {
        Vec3<int> startPos = playerPos.floor().toInt();  
        for (int x = -3; x <= 3; x++) {
            for (int z = -3; z <= 3; z++) {
                Vec3<int> targetPos = startPos.add(x, 0, z);  
                RenderUtils::drawBox(Vec3<float>(targetPos.x, targetPos.y, targetPos.z), color, lineColor, 0.3f, true, false);  // float parametreler için dönüþüm yapýyoruz
            }
        }
    }
    else {
        RenderUtils::drawBox(playerPos, color, lineColor, 0.3f, true, false);  
    }
}
void Scaffold::onNormalTick(Actor* actor) {
    LocalPlayer* localPlayer = mc.getLocalPlayer();
    if (localPlayer == nullptr) return;

    Vec3<float> playerPos = *localPlayer->getPosition();
    playerPos.y -= 2.f; 
    Vec3<int> blockBelow = playerPos.floor().toInt();

    tryBuildBlock(blockBelow);  

    tryBuild7x7();  

    if (disableComplete) {
        this->setEnabled(false);
    }
}

void Scaffold::onEnable() {
    LocalPlayer* localPlayer = mc.getLocalPlayer();
    if (localPlayer == nullptr) return;

    if (center) {
        Vec3<float> playerPos = *localPlayer->getPosition();
        Vec3<float> centeredPos = playerPos.floor();
        centeredPos.x += 0.5f;
        centeredPos.z += 0.5f;
        localPlayer->setPos(centeredPos);  
    }
}