#include "DamageText.h"

DamageText::DamageText() : Module("DamageText", "Hitbox for players and mobs", Category::MISC) {
    addBoolCheck("Mobs", "NULL", &mobs);
}

void DamageText::onNormalTick(Actor* actor) {
    LocalPlayer* localPlayer = mc.getLocalPlayer();

    if (localPlayer == nullptr) return;
    if (!mc.canUseMoveKeys()) return;
    Level* level = localPlayer->getLevel();
    if (level == nullptr) return;

    DamageList.clear();

    for (Actor* ent : level->getRuntimeActorList()) {
        bool isValid = TargetUtils::isTargetValid(ent, mobs);
        if (isValid) DamageList.push_back(ent);
    }

    // Controle de dano dos alvos válidos
    for (Actor* ent : DamageList) {
        if (ent->hurtTime > 8) { // Verificar hurtTime do ator válido
            std::string playerName = *ent->getNameTag(); // Obter nome do jogador
            mc.DisplayClientMessage("%sEnemy Damaged:%s %s", RED, WHITE, playerName.c_str());
        }
    }
}