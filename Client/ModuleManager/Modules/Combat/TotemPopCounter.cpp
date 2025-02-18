#include "TotemPopCounter.h"

TotemPopCounter::TotemPopCounter() : Module("TotemPopCounter", "Counts totem pops for each player", Category::COMBAT) {
    addBoolCheck("Send Chat", "Send pop messages to chat", &sendChat);
}

TotemPopCounter::~TotemPopCounter() {
}

void TotemPopCounter::onEnable() {
    popList.clear();
    totemEquipped.clear();
}

void TotemPopCounter::onDisable() {
    popList.clear();
    totemEquipped.clear();
}

void TotemPopCounter::onNormalTick(Actor* actor) {
    if (!actor) return;
    
    std::vector<Actor*> actors = actor->getLevel()->getRuntimeActorList();
    
    for (auto currentEntity : actors) {
        if (!currentEntity) continue;
        if (currentEntity == actor) continue;
        if (!currentEntity->isAlive()) continue;
        
        std::string playerName = *currentEntity->getNameTag();
        
        // Inicializar el contador y estado del tótem si no existe
        if (popList.find(playerName) == popList.end()) {
            popList[playerName] = 0;
            totemEquipped[playerName] = false;
        }
        
        // Verificar si tiene tótem equipado
        ItemStack* currentTotem = currentEntity->getEquippedTotem();
        
        if (!currentTotem || currentTotem->item == nullptr) {
            if (!totemEquipped[playerName]) {
                ItemStack* chestplate = currentEntity->getArmor(1);
                if (chestplate && chestplate->item != nullptr) {
                    popList[playerName]++;
                    
                    if (sendChat) {
                        std::string message = playerName + " popped " + 
                            std::to_string(popList[playerName]) + " totems!";
                        mc.DisplayClientMessage(message.c_str());
                    }
                    totemEquipped[playerName] = true;
                }
            }
        } else {
            totemEquipped[playerName] = false;
        }
    }
}
