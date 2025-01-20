#include "emote.h"

Emote::Emote() : Module("Emote", "Spam emotes automatically", Category::PLAYER) {
    addSlider<int>("Delay", "Delay between emotes", ValueType::INT_T, &delay, 0, 15);
}

void Emote::onNormalTick(Actor* actor) {
    if (!actor) return;

    LocalPlayer* player = mc.getLocalPlayer();
    if (!player) return;

    tickDelay++;
    if (tickDelay > delay) {
        // Generador de números aleatorios
        std::random_device rd;
        std::mt19937 gen(rd());
        std::uniform_int_distribution<> dis(0, emoteUUIDs.size() - 1);

        // Seleccionar un índice aleatorio
        int randomIndex = dis(gen);

        // Reproducir el emote aleatorio
        player->playEmote(emoteUUIDs[randomIndex], true);
        tickDelay = 0;
    }
}

void Emote::onRender(ImDrawList* drawlist) {
    if (!mc.canUseMoveKeys()) return;
}