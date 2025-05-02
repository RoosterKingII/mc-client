#include "Origem.h"

const float heightOffset = 0.70f; // Defina o offset de altura

Origem::Origem() : Module("Criticals", "Keeps the player 0.25 blocks above.", Category::COMBAT) {
}

std::string Origem::getModName() {
    static char textStr[15];
    sprintf_s(textStr, 15, "-");
    return std::string(textStr);
}

void Origem::onSendPacket(Packet* packet, bool& shouldCancel) {
    // Checa se o pacote � de movimenta��o
    if (packet->getName() == "PlayerAuthInputPacket") {
        PlayerAuthInputPacket* paip = static_cast<PlayerAuthInputPacket*>(packet);

        // Aumenta a posi��o Y em 0.25 blocos acima da posi��o atual
        paip->position.y += heightOffset;

    }
    else if (packet->getName() == "MovePlayerPacket") {
        MovePlayerPacket* mpp = static_cast<MovePlayerPacket*>(packet);

        // Aumenta a posi��o Y em 0.25 blocos acima da posi��o atual
        mpp->position.y += heightOffset;
    }
}
