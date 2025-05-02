#include "Blink.h"
#include <chrono>
#include <thread>

Blink::Blink() : Module("Blink", "Blink", Category::MISC), spam(false) {
    addBoolCheck("Spam", "spamming blink", &spam);
    lastToggleTime = std::chrono::steady_clock::now(); // Baþlangýç zamanýný ayarla
}

void Blink::onSendPacket(Packet* packet, bool& shouldCancel) {
    auto currentTime = std::chrono::steady_clock::now();
    auto elapsedTime = std::chrono::duration_cast<std::chrono::milliseconds>(currentTime - lastToggleTime).count();


    if (spam) {
        if (packet->getId() == PacketID::PlayerAuthInput || packet->getId() == PacketID::MovePlayerPacket) {
            if (elapsedTime < 100) {
                shouldCancel = true; // Paket iptal edilecek
            }
            else {
                shouldCancel = false; // Paket gönderilecek
                lastToggleTime = currentTime; // Zamanlayýcýyý güncelle
            }
        }
    }
    else {
        if (packet->getId() == PacketID::PlayerAuthInput || packet->getId() == PacketID::MovePlayerPacket) {
            shouldCancel = true; // Normal iptal
        }
    }
}