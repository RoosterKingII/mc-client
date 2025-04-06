#include "Killaura.h"
#include <random>
#include <cmath>
#include <algorithm>
#include "../../../Client.h"

Killaura::Killaura() : Module("Killaura", "Auto attack players around you.", Category::COMBAT) {
    addEnumSetting("TargetMode", "NULL", { "Single", "Multi" }, &targetmode);
    addSlider<float>("Range", "Target range", ValueType::FLOAT_T, &this->range, 2.f, 150.f);
    addSlider<float>("speed predict head", "speed of rotations of head of predict.", ValueType::FLOAT_T, &this->PredictSpeed, 0.f, 250.f);
    addSlider<int>("Interval", "Attack delay in ticks", ValueType::INT_T, &this->delay, 0, 20);
    addSlider<int>("Multiplier", "Number of attacks per target", ValueType::INT_T, &this->multiplier, 1, 10);
    addSlider<int>("Prediction test", "amount predict", ValueType::INT_T, &DistPredict, 0, 30);
    addSlider<int>("Prediction", "amount predict strafe distance", ValueType::INT_T, &test, 0, 30);
    addEnumSetting("Rotation", "Select rotation type", { "None", "Silent", "Strafe", "Predict", "FrontStrafe" }, &rots);
    addEnumSetting("Switch", "NULL", { "None", "Full", "Silent" }, &switchMode);
    addBoolCheck("Hurttime", "Check hurttime", &this->hurttime);
    addSlider<int>("Hit Chance", "Chance to hit (1-100)", ValueType::INT_T, &this->hitChance, 1, 100);
    addBoolCheck("Randomize Hit", "Randomize hit", &this->randomizeHit);
    addBoolCheck("Target Visualize", "NULL", &targetVisualize);
    addColorPicker("TV Color", "NULL", &visualizeColor);
    addBoolCheck("NoSwing", "Disable swing animation", &this->noSwing);
    addBoolCheck("Auto Trident", "Switch to trident when in water", &this->autoTrident);
}

Killaura::~Killaura() {
}

std::string Killaura::getModName() {
    static char textStr[15];
    if (rots == 0) {
        sprintf_s(textStr, 15, "None");
    }
    else if (rots == 1) {
        sprintf_s(textStr, 15, "Silent");
    }
    else if (rots == 2) {
        sprintf_s(textStr, 15, "Strafe");
    }
    else if (rots == 3) {
        sprintf_s(textStr, 15, "Predict");
    }
    else if (rots == 4) {
        sprintf_s(textStr, 15, "FrontStrafe");
    }
    return std::string(textStr);
}

int Killaura::getBestWeaponSlot() {
    auto localPlayer = mc.getLocalPlayer();
    if (!localPlayer) return -1;

    auto plrInv = localPlayer->getPlayerInventory();
    if (!plrInv) return -1;

    auto inv = plrInv->inventory;
    if (!inv) return -1;

    float damage = 0.f;
    int slot = plrInv->selectedSlot;

    for (int i = 0; i < 9; i++) {
        auto itemStack = inv->getItemStack(i);
        if (itemStack && itemStack->isValid()) {
            float currentDamage = itemStack->getItemPtr()->getAttackDamage() + (1.25f * itemStack->getEnchantLevel(EnchantID::sharpness));
            if (currentDamage > damage) {
                damage = currentDamage;
                slot = i;
            }
        }
    }
    return slot;
}

void Killaura::sortByDist() {
    auto localPlayer = mc.getLocalPlayer();
    if (!localPlayer) return;

    std::sort(targetListJ.begin(), targetListJ.end(), [&localPlayer](Actor* a, Actor* b) {
        float distA = a->stateVectorComponent->pos.dist(localPlayer->stateVectorComponent->pos);
        float distB = b->stateVectorComponent->pos.dist(localPlayer->stateVectorComponent->pos);
        return distA < distB;
        });
}

void Killaura::findEntityhhD() {
    auto localPlayer = mc.getLocalPlayer();
    auto level = localPlayer->getLevel();

    if (!level) return;

    targetListJ.clear();
    for (auto& target : level->getRuntimeActorList()) {
        if (TargetUtils::isFriendValid(target, isMobAura)) {
            float dist = target->stateVectorComponent->pos.dist(localPlayer->stateVectorComponent->pos);
            if (dist < range) {
                targetListJ.push_back(target);
            }
        }
    }
    sortByDist();
}

// Nuevo método: busca en la hotbar el slot que contenga un tridente (texture_name == "trident")
int Killaura::getTridentSlot() {
    auto localPlayer = mc.getLocalPlayer();
    if (!localPlayer) return -1;

    auto plrInv = localPlayer->getPlayerInventory();
    if (!plrInv) return -1;

    auto inv = plrInv->inventory;
    if (!inv) return -1;

    for (int i = 0; i < 9; i++) {
        auto itemStack = inv->getItemStack(i);
        if (itemStack && itemStack->isValid()) {
            if (itemStack->getItemPtr()->texture_name == "trident")
                return i;
        }
    }
    return -1;
}

