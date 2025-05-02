#include "LbReach.h"
#include "../Render/NameTags.h"
#include <Windows.h>
#include <chrono>
#include <thread>
#include <cmath>

LifeboatReach::LifeboatReach() : Module("Hit-Reach", "Hit Ur Enemys from 20 Blocks max. ( NEED NAMETAGS ACTIVE TO WORKS ) Press ''F'' To Hit. ", Category::COMBAT) {
    // Removido o modo "TimerAndTP"
    addEnumSetting("Mode", "NULL", { "Teleport" }, &Mode);
}

float lerpto(float endPoint, float current, float speed) {
    if (speed < 0.0) speed = 0.0;
    else if (speed > 1.0) speed = 1.0;

    float dif = std::max(endPoint, current) - std::min(endPoint, current);
    float factor = dif * speed;
    return current + (endPoint > current ? factor : -factor);
}

bool areMovementKeysPressed() {
    return (GetAsyncKeyState('W') & 0x8000) || (GetAsyncKeyState('A') & 0x8000) ||
        (GetAsyncKeyState('S') & 0x8000) || (GetAsyncKeyState('D') & 0x8000);
}

void LifeboatReach::onSendPacket(Packet* packet, bool& shouldCancel) {
    static auto lastActionTime = std::chrono::steady_clock::now();
    const std::chrono::milliseconds tpStepDelay(50);  // Her ad�m i�in 50 ms bekleme s�resi
    const float tpStepDistance = 15.0f;  // Her ad�mda gidilecek mesafe

    // Sol t�klama bas�l�yken i�lemler
    if (mc.isLeftClickDown()) {
        auto localPlayer = mc.getLocalPlayer();
        if (!localPlayer) return;

        // En yak�n hedefi bulma
        Actor* closestActor = nullptr;
        float minDistance = 100.0f;

        for (Actor*& actor : localPlayer->getLevel()->getRuntimeActorList()) {
            if (TargetUtils::isTargetValid(actor, false)) {
                float currentDist = actor->getEyePos().dist(localPlayer->getEyePos());
                if (currentDist < minDistance) {
                    minDistance = currentDist;
                    closestActor = actor;
                }
            }
        }

        // TP i�lemi ve sald�r�
        if (closestActor && packet->getName() == "PlayerAuthInputPacket") {
            PlayerAuthInputPacket* paip = static_cast<PlayerAuthInputPacket*>(packet);
            auto startPosition = localPlayer->getPosition();
            auto targetPosition = closestActor->getPosition();

            // Toplam mesafeyi 15 blokluk ad�mlara b�leriz
            int numSteps = static_cast<int>(minDistance / tpStepDistance);
            float dx = (targetPosition->x - startPosition->x) / numSteps;
            float dy = (targetPosition->y - startPosition->y) / numSteps;
            float dz = (targetPosition->z - startPosition->z) / numSteps;

            for (int i = 0; i < numSteps; i++) {
                // Her ad�mda pozisyonu ayarlar
                paip->position.x = startPosition->x + dx * i;
                paip->position.y = startPosition->y + dy * i;
                paip->position.z = startPosition->z + dz * i;

                // Her ad�mda k�sa bir bekleme yapar
                std::this_thread::sleep_for(tpStepDelay);
            }

            // Hedefe ula�t���nda sald�r� yapar
            mc.getGameMode()->attack(closestActor);
            localPlayer->swing();

            // Hedefe ula�t�ktan sonra ayn� ad�mlarla geri d�n
            for (int i = numSteps; i >= 0; i--) {
                paip->position.x = startPosition->x + dx * i;
                paip->position.y = startPosition->y + dy * i;
                paip->position.z = startPosition->z + dz * i;
                std::this_thread::sleep_for(tpStepDelay);
            }
        }
    }
}
void LifeboatReach::onDisable() {
    mc.getClientInstance()->minecraft->mctimer->tps = 20.f;
}