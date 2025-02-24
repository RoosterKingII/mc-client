#pragma once
#include "../Module.h"
#include <vector>

class Killaura : public Module {
public:
    Killaura();
    ~Killaura();

    void onNormalTick(Actor* actor) override;
    void onEnable() override;
    void onDisable() override;
    void onSendPacket(Packet* packet, bool& shouldcancel) override;

    void onImGuiRender(ImDrawList* d) override;

private:
    int getBestWeaponSlot();    // Seleciona o melhor slot com a arma mais poderosa
    void findEntityhhD();       // Encontra entidades v�lidas pr�ximas
    void sortByDist();          // Ordena a lista de alvos pela dist�ncia ao jogador local

    std::vector<Actor*> targetListJ; // Lista de alvos

    // Configura��es do Killaura
    float range = 2.0f;    
    bool visualRange = false;
    UIColor vRColor = UIColor(255, 255, 255, 255);
    bool targetVisualize = false;
    UIColor visualizeColor = UIColor(255, 255, 255, 255);// Alcance padr�o para alvos
    int delay = 0;                  // Intervalo de ataque em ticks
    int multiplier = 1;             // N�mero de ataques por alvo
    int thePredict = 8;             // N�mero de ataques por alvo
    float smoothness = 0.1f;        // Controle da suavidade da rota��o
    float hookY = 0.1f;        // Controle da suavidade da rota��o
    float PredictSpeed = 0.1f;        // Controle da suavidade da rota��o
    int hitChance = 100;            // Probabilidade de acertar (1-100)
    int DistPredict = 20;            // Probabilidade de acertar (1-100)
    bool randomizeHit = false;      // Aleatoriza��o na chance de acertar
    int targetmode = 0;             // Modo de alvo (Single ou Multi)
    int test = 0;                   // Configura��o de previs�o para rota��o
    bool isMulti = false;           // Habilitar ataques m�ltiplos por alvo
    int switchMode = 0;             // Controle de troca autom�tica de armas
    bool hurttime = false;          // Verifica��o de tempo de invulnerabilidade
    bool isMobAura = false;         // Habilitar ataque em mobs (MobAura)
    int Odelay = 0;                 // Contador de delay
    int rots = 0;                   // Modo de rota��o (None, Silent, Strafe, Predict)
};