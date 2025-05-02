#include "AutoTrap.h"

AutoTrap::AutoTrap() : Module("AutoTrap", "Automaticly covers target with obsidian", Category::COMBAT) {
    addSlider<float>("rango", "NULL", ValueType::FLOAT_T, &range, 0.1f, 7.f);
	//addEnumSetting("TrapMode", "NULL", { "Box", "Surround" }, &mode);
	addBoolCheck("Box", "NULL", &mode1);
	addBoolCheck("Surround", "NULL", &mode2);
   // addEnumSetting("TrapMode", "What", { "Box", "Surround" }, &mode);
    addBoolCheck("Mobs", "NULL", &mobs);
}
std::vector<Actor*> targetList;

void AutoTrap::predictBlock(Vec3<float> pos) {
    GameMode* gm = mc.getGameMode();
    static std::vector<Vec3<float>> blocks;

    if (blocks.empty()) {
        for (int y = -5; y <= 3; y++) {
            for (int x = -5; x <= 5; x++) {
                for (int z = -5; z <= 5; z++) {
                    blocks.emplace_back(Vec3<float>(x, y, z));
                }
            }
        }

        std::ranges::sort(blocks, {}, &Math::calculateDistance);
    }

    auto tryPlaceBlock = [&](const Vec3<float>& offset) {
        Vec3<float> currentBlock = (Vec3<float>(pos.floor())).add(offset);
        if (gm->tryPlaceBlock(currentBlock.toInt())) {
            return true;
        }
        return false;
        };

    for (const Vec3<float>& offset : blocks) {
        if (tryPlaceBlock(offset)) {
            return;
        }
    }
}

