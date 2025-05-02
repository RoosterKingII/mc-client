#pragma once
#include "../Module.h"

class FallCounter : public Module {
public:
    FallCounter();  // Construtor do m�dulo

    // Vari�veis para controlar a posi��o e o contador de queda
    float Ydd = -11.55;       // Dist�ncia de queda
    float Xdd = -66.51;       // Coordenada X (n�o sendo usada diretamente para queda, mas voc� pode expandir conforme necess�rio)
    float YposMC = 0;    // Posi��o Y do jogador (altura inicial)
    float fallPT = 0;    // Potencial de queda ou contador de dano de queda
    float Kaka23 = 0;    // Potencial de queda ou contador de dano de queda
    float inGuiLoopOp = 0.4;    // Potencial de queda ou contador de dano de queda

    // Override para renderizar elementos no ImGui (como a dist�ncia de queda ou alertas)
    void onImGuiRender(ImDrawList* drawlist) override;

    void onEnable();

    // M�todo chamado a cada tick (para controle do contador de queda)
    void onNormalTick(Actor* actor) override;

private:
};