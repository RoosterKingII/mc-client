#include "TPAura.h"
#include <random> // Rastgele pozisyonlar için

TPAura::TPAura() : Module("TPAura", "Attack targets with teleport bypass", Category::COMBAT) {
    addSlider<float>("Range", "Maximum range", ValueType::FLOAT_T, &range, 3.f, 10.f); // Hedefleme mesafesi
    addSlider<float>("Speed", "Movement speed", ValueType::FLOAT_T, &speed, 0.1f, 1.5f); // Hareket hýzý
    addSlider<int>("Points", "Teleport points", ValueType::INT_T, &teleportPoints, 2, 8); // Rastgele nokta sayýsý
    addBoolCheck("Mobs", "Target mobs", &targetMobs); // Moblarý hedef al
    addBoolCheck("Players", "Target players", &targetPlayers); // Oyuncularý hedef al
}

void TPAura::onNormalTick(Actor* actor) {
    auto localPlayer = mc.getLocalPlayer();
    if (!localPlayer)
        return;

    Level* level = localPlayer->getLevel();
    if (!level)
        return;

    // Hedef listesini temizle
    targetList.clear();

    // Varlýklarý kontrol et
    for (Actor* ent : level->getRuntimeActorList()) {
        if (ent == nullptr || ent == localPlayer) continue;

        bool isValidTarget = TargetUtils::isTargetValid(ent, targetMobs || targetPlayers);
        if (!isValidTarget) continue;

        // `getPosition()` fonksiyonu pointer döndürüyor mu? Onu kontrol et.
        float distance = localPlayer->getPosition()->distanceTo(ent->getPosition()->toFloat());
        if (distance > range) continue;

        targetList.push_back(ent);
    }

    if (targetList.empty()) return;

    // Rastgele hedef seç
    Actor* target = targetList[std::rand() % targetList.size()];

    // Rastgele noktalar oluþtur
    std::vector<Vec3<float>> teleportPositions;
    Vec3<float> targetPos = *target->getPosition(); // Bu, Vec3<float> olmalý
    for (int i = 0; i < teleportPoints; ++i) {
        float angle = (float(i) / teleportPoints) * 2.f * 3.14159f; // Daire çevresinde konum
        float offsetX = cos(angle) * range * (0.5f + (std::rand() % 50) / 100.f); // 50% ile 100% arasý rastgele
        float offsetZ = sin(angle) * range * (0.5f + (std::rand() % 50) / 100.f);
        teleportPositions.push_back(Vec3<float>(targetPos.x + offsetX, targetPos.y, targetPos.z + offsetZ));
    }

    // Rastgele bir pozisyona ilerle
    if (!teleportPositions.empty()) {
        Vec3<float> teleportTarget = teleportPositions[std::rand() % teleportPositions.size()];

        // Hedefin pozisyonuna doðru ilerle
        float deltaX = teleportTarget.x - localPlayer->getPosition()->x;
        float deltaY = teleportTarget.y - localPlayer->getPosition()->y;
        float deltaZ = teleportTarget.z - localPlayer->getPosition()->z;

        // Hareket vektörünü hesapla
        Vec3<float> movementVector(deltaX, deltaY, deltaZ);
        movementVector = movementVector.normalized() * speed;

        // Y eksenini, oyuncunun mevcut y hýzýný koruyacak þekilde ayarla
        movementVector.y = localPlayer->stateVectorComponent->velocity.y;

        // Hareketi uygula
        localPlayer->lerpMotion(movementVector);
    }

    // Hedefe saldýr
    if (localPlayer->getPosition()->distanceTo(targetPos) < 2.0f) { // Yaklaþýnca saldýr
        localPlayer->swing();
        mc.getGameMode()->attack(target);
    }
}