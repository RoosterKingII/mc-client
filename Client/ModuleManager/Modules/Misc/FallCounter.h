#pragma once
#include "../Module.h"

class FallCounter : public Module {
public:
    FallCounter();  // Construtor do módulo

    // Variáveis para controlar a posição e o contador de queda
    float Ydd = -11.55;       // Distância de queda
    float Xdd = -66.51;       // Coordenada X (não sendo usada diretamente para queda, mas você pode expandir conforme necessário)
    float YposMC = 0;    // Posição Y do jogador (altura inicial)
    float fallPT = 0;    // Potencial de queda ou contador de dano de queda
    float Kaka23 = 0;    // Potencial de queda ou contador de dano de queda
    float inGuiLoopOp = 0.4;    // Potencial de queda ou contador de dano de queda

    // Override para renderizar elementos no ImGui (como a distância de queda ou alertas)
    void onImGuiRender(ImDrawList* drawlist) override;

    void onEnable();

    // Método chamado a cada tick (para controle do contador de queda)
    void onNormalTick(Actor* actor) override;

private:
};