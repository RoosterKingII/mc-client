#pragma once
#include "../Module.h"
#include <map>
#include <sstream>
#include <string>

class TotemPopCounter : public Module {
private:
    std::map<std::string, int> popList;
    std::map<std::string, bool> totemEquipped;
    bool sendClientMessage = false;
    bool sendGlobalChat = false;

    void sendChat(std::string text, ...) {
        va_list arg;
        va_start(arg, text);

        char message[300];
        vsprintf_s(message, 300, text.c_str(), arg);
        va_end(arg);

        TextPacket textPacket;
        textPacket.message.setText(std::string(message));
        textPacket.userName.setText(mc.getLocalPlayer()->getNameTag());
        textPacket.xboxUserId = std::to_string(mc.getLocalPlayer()->getUserId());

        mc.getClientInstance()->loopbackPacketSender->sendToServer(&textPacket);

    }

public:
    TotemPopCounter();
    ~TotemPopCounter();

    virtual void onEnable() override;
    virtual void onDisable() override;
    virtual void onNormalTick(Actor* actor) override;
};