void Killaura::onNormalTick(Actor* actor) {
    auto localPlayer = mc.getLocalPlayer();
    if (!localPlayer) return;

    auto plrInv = localPlayer->getPlayerInventory();
    if (!plrInv) return;

    auto inv = plrInv->inventory;
    if (!inv) return;

    // Bloque de Auto Trident: si el setting está activado y se detecta agua en las cercanías, se cambia al slot del tridente.
    if (autoTrident) {
        bool inWater = false;
        Vec3<float>* playerPos = localPlayer->getPosition();
        if (playerPos) {
            // Se itera en un cubo 3x3x3 alrededor de la posición del jugador (con offset en Y)
            for (int x = (int)playerPos->x - 1; x <= (int)playerPos->x + 1; x++) {
                for (int y = (int)playerPos->y - 1; y <= (int)playerPos->y + 1; y++) {
                    for (int z = (int)playerPos->z - 1; z <= (int)playerPos->z + 1; z++) {
                        Vec3<int> blockPos(x, y + 1, z);
                        int blockID = localPlayer->dimension->blockSource->getBlock(blockPos)->blockLegacy->blockId;
                        if (blockID == 8 || blockID == 9) {
                            inWater = true;
                            break;
                        }
                    }
                    if (inWater) break;
                }
                if (inWater) break;
            }
        }
        if (inWater) {
            int tridentSlot = getTridentSlot();
            if (tridentSlot != -1 && tridentSlot != plrInv->selectedSlot) {
                plrInv->selectedSlot = tridentSlot;
            }
            // Se retorna sin ejecutar la lógica normal de ataque mientras se está en agua.
            return;
        }
    }

    targetListJ.clear();
    findEntityhhD();

    float DistT0 = targetListJ.empty() ? 0.0f : sqrt(pow(localPlayer->stateVectorComponent->pos.x - targetListJ[0]->stateVectorComponent->pos.x, 2) +
        pow(localPlayer->stateVectorComponent->pos.z - targetListJ[0]->stateVectorComponent->pos.z, 2));
    Odelay++;
    if (!targetListJ.empty() && Odelay >= delay) {
        int bestSlot = getBestWeaponSlot();
        int oldSlot = plrInv->selectedSlot;
        bool shouldSwitch = (bestSlot != plrInv->selectedSlot);

        if (shouldSwitch && (switchMode == 1 || switchMode == 2)) {
            plrInv->selectedSlot = bestSlot;
            if (switchMode == 2) {
                MobEquipmentPacket pk(localPlayer->getRuntimeID(), inv->getItemStack(bestSlot), bestSlot, bestSlot);
                mc.getLocalPlayer()->sendNetworkPacket(pk);
                mc.getClientInstance()->loopbackPacketSender->send(&pk);
            }
        }

        Vec2<float> ange = mc.getLocalPlayer()->getPosition()->CalcAngle(*targetListJ[0]->getPosition()).normAngles();

        if (rots == 2) {
            float distance = 10.f; // Radio para el oscilado
            float time = ImGui::GetTime();
            ange.x += sin(time * 0.1f) * distance;
            ange.y += cos(time * 0.1f) * distance;
            localPlayer->rotationComponent->rotation.x = ange.x;
            localPlayer->rotationComponent->rotation.y = ange.y;
        }
        else if (rots == 3) {
            if (DistT0 < DistPredict) {
                Vec3 targetPosition = *targetListJ[0]->getPosition();
                float targetYaw = targetListJ[0]->rotationComponent->rotation.y;
                float targetYawRad = targetYaw * (M_PI / 180.0f);

                targetPosition.x += test * cos(targetYawRad + 1.53f);
                targetPosition.z += test * sin(targetYawRad + 1.53f);

                Vec2 targetAngle = mc.getLocalPlayer()->getPosition()->CalcAngle(targetPosition).normAngles();
                localPlayer->rotationComponent->rotation.x = targetAngle.x;
                localPlayer->rotationComponent->rotation.y = targetAngle.y + PredictSpeed;
            }
            else {
                localPlayer->rotationComponent->rotation.x = ange.x;
                localPlayer->rotationComponent->rotation.y = ange.y;
            }
        }
        else if (rots == 4) { // Modo FrontStrafe
            if (DistT0 < DistPredict) {
                Vec3 targetPosition = *targetListJ[0]->getPosition();
                float targetYaw = targetListJ[0]->rotationComponent->rotation.y;
                float targetYawRad = targetYaw * (M_PI / 180.0f);

                targetPosition.x += test * cos(targetYawRad - 1.53f);
                targetPosition.z += test * sin(targetYawRad - 1.53f);

                Vec2 targetAngle = mc.getLocalPlayer()->getPosition()->CalcAngle(targetPosition).normAngles();
                localPlayer->rotationComponent->rotation.x = targetAngle.x;
                localPlayer->rotationComponent->rotation.y = targetAngle.y + PredictSpeed;
            }
            else {
                localPlayer->rotationComponent->rotation.x = ange.x;
                localPlayer->rotationComponent->rotation.y = ange.y;
            }
        }

        std::random_device rd;
        std::mt19937 gen(rd());

        if (targetmode == 1) {
            for (auto& i : targetListJ) {
                if (!(i->hurtTime > 8 && hurttime)) {
                    for (int j = 0; j < multiplier; ++j) {
                        std::uniform_int_distribution<int> dis(1, 100);
                        int randomHit = dis(gen);

                        if (randomizeHit) {
                            if (randomHit <= hitChance) {
                                if (!noSwing) mc.getLocalPlayer()->swing();
                                mc.getGameMode()->attack(i);
                                if (!noSwing) mc.getLocalPlayer()->swing();
                            }
                        }
                        else {
                            if (hitChance >= 100 || randomHit <= hitChance) {
                                if (!noSwing) mc.getLocalPlayer()->swing();
                                mc.getGameMode()->attack(i);
                                if (!noSwing) mc.getLocalPlayer()->swing();
                            }
                        }
                    }
                }
            }
        }
        else if (targetmode == 0) {
            if (!(targetListJ[0]->hurtTime > 8 && hurttime)) {
                for (int j = 0; j < multiplier; ++j) {
                    std::uniform_int_distribution<int> dis(1, 100);
                    int randomHit = dis(gen);

                    if (randomizeHit) {
                        if (randomHit <= hitChance) {
                            if (!noSwing) mc.getLocalPlayer()->swing();
                            mc.getGameMode()->attack(targetListJ[0]);
                            if (!noSwing) mc.getLocalPlayer()->swing();
                        }
                    }
                    else {
                        if (hitChance >= 100 || randomHit <= hitChance) {
                            if (!noSwing) mc.getLocalPlayer()->swing();
                            mc.getGameMode()->attack(targetListJ[0]);
                            if (!noSwing) mc.getLocalPlayer()->swing();
                        }
                    }
                }
            }
        }

        if (shouldSwitch && switchMode == 2) {
            plrInv->selectedSlot = oldSlot;
        }

        Odelay = 0;
    }
}

