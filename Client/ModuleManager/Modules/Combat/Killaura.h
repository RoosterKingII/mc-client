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
    void findEntityhhD();       // Encontra entidades válidas próximas
    void sortByDist();          // Ordena a lista de alvos pela distância ao jogador local

    std::vector<Actor*> targetListJ; // Lista de alvos

    // Configurações do Killaura
    float range = 2.0f;
    bool visualRange = false;
    UIColor vRColor = UIColor(255, 255, 255, 255);
    bool targetVisualize = false;
    UIColor visualizeColor = UIColor(255, 255, 255, 255);// Alcance padrão para alvos
    int delay = 0;                  // Intervalo de ataque em ticks
    int multiplier = 1;             // Número de ataques por alvo
    int thePredict = 8;             // Número de ataques por alvo
    float smoothness = 0.1f;        // Controle da suavidade da rotação
    float hookY = 0.1f;             // Controle da suavidade da rotação
    float PredictSpeed = 0.1f;      // Controle da suavidade da rotação
    int hitChance = 100;            // Probabilidade de acertar (1-100)
    int DistPredict = 20;           // Probabilidade de acertar (1-100)
    bool randomizeHit = false;      // Aleatorização na chance de acertar
    int targetmode = 0;             // Modo de alvo (Single ou Multi)
    int test = 0;                   // Configuração de previsão para rotação
    bool isMulti = false;           // Habilitar ataques múltiplos por alvo
    int switchMode = 0;             // Controle de troca automática de armas
    bool hurttime = false;          // Verificação de tempo de invulnerabilidade
    bool isMobAura = false;         // Habilitar ataque em mobs (MobAura)
    int Odelay = 0;                 // Contador de delay
    int rots = 0;                   // Modo de rotação (None, Silent, Strafe, Predict, FrontStrafe)
    bool noSwing = false;   
    std::string getModName();
};
