#include "emote.h"

Emote::Emote() : Module("Emote", "Spams emotes automatically", Category::PLAYER) {
    addSlider("Delay", "Delay between emotes", ValueType::INT_T, &delay, 0, 15);
	addBoolCheck("togle", "NULL", &lol);
}

void Emote::onNormalTick(Actor* actor) {
    // Verificar si el jugador local existe
    LocalPlayer* localPlayer = mc.getLocalPlayer();
    if (!localPlayer) return;

    tickDelay++;
    if (tickDelay > delay) {
        // Reiniciar el índice si llegamos al final de la lista
        if (currentEmoteIndex >= emoteUUIDs.size()) {
            currentEmoteIndex = 0;
        }

        // Usar el método playEmote directamente desde la clase Player
        // El segundo parámetro (false) indica que no es un emote personalizado
        localPlayer->playEmote(emoteUUIDs[currentEmoteIndex], lol);

        // Avanzar al siguiente emote y reiniciar el contador de ticks
        currentEmoteIndex++;
        tickDelay = 0;
    }
}
