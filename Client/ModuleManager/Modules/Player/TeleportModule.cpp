#include "TeleportModule.h"
#include "../../../Client.h"

const int MIN_X = 60;
const int MAX_X = 1090;
const int MIN_Z = 60;
const int MAX_Z = 1090;

TeleportModule::TeleportModule() : Module("AutoMine", "Teleport to a specific block with a certain speed.", Category::PLAYER) {
    addBoolCheck("Spawners", "Enable spawner mining", &spawner);
    addBoolCheck("Ancient Debris", "Enable mining of ancient debris", &ad);
    addBoolCheck("Redstone", "Enable mining of redstone", &rOre);
    addBoolCheck("Emeralds", "Enable mining of emeralds", &eOre);
    addBoolCheck("Diamonds", "Enable mining of diamonds", &dOre);
    addBoolCheck("Iron", "Enable mining of iron", &iOre);
    addBoolCheck("Gold", "Enable mining of gold", &gOre);
    addBoolCheck("Coal", "Enable mining of coal", &cOre);
    addBoolCheck("Custom", "Enable mining of custom ID blocks", &cuOre);

    // Nuevas opciones para vetas
    addBoolCheck("Only Veins", "Only teleport to ore veins", &onlyVeins);
    addSlider<int>("Min Vein Size", "Minimum number of ores in vein", ValueType::INT_T, &minVeinSize, 2, 8);
    addSlider<int>("Vein Radius", "Radius to check for ore veins", ValueType::INT_T, &veinRadius, 1, 2);

    addSlider<int>("Custom ID", "ID of the custom block to mine", ValueType::INT_T, &cuid, 1, 252);
    addSlider<int>("Range", "ATTENTION > **High values make the game lose performance!**", ValueType::INT_T, &range, 3, 140);
}

int TeleportModule::countNearbyOres(Vec3<int> pos, int targetBlockId) {
    if (!onlyVeins) return 1; // Si onlyVeins está desactivado, retorna 1 para que siempre sea válido

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
        mc.DisplayClientMessage("%s[%sAutoMine%s] %sOre found!", WHITE, WHITE, GREEN, GREEN);
    } else {
        mc.DisplayClientMessage("%s[%sAutoMine%s] %sCouldn't find any available blocks in specified range%s!", WHITE, WHITE, RED);
        this->setEnabled(false);
    }
}

void TeleportModule::onNormalTick(Actor* actor) {
    auto player = mc.getLocalPlayer();
    if (player == nullptr || !works || tpdone) return; // Se não houver jogador ou se já teleportou, sai

    Vec3<float> currentPos = *player->getPosition();
    float distance = currentPos.distanceTo(goal);
    float lerpFactor = 0.0f; // Fator de interpolação

    // Ajusta o fator de interpolação com base na distância
    if (distance <= 0.25f) {
        lerpFactor = 0.0f; // Parar o movimento
        mc.getClientInstance()->minecraft->mctimer->tps = 20.f;
        static TeleportModule* mineMod = (TeleportModule*)client->moduleMgr->getModule("AutoMine");
        tpdone = true; // Marca o teleporte como concluído
        this->setEnabled(false); // Desativa o módulo
        return;
        mc.getClientInstance()->minecraft->mctimer->tps = 20.f;
    }
    else if (distance <= 13.0f) {
        lerpFactor = 0.55f; // Distância de 13 blocos
    }
    else if (distance <= 15.0f) {
        lerpFactor = 0.45f; // Distância de 15 blocos
    }
    else if (distance <= 20.0f) {
        lerpFactor = 0.45f; // Distância de 20 blocos
    }
    else if (distance <= 50.0f) {
        lerpFactor = 0.25f; // Distância de 50 blocos
    }
    else if (distance <= 100.0f) {
        lerpFactor = 0.10f; // Distância de 100 blocos
    }
    else if (distance <= 140.0f) {
        lerpFactor = 0.10f; // Distância de 100 blocos
    }
    else if (distance <= 170.0f) {
        lerpFactor = 0.10f; // Distância de 100 blocos
    }
    else if (distance <= 200.0f) {
        lerpFactor = 0.10f; // Distância de 100 blocos
    }
    else if (distance <= 250.0f) {
        lerpFactor = 0.075f; // Distância de 100 blocos
    }
    else if (distance <= 300.0f) {
        lerpFactor = 0.075f; // Distância de 100 blocos
    }

    Vec3<float> newPos = currentPos.lerp(goal, lerpFactor);
    player->setPos(newPos); // Atualiza a posição
}


void TeleportModule::onDisable() {
    mc.getClientInstance()->minecraft->mctimer->tps = 20.f;
}
