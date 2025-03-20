// TotemPopCounter.cpp
#include "TotemPopCounter.h"

TotemPopCounter::TotemPopCounter() : Module("TotemPopCounter", "Counts totem pops for each player", Category::COMBAT) {
    addBoolCheck("Send Chat", "Send pop messages to chat", &sendChat);
    addBoolCheck("Client Only", "Only show messages to client", &clientOnly);
}

// Función auxiliar para enviar mensajes (similar a Spammer)
void sendTotemMessage(std::string str, bool clientOnly) {
    if (clientOnly) {
        mc.DisplayClientMessage(str.c_str());
    }
    else {
        std::shared_ptr<Packet> packet = MinecraftPackets::createPacket(PacketID::Text);
        auto* pkt = reinterpret_cast<TextPacket*>(packet.get());

        pkt->messageType = 1;
        pkt->message = str;
        pkt->userName = "";
        pkt->translationNeeded = false;

        mc.getClientInstance()->loopbackPacketSender->sendToServer(pkt);
    }
}

void TotemPopCounter::onNormalTick(Actor* actor) {
    if (!actor) return;

    std::vector<Actor*> actors = actor->getLevel()->getRuntimeActorList();

    for (auto currentEntity : actors) {
        if (!currentEntity) continue;
        if (currentEntity == actor) continue;
        if (!currentEntity->isAlive()) continue;

        // Añadimos la verificación de TargetUtils
        if (!TargetUtils::isTargetValid(currentEntity, false)) continue;

        std::string playerName = *currentEntity->getNameTag();

        // Inicializar el contador y estado del tótem si no existe
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

                    if (sendChat) {
                        std::string message =">" + playerName + " popped " +
                            std::to_string(popList[playerName]) + " totems! lol";
                        if (clientOnly) {
                            mc.DisplayClientMessage(message.c_str());
                        }
                        else {
                            sendTotemMessage(message, clientOnly);
                        }
                    }
                    totemEquipped[playerName] = true;
                }
            }
        }
        else {
            totemEquipped[playerName] = false;
        }
    }
}