int AutoTrap::getObsidian() {
    PlayerInventory* plrInv = mc.getLocalPlayer()->getPlayerInventory();
    Inventory* inv = plrInv->inventory;
    int slot = plrInv->selectedSlot;
    for (int i = 0; i < 9; i++) {
        ItemStack* itemStack = inv->getItemStack(i);
        if (itemStack->isValid()) {
            if (itemStack->getItemPtr()->itemId == 49) {
                return i;
            }
        }
    }
    return slot;
}
void AutoTrap::tryBuildBlock(Vec3<int> tryBuildPos) {
    LocalPlayer* localPlayer = mc.getLocalPlayer();
    GameMode* gm = localPlayer->getGameMode();
    PlayerInventory* plrInv = localPlayer->getPlayerInventory();
    Inventory* inv = plrInv->inventory;

    Vec3<float> playerPos = *localPlayer->getPosition();
    playerPos.y -= 1.f;
    playerPos = playerPos.floor();

    Block* block = localPlayer->dimension->blockSource->getBlock(tryBuildPos);
    if (block->blockLegacy->blockId == 0) {

        int bestSlot = getObsidian();
        int oldSlot = plrInv->selectedSlot;
        bool shouldSwitch = (bestSlot != plrInv->selectedSlot);
        if (shouldSwitch) {
            plrInv->selectedSlot = bestSlot;
                MobEquipmentPacket pk(localPlayer->getRuntimeID(), inv->getItemStack(bestSlot), bestSlot, bestSlot);
                mc.getClientInstance()->loopbackPacketSender->send(&pk);
        }
        InteractPacket inter(InteractAction::LEFT_CLICK, mc.getLocalPlayer()->getRuntimeID(), tryBuildPos.toFloat());
        predictBlock(tryBuildPos.toFloat());
        mc.getClientInstance()->loopbackPacketSender->sendToServer(&inter);
        if (shouldSwitch) {
            plrInv->selectedSlot = oldSlot;
        }
    }
}
void AutoTrap::onNormalTick(Actor* actor) {
    targetList.clear();
    auto* localPlayer = mc.getLocalPlayer();
    BlockSource* region = localPlayer->dimension->blockSource;
    auto* level = localPlayer->getLevel();
    for (Actor* actor : level->getRuntimeActorList()) {
        if (TargetUtils::isFriendValid(actor, mobs)) {
            float seenPercent = region->getSeenPercent(localPlayer->getEyePos(), *actor->getAABB());
            float dist = actor->getPosition()->dist(*localPlayer->getPosition());
            float rangeCheck = (seenPercent > 0.f) ? 7 : 7;
            if (dist < rangeCheck) targetList.push_back(actor);
        }
    }
    if (!targetList.empty()) {
        Vec3<float> enemyLoc = targetList[0]->getPosition()->floor().sub(Vec3<float>(0.f, 0.f, 0.f));
        std::vector<Vec3<float>> positions = {
    { enemyLoc } };
        if (mode1) {
            for (auto& pos : positions) {
                Vec3<int> abovePos = (Vec3<int>(pos.x, pos.y, pos.z).add(Vec3<int>(0, 2, 0))).toInt();
                Vec3<int> abovePo2 = (Vec3<int>(pos.x, pos.y, pos.z).add(Vec3<int>(1, 0, 0))).toInt();
                Vec3<int> abovePo3 = (Vec3<int>(pos.x, pos.y, pos.z).add(Vec3<int>(-1, 0, 0))).toInt();
                Vec3<int> abovePo4 = (Vec3<int>(pos.x, pos.y, pos.z).add(Vec3<int>(0, 0, 1))).toInt();
                Vec3<int> abovePo5 = (Vec3<int>(pos.x, pos.y, pos.z).add(Vec3<int>(0, 0, -1))).toInt();
                Vec3<int> abovePo7 = (Vec3<int>(pos.x, pos.y, pos.z).add(Vec3<int>(1, 1, 0))).toInt();
                Vec3<int> abovePo8 = (Vec3<int>(pos.x, pos.y, pos.z).add(Vec3<int>(-1, 1, 0))).toInt();
                Vec3<int> abovePo9 = (Vec3<int>(pos.x, pos.y, pos.z).add(Vec3<int>(0, 1, 1))).toInt();
                Vec3<int> abovePo10 = (Vec3<int>(pos.x, pos.y, pos.z).add(Vec3<int>(0, 1, -1))).toInt();
                Vec3<int> abovePo6 = (Vec3<int>(pos.x, pos.y, pos.z).add(Vec3<int>(0, -1, 0))).toInt();
                Vec3<int> abovePo11 = (Vec3<int>(pos.x, pos.y, pos.z).add(Vec3<int>(0, -2, 0))).toInt();
                tryBuildBlock(abovePos);
                tryBuildBlock(abovePo2);
                tryBuildBlock(abovePo3);
                tryBuildBlock(abovePo4);
                tryBuildBlock(abovePo5);
                tryBuildBlock(abovePo6);
                tryBuildBlock(abovePo7);
                tryBuildBlock(abovePo8);
                tryBuildBlock(abovePo9);
                tryBuildBlock(abovePo10);
                tryBuildBlock(abovePo11);
            }
        }
        if (mode2) {
                for (auto& pos : positions) {
                    Vec3<int> abovePos = (Vec3<int>(pos.x, pos.y, pos.z).add(Vec3<int>(0, 2, 0))).toInt();
                    Vec3<int> abovePo2 = (Vec3<int>(pos.x, pos.y, pos.z).add(Vec3<int>(1, 0, 0))).toInt();
                    Vec3<int> abovePo3 = (Vec3<int>(pos.x, pos.y, pos.z).add(Vec3<int>(-1, 0, 0))).toInt();
                    Vec3<int> abovePo4 = (Vec3<int>(pos.x, pos.y, pos.z).add(Vec3<int>(0, 0, 1))).toInt();
                    Vec3<int> abovePo5 = (Vec3<int>(pos.x, pos.y, pos.z).add(Vec3<int>(0, 0, -1))).toInt();
                    Vec3<int> abovePo6 = (Vec3<int>(pos.x, pos.y, pos.z).add(Vec3<int>(0, -1, 0))).toInt();
                    Vec3<int> abovePo11 = (Vec3<int>(pos.x, pos.y, pos.z).add(Vec3<int>(0, -2, 0))).toInt();
                    tryBuildBlock(abovePos);
                    tryBuildBlock(abovePo2);
                    tryBuildBlock(abovePo3);
                    tryBuildBlock(abovePo4);
                    tryBuildBlock(abovePo5);
                    tryBuildBlock(abovePo6);
                    tryBuildBlock(abovePo11);
            }
        }
    }
}
void AutoTrap::onEnable() {
    targetList.clear();
}
void AutoTrap::onDisable() {
    targetList.clear();
}