#include "Speed.h"
#include <cmath> // cos, sin fonksiyonlar� i�in

Speed::Speed() : Module("Speed", "Increases movement speed", Category::MOVEMENT) {
    addSlider<float>("Speed", "Speed multiplier", ValueType::FLOAT_T, &speed, 1.0f, 5.0f); // 1x ila 5x h�z
}

void Speed::onNormalTick(Actor* actor) {
    auto player = mc.getLocalPlayer();
    if (!player)
        return;

    // Bile�enlere eri�im
    auto stateVec = player->stateVectorComponent;
    auto rotVec = player->rotationComponent;
    if (!stateVec || !rotVec)
        return;

    auto bodyRot = rotVec->rotation; // Oyuncunun d�nd��� a��
    float yaw = bodyRot.y;          // Y y�n�ndeki rotasyon (yaw)

    // Hareket giri�leri
    bool forward = mc.isKeyDown('A');
    bool backward = mc.isKeyDown('D');
    bool left = mc.isKeyDown('S');
    bool right = mc.isKeyDown('W');

    // Y�n hesaplama
    if (forward) {
        yaw += (left ? -45.f : (right ? 45.f : 0.f)); // W + A veya W + D kontrol�
    }
    else if (backward) {
        yaw += (left ? -135.f : (right ? 135.f : 180.f)); // S + A veya S + D kontrol�
    }
    else if (!forward && !backward && (left || right)) {
        yaw += (left ? -90.f : 90.f); // Yaln�zca A veya D tu�u
    }
    else {
        return; // Hi�bir tu�a bas�lmam��sa i�lem yapma
    }

    // Hareket vekt�r�
    float radianYaw = yaw * (3.14f / 180.f); // Dereceyi radyana �evirme
    auto motion = Vec3<float>(
        cos(radianYaw) * this->speed, // X ekseni hareketi
        stateVec->velocity.y,         // Y ekseni korunur (d��me/atlama)
        sin(radianYaw) * this->speed  // Z ekseni hareketi
    );

    // Hareketi uygula
    player->lerpMotion(motion); // Oyuncunun h�z�n� ayarla
}
