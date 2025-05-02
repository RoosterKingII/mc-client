#include "TeleportModule.h"
#include "../../../Client.h"

const int MIN_X = 60;
const int MAX_X = 1090;
const int MIN_Z = 60;
const int MAX_Z = 1090;

TeleportModule::TeleportModule() : Module("AutoMine", "Teleport to a specific block with a certain speed.", Category::PLAYER) {
    this->toggleMod = 1;
    addBoolCheck("Spawners", "Enable spawner mining", &spawner);
    addBoolCheck("Ancient Debris", "Enable mining of ancient debris", &ad);
    addBoolCheck("Redstone", "Enable mining of redstone", &rOre);
    addBoolCheck("Emeralds", "Enable mining of emeralds", &eOre);
    addBoolCheck("Diamonds", "Enable mining of diamonds", &dOre);
    addBoolCheck("Iron", "Enable mining of iron", &iOre);
    addBoolCheck("Gold", "Enable mining of gold", &gOre);
    addBoolCheck("Coal", "Enable mining of coal", &cOre);
    addBoolCheck("Custom", "Enable mining of custom ID blocks", &cuOre);
   // addBoolCheck("AutoAutoMine", "Automatically mine entire vein", &autoAutoMine);
	addBoolCheck("Anti Lava", "NULL", &al);
    addBoolCheck("Only Veins", "Only teleport to ore veins", &onlyVeins);
    addSlider<int>("Min Vein Size", "Minimum number of ores in vein", ValueType::INT_T, &minVeinSize, 2, 8);
   // addSlider<int>("Vein Radius", "Radius to check for ore veins", ValueType::INT_T, &veinRadius, 1, 2);
    addSlider<int>("Custom ID", "ID of the custom block to mine", ValueType::INT_T, &cuid, 1, 252);
    addSlider<int>("Range", "ATTENTION > **High values make the game lose performance!**", ValueType::INT_T, &range, 3, 140);
}
void TeleportModule::mineNearbyOres(Vec3<float> pos) {
    auto player = mc.getLocalPlayer();
    if (!player) return;

    auto packetMine = (PacketMine*)client->moduleMgr->getModule("PacketMine");
    if (!packetMine) return;

    // Primero minar el bloque sobre la cabeza del jugador
    Vec3<int> headPos((int)pos.x, (int)pos.y + 1, (int)pos.z);
    Block* headBlock = player->dimension->blockSource->getBlock(headPos);
    if (headBlock && headBlock->blockLegacy) {
        packetMine->setEnabled(true);
        packetMine->setBreakPos(headPos, 1);
        return; // Esperar hasta que este bloque esté minado
    }

    // Luego continuar con los demás bloques en el radio
    for (int x = -2; x <= 2; x++) {
        for (int y = -2; y <= 2; y++) {
            for (int z = -2; z <= 2; z++) {
                Vec3<int> checkPos((int)pos.x + x, (int)pos.y + y, (int)pos.z + z);
                Block* block = player->dimension->blockSource->getBlock(checkPos);
                if (!block || !block->blockLegacy) continue;

                int blockId = (int)block->blockLegacy->blockId;
                bool shouldMine = false;

                // Verificar si el bloque coincide con los tipos activados
                if ((blockId == 526 && ad) ||
                    (blockId == 52 && spawner) ||
                    (blockId == 56 && dOre) ||
                    (blockId == 129 && eOre) ||
                    (blockId == 74 && rOre) ||
                    (blockId == 14 && gOre) ||
                    (blockId == 15 && iOre) ||
                    (blockId == cuid && cuOre)) {
                    shouldMine = true;
                }

                if (shouldMine) {
                    packetMine->setEnabled(true);
                    packetMine->setBreakPos(checkPos, 1);
                }
            }
        }
    }
}
bool TeleportModule::checkForLava(Vec3<int> pos) {
    auto player = mc.getLocalPlayer();
    if (!player) return true; // Retorna true por seguridad

    if (al) {
        for (int x = -4; x <= 4; x++) {
            for (int y = -4; y <= 4; y++) {
                for (int z = -4; z <= 4; z++) {
                    Vec3<int> checkPos(pos.x + x, pos.y + y, pos.z + z);
                    int blockId = (int)player->dimension->blockSource->getBlock(checkPos)->blockLegacy->blockId;

                    // IDs 10 y 11 son lava
                    if (blockId == 10 || blockId == 11) {
                        return true; // Hay lava cerca
                    }
                }
            }
        }
        return false; // No hay lava cerca
    }
}
Vec3<int> TeleportModule::findNextOreInVein(Vec3<float> currentPos, int targetBlockId) {
    auto player = mc.getLocalPlayer();
    if (!player) return Vec3<int>(0, 0, 0);

    for (int x = -veinRadius; x <= veinRadius; x++) {
        for (int y = -veinRadius; y <= veinRadius; y++) {
            for (int z = -veinRadius; z <= veinRadius; z++) {
                Vec3<int> checkPos((int)currentPos.x + x, (int)currentPos.y + y, (int)currentPos.z + z);
                Block* block = player->dimension->blockSource->getBlock(checkPos);
                if (block && block->blockLegacy && (int)block->blockLegacy->blockId == targetBlockId) {
                    return checkPos;
                }
            }
        }
    }
    return Vec3<int>(0, 0, 0);
}

