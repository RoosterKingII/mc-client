#include "TotemPopCounter.h"
#include <set>

TotemPopCounter::TotemPopCounter() : Module("TotemPopCounter", "Counts totem pops for each player", Category::COMBAT) {
    addBoolCheck("Send Chat", "Send pop messages to chat", &sendChat);
    addBoolCheck("Client Only", "Only show messages to client", &clientOnly);
    addBoolCheck("Random String", "Add random string suffix", &randomString);
    addSlider<int>("String Length", "Length of random string", ValueType::INT_T, &stringLength, 1, 32);

    std::random_device rd;
    rng = std::mt19937(rd());
}
void sendTotemMessage(std::string str, bool isDeathMessage = false) {
    auto* pkt = reinterpret_cast<TextPacket*>(packet.get());
    mc.getClientInstance()->loopbackPacketSender->sendToServer(pkt);
}

std::string TotemPopCounter::generateRandomString(int length) {
    const std::string charset = "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789";
    std::uniform_int_distribution<int> dist(0, charset.length() - 1);
    std::string result;
    for (int i = 0; i < length; i++) {
        result += charset[dist(rng)];
    }
    return result;
}

void TotemPopCounter::onNormalTick(Actor* actor) {
    if (!actor) return;

    std::vector<Actor*> actors = actor->getLevel()->getRuntimeActorList();
    std::set<std::string> currentPlayers;

    for (auto currentEntity : actors) {
        if (!currentEntity) continue;
        if (currentEntity == actor) continue;
        if (!currentEntity->isAlive()) continue;

        if (!TargetUtils::isTargetValid(currentEntity, false)) continue;

        std::string playerName = *currentEntity->getNameTag();
        currentPlayers.insert(playerName);

        if (popList.find(playerName) == popList.end()) {
            popList[playerName] = 0;
            totemEquipped[playerName] = false;
        }

        ItemStack* currentTotem = currentEntity->getEquippedTotem();

        if (!currentTotem || currentTotem->item == nullptr) {
            if (!totemEquipped[playerName]) {
                ItemStack* chestplate = currentEntity->getArmor(1);
                if (chestplate && chestplate->item != nullptr) {
                    popList[playerName]++;

                    std::string suffix = randomString ? " | " + generateRandomString(stringLength) : "";

                    if (clientOnly) {
                        std::string colorMessage = std::string(RED) + playerName + std::string(WHITE) +
                            " popped " + std::string(GOLD) + std::string(BOLD) +
                            std::to_string(popList[playerName]) + std::string(RESET) +
                            std::string(WHITE) + " totems!" + suffix;
                        mc.DisplayClientMessage(colorMessage.c_str());
                    }

                    // Mensaje sin colores para sendTotemMessage
                    if (sendChat) {
                        std::string plainMessage = "> @" + playerName + " popped " +
                            std::to_string(popList[playerName]) + " totems! lol |SX1 ON TOP" + suffix;
                        sendTotemMessage(plainMessage);
                    }

                    totemEquipped[playerName] = true;
                }
            }
        }
        else {
            totemEquipped[playerName] = false;
        }
    }
    
    for (auto it = popList.begin(); it != popList.end(); ) {
        std::string playerName = it->first;

        if (currentPlayers.find(playerName) == currentPlayers.end() && it->second > 0) {
            std::string suffix = randomString ? " | " + generateRandomString(stringLength) : "";
            
            if (clientOnly) {
                std::string colorDeathMessage = std::string(RED) + playerName + std::string(WHITE) +
                    " died after popping " + std::string(GOLD) + std::string(BOLD) +
                    std::to_string(it->second) + std::string(RESET) + std::string(WHITE) +
                    " totems!" + suffix;
                mc.DisplayClientMessage(colorDeathMessage.c_str());
            }

            if (sendChat) {
                std::string plainDeathMessage ="> @" + playerName + " died after popping " +
                    std::to_string(it->second) + " totems! lol |SX1 ON TOP" + suffix;
                sendTotemMessage(plainDeathMessage, true);
            }

            it = popList.erase(it);
            totemEquipped.erase(playerName);
        }
        else {
            ++it;
        }
    }
}

void TotemPopCounter::onEnable() {
    popList.clear();
    totemEquipped.clear();
}

void TotemPopCounter::onDisable() {
    popList.clear();
    totemEquipped.clear();
}

TotemPopCounter::~TotemPopCounter() {
    popList.clear();
    totemEquipped.clear();
}
