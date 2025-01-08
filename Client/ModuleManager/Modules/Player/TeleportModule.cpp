#include "TeleportModule.h" 
#include "../../../Client.h"

// Limites de fronteira do servidor
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

    addSlider<int>("Custom ID", "ID of the custom block to mine", ValueType::INT_T, &cuid, 1, 252);
    addSlider<int>("Range", "ATTENTION > **High values make the game lose performance!**", ValueType::INT_T, &range, 3, 140);

}

void TeleportModule::onEnable() {
    mc.getClientInstance()->minecraft->mctimer->tps = timerV;
    works = false;
    tpdone = false; // Reset the teleportation flag
    closestDistance = std::numeric_limits<float>::max(); // Inicializa a distância como infinita
    goal = Vec3<float>(0, 0, 0); // Reinicializa o objetivo

    auto player = mc.getLocalPlayer();
    if (player == nullptr) return;

    Vec3<float> currentPos = *player->getPosition();

    // Procura pelo bloco mais próximo
    for (int x = (int)currentPos.x - range; x <= currentPos.x + range; x++) {
        for (int z = (int)currentPos.z - range; z <= currentPos.z + range; z++) {
            for (int y = (int)currentPos.y - range * 0.5; y <= currentPos.y + range * 0.3; y++) {
                int id = (int)player->dimension->blockSource->getBlock(Vec3(x, y, z))->blockLegacy->blockId;

                TP = false;

                if (id == 526 && ad) TP = true;       // Ancient Debris
                if (id == 52 && spawner) TP = true;   // Spawner
                if (id == 56 && dOre) TP = true;      // Diamond
                if (id == 129 && eOre) TP = true;     // Emerald
                if (id == 74 && rOre) TP = true;      // Redstone
                if (id == 14 && gOre) TP = true;      // Gold
                if (id == 15 && iOre) TP = true;      // Iron
                if (id == cuid && cuOre) TP = true;   // Custom block

                if (TP) {
                    Vec3<float> targetPos(x + 0.5f, y + 0.55f, z + 0.5f);

                    // Verifica se o bloco alvo está abaixo da camada Y=5
                    if (targetPos.y > 6 & targetPos.x > 60) {

                        if (targetPos.x < 1090 & targetPos.z > 60) {

                            if (targetPos.z < 1090) {

                                float distance = currentPos.distanceTo(targetPos);

                                // Verifica se esse bloco é o mais próximo encontrado até agora
                                if (distance < closestDistance) {
                                    closestDistance = distance;
                                    goal = targetPos; // Atualiza o bloco mais próximo
                                }
                            }
                        }
                    }
                    else {

                    }
                }
            }
        }
    }

    if (closestDistance < std::numeric_limits<float>::max()) {
        works = true; // Um bloco foi encontrado
        mc.DisplayClientMessage("%s[%sAutoMine%s] %sOre found!", WHITE, WHITE, GREEN, GREEN);
    }
    else {
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