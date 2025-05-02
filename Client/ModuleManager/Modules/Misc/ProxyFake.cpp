#include "ProxyFake.h"
#include "../../../../SDK/MCTextFormat.cpp"

ProxyFake::ProxyFake() : Module("Fake Proxy", "Spoof Svile proxy message In Boost.", Category::PLAYER) {
    addSlider<int>("DelayInTicks", "delay for the message to appear in ticks", ValueType::INT_T, &Ticks, 3, 5000);
    tickCounter = 0; // Inicialização do contador de tempo
}

void ProxyFake::onNormalTick(Actor* actor) {
    LocalPlayer* localPlayer = mc.getLocalPlayer();

    if (localPlayer == nullptr) return;

    // Incrementa o contador de ticks
    tickCounter++;

    // Verifica se já se passaram o número de ticks especificado
    if (tickCounter >= Ticks) {
        mc.DisplayClientMessage("\n \n%s%sTranslator proxy by svile\n%s Telegram: %s@svilex %sDiscord: %s@svile. \n \n",
            GRAY, CYAN, PURPLE, WHITE, PURPLE, WHITE); // Envia a mensagem no chat com cores
        tickCounter = 0; // Reseta o contador
    }
}

void ProxyFake::onEnable() {
    tickCounter = 0; // Reinicia o contador ao ativar o módulo
}

void ProxyFake::onDisable() {
    tickCounter = 0; // Reseta o contador ao desativar o módulo
}