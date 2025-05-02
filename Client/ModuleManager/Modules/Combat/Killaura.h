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

    std::string getModName();

private:
    int getBestWeaponSlot();    // Retorna el slot con el arma que tenga mayor daño
    void findEntityhhD();       // Encuentra entidades válidas (targets)
    void sortByDist();          // Ordena la lista de alvos por distancia al jugador
    int getTridentSlot();       // Retorna el slot donde se encuentre un tridente (usando texture_name=="trident")

    std::vector<Actor*> targetListJ; // Lista de alvos

    // Configuraciones del Killaura
    float range = 2.0f;
    bool visualRange = false;
    UIColor vRColor = UIColor(255, 255, 255, 255);
    bool targetVisualize = false;
    UIColor visualizeColor = UIColor(255, 255, 255, 255);
    int delay = 0;                  // Intervalo (en ticks) entre ataques
    int multiplier = 1;             // Número de ataques por alvo
    int thePredict = 8;
    float smoothness = 0.1f;
    float hookY = 0.1f;
    float PredictSpeed = 0.1f;
    int hitChance = 100;
    int DistPredict = 20;
    bool randomizeHit = false;
    int targetmode = 0;             // 0: Single, 1: Multi
    int test = 0;
    bool isMulti = false;
    int switchMode = 0;             // 0: None, 1: Full, 2: Silent
    bool hurttime = false;
    bool isMobAura = false;
    int Odelay = 0;
    int rots = 0;                   // 0: None, 1: Silent, 2: Strafe, 3: Predict, 4: FrontStrafe
    bool noSwing = false;

    // NUEVO SETTING: Activa el auto cambio al tridente cuando el jugador está en agua
    bool autoTrident = false;
};
