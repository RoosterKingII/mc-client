#include "Speed.h"
#include <cmath> // cos, sin fonksiyonlarý için

Speed::Speed() : Module("Speed", "Increases movement speed", Category::MOVEMENT) {
    addSlider<float>("Speed", "Speed multiplier", ValueType::FLOAT_T, &speed, 1.0f, 5.0f); // 1x ila 5x hýz
}

void Speed::onNormalTick(Actor* actor) {
    auto player = mc.getLocalPlayer();
    if (!player)
        return;

    // Bileþenlere eriþim
    auto stateVec = player->stateVectorComponent;
    auto rotVec = player->rotationComponent;
    if (!stateVec || !rotVec)
        return;

    auto bodyRot = rotVec->rotation; // Oyuncunun döndüðü açý
    float yaw = bodyRot.y;          // Y yönündeki rotasyon (yaw)

    // Hareket giriþleri
    bool forward = mc.isKeyDown('A');
    bool backward = mc.isKeyDown('D');
    bool left = mc.isKeyDown('S');
    bool right = mc.isKeyDown('W');

    // Yön hesaplama
    if (forward) {
        yaw += (left ? -45.f : (right ? 45.f : 0.f)); // W + A veya W + D kontrolü
    }
    else if (backward) {
        yaw += (left ? -135.f : (right ? 135.f : 180.f)); // S + A veya S + D kontrolü
    }
    else if (!forward && !backward && (left || right)) {
        yaw += (left ? -90.f : 90.f); // Yalnýzca A veya D tuþu
    }
    else {
        return; // Hiçbir tuþa basýlmamýþsa iþlem yapma
    }

    // Hareket vektörü
    float radianYaw = yaw * (3.14f / 180.f); // Dereceyi radyana çevirme
    auto motion = Vec3<float>(
        cos(radianYaw) * this->speed, // X ekseni hareketi
        stateVec->velocity.y,         // Y ekseni korunur (düþme/atlama)
        sin(radianYaw) * this->speed  // Z ekseni hareketi
    );

    // Hareketi uygula
    player->lerpMotion(motion); // Oyuncunun hýzýný ayarla
}
