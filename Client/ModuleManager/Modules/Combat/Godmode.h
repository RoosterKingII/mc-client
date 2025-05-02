#pragma once
#include "../Module.h"
#include <vector>

class Godmode : public Module {
public:
    Godmode();
    ~Godmode();


    void findTargets();

    void onSendPacket(Packet* packet, bool &shouldCancel) override;

    void onNormalTick(Actor* actor) override;
    void onEnable() override;
    void onDisable() override;

private:

    std::vector<Actor*> targetListJ; // Lista de alvos

    // Configurações do Godmode
    float range = 2.0f;             // Alcance padrão para alvos
    int delay = 0;                  // Intervalo de ataque em ticks
    int numbeRclick = 0;                  // Intervalo de ataque em ticks
    int loop1 = 0;                  // Intervalo de ataque em ticks
    int multiplier2 = 1;             // Número de ataques por alvo
    float smoothness = 0.1f;        // Controle da suavidade da rotação
    int hitChance = 100;            // Probabilidade de acertar (1-100)
    bool randomizeHit = false;      // Aleatorização na chance de acertar
    int targetmode = 0;             // Modo de alvo (Single ou Multi)
    int test = 0;                   // Configuração de previsão para rotação
    bool isMulti = false;           // Habilitar ataques múltiplos por alvo
    int switchMode = 0;             // Controle de troca automática de armas
    bool hurttime = false;          // Verificação de tempo de invulnerabilidade
    bool isMobAura = false;         // Habilitar ataque em mobs (MobAura)
    int Odelay = 0;                 // Contador de delay
    int rots = 0;                   // Modo de rotação (None, Silent, Strafe, Predict)
};
