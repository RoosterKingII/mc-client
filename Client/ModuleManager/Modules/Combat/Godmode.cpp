#include "Godmode.h"
#include "../../../Client.h"

Godmode::Godmode() : Module("GodMode", "godmode for lifeboat.", Category::COMBAT) {
    addSlider<int>("Multiplier", "Set the strenght OF godmode packets, maybe you can get kick.", ValueType::INT_T, &this->multiplier2, 0, 4);
    addSlider<int>("Packets Clicks", "Select the value of extra Click Packets", ValueType::INT_T, &this->numbeRclick, 0, 40);
}

Godmode::~Godmode() {
}


static std::vector<Actor*> targetList;

void Godmode::findTargets() {
    auto localPlayer = mc.getLocalPlayer();
    auto level = localPlayer->getLevel();

    if (!level) return;

    targetList.clear();
    for (auto& target : level->getRuntimeActorList()) {
        float dist = target->stateVectorComponent->pos.dist(localPlayer->stateVectorComponent->pos);
        if (dist < 0.01f) { // Apenas jogadores em um raio de 100 blocos
            targetList.push_back(target);
        }
    }
}

void Godmode::onSendPacket(Packet* packet, bool &shouldCancel) {
    auto* mpp = (MovePlayerPacket*)packet;
    if (packet->getName() == "MovePlayerPacket") {
        mpp->onGround = true;
    }
}
void Godmode::onNormalTick(Actor* actor) {
    auto localPlayer = mc.getLocalPlayer();
    if (!localPlayer) return;

    findTargets();

    if (!targetList.empty()) {

        Vec3 targetPos = targetList[0]->stateVectorComponent->pos.sub(Vec3(0.f, 0.f, 0.f)); // Posição ajustada
        InteractPacket inter(InteractAction::LEFT_CLICK, targetList[0]->getRuntimeID(), targetPos);
        for (int i = 0; i < numbeRclick; ++i) {
            mc.getClientInstance()->loopbackPacketSender->sendToServer(&inter);
        }

        for (int i = 0; i < multiplier2; ++i) { // Ataque múltiplo baseado no multiplicador
            mc.getLocalPlayer()->swing(); // Realiza o ataque
            mc.getGameMode()->attack(targetList[0]); // Ataca o primeiro alvo encontrado

        }
    }
}

void Godmode::onEnable() {
    if (mc.getLocalPlayer() == nullptr)
        this->setEnabled(false);
}

void Godmode::onDisable() {
    targetList.clear();
}

