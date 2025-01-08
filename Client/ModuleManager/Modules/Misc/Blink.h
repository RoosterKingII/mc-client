#pragma once

#include "../Module.h"

class Blink : public Module {
public:
    Blink();

    void onSendPacket(Packet* packet, bool& shouldCancel) override;
    void sendNormalBlinkPacket(Packet* packet);
private:
    bool spam = false;
    std::vector<Packet*> packets;
    std::chrono::steady_clock::time_point lastToggleTime; // Zamanlayýcý için
};