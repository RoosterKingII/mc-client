#include "Fly2.h"
#include "../Combat/Killaura.h"
#include <cmath>

Fly2::Fly2() : Module("TestFly", "USE WITH DISABLER TO BYPASS FLYING IN LB.", Category::MOVEMENT) {
    addSlider<float>("Horizontal Speed", "Base movement speed", ValueType::FLOAT_T, &this->HorizontalSpeed, 1.00f, 5.25f);
    addSlider<float>("Vertical Speed", "Vertical movement speed", ValueType::FLOAT_T, &this->upanddown, 1.00f, 5.0f);
    addSlider<float>("Glide", "How fast you want to glide", ValueType::FLOAT_T, &glideMod, -0.2f, 1.0f);
  //  addSlider<int>("Distance Threshold", "Distance where speed reduction starts", ValueType::INT_T, &distanceThreshold, 900000, 900000);
    addSlider<float>("Min Speed Multiplier", "Minimum speed in far distances", ValueType::FLOAT_T, &speedMultiplierMin, 0.1f, 1.0f);
    addSlider<float>("Norm Strength", "Strength of speed normalization", ValueType::FLOAT_T, &normalizationStrength, 0.1f, 2.0f);
    addBoolCheck("SM Border", "Enable border limits", &smborder);
    addEnumSetting("Mode", "Flight mode", { "Motion", "Noclip" }, &Modefly);
}

void Fly2::onNormalTick(Actor* actor) {
    auto* pos = mc.getLocalPlayer()->getPosition();
    auto gm = mc.getGameMode();
    gm->player->stateVectorComponent->velocity = Vec3<float>(0, 0, 0);

    // Cálculo de normalización por distancia
    float distanceFromOrigin = sqrt(pos->x * pos->x + pos->z * pos->z);
    float speedMultiplier = 1.0f;

    if (distanceFromOrigin > distanceThreshold) {
        float distanceFactor = (distanceFromOrigin - distanceThreshold) / distanceThreshold;
        speedMultiplier = 1.0f - ((1.0f - speedMultiplierMin) * std::min(distanceFactor * normalizationStrength, 1.0f));
    }

    // Movimiento vertical y glide
    float glideModEffective = glideMod;
    if (mc.canUseMoveKeys()) {
        if (mc.isKeyDown(VK_SPACE)) {
            glideModEffective += upanddown;
        }
        if (mc.isKeyDown(VK_SHIFT)) {
            glideModEffective -= upanddown;
        }
    }

    // Border checks
    if (smborder) {
        if (pos->x > 1090) pos->x = 1090;
        if (pos->z > 1090) pos->z = 1090;
        if (pos->x < 60) pos->x = 60;
        if (pos->z < 60) pos->z = 60;
        if (pos->y > 155) pos->y = 155;
        mc.getLocalPlayer()->setPos(*pos);
    }

    gm->player->stateVectorComponent->velocity.y = glideModEffective;

    auto player = mc.getLocalPlayer();
    if (player == nullptr) return;

    bool w = mc.isKeyDown('W');
    bool a = mc.isKeyDown('A');
    bool s = mc.isKeyDown('S');
    bool d = mc.isKeyDown('D');

    float yaw = player->rotationComponent->Get().y;
    float radiansYaw = yaw * (PI / 180);

    Vec3<float> moveVec(0, 0, 0);

    // Cálculo de dirección base (sin multiplicar por velocidad)
    if (w) {
        moveVec.x -= sin(radiansYaw);
        moveVec.z += cos(radiansYaw);
    }
    if (a) {
        moveVec.x += cos(radiansYaw);
        moveVec.z += sin(radiansYaw);
    }
    if (s) {
        moveVec.x += sin(radiansYaw);
        moveVec.z -= cos(radiansYaw);
    }
    if (d) {
        moveVec.x -= cos(radiansYaw);
        moveVec.z -= sin(radiansYaw);
    }

    // Normalización del vector y aplicación de velocidad
    float length = sqrt(moveVec.x * moveVec.x + moveVec.z * moveVec.z);
    if (length > 0) {
        float finalSpeed = HorizontalSpeed * speedMultiplier;
        moveVec.x = (moveVec.x / length) * finalSpeed;
        moveVec.z = (moveVec.z / length) * finalSpeed;
    }

    moveVec.y = player->stateVectorComponent->velocity.y;

    // Aplicar movimiento según el modo
    if (Modefly == 0) {
        player->lerpMotion(moveVec);
    }
    else {
        pos->x += moveVec.x;
        pos->y += moveVec.y;
        pos->z += moveVec.z;
        player->setPos(*pos);
    }
}

void Fly2::onEnable() {
}

void Fly2::onDisable() {
    if (mc.getLocalPlayer() != nullptr) {
        // Reset velocities if needed
    }
}