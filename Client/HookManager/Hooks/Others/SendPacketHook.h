
#pragma once
#include "../../../../Utils/MemoryUtils.h"
#include "../../../../SDK/Classes/LoopbackPacketSender.h"
#include "../../../Client.h"
class VerifyHook;
class SendPacketHook {
protected:
    using func_t = void(__fastcall*)(LoopbackPacketSender*, Packet*);
    static inline func_t oFunc;

    static void LoopbackPacketSender_sendToServer(LoopbackPacketSender* _this, Packet* packet) {
        VerifyHook::onPacketSend(packet);
        LocalPlayer* localPlayer = mc.getLocalPlayer();
        if (localPlayer != nullptr) {
            Verify* verifyMod = (Verify*)client->moduleMgr->getModule("Verify");
            if (verifyMod != nullptr) {
                bool dummy = false;
                verifyMod->onSendPacket(packet, dummy);
            }
        }

        if (packet->getName() == "LoginPacket") {
            int numDots = (static_cast<int>(ImGui::GetTime() * 8) % 3) + 1;
            std::string dots = std::string(numDots, '.');
            Notifications::addNotifBox("Connecting to server" + dots, 5.f);
        }

        bool shouldCancel = false;
        if (packet->getId() == PacketID::Text) {
            TextPacket* tPacket = (TextPacket*)packet;
            if (tPacket->messageType == 1) {
                client->commandMgr->onSendTextPacket(tPacket, shouldCancel);
            }
        }

        static NoPacket* noPacketMod = (NoPacket*)client->moduleMgr->getModule("NoPacket");
        if (noPacketMod->isEnabled() && localPlayer != nullptr) return;

        client->moduleMgr->onSendPacket(packet, shouldCancel);
        if (shouldCancel) return;
        oFunc(_this, packet);
    }

public:
    static void init(uintptr_t address) {
        MemoryUtils::CreateHook("SendPacketHook", address, (void*)&SendPacketHook::LoopbackPacketSender_sendToServer, (void*)&oFunc);
    }
};