int TeleportModule::countNearbyOres(Vec3<int> pos, int targetBlockId) {
    if (!onlyVeins) return 1;

    auto player = mc.getLocalPlayer();
    if (!player) return 0;

    int count = 0;
    for (int x = -veinRadius; x <= veinRadius; x++) {
        for (int y = -veinRadius; y <= veinRadius; y++) {
            for (int z = -veinRadius; z <= veinRadius; z++) {
                Vec3<int> checkPos(pos.x + x, pos.y + y, pos.z + z);
                int blockId = (int)player->dimension->blockSource->getBlock(checkPos)->blockLegacy->blockId;
                if (blockId == targetBlockId) {
                    count++;
                }
            }
        }
    }
    return count;
}

void TeleportModule::onEnable() {
    mc.getClientInstance()->minecraft->mctimer->tps = timerV;
    works = false;
    tpdone = false;
    closestDistance = std::numeric_limits<float>::max();
    goal = Vec3<float>(0, 0, 0);

    auto player = mc.getLocalPlayer();
    if (player == nullptr) return;

    Vec3<float> currentPos = *player->getPosition();

    for (int x = (int)currentPos.x - range; x <= currentPos.x + range; x++) {
        for (int z = (int)currentPos.z - range; z <= currentPos.z + range; z++) {
            for (int y = (int)currentPos.y - range * 0.5; y <= currentPos.y + range * 0.3; y++) {
                int id = (int)player->dimension->blockSource->getBlock(Vec3(x, y, z))->blockLegacy->blockId;

                TP = false;

                if (id == 526 && ad) TP = true;
                if (id == 52 && spawner) TP = true;
                if (id == 56 && dOre) TP = true;
                if (id == 129 && eOre) TP = true;
                if (id == 74 && rOre) TP = true;
                if (id == 14 && gOre) TP = true;
                if (id == 15 && iOre) TP = true;
                if (id == cuid && cuOre) TP = true;

                if (TP) {
                    bool validTarget = true;

                    // Verificar si hay lava cerca
                    if (checkForLava(Vec3<int>(x, y, z))) {
                        validTarget = false;
                    }

                    if (onlyVeins) {
                        int oreCount = countNearbyOres(Vec3<int>(x, y, z), id);
                        if (oreCount < minVeinSize) {
                            validTarget = false;
                        }
                    }

                    if (validTarget) {
                        Vec3<float> targetPos(x + 0.5f, y + 0.55f, z + 0.5f);

                        if (targetPos.y > 6 &&
                            targetPos.x > MIN_X && targetPos.x < MAX_X &&
                            targetPos.z > MIN_Z && targetPos.z < MAX_Z) {

                            float distance = currentPos.distanceTo(targetPos);
                            if (distance < closestDistance) {
                                closestDistance = distance;
                                goal = targetPos;
                            }
                        }
                    }
                }
            }
        }
    }

    if (closestDistance < std::numeric_limits<float>::max()) {
        works = true;
    //    mc.DisplayClientMessage("%s[%sAutoMine%s] %sOre found!", WHITE, WHITE, GREEN, GREEN);
    }
    else {
      //  mc.DisplayClientMessage("%s[%sAutoMine%s] %sCouldn't find any available blocks in specified range%s!", WHITE, WHITE, RED);
        this->setEnabled(false);
    }
}

void TeleportModule::onNormalTick(Actor* actor) {
    auto player = mc.getLocalPlayer();
    if (player == nullptr || !works) return;

    Vec3<float> currentPos = *player->getPosition();
    float distance = currentPos.distanceTo(goal);
    float lerpFactor = 0.0f;

    if (distance <= 0.25f) {
        if (autoAutoMine) {
            mineNearbyOres(goal);
        }
        else {
            mc.getClientInstance()->minecraft->mctimer->tps = 20.f;
            tpdone = true;
            this->setEnabled(false);
        }
        return;
    }

    // Ajustar factores de interpolación
    if (distance <= 13.0f) lerpFactor = 0.55f;
    else if (distance <= 15.0f) lerpFactor = 0.45f;
    else if (distance <= 20.0f) lerpFactor = 0.45f;
    else if (distance <= 50.0f) lerpFactor = 0.25f;
    else if (distance <= 100.0f) lerpFactor = 0.10f;
    else if (distance <= 300.0f) lerpFactor = 0.075f;

    Vec3<float> newPos = currentPos.lerp(goal, lerpFactor);
    // Ajustar la altura final
   // newPos.y += 0.1f;
    player->setPos(newPos);
}

void TeleportModule::onDisable() {
    mc.getClientInstance()->minecraft->mctimer->tps = 20.f;
    if (autoAutoMine) {
        auto packetMine = (PacketMine*)client->moduleMgr->getModule("PacketMine");
        if (packetMine && packetMine->isEnabled()) {
            packetMine->setEnabled(false);
        }
    }
}