void Killaura::onEnable() {
    if (mc.getLocalPlayer() == nullptr)
        this->setEnabled(false);
}

void Killaura::onDisable() {
    targetListJ.clear();
}

void Killaura::onSendPacket(Packet* packet, bool& shouldcancel) {
    if (packet->getId() == PacketID::MovePlayerPacket && mc.getLocalPlayer() != nullptr && rots == 1) {
        if (!targetListJ.empty()) {
            auto* movePacket = reinterpret_cast<MovePlayerPacket*>(packet);
            Vec2 angle = mc.getLocalPlayer()->getPosition()->CalcAngle(*targetListJ[0]->getPosition());
            movePacket->headYaw = angle.y;
            movePacket->rotation = angle.y;

            if (packet->getId() == PacketID::MovePlayerPacket) {
                auto* MovePlayer = reinterpret_cast<MovePlayerPacket*>(packet);
                MovePlayer->onGround = true;
            }
        }
    }
}

void Killaura::onImGuiRender(ImDrawList* d) {
    if (mc.getClientInstance()->getLevelRenderer() == nullptr) return;
    if (mc.getClientInstance()->getLevelRenderer()->levelRendererPlayer == nullptr) return;
    if (!mc.canUseMoveKeys()) return;

    LocalPlayer* localPlayer = mc.getLocalPlayer();
    ImGuiIO& io = ImGui::GetIO();
    if (targetVisualize) {
        if (!targetListJ.empty() && targetListJ[0] != nullptr) {
            static unsigned int anim = 0;
            anim++;
            float height = targetListJ[0]->aabbComponent->aabb.upper.y - targetListJ[0]->aabbComponent->aabb.lower.y;
            const float coolAnim = (height / 2.f) + (height / 2.f) * sin(((float)anim / 60.f) * PI * 0.8f);

            Vec3<float> tgPos = targetListJ[0]->stateVectorComponent->pos;
            if (targetListJ[0]->isPlayer()) tgPos.y -= 1.6f;

            std::vector<Vec2<float>> pointsList;
            for (int i = 0; i < 360; i += 10) {
                float calcYaw = (i + 90) * (PI / 180);
                float x = cos(calcYaw) * 0.7f;
                float z = sin(calcYaw) * 0.7f;
                Vec2<float> pointsVec2;
                if (ImGuiUtils::worldToScreen(tgPos.add(x, coolAnim, z), pointsVec2)) {
                    pointsList.push_back(pointsVec2);
                }
            }
            for (int i = 0; i < pointsList.size(); i++) {
                int next = i + 1;
                if (next == pointsList.size()) next = 0;
                d->AddLine(pointsList[i].toImVec2(), pointsList[next].toImVec2(), visualizeColor.toImColor(), 2.f);
            }
        }
    }
}
