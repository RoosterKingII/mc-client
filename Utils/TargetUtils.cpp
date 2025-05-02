#include "TargetUtils.h"
#include "../Utils/Math.h"
#include "../SDK/GameData.h"
#include "../Client/Client.h"
#include <string>
#include "../Client/ModuleManager/Modules/Player/Antibot.h"
#include "../Client/ModuleManager/Modules/Player/NoFriends.h"

bool TargetUtils::isTargetValid(Actor* target, bool isMob) {
    static Antibot* bot = (Antibot*)client->moduleMgr->getModule("AntiBot");
    static NoFriends* noFriends = (NoFriends*)client->moduleMgr->getModule("NoFriends");
    
    LocalPlayer* localPlayer = mc.getLocalPlayer();
    if (localPlayer == nullptr) return false;
    if (!localPlayer->canAttack(target, false)) return false;
    if (target == nullptr) return false;
    if (target == localPlayer) return false;
    if (!target->isAlive()) return false;

    if (bot->isEnabled()) {
        if (target->getNameTag()->length() <= 3 || target->getNameTag()->find("game selector") != std::string::npos) return false;
        if (target->aabbComponent->width >= 0.70f && target->aabbComponent->width <= 0.80f && target->aabbComponent->height >= 2.0f && target->aabbComponent->height <= 2.2f) return false;
    }

    // Solo verificamos amigos si NoFriends está desactivado
    if (!noFriends->isEnabled()) {
        std::string targetName = Utils::sanitize(*target->getNameTag());
        if (std::find(Friend.begin(), Friend.end(), targetName) != Friend.end()) 
            return false;
    }

    const int entId = target->getEntityTypeId();

    const std::vector<int> invalidEntityIds = {
        0,     // unknown
        64,    // item
        66,    // falling block
        69,    // xp orb
        70,    // enderEye
        95,    // falling block
        4194372, // exp bottle
        4194390, // potion
        4194405, // still potion
        4194391, // ender pearl
        12582992 // arrow???
    };

    if (std::find(invalidEntityIds.begin(), invalidEntityIds.end(), entId) != invalidEntityIds.end()) 
        return false;

    if (!isMob) {
        if (!target->isPlayer()) return false;
        if (entId != 319) return false;
        if (!target->canShowNameTag()) return false;
    }

    return true;
}

bool TargetUtils::isItemValid(Actor* target, bool isMob) {
    static ESP* esp = (ESP*)client->moduleMgr->getModule("ESP");
    LocalPlayer* localPlayer = mc.getLocalPlayer();
    if (localPlayer == nullptr) return false;
    if (!localPlayer->canAttack(target, false)) return false;
    if (target == nullptr) return false;
    if (target == localPlayer) return false;
    if (esp->mobs == false) {
        if (target->aabbComponent->width != 0.25f && target->aabbComponent->height != 0.25f) return false;
    }
    if (!target->isAlive()) return false;

    const int entId = target->getEntityTypeId();

    if (!isMob) {
        if (!target->isPlayer()) return false;
        if (entId != 319) return false;
        if (!target->canShowNameTag()) return false;
    }

    return true;
}

bool TargetUtils::isFriendValid(Actor* target, bool isMob) {
    static Antibot* bot = (Antibot*)client->moduleMgr->getModule("AntiBot");
    static NoFriends* noFriends = (NoFriends*)client->moduleMgr->getModule("NoFriends");
    
    LocalPlayer* localPlayer = mc.getLocalPlayer();
    if (localPlayer == nullptr) return false;
    if (!localPlayer->canAttack(target, false)) return false;
    if (target == nullptr) return false;
    if (target == localPlayer) return false;
    if (!target->isAlive()) return false;

    if (bot->isEnabled()) {
        if (target->getNameTag()->length() <= 3 || target->getNameTag()->find("game selector") != std::string::npos) return false;
        if (target->aabbComponent->width >= 0.70f && target->aabbComponent->width <= 0.80f && target->aabbComponent->height >= 2.0f && target->aabbComponent->height <= 2.2f) return false;
    }

    // Solo verificamos amigos si NoFriends está desactivado
    if (!noFriends->isEnabled()) {
        std::string targetName = Utils::sanitize(*target->getNameTag());
        if (std::find(Friend.begin(), Friend.end(), targetName) != Friend.end()) 
            return false;
    }

    const int entId = target->getEntityTypeId();

    const std::vector<int> invalidEntityIds = {
        0,     // unknown
        64,    // item
        66,    // falling block
        69,    // xp orb
        70,    // enderEye
        95,    // falling block
        4194372, // exp bottle
        4194390, // potion
        4194405, // still potion
        4194391, // ender pearl
        12582992 // arrow???
    };

    if (std::find(invalidEntityIds.begin(), invalidEntityIds.end(), entId) != invalidEntityIds.end()) 
        return false;

    if (!isMob) {
        if (!target->isPlayer()) return false;
        if (entId != 319) return false;
        if (!target->canShowNameTag()) return false;
    }

    return true;
}

bool TargetUtils::sortByDist(Actor* a1, Actor* a2) {
    Vec3<float> lpPos = *mc.getLocalPlayer()->getPosition();
    return ((a1->getPosition()->dist(lpPos)) < (a2->getPosition()->dist(lpPos)));
}
